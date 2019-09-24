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
#include "networkutility.h"

using namespace QMTNetwork;

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

void NetworkDownloadRequest::start()
{
    __super::start();

    const QUrl& url = NetworkUtility::currentRequestUrl(m_request);
    if (!url.isValid())
    {
        m_strError = QStringLiteral("Error: Invaild Url -").arg(url.toString());
        LOG_INFO(m_strError.toStdWString());
        emit requestFinished(false, QByteArray(), m_strError);
        return;
    }

    m_pFile = std::move(NetworkUtility::createAndOpenFile(m_request, m_strError));
    if (!m_pFile.get())
    {
        emit requestFinished(false, QByteArray(), m_strError);
        return;
    }

    QNetworkRequest request(url);
    request.setRawHeader("Accept-Encoding", "gzip,deflate,compress,br");
    request.setRawHeader("Connection", "keep-alive");
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

void NetworkDownloadRequest::onReadyRead()
{
    if (m_pNetworkReply
        && m_pNetworkReply->error() == QNetworkReply::NoError
        && m_pNetworkReply->isOpen())
    {
        if (NetworkUtility::fileOpened(m_pFile.get()))
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
    const QUrl& url = NetworkUtility::currentRequestUrl(m_request);
    Q_ASSERT(url.isValid());

    if (isHttpProxy(url.scheme()) || isHttpsProxy(url.scheme()))
    {
        bSuccess = bSuccess && (statusCode >= 200 && statusCode < 300);
    }
    if (!bSuccess)
    {
        if (statusCode == 301 || statusCode == 302)
        {//301,302重定向
            const QVariant& redirectionTarget = m_pNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            const QUrl& redirectUrl = url.resolved(redirectionTarget.toUrl());
            if (redirectUrl.isValid())
            {
                m_request.redirectUrl = redirectUrl.toString();
                if (url != redirectUrl)
                {
                    LOG_INFO("url: " << url.toString().toStdWString() << "; redirectUrl:" << m_request.redirectUrl.toStdWString());
                    qDebug() << "[QMultiThreadNetwork] url:" << url.toString() << "redirectUrl:" << m_request.redirectUrl;

                    m_pNetworkReply->deleteLater();
                    m_pNetworkReply = nullptr;

                    //重定向需要关闭之前打开的文件
                    if (NetworkUtility::fileExists(m_pFile.get()))
                    {
                        m_pFile->close();
                        m_pFile->remove();
                    }
                    m_pFile.reset();

                    start();
                    return;
                }
            }
        }
        else if ((statusCode >= 300 || statusCode < 200) && statusCode != 0)
        {
            LOG_INFO("HttpStatusCode: " << statusCode);
            qDebug() << "[QMultiThreadNetwork] HttpStatusCode:" << statusCode;
        }
    }

    if (NetworkUtility::fileExists(m_pFile.get()))
    {
        m_pFile->close();
        if (!bSuccess)
        {
            m_pFile->remove();
        }
    }
    m_pFile.reset();

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
        int progress = iReceived * 100 / iTotal;
        if (m_nProgress < progress)
        {
            m_nProgress = progress;
            NetworkProgressEvent *event = new NetworkProgressEvent;
            event->uiId = m_request.uiId;
            event->uiBatchId = m_request.uiBatchId;
            event->iBtyes = iReceived;
            event->iTotalBtyes = iTotal;
            QCoreApplication::postEvent(NetworkManager::globalInstance(), event);
        }
    }
}