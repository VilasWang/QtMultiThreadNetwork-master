#ifndef NETWORKDOWNLOADREQUEST_H
#define NETWORKDOWNLOADREQUEST_H

#include <QObject>
#include "networkrequest.h"

class QFile;

//下载请求
class NetworkDownloadRequest : public NetworkRequest
{
    Q_OBJECT;

public:
    explicit NetworkDownloadRequest(QObject *parent = 0);
    ~NetworkDownloadRequest();

public Q_SLOTS:
    void start() Q_DECL_OVERRIDE;
    void onFinished() Q_DECL_OVERRIDE;
    void onReadyRead();
    void onDownloadProgress(qint64 iReceived, qint64 iTotal);

private:
    std::unique_ptr<QFile> m_pFile;
};

#endif // NETWORKDOWNLOADREQUEST_H