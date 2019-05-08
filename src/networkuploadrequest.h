#ifndef NETWORKUPLOADREQUEST_H
#define NETWORKUPLOADREQUEST_H

#include <QObject>
#include "networkrequest.h"

class QFile;

//上传请求
class NetworkUploadRequest : public NetworkRequest
{
    Q_OBJECT;

public:
    explicit NetworkUploadRequest(QObject *parent = 0);
    ~NetworkUploadRequest();

public Q_SLOTS:
    void start() Q_DECL_OVERRIDE;
    void onFinished() Q_DECL_OVERRIDE;
    void onUploadProgress(qint64, qint64);

private:
    //读取本地文件的内容
    bool readLocalFile(const QString& strFilePath, QByteArray& bytes);
};

#endif // NETWORKUPLOADREQUEST_H