#include "networkrunnable.h"
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>
#include "classmemorytracer.h"
#include "networkrequest.h"
#include "networkmanager.h"

using namespace QMTNetwork;

NetworkRunnable::NetworkRunnable(const RequestTask &task, QObject *parent)
    : QObject(parent)
    , m_task(task)
{
    TRACE_CLASS_CONSTRUCTOR(NetworkRunnable);
    setAutoDelete(false);
}

NetworkRunnable::~NetworkRunnable()
{
    TRACE_CLASS_DESTRUCTOR(NetworkRunnable);
}

void NetworkRunnable::run()
{
    std::unique_ptr<NetworkRequest> pRequest = nullptr;
    QEventLoop loop;

    try
    {
        connect(this, &NetworkRunnable::exitEventLoop, &loop, [&loop]() {
            loop.quit();
        }, Qt::QueuedConnection);

        pRequest = std::move(NetworkRequestFactory::create(m_task.eType));
        if (pRequest.get())
        {
            m_connect = connect(pRequest.get(), &NetworkRequest::requestFinished, this,
                [=](bool bSuccess, const QByteArray& bytesContent, const QString& strError) {
                m_task.bSuccess = bSuccess;
                m_task.bytesContent = bytesContent;
                m_task.strError = strError;
                emit requestFinished(m_task);
            });
            pRequest->setRequestTask(m_task);
            pRequest->start();
        }
        else
        {
            qWarning() << QString("[QMultiThreadNetwork] Unsupported type(%1) ----").arg((int)m_task.eType) << m_task.url;

            m_task.bSuccess = false;
            m_task.strError = QString("[QMultiThreadNetwork] Unsupported type(%1)").arg((int)m_task.eType);
            emit requestFinished(m_task);
        }
        loop.exec();
    }
    catch (std::exception* e)
    {
        qCritical() << "[QMultiThreadNetwork] NetworkRunnable::run() exception:" << QString::fromUtf8(e->what());
    }
    catch (...)
    {
        qCritical() << "[QMultiThreadNetwork] NetworkRunnable::run() unknown exception";
    }

    if (pRequest.get())
    {
        pRequest->abort();
        pRequest.reset();
    }
}

quint64 NetworkRunnable::requsetId() const
{
    return m_task.uiId;
}

quint64 NetworkRunnable::batchId() const
{
    return m_task.uiBatchId;
}

void NetworkRunnable::quit()
{
    this->disconnect(m_connect);
    emit exitEventLoop();
}