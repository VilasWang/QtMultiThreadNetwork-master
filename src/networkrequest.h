#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QObject>
#include "NetworkDef.h"

/*//////////////////////////////////////////////////////////////////////////
	Example:

	RequestTask request;
	...
	NetworkRequest *pRequest = NetworkRequestFactory::createRequestInstance(request.eType);
	if (nullptr != pRequest)
	{
		connect(pRequest, SIGNAL(requestFinished(bool, const QByteArray&)),
		this, SLOT(onRequestFinished(bool, const QByteArray&)));

		pRequest->setRequestTask(m_task);
		pRequest->start();
	}
/////////////////////////////////////////////////////////////////////////*/

class NetworkRequest : public QObject
{
	Q_OBJECT

public:
	explicit NetworkRequest(QObject *parent = 0) : QObject(parent), m_bAbortManual(false){}
	virtual ~NetworkRequest(){}

	void setRequestTask(const RequestTask &request){m_request = request;}
	//是否重定向
	bool redirected() const{return (m_redirectUrl.isValid() && m_redirectUrl != m_request.url);}

public Q_SLOTS:
	virtual void start() = 0;
	virtual void abort() = 0;

Q_SIGNALS:
	void requestFinished(bool, const QByteArray&);
	void aboutToAbort();

protected:
	RequestTask m_request;
	bool m_bAbortManual;
	QUrl m_redirectUrl;
	QString m_strError;
};

class NetworkRequestFactory
{
public:
	///根据task类型创建request对象。若parent不为null，request对象由parent管理；否则，需要用户自己去销毁
	static NetworkRequest *createRequestInstance(const RequestType& type, bool bBigFileMode = false, QObject *parent = nullptr);
};

inline bool isHttpProxy(const QString& strScheme) {return (strScheme.compare(QLatin1String("http"), Qt::CaseInsensitive) == 0);}
inline bool isHttpsProxy(const QString& strScheme) {return (strScheme.compare(QLatin1String("https"), Qt::CaseInsensitive) == 0);}
inline bool isFtpProxy(const QString& strScheme) {return (strScheme.compare(QLatin1String("ftp"), Qt::CaseInsensitive) == 0);}

#endif // NETWORKREQUEST_H
