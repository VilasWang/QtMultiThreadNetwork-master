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
    RequestTask task = m_task;
    std::unique_ptr<NetworkRequest> pRequest = nullptr;

    bool bQuit = false;
    QEventLoop loop;

    try
    {
        connect(this, &NetworkRunnable::exitEventLoop, &loop, [&bQuit, &loop, &pRequest]() {
            loop.quit();
            bQuit = true;
            if (pRequest.get())
            {
                pRequest->disconnect();
            }
        }, Qt::QueuedConnection);

        if (!bQuit)
        {
            pRequest = std::move(NetworkRequestFactory::create(task.eType));

            if (pRequest.get())
            {
                connect(pRequest.get(), &NetworkRequest::requestFinished,
                    [this, &task](bool bSuccess, const QByteArray& bytesContent, const QString& strError) {
                    task.bSuccess = bSuccess;
                    task.bytesContent = bytesContent;
                    task.strError = strError;
                    emit requestFinished(task);
                });
                pRequest->setRequestTask(task);
                pRequest->start();
            }
            else
            {
                qWarning() << QString("[QMultiThreadNetwork] Unsupported type(%1) ----").arg(task.eType) << task.url;

                task.bSuccess = false;
                task.strError = QString("[QMultiThreadNetwork] Unsupported type(%1)").arg(task.eType);
                emit requestFinished(task);
            }
            loop.exec();
        }
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
    disconnect(this, &NetworkRunnable::requestFinished,
        NetworkManager::globalInstance(), &NetworkManager::onRequestFinished);
    emit exitEventLoop();
}