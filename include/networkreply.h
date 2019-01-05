#ifndef NETWORKREPLY_H
#define NETWORKREPLY_H

#include <QObject>
#include "NetworkDef.h"
#include "Network_Global.h"

// 对象会自动销毁，不要主动销毁
class NETWORK_EXPORT NetworkReply : public QObject
{
	Q_OBJECT;
	friend class NetworkManager;

public:
	explicit NetworkReply(bool bBatch, QObject *parent = 0);
	~NetworkReply();

	bool isBatchRequest(){return m_bBatch;}

	virtual bool event(QEvent *);

Q_SIGNALS:
	void requestFinished(const RequestTask &);

private:
	void replyResult(const RequestTask& request, bool bDestroy = false);
	bool m_bBatch;
};

#endif // NETWORKREPLY_H