#include "networkmanager.h"
#include <atomic>
#include <QMutex>
#include <QMutexLocker>
#include <QMap>
#include <QQueue>
#include <QThread>
#include <QThreadPool>
#include <QEvent>
#include <QDebug>
#include <QCoreApplication>
#include "Log4cplusWrapper.h"
#include "classmemorytracer.h"
#include "networkrunnable.h"


#define DEFAULT_MAX_THREAD_COUNT 5

class NetworkManagerPrivate
{
    Q_DECLARE_PUBLIC(NetworkManager)

public:
    NetworkManagerPrivate();
    ~NetworkManagerPrivate();

private:
    std::shared_ptr<NetworkReply> addRequest(const QUrl& url, quint64& uiTaskId);
    std::shared_ptr<NetworkReply> addBatchRequest(BatchRequestTask& tasks, quint64& uiBatchId);

    bool startRunnable(std::shared_ptr<NetworkRunnable> r);
    void stopRequest(quint64 uiTaskId);
    void stopBatchRequests(quint64 uiBatchId);
    void stopAllRequest();

    bool releaseRequestThread(quint64 uiId);

    bool setMaxThreadCount(int iMax);
    int maxThreadCount() const;

    bool isRequestValid(const QUrl &url) const;
    bool isThreadAvailable() const;

    bool addToFailedQueue(const RequestTask &request);
    void clearFailQueue();

    std::shared_ptr<NetworkReply> getReply(quint64 uiId, bool bRemove = true);
    std::shared_ptr<NetworkReply> getBatchReply(quint64 uiBatchId, bool bRemove = true);
    qint64 updateBatchProgress(quint64 uiId, quint64 uiBatchId, qint64 iBytes, qint64 iTotalBytes, bool bDownload);

    quint64 nextRequestId() const;
    quint64 nextBatchId() const;

    void initialize();
    void unInitialize();
    void reset();
    void resetStopAllFlag();
    void markStopAllFlag();
    bool isStopAllState() const;

private:
    Q_DISABLE_COPY(NetworkManagerPrivate);
    NetworkManager *q_ptr;

private:
#if _MSC_VER >= 1700
    static std::atomic<quint64> ms_uiRequestId;
    static std::atomic<quint64> ms_uiBatchId;
    std::atomic<bool> m_bStopAllFlag;
#else
    static quint64 ms_uiRequestId;
    static quint64 ms_uiBatchId;
    bool m_bStopAllFlag;	// 停止所有请求标记
#endif

    mutable QMutex m_mutex;
    QThreadPool *m_pThreadPool;

    QMap<quint64, std::shared_ptr<NetworkRunnable>> m_mapRunnable;
    QMap<quint64, std::shared_ptr<NetworkReply>> m_mapReply;			// (requestId <---> NetworkReply *)
    QMap<quint64, std::shared_ptr<NetworkReply>> m_mapBatchReply;		// (batchId <---> NetworkReply *)

    QMap<quint64, RequestTask> m_queFailed;	// 请求失败队列

    // (batchId <---> 任务总数)
    QMap<quint64, int> m_mapBatchTotalSize;
    // (batchId <----> 任务完成数)
    QMap<quint64, int> m_mapBatchFinishedSize;

    // (<batchId, <requestId, 已下载字节数>>)
    QMap<quint64, QMap<quint64, qint64>> m_mapBatchDCurrentBytes;
    // (batchId <---> 总下载字节数)
    QMap<quint64, qint64> m_mapBatchDTotalBytes;
    // (<batchId, <requestId, 已上传字节数>>)
    QMap<quint64, QMap<quint64, qint64>> m_mapBatchUCurrentBytes;
    // (batchId <---> 总上传字节数)
    QMap<quint64, qint64> m_mapBatchUTotalBytes;
};
#if _MSC_VER >= 1700
std::atomic<quint64> NetworkManagerPrivate::ms_uiRequestId = 0;
std::atomic<quint64> NetworkManagerPrivate::ms_uiBatchId = 0;
#else
quint64 NetworkManagerPrivate::ms_uiRequestId = 0;
quint64 NetworkManagerPrivate::ms_uiBatchId = 0;
#endif


NetworkManagerPrivate::NetworkManagerPrivate()
    : m_mutex(QMutex::Recursive)
    , m_bStopAllFlag(false)
    , m_pThreadPool(new QThreadPool)
{
}

NetworkManagerPrivate::~NetworkManagerPrivate()
{
    LOG_FUN("");
    LOG_INFO("Runnable size: " << m_mapRunnable.size());
    qDebug() << "[QMultiThreadNetwork] Runnable size: " << m_mapRunnable.size();

    unInitialize();
    m_pThreadPool->deleteLater();

    TRACE_CLASS_CHECK_LEAKS();
}

void NetworkManagerPrivate::initialize()
{
    int nIdeal = QThread::idealThreadCount();
    if (-1 != nIdeal)
    {
        m_pThreadPool->setMaxThreadCount(nIdeal);
    }
    else
    {
        m_pThreadPool->setMaxThreadCount(DEFAULT_MAX_THREAD_COUNT);
    }
    LOG_INFO("idealThreadCount: " << nIdeal);
    LOG_INFO("maxThreadCount: " << m_pThreadPool->maxThreadCount());

    //To add something intialize...
}

void NetworkManagerPrivate::unInitialize()
{
    stopAllRequest();
    reset();
    m_pThreadPool->clear();
    if (!m_pThreadPool->waitForDone(3000))
    {
        LOG_INFO("ThreadPool waitForDone failed!");
        qDebug() << "[QMultiThreadNetwork] ThreadPool waitForDone failed!";
    }
}

void NetworkManagerPrivate::reset()
{
    QMutexLocker locker(&m_mutex);

    m_queFailed.clear();

    m_mapBatchTotalSize.clear();
    m_mapBatchFinishedSize.clear();
    m_mapBatchDCurrentBytes.clear();
    m_mapBatchDTotalBytes.clear();
    m_mapBatchUCurrentBytes.clear();
    m_mapBatchUTotalBytes.clear();

    m_mapRunnable.clear();
    m_mapReply.clear();
    m_mapBatchReply.clear();
}

void NetworkManagerPrivate::resetStopAllFlag()
{
#if _MSC_VER < 1700
    QMutexLocker locker(&m_mutex);
#endif
    if (m_bStopAllFlag)//线程间同步读比写快
    {
        m_bStopAllFlag = false;
    }
}

void NetworkManagerPrivate::markStopAllFlag()
{
#if _MSC_VER < 1700
    QMutexLocker locker(&m_mutex);
#endif
    if (!m_bStopAllFlag)//线程间同步读比写快
    {
        m_bStopAllFlag = true;
    }
}

bool NetworkManagerPrivate::isStopAllState() const
{
#if _MSC_VER < 1700
    QMutexLocker locker(&m_mutex);
#endif
    return m_bStopAllFlag;
}

void NetworkManagerPrivate::stopRequest(quint64 uiTaskId)
{
    RequestTask t;
    std::shared_ptr<NetworkReply> reply = nullptr;

    {
        QMutexLocker locker(&m_mutex);

        if (m_mapRunnable.contains(uiTaskId))
        {
            std::shared_ptr<NetworkRunnable> r = m_mapRunnable.take(uiTaskId);
            if (r.get())
            {
                t = r->task();

#if (QT_VERSION >= QT_VERSION_CHECK(5,9,0))
                if (!m_pThreadPool->tryTake(r.get()))
                {
                    r->quit();
                }
#else
                m_pThreadPool->cancel(r.get());
                r->quit();
#endif
            }
        }

        if (m_queFailed.contains(uiTaskId))
        {
            m_queFailed.remove(uiTaskId);
        }

        reply = m_mapReply.take(uiTaskId);
    }

    if (reply.get())
    {
        t.uiId = uiTaskId;
        t.bSuccess = false;
        t.bCancel = true;
        t.bytesContent = QString("Operation cancelled (id: %1)").arg(uiTaskId).toUtf8();

        reply->replyResult(t, true);
    }
}

void NetworkManagerPrivate::stopBatchRequests(quint64 uiBatchId)
{
    std::shared_ptr<NetworkReply> reply = nullptr;

    {
        QMutexLocker locker(&m_mutex);

        std::shared_ptr<NetworkRunnable> r = nullptr;
        //qDebug() << "Runnable[Before]: " << m_mapRunnable.size();
        for (auto iter = m_mapRunnable.cbegin(); iter != m_mapRunnable.cend(); ++iter)
        {
            r = iter.value();
            if (r.get() && r->batchId() == uiBatchId)
            {
#if (QT_VERSION >= QT_VERSION_CHECK(5,9,0))
                if (!m_pThreadPool->tryTake(r.get()))
                {
                    r->quit();
                }
#else
                m_pThreadPool->cancel(r.get());
                r->quit();
#endif
            }
        }

        try
        {
            //之所以遍历两遍，是让runnable对象先退出,再让他析构
            for (auto iter = m_mapRunnable.begin(); iter != m_mapRunnable.end();)
            {
                r = iter.value();
                if (r.get() && r->batchId() == uiBatchId)
                {
                    iter = m_mapRunnable.erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }
        catch (std::exception* e)
        {
            qCritical() << "erase batch runnable exception:" << QString::fromUtf8(e->what());
            LOG_ERROR("erase batch runnable exception: " << e->what());
        }
        catch (...)
        {
            qCritical() << "erase batch runnable exception:" << GetLastError();
            LOG_ERROR("erase batch runnable exception: " << GetLastError());
        }
        //qDebug() << "Runnable[After]: " << m_mapRunnable.size();

        reply = m_mapBatchReply.take(uiBatchId);

        if (m_mapBatchTotalSize.contains(uiBatchId))
        {
            m_mapBatchTotalSize.remove(uiBatchId);
        }
        if (m_mapBatchFinishedSize.contains(uiBatchId))
        {
            m_mapBatchFinishedSize.remove(uiBatchId);
        }
        if (m_mapBatchDCurrentBytes.contains(uiBatchId))
        {
            m_mapBatchDCurrentBytes.remove(uiBatchId);
        }
        if (m_mapBatchDTotalBytes.contains(uiBatchId))
        {
            m_mapBatchDTotalBytes.remove(uiBatchId);
        }
        if (m_mapBatchUCurrentBytes.contains(uiBatchId))
        {
            m_mapBatchUCurrentBytes.remove(uiBatchId);
        }
        if (m_mapBatchUTotalBytes.contains(uiBatchId))
        {
            m_mapBatchUTotalBytes.remove(uiBatchId);
        }
    }

    if (reply.get())
    {
        RequestTask t;
        t.uiBatchId = uiBatchId;
        t.bSuccess = false;
        t.bCancel = true;
        t.bytesContent = QString("Operation cancelled (Batch id: %1)").arg(uiBatchId).toUtf8();

        reply->replyResult(t, true);
    }
}

void NetworkManagerPrivate::stopAllRequest()
{
    if (!isStopAllState())
    {
        markStopAllFlag();
        std::shared_ptr<NetworkReply> reply = nullptr;

        {
            QMutexLocker locker(&m_mutex);
            std::shared_ptr<NetworkRunnable> r = nullptr;
            for (auto iter = m_mapRunnable.cbegin(); iter != m_mapRunnable.cend(); ++iter)
            {
                r = iter.value();
                if (r.get())
                {
#if (QT_VERSION >= QT_VERSION_CHECK(5,9,0))
                    if (!m_pThreadPool->tryTake(r.get()))
                    {
                        r->quit();
                    }
#else
                    m_pThreadPool->cancel(r.get());
                    r->quit();
#endif
                }
            }
            m_mapRunnable.clear();

            if (!m_mapReply.isEmpty())
            {
                reply = m_mapReply.last();
            }
            else if (!m_mapBatchReply.isEmpty())
            {
                reply = m_mapBatchReply.last();
            }
        }
        reset();

        if (reply.get())
        {
            RequestTask t;
            t.uiId = RequestTask::ALL_TASK;
            t.bSuccess = false;
            t.bCancel = true;
            t.bytesContent = QString("Operation cancelled (All Request)").toUtf8();

            reply->replyResult(t, true);
        }
    }
}

std::shared_ptr<NetworkReply> NetworkManagerPrivate::addRequest(const QUrl& url, quint64& uiId)
{
    if (isRequestValid(url))
    {
        uiId = nextRequestId();
        std::shared_ptr<NetworkReply> pReply = std::make_shared<NetworkReply>(false);
        m_mapReply.insert(uiId, pReply);

        return pReply;
    }
    return nullptr;
}

std::shared_ptr<NetworkReply> NetworkManagerPrivate::addBatchRequest(BatchRequestTask& tasks, quint64& uiBatchId)
{
    uiBatchId = nextBatchId();
    m_mapBatchTotalSize[uiBatchId] = tasks.size();

    std::shared_ptr<NetworkReply> pReply = std::make_shared<NetworkReply>(true);
    m_mapBatchReply.insert(uiBatchId, pReply);

    for (int i = 0; i < tasks.size(); ++i)
    {
        tasks[i].uiBatchId = uiBatchId;
        tasks[i].uiId = nextRequestId();

        Q_Q(NetworkManager);
        q->startAsRunnable(tasks[i]);
    }

    return pReply;
}

quint64 NetworkManagerPrivate::nextRequestId() const
{
#if _MSC_VER < 1700
    QMutexLocker locker(&m_mutex);
#endif
    return ++ms_uiRequestId;
}

quint64 NetworkManagerPrivate::nextBatchId() const
{
#if _MSC_VER < 1700
    QMutexLocker locker(&m_mutex);
#endif
    return ++ms_uiBatchId;
}

bool NetworkManagerPrivate::startRunnable(std::shared_ptr<NetworkRunnable> r)
{
    if (r.get())
    {
        try
        {
            m_pThreadPool->start(r.get());
            {
                QMutexLocker locker(&m_mutex);
                m_mapRunnable.insert(r->requsetId(), r);
            }
            return true;
        }
        catch (std::exception* e)
        {
            qDebug() << "[QMultiThreadNetwork] startRunnable() exception:" << QString::fromUtf8(e->what());
            LOG_ERROR("startRunnable() exception: " << e->what());
        }
        catch (...)
        {
            qDebug() << "[QMultiThreadNetwork] startRunnable() exception:" << GetLastError();
            LOG_ERROR("startRunnable() exception: " << GetLastError());
        }
    }

    return false;
}

bool NetworkManagerPrivate::setMaxThreadCount(int nMax)
{
    bool bRet = false;
    if (nMax >= 1 && nMax <= 16 && m_pThreadPool)
    {
        LOG_INFO("ThreadPool maxThreadCount: " << nMax);
        qDebug() << "[QMultiThreadNetwork] ThreadPool maxThreadCount: " << nMax;
        m_pThreadPool->setMaxThreadCount(nMax);
        bRet = true;
    }
    return bRet;
}

int NetworkManagerPrivate::maxThreadCount() const
{
    if (m_pThreadPool)
    {
        return m_pThreadPool->maxThreadCount();
    }
    return -1;
}

bool NetworkManagerPrivate::isThreadAvailable() const
{
    if (m_pThreadPool)
    {
        return (m_pThreadPool->activeThreadCount() < m_pThreadPool->maxThreadCount());
    }
    return false;
}

bool NetworkManagerPrivate::isRequestValid(const QUrl &url) const
{
    return (url.isValid());
}

std::shared_ptr<NetworkReply> NetworkManagerPrivate::getReply(quint64 uiRequestId, bool bRemove)
{
    QMutexLocker locker(&m_mutex);
    if (m_mapReply.contains(uiRequestId))
    {
        if (bRemove)
        {
            return m_mapReply.take(uiRequestId);
        }
        else
        {
            return m_mapReply.value(uiRequestId);
        }
    }
    qDebug() << QString("%1 failed! Id: ").arg(__FUNCTION__) << uiRequestId;
    LOG_ERROR(__FUNCTION__ << " failed! Id: " << uiRequestId);
    return nullptr;
}

std::shared_ptr<NetworkReply> NetworkManagerPrivate::getBatchReply(quint64 uiBatchId, bool bRemove)
{
    QMutexLocker locker(&m_mutex);
    if (m_mapBatchReply.contains(uiBatchId))
    {
        if (bRemove)
        {
            return m_mapBatchReply.take(uiBatchId);
        }
        else
        {
            return m_mapBatchReply.value(uiBatchId);
        }
    }
    return nullptr;
}

bool NetworkManagerPrivate::addToFailedQueue(const RequestTask &request)
{
    QMutexLocker locker(&m_mutex);
    if (!m_queFailed.contains(request.uiId))
    {
        m_queFailed.insert(request.uiId, request);
        return true;
    }
    return false;
}

void NetworkManagerPrivate::clearFailQueue()
{
    QMutexLocker locker(&m_mutex);
    m_queFailed.clear();
}

qint64 NetworkManagerPrivate::updateBatchProgress(quint64 uiRequestId, quint64 uiBatchId, qint64 iBytes, qint64 iTotalBytes, bool bDownload)
{
    Q_UNUSED(iTotalBytes);
    //postEvent()过来的都在主线程，不用加锁

    //该请求任务比上次多下载/上传的字节数
    quint64 uiIncreased = 0;
    quint64 uiTotalBytes = 0;
    if (iBytes == 0)
    {
        if (bDownload)
        {
            return m_mapBatchDTotalBytes[uiBatchId];
        }
        else
        {
            return m_mapBatchUTotalBytes[uiBatchId];
        }
    }

    if (bDownload)
    {
        QMap<quint64, qint64> mapReqId2Bytes = m_mapBatchDCurrentBytes.value(uiBatchId);
        if (mapReqId2Bytes.contains(uiRequestId))
        {
            if (iBytes > mapReqId2Bytes.value(uiRequestId))
            {
                uiIncreased = iBytes - mapReqId2Bytes.value(uiRequestId);
            }
        }
        else
        {
            uiIncreased = iBytes;
        }
        m_mapBatchDCurrentBytes[uiBatchId][uiRequestId] = iBytes;

        uiTotalBytes = m_mapBatchDTotalBytes.value(ms_uiBatchId) + uiIncreased;
        m_mapBatchDTotalBytes[uiBatchId] = uiTotalBytes;
    }
    else
    {
        QMap<quint64, qint64> mapReqId2Bytes = m_mapBatchUCurrentBytes.value(uiBatchId);
        if (mapReqId2Bytes.contains(uiRequestId))
        {
            if (iBytes > mapReqId2Bytes.value(uiRequestId))
            {
                uiIncreased = iBytes - mapReqId2Bytes.value(uiRequestId);
            }
        }
        else
        {
            uiIncreased = iBytes;
        }
        m_mapBatchUCurrentBytes[uiBatchId][uiRequestId] = iBytes;

        uiTotalBytes = m_mapBatchUTotalBytes.value(ms_uiBatchId) + uiIncreased;
        m_mapBatchUTotalBytes[uiBatchId] = uiTotalBytes;
    }

    return uiTotalBytes;
}

bool NetworkManagerPrivate::releaseRequestThread(quint64 uiRequestId)
{
    QMutexLocker locker(&m_mutex);
    if (m_mapRunnable.contains(uiRequestId))
    {
        std::shared_ptr<NetworkRunnable> r = m_mapRunnable.take(uiRequestId);
        if (r.get())
        {
            r->quit();
        }
        return true;
    }
    else
    {
        LOG_ERROR("releaseRequestThread - request not exist, id:" << uiRequestId);
        qWarning() << "releaseRequestThread - request not exist, id:" << uiRequestId;
    }
    return false;
}


//////////////////////////////////////////////////////////////////////////
NetworkManager *NetworkManager::ms_pInstance = nullptr;
bool NetworkManager::ms_bIntialized = false;

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new NetworkManagerPrivate)
{
    LOG_FUN("");
    Q_D(NetworkManager);
    d->q_ptr = this;
    qDebug() << "[QMultiThreadNetwork] Thread : " << QThread::currentThreadId();
}

NetworkManager::~NetworkManager()
{
    LOG_FUN("");
    ms_pInstance = nullptr;
}

NetworkManager* NetworkManager::globalInstance()
{
    if (!ms_pInstance)
    {
        ms_pInstance = new NetworkManager;
    }
    return ms_pInstance;
}

void NetworkManager::deleteInstance()
{
    if (ms_pInstance)
    {
        delete ms_pInstance;
        ms_pInstance = nullptr;
    }
}

bool NetworkManager::isInstantiated()
{
    return (ms_pInstance != nullptr);
}

void NetworkManager::initialize()
{
    LOG_FUN("");
    if (!ms_bIntialized)
    {
        NetworkManager::globalInstance()->init();
        ms_bIntialized = true;
    }
}

void NetworkManager::unInitialize()
{
    LOG_FUN("");
    if (ms_bIntialized)
    {
        if (isInstantiated())
        {
            NetworkManager::globalInstance()->fini();
            NetworkManager::deleteInstance();
        }
        ms_bIntialized = false;
    }
}

bool NetworkManager::isInitialized()
{
    return ms_bIntialized;
}

void NetworkManager::init()
{
    LOG_FUN("");
    Q_D(NetworkManager);
    d->initialize();
}

void NetworkManager::fini()
{
    LOG_FUN("");
    Q_D(NetworkManager);
    d->unInitialize();
}

NetworkReply *NetworkManager::addRequest(RequestTask& request)
{
    Q_D(NetworkManager);
    d->resetStopAllFlag();

    std::shared_ptr<NetworkReply> pReply = d->addRequest(request.url, request.uiId);
    if (pReply.get())
    {
        startAsRunnable(request);
    }
    return pReply.get();
}

NetworkReply *NetworkManager::addBatchRequest(BatchRequestTask& tasks, quint64 &uiBatchId)
{
    Q_D(NetworkManager);
    d->resetStopAllFlag();

    uiBatchId = 0;
    if (!tasks.isEmpty())
    {
        std::shared_ptr<NetworkReply> pReply = d->addBatchRequest(tasks, uiBatchId);
        return pReply.get();
    }
    return nullptr;
}

void NetworkManager::stopRequest(quint64 uiTaskId)
{
    LOG_FUN("");
    Q_D(NetworkManager);
    d->stopRequest(uiTaskId);
}

void NetworkManager::stopBatchRequests(quint64 uiBatchId)
{
    LOG_FUN("");
    Q_D(NetworkManager);
    d->stopBatchRequests(uiBatchId);
}

void NetworkManager::stopAllRequest()
{
    LOG_FUN("");
    Q_D(NetworkManager);
    d->stopAllRequest();
}

bool NetworkManager::startAsRunnable(const RequestTask &request)
{
    std::shared_ptr<NetworkRunnable> r = std::make_shared<NetworkRunnable>(request);
    qRegisterMetaType<RequestTask>("RequestTask");
    connect(r.get(), SIGNAL(requestFinished(const RequestTask &)),
        this, SLOT(onRequestFinished(const RequestTask &)));

    Q_D(NetworkManager);
    if (!d->startRunnable(r))
    {
        qDebug() << "[QMultiThreadNetwork] ThreadPool->start() failed!";
        LOG_ERROR("ThreadPool->start() failed!");

        d->addToFailedQueue(request);
        r.reset();
        return false;
    }
    return true;
}

bool NetworkManager::setMaxThreadCount(int iMax)
{
    Q_D(NetworkManager);
    return d->setMaxThreadCount(iMax);
}

int NetworkManager::maxThreadCount()
{
    Q_D(NetworkManager);
    return d->maxThreadCount();
}

bool NetworkManager::event(QEvent *event)
{
    if (event->type() == NetworkEvent::NetworkProgress)
    {
        Q_D(NetworkManager);
        if (d->isStopAllState())
        {
            return true;
        }

        NetworkProgressEvent *evtProgress = static_cast<NetworkProgressEvent *>(event);
        if (nullptr != evtProgress)
        {
            updateProgress(evtProgress->uiId,
                evtProgress->uiBatchId,
                evtProgress->iBtyes,
                evtProgress->iTotalBtyes,
                evtProgress->bDownload);
        }
        return true;
    }

    return QObject::event(event);
}

void NetworkManager::updateProgress(quint64 uiId, quint64 uiBatchId, qint64 iBytes, qint64 iTotalBytes, bool bDownload)
{
    if (uiId == 0 || iBytes == 0 || iTotalBytes == 0)
        return;

    if (bDownload)
    {
        emit downloadProgress(uiId, iBytes, iTotalBytes);
    }
    else
    {
        emit uploadProgress(uiId, iBytes, iTotalBytes);
    }

    if (uiBatchId > 0)//批量请求
    {
        Q_D(NetworkManager);
        quint64 uiBytes = d->updateBatchProgress(uiId, uiBatchId, iBytes, iTotalBytes, bDownload);
        if (bDownload)
        {
            emit batchDownloadProgress(uiBatchId, uiBytes);
        }
        else
        {
            emit batchUploadProgress(uiBatchId, uiBytes);
        }
    }
}

void NetworkManager::onRequestFinished(const RequestTask &request)
{
    //Q_ASSERT(QThread::currentThread() == NetworkManager::globalInstance()->thread());
    Q_D(NetworkManager);
    if (d->isStopAllState())
        return;

    bool bNotify = true;

    RequestTask task = request;
    //1.处理请求失败的情况
    if (!task.bSuccess)
    {
        // 加入到失败队列，如果成功表示第一次失败，否则表示是第二次失败
        //		第一次失败的情况: 需要将任务再次加入到等待队列重新执行一遍
        //		第二次失败的情况: 需要将任务结果反馈给用户
        if (task.bTryAgainWhileFailed && d->addToFailedQueue(task))
        {
            bNotify = false;
        }
    }

    try
    {
        //2.通知用户结果
        if (bNotify)
        {
            std::shared_ptr<NetworkReply> pReply;
            bool bDestroyed = true;
            if (task.uiBatchId == 0)
            {
                pReply = d->getReply(task.uiId, bDestroyed);
            }
            else if (task.uiBatchId > 0)//批量任务
            {
                int sizeFinished = 0;
                int sizeTotal = 0;
                {
                    QMutexLocker locker(&d->m_mutex);
                    sizeTotal = d->m_mapBatchTotalSize.value(task.uiBatchId);
                    if (sizeTotal > 0)
                    {
                        sizeFinished = d->m_mapBatchFinishedSize.value(task.uiBatchId);
                        d->m_mapBatchFinishedSize[task.uiBatchId] = ++sizeFinished;

                        if (sizeFinished == sizeTotal)
                        {
                            d->m_mapBatchTotalSize.remove(task.uiBatchId);
                            d->m_mapBatchFinishedSize.remove(task.uiBatchId);
                        }
                    }
                }

                if (task.bSuccess)
                {
                    if (sizeFinished < sizeTotal) // 还有请求未完成
                    {
                        bDestroyed = false;
                    }
                }
                else//批量任务失败
                {
                    if (!task.bAbortBatchWhileOneFailed && (sizeFinished < sizeTotal))
                    {
                        bDestroyed = false;
                    }
                }
                pReply = d->getBatchReply(task.uiBatchId, bDestroyed);
            }

            if (pReply.get())
            {
                task.bFinished = true;
                task.bCancel = (task.uiBatchId > 0 && !task.bSuccess && task.bAbortBatchWhileOneFailed);
                pReply->replyResult(task, bDestroyed);
                if (task.uiBatchId > 0 && bDestroyed)
                {
                    qDebug() << QStringLiteral("[QMultiThreadNetwork] Batch finished! Id：%1").arg(task.uiBatchId);
                    emit batchRequestFinished(task.uiBatchId, task.bSuccess);
                }
            }

            //3.如果是批量任务失败后，并且指定了bAbortBatchWhileOneFailed，就停止该批次的任务
            if (task.uiBatchId > 0 && !task.bSuccess && task.bAbortBatchWhileOneFailed)
            {
                d->stopBatchRequests(task.uiBatchId);
            }
        }

        //4.释放任务线程，使其变成空闲状态
        d->releaseRequestThread(task.uiId);

        if (!bNotify)
        {
            startAsRunnable(task);
        }
    }
    catch (std::exception* e)
    {
        qCritical() << "NetworkManager::onRequestFinished() exception:" << QString::fromUtf8(e->what());
        LOG_ERROR("NetworkManager::onRequestFinished() exception: " << e->what());
    }
    catch (...)
    {
        qCritical() << "NetworkManager::onRequestFinished() exception:" << GetLastError();
        LOG_ERROR("NetworkManager::onRequestFinished() exception: " << GetLastError());
    }
}
