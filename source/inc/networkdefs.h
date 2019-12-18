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

#ifndef NETWORKDEF_H
#define NETWORKDEF_H

#include <QMap>
#include <QByteArray>
#include <QVariant>

#pragma pack(push, _CRT_PACKING)

namespace QMTNetwork {

    // 本模块支持的协议：HTTP(S)/FTP
    // 本模块支持的HTTP(s)协议请求方法：GET/POST/PUT/DELETE/HEAD

    enum RequestType
    {
        // Download（支持http(s)和ftp）
        eTypeDownload = 0,
        // Multi-Thread Download（支持http(s)）
        eTypeMTDownload = 1,
        // Upload（支持http(s)和ftp）
        eTypeUpload = 2,
        // GET方式请求（支持http(s)和ftp）
        eTypeGet = 3,
        // POST方式请求（支持http(s)）
        eTypePost = 4,
        // PUT方式请求（支持http(s)和ftp）
        eTypePut = 5,
        // DELETE方式请求（支持http(s)）
        eTypeDelete = 6,
        // HEAD方式请求（支持http(s)）
        eTypeHead = 7,

        eTypeUnknown = -1,
    };

    //请求结构
    struct RequestTask
    {
        // 请求的类型：上传/下载/其他请求
        RequestType eType;

        // url
        // 注意: ftp上传的url需指定文件名.如"ftp://10.0.192.47:21/upload/test.zip", 文件将被保存为test.zip.
        QString url;
        QString redirectUrl;

        // case eTypeDownload:	下载的文件存放的本地目录. (绝对路径 or 相对路径)
        // case eTypeUpload：	待上传的文件路径. (绝对路径 or 相对路径)
        // case eTypePost：		post的参数. 如："a=b&c=d".
        // case eTypePut：		put的数据流.
        QString strReqArg;

        // case eTypeDownload: 若指定了strSaveFileName，则保存的文件名是strSaveFileName;否则，根据url.
        QString strSaveFileName;

        // 请求的header信息
        //void QNetworkRequest::setRawHeader(const QByteArray &headerName, const QByteArray &value);
        QMap<QByteArray, QByteArray> mapRawHeader;

        // 是否显示进度，默认为false.
        bool bShowProgress;

        // 若文件存在，是否替换，默认为false.
        bool bReplaceFileIfExist;

        // 若任务失败，是否再尝试请求一次，默认为false.
        bool bTryAgainIfFailed;

        // 批量请求，是否有一个失败就终止整批请求，默认为false.
        bool bAbortBatchWhenFailed;

        // 上传文件使用PUT方式，否则POST方式，仅HTTP(s)有效，默认为true.
        bool bUploadUsePut;

        // 单文件多线程下载模式(需服务器支持) 注：eType为eTypeMTDownload时有效
        //	 多线程下载模式下，一个文件由多个下载通道同时下载.
        //	 需要先获取http head的Content-Length，所以需要服务器的支持.
        // n个下载通道(默认是5)(取值范围2-10)
        quint16 nDownloadThreadCount;

        // 最大重定向次数
        quint16 nMaxRedirectionCount;

        // 用户自定义内容（可用于回传）
        QVariant varArg1;
        // 用户自定义内容（可用于回传）
        QVariant varArg2;
        // 用户自定义内容（可用于回传）
        QVariant varArg3;


        //////////////////////返回结果的字段/////////////////////////////////////////////
        // 正常结束
        bool bFinished;
        // 玩家取消
        bool bCancel;

        // 请求是否成功
        bool bSuccess;
        // 请求返回的内容
        QByteArray bytesContent;
        // 返回的错误信息
        QString strError;

        // 请求ID
        quint64 uiId;
        // 批次ID (批量请求)
        quint64 uiBatchId;

        RequestTask()
        {
            uiId = 0;
            uiBatchId = 0;
            eType = eTypeUnknown;
            bFinished = false;
            bCancel = false;
            bSuccess = false;
            bShowProgress = false;
            bReplaceFileIfExist = false;
            bTryAgainIfFailed = false;
            bAbortBatchWhenFailed = false;
            nDownloadThreadCount = 5;
            bUploadUsePut = true;
            nMaxRedirectionCount = 5;
        }
    };
    Q_DECLARE_METATYPE(RequestTask);
    typedef QVector<RequestTask> BatchRequestTask;
}

#pragma pack(pop)

#endif ///NETWORKDEF_H