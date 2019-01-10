#ifndef NETWORKREQUESTTHREAD_H
#define NETWORKREQUESTTHREAD_H

#include <QObject>
#include <QRunnable>
#include "NetworkDef.h"

class NetworkRequestThread: public QObject, public QRunnable
{
	Q_OBJECT;

public:
	explicit NetworkRequestThread(const RequestTask &, QObject *parent = 0);
	~NetworkRequestThread();

	//继承抽象类QRunnable，执行该函数会启动一个新的线程
	//执行QThreadPool::start(QRunnable) 或者 QThreadPool::tryStart(QRunnable)之后会自动调用
	//所有完成的功能在该函数中执行
	virtual void run() Q_DECL_OVERRIDE;

	quint64 requsetId() const;
	quint64 batchId() const;
	const RequestTask task() {return m_task;}

	//结束事件循环以结束线程,会自动结束正在执行的请求
	void quit();

Q_SIGNALS:
	void requestFinished(const RequestTask &);
	void exitEventLoop();

private Q_SLOTS:
	void onRequestFinished(bool, const QByteArray&);

private:
	RequestTask m_task;
};

#endif //CHQHTTPREQUESTTHREAD_H