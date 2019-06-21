#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QObject>
#include <memory>
#include <QNetworkReply>
#include "networkdef.h"
#include "classmemorytracer.h"


class QNetworkAccessManager;
class NetworkRequest : public QObject
{
    Q_OBJECT

public:
    explicit NetworkRequest(QObject *parent = 0);
    virtual ~NetworkRequest();

    void setRequestTask(const RequestTask &request) { m_request = request; }
    //是否重定向
    bool redirected() const { return (m_redirectUrl.isValid() && m_redirectUrl != m_request.url); }

    QString error() const { return m_strError; }

public Q_SLOTS:
    virtual void start();
    virtual void abort();
    virtual void onFinished() = 0;
    virtual void onError(QNetworkReply::NetworkError);
    virtual void onAuthenticationRequired(QNetworkReply *, QAuthenticator *);

Q_SIGNALS:
    void requestFinished(bool, const QByteArray&);
    void aboutToAbort();

protected:
    RequestTask m_request;
    bool m_bAbortManual;
    QUrl m_redirectUrl;
    QString m_strError;
    QNetworkAccessManager *m_pNetworkManager;
    QNetworkReply *m_pNetworkReply;
};

//工厂类
class NetworkRequestFactory
{
public:
    ///根据类型创建request对象
    static std::unique_ptr<NetworkRequest> create(const RequestType& type);
};

inline bool isHttpProxy(const QString& strScheme) { return (strScheme.compare(QLatin1String("http"), Qt::CaseInsensitive) == 0); }
inline bool isHttpsProxy(const QString& strScheme) { return (strScheme.compare(QLatin1String("https"), Qt::CaseInsensitive) == 0); }
inline bool isFtpProxy(const QString& strScheme) { return (strScheme.compare(QLatin1String("ftp"), Qt::CaseInsensitive) == 0); }

#endif // NETWORKREQUEST_H
