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
#include "networkutility.h"

using namespace QMTNetwork;

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
    if (m_nFailed)
    {
    }
}

void NetworkMTDownloadRequest::abort()
{
    __super::abort();
    clearDownloaders();
    clearProgress();
}

bool NetworkMTDownloadRequest::requestFileSize(const QUrl& url)
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
    request.setRawHeader("Accept-Encoding", "gzip,deflate,compress,br");

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
    m_nThreadCount = 1;

    if (!requestFileSize(m_request.url))
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
        m_strError = QStringLiteral("MT download 服务器未返回Content-Length");
        LOG_INFO(m_strError.toStdWString());
        qDebug() << "[QMultiThreadNetwork]" << m_strError;

        emit requestFinished(false, QByteArray(), m_strError);
        return;
    }

    m_strDstFilePath = NetworkUtility::createSharedRWFileWin32(m_request, m_strError);
    if (m_strDstFilePath.isEmpty())
    {
        emit requestFinished(false, QByteArray(), m_strError);
        return;
    }

    if (m_bAbortManual)
    {
        return;
    }

    clearDownloaders();
    m_nThreadCount = m_request.nDownloadThreadCount;
    if (m_nThreadCount < 1)
    {
        m_nThreadCount = 1;
    }
    if (m_nThreadCount > 10)
    {
        m_nThreadCount = 10;
    }

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
        if (downloader->start(m_request.url, m_strDstFilePath, m_pNetworkManager,
            start, end, m_request.bShowProgress))
        {
            m_mapDownloader[i] = std::move(downloader);
            m_mapBytes.insert(i, ProgressData());
        }
        else
        {
            abort();
            m_strError = QStringLiteral("Subpart %1 download failed!").arg(i);
            LOG_ERROR(m_strError.toStdWString());
            emit requestFinished(false, QByteArray(), m_strError);
            return;
        }
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
        if (++m_nFailed == 1)
        {
            abort();
        }
        if (m_strError.isEmpty())
        {
            m_strError = strErr;
        }
    }

    //如果完成数等于文件段数，则说明文件下载成功；失败数大于0，说明下载失败
    if (m_nSuccess == m_nThreadCount || m_nFailed > 0)
    {
        emit requestFinished((m_nFailed == 0), QByteArray(), m_strError);
        if (m_nFailed == 0)
        {
            LOG_INFO("MT download success.");
            qDebug() << "[QMultiThreadNetwork] MT download success.";
        }
    }
}

void NetworkMTDownloadRequest::onSubPartDownloadProgress(int index, qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_bAbortManual || bytesReceived <= 0 || bytesTotal <= 0)
        return;

    if (NetworkManager::isInstantiated() && m_mapBytes.contains(index))
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
            int progress = m_bytesReceived * 100 / m_bytesTotal;
            if (m_nProgress < progress)
            {
                m_nProgress = progress;
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
            const QUrl& redirectUrl = m_url.resolved(redirectionTarget.toUrl());
            if (redirectUrl.isValid() && m_url != redirectUrl)
            {
                LOG_INFO("url: " << url.toString().toStdWString() << "; redirectUrl:" << m_redirectUrl.toString().toStdWString());
                qDebug() << "[QMultiThreadNetwork] url:" << m_url.toString() << "redirectUrl:" << redirectUrl.toString();

                m_pNetworkReply->deleteLater();
                m_pNetworkReply = nullptr;

                requestFileSize(redirectUrl);
                return;
            }
        }

        m_strError = QStringLiteral("MT download get file size failed! http status code(%1)").arg(statusCode);
        LOG_ERROR(m_strError.toStdWString());
        qDebug() << "[QMultiThreadNetwork]" << m_strError;

        emit requestFinished(false, QByteArray(), m_strError);
        return;
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

        const QVariant& var = m_pNetworkReply->header(QNetworkRequest::ContentLengthHeader);
        m_nFileSize = var.toLongLong();
        m_bytesTotal = m_nFileSize;
        LOG_INFO("File size: " << m_nFileSize);
        qDebug() << "[QMultiThreadNetwork] File size:" << m_nFileSize;

        m_pNetworkReply->deleteLater();
        m_pNetworkReply = nullptr;

        startMTDownload();
    }
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
    m_bAbortManual = true;
    if (m_pNetworkReply)
    {
        if (m_pNetworkReply->isRunning())
        {
            m_pNetworkReply->abort();
        }
        m_pNetworkReply->deleteLater();
        m_pNetworkReply = nullptr;
    }
    if (m_hFile)
    {
        CloseHandle(m_hFile);
        m_hFile = nullptr;
    }
    m_pNetworkManager = nullptr;
}

bool Downloader::start(const QUrl &url,
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
    request.setRawHeader("Accept-Encoding", "gzip,deflate,compress,br");
    request.setRawHeader("Connection", "keep-alive");

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

    LOG_INFO("Part " << m_nIndex << " Range: " << range.toStdString());
    qDebug() << "[QMultiThreadNetwork] Part" << m_nIndex << "Range:" << range;

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
                    LOG_ERROR("incompatible bytes! receive: " << bytesRev.size() << "; write: " << byteWritten);
                    qCritical() << "[QMultiThreadNetwork] incompatible bytes! receive:" << bytesRev.size() << "write:" << byteWritten;
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
                const QUrl& redirectUrl = m_url.resolved(redirectionTarget.toUrl());
                if (redirectUrl.isValid() && redirectUrl != m_url)
                {
                    LOG_INFO("url: " << m_url.toString().toStdWString() << "; redirectUrl:" << redirectUrl.toString().toStdWString());
                    qDebug() << "[QMultiThreadNetwork] url:" << m_url.toString() << "redirectUrl:" << redirectUrl.toString();

                    m_pNetworkReply->deleteLater();
                    m_pNetworkReply = nullptr;
#ifdef WIN32
                    if (m_hFile)
                    {
                        CloseHandle(m_hFile);
                        m_hFile = nullptr;
                    }
#endif
                    start(redirectUrl, m_strDstFilePath, m_pNetworkManager.data(),
                        m_nStartPoint, m_nEndPoint, m_bShowProgress);
                    return;
                }
            }

            if (statusCode != 200 && statusCode != 0)
            {
                qDebug() << "[QMultiThreadNetwork] HttpStatusCode: " << statusCode;
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

        if (!bSuccess)
        {
            LOG_INFO("Part " << m_nIndex << " download failed!");
            qDebug() << "[QMultiThreadNetwork] Part" << m_nIndex << "download failed!";
        }

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