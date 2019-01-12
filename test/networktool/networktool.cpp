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
#include "networktool.h"
#include "NetworkManager.h"
#include "NetworkReply.h"

#define QMT_POST_TEST
#define DEFAULT_CONCURRENT_TASK 4
#define DEFAULT_MTDOWNLOAD_COUNT 5
//局域网Apache http服务器
#define HTTP_SERVER_IP "127.0.0.1"
#define HTTP_SERVER_PORT "80"


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
	ui.setupUi(this);
	setFixedSize(700, 620);
	setWindowTitle(QStringLiteral("Qt Network Tool"));

	ui.progressBar_d->setFormat("%p%(%v / %m)");
	ui.progressBar_u->setFormat("%p%(%v / %m)");

	ui.cmb_concurrentTask->setView(new QListView(this));
	ui.cmb_multiDownload->setView(new QListView(this));
	for (int i = 1; i <= 8; ++i)
	{
		ui.cmb_concurrentTask->addItem(QString::number(i));
	}
	for (int i = 2; i <= 10; ++i)
	{
		ui.cmb_multiDownload->addItem(QString::number(i));
	}
	ui.cmb_concurrentTask->setCurrentText(QString::number(DEFAULT_CONCURRENT_TASK));
	ui.cmb_multiDownload->setCurrentText(QString::number(DEFAULT_MTDOWNLOAD_COUNT));

	QButtonGroup *bg1 = new QButtonGroup(this);
	bg1->addButton(ui.cb_http);
	bg1->addButton(ui.cb_ftp);
	bg1->setExclusive(true);
	bg1->metaObject()->className();

	QButtonGroup *bg2 = new QButtonGroup(this);
	bg2->addButton(ui.cb_download);
	bg2->addButton(ui.cb_upload);
	bg2->addButton(ui.cb_get);
	bg2->addButton(ui.cb_post);
	bg2->addButton(ui.cb_put);
	bg2->addButton(ui.cb_delete);
	bg2->addButton(ui.cb_head);
	bg2->addButton(ui.cb_batchDownload);
	bg2->addButton(ui.cb_batchMixed);
	bg2->setExclusive(true);

#ifndef _DEBUG
	ui.cb_batchMixed->hide();
#endif

	connect(ui.btn_start, SIGNAL(clicked()), this, SLOT(onStartTask()));
	connect(ui.btn_abort, SIGNAL(clicked()), this, SLOT(onAbortTask()));
	connect(ui.btn_browser1, SIGNAL(clicked()), this, SLOT(onGetSaveDirectory()));
	connect(ui.btn_browser2, SIGNAL(clicked()), this, SLOT(onGetUploadFile()));
	connect(ui.cb_useDefault, &QAbstractButton::toggled, this, [=](bool checked) {
		if (checked)
		{
			onUpdateDefaultInfos();
		}
	});
	connect(ui.cmb_concurrentTask, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
		this, [=](const QString &strText) {
		int num = strText.toInt();
		if (num >= 1 && num <= 8)
		{
			NetworkManager::globalInstance()->setMaxThreadCount(num);
		}
	});
	connect(bg1, SIGNAL(buttonToggled(int, bool)), this, SLOT(onUpdateDefaultInfos()));
	connect(bg2, SIGNAL(buttonToggled(int, bool)), this, SLOT(onUpdateDefaultInfos()));

	initialize();
	onUpdateDefaultInfos();
}

NetworkTool::~NetworkTool()
{
	unIntialize();
}

void NetworkTool::initialize()
{
	NetworkManager::initialize();
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

void NetworkTool::unIntialize()
{
	NetworkManager::unInitialize();
}

void NetworkTool::onUpdateDefaultInfos()
{
	ui.lineEdit_url->clear();
	ui.lineEdit_arg->clear();
	ui.lineEdit_filename->clear();
	ui.lineEdit_saveDir->clear();
	ui.lineEdit_uploadFile->clear();

	if (ui.cb_useDefault->isChecked())
	{
		//HTTP
		if (ui.cb_http->isChecked())
		{
			if (ui.cb_download->isChecked())
			{
				QString strUrl = "https://cdn.mysql.com//Downloads/MySQL-8.0/mysql-8.0.13-winx64.zip";
				//QString strUrl = "http://8dx.pc6.com/xzx6/curl_v7.61.1.zip";
				ui.lineEdit_url->setText(strUrl);
				ui.lineEdit_saveDir->setText(getDefaultDownloadDir());
			}
			else if (ui.cb_upload->isChecked())
			{
				QString strUrl = QString("http://%1:%2/_php/upload.php?filename=upload/VerComp_qt.dat")
					.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
				ui.lineEdit_url->setText(strUrl);
				ui.lineEdit_uploadFile->setText("resources/test/VerComp.dat");
			}
			else if (ui.cb_get->isChecked())
			{
				ui.lineEdit_url->setText(QStringLiteral("http://m.kugou.com/singer/list/88?json=true"));
			}
			else if (ui.cb_post->isChecked())
			{
				QString strArg = "userId=121892674&userName=33CxghNmt1FhAA==&st=QQBnAEEAQQBBAEUATA"
					"B2AFEAdwBjAEEAQQBBAEEAQQBBAEEAQQBBAEEATAB2AFAANwBoAE4AcwBJ"
					"AC8AbwBWAFMAQQArAEQAVgBIADIAdgAyAHcARgBRAGYANABJAHkAOQA3AFAAYQBkAFMARwBoAEoA"
					"KwBUAEoAcAAzADkAVgBYAFYAMwBDAE4AVABiAHEAZQB3AE4AMAANAAoAOABlAHUANQBBAHMAUwBY"
					"AFEAbQAyAFUAWQBmAHEAMgA1ADkAcQBvAG4AZQBCAFEAYgB5AE8ANwAyAFQAMQB0AGwARwBIADYA"
					"dAB1AGYAYQBxAEoAMwBnAFUARwA4AGoAdQA5AGsAOQBzAFoAYQB1AHAARwBjAE8ANABnADIAegBn"
					"ADIANgB1AEcANwBoAHAAUwBHADIAVQANAAoAWQBmAHEAMgA1ADkAcQBvAG4AZQBCAFEAYgB5AE8A"
					"NwAyAFQAMAA9AA==";

				ui.lineEdit_url->setText("https://passportservice.7fgame.com/HttpService/PlatService.ashx");
				ui.lineEdit_arg->setText(strArg);
			}
			else if (ui.cb_put->isChecked())
			{
				QString strUrl = QString("http://%1:%2/_php/upload.php?filename=upload/VerComp_qt.dat")
					.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
				ui.lineEdit_url->setText(strUrl);
				ui.lineEdit_uploadFile->setText("resources/test/VerComp.dat");
			}
			else if (ui.cb_delete->isChecked())
			{
				QString strUrl = QString("http://%1:%2/_php/delete.php?filename=upload/1.jpg")
					.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
				ui.lineEdit_url->setText(strUrl);
			}
			else if (ui.cb_head->isChecked())
			{
				ui.lineEdit_url->setText("http://forspeed.onlinedown.net/down/warcraftiii1.24bbzh.rar");
			}
			else if (ui.cb_batchDownload->isChecked())
			{
				ui.lineEdit_url->setText("resources/test/VerComp.dat");
				ui.lineEdit_saveDir->setText(getDefaultDownloadDir());
			}
		}
		//FTP
		else if (ui.cb_ftp->isChecked())
		{
			if (ui.cb_download->isChecked())
			{
				//ui.lineEdit_url->setText("ftp://10.1.28.101/tools/Git-1.8.1.2-preview20130201.exe");
				ui.lineEdit_url->setText(QStringLiteral("ftp://ygdy8:ygdy8@yg45.dydytt.net:3161/阳光电影www.ygdy8.com.宝贝儿.HD.1080p.国语中英双字.mp4"));
				ui.lineEdit_saveDir->setText(getDefaultDownloadDir());
			}
			else if (ui.cb_upload->isChecked())
			{
				ui.lineEdit_uploadFile->setText("resources/test/VerComp.dat");
				ui.lineEdit_url->setText("ftp://10.1.28.101/incoming/QtUploadTest.dat");
			}
			else if (ui.cb_get->isChecked())
			{
				ui.lineEdit_url->setText("ftp://10.1.28.101/incoming/UploadTest.dat");
			}
			else if (ui.cb_put->isChecked())
			{
				ui.lineEdit_url->setText("ftp://10.1.28.101/incoming/QtPutTest.dat");
			}
		}
	}
}

void NetworkTool::onStartTask()
{
	m_nFailedNum = 0;
	m_nSuccessNum = 0;
	m_nTotalNum = 0;
	m_nbytesReceived = 0;
	m_nBytesTotalDownload = 0;
	m_nbytesSent = 0;
	m_nBytesTotalUpload = 0;
	ui.progressBar_d->setValue(0);
	ui.progressBar_u->setValue(0);
	ui.btn_start->setEnabled(false);
	ui.btn_abort->setEnabled(true);

	if (ui.cb_download->isChecked())
	{
		onDownload();
	}
	else if (ui.cb_upload->isChecked())
	{
		onUpload();
	}
	else if (ui.cb_get->isChecked())
	{
		onGetRequest();
	}
	else if (ui.cb_post->isChecked())
	{
		onPostRequest();
	}
	else if (ui.cb_put->isChecked())
	{
		onPutRequest();
	}
	else if (ui.cb_delete->isChecked())
	{
		onDeleteRequest();
	}
	else if (ui.cb_head->isChecked())
	{
		onHeadRequest();
	}
	else if (ui.cb_batchDownload->isChecked())
	{
		onBatchDownload();
	}
	else if (ui.cb_batchMixed->isChecked())
	{
		onBatchMixedTask();
	}
}

void NetworkTool::onAbortTask()
{
	qDebug() << __FUNCTION__ << m_requestId << m_batchId;
	ui.btn_abort->setEnabled(false);
#if 1
	if (m_requestId != 0)
	{
		NetworkManager::globalInstance()->stopRequest(m_requestId);
	}
	if (m_batchId != 0)
	{
		NetworkManager::globalInstance()->stopBatchRequests(m_batchId);
	}
#else
	NetworkManager::globalInstance()->stopAllRequest();
#endif
	reset();
}

//////////////////////////////////////////////////////////////////////////
//  以下是调用方法
//////////////////////////////////////////////////////////////////////////
void NetworkTool::onDownload()
{
	m_nTotalNum = 1;

	QString strUrl = ui.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strSavePath = ui.lineEdit_saveDir->text().trimmed();
	if (strSavePath.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("文件保存位置不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	m_timeStart = QTime::currentTime();
	appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeDownload;
	request.bShowProgress = ui.cb_showProgress->isChecked();
	request.strRequestArg = strSavePath;
	if (!ui.lineEdit_filename->text().isEmpty())
	{
		request.strSaveFileName = ui.lineEdit_filename->text().trimmed();
	}
	if (ui.cb_multiDownload->isChecked())
	{
		request.bMultiDownloadMode = true;
		request.nDownloadThreadCount = ui.cmb_multiDownload->currentText().toInt();
	}

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));
	}
}

void NetworkTool::onUpload()
{
	m_nTotalNum = 1;

	QString strUrl = ui.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strUploadFilePath = ui.lineEdit_uploadFile->text().trimmed();
	if (strUploadFilePath.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("上传文件不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	m_timeStart = QTime::currentTime();
	appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeUpload;
	request.strRequestArg = strUploadFilePath; //本地文件路径
	request.bShowProgress = ui.cb_showProgress->isChecked();

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));
	}
}

void NetworkTool::onGetRequest()
{
	m_nTotalNum = 1;

	QString strUrl = ui.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	m_timeStart = QTime::currentTime();
	appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

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
}

void NetworkTool::onPostRequest()
{
	m_nTotalNum = 1;

	QString strUrl = ui.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strArg = ui.lineEdit_arg->text().trimmed();
	if (strArg.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("参数不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	m_timeStart = QTime::currentTime();
	appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypePost;
	request.strRequestArg = strArg;

#ifdef QMT_POST_TEST
	BatchRequestTask requests;
	for (int i = 0; i < 1000; ++i)
	{
		request.bAbortBatchWhileOneFailed = false;
		requests.append(request);
	}
	m_nTotalNum = requests.size();
	NetworkReply *pReply = NetworkManager::globalInstance()->addBatchRequest(requests, m_batchId);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));
	}
#else
	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		m_requestId = request.uiId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));
	}
#endif
}

void NetworkTool::onPutRequest()
{
	m_nTotalNum = 1;

	QString strUrl = ui.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strUploadFilePath = ui.lineEdit_uploadFile->text().trimmed();
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

	m_timeStart = QTime::currentTime();
	appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

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
}

void NetworkTool::onDeleteRequest()
{
	m_nTotalNum = 1;

	QString strUrl = ui.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	m_timeStart = QTime::currentTime();
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
}

void NetworkTool::onHeadRequest()
{
	m_nTotalNum = 1;

	QString strUrl = ui.lineEdit_url->text().trimmed();
	if (strUrl.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	m_timeStart = QTime::currentTime();
	appendMsg(m_timeStart.toString() + " - Start request[" + strUrl + "]");

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
}

void NetworkTool::onBatchDownload()
{
	QString strFile = ui.lineEdit_url->text().trimmed();
	if (strFile.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
		reset();
		return;
	}

	QStringList strlstUrl;
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
			strlstUrl += strLine;
		}
		file.close();
	}
	else
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("文件打开失败！"), QMessageBox::Ok);
		reset();
		return;
	}

	QString strSaveDir = ui.lineEdit_saveDir->text().trimmed();
	if (strSaveDir.isEmpty())
	{
		strSaveDir = getDefaultDownloadDir();
	}
	if (!strSaveDir.endsWith("/"))
	{
		strSaveDir.append("/");
	}

	BatchRequestTask requests;
	RequestTask request;
	foreach(const QString& strUrl, strlstUrl)
	{
		const QString& strUrlStandard = QDir::fromNativeSeparators(strUrl);
		QUrl url(strUrlStandard);
		url = url.adjusted(QUrl::RemoveFilename);
		url = url.adjusted(QUrl::RemoveAuthority);
		url = url.adjusted(QUrl::RemoveScheme);

		const QString& strDir = strSaveDir + url.toString();

		QUrl urlHost(strUrlStandard);
		Q_ASSERT(urlHost.isValid());
		request.url = urlHost;
		request.eType = eTypeDownload;
		request.bShowProgress = ui.cb_showProgress->isChecked();
		request.strRequestArg = strDir;
		request.bAbortBatchWhileOneFailed = ui.cb_abortBatch->isChecked();

#if _MSC_VER >= 1700
		requests.append(std::move(request));
#else
		requests.append(request);
#endif
	}
	m_nTotalNum = requests.size();

	NetworkReply *pReply = NetworkManager::globalInstance()->addBatchRequest(requests, m_batchId);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));
	}
	m_timeStart = QTime::currentTime();
	appendMsg(m_timeStart.toString() + " - Start batch request. Batch id: "
		+ QString::number(m_batchId) + ", Total: " + QString::number(m_nTotalNum));
}

void NetworkTool::onBatchMixedTask()
{
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
		request.bShowProgress = ui.cb_showProgress->isChecked();
		request.strRequestArg = getDefaultDownloadDir();
		request.bAbortBatchWhileOneFailed = ui.cb_abortBatch->isChecked();
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
		request.bShowProgress = ui.cb_showProgress->isChecked();
		request.strRequestArg = strFile;
		request.bAbortBatchWhileOneFailed = ui.cb_abortBatch->isChecked();
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
	request.bAbortBatchWhileOneFailed = ui.cb_abortBatch->isChecked();
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
	request.bAbortBatchWhileOneFailed = ui.cb_abortBatch->isChecked();
#if _MSC_VER >= 1700
	requests.append(std::move(request));
#else
	requests.append(request);
#endif

	m_nTotalNum = requests.size();

	quint64 uiBatchId = 0;
	NetworkReply *pReply = NetworkManager::globalInstance()->addBatchRequest(requests, uiBatchId);
	if (nullptr != pReply)
	{
		m_batchId = uiBatchId;
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));
	}
	m_timeStart = QTime::currentTime();
	appendMsg(m_timeStart.toString() + " - Start batch request, uiBatchId: "
		+ QString::number(uiBatchId) + ", Total: " + QString::number(m_nTotalNum));
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
			//qDebug() << "Success[" + QString::number(m_nSuccessNum) + "]" << request.url.url();
		}
	}
	else  //下载成功后的文件(文件绝对路径：request.strFileSavePath + 文件名：request.url.filename())
	{
		m_nFailedNum++;
		if (bBatch)
		{
			//qDebug() << "Failed[" + QString::number(m_nFailedNum) + "]" << request.url.url();
		}
	}
	appendMsg(request.bytesContent, false);

	//批处理请求
	if (bBatch)
	{
		if (m_nSuccessNum + m_nFailedNum == m_nTotalNum
			|| (ui.cb_abortBatch->isChecked() && m_nFailedNum > 0))
		{
			QTime time = QTime::currentTime();
			appendMsg(time.toString() + " - Batch request finished. Total["
				+ QString::number(m_nTotalNum) + "] Success[" + QString::number(m_nSuccessNum)
				+ "] Failed[" + QString::number(m_nFailedNum) + "]");

			int msec = m_timeStart.msecsTo(time);
			float sec = (float)msec / 1000;
			QString strMsg = QString("Time elapsed: %1s.").arg(sec);
			appendMsg(strMsg);
			reset();
		}
	}
	else //单任务
	{
		QTime time = QTime::currentTime();
		int msec = m_timeStart.msecsTo(time);

		appendMsg(time.toString() + " - Request finished. Success["
			+ QString::number(request.bSuccess) + "] url[" + request.url.url() + "]");

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
			m_strTotalDownload = bytes2String(bytesTotal);
			ui.progressBar_d->setMaximum(bytesTotal);
		}
		ui.progressBar_d->setValue(bytesReceived);

		/*const QString& strReceived = bytes2String(bytesReceived);
		appendMsg(QStringLiteral("任务：%1   下载：%2 / %3")
		.arg(taskId).arg(strReceived).arg(m_strTotalDownload), false);*/
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
			m_strTotalUpload = bytes2String(bytesTotal);
			ui.progressBar_u->setMaximum(bytesTotal);
		}
		ui.progressBar_u->setValue(bytesSent);

		/*const QString& strSent = bytes2String(bytesSent);
		appendMsg(QStringLiteral("任务：%1   上传：%2 / %3")
		.arg(taskId).arg(strSent).arg(m_strTotalUpload), false);*/
	}
}

void NetworkTool::onBatchDownloadProgress(quint64 batchId, qint64 bytes)
{
	const QString& str = bytes2String(bytes);
	appendMsg(QStringLiteral("批任务[%1]   下载：%2").arg(batchId).arg(str), false);
}

void NetworkTool::onBatchUploadProgress(quint64 batchId, qint64 bytes)
{
	const QString& str = bytes2String(bytes);
	appendMsg(QStringLiteral("批任务[%1]   上传：%2").arg(batchId).arg(str), false);
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
		ui.lineEdit_saveDir->setText(dir);
	}
}

void NetworkTool::onGetUploadFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/home",
		tr("File (*.*)"));

	if (!fileName.isNull() && !fileName.isEmpty())
	{
		ui.lineEdit_uploadFile->setText(fileName);
	}
}

void NetworkTool::reset()
{
	ui.btn_start->setEnabled(true);
	ui.btn_abort->setEnabled(false);

	m_nFailedNum = 0;
	m_nSuccessNum = 0;
	m_nTotalNum = 0;
	m_nbytesReceived = 0;
	m_nBytesTotalDownload = 0;
	m_nbytesSent = 0;
	m_nBytesTotalUpload = 0;
	ui.progressBar_d->setMaximum(100);
	ui.progressBar_d->setValue(0);
	ui.progressBar_u->setMaximum(100);
	ui.progressBar_u->setValue(0);
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

		ui.textEdit_output->append(strMsg);
		ui.textEdit_output->append("");
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