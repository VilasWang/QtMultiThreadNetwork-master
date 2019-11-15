#include "networkuploadrequest.h"
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include "networkmanager.h"
#include "networkutility.h"
#include "networkevent.h"

using namespace QMTNetwork;

NetworkUploadRequest::NetworkUploadRequest(QObject *parent /* = nullptr */)
    : NetworkRequest(parent)
{
}

NetworkUploadRequest::~NetworkUploadRequest()
{
}

void NetworkUploadRequest::start()
{
    __super::start();

    const QUrl& url = NetworkUtility::currentRequestUrl(m_request);
    if (!url.isValid())
    {
        m_strError = QStringLiteral("Error: Invaild Url -").arg(url.toString());
        emit requestFinished(false, QByteArray(), m_strError);
        return;
    }

    QByteArray bytes;
    if (NetworkUtility::readFileContent(m_request.strReqArg, bytes, m_strError))
    {
        if (nullptr == m_pNetworkManager)
        {
            m_pNetworkManager = new QNetworkAccessManager;
        }
        m_pNetworkManager->connectToHost(url.host(), url.port());

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        request.setHeader(QNetworkRequest::ContentLengthHeader, bytes.length());
        request.setRawHeader("Connection", "keep-alive");
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
    const QUrl& url = NetworkUtility::currentRequestUrl(m_request);
    Q_ASSERT(url.isValid());

    if (isHttpProxy(url.scheme()) || isHttpsProxy(url.scheme()))
    {
        bSuccess = bSuccess && (statusCode >= 200 && statusCode < 300);
    }
    if (!bSuccess)
    {
        if (statusCode == 301 || statusCode == 302)
        {//301,302重定向
            const QVariant& redirectionTarget = m_pNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            const QUrl& redirectUrl = url.resolved(redirectionTarget.toUrl());
            if (redirectUrl.isValid() && url != redirectUrl && ++m_nRedirectionCount <= m_request.nMaxRedirectionCount)
            {
                m_request.redirectUrl = redirectUrl.toString();
                qDebug() << "[QMultiThreadNetwork] url:" << url.toString() << "redirectUrl:" << m_request.redirectUrl;

                m_pNetworkReply->deleteLater();
                m_pNetworkReply = nullptr;

                start();
                return;
            }
        }
        else if (statusCode != 200 && statusCode != 0)
        {
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

    int progress = iSent * 100 / iTotal;
    if (m_nProgress < progress)
    {
        m_nProgress = progress;
        NetworkProgressEvent *event = new NetworkProgressEvent;
        event->bDownload = false;
        event->uiId = m_request.uiId;
        event->uiBatchId = m_request.uiBatchId;
        event->iBtyes = iSent;
        event->iTotalBtyes = iTotal;
        QCoreApplication::postEvent(NetworkManager::globalInstance(), event);
    }
}
