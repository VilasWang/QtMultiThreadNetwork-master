#ifndef NETWORKCOMMONREQUEST_H
#define NETWORKCOMMONREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "NetworkRequest.h"

class QNetworkAccessManager;

//“ª∞„«Î«Û
class NetworkCommonRequest : public NetworkRequest
{
	Q_OBJECT;

public:
	explicit NetworkCommonRequest(QObject *parent = 0);
	~NetworkCommonRequest();

public Q_SLOTS:
	void start() Q_DECL_OVERRIDE;
	void abort() Q_DECL_OVERRIDE;

	void onFinished();
	void onError(QNetworkReply::NetworkError);
	void onAuthenticationRequired(QNetworkReply *, QAuthenticator *);

private:
	QString getRequestTypeString() const;

private:
	QNetworkAccessManager *m_pNetworkManager;
	QNetworkReply *m_pNetworkReply;
};

#endif // NETWORKCOMMONREQUEST_H