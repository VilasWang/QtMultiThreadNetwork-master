#ifndef NETWORKBIGFLEDOWNLOADREQUEST_H
#define NETWORKBIGFLEDOWNLOADREQUEST_H

#include <QObject>
#include <QPointer>
#include <QMutex>
#include "networkrequest.h"

class QFile;
//用于下载文件（或文件的一部分）
class Downloader : public QObject
{
    Q_OBJECT

public:
    Downloader(int index, QObject *parent = 0);
    ~Downloader();

    bool startDownload(const QUrl &url,
        const QString& strDstFile,
        QNetworkAccessManager* pNetworkManager,
        qint64 startPoint = 0,
        qint64 endPoint = -1,
        bool bShowProgress = false);

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
    HANDLE m_hFile;
    QString m_strDstFilePath;
    bool m_bAbortManual;
    QString m_strError;

    const int m_nIndex;
    qint64 m_nStartPoint;
    qint64 m_nEndPoint;
    bool m_bShowProgress;
};

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
    bool requestFileSize(QUrl url);
    //根据文件名创建本地文件
    bool createLocalFile();
    bool fileAccessible(QFile *pFile) const;
    bool removeFile(QFile *file);
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
        ProgressData()
        {
            bytesReceived = 0;
            bytesTotal = 0;
        }
    };

    QMap<int, ProgressData> m_mapBytes;
    qint64 m_bytesTotal;
    qint64 m_bytesReceived;
};

#endif // NETWORKBIGFLEDOWNLOADREQUEST_H
