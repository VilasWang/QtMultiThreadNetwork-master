#ifndef NETWORKBIGFLEDOWNLOADREQUEST_H
#define NETWORKBIGFLEDOWNLOADREQUEST_H

#include <QObject>
#include <QPointer>
#include <QMutex>
#include "networkrequest.h"

class QFile;
class Downloader;

//多线程下载请求(这里的线程是指下载的通道。一个文件被分成多个部分，由多个下载通道同时下载)
class NetworkMTDownloadRequest : public NetworkRequest
{
    Q_OBJECT;

public:
    explicit NetworkMTDownloadRequest(QObject *parent = 0);
    ~NetworkMTDownloadRequest();

public Q_SLOTS:
    void start() Q_DECL_OVERRIDE;
    void abort() Q_DECL_OVERRIDE;
    void onFinished() Q_DECL_OVERRIDE;
    void onSubPartFinished(int index, bool bSuccess, const QString& strErr);
    void onSubPartDownloadProgress(int index, qint64 bytesReceived, qint64 bytesTotal);

private:
    bool requestFileSize(const QUrl& url);
    void startMTDownload();
    void clearDownloaders();
    void clearProgress();

private:
    QUrl m_url;
    QString m_strDstFilePath;
    qint64 m_nFileSize;

    std::map<int, std::unique_ptr<Downloader>> m_mapDownloader;
    int m_nThreadCount;//分割成多少段下载
    int m_nSuccess;
    int m_nFailed;

    struct ProgressData
    {
        qint64 bytesReceived;
        qint64 bytesTotal;
        ProgressData() : bytesReceived(0), bytesTotal(0) {}
    };

    QMap<int, ProgressData> m_mapBytes;
    qint64 m_bytesTotal;
    qint64 m_bytesReceived;
};

//用于下载文件（或文件的一部分）
class Downloader : public QObject
{
    Q_OBJECT

public:
    explicit Downloader(int index, 
        const QString& strDstFile, 
        QNetworkAccessManager* pNetworkManager, 
        bool bShowProgress = false, 
        quint16 nMaxRedirectionCount = 5,
        QObject *parent = 0);

    virtual ~Downloader();

    bool start(const QUrl &url, qint64 startPoint = 0, qint64 endPoint = -1);

    void abort();

Q_SIGNALS:
    void downloadFinished(int index, bool bSuccess, const QString& strErr);
    void downloadProgress(int index, qint64 bytesReceived, qint64 bytesTotal);

public Q_SLOTS:
    void onFinished();
    void onReadyRead();
    void onError(QNetworkReply::NetworkError code);

private:
    QPointer<QNetworkAccessManager> m_pNetworkManager;
    QNetworkReply *m_pNetworkReply;
    QUrl m_url;
    bool m_bAbortManual;
    QString m_strError;
    const int m_nIndex;
    qint64 m_nStartPoint;
    qint64 m_nEndPoint;
    bool m_bShowProgress;
    quint16 m_nRedirectionCount;
    quint16 m_nMaxRedirectionCount;

    typedef void * HANDLE;
    HANDLE m_hFile;
    QString m_strDstFilePath;
};

#endif // NETWORKBIGFLEDOWNLOADREQUEST_H
