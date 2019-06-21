#ifndef NETWORKCOMMONREQUEST_H
#define NETWORKCOMMONREQUEST_H

#include <QObject>
#include "networkrequest.h"


//一般请求
class NetworkCommonRequest : public NetworkRequest
{
    Q_OBJECT;

public:
    explicit NetworkCommonRequest(QObject *parent = 0);
    ~NetworkCommonRequest();

public Q_SLOTS:
    void start() Q_DECL_OVERRIDE;
    void onFinished() Q_DECL_OVERRIDE;
};

#endif // NETWORKCOMMONREQUEST_H