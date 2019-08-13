#include "networkuploadrequest.h"
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include "Log4cplusWrapper.h"
#include "networkmanager.h"


NetworkUploadRequest::NetworkUploadRequest(QObject *parent /* = nullptr */)
    : NetworkRequest(parent)
{
}

NetworkUploadRequest::~NetworkUploadRequest()
{
}

bool NetworkUploadRequest::readLocalFile(const QString& strFilePath, QByteArray& bytes)
{
    m_strError.clear();
    if (QFile::exists(strFilePath))
    {
        QFile file(strFilePath);
        if (file.open(QIODevice::ReadOnly))
        {
            bytes = file.readAll();
            file.close();
            return true;
        }
        m_strError = QStringLiteral("Error: QFile::open(%1) - %2").arg(strFilePath).arg(file.errorString());
    }
    else
    {
        m_strError = QStringLiteral("Error: File is not exists(%1)").arg(strFilePath);
    }
    LOG_INFO(m_strError.toStdWString());
    qDebug() << "[QMultiThreadNetwork]" << m_strError;
    return false;
}

void NetworkUploadRequest::start()
{
    __super::start();

    QByteArray bytes;
    if (readLocalFile(m_request.strReqArg, bytes))
    {
        QUrl url;
        if (!redirected())
        {
            url = m_request.url;
        }
        else
        {
            url = m_redirectUrl;
        }

        if (nullptr == m_pNetworkManager)
        {
            m_pNetworkManager = new QNetworkAccessManager;
        }
        m_pNetworkManager->connectToHost(url.host(), url.port());

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        request.setHeader(QNetworkRequest::ContentLengthHeader, bytes.length());
        auto iter = m_request.mapRawHeader.cbegin();
        for (; iter != m_request.mapRawHeader.cend(); ++iter)
        {
            request.setRawHeader(iter.key(), iter.value());
        }

        if (isFtpProxy(url.scheme()))
        {
            m_pNetworkReply = m_pNetworkManager->put(request, bytes);
        }
        else // http / https
        {
#ifndef QT_NO_SSL
            if (isHttpsProxy(url.scheme()))
            {
                // 发送https请求前准备工作;
                QSslConfiguration conf = request.sslConfiguration();
                conf.setPeerVerifyMode(QSslSocket::VerifyNone);
                conf.setProtocol(QSsl::TlsV1SslV3);
                request.setSslConfiguration(conf);
            }
#endif
            if (m_request.bUploadUsePut)
            {
                m_pNetworkReply = m_pNetworkManager->put(request, bytes);
            }
            else
            {
                m_pNetworkReply = m_pNetworkManager->post(request, bytes);
            }
        }

        connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(onFinished()));
        connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
        connect(m_pNetworkManager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)),
            SLOT(onAuthenticationRequired(QNetworkReply *, QAuthenticator *)));
        if (m_request.bShowProgress)
        {
            connect(m_pNetworkReply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(onUploadProgress(qint64, qint64)));
        }
    }
    else
    {
        emit requestFinished(false, QByteArray(), m_strError);
    }
}

void NetworkUploadRequest::onFinished()
{
    bool bSuccess = (m_pNetworkReply->error() == QNetworkReply::NoError);
    int statusCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (isHttpProxy(m_request.url.scheme()) || isHttpsProxy(m_request.url.scheme()))
    {
        bSuccess = bSuccess && (statusCode >= 200 && statusCode < 300);
    }
    if (!bSuccess)
    {
        if (statusCode == 301 || statusCode == 302)
        {//301,302重定向
            const QVariant& redirectionTarget = m_pNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if (!redirectionTarget.isNull())
            {
                const QUrl& url = m_request.url;
                const QUrl& redirectUrl = url.resolved(redirectionTarget.toUrl());
                if (url != redirectUrl && m_redirectUrl != redirectUrl)
                {
                    m_redirectUrl = redirectUrl;
                    if (m_redirectUrl.isValid())
                    {
                        LOG_INFO("url: " << url.toString().toStdWString() << "; redirectUrl:" << m_redirectUrl.toString().toStdWString());
                        qDebug() << "[QMultiThreadNetwork] url:" << url.toString() << "redirectUrl:" << m_redirectUrl.toString();

                        m_pNetworkReply->deleteLater();
                        m_pNetworkReply = nullptr;

                        start();
                        return;
                    }
                }
            }
        }
        else if (statusCode != 200 && statusCode != 0)
        {
            LOG_INFO("HttpStatusCode: " << statusCode);
            qDebug() << "[QMultiThreadNetwork] HttpStatusCode: " << statusCode;
        }
    }

    QByteArray bytes;
    if (!m_bAbortManual)//非调用abort()结束
    {
        if (m_pNetworkReply->isOpen())
        {
            if (bSuccess)
            {
                bytes = m_pNetworkReply->readAll();
            }
            else
            {
                m_strError.append(QString::fromUtf8(m_pNetworkReply->readAll()));
            }
        }
    }
    emit requestFinished(bSuccess, bytes, m_strError);

    m_pNetworkReply->deleteLater();
    m_pNetworkReply = nullptr;
}

void NetworkUploadRequest::onUploadProgress(qint64 iSent, qint64 iTotal)
{
    if (m_bAbortManual || iSent <= 0 || iTotal <= 0)
        return;

    if (NetworkManager::isInstantiated())
    {
        NetworkProgressEvent *event = new NetworkProgressEvent;
        event->bDownload = false;
        event->uiId = m_request.uiId;
        event->uiBatchId = m_request.uiBatchId;
        event->iBtyes = iSent;
        event->iTotalBtyes = iTotal;
        QCoreApplication::postEvent(NetworkManager::globalInstance(), event);
    }
}
