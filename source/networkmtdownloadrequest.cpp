#include "networkmtdownloadrequest.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QCoreApplication>
#include "Log4cplusWrapper.h"
#include "networkmanager.h"


NetworkMTDownloadRequest::NetworkMTDownloadRequest(QObject *parent /* = nullptr */)
    : NetworkRequest(parent)
    , m_nThreadCount(0)
    , m_nSuccess(0)
    , m_nFailed(0)
    , m_bytesReceived(0)
    , m_bytesTotal(0)
    , m_nFileSize(-1)
{
}

NetworkMTDownloadRequest::~NetworkMTDownloadRequest()
{
}

void NetworkMTDownloadRequest::abort()
{
    __super::abort();
    clearDownloaders();
    clearProgress();
}

bool NetworkMTDownloadRequest::createLocalFile()
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

    m_strDstFilePath = strFilePath;
#ifdef WIN32
    HANDLE hFile = CreateFileW(m_strDstFilePath.toStdWString().c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER li = { 0 };
        li.QuadPart = m_nFileSize;
        if (!SetFilePointerEx(hFile, li, &li, FILE_BEGIN))
        {
            LOG_ERROR("SetFilePointerEx error:" << GetLastError());
            qCritical() << "[QMultiThreadNetwork] SetFilePointerEx error:" << GetLastError();
            return false;
        }
        CloseHandle(hFile);
    }
    else
    {
        LOG_ERROR("CreateFileW error:" << GetLastError());
        qCritical() << "[QMultiThreadNetwork] CreateFileW error:" << GetLastError();
        return false;
    }
#else
    m_strError = QStringLiteral("Error: NetworkMTDownloadRequest only support platform of win32.");
    qWarning() << m_strError;
    LOG_INFO(m_strError.toStdWString());
    return false;
#endif

    return true;
}

//用获取下载文件的长度
bool NetworkMTDownloadRequest::requestFileSize(QUrl url)
{
    if (!url.isValid())
    {
        return false;
    }
    m_nFileSize = -1;
    m_url = url;

    if (nullptr == m_pNetworkManager)
    {
        m_pNetworkManager = new QNetworkAccessManager;
    }
    QNetworkRequest request(url);
    request.setRawHeader("Accept-Encoding", "identity");
    //request.setRawHeader("Accept-Encoding", "gzip");

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

    m_pNetworkReply = m_pNetworkManager->head(request);
    if (m_pNetworkReply)
    {
        connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(onFinished()));
        connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    }
    return true;
}

void NetworkMTDownloadRequest::start()
{
    __super::start();

    m_nSuccess = 0;
    m_nFailed = 0;
    m_nThreadCount = m_request.nDownloadThreadCount;
    if (m_nThreadCount < 1)
    {
        m_nThreadCount = 1;
    }
    if (m_nThreadCount > 10)
    {
        m_nThreadCount = 10;
    }

    bool b = requestFileSize(m_request.url);
    if (!b)
    {
        m_strError = QStringLiteral("Invalid Url").toUtf8();
        emit requestFinished(false, QByteArray(), m_strError);
    }
}

void NetworkMTDownloadRequest::startMTDownload()
{
    if (m_bAbortManual)
    {
        return;
    }

    if (m_nFileSize <= 0)
    {
        m_nThreadCount = 1;
        m_strError = QStringLiteral("MT download 服务器未返回Content-Length");
        LOG_INFO(m_strError.toStdWString());
        qDebug() << "[QMultiThreadNetwork]" << m_strError;

        emit requestFinished(false, QByteArray(), m_strError);
        return;
    }

    if (createLocalFile())
    {
        if (m_bAbortManual)
        {
            return;
        }

        clearDownloaders();

        //将文件分成n段，用异步的方式下载
        for (int i = 0; i < m_nThreadCount; i++)
        {
            qint64 start = 0;
            qint64 end = -1;
            if (m_nThreadCount > 1 && m_nFileSize > 0)
            {
                //先算出每段的开头和结尾（HTTP协议所需要的信息）
                start = m_nFileSize * i / m_nThreadCount;
                end = m_nFileSize * (i + 1) / m_nThreadCount;
                if (i == m_nThreadCount - 1)
                {
                    end--;
                }
            }

            //分段下载该文件
            std::unique_ptr<Downloader> downloader;
#if _MSC_VER >= 1700
            downloader = std::make_unique<Downloader>(i, this);
#else
            downloader.reset(new Downloader(i, this));
#endif
            connect(downloader.get(), SIGNAL(downloadFinished(int, bool, const QString&)),
                this, SLOT(onSubPartFinished(int, bool, const QString&)));
            connect(downloader.get(), SIGNAL(downloadProgress(int, qint64, qint64)),
                this, SLOT(onSubPartDownloadProgress(int, qint64, qint64)));
            if (downloader->startDownload(m_request.url, m_strDstFilePath, m_pNetworkManager,
                start, end, m_request.bShowProgress))
            {
                m_mapDownloader[i] = std::move(downloader);
                m_mapBytes.insert(i, ProgressData());
            }
            else
            {
                abort();
                m_strError = QStringLiteral("Subpart %1 startDownload() failed!").arg(i);
                LOG_ERROR(m_strError.toStdWString());
                emit requestFinished(false, QByteArray(), m_strError);
                return;
            }
        }
    }
    else
    {
        emit requestFinished(false, QByteArray(), m_strError);
    }
}

void NetworkMTDownloadRequest::onSubPartFinished(int index, bool bSuccess, const QString& strErr)
{
    if (m_bAbortManual)
    {
        return;
    }

    if (bSuccess)
    {
        m_nSuccess++;
    }
    else
    {
        m_nFailed++;
        if (m_nFailed == 1)
        {
            abort();
        }
        if (m_strError.isEmpty())
        {
            m_strError = strErr;
        }
    }

    //如果完成数等于文件段数，则说明文件下载成功；失败数大于0，说明下载失败
    if (m_nSuccess == m_nThreadCount || m_nFailed == 1)
    {
        emit requestFinished((m_nFailed == 0), QByteArray(), m_strError);
        LOG_INFO("MT download finished. [result] " << (m_nFailed == 0));
        qDebug() << "[QMultiThreadNetwork] MT download finished. [result]" << (m_nFailed == 0);
    }
}

void NetworkMTDownloadRequest::onSubPartDownloadProgress(int index, qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_bAbortManual || bytesReceived <= 0 || bytesTotal <= 0)
        return;

    if (m_mapBytes.contains(index))
    {
        //qDebug() << "Part:" << index << " progress:" << bytesReceived << "/" << bytesTotal;

        qint64 bytesRevIncreased = 0;//本次接收增加的字节数
        qint64 bytesTotalIncreased = 0;//本次总增加的字节数

        qint64 bytesRev = m_mapBytes.value(index).bytesReceived;
        if (bytesReceived > bytesRev)
        {
            bytesRevIncreased = bytesReceived - bytesRev;
            m_bytesReceived += bytesRevIncreased;
        }
        m_mapBytes[index].bytesReceived = bytesReceived;

        if (m_nThreadCount <= 1 || m_nFileSize <= 0)
        {
            qint64 bytesTo = m_mapBytes.value(index).bytesTotal;
            if (bytesTotal > bytesTo)
            {
                bytesTotalIncreased = bytesTotal - bytesTo;
                m_bytesTotal += bytesTotalIncreased;
            }
            m_mapBytes[index].bytesTotal = bytesTotal;
        }

        if (m_bytesTotal > 0 && m_bytesReceived > 0)
        {
            if (NetworkManager::isInstantiated())
            {
                NetworkProgressEvent *event = new NetworkProgressEvent;
                event->uiId = m_request.uiId;
                event->uiBatchId = m_request.uiBatchId;
                event->iBtyes = m_bytesReceived;
                event->iTotalBtyes = m_bytesTotal;
                QCoreApplication::postEvent(NetworkManager::globalInstance(), event);
            }
        }
    }
}

void NetworkMTDownloadRequest::onFinished()
{
    bool bSuccess = (m_pNetworkReply->error() == QNetworkReply::NoError);
    int statusCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (isHttpProxy(m_url.scheme()) || isHttpsProxy(m_url.scheme()))
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
                const QUrl& redirectUrl = m_url.resolved(redirectionTarget.toUrl());
                if (m_url != redirectUrl && m_redirectUrl != redirectUrl)
                {
                    m_redirectUrl = redirectUrl;
                    if (m_redirectUrl.isValid())
                    {
                        LOG_INFO("url: " << url.toString().toStdWString() << "; redirectUrl:" << m_redirectUrl.toString().toStdWString());
                        qDebug() << "[QMultiThreadNetwork] url:" << m_url.toString() << "redirectUrl:" << redirectUrl.toString();

                        m_pNetworkReply->deleteLater();
                        m_pNetworkReply = nullptr;

                        requestFileSize(redirectUrl);
                        return;
                    }
                }
            }
        }
        else
        {
            m_strError = QStringLiteral("MT download get file size failed! http status code(%1)").arg(statusCode);
            LOG_ERROR(m_strError.toStdWString());
            qDebug() << "[QMultiThreadNetwork]" << m_strError;

            emit requestFinished(false, QByteArray(), m_strError);
            return;
        }
    }
    else
    {
        if (m_request.uiBatchId == 0)
        {
            foreach(const QByteArray& header, m_pNetworkReply->rawHeaderList())
            {
                qDebug() << header << ":" << m_pNetworkReply->rawHeader(header);
            }
        }
        clearProgress();

        QVariant var = m_pNetworkReply->header(QNetworkRequest::ContentLengthHeader);
        m_nFileSize = var.toLongLong();
        m_bytesTotal = m_nFileSize;
        LOG_INFO("MT File size: " << m_nFileSize);
        qDebug() << "[QMultiThreadNetwork] MT File size:" << m_nFileSize;

        m_pNetworkReply->deleteLater();
        m_pNetworkReply = nullptr;

        startMTDownload();
    }
}

bool NetworkMTDownloadRequest::fileAccessible(QFile *pFile) const
{
    return (nullptr != pFile && pFile->exists());
}

bool NetworkMTDownloadRequest::removeFile(QFile *pFile)
{
    if (fileAccessible(pFile))
    {
        pFile->close();
        return pFile->remove();
    }
    return true;
}

void NetworkMTDownloadRequest::clearDownloaders()
{
    for (std::pair<const int, std::unique_ptr<Downloader>>& pair : m_mapDownloader)
    {
        if (pair.second.get())
        {
            pair.second->abort();
        }
    }
    m_mapDownloader.clear();
}

void NetworkMTDownloadRequest::clearProgress()
{
    m_mapBytes.clear();
    m_bytesTotal = 0;
    m_bytesReceived = 0;
}

//////////////////////////////////////////////////////////////////////////
Downloader::Downloader(int index, QObject *parent)
    : QObject(parent)
    , m_nIndex(index)
    , m_pNetworkManager(nullptr)
    , m_pNetworkReply(nullptr)
    , m_bAbortManual(false)
    , m_bShowProgress(false)
    , m_nStartPoint(0)
    , m_nEndPoint(0)
    , m_hFile(0)
{
    TRACE_CLASS_CONSTRUCTOR(Downloader);
}

Downloader::~Downloader()
{
    TRACE_CLASS_DESTRUCTOR(Downloader);
    abort();
}

void Downloader::abort()
{
    if (m_pNetworkReply)
    {
        m_bAbortManual = true;
        m_pNetworkReply->abort();
        m_pNetworkReply->deleteLater();
        m_pNetworkReply = nullptr;
        m_pNetworkManager = nullptr;
        if (m_hFile)
        {
            CloseHandle(m_hFile);
            m_hFile = nullptr;
        }
    }
}

bool Downloader::startDownload(const QUrl &url,
    const QString& strDstFile,
    QNetworkAccessManager* pNetworkManager,
    qint64 startPoint,
    qint64 endPoint,
    bool bShowProgress)
{
    if (nullptr == pNetworkManager || !url.isValid() || strDstFile.isEmpty())
        return false;

    m_bAbortManual = false;

    m_url = url;
    m_pNetworkManager = QPointer<QNetworkAccessManager>(pNetworkManager);
    m_nStartPoint = startPoint;
    m_nEndPoint = endPoint;
    m_bShowProgress = bShowProgress;

    m_strDstFilePath = strDstFile;
#ifdef WIN32
    m_hFile = CreateFileW(strDstFile.toStdWString().c_str(), GENERIC_WRITE,
        FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (m_hFile != nullptr && m_hFile != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER li = { 0 };
        li.HighPart = (long)(startPoint << 32);
        li.LowPart = (DWORD)startPoint;
        if (!SetFilePointerEx(m_hFile, li, nullptr, FILE_BEGIN))
        {
            LOG_ERROR("SetFilePointerEx error:" << GetLastError());
            qCritical() << "[QMultiThreadNetwork] SetFilePointerEx error:" << GetLastError();
            return false;
        }
    }
    else
    {
        LOG_ERROR("CreateFileW error:" << GetLastError());
        qCritical() << "[QMultiThreadNetwork] CreateFileW error:" << GetLastError();
        return false;
    }
#else
    return false;
#endif

    //根据HTTP协议，写入RANGE头部，说明请求文件的范围
    QNetworkRequest request;
    request.setUrl(url);
    QString range;
    range.sprintf("Bytes=%lld-%lld", m_nStartPoint, m_nEndPoint);
    request.setRawHeader("Range", range.toLocal8Bit());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setRawHeader("Accept-Encoding", "gzip");

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

    LOG_INFO("Part " << m_nIndex << " start, Range: " << range.toStdString());
    qDebug() << "[QMultiThreadNetwork] Part" << m_nIndex << "start, Range:" << range;

    m_pNetworkReply = m_pNetworkManager->get(request);
    if (m_pNetworkReply)
    {
        connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(onFinished()));
        connect(m_pNetworkReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
        if (bShowProgress)
        {
            connect(m_pNetworkReply, &QNetworkReply::downloadProgress, this, [=](qint64 bytesReceived, qint64 bytesTotal) {
                if (m_bAbortManual || bytesReceived < 0 || bytesTotal < 0)
                    return;
                emit downloadProgress(m_nIndex, bytesReceived, bytesTotal);
            });
        }
    }
    return true;
}

void Downloader::onReadyRead()
{
#ifdef WIN32
    if (m_pNetworkReply
        && m_pNetworkReply->error() == QNetworkReply::NoError
        && m_pNetworkReply->isOpen())
    {
        if (m_hFile != nullptr)
        {
            const QByteArray& bytesRev = m_pNetworkReply->readAll();
            if (!bytesRev.isEmpty())
            {
                DWORD byteWritten = 0;
                if (!WriteFile(m_hFile, bytesRev.data(), bytesRev.size(), &byteWritten, nullptr))
                {
                    LOG_ERROR("WriteFile error:" << GetLastError());
                    qCritical() << "[QMultiThreadNetwork] WriteFile error:" << GetLastError();
                }
                if (byteWritten != bytesRev.size())
                {
                    LOG_ERROR("mismatched bytes! receive: " << bytesRev.size() << "; write: " << byteWritten);
                    qCritical() << "[QMultiThreadNetwork] mismatched bytes! receive:" << bytesRev.size() << "write:" << byteWritten;
                }
            }
        }
    }
#endif
}

void Downloader::onFinished()
{
    try
    {
        bool bSuccess = (m_pNetworkReply->error() == QNetworkReply::NoError);
        int statusCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (isHttpProxy(m_url.scheme()) || isHttpsProxy(m_url.scheme()))
        {
            bSuccess = bSuccess && (statusCode >= 200 && statusCode < 300);
        }
        if (!bSuccess)
        {
            if (statusCode == 301 || statusCode == 302)
            {//301,302重定向
                const QVariant& redirectionTarget = m_pNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
                if (!redirectionTarget.isNull())
                {//如果网址跳转重新请求
                    const QUrl& redirectUrl = m_url.resolved(redirectionTarget.toUrl());
                    if (redirectUrl.isValid() && redirectUrl != m_url)
                    {
                        LOG_INFO("url: " << m_url.toString().toStdWString() << "; redirectUrl:" << redirectUrl.toString().toStdWString());
                        qDebug() << "[QMultiThreadNetwork] url:" << m_url.toString() << "redirectUrl:" << redirectUrl.toString();

                        m_pNetworkReply->abort();
                        m_pNetworkReply->deleteLater();
                        m_pNetworkReply = nullptr;
#ifdef WIN32
                        if (m_hFile)
                        {
                            CloseHandle(m_hFile);
                            m_hFile = nullptr;
                        }
#endif
                        startDownload(redirectUrl, m_strDstFilePath, m_pNetworkManager.data(),
                            m_nStartPoint, m_nEndPoint, m_bShowProgress);
                        return;
                    }
                }
            }
            else if (statusCode != 200 && statusCode != 0)
            {
                //qDebug() << "HttpStatusCode: " << statusCode;
            }
        }
        else
        {
#ifdef WIN32
            if (m_hFile)
            {
                FlushFileBuffers(m_hFile);
            }
#endif
        }

        LOG_INFO("Part " << m_nIndex << " download " << bSuccess);
        qDebug() << "[QMultiThreadNetwork] Part" << m_nIndex << "download" << bSuccess;

        m_pNetworkReply->deleteLater();
        m_pNetworkReply = nullptr;
#ifdef WIN32
        if (m_hFile)
        {
            CloseHandle(m_hFile);
            m_hFile = nullptr;
        }
#endif

        emit downloadFinished(m_nIndex, bSuccess, m_strError);
    }
    catch (std::exception* e)
    {
        LOG_ERROR("Part" << m_nIndex << " Downloader::onFinished() exception: " << e->what());
        qCritical() << "Part" << m_nIndex << "Downloader::onFinished() exception:" << QString::fromUtf8(e->what());
    }
    catch (...)
    {
        LOG_ERROR("Part" << m_nIndex << " Downloader::onFinished() exception: " << GetLastError());
        qCritical() << "Part" << m_nIndex << "Downloader::onFinishede() exception:" << GetLastError();
    }
}

void Downloader::onError(QNetworkReply::NetworkError code)
{
    LOG_FUN("");
    Q_UNUSED(code);

    m_strError = m_pNetworkReply->errorString();
    LOG_ERROR("[Part " << m_nIndex << "]" << m_strError.toStdString());
    qDebug() << "[QMultiThreadNetwork] Downloader::onError - Part" << m_nIndex << m_strError;
}