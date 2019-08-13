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
#include <QPushButton>
#include <QComboBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>
#include "networktool.h"
#include "networkmanager.h"
#include "networkreply.h"


//#define TEST_PERFORMANCE_NO_TASK_LIST
#define BATCH_REQUEST_EXEC_COUNT    1
#define POST_REQUEST_EXEC_COUNT     1

#define DEFAULT_CONCURRENT_TASK		8
#define MAX_CONCURRENT_TASK		    16
#define MIN_CONCURRENT_TASK		    1

#define DEFAULT_MTDOWNLOAD_COUNT	5
#define MAX_MTDOWNLOAD_COUNT	    10
#define MIN_MTDOWNLOAD_COUNT		2

#define TASKING_TEXT_FORMAT			QString::fromStdWString(L"  执行中的任务(%1)")
#define FINISHED_TEXT_FORMAT		QString::fromStdWString(L"  已完成的任务(%1)")

//Apache http 本地服务器
#define HTTP_SERVER_IP				"127.0.0.1"
#define HTTP_SERVER_PORT			"80"


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
    onResetDefaultValue();
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
    for (int i = MIN_CONCURRENT_TASK; i <= MAX_CONCURRENT_TASK; ++i)
    {
        uiMain.cmb_concurrentTask->addItem(QString::number(i));
    }
    for (int i = MIN_MTDOWNLOAD_COUNT; i <= MAX_MTDOWNLOAD_COUNT; ++i)
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

    m_pLblTasking = new QLabelEx(this);
    m_pLblTasking->setGeometry(0, 80, 320, 20);
    m_pLblTasking->setText(TASKING_TEXT_FORMAT.arg(0));
    m_pLblTasking->setStyleSheet("QLabel{background-color:#636363;color:#DBDBDB;}");

    m_pLblFinished = new QLabelEx(this);
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
        uiMain.btn_add->setEnabled(false);
    });
    connect(uiMain.btn_addBatch, &QPushButton::clicked, m_pWidgetAddBatch, [=] {
        m_pWidgetAddBatch->move(this->width() / 2 - m_pWidgetAddBatch->width() / 2,
            this->height() / 2 - m_pWidgetAddBatch->height() / 2);
        m_pWidgetAddBatch->show();
        m_pWidgetAddBatch->raise();
        uiMain.btn_addBatch->setEnabled(false);
    });
    connect(uiMain.btn_abort, SIGNAL(clicked()), this, SLOT(onAbortTask()));
    connect(uiMain.btn_abortAll, SIGNAL(clicked()), this, SLOT(onAbortAllTask()));
    connect(uiAddTask.btn_browser1, SIGNAL(clicked()), this, SLOT(onGetSaveDirectory()));
    connect(uiAddTask.btn_browser2, SIGNAL(clicked()), this, SLOT(onGetUploadFile()));
    connect(uiAddBatchTask.btn_browser, SIGNAL(clicked()), this, SLOT(onGetBatchTaskConfigFile()));
    connect(uiAddTask.btn_start, SIGNAL(clicked()), this, SLOT(onAddTask()));
    connect(uiAddBatchTask.btn_start, SIGNAL(clicked()), this, SLOT(onAddBatchTasks()));
    connect(uiAddTask.btn_close, &QAbstractButton::clicked, this, [=] {
        m_pWidgetAddTask->hide();
        uiMain.btn_add->setEnabled(true);
    });
    connect(uiAddBatchTask.btn_close, &QAbstractButton::clicked, this, [=] {
        m_pWidgetAddBatch->hide();
        uiMain.btn_addBatch->setEnabled(true);
    });
    connect(uiAddTask.cb_useDefault, &QAbstractButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            onResetDefaultValue();
        }
    });
    connect(uiAddBatchTask.btn_help, &QAbstractButton::clicked, this, [=]() {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("请参考默认的批处理请求配置文件。文件位置（工程目录/samples/networktool/help/Vercomp.dat）"), QMessageBox::Ok);
    });
    connect(uiMain.cmb_concurrentTask, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
        this, [=](const QString &strText) {
        int num = strText.toInt();
        NetworkManager::globalInstance()->setMaxThreadCount(num);
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
        m_pLblTasking->setText(QString(TASKING_TEXT_FORMAT).arg(size));
    });
    connect(m_pModelFinished, &ListModel::sizeChanged, this, [=](int size) {
        m_pLblFinished->setText(QString(FINISHED_TEXT_FORMAT).arg(size));
    });
    connect(m_pLblTasking, &QLabelEx::dbClicked, this, [=]() {
        switchTaskView();
    });
    connect(m_pLblFinished, &QLabelEx::dbClicked, this, [=]() {
        switchTaskView();
    });
    connect(bg_protocal, SIGNAL(buttonToggled(int, bool)), this, SLOT(onResetDefaultValue()));
    connect(bg_type, SIGNAL(buttonToggled(int, bool)), this, SLOT(onResetDefaultValue()));
    connect(uiMain.btn_clear, &QAbstractButton::clicked, uiMain.textEdit_output, &QTextEdit::clear);


    //////////////////////////////////////////////////////////////////////////
    connect(NetworkManager::globalInstance(), &NetworkManager::downloadProgress
        , m_pListViewDoing, &TaskListView::onUpdateTaskProgress);
    connect(NetworkManager::globalInstance(), &NetworkManager::uploadProgress
        , m_pListViewDoing, &TaskListView::onUpdateTaskProgress);
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

void NetworkTool::onResetDefaultValue()
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
                const QString& strUrl = "https://1.as.dl.wireshark.org/win64/Wireshark-win64-3.0.2.exe";
                uiAddTask.lineEdit_url->setText(strUrl);
                uiAddTask.lineEdit_saveDir->setText(getDefaultDownloadDir());
            }
            else if (uiAddTask.cb_upload->isChecked())
            {
                const QString& strUrl = QString("http://%1:%2/_php/upload.php?filename=upload/VerComp_qt.dat")
                    .arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
                uiAddTask.lineEdit_url->setText(strUrl);
                uiAddTask.lineEdit_uploadFile->setText("help/VerComp.dat");
            }
            else if (uiAddTask.cb_get->isChecked())
            {
                const QString& strUrl = QStringLiteral("http://m.kugou.com/singer/list/88?json=true");
                uiAddTask.lineEdit_url->setText(strUrl);
            }
            else if (uiAddTask.cb_post->isChecked())
            {
                const QString& strArg = "userId=121892674&userName=33CxghNmt1FhAA==&st=QQBnAEEAQQBBAEUATA"
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
                const QString& strUrl = QString("http://%1:%2/_php/upload.php?filename=upload/VerComp_qt.dat")
                    .arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
                uiAddTask.lineEdit_url->setText(strUrl);
                uiAddTask.lineEdit_uploadFile->setText("help/VerComp.dat");

                //const QString& strUrl = QString("https://api.bimface.com/translate");
                //uiAddTask.lineEdit_url->setText(strUrl);
            }
            else if (uiAddTask.cb_delete->isChecked())
            {
                const QString& strUrl = QString("http://%1:%2/_php/delete.php?filename=upload/1.jpg")
                    .arg(HTTP_SERVER_IP).arg(HTTP_SERVER_PORT);
                uiAddTask.lineEdit_url->setText(strUrl);
            }
            else if (uiAddTask.cb_head->isChecked())
            {
                uiAddTask.lineEdit_url->setText("http://iso.mirrors.ustc.edu.cn/qtproject/archive/qt/5.12/5.12.1/single/qt-everywhere-src-5.12.1.zip");
            }
        }
        //FTP
        else if (uiAddTask.cb_ftp->isChecked())
        {
            if (uiAddTask.cb_download->isChecked())
            {
                uiAddTask.lineEdit_url->setText("ftp://10.1.28.101/tools/Git-1.8.1.2-preview20130201.exe");
                uiAddTask.lineEdit_saveDir->setText(getDefaultDownloadDir());
            }
            else if (uiAddTask.cb_upload->isChecked())
            {
                uiAddTask.lineEdit_uploadFile->setText("help/VerComp.dat");
                uiAddTask.lineEdit_url->setText("ftp://10.1.28.101/incoming/QtUploadTest.dat");
            }
            else if (uiAddTask.cb_get->isChecked())
            {
                uiAddTask.lineEdit_url->setText("ftp://10.1.28.101/incoming/QtGetTest.dat");
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
    m_pWidgetAddTask->hide();
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

    uiMain.btn_add->setEnabled(true);
}

void NetworkTool::onAddBatchTasks()
{
    m_pWidgetAddBatch->hide();
    onBatchRequest();

    uiMain.btn_addBatch->setEnabled(true);
}

void NetworkTool::onAbortTask()
{
    //qDebug() << __FUNCTION__;
    uiMain.btn_abort->setEnabled(false);

    const QModelIndex& index = m_pListViewDoing->currentIndex();
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

void NetworkTool::onDownload()
{
    const QString& strUrl = uiAddTask.lineEdit_url->text().trimmed();
    if (strUrl.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    const QString& strSavePath = uiAddTask.lineEdit_saveDir->text().trimmed();
    if (strSavePath.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("文件保存位置不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    QUrl urlHost(strUrl);
    Q_ASSERT(urlHost.isValid());

    RequestTask req;
    req.url = urlHost;
    req.eType = eTypeDownload;
    req.bShowProgress = uiAddTask.cb_showProgress->isChecked();
    req.bRemoveFileWhileExist = true;
    req.strReqArg = strSavePath;
    req.bTryAgainWhileFailed = true;
    if (!uiAddTask.lineEdit_filename->text().isEmpty())
    {
        req.strSaveFileName = uiAddTask.lineEdit_filename->text().trimmed();
    }
    if (uiAddTask.cb_multiDownload->isChecked())
    {
        req.eType = eTypeMTDownload;
        req.nDownloadThreadCount = uiAddTask.cmb_multiDownload->currentText().toInt();
    }

    NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(req);
    if (nullptr != pReply)
    {
        appendStartTaskMsg(req.uiId, strUrl);

        connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
            this, SLOT(onRequestFinished(const RequestTask &)));

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
        m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(req));
        switchTaskView(true);
#endif
    }
}

void NetworkTool::onUpload()
{
    const QString& strUrl = uiAddTask.lineEdit_url->text().trimmed();
    if (strUrl.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    const QString& strUploadFilePath = uiAddTask.lineEdit_uploadFile->text().trimmed();
    if (strUploadFilePath.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("上传文件不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    QUrl urlHost(strUrl);
    Q_ASSERT(urlHost.isValid());

    RequestTask req;
    req.url = urlHost;
    req.eType = eTypeUpload;
    req.strReqArg = strUploadFilePath; //本地文件路径
    req.bShowProgress = uiAddTask.cb_showProgress->isChecked();
    req.bTryAgainWhileFailed = true;

    NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(req);
    if (nullptr != pReply)
    {
        appendStartTaskMsg(req.uiId, strUrl);

        connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
            this, SLOT(onRequestFinished(const RequestTask &)));

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
        m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(req));
        switchTaskView(true);
#endif
    }
}

void NetworkTool::onGetRequest()
{
    const QString& strUrl = uiAddTask.lineEdit_url->text().trimmed();
    if (strUrl.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    QUrl urlHost(strUrl);
    Q_ASSERT(urlHost.isValid());

    RequestTask req;
    req.url = urlHost;
    req.eType = eTypeGet;
    req.bTryAgainWhileFailed = true;

    NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(req);
    if (nullptr != pReply)
    {
        appendStartTaskMsg(req.uiId, strUrl);

        connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
            this, SLOT(onRequestFinished(const RequestTask &)));

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
        m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(req));
        switchTaskView(true);
#endif
    }
}

void NetworkTool::onPostRequest()
{
    const QString& strUrl = uiAddTask.lineEdit_url->text().trimmed();
    if (strUrl.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    const QString& strArg = uiAddTask.lineEdit_arg->text().trimmed();
    if (strArg.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("参数不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    QUrl urlHost(strUrl);
    Q_ASSERT(urlHost.isValid());

    RequestTask req;
    req.url = urlHost;
    req.eType = eTypePost;
    req.strReqArg = strArg;
    req.bTryAgainWhileFailed = true;

    for (int i = 0; i < POST_REQUEST_EXEC_COUNT; ++i)
    {
        NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(req);
        if (nullptr != pReply)
        {
            appendStartTaskMsg(req.uiId, strUrl);

            connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
                this, SLOT(onRequestFinished(const RequestTask &)));

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
            m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(req));
            switchTaskView(true);
#endif
        }
    }
}

void NetworkTool::onPutRequest()
{
    const QString& strUrl = uiAddTask.lineEdit_url->text().trimmed();
    if (strUrl.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    const QString& strUploadFilePath = uiAddTask.lineEdit_uploadFile->text().trimmed();
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

    QUrl urlHost(strUrl);
    Q_ASSERT(urlHost.isValid());

    RequestTask req;
    req.url = urlHost;
    req.eType = eTypePut;
    req.strReqArg = QString::fromUtf8(bytes);
    req.bTryAgainWhileFailed = true;

#if 0
    QFile file("D:\\test.igms");
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("上传文件不存在或者已被占用"), QMessageBox::Ok);
        reset();
        return;
    }
    const QByteArray& bytes = file.readAll();
    file.close();

    req.mapRawHeader.insert("Authorization", "bearer cn-ad0c0fe1-7643-42c1-8cfa-e7dc74e05e7b");
    req.mapRawHeader.insert("Content-Length", QString::number(bytes.size()).toUtf8());
    req.strReqArg = QString::fromUtf8(bytes);
#endif

    NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(req);
    if (nullptr != pReply)
    {
        appendStartTaskMsg(req.uiId, strUrl);

        connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
            this, SLOT(onRequestFinished(const RequestTask &)));

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
        m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(req));
        switchTaskView(true);
#endif
    }
}

void NetworkTool::onDeleteRequest()
{
    const QString& strUrl = uiAddTask.lineEdit_url->text().trimmed();
    if (strUrl.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    QUrl urlHost(strUrl);
    Q_ASSERT(urlHost.isValid());

    RequestTask req;
    req.url = urlHost;
    req.eType = eTypeDelete;
    req.bTryAgainWhileFailed = true;

    NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(req);
    if (nullptr != pReply)
    {
        appendStartTaskMsg(req.uiId, strUrl);

        connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
            this, SLOT(onRequestFinished(const RequestTask &)));

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
        m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(req));
        switchTaskView(true);
#endif
    }
}

void NetworkTool::onHeadRequest()
{
    const QString& strUrl = uiAddTask.lineEdit_url->text().trimmed();
    if (strUrl.isEmpty())
    {
        QMessageBox::information(nullptr, "Tips",
            QStringLiteral("链接地址不能为空，详情请参考使用说明"), QMessageBox::Ok);
        reset();
        return;
    }

    QUrl urlHost(strUrl);
    Q_ASSERT(urlHost.isValid());

    RequestTask req;
    req.url = urlHost;
    req.eType = eTypeHead;
    req.bTryAgainWhileFailed = true;

    NetworkReply *pReply = NetworkManager::globalInstance()->addRequest(req);
    if (nullptr != pReply)
    {
        appendStartTaskMsg(req.uiId, strUrl);

        connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
            this, SLOT(onRequestFinished(const RequestTask &)));

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
        m_pListViewDoing->insert(QVariant::fromValue<RequestTask>(req));
        switchTaskView(true);
#endif
    }
}

void NetworkTool::onBatchRequest()
{
    const QString& strFile = uiAddBatchTask.lineEdit_config->text().trimmed();
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
            if (!strLine.isEmpty())
            {
                strlstLine += strLine;
            }
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
    RequestTask req;
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
        const QString& strArg = strlst[2];

        QUrl urlHost(strUrlStandard);
        Q_ASSERT(urlHost.isValid());
        req.url = urlHost;
        req.eType = RequestType(strlst[1].toInt());
        req.bAbortBatchWhileOneFailed = uiAddBatchTask.cb_abortBatch->isChecked();
        req.bTryAgainWhileFailed = true;
        switch (req.eType)
        {
        case eTypeDownload:
        case eTypeMTDownload:
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
            req.strReqArg = strDir;
            req.bShowProgress = uiAddBatchTask.cb_showProgress->isChecked();
            req.bRemoveFileWhileExist = true;
        }
        case eTypeUpload:
        {
            req.strReqArg = strArg;
            req.bShowProgress = uiAddBatchTask.cb_showProgress->isChecked();
        }
        break;
        case eTypePost:
        case eTypePut:
        {
            req.strReqArg = strArg;
        }
        break;
        case eTypeGet:
        case eTypeDelete:
        case eTypeHead:
            break;
        default:
            break;
        }

        requests.append(std::move(req));
    }

    for (int i = 0; i < BATCH_REQUEST_EXEC_COUNT; ++i)
    {
        quint64 batchId;
        NetworkReply *pReply = NetworkManager::globalInstance()->addBatchRequest(requests, batchId);
        if (nullptr != pReply)
        {
            appendStartBatchTasksMsg(batchId, requests.size());
            m_mapBatchTotalSize.insert(batchId, requests.size());

            connect(pReply, SIGNAL(requestFinished(const RequestTask &)),
                this, SLOT(onRequestFinished(const RequestTask &)));

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
            QVector<QVariant> vec;
            vec.resize(requests.size());
            int i = 0;
            foreach(const RequestTask& r, requests)
            {
                vec[i] = QVariant::fromValue(r);
                i++;
            }
            m_pListViewDoing->insert(vec);
            switchTaskView(true);
#endif
        }
    }
}

//request:		任务信息
//bSuccess：	任务是否成功
void NetworkTool::onRequestFinished(const RequestTask &request)
{
    bool bBatch = (request.uiBatchId > 0);
    appendTaskFinishMsg(request.uiId, bBatch, request.bSuccess, request.url.url(), request.bytesContent, request.strError);

    if (bBatch)
    {
        if (request.bSuccess)
        {
            m_mapBatchSuccessSize[request.uiBatchId] = m_mapBatchSuccessSize.value(request.uiBatchId) + 1;
        }
        else
        {
            m_mapBatchFailedSize[request.uiBatchId] = m_mapBatchFailedSize.value(request.uiBatchId) + 1;
        }

        qDebug() << "Batch[" + QString::number(request.uiBatchId)
            + "] Total["
            + QString::number(m_mapBatchTotalSize.value(request.uiBatchId)) + "] Success["
            + QString::number(m_mapBatchSuccessSize.value(request.uiBatchId))
            + "] Failed[" + QString::number(m_mapBatchFailedSize.value(request.uiBatchId)) + "]";

        if (m_mapBatchSuccessSize.value(request.uiBatchId) + m_mapBatchFailedSize.value(request.uiBatchId) == m_mapBatchTotalSize.value(request.uiBatchId)
            || (request.bAbortBatchWhileOneFailed && m_mapBatchFailedSize.value(request.uiBatchId) > 0))
        {
            appendMsg(QTime::currentTime().toString() + " - Batch finished. Id[" + QString::number(request.uiBatchId) + "]");

            m_mapBatchSuccessSize.remove(request.uiBatchId);
            m_mapBatchFailedSize.remove(request.uiBatchId);
            m_mapBatchTotalSize.remove(request.uiBatchId);
        }
    }

#ifndef TEST_PERFORMANCE_NO_TASK_LIST
    if (m_pListViewDoing)
    {
        m_pListViewDoing->onTaskFinished(request);
    }
#endif // !TEST_PERFORMANCE_NO_TASK_LIST
}

void NetworkTool::onBatchDownloadProgress(quint64 batchId, qint64 bytes)
{
    //const QString& str = bytes2String(bytes);
    //appendMsg(QStringLiteral("批任务[%1]   下载：%2").arg(batchId).arg(bytes), false);
}

void NetworkTool::onBatchUploadProgress(quint64 batchId, qint64 bytes)
{
    //const QString& str = bytes2String(bytes);
    //appendMsg(QStringLiteral("批任务[%1]   上传：%2").arg(batchId).arg(bytes), false);
}

void NetworkTool::onErrorMessage(const QString& error)
{
    appendMsg(QTime::currentTime().toString() + " - error: " + error);
}

void NetworkTool::onGetSaveDirectory()
{
    const QString& dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
        "/home",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isNull() && !dir.isEmpty())
    {
        uiAddTask.lineEdit_saveDir->setText(dir);
    }
}

void NetworkTool::onGetUploadFile()
{
    const QString& fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        "/desktop",
        tr("File (*.*)"));

    if (!fileName.isNull() && !fileName.isEmpty())
    {
        uiAddTask.lineEdit_uploadFile->setText(fileName);
    }
}

void NetworkTool::onGetBatchTaskConfigFile()
{
    const QString& fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
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

void NetworkTool::appendStartTaskMsg(quint64 uiTaskid, const QString& strUrl)
{
    const QString& str = QString(" - Start task[id:%1][%2]").arg(uiTaskid).arg(strUrl);
    appendMsg(QTime::currentTime().toString() + str);
}

void NetworkTool::appendStartBatchTasksMsg(quint64 uiBatchid, int nTotalSize)
{
    const QString& str = QString(" - Start batch tasks[id:%1], total[%2]").arg(uiBatchid).arg(nTotalSize);
    appendMsg(QTime::currentTime().toString() + str);
}

void NetworkTool::appendTaskFinishMsg(quint64 uiTaskid, bool isBatch, bool bSuccess, const QString& strUrl, const QString& strBody, const QString& strError)
{
    if (RequestTask::ALL_TASK == uiTaskid)
    {
        const QString& str = QString(" - All tasks cancelled!");
        appendMsg(QTime::currentTime().toString() + str, !isBatch);
    }
    else
    {
        if (bSuccess)
        {
            const QString& str = QString(" - Task[Success][id:%1][%2]").arg(uiTaskid).arg(strUrl);
            appendMsg(QTime::currentTime().toString() + str, !isBatch);
        }
        else
        {
            const QString& str = QString(" - Task[Failed][id:%1][%2]").arg(uiTaskid).arg(strUrl);
            appendMsg(QTime::currentTime().toString() + str, !isBatch);
        }
    }

    if (!strBody.isEmpty())
    {
        appendMsg(QString("[Body][id:%1]\n").arg(uiTaskid) + strBody, !isBatch);
    }
    if (!strError.isEmpty())
    {
        appendMsg(QString("[Error][id:%1]\n").arg(uiTaskid) + strError, !isBatch);
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
QLabelEx::QLabelEx(QWidget* parent)
    : QLabel(parent)
{
}

void QLabelEx::mouseDoubleClickEvent(QMouseEvent *event)
{
    __super::mouseDoubleClickEvent(event);
    emit dbClicked();
}

//////////////////////////////////////////////////////////////////////////
TaskListView::TaskListView(QWidget* parent)
    : ListView(parent)
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

void TaskListView::onUpdateTaskProgress(quint64 taskId, qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0 && taskId > 0)
    {
        //qDebug() << taskId << bytesReceived << bytesTotal;
        int p = bytesReceived * 100 / bytesTotal;
        QAbstractItemDelegate *delegate = this->itemDelegate();
        TaskDelegate *pDelegate = dynamic_cast<TaskDelegate *>(delegate);
        if (pDelegate)
        {
            if (pDelegate->progress(taskId) < p)
            {
                pDelegate->setProgress(taskId, p);
                update();
            }
        }
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
    if (request.uiId == RequestTask::ALL_TASK && request.bCancel)
    {
        clear();
    }
    else if (request.bCancel && request.uiBatchId > 0)
    {
        int nSizePre = m_vecVariant.size();
        auto iter = m_vecVariant.begin();
        for (; iter != m_vecVariant.end();)
        {
            task = iter->value<RequestTask>();
            if (task.uiBatchId == request.uiBatchId)
            {
                iter = m_vecVariant.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
        int nSizeAft = m_vecVariant.size();
        if (nSizeAft != nSizePre)
        {
            resetAll(m_vecVariant);
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

                variant = m_vecVariant[i];
                remove(i);
                break;
            }
        }
    }

    return variant;
}

//////////////////////////////////////////////////////////////////////////
TaskDelegate::TaskDelegate(QObject* parent /*= NULL*/)
    : ListItemDelegate(parent)
{
}

TaskDelegate::~TaskDelegate()
{
    m_mapProgress.clear();
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
        const QRect& rect = option.rect;
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
            const QRect& boundingRect = fm.boundingRect(QRect(rect.left() + 10, rect.top() + 16, 300, 0), Qt::TextWordWrap, stTask.url.toString());
            painter->drawText(boundingRect, Qt::TextWordWrap, stTask.url.toString());
            painter->drawText(QRect(rect.left() + 10, rect.top(), 100, 14), QStringLiteral("类型（%1）")
                .arg(getTypeString(stTask.eType)), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));

            if (stTask.bCancel)
            {
                painter->setPen(Qt::red);
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

            if (!stTask.bFinished && !stTask.bCancel && stTask.bShowProgress
                && (stTask.eType == eTypeDownload || stTask.eType == eTypeUpload || stTask.eType == eTypeMTDownload))
            {
                int p = m_mapProgress.value(stTask.uiId);
                painter->fillRect(rect.left() + 180, rect.top() + 1, 102, 12, QBrush("#191919"));
                painter->fillRect(rect.left() + 181, rect.top() + 2, p, 10, QBrush("#08ce5b"));
                font.setPixelSize(10);
                painter->setFont(font);
                painter->setPen(Qt::white);
                painter->drawText(QRect(rect.left() + 220, rect.top() + 1, 60, 12),
                    QStringLiteral("%1%").arg(p), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
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

void TaskDelegate::setProgress(quint64 id, int progress)
{
    m_mapProgress[id] = progress;
}

int TaskDelegate::progress(quint64 id)
{
    return m_mapProgress.value(id);
}