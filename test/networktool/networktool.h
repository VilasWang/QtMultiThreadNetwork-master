#ifndef INTERNET_H
#define INTERNET_H

#include <QtWidgets/QMainWindow>
#include <QTime>
#include "ui_networktool.h"
#include "ui_addBatchTask.h"
#include "ui_addtask.h"
#include "listview.h"
#include "NetworkDef.h"

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
	void reset();
	QString getDefaultDownloadDir();//获取系统默认下载目录

private:
	Ui::networkClass uiMain;
	Ui::Widget_addTask uiAddTask;
	Ui::Widget_addBatch uiAddBatchTask;

	QWidget *m_pWidgetAddTask;
	QWidget *m_pWidgetAddBatch;
	Listview *m_pListView;
	Model *m_pModel;
	Delegate *m_pDelegate;
	QButtonGroup *bg_protocal;
	QButtonGroup *bg_type;

	quint64 m_requestId;
	quint64 m_batchId;

	int m_nTotalNum;
	int m_nFailedNum;
	int m_nSuccessNum;

	qint64 m_nbytesReceived;
	qint64 m_nBytesTotalDownload;
	QString m_strTotalDownload;
	qint64 m_nbytesSent;
	qint64 m_nBytesTotalUpload;
	QString m_strTotalUpload;

	QTime m_timeStart;
};

class TaskDelegate : public Delegate
{
public:
	TaskDelegate(QObject* parent = NULL);
	~TaskDelegate();

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
