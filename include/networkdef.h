#ifndef NETWORKDEF_H
#define NETWORKDEF_H

#include <QUrl>
#include <QEvent>
#include <QMap>
#include <QByteArray>
#include <QVariant>

#pragma pack(push, _CRT_PACKING)

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
	QUrl url;

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

	bool bRemoveFileWhileExist;

	// 任务失败后，是否再尝试请求一次，默认为false.
	bool bTryAgainWhileFailed;

	// 批量请求失败一个就终止整批请求，默认为false.
	bool bAbortBatchWhileOneFailed;

	// 多线程下载模式(需服务器支持)
	//	 多线程下载模式下，一个文件由多个下载通道同时下载.
	//	 需要先获取http head的Content-Length，所以需要服务器的支持.
	// n个下载通道(默认是5)(取值范围2-10)
	quint16 nDownloadThreadCount;

	// 用户自定义内容（可用于回传）
	QVariant varArg1;
	// 用户自定义内容（可用于回传）
	QVariant varArg2;
	// 用户自定义内容（可用于回传）
	QVariant varArg3;

	//////////////////////返回结果的字段/////////////////////////////////////////////
	bool bFinished;	//正常结束
	bool bCancel;	//玩家取消

	// 请求是否成功
	bool bSuccess;
	// 请求返回的内容/返回的错误信息等.
	QByteArray bytesContent;

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
		bRemoveFileWhileExist = false;
		bTryAgainWhileFailed = false;
		bAbortBatchWhileOneFailed = false;
		nDownloadThreadCount = 5;
	}
};
Q_DECLARE_METATYPE(RequestTask);
typedef QVector<RequestTask> BatchRequestTask;


#define ALL_TASK_ID 0xFFFFFFFF

#if _MSC_VER >= 1700
#define DECL_EQ_DELETE = delete
#else
#define DECL_EQ_DELETE
#endif

#define CLASS_DISABLE_COPY(Class) \
	Class(const Class &) DECL_EQ_DELETE;\
    Class &operator=(const Class &) DECL_EQ_DELETE;


////////////////// Event ////////////////////////////////////////////////////
namespace QEventRegister
{
	template <class Type>
	int regiesterEvent(const Type& eventName)
	{
		typedef std::map<Type, int> TUserEventMap;
		static TUserEventMap s_mapUserEvent;

		TUserEventMap::const_iterator iter = s_mapUserEvent.find(eventName);
		if (iter != s_mapUserEvent.cend())
		{
			return iter->second;
		}

		int nEventType = QEvent::registerEventType();
		s_mapUserEvent[eventName] = nEventType;
		return nEventType;
	}
}

namespace NetworkEvent
{
	const QEvent::Type WaitForIdleThread = (QEvent::Type)QEventRegister::regiesterEvent(QLatin1String("WaitForIdleThread"));
	const QEvent::Type ReplyResult = (QEvent::Type)QEventRegister::regiesterEvent(QLatin1String("ReplyResult"));
	const QEvent::Type NetworkProgress = (QEvent::Type)QEventRegister::regiesterEvent(QLatin1String("NetworkProgress"));
	const QEvent::Type DestroyRunnable = (QEvent::Type)QEventRegister::regiesterEvent(QLatin1String("DestroyRunnable"));
}

//等待空闲线程事件
class WaitForIdleThreadEvent : public QEvent
{
public:
	WaitForIdleThreadEvent() : QEvent(QEvent::Type(NetworkEvent::WaitForIdleThread)) {}
};

//通知结果事件
class ReplyResultEvent : public QEvent
{
public:
	ReplyResultEvent() : QEvent(QEvent::Type(NetworkEvent::ReplyResult)), bDestroyed(true) {}

	RequestTask request;
	bool bDestroyed;
};

//下载/上传进度事件
class NetworkProgressEvent : public QEvent
{
public:
	NetworkProgressEvent() : QEvent(QEvent::Type(NetworkEvent::NetworkProgress))
		, bDownload(true)
		, uiId(0)
		, uiBatchId(0)
		, iBtyes(0)
		, iTotalBtyes(0)
	{
	}

	bool bDownload;
	quint64 uiId;
	quint64 uiBatchId;
	qint64 iBtyes;
	qint64 iTotalBtyes;
};

//通知结果事件
class DestroyRunnableEvent : public QEvent
{
public:
	DestroyRunnableEvent() : QEvent(QEvent::Type(NetworkEvent::ReplyResult)), uiId(0) {}
	quint64 uiId;
};

#pragma pack(pop)

#endif ///NETWORKDEF_H