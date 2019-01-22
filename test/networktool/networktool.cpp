#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QTextCodec>
#include <QFileDialog>
#include <QButtonGroup>
#include <QListView>
#include <QMessageBox>
#include <QButtonGroup>
#include <QStandardPaths>
#include <QPainter>
#include "networktool.h"
#include "NetworkManager.h"
#include "NetworkReply.h"

#define QMT_POST_TEST
#define DEFAULT_CONCURRENT_TASK 4
#define DEFAULT_MTDOWNLOAD_COUNT 5
//局域网Apache http服务器
#define HTTP_SERVER_IP "127.0.0.1"
#define HTTP_SERVER_PORT "80"

struct STTask
{
	QString strUrl;
	RequestType eType;
	QString strSaveFileName;
	bool bShowProgress;
	// 请求ID
	quint64 uiId;
	// 批次ID (批量请求)
	quint64 uiBatchId;
};
Q_DECLARE_METATYPE(STTask);


NetworkTool::NetworkTool(QWidget *parent)
	: QMainWindow(parent)
	, m_nFailedNum(0)
	, m_nSuccessNum(0)
	, m_nTotalNum(0)
	, m_nbytesReceived(0)
	, m_nBytesTotalDownload(0)
	, m_nbytesSent(0)
	, m_nBytesTotalUpload(0)
	, m_requestId(0)
	, m_batchId(0)
{
	uiMain.setupUi(this);
	setFixedSize(700, 620);
	setWindowTitle(QStringLiteral("Qt Network Tool"));

	initialize();
	onUpdateDefaultValue();
}

NetworkTool::~NetworkTool()
{
	unIntialize();
}

void NetworkTool::initialize()
{
	NetworkManager::initialize();

	initCtrls();
	initConnecting();
}

void NetworkTool::unIntialize()
{
	NetworkManager::unInitialize();
}

void NetworkTool::initCtrls()
{
	//uiMain.progressBar_d->setFormat("%p%(%v / %m)");
	//uiMain.progressBar_u->setFormat("%p%(%v / %m)");

	m_pWidgetAddTask = new QWidget;
	uiAddTask.setupUi(m_pWidgetAddTask);
	m_pWidgetAddTask->hide();

	m_pWidgetAddBatch = new QWidget;
	uiAddBatchTask.setupUi(m_pWidgetAddBatch);
	m_pWidgetAddBatch->hide();

	uiMain.cmb_concurrentTask->setView(new QListView(this));
	uiAddTask.cmb_multiDownload->setView(new QListView(this));
	for (int i = 1; i <= 8; ++i)
	{
		uiMain.cmb_concurrentTask->addItem(QString::number(i));
	}
	for (int i = 2; i <= 10; ++i)
	{
		uiAddTask.cmb_multiDownload->addItem(QString::number(i));
	}
	uiMain.cmb_concurrentTask->setCurrentText(QString::number(DEFAULT_CONCURRENT_TASK));
	uiAddTask.cmb_multiDownload->setCurrentText(QString::number(DEFAULT_MTDOWNLOAD_COUNT));

	bg_protocal = new QButtonGroup(this);
	bg_protocal->addButton(uiAddTask.cb_http);
	bg_protocal->addButton(uiAddTask.cb_ftp);
	bg_protocal->setExclusive(true);
	bg_protocal->metaObject()->className();

	bg_type = new QButtonGroup(this);
	bg_type->addButton(uiAddTask.cb_download);
	bg_type->addButton(uiAddTask.cb_upload);
	bg_type->addButton(uiAddTask.cb_get);
	bg_type->addButton(uiAddTask.cb_post);
	bg_type->addButton(uiAddTask.cb_put);
	bg_type->addButton(uiAddTask.cb_delete);
	bg_type->addButton(uiAddTask.cb_head);
	bg_type->setExclusive(true);

	m_pModel = new Model(this);
	m_pDelegate = new TaskDelegate(this);

	m_pListView = new Listview(this);
	m_pListView->setGeometry(10, 110, 310, 490);
	m_pListView->setStyleSheet("background:transparent;");
	m_pListView->setListModel(m_pModel);
	m_pListView->setListDelegate(m_pDelegate);
}

void NetworkTool::initConnecting()
{
	connect(uiMain.btn_add, SIGNAL(clicked()), m_pWidgetAddTask, SLOT(show()));
	connect(uiMain.btn_addBatch, SIGNAL(clicked()), m_pWidgetAddBatch, SLOT(show()));
	connect(uiMain.btn_abort, SIGNAL(clicked()), this, SLOT(onAbortTask()));
	connect(uiMain.btn_abortAll, SIGNAL(clicked()), this, SLOT(onAbortAllTask()));
	connect(uiAddTask.btn_browser1, SIGNAL(clicked()), this, SLOT(onGetSaveDirectory()));
	connect(uiAddTask.btn_browser2, SIGNAL(clicked()), this, SLOT(onGetUploadFile()));
	connect(uiAddBatchTask.btn_browser, SIGNAL(clicked()), this, SLOT(onGetBatchTaskConfigFile()));
	connect(uiAddTask.btn_start, SIGNAL(clicked()), this, SLOT(onAddTask()));
	connect(uiAddBatchTask.btn_start_2, SIGNAL(clicked()), this, SLOT(onBatchRequest()));
	connect(uiAddTask.cb_useDefault, &QAbstractButton::toggled, this, [=](bool checked) {
		if (checked)
		{
			onUpdateDefaultValue();
		}
	});
	connect(uiMain.cmb_concurrentTask, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
			this, [=](const QString &strText) {
		int num = strText.toInt();
		if (num >= 1 && num <= 8)
		{
			NetworkManager::globalInstance()->setMaxThreadCount(num);
		}
	});
	connect(bg_protocal, SIGNAL(buttonToggled(int, bool)), this, SLOT(onUpdateDefaultValue()));
	connect(bg_type, SIGNAL(buttonToggled(int, bool)), this, SLOT(onUpdateDefaultValue()));

	connect(NetworkManager::globalInstance(), &NetworkManager::downloadProgress
			, this, &NetworkTool::onDownloadProgress);
	connect(NetworkManager::globalInstance(), &NetworkManager::uploadProgress
			, this, &NetworkTool::onUploadProgress);
	connect(NetworkManager::globalInstance(), &NetworkManager::batchDownloadProgress
			, this, &NetworkTool::onBatchDownloadProgress);
	connect(NetworkManager::globalInstance(), &NetworkManager::batchUploadProgress
			, this, &NetworkTool::onBatchUploadProgress);
	connect(NetworkManager::globalInstance(), &NetworkManager::errorMessage
			, this, &NetworkTool::onErrorMessage);
}

void NetworkTool::onUpdateDefaultValue()
{
	uiAddTask.lineEdit_url->clear();
	uiAddTask.lineEdit_arg->clear();
	uiAddTask.lineEdit_filename->clear();
	uiAddTask.lineEdit_saveDir->clear();
	uiAddTask.lineEdit_uploadFile->clear();

	if (uiAddTask.cb_useDefault->isChecked())
	{
		//HTTP
		if (uiAddTask.cb_http->isChecked())
		{
			if (uiAddTask.cb_download->isChecked())
			{
				QString strUrl = "https://cdn.mysql.com//Downloads/MySQL-8.0/mysql-8.0.13-winx64.zip";
				//QString strUrl = "http://8dx.pc6.com/xzx6/curl_v7.61.1.zip";
				uiAddTask.lineEdit_url->setText(strUrl);
				uiAddTask.lineEdit_saveDir->setText(getDefaultDownloadDir());
			}
			else if (uiAddTask.cb_upload->isChecked())
			{
				QString strUrl = QString("http://%1:%2/_php/upload.php?filename=upload/VerComp_qt.dat")
					.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
				uiAddTask.lineEdit_url->setText(strUrl);
				uiAddTask.lineEdit_uploadFile->setText("resources/test/VerComp.dat");
			}
			else if (uiAddTask.cb_get->isChecked())
			{
				uiAddTask.lineEdit_url->setText(QStringLiteral("http://m.kugou.com/singer/list/88?json=true"));
			}
			else if (uiAddTask.cb_post->isChecked())
			{
				QString strArg = "userId=121892674&userName=33CxghNmt1FhAA==&st=QQBnAEEAQQBBAEUATA"
					"B2AFEAdwBjAEEAQQBBAEEAQQBBAEEAQQBBAEEATAB2AFAANwBoAE4AcwBJ"
					"AC8AbwBWAFMAQQArAEQAVgBIADIAdgAyAHcARgBRAGYANABJAHkAOQA3AFAAYQBkAFMARwBoAEoA"
					"KwBUAEoAcAAzADkAVgBYAFYAMwBDAE4AVABiAHEAZQB3AE4AMAANAAoAOABlAHUANQBBAHMAUwBY"
					"AFEAbQAyAFUAWQBmAHEAMgA1ADkAcQBvAG4AZQBCAFEAYgB5AE8ANwAyAFQAMQB0AGwARwBIADYA"
					"dAB1AGYAYQBxAEoAMwBnAFUARwA4AGoAdQA5AGsAOQBzAFoAYQB1AHAARwBjAE8ANABnADIAegBn"
					"ADIANgB1AEcANwBoAHAAUwBHADIAVQANAAoAWQBmAHEAMgA1ADkAcQBvAG4AZQBCAFEAYgB5AE8A"
					"NwAyAFQAMAA9AA==";

				uiAddTask.lineEdit_url->setText("https://passportservice.7fgame.com/HttpService/PlatService.ashx");
				uiAddTask.lineEdit_arg->setText(strArg);
			}
			else if (uiAddTask.cb_put->isChecked())
			{
				QString strUrl = QString("http://%1:%2/_php/upload.php?filename=upload/VerComp_qt.dat")
					.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
				uiAddTask.lineEdit_url->setText(strUrl);
				uiAddTask.lineEdit_uploadFile->setText("resources/test/VerComp.dat");
			}
			else if (uiAddTask.cb_delete->isChecked())
			{
				QString strUrl = QString("http://%1:%2/_php/delete.php?filename=upload/1.jpg")
					.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
				uiAddTask.lineEdit_url->setText(strUrl);
			}
			else if (uiAddTask.cb_head->isChecked())
			{
				uiAddTask.lineEdit_url->setText("http://forspeed.onlinedown.net/down/warcraftiii1.24bbzh.rar");
			}
			/*else if (uiAddTask.cb_batchDownload->isChecked())
			{
				uiAddTask.lineEdit_url->setText("resources/test/VerComp.dat");
				uiAddTask.lineEdit_saveDir->setText(getDefaultDownloadDir());
			}*/
		}
		//FTP
		else if (uiAddTask.cb_ftp->isChecked())
		{
			if (uiAddTask.cb_download->isChecked())
			{
				//uiAddTask.lineEdit_url->setText("ftp://10.1.28.101/tools/Git-1.8.1.2-preview20130201.exe");
				uiAddTask.lineEdit_url->setText(QStringLiteral("ftp://ygdy8:ygdy8@yg45.dydytt.net:3161/阳光电影www.ygdy8.com.宝贝儿.HD.1080p.国语中英双字.mp4"));
				uiAddTask.lineEdit_saveDir->setText(getDefaultDownloadDir());
			}
			else if (uiAddTask.cb_upload->isChecked())
			{
				uiAddTask.lineEdit_uploadFile->setText("resources/test/VerComp.dat");
				uiAddTask.lineEdit_url->setText("ftp://10.1.28.101/incoming/QtUploadTest.dat");
			}
			else if (uiAddTask.cb_get->isChecked())
			{
				uiAddTask.lineEdit_url->setText("ftp://10.1.28.101/incoming/UploadTest.dat");
			}
			else if (uiAddTask.cb_put->isChecked())
			{
				uiAddTask.lineEdit_url->setText("ftp://10.1.28.101/incoming/QtPutTest.dat");
			}
		}
	}
}

void NetworkTool::onAddTask()
{
	/*m_nFailedNum = 0;
	m_nSuccessNum = 0;
	m_nTotalNum = 0;
	m_nbytesReceived = 0;
	m_nBytesTotalDownload = 0;
	m_nbytesSent = 0;
	m_nBytesTotalUpload = 0;*/

	if (uiAddTask.cb_download->isChecked())
	{
		onDownload();
	}
	else if (uiAddTask.cb_upload->isChecked())
	{
		onUpload();
	}
	else if (uiAddTask.cb_get->isChecked())
	{
		onGetRequest();
	}
	else if (uiAddTask.cb_post->isChecked())
	{
		onPostRequest();
	}
	else if (uiAddTask.cb_put->isChecked())
	{
		onPutRequest();
	}
	else if (uiAddTask.cb_delete->isChecked())
	{
		onDeleteRequest();
	}
	else if (uiAddTask.cb_head->isChecked())
	{
		onHeadRequest();
	}
	/*else if (uiAddTask.cb_batchDownload->isChecked())
	{
		onBatchDownload();
	}
	else if (uiAddTask.cb_batchMixed->isChecked())
	{
		onBatchMixedTask();
	}*/

	m_pWidgetAddTask->hide();
	uiMain.btn_add->setEnabled(true);
}

void NetworkTool::onAbortTask()
{
	qDebug() << __FUNCTION__ << m_requestId << m_batchId;
	uiMain.btn_abort->setEnabled(false);
	if (m_requestId != 0)
	{
		NetworkManager::globalInstance()->stopRequest(m_requestId);
	}
	if (m_batchId != 0)
	{
		NetworkManager::globalInstance()->stopBatchRequests(m_batchId);
	}
	reset();
}

void NetworkTool::onAbortAllTask()
{
	NetworkManager::globalInstance()->stopAllRequest();
	reset();
}

//////////////////////////////////////////////////////////////////////////
//  以下是调用方法
//////////////////////////////////////////////////////////////////////////
void NetworkTool::onDownload()
{
	QString strUrl = uiAddTask.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strSavePath = uiAddTask.lineEdit_saveDir->text().trimmed();
	if (strSavePath.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("文件保存位置不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	//m_timeStart = QTime::currentTime();
	//m_timeStart.start();
	//appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeDownload;
	request.bShowProgress = uiAddTask.cb_showProgress->isChecked();
	request.strRequestArg = strSavePath;
	if (!uiAddTask.lineEdit_filename->text().isEmpty())
	{
		request.strSaveFileName = uiAddTask.lineEdit_filename->text().trimmed();
	}
	if (uiAddTask.cb_multiDownload->isChecked())
	{
		request.bMultiDownloadMode = true;
		request.nDownloadThreadCount = uiAddTask.cmb_multiDownload->currentText().toInt();
	}

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}

	STTask stTask;
	stTask.strUrl = request.url.toString();
	stTask.eType = request.eType;
	m_pListView->insert(QVariant::fromValue<STTask>(stTask));
	m_pListView->update();
}

void NetworkTool::onUpload()
{
	QString strUrl = uiAddTask.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strUploadFilePath = uiAddTask.lineEdit_uploadFile->text().trimmed();
	if (strUploadFilePath.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("上传文件不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	//m_timeStart = QTime::currentTime();
	//m_timeStart.start();
	//appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeUpload;
	request.strRequestArg = strUploadFilePath; //本地文件路径
	request.bShowProgress = uiAddTask.cb_showProgress->isChecked();

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}

	STTask stTask;
	stTask.strUrl = request.url.toString();
	stTask.eType = request.eType;
	m_pListView->insert(QVariant::fromValue<STTask>(stTask));
	m_pListView->update();
}

void NetworkTool::onGetRequest()
{
	QString strUrl = uiAddTask.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	///m_timeStart = QTime::currentTime();
	//m_timeStart.start();
	//appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeGet;

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}

	STTask stTask;
	stTask.strUrl = request.url.toString();
	stTask.eType = request.eType;
	m_pListView->insert(QVariant::fromValue<STTask>(stTask));
	m_pListView->update();
}

void NetworkTool::onPostRequest()
{
	QString strUrl = uiAddTask.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strArg = uiAddTask.lineEdit_arg->text().trimmed();
	if (strArg.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("参数不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	//m_timeStart = QTime::currentTime();
	//m_timeStart.start();
	//appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypePost;
	request.strRequestArg = strArg;

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}

	STTask stTask;
	stTask.strUrl = request.url.toString();
	stTask.eType = request.eType;
	m_pListView->insert(QVariant::fromValue<STTask>(stTask));
	m_pListView->update();
}

void NetworkTool::onPutRequest()
{
	QString strUrl = uiAddTask.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strUploadFilePath = uiAddTask.lineEdit_uploadFile->text().trimmed();
	if (strUploadFilePath.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("上传文件不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QFile file(strUploadFilePath);
	if (!file.exists() || !file.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("上传文件不存在或者已被占用"), QMessageBox::Ok);
		reset();
		return;
	}
	const QByteArray& bytes = file.readAll();
	file.close();

	//m_timeStart = QTime::currentTime();
	//m_timeStart.start();
	//appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypePut;
	request.strRequestArg = QString::fromUtf8(bytes);

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}

	STTask stTask;
	stTask.strUrl = request.url.toString();
	stTask.eType = request.eType;
	m_pListView->insert(QVariant::fromValue<STTask>(stTask));
	m_pListView->update();
}

void NetworkTool::onDeleteRequest()
{
	QString strUrl = uiAddTask.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	m_timeStart = QTime::currentTime();
	m_timeStart.start();
	appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeDelete;

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}

	STTask stTask;
	stTask.strUrl = request.url.toString();
	stTask.eType = request.eType;
	m_pListView->insert(QVariant::fromValue<STTask>(stTask));
	m_pListView->update();
}

void NetworkTool::onHeadRequest()
{
	QString strUrl = uiAddTask.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	//m_timeStart = QTime::currentTime();
	//m_timeStart.start();
	//appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeHead;

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}

	STTask stTask;
	stTask.strUrl = request.url.toString();
	stTask.eType = request.eType;
	m_pListView->insert(QVariant::fromValue<STTask>(stTask));
	m_pListView->update();
}

void NetworkTool::onBatchRequest()
{
	QString strFile = uiAddBatchTask.lineEdit_config->text().trimmed();
	if (strFile.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("批处理配置文件不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QStringList strlstLine;
	QFile file(strFile);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);
		QTextCodec *codec = QTextCodec::codecForName("UTF-8");
		stream.setCodec(codec);

		QString strLine;
		while (!stream.atEnd())
		{
			strLine = stream.readLine();
			strLine = strLine.trimmed();
			strlstLine += strLine;
		}
		file.close();
	}
	else
	{
		QMessageBox::information(nullptr, "Tips",
								 QStringLiteral("打开批处理配置文件失败！"), QMessageBox::Ok);
		reset();
		return;
	}

	STTask task;
	QVector<QVariant> tasks;
	BatchRequestTask requests;
	RequestTask request;
	QStringList strlst;
	foreach(const QString& strline, strlstLine)
	{
		strlst = strline.split(",");
		if (strlst.size() != 3)
		{
			qDebug() << "error line:" << strline;
			continue;
		}

		const QString& strUrlStandard = QDir::fromNativeSeparators(strlst[0]);
		QString strArg = strlst[2];

		QUrl urlHost(strUrlStandard);
		Q_ASSERT(urlHost.isValid());
		request.url = urlHost;
		request.eType = RequestType(strlst[1].toInt());
		switch (request.eType)
		{
		case eTypeDownload:
			{
				QString strSaveDir = strArg;
				if (strSaveDir.isEmpty())
				{
					strSaveDir = getDefaultDownloadDir();
				}
				if (!strSaveDir.endsWith("/"))
				{
					strSaveDir.append("/");
				}
				QUrl url(strUrlStandard);
				url = url.adjusted(QUrl::RemoveFilename);
				url = url.adjusted(QUrl::RemoveAuthority);
				url = url.adjusted(QUrl::RemoveScheme);
				const QString& strDir = strSaveDir + url.toString();
				request.strRequestArg = strDir;
				request.bShowProgress = uiAddBatchTask.cb_showProgress->isChecked();
				request.bAbortBatchWhileOneFailed = uiAddBatchTask.cb_abortBatch->isChecked();
			}
		case eTypeUpload:
			{
				request.strRequestArg = strArg;
				request.bShowProgress = uiAddBatchTask.cb_showProgress->isChecked();
			}
			break;
		case eTypePost:
		case eTypePut:
			{
				request.strRequestArg = strArg;
			}
			break;
		case eTypeGet:
		case eTypeDelete:
		case eTypeHead:
			break;
		default:
			break;
		}

		task.strUrl = request.url.toString();
		task.eType = request.eType;

#if _MSC_VER >= 1700
		requests.append(std::move(request));
		tasks.append(std::move(QVariant::fromValue(task)));
#else
		requests.append(request);
		tasks.append(QVariant::fromValue(task));
#endif
	}
	m_pListView->insert(tasks);
	m_pListView->update();
	m_nTotalNum = requests.size();

	NetworkReply *pReply = NetworkManager::globalInstance()->addBatchRequest(requests, m_batchId);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}
	m_timeStart = QTime::currentTime();
	m_timeStart.start();
	appendMsg(m_timeStart.toString() + " - Start batch request, uiBatchId["
		+ QString::number(m_batchId) + "] Total[" + QString::number(m_nTotalNum) + "]");
}

void NetworkTool::onBatchMixedTask()
{
#if 0
	//下载
	QStringList strlstUrlDownload;
	strlstUrlDownload.append("http://pic2.52pk.com/files/131112/2255194_1405292P.png");
	strlstUrlDownload.append("http://img.t.388g.com/jzd/201706/149794680474993.jpeg");
	strlstUrlDownload.append("http://img.t.388g.com/jzd/201706/149794740247819.jpeg");
	strlstUrlDownload.append("http://img.t.388g.com/jzd/201706/149794986284124.jpeg");

	BatchRequestTask requests;
	RequestTask request;
	foreach(const QString& strFile, strlstUrlDownload)
	{
		QUrl urlHost(strFile);
		Q_ASSERT(urlHost.isValid());

		request.url = urlHost;
		request.eType = eTypeDownload;
		request.bShowProgress = uiMain.cb_showProgress->isChecked();
		request.strRequestArg = getDefaultDownloadDir();
		request.bAbortBatchWhileOneFailed = uiMain.cb_abortBatch->isChecked();
#if _MSC_VER >= 1700
		requests.append(std::move(request));
#else
		requests.append(request);
#endif
	}

	//上传
	QString strUrl = QString("http://%1:%2/_php/upload.php?filename=upload/%3.jpg")
		.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
	QStringList strlstFileUpload;
	strlstFileUpload.append("resources/test/1.png");
	strlstFileUpload.append("resources/test/2.jpg");
	strlstFileUpload.append("resources/test/3.jpeg");
	int i = 1;
	foreach(const QString& strFile, strlstFileUpload)
	{
		QUrl urlHost(strUrl.arg(i++));
		Q_ASSERT(urlHost.isValid());

		request.url = urlHost;
		request.eType = eTypeUpload;
		request.bShowProgress = uiMain.cb_showProgress->isChecked();
		request.strRequestArg = strFile;
		request.bAbortBatchWhileOneFailed = uiMain.cb_abortBatch->isChecked();
#if _MSC_VER >= 1700
		requests.append(std::move(request));
#else
		requests.append(request);
#endif
	}

	//POST
	QString strArg = "userId=121892674&userName=33CxghNmt1FhAA==&st=QQBnAEEAQQBBAEUAT"
		"AB2AFEAdwBjAEEAQQBBAEEAQQBBAEEAQQBBAEEATAB2AFAANwBoAE4AcwBJ"
		"AC8AbwBWAFMAQQArAEQAVgBIADIAdgAyAHcARgBRAGYANABJAHkAOQA3AFAAYQBkAFMARwBoAEoA"
		"KwBUAEoAcAAzADkAVgBYAFYAMwBDAE4AVABiAHEAZQB3AE4AMAANAAoAOABlAHUANQBBAHMAUwBY"
		"AFEAbQAyAFUAWQBmAHEAMgA1ADkAcQBvAG4AZQBCAFEAYgB5AE8ANwAyAFQAMQB0AGwARwBIADYA"
		"dAB1AGYAYQBxAEoAMwBnAFUARwA4AGoAdQA5AGsAOQBzAFoAYQB1AHAARwBjAE8ANABnADIAegBn"
		"ADIANgB1AEcANwBoAHAAUwBHADIAVQANAAoAWQBmAHEAMgA1ADkAcQBvAG4AZQBCAFEAYgB5AE8A"
		"NwAyAFQAMAA9AA==";

	QUrl urlHost("https://passportservice.7fgame.com/HttpService/PlatService.ashx");
	Q_ASSERT(urlHost.isValid());
	request.url = urlHost;
	request.eType = eTypePost;
	request.strRequestArg = strArg;
	request.bAbortBatchWhileOneFailed = uiMain.cb_abortBatch->isChecked();
#if _MSC_VER >= 1700
	requests.append(std::move(request));
#else
	requests.append(request);
#endif

	//DELETE
	strUrl = QString("http://%1:%2/_php/delete.php?filename=upload/1.jpg")
		.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
	urlHost = QUrl(strUrl);
	Q_ASSERT(urlHost.isValid());
	request.url = urlHost;
	request.eType = eTypeDelete;
	request.bAbortBatchWhileOneFailed = uiMain.cb_abortBatch->isChecked();
#if _MSC_VER >= 1700
	requests.append(std::move(request));
#else
	requests.append(request);
#endif

	m_nTotalNum = requests.size();

	NetworkReply *pReply = NetworkManager::globalInstance()->addBatchRequest(requests, m_batchId);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
				this, SLOT(onRequestFinished(const RequestTask &)));
	}
	m_timeStart = QTime::currentTime();
	m_timeStart.start();
	appendMsg(m_timeStart.toString() + " - Start batch request, uiBatchId["
			  + QString::number(m_batchId) + "] Total[" + QString::number(m_nTotalNum) + "]");
#endif
}

//request:		任务信息
//bSuccess：	任务是否成功
void NetworkTool::onRequestFinished(const RequestTask &request)
{
	bool bBatch = (request.uiBatchId > 0);
	if (request.bSuccess)
	{
		m_nSuccessNum++;
		if (bBatch)
		{
			qDebug() << "Success[" + QString::number(m_nSuccessNum) + "]" << request.url.url();
		}
	}
	else  //下载成功后的文件(文件绝对路径：request.strFileSavePath + 文件名：request.url.filename())
	{
		m_nFailedNum++;
		if (bBatch)
		{
			qDebug() << "Failed[" + QString::number(m_nFailedNum) + "]" << request.url.url();
		}
	}
	appendMsg(request.bytesContent, false);

	//批量请求
	if (bBatch)
	{
		if (m_nSuccessNum + m_nFailedNum == m_nTotalNum
			|| (request.bAbortBatchWhileOneFailed && m_nFailedNum > 0))
		{
			appendMsg("Batch request finished. Total["
					  + QString::number(m_nTotalNum) + "] Success[" + QString::number(m_nSuccessNum)
					  + "] Failed[" + QString::number(m_nFailedNum) + "]");

			int msec = m_timeStart.elapsed();
			float sec = (float)msec / 1000;
			QString strMsg = QString("Time elapsed: %1s.").arg(sec);
			appendMsg(strMsg);
			reset();
		}
	}
	else //单任务
	{
		appendMsg("Request finished. Success["
				  + QString::number(request.bSuccess) + "] url[" + request.url.url() + "]");

		int msec = m_timeStart.elapsed();
		float sec = (float)msec / 1000;
		quint64 uiSpeed = 0;
		if (sec > 0)
		{
			if (request.bSuccess && request.eType == eTypeDownload && m_nBytesTotalDownload > 0)
			{
				uiSpeed = m_nBytesTotalDownload / sec;

			}
			else if (request.bSuccess && request.eType == eTypeUpload && m_nBytesTotalUpload > 0)
			{
				uiSpeed = m_nBytesTotalUpload / sec;
			}
		}

		QString strMsg = QString("Time elapsed: %1s.").arg(sec);
		if (uiSpeed > 0)
		{
			strMsg = QString("Time elapsed: %1s. Speed: %2/s")
				.arg(sec).arg(bytes2String(uiSpeed));
		}

		appendMsg(strMsg);
		reset();
	}
}

void NetworkTool::onDownloadProgress(quint64 taskId, qint64 bytesReceived, qint64 bytesTotal)
{
	if (bytesReceived > m_nbytesReceived)
	{
		m_nbytesReceived = bytesReceived;
		if (bytesTotal != m_nBytesTotalDownload)
		{
			m_nBytesTotalDownload = bytesTotal;
			//uiMain.progressBar_d->setMaximum(bytesTotal);
		}
		//uiMain.progressBar_d->setValue(bytesReceived);
	}
}

void NetworkTool::onUploadProgress(quint64 taskId, qint64 bytesSent, qint64 bytesTotal)
{
	if (bytesSent > m_nbytesSent)
	{
		m_nbytesSent = bytesSent;
		if (bytesTotal != m_nBytesTotalUpload)
		{
			m_nBytesTotalUpload = bytesTotal;
			//uiMain.progressBar_u->setMaximum(bytesTotal);
		}
		//uiMain.progressBar_u->setValue(bytesSent);
	}
}

void NetworkTool::onBatchDownloadProgress(quint64 batchId, qint64 bytes)
{
	//const QString& str = bytes2String(bytes);
	appendMsg(QStringLiteral("批任务[%1]   下载：%2").arg(batchId).arg(bytes), false);
}

void NetworkTool::onBatchUploadProgress(quint64 batchId, qint64 bytes)
{
	//const QString& str = bytes2String(bytes);
	appendMsg(QStringLiteral("批任务[%1]   上传：%2").arg(batchId).arg(bytes), false);
}

void NetworkTool::onErrorMessage(const QString& error)
{
	appendMsg(QTime::currentTime().toString() + " - error: " + error);
}

void NetworkTool::onGetSaveDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
													"/home",
													QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (!dir.isNull() && !dir.isEmpty())
	{
		uiAddTask.lineEdit_saveDir->setText(dir);
	}
}

void NetworkTool::onGetUploadFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
													"/desktop",
													tr("File (*.*)"));

	if (!fileName.isNull() && !fileName.isEmpty())
	{
		uiAddTask.lineEdit_uploadFile->setText(fileName);
	}
}

void NetworkTool::onGetBatchTaskConfigFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/desktop",
		tr("File (*.dat)"));

	if (!fileName.isNull() && !fileName.isEmpty())
	{
		uiAddBatchTask.lineEdit_config->setText(fileName);
	}
}

void NetworkTool::reset()
{
	uiMain.btn_add->setEnabled(true);
	uiMain.btn_addBatch->setEnabled(true);

	m_nFailedNum = 0;
	m_nSuccessNum = 0;
	m_nTotalNum = 0;
	m_nbytesReceived = 0;
	m_nBytesTotalDownload = 0;
	m_nbytesSent = 0;
	m_nBytesTotalUpload = 0;
}

QString NetworkTool::bytes2String(qint64 bytes)
{
	QString str;
	if (bytes < 1024)
	{
		str = QString("%1B").arg(bytes);
	}
	else if (bytes < 1024 * 1024)
	{
		bytes = bytes / 1024;
		str = QString("%1KB").arg(bytes);
	}
	else if (bytes < 1024 * 1024 * 1024)
	{
		qreal dSize = (qreal)bytes / 1024 / 1024;
		char ch[8] = {0};
		sprintf(ch, "%.2f", dSize);
		str = QString("%1MB").arg(ch);
	}
	else
	{
		qreal dSize = (qreal)bytes / 1024 / 1024 / 1024;
		char ch[8] = {0};
		sprintf(ch, "%.2f", dSize);
		str = QString("%1GB").arg(ch);
	}
	return str;
}

void NetworkTool::appendMsg(const QString& strMsg, bool bQDebug)
{
	if (!strMsg.isEmpty())
	{
		if (bQDebug)
		{
			qDebug() << strMsg;
		}

		uiMain.textEdit_output->append(strMsg);
		uiMain.textEdit_output->append("");
	}
}

QString NetworkTool::getDefaultDownloadDir()
{
	const QStringList& lstDir = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
	if (!lstDir.isEmpty())
	{
		return lstDir[0];
	}
	return QLatin1String("download/");
}

//////////////////////////////////////////////////////////////////////////
template<typename T, typename TBase> class ClassIsDerived
{
public:
	static int t(TBase* base)
	{
		return 1;
	}

	static char t(void* t2)
	{
		return 0;
	}

	enum
	{
		Result = (sizeof(int) == sizeof(t((T*)NULL))),
	};
};

//////////////////////////////////////////////////////////////////////////
TaskDelegate::TaskDelegate(QObject* parent /*= NULL*/)
	: Delegate(parent)
{

}

TaskDelegate::~TaskDelegate()
{

}

QSize TaskDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QSize(310, 60);
}

void TaskDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	painter->save();
	if (index.isValid())
	{
		QRect rect = option.rect;
		if (option.state & QStyle::State_MouseOver)
		{
			painter->fillRect(rect, QColor("#808080"));
		}
		else
		{
			painter->fillRect(rect, QColor("#333333"));
		}

		QVariant var = index.data(Qt::DisplayRole);
		if (var.isValid())
		{
			STTask stTask = qvariant_cast<STTask>(var);

			QFont font;
			font.setFamily("Microsoft YaHei");
			font.setPixelSize(12);
			painter->setFont(font);
			painter->setPen(Qt::white);

			painter->drawText(QRect(rect.left(), rect.top(), 250, 14), stTask.strUrl, QTextOption(Qt::AlignLeft|Qt::AlignVCenter));
		}
	}
	painter->restore();
}

bool TaskDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
						   const QStyleOptionViewItem &option, const QModelIndex &index)
{
	return __super::editorEvent(event, model, option, index);
}