/*
@Brief:			Qt multi-threaded network module
@Author:		vilas wang
@Contact:		QQ451930733

The Qt multi-threaded network module is a wrapper of Qt Network module, and combine with thread-pool to realize multi-threaded networking.
- Multi-task concurrent(Each request task is executed in different threads).
- Both single request and batch request mode are supported.
- Large file multi-thread downloading supported. (The thread here refers to the download channel. Download speed is faster.)
- HTTP(S)/FTP protocol supported.
- Multiple request methods supported. (GET/POST/PUT/DELETE/HEAD)
- Asynchronous API.
- Thread-safe.

Note: You must call NetworkManager::initialize() before use, and call NetworkManager::unInitialize() before application quit.
That must be called in the main thread.
*/

#ifndef NETWORKREPLY_H
#define NETWORKREPLY_H

#include <QObject>
#include "networkdefs.h"
#include "networkglobal.h"

class QEvent;
// 对象会自动销毁，不要主动销毁
class NETWORK_EXPORT NetworkReply : public QObject
{
    Q_OBJECT

public:
    NetworkReply(bool bBatch, QObject *parent = 0);
    ~NetworkReply();

    bool isBatchRequest() const { return m_bBatch; }

    virtual bool event(QEvent *) Q_DECL_OVERRIDE;

Q_SIGNALS:
    void requestFinished(const QMTNetwork::RequestTask &);

protected:
    void replyResult(const QMTNetwork::RequestTask& request, bool bDestroy = false);
    friend class NetworkManager;
    friend class NetworkManagerPrivate;

private:
    bool m_bBatch;
};

#endif // NETWORKREPLY_H