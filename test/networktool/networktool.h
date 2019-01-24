#ifndef INTERNET_H
#define INTERNET_H

#include <QtWidgets/QMainWindow>
#include <QTime>
#include "ui_networktool.h"
#include "ui_addBatchTask.h"
#include "ui_addtask.h"
#include "listview.h"
#include "NetworkDef.h"

class TaskListView;
class TaskModel;
class LabelEx;
class NetworkTool : public QMainWindow
{
	Q_OBJECT

public:
	NetworkTool(QWidget *parent = 0);
	~NetworkTool();

public Q_SLOTS:
	void onAddTask();
	void onAbortTask();
	void onAbortAllTask();

private Q_SLOTS:
	void onDownload();
	void onUpload();
	void onGetRequest();
	void onPostRequest();
	void onPutRequest();
	void onDeleteRequest();
	void onHeadRequest();
	void onBatchRequest();
	void onBatchMixedTask();
	void onRequestFinished(const RequestTask &);
	
	void onDownloadProgress(quint64, qint64, qint64);
	void onUploadProgress(quint64, qint64, qint64);
	void onBatchDownloadProgress(quint64, qint64);
	void onBatchUploadProgress(quint64, qint64);
	void onErrorMessage(const QString& error);

	void onUpdateDefaultValue();
	void onGetSaveDirectory();
	void onGetUploadFile();
	void onGetBatchTaskConfigFile();

private:
	void initialize();
	void unIntialize();
	void initCtrls();
	void initConnecting();

	QString bytes2String(qint64 bytes);
	void appendMsg(const QString& strMsg, bool bQDebug = true);
	QString getDefaultDownloadDir();//获取系统默认下载目录
	void switchTaskView(bool bForceDoing = false);
	void reset();

private:
	Ui::networkClass uiMain;
	Ui::Widget_addTask uiAddTask;
	Ui::Widget_addBatch uiAddBatchTask;

	QWidget *m_pWidgetAddTask;
	QWidget *m_pWidgetAddBatch;
	LabelEx *m_pLblTasking;
	LabelEx *m_pLblFinished;
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

	QTime m_timeStart;
};

class LabelEx : public QLabel
{
	Q_OBJECT

public:
	LabelEx(QWidget* parent = NULL);
	~LabelEx() {}

Q_SIGNALS:
	void dbClicked();

protected:
	void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

class TaskListView : public Listview
{
	Q_OBJECT

public:
	TaskListView(QWidget* parent = NULL);
	~TaskListView(){}

Q_SIGNALS:
	void taskFinished(const QVariant&);

public:
	void onTaskFinished(const RequestTask &request);
};

class TaskModel : public ListModel
{
public:
	TaskModel(QObject* parent = NULL);
	~TaskModel() {}

	QVariant onTaskFinished(const RequestTask &request);
};

class TaskDelegate : public ListDelegate
{
public:
	TaskDelegate(QObject* parent = NULL);
	~TaskDelegate(){}

public:
	void paint(QPainter *painter,
			   const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

	QSize sizeHint(const QStyleOptionViewItem &option,
				   const QModelIndex &index) const Q_DECL_OVERRIDE;

protected:
	bool editorEvent(QEvent *event, QAbstractItemModel *model,
					 const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;

};

#endif // INTERNET_H
