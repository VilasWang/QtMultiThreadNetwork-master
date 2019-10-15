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

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <atomic>
#include "networkdefs.h"
#include "networkglobal.h"

class QEvent;
class NetworkManagerPrivate;
class NetworkReply;
class NETWORK_EXPORT NetworkManager : public QObject
{
    Q_OBJECT;
    Q_DECLARE_PRIVATE(NetworkManager)

public:
    // 初始化和反初始化必须在主线程中调用
    static void initialize();
    static void unInitialize();
    // 是否已经初始化
    static bool isInitialized();

    static NetworkManager* globalInstance();
    static void deleteInstance();
    // 是否已经实例化
    static bool isInstantiated();

public:
    // NetworkReply对象会在请求结束后自动销毁，用户不用主动销毁
    // 添加单个请求任务（若返回nullptr，表示url无效）
    NetworkReply *addRequest(QMTNetwork::RequestTask& task);

    // 添加批量请求任务
    NetworkReply *addBatchRequest(QMTNetwork::BatchRequestTask& tasks, quint64 &uiBatchId);

    // 停止所有的请求任务
    void stopAllRequest();
    // 停止指定batchid的批次请求任务
    void stopBatchRequests(quint64 uiBatchId);
    // 停止某个请求任务
    void stopRequest(quint64 uiTaskId);

    // 设置线程池最大线程数（从1-16个, 默认5线程）
    bool setMaxThreadCount(int iMax);
    int maxThreadCount();

Q_SIGNALS:
    void errorMessage(const QString& error);
    void batchRequestFinished(quint64 uiBatchId, bool bAllSuccess);

    // Progress
    void downloadProgress(quint64 uiRequestId, qint64 iBytesDownload, qint64 iBytesTotal);
    void uploadProgress(quint64 uiRequestId, qint64 iBytesUpload, qint64 iBytesTotal);
    void batchDownloadProgress(quint64 uiBatchId, qint64 iBytesDownload);
    void batchUploadProgress(quint64 uiBatchId, qint64 iBytesUpload);

public Q_SLOTS :
    void onRequestFinished(const QMTNetwork::RequestTask &);

public:
    bool event(QEvent *pEvent) Q_DECL_OVERRIDE;

private:
    explicit NetworkManager(QObject *parent = 0);
    ~NetworkManager();
    Q_DISABLE_COPY(NetworkManager);

private:
    void init();
    void fini();

    bool startAsRunnable(const QMTNetwork::RequestTask &task);

    // bDownload(false: upload)
    void updateProgress(quint64 uiRequestId, quint64 uiBatchId,
        qint64 iBytes, qint64 iTotalBytes, bool bDownload);

private:
    QScopedPointer<NetworkManagerPrivate> d_ptr;
    static NetworkManager* ms_pInstance;
#if _MSC_VER >= 1700
    static std::atomic<bool> ms_bIntialized;
#else
    static bool ms_bIntialized;
#endif
};

#endif //NETWORKMANAGER_H