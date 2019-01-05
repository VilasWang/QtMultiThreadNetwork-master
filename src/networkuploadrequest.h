#ifndef NETWORKUPLOADREQUEST_H
#define NETWORKUPLOADREQUEST_H

#include <QObject>
#include <QNetworkReply>
#include "NetworkRequest.h"

class QFile;
class QNetworkAccessManager;

//上传请求
class NetworkUploadRequest : public NetworkRequest
{
	Q_OBJECT;

public:
	explicit NetworkUploadRequest(QObject *parent = 0);
	~NetworkUploadRequest();

public Q_SLOTS:
	void start() Q_DECL_OVERRIDE;
	void abort() Q_DECL_OVERRIDE;

	void onFinished();
	void onUploadProgress(qint64, qint64);
	void onError(QNetworkReply::NetworkError);

private:
	//读取本地文件的内容
	bool readLocalFile(const QString& strFilePath, QByteArray& bytes);

private:
	QNetworkAccessManager *m_pNetworkManager;
	QNetworkReply *m_pNetworkReply;
};

#endif // NETWORKUPLOADREQUEST_H