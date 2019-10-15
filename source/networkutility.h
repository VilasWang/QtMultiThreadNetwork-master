#pragma once

#include <memory>
#include <QString>

class QFile;
class QUrl;
namespace QMTNetwork {
    struct RequestTask;
}

class NetworkUtility
{
public:
    //创建并打开文件
    static std::unique_ptr<QFile> createAndOpenFile(const QMTNetwork::RequestTask&, QString& errMessage);

    //创建共享读写的文件
    static QString createSharedRWFileWin32(const QMTNetwork::RequestTask&, QString& errMessage, qint64 nDefaultFileSize = 0);

    //读取文件的内容
    static bool readFileContent(const QString& strFilePath, QByteArray& bytes, QString& errMessage);

    //获取下载文件保存的文件名
    static QString getDownloadFileSaveName(const QMTNetwork::RequestTask&);
    //获取下载文件保存的目录
    static QString getDownloadFileSaveDir(const QMTNetwork::RequestTask&, QString& errMessage);

    static bool fileExists(QFile *pFile);
    static bool fileOpened(QFile *pFile);
    static bool removeFile(const QString& strFilePath, QString& errMessage);
    static QUrl currentRequestUrl(const QMTNetwork::RequestTask&);

private:
    NetworkUtility();
    ~NetworkUtility();
    NetworkUtility(const NetworkUtility &);
    NetworkUtility &operator=(const NetworkUtility &);
};

