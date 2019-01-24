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
#include <QScrollBar>
#include "networktool.h"
#include "NetworkManager.h"
#include "NetworkReply.h"

#define DEFAULT_CONCURRENT_TASK 4
#define DEFAULT_MTDOWNLOAD_COUNT 5
//局域网Apache http服务器
#define HTTP_SERVER_IP "127.0.0.1"
#define HTTP_SERVER_PORT "80"
#define TASKING_TEXT_FORMAT QStringLiteral("  执行中的任务(%1)")
#define FINISHED_TEXT_FORMAT QStringLiteral("  已完成的任务(%1)")


NetworkTool::NetworkTool(QWidget *parent)
	: QMainWindow(parent)
	, m_nbytesReceived(0)
	, m_nBytesTotalDownload(0)
	, m_nbytesSent(0)
	, m_nBytesTotalUpload(0)
{
	uiMain.setupUi(this);
	setFixedSize(700, 510);
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

	m_pWidgetAddTask = new QWidget(this);
	uiAddTask.setupUi(m_pWidgetAddTask);
	m_pWidgetAddTask->hide();

	m_pWidgetAddBatch = new QWidget(this);
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

	m_pModelDoing = new TaskModel(this);
	m_pModelFinished = new TaskModel(this);
	m_pDelegate = new TaskDelegate(this);

	m_pListViewDoing = new TaskListView(this);
	m_pListViewDoing->setGeometry(0, 105, 320, 350);
	m_pListViewDoing->setListModel(m_pModelDoing);
	m_pListViewDoing->setListDelegate(m_pDelegate);
	m_pListViewDoing->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pListViewDoing->setStyleSheet("QListView{background:transparent;}\
			QProgressBar{border:1px solid #808080;background-color:#191919;}");
	m_pListViewDoing->verticalScrollBar()->setStyleSheet("QWidget{border:1px solid #808080;border-radius:5%;background-color:#333333;color:#DBDBDB;}\
		QScrollBar{border:0px;background-color:#292929;width:12px;}\
		QScrollBar::handle{ border:1px solid #636363; background-color:#333333; margin: 16px 0px 16px 0px; min-height:30px; }\
		QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{background: transparent;}");
	m_pListViewDoing->hide();

	m_pListViewFinished = new TaskListView(this);
	m_pListViewFinished->setGeometry(0, 127, 320, 330);
	m_pListViewFinished->setListModel(m_pModelFinished);
	m_pListViewFinished->setListDelegate(m_pDelegate);
	m_pListViewFinished->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pListViewFinished->setStyleSheet(m_pListViewDoing->styleSheet());
	m_pListViewFinished->verticalScrollBar()->setStyleSheet(m_pListViewDoing->verticalScrollBar()->styleSheet());

	m_pLblTasking = new LabelEx(this);
	m_pLblTasking->setGeometry(0, 80, 320, 20);
	m_pLblTasking->setText(TASKING_TEXT_FORMAT.arg(0));
	m_pLblTasking->setStyleSheet("QLabel{background-color:#636363;color:#DBDBDB;}");

	m_pLblFinished = new LabelEx(this);
	m_pLblFinished->setGeometry(0, 102, 320, 20);
	m_pLblFinished->setText(FINISHED_TEXT_FORMAT.arg(0));
	m_pLblFinished->setStyleSheet("QLabel{background-color:#636363;color:#DBDBDB;}");
}

void NetworkTool::initConnecting()
{
	connect(uiMain.btn_add, &QPushButton::clicked, m_pWidgetAddTask, [=] {
		m_pWidgetAddTask->move(this->width() / 2 - m_pWidgetAddTask->width() / 2,
			this->height() / 2 - m_pWidgetAddTask->height() / 2);
		m_pWidgetAddTask->show();
		m_pWidgetAddTask->raise();
	});
	connect(uiMain.btn_addBatch, &QPushButton::clicked, m_pWidgetAddBatch, [=] {
		m_pWidgetAddBatch->move(this->width() / 2 - m_pWidgetAddBatch->width() / 2,
			this->height() / 2 - m_pWidgetAddBatch->height() / 2);
		m_pWidgetAddBatch->show();
		m_pWidgetAddBatch->raise();
	});
	connect(uiMain.btn_abort, SIGNAL(clicked()), this, SLOT(onAbortTask()));
	connect(uiMain.btn_abortAll, SIGNAL(clicked()), this, SLOT(onAbortAllTask()));
	connect(uiAddTask.btn_browser1, SIGNAL(clicked()), this, SLOT(onGetSaveDirectory()));
	connect(uiAddTask.btn_browser2, SIGNAL(clicked()), this, SLOT(onGetUploadFile()));
	connect(uiAddBatchTask.btn_browser, SIGNAL(clicked()), this, SLOT(onGetBatchTaskConfigFile()));
	connect(uiAddTask.btn_start, SIGNAL(clicked()), this, SLOT(onAddTask()));
	connect(uiAddBatchTask.btn_start, SIGNAL(clicked()), this, SLOT(onBatchRequest()));
	connect(uiAddTask.btn_close, SIGNAL(clicked()), m_pWidgetAddTask, SLOT(hide()));
	connect(uiAddBatchTask.btn_close, SIGNAL(clicked()), m_pWidgetAddBatch, SLOT(hide()));
	connect(uiAddTask.cb_useDefault, &QAbstractButton::toggled, this, [=](bool checked) {
		if (checked)
		{
			onUpdateDefaultValue();
		}
	});
	connect(uiAddBatchTask.btn_help, &QAbstractButton::clicked, this, [=]() {
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("请参考默认的批处理请求配置文件。文件位置（工程目录/test/networktool/help/Vercomp2.dat）"), QMessageBox::Ok);
	});
	connect(uiMain.cmb_concurrentTask, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
		this, [=](const QString &strText) {
		int num = strText.toInt();
		if (num >= 1 && num <= 8)
		{
			NetworkManager::globalInstance()->setMaxThreadCount(num);
		}
	});
	connect(m_pListViewDoing, &QAbstractItemView::clicked, this, [=](const QModelIndex &index) {
		if (index.isValid())
		{
			const QVariant& var = index.data(Qt::DisplayRole);
			if (var.isValid())
			{
				const RequestTask& stTask = qvariant_cast<RequestTask>(var);
				if (stTask.uiId > 0 && !stTask.bCancel && !stTask.bFinished)
				{
					uiMain.btn_abort->setEnabled(true);
				}
				else
				{
					uiMain.btn_abort->setEnabled(false);
				}
			}
		}
	});
	connect(m_pListViewDoing, &TaskListView::taskFinished, this, [=](const QVariant& var) {
		if (var.isValid())
		{
			m_pListViewFinished->insert(var);
		}
	});
	connect(m_pModelDoing, &ListModel::sizeChanged, this, [=](int size) {
		m_pLblTasking->setText(TASKING_TEXT_FORMAT.arg(size));
	});
	connect(m_pModelFinished, &ListModel::sizeChanged, this, [=](int size) {
		m_pLblFinished->setText(FINISHED_TEXT_FORMAT.arg(size));
	});
	connect(m_pLblTasking, &LabelEx::dbClicked, this, [=]() {
		switchTaskView();
	});
	connect(m_pLblFinished, &LabelEx::dbClicked, this, [=]() {
		switchTaskView();
	});
	connect(bg_protocal, SIGNAL(buttonToggled(int, bool)), this, SLOT(onUpdateDefaultValue()));
	connect(bg_type, SIGNAL(buttonToggled(int, bool)), this, SLOT(onUpdateDefaultValue()));


	//////////////////////////////////////////////////////////////////////////
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

void NetworkTool::switchTaskView(bool bForceDoing)
{
	if (bForceDoing)
	{
		m_pLblFinished->move(0, 460);
		m_pListViewDoing->show();
		m_pListViewFinished->hide();
	}
	else
	{
		if (m_pListViewFinished->isVisible())
		{
			m_pLblFinished->move(0, 460);
			m_pListViewDoing->show();
			m_pListViewFinished->hide();
		}
		else
		{
			m_pLblFinished->move(0, 102);
			m_pListViewDoing->hide();
			m_pListViewFinished->show();
		}
	}
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

	m_pWidgetAddTask->hide();
	uiMain.btn_add->setEnabled(true);
}

void NetworkTool::onAbortTask()
{
	qDebug() << __FUNCTION__;
	uiMain.btn_abort->setEnabled(false);

	QModelIndex index = m_pListViewDoing->currentIndex();
	if (index.isValid())
	{
		const QVariant& var = index.data(Qt::DisplayRole);
		if (var.isValid())
		{
			const RequestTask& stTask = qvariant_cast<RequestTask>(var);
			if (stTask.uiBatchId > 0)
			{
				if (QMessageBox::Yes == QMessageBox::information(nullptr, "Tips",
					QStringLiteral("该任务为批处理任务，是否终止整批任务？ 选择是终止整批任务，否则终止该任务。"), QMessageBox::Yes | QMessageBox::No))
				{
					NetworkManager::globalInstance()->stopBatchRequests(stTask.uiBatchId);
					return;
				}
			}

			if (stTask.uiId > 0)
			{
				NetworkManager::globalInstance()->stopRequest(stTask.uiId);
			}
		}
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

	appendMsg(QTime::currentTime().toString() + " - Start task[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeDownload;
	request.bShowProgress = uiAddTask.cb_showProgress->isChecked();
	request.strReqArg = strSavePath;
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
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));

		m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(request));
		m_pListViewDoing->update();
		switchTaskView(true);
	}
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

	appendMsg(QTime::currentTime().toString() + " - Start task[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeUpload;
	request.strReqArg = strUploadFilePath; //本地文件路径
	request.bShowProgress = uiAddTask.cb_showProgress->isChecked();

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));

		m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(request));
		m_pListViewDoing->update();
		switchTaskView(true);
	}
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

	appendMsg(QTime::currentTime().toString() + " - Start task[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeGet;

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));

		m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(request));
		m_pListViewDoing->update();
		switchTaskView(true);
	}
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

	appendMsg(QTime::currentTime().toString() + " - Start task[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypePost;
	request.strReqArg = strArg;

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));

		m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(request));
		m_pListViewDoing->update();
		switchTaskView(true);
	}
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

	appendMsg(QTime::currentTime().toString() + " - Start task[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypePut;
	request.strReqArg = QString::fromUtf8(bytes);

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));

		m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(request));
		m_pListViewDoing->update();
	}
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

	appendMsg(QTime::currentTime().toString() + " - Start task[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeDelete;

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));

		m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(request));
		m_pListViewDoing->update();
		switchTaskView(true);
	}
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

	appendMsg(m_timeStart.toString() + " - Start task[" + strUrl + "]");

	QUrl urlHost(strUrl);
	Q_ASSERT(urlHost.isValid());

	RequestTask request;
	request.url = urlHost;
	request.eType = eTypeHead;

	NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(request);
	if (nullptr != pReply)
	{
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));

		m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(request));
		m_pListViewDoing->update();
		switchTaskView(true);
	}
}

void NetworkTool::onBatchRequest()
{
	QString strFile = uiAddBatchTask.lineEdit_config->text().trimmed();
	if (strFile.isEmpty())
	{
		QMessageBox::information(nullptr, "Tips",
			QStringLiteral("批处理配置文件不能为空，请点击帮助说明按钮查看信息"), QMessageBox::Ok);
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
				request.strReqArg = strDir;
				request.bShowProgress = uiAddBatchTask.cb_showProgress->isChecked();
				request.bAbortBatchWhileOneFailed = uiAddBatchTask.cb_abortBatch->isChecked();
			}
		case eTypeUpload:
			{
				request.strReqArg = strArg;
				request.bShowProgress = uiAddBatchTask.cb_showProgress->isChecked();
			}
			break;
		case eTypePost:
		case eTypePut:
			{
				request.strReqArg = strArg;
			}
			break;
		case eTypeGet:
		case eTypeDelete:
		case eTypeHead:
			break;
		default:
			break;
		}

#if _MSC_VER >= 1700
		requests.append(std::move(request));
#else
		requests.append(request);
#endif
	}
	quint64 batchId;
	NetworkReply *pReply = NetworkManager::globalInstance()->addBatchRequest(requests, batchId);
	if (nullptr != pReply)
	{
		m_mapBatchTotalSize.insert(batchId, requests.size());
		connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
			this, SLOT(onRequestFinished(const RequestTask &)));

		appendMsg(QTime::currentTime().toString() + " - Start batch request, uiBatchId["
			+ QString::number(batchId) + "] Total[" + QString::number(requests.size()) + "]");

		QVector<QVariant> tasks;
		foreach(const RequestTask& req, requests)
		{
#if _MSC_VER >= 1700
			tasks.append(std::move(QVariant::fromValue(req)));
#else
			tasks.append(QVariant::fromValue(req));
#endif
		}
		m_pListViewDoing->insert(tasks);
		m_pListViewDoing->update();
		m_pWidgetAddBatch->hide();
		switchTaskView(true);
	}
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


	//上传
	QString strUrl = QString("http://%1:%2/_php/upload.php?filename=upload/%3.jpg")
		.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
	QStringList strlstFileUpload;
	strlstFileUpload.append("resources/test/1.png");
	strlstFileUpload.append("resources/test/2.jpg");
	strlstFileUpload.append("resources/test/3.jpeg");

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

	//DELETE
	strUrl = QString("http://%1:%2/_php/delete.php?filename=upload/1.jpg")
		.arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
#endif
}

//request:		任务信息
//bSuccess：	任务是否成功
void NetworkTool::onRequestFinished(const RequestTask &request)
{
	appendMsg(QTime::currentTime().toString() + " - Task finished. IsSuccess["
		+ QString::number(request.bSuccess) + "] url[" + request.url.url() + "]");

	if (!request.bytesContent.isEmpty())
	{
		appendMsg(QString("Content:\n") + request.bytesContent, false);
	}

	/*int msec = m_timeStart.elapsed();
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

	appendMsg(strMsg);*/

	bool bBatch = (request.uiBatchId > 0);
	if (bBatch)
	{
		if (request.bSuccess)
		{
			m_mapBatchSuccessSize[request.uiBatchId] = m_mapBatchSuccessSize.value(request.uiBatchId) + 1;
			qDebug() << "Success. Batch[" + QString::number(request.uiBatchId)
				+ "] [" + QString::number(m_mapBatchSuccessSize.value(request.uiBatchId))
				+ "/" + QString::number(m_mapBatchTotalSize.value(request.uiBatchId)) + "]";
		}
		else
		{
			m_mapBatchFailedSize[request.uiBatchId] = m_mapBatchFailedSize.value(request.uiBatchId) + 1;
			qDebug() << "Failed. Batch[" + QString::number(request.uiBatchId)
				+ "] [" + QString::number(m_mapBatchFailedSize.value(request.uiBatchId))
				+ "/" + QString::number(m_mapBatchTotalSize.value(request.uiBatchId)) + "]";
		}

		if (m_mapBatchSuccessSize.value(request.uiBatchId) + m_mapBatchFailedSize.value(request.uiBatchId) == m_mapBatchTotalSize.value(request.uiBatchId)
			|| (request.bAbortBatchWhileOneFailed && m_mapBatchFailedSize.value(request.uiBatchId) > 0))
		{
			appendMsg("Batch finished. Total["
				+ QString::number(m_mapBatchTotalSize.value(request.uiBatchId)) + "] Success["
				+ QString::number(m_mapBatchSuccessSize.value(request.uiBatchId))
				+ "] Failed[" + QString::number(m_mapBatchFailedSize.value(request.uiBatchId)) + "]");

			m_mapBatchSuccessSize.remove(request.uiBatchId);
			m_mapBatchFailedSize.remove(request.uiBatchId);
			m_mapBatchTotalSize.remove(request.uiBatchId);
		}
	}

	if (m_pListViewDoing)
	{
		m_pListViewDoing->onTaskFinished(request);
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
		}
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
		}
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
		char ch[8] = { 0 };
		sprintf(ch, "%.2f", dSize);
		str = QString("%1MB").arg(ch);
	}
	else
	{
		qreal dSize = (qreal)bytes / 1024 / 1024 / 1024;
		char ch[8] = { 0 };
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
LabelEx::LabelEx(QWidget* parent)
	: QLabel(parent)
{
}

void LabelEx::mouseDoubleClickEvent(QMouseEvent *event)
{
	__super::mouseDoubleClickEvent(event);
	emit dbClicked();
}

//////////////////////////////////////////////////////////////////////////
TaskListView::TaskListView(QWidget* parent)
	: Listview(parent)
{
}

void TaskListView::onTaskFinished(const RequestTask &request)
{
	QAbstractItemModel *model = this->model();
	TaskModel *pModel = dynamic_cast<TaskModel *>(model);
	if (pModel)
	{
		const QVariant& variant = pModel->onTaskFinished(request);
		if (variant.isValid())
		{
			emit taskFinished(variant);
		}
		update();
	}
}

//////////////////////////////////////////////////////////////////////////
TaskModel::TaskModel(QObject* parent)
	: ListModel(parent)
{
}

QVariant TaskModel::onTaskFinished(const RequestTask &request)
{
	QVariant variant;
	RequestTask task;
	if (request.uiId == ALL_TASK_ID && request.bCancel)
	{
		for (int i = 0; i < m_vecVariant.size(); ++i)
		{
			task = m_vecVariant[i].value<RequestTask>();
			task.bFinished = request.bFinished;
			task.bCancel = request.bCancel;
			task.bSuccess = request.bSuccess;
			m_vecVariant[i] = QVariant::fromValue(task);
		}
	}
	else if (request.bCancel && request.uiBatchId > 0)
	{
		for (int i = 0; i < m_vecVariant.size(); ++i)
		{
			task = m_vecVariant[i].value<RequestTask>();
			if (task.uiBatchId == request.uiBatchId)
			{
				task.bFinished = request.bFinished;
				task.bCancel = request.bCancel;
				task.bSuccess = request.bSuccess;
				m_vecVariant[i] = QVariant::fromValue(task);
			}
		}
	}
	else
	{
		for (int i = 0; i < m_vecVariant.size(); ++i)
		{
			task = m_vecVariant[i].value<RequestTask>();
			if (task.uiId == request.uiId)
			{
				task.bFinished = request.bFinished;
				task.bCancel = request.bCancel;
				task.bSuccess = request.bSuccess;
				m_vecVariant[i] = QVariant::fromValue(task);

				if (task.bSuccess)
				{
					variant = m_vecVariant[i];
					m_vecVariant.remove(i);
					emit sizeChanged(m_vecVariant.size());
				}
				break;
			}
		}
	}

	return variant;
}

//////////////////////////////////////////////////////////////////////////
TaskDelegate::TaskDelegate(QObject* parent /*= NULL*/)
	: ListDelegate(parent)
{
}

QSize TaskDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QSize(310, 60);
}

QString getTypeString(RequestType eType)
{
	QString strType;
	if (eType == eTypeDownload)
	{
		strType = QStringLiteral("下载");
	}
	else if (eType == eTypeUpload)
	{
		strType = QStringLiteral("上传");
	}
	else if (eType == eTypeGet)
	{
		strType = QLatin1String("GET");
	}
	else if (eType == eTypePost)
	{
		strType = QLatin1String("POST");
	}
	else if (eType == eTypePut)
	{
		strType = QLatin1String("PUT");
	}
	else if (eType == eTypeDelete)
	{
		strType = QLatin1String("DELETE");
	}
	else if (eType == eTypeHead)
	{
		strType = QLatin1String("HEAD");
	}
	return strType;
}

void TaskDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	painter->save();
	if (index.isValid())
	{
		QRect rect = option.rect;
		QRect rtBg = rect;
		rtBg.setHeight(rect.height() - 1);
		if (option.state & QStyle::State_MouseOver || option.state & QStyle::State_Selected)
		{
			painter->fillRect(rtBg, QColor("#808080"));
		}
		else
		{
			painter->fillRect(rtBg, QColor("#333333"));
		}

		const QVariant& var = index.data(Qt::DisplayRole);
		if (var.isValid())
		{
			const RequestTask& stTask = qvariant_cast<RequestTask>(var);

			QFont font;
			font.setFamily("Microsoft YaHei");
			font.setPixelSize(12);
			painter->setFont(font);
			painter->setPen(Qt::white);

			QFontMetrics fm(font);
			QRect boundingRect = fm.boundingRect(QRect(rect.left() + 10, rect.top() + 16, 300, 0), Qt::TextWordWrap, stTask.url.toString());
			painter->drawText(boundingRect, Qt::TextWordWrap, stTask.url.toString());
			painter->drawText(QRect(rect.left() + 10, rect.top(), 100, 14), QStringLiteral("类型（%1）")
				.arg(getTypeString(stTask.eType)), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));

			if (stTask.bCancel)
			{
				painter->setPen(Qt::blue);
				painter->drawText(QRect(rect.left() + 100, rect.top(), 60, 14),
					QStringLiteral("已取消"), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
			}
			else if (stTask.bFinished)
			{

				if (stTask.bSuccess)
				{
					painter->setPen(Qt::green);
					painter->drawText(QRect(rect.left() + 100, rect.top(), 60, 14),
						QStringLiteral("已完成"), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
				}
				else
				{
					painter->setPen(Qt::red);
					painter->drawText(QRect(rect.left() + 100, rect.top(), 60, 14),
						QStringLiteral("任务出错"), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
				}
			}
		}
	}
	painter->restore();
}

bool TaskDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
	const QStyleOptionViewItem &option, const QModelIndex &index)
{
	return __super::editorEvent(event, model, option, index);
}