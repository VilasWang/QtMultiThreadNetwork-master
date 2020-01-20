#include "networkutility.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <QUrlQuery>
#include <QList>
#include <QPair>
#include <QDir>
#include <QDebug>
#include <QFile>
#include "networkdefs.h"

using namespace QMTNetwork;

NetworkUtility::NetworkUtility()
{
}

NetworkUtility::~NetworkUtility()
{
}

std::unique_ptr<QFile> NetworkUtility::createAndOpenFile(const QMTNetwork::RequestTask& request, QString& strError)
{
    std::unique_ptr<QFile> pFile;
    strError.clear();

    //取下载文件保存目录
    const QString& strSaveDir = getDownloadFileSaveDir(request, strError);
    if (strSaveDir.isEmpty())
    {
        return pFile;
    }

    //取下载保存的文件名
    const QString& strFileName = getDownloadFileSaveName(request);
    if (strFileName.isEmpty())
    {
        strError = QLatin1String("Error: fileName is empty!");
        qWarning() << strError;
        return pFile;
    }

    //如果文件存在并且设置了bReplaceFileIfExist，关闭文件并移除
    const QString& strFilePath = QDir::toNativeSeparators(strSaveDir + strFileName);
    if (QFile::exists(strFilePath))
    {
        if (request.bReplaceFileIfExist)
        {
            QString strFileErr;
            if (!removeFile(strFilePath, strFileErr))
            {
                strError = QStringLiteral("Error: QFile::remove(%1) - %2").arg(strFilePath).arg(strFileErr);
                qWarning() << strError;
                return pFile;
            }
        }
        else
        {
            strError = QStringLiteral("Error: File is already exist(%1)").arg(strFilePath);
            qWarning() << strError;
            return pFile;
        }
    }

    //创建并打开文件
#if defined(_MSC_VER) && _MSC_VER < 1700
    pFile.reset(new QFile(strFilePath));
#else
    pFile = std::make_unique<QFile>(strFilePath);
#endif
    if (!pFile->open(QIODevice::WriteOnly))
    {
        strError = QStringLiteral("Error: QFile::open(%1) - %2").arg(strFilePath).arg(pFile->errorString());
        qWarning() << strError;
        pFile.reset();
        return pFile;
    }
    return pFile;
}

bool NetworkUtility::readFileContent(const QString& strFilePath, QByteArray& bytes, QString& strError)
{
    strError.clear();
    if (QFile::exists(strFilePath))
    {
        QFile file(strFilePath);
        if (file.open(QIODevice::ReadOnly))
        {
            bytes = file.readAll();
            file.close();
            return true;
        }
        strError = QStringLiteral("Error: QFile::open(%1) - %2").arg(strFilePath).arg(file.errorString());
    }
    else
    {
        strError = QStringLiteral("Error: File is not exists(%1)").arg(strFilePath);
    }
    qDebug() << "[QMultiThreadNetwork]" << strError;
    return false;
}

QString NetworkUtility::createSharedRWFileWin32(const QMTNetwork::RequestTask& request, QString& strError, qint64 nDefaultFileSize)
{
    QString strCreatedFile;
    strError.clear();

#ifdef WIN32

    //取下载文件保存目录
    const QString& strSaveDir = getDownloadFileSaveDir(request, strError);
    if (strSaveDir.isEmpty())
    {
        return strCreatedFile;
    }

    //取下载保存的文件名
    const QString& strFileName = getDownloadFileSaveName(request);
    if (strSaveDir.isEmpty())
    {
        strError = QLatin1String("Error: fileName is empty!");
        qWarning() << strError;
        return strCreatedFile;
    }

    //如果文件存在并且设置了bReplaceFileIfExist，关闭文件并移除
    const QString& strFilePath = QDir::toNativeSeparators(strSaveDir + strFileName);
    if (QFile::exists(strFilePath))
    {
        if (request.bReplaceFileIfExist)
        {
            QString strFileErr;
            if (!removeFile(strFilePath, strFileErr))
            {
                strError = QStringLiteral("Error: QFile::remove(%1) - %2").arg(strFilePath).arg(strFileErr);
                qWarning() << strError;
                return strCreatedFile;
            }
        }
        else
        {
            strError = QStringLiteral("Error: File is already exist(%1)").arg(strFilePath);
            qWarning() << strError;
            return strCreatedFile;
        }
    }

    HANDLE hFile = CreateFileW(strFilePath.toStdWString().c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE)
    {
        if (nDefaultFileSize > 0)
        {
            LARGE_INTEGER li = { 0 };
            li.QuadPart = nDefaultFileSize;
            if (!SetFilePointerEx(hFile, li, &li, FILE_BEGIN))
            {
                strError = QStringLiteral("Error: SetFilePointerEx(%1)").arg(GetLastError());
                qWarning() << strError;
                return strCreatedFile;
            }
        }
        CloseHandle(hFile);
        strCreatedFile = strFilePath;
    }
    else
    {
        strError = QStringLiteral("Error: CreateFileW(%1)").arg(GetLastError());
        qWarning() << strError;
        return strCreatedFile;
    }

#else
    strError = QStringLiteral("Error: createSharedRWFileWin32() only support win32 platform.");
#endif
    return strFilePath;
}

QString NetworkUtility::getDownloadFileSaveName(const QMTNetwork::RequestTask& request)
{
    if (!request.strSaveFileName.isEmpty())
    {
        return request.strSaveFileName;
    }

    const QUrl& url = currentRequestUrl(request);
    if (!url.isValid())
    {
        return QString();
    }

    QString strFileName;
    // url中提取文件名，格式如：response-content-disposition=attachment; filename=test.exe
    QUrlQuery query(url.query(QUrl::FullyDecoded));
    const QList<QPair<QString, QString>>& querys = query.queryItems();
    foreach(auto pair, querys)
    {
        if (pair.first.compare("response-content-disposition", Qt::CaseInsensitive) == 0
            || pair.first.compare("content-disposition", Qt::CaseInsensitive) == 0)
        {
            const QStringList& listString = pair.second.split(";", QString::SkipEmptyParts);
            foreach(QString str, listString)
            {
                str = str.trimmed();
                if (str.startsWith(QString("filename="), Qt::CaseInsensitive))
                {
                    strFileName = str.right(str.size() - QString("filename=").size());

                    //文件名不能带 \/|":<>符号
                    QStringList strlst;
                    strlst << "\"" << ":" << "<" << ">" << "|" << "/" << "\\";
                    for (auto& str : strlst)
                    {
                        int index = strFileName.indexOf(str);
                        while (-1 != index)
                        {
                            strFileName.remove(index, str.length());
                            index = strFileName.indexOf(str);
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
    if (strFileName.isEmpty())
    {
        strFileName = url.fileName();
    }

    return strFileName;
}

QString NetworkUtility::getDownloadFileSaveDir(const QMTNetwork::RequestTask& request, QString& strError)
{
    strError.clear();
    QString strSaveDir = QDir::toNativeSeparators(request.strReqArg);
    if (!strSaveDir.isEmpty())
    {
        QDir dir;
        if (!dir.exists(strSaveDir) && !dir.mkpath(strSaveDir))
        {
            strError = QStringLiteral("Error: QDir::mkpath failed! Dir(%1)").arg(strSaveDir);
            qWarning() << strError;
            return QString();
        }
    }
    else
    {
        strError = QLatin1String("Error: RequestTask::strRequestArg is empty!");
        qWarning() << strError;
        return QString();
    }
    if (!strSaveDir.endsWith("\\"))
    {
        strSaveDir.append("\\");
    }
    return strSaveDir;
}

bool NetworkUtility::fileExists(QFile *pFile)
{
    return (nullptr != pFile && pFile->exists());
}

bool NetworkUtility::fileOpened(QFile *pFile)
{
    return (nullptr != pFile && pFile->exists() && pFile->isOpen());
}

bool NetworkUtility::removeFile(const QString& strFilePath, QString& errMessage)
{
    QFile file(strFilePath);
    if (fileExists(&file))
    {
        file.close();
        if (!file.remove())
        {
            errMessage = file.errorString();
            return false;
        }
    }
    return true;
}

QUrl NetworkUtility::currentRequestUrl(const QMTNetwork::RequestTask& request)
{
    QUrl url;
    if (!request.redirectUrl.isEmpty() && QUrl(request.redirectUrl).isValid())
    {
        url = QUrl(request.redirectUrl);
    }
    else if (!request.url.isEmpty() && QUrl(request.url).isValid())
    {
        url = QUrl(request.url);
    }
    return url;
}

const QString NetworkUtility::getTypeString(const RequestType eType)
{
    QString strType;
    switch (eType)
    {
    case RequestType::Download:
    {
        strType = QStringLiteral("下载");
    }
    break;
    case RequestType::MTDownload:
    {
        strType = QStringLiteral("多线程下载");
    }
    break;
    case RequestType::Upload:
    {
        strType = QStringLiteral("上传");
    }
    break;
    case RequestType::Get:
    {
        strType = QStringLiteral("GET");
    }
    break;
    case RequestType::Post:
    {
        strType = QStringLiteral("POST");
    }
    break;
    case RequestType::Put:
    {
        strType = QStringLiteral("PUT");
    }
    break;
    case RequestType::Delete:
    {
        strType = QStringLiteral("DELETE");
    }
    break;
    case RequestType::Head:
    {
        strType = QStringLiteral("HEAD");
    }
    break;
    default:
        break;
    }
    return strType;
}