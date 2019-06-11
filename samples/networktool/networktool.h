#ifndef INTERNET_H
#define INTERNET_H

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include "ui_networktool.h"
#include "ui_addBatchTask.h"
#include "ui_addtask.h"
#include "NetworkDef.h"
#include "listview.h"

class QWidget;
class QButtonGroup;
class TaskListView;
class TaskModel;
class QLabelEx;
class NetworkTool : public QMainWindow
{
    Q_OBJECT

public:
    NetworkTool(QWidget *parent = 0);
    ~NetworkTool();

private Q_SLOTS:
    void onAddTask();
    void onAddBatchTasks();
    void onAbortTask();
    void onAbortAllTask();

    void onDownload();
    void onUpload();
    void onGetRequest();
    void onPostRequest();
    void onPutRequest();
    void onDeleteRequest();
    void onHeadRequest();
    void onBatchRequest();

    void onRequestFinished(const RequestTask &);
    void onErrorMessage(const QString& error);
    void onBatchDownloadProgress(quint64, qint64);
    void onBatchUploadProgress(quint64, qint64);

    void onResetDefaultValue();
    void onGetSaveDirectory();
    void onGetUploadFile();
    void onGetBatchTaskConfigFile();

private:
    void initialize();
    void unIntialize();
    void initCtrls();
    void initConnecting();

    QString getDefaultDownloadDir();//获取系统默认下载目录
    QString bytes2String(qint64 bytes);

    void appendMsg(const QString& strMsg, bool bQDebug = true);
    void appendStartTaskMsg(quint64 uiTaskid, const QString& strUrl);
    void appendStartBatchTasksMsg(quint64 uiBatchid, int nTotalSize);
    void appendTaskFinishMsg(quint64 uiTaskid, bool isBatch, bool bSuccess, const QString& strUrl, const QString& strBody);

    void switchTaskView(bool bForceDoing = false);
    void reset();

private:
    Ui::networkClass uiMain;
    Ui::Widget_addTask uiAddTask;
    Ui::Widget_addBatch uiAddBatchTask;

    QWidget *m_pWidgetAddTask;
    QWidget *m_pWidgetAddBatch;
    QLabelEx *m_pLblTasking;
    QLabelEx *m_pLblFinished;
    TaskListView *m_pListViewDoing;
    TaskListView *m_pListViewFinished;
    ListModel *m_pModelDoing;
    ListModel *m_pModelFinished;
    ListDelegate *m_pDelegate;
    QButtonGroup *bg_protocal;
    QButtonGroup *bg_type;

    QMap<quint64, int> m_mapBatchTotalSize;
    QMap<quint64, int> m_mapBatchSuccessSize;
    QMap<quint64, int> m_mapBatchFailedSize;

    qint64 m_nbytesReceived;
    qint64 m_nBytesTotalDownload;
    QString m_strTotalDownload;
    qint64 m_nbytesSent;
    qint64 m_nBytesTotalUpload;
    QString m_strTotalUpload;
};

class QLabelEx : public QLabel
{
    Q_OBJECT

public:
    explicit QLabelEx(QWidget* parent = NULL);
    virtual ~QLabelEx() {}

Q_SIGNALS:
    void dbClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

class TaskListView : public Listview
{
    Q_OBJECT

public:
    explicit TaskListView(QWidget* parent = NULL);
    ~TaskListView() {}

Q_SIGNALS:
    void taskFinished(const QVariant&);

    public Q_SLOTS:
    void onTaskFinished(const RequestTask &request);
    void onUpdateTaskProgress(quint64 taskId, qint64 bytesReceived, qint64 bytesTotal);
};

class TaskModel : public ListModel
{
public:
    explicit TaskModel(QObject* parent = NULL);
    ~TaskModel() {}

    QVariant onTaskFinished(const RequestTask &request);
};

class TaskDelegate : public ListDelegate
{
public:
    explicit TaskDelegate(QObject* parent = NULL);
    ~TaskDelegate();

public:
    void paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
        const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setProgress(quint64, int);
    int progress(quint64);

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
        const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;

private:
    QMap<quint64, int> m_mapProgress;

};

#endif // INTERNET_H
