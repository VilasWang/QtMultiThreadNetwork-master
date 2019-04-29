#ifndef NETWORKRUNNABLE_H
#define NETWORKRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include "NetworkDef.h"

class NetworkRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	explicit NetworkRunnable(const RequestTask &, QObject *parent = 0);
	~NetworkRunnable();

	//执行QThreadPool::start(QRunnable) 或者 QThreadPool::tryStart(QRunnable)之后会自动调用
	virtual void run() Q_DECL_OVERRIDE;

	quint64 requsetId() const;
	quint64 batchId() const;
	const RequestTask task() const { return m_task; }

	//结束事件循环以释放任务线程，使其变成空闲状态,并且会自动结束正在执行的请求
	void quit();

Q_SIGNALS:
	void requestFinished(const RequestTask &);
	void exitEventLoop();
	void aboutToDelete();

private Q_SLOTS:
	void onRequestFinished(bool, const QByteArray&);

private:
	Q_DISABLE_COPY(NetworkRunnable);
	RequestTask m_task;
};

#endif //NETWORKRUNNABLE_H