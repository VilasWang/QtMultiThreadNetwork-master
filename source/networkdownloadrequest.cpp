#include "networkdownloadrequest.h"
#include <memory>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QCoreApplication>
#include "Log4cplusWrapper.h"
#include "networkmanager.h"


NetworkDownloadRequest::NetworkDownloadRequest(QObject *parent /* = nullptr */)
    : NetworkRequest(parent)
    , m_pFile(nullptr)
{
}

NetworkDownloadRequest::~NetworkDownloadRequest()
{
    if (m_pFile.get())
    {
        m_pFile->close();
        m_pFile.reset();
    }
}

bool NetworkDownloadRequest::createLocalFile()
{
    m_strError.clear();
    //取下载文件保存目录
    QString strSaveDir = QDir::toNativeSeparators(m_request.strReqArg);
    if (!strSaveDir.isEmpty())
    {
        QDir dir;
        if (!dir.exists(strSaveDir))
        {
            if (!dir.mkpath(strSaveDir))
            {
                m_strError = QStringLiteral("Error: QDir::mkpath failed! Dir(%1)").arg(strSaveDir);
                qWarning() << m_strError;
                LOG_INFO(m_strError.toStdWString());
                return false;
            }
        }
    }
    else
    {
        m_strError = QLatin1String("Error: RequestTask::strRequestArg is empty!");
        qWarning() << m_strError;
        LOG_INFO(m_strError.toStdWString());
        return false;
    }
    if (!strSaveDir.endsWith("\\"))
    {
        strSaveDir.append("\\");
    }

    //取下载保存的文件名
    QString strFileName;
    if (!m_request.strSaveFileName.isEmpty())
    {
        strFileName = m_request.strSaveFileName;
    }
    else
    {
        QUrl url;
        if (redirected())
        {
            url = m_redirectUrl;
        }
        else
        {
            url = m_request.url;
        }
        // url中提取文件名，格式如：response-content-disposition=attachment; filename=test.exe
        QUrlQuery query(url.query(QUrl::FullyDecoded));
        const QList<QPair<QString, QString>>& querys = query.queryItems();
        foreach(auto pair, querys)
        {
            if (pair.first.compare("response-content-disposition", Qt::CaseInsensitive) == 0
                || pair.first.compare("content-disposition", Qt::CaseInsensitive) == 0)
            {
                const QStringList& listString = pair.second.split(";", QString::SkipEmptyParts);
                foreach(QString str, listString)
                {
                    str = str.trimmed();
                    if (str.startsWith(QString("filename="), Qt::CaseInsensitive))
                    {
                        strFileName = str.right(str.size() - QString("filename=").size());
                        break;
                    }
                }
                break;
            }
        }
        if (strFileName.isEmpty())
        {
            strFileName = url.fileName();
        }
    }

    if (strFileName.isEmpty())
    {
        m_strError = QLatin1String("Error: fileName is empty!");
        qWarning() << m_strError;
        LOG_INFO(m_strError.toStdWString());
        return false;
    }

    //重定向等操作后需要关闭打开的文件
    if (m_pFile.get())
    {
        if (m_pFile->exists())
        {
            m_pFile->close();
            m_pFile->remove();
        }
        m_pFile.reset();
    }

    //如果文件存在，关闭文件并移除
    const QString& strFilePath = QDir::toNativeSeparators(strSaveDir + strFileName);
    if (QFile::exists(strFilePath))
    {
        if (m_request.bRemoveFileWhileExist)
        {
            QFile file(strFilePath);
            if (!removeFile(&file))
            {
                m_strError = QStringLiteral("Error: QFile::remove(%1) - %2").arg(strFilePath).arg(file.errorString());
                qWarning() << m_strError;
                LOG_INFO(m_strError.toStdWString());
                return false;
            }
        }
        else
        {
            m_strError = QStringLiteral("Error: File is already exist(%1)").arg(strFilePath);
            qWarning() << m_strError;
            LOG_INFO(m_strError.toStdWString());
            return false;
        }
    }

    //创建并打开文件
#if _MSC_VER >= 1700
    m_pFile = std::make_unique<QFile>(strFilePath);
#else
    m_pFile->reset(new QFile(strFilePath));
#endif
    if (!m_pFile->open(QIODevice::WriteOnly))
    {
        m_strError = QStringLiteral("Error: QFile::open(%1) - %2").arg(strFilePath).arg(m_pFile->errorString());
        qWarning() << m_strError;
        LOG_INFO(m_strError.toStdWString());
        m_pFile.reset();
        return false;
    }
    return true;
}

void NetworkDownloadRequest::start()
{
    __super::start();

    if (createLocalFile())
    {
        QUrl url;
        if (!redirected())
        {
            url = m_request.url;
        }
        else
        {
            url = m_redirectUrl;
        }
        QNetworkRequest request(url);
        //request.setRawHeader("Accept-Charset", "utf-8");
        //request.setRawHeader("Accept-Language", "zh-CN");
        auto iter = m_request.mapRawHeader.cbegin();
        for (; iter != m_request.mapRawHeader.cend(); ++iter)
        {
            request.setRawHeader(iter.key(), iter.value());
        }

#ifndef QT_NO_SSL
        if (isHttpsProxy(url.scheme()))
        {
            // 发送https请求前准备工作;
            QSslConfiguration conf = request.sslConfiguration();
            conf.setPeerVerifyMode(QSslSocket::VerifyNone);
            conf.setProtocol(QSsl::TlsV1SslV3);
            request.setSslConfiguration(conf);
        }
#endif

        if (nullptr == m_pNetworkManager)
        {
            m_pNetworkManager = new QNetworkAccessManager;
        }
        m_pNetworkReply = m_pNetworkManager->get(request);

        connect(m_pNetworkReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(onFinished()));
        connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
        if (m_request.bShowProgress)
        {
            connect(m_pNetworkReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
        }
    }
    else
    {
        emit requestFinished(false, QByteArray(), m_strError);
    }
}

bool NetworkDownloadRequest::fileAccessible(QFile *pFile) const
{
    return (nullptr != pFile && pFile->exists());
}

bool NetworkDownloadRequest::removeFile(QFile *pFile)
{
    if (fileAccessible(pFile))
    {
        pFile->close();
        return pFile->remove();
    }
    return true;
}

void NetworkDownloadRequest::onReadyRead()
{
    if (m_pNetworkReply
        && m_pNetworkReply->error() == QNetworkReply::NoError
        && m_pNetworkReply->isOpen())
    {
        if (fileAccessible(m_pFile.get()) && m_pFile->isOpen())
        {
            const QByteArray& bytesRev = m_pNetworkReply->readAll();
            if (!bytesRev.isEmpty() && -1 == m_pFile->write(bytesRev))
            {
                LOG_ERROR(m_pFile->errorString().toStdWString());
                qDebug() << "[QMultiThreadNetwork]" << m_pFile->errorString();
            }
        }
    }
}

void NetworkDownloadRequest::onFinished()
{
    bool bSuccess = (m_pNetworkReply->error() == QNetworkReply::NoError);
    int statusCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (isHttpProxy(m_request.url.scheme()) || isHttpsProxy(m_request.url.scheme()))
    {
        bSuccess = bSuccess && (statusCode >= 200 && statusCode < 300);
    }
    if (!bSuccess)
    {
        if (statusCode == 301 || statusCode == 302)
        {//301,302重定向
            const QVariant& redirectionTarget = m_pNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if (!redirectionTarget.isNull())
            {
                const QUrl& url = m_request.url;
                const QUrl& redirectUrl = url.resolved(redirectionTarget.toUrl());
                if (url != redirectUrl && m_redirectUrl != redirectUrl)
                {
                    m_redirectUrl = redirectUrl;
                    if (m_redirectUrl.isValid())
                    {
                        LOG_INFO("url: " << url.toString().toStdWString() << "; redirectUrl:" << m_redirectUrl.toString().toStdWString());
                        qDebug() << "[QMultiThreadNetwork] url:" << url.toString() << "redirectUrl:" << m_redirectUrl.toString();

                        m_pNetworkReply->deleteLater();
                        m_pNetworkReply = nullptr;

                        start();
                        return;
                    }
                }
            }
        }
        else if ((statusCode >= 300 || statusCode < 200) && statusCode != 0)
        {
            LOG_INFO("HttpStatusCode: " << statusCode);
            qDebug() << "[QMultiThreadNetwork] HttpStatusCode:" << statusCode;
        }
    }

    if (fileAccessible(m_pFile.get()))
    {
        m_pFile->close();
        if (!bSuccess)
        {
            m_pFile->remove();
        }
    }

    if (!m_bAbortManual)//非调用abort()结束
    {
        if (m_pNetworkReply->isOpen())
        {
            if (!bSuccess)
            {
                m_strError.append(QString::fromUtf8(m_pNetworkReply->readAll()));
            }
        }
    }
    emit requestFinished(bSuccess, QByteArray(), m_strError);

    m_pNetworkReply->deleteLater();
    m_pNetworkReply = nullptr;
}

void NetworkDownloadRequest::onDownloadProgress(qint64 iReceived, qint64 iTotal)
{
    if (m_bAbortManual || iReceived <= 0 || iTotal <= 0)
        return;

    if (NetworkManager::isInstantiated())
    {
        NetworkProgressEvent *event = new NetworkProgressEvent;
        event->uiId = m_request.uiId;
        event->uiBatchId = m_request.uiBatchId;
        event->iBtyes = iReceived;
        event->iTotalBtyes = iTotal;
        QCoreApplication::postEvent(NetworkManager::globalInstance(), event);
    }
}