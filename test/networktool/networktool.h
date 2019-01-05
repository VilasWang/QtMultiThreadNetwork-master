#ifndef INTERNET_H
#define INTERNET_H

#include <QtWidgets/QMainWindow>
#include <QTime>
#include "ui_networktool.h"
#include "NetworkDef.h"

class NetworkTool : public QMainWindow
{
	Q_OBJECT

public:
	NetworkTool(QWidget *parent = 0);
	~NetworkTool();

public Q_SLOTS:
	void onStartTask();
	void onAbortTask();

private Q_SLOTS:
	void onDownload();
	void onUpload();
	void onGetRequest();
	void onPostRequest();
	void onPutRequest();
	void onDeleteRequest();
	void onHeadRequest();
	void onBatchDownload();
	void onBatchMixedTask();
	void onRequestFinished(const RequestTask &);
	
	void onDownloadProgress(quint64, qint64, qint64);
	void onUploadProgress(quint64, qint64, qint64);
	void onBatchDownloadProgress(quint64, qint64);
	void onBatchUploadProgress(quint64, qint64);
	void onErrorMessage(const QString& error);

	void onUpdateDefaultInfos();
	void onGetSaveDirectory();
	void onGetUploadFile();

private:
	void initialize();
	void unIntialize();
	QString bytes2String(qint64 bytes);
	void appendMsg(const QString& strMsg, bool bQDebug = true);
	void reset();

private:
	Ui::networkClass ui;

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

#endif // INTERNET_H
