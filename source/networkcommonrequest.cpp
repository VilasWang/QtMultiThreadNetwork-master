#include "networkcommonrequest.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include "networkutility.h"

using namespace QMTNetwork;

NetworkCommonRequest::NetworkCommonRequest(QObject *parent /* = nullptr */)
    : NetworkRequest(parent)
{
}

NetworkCommonRequest::~NetworkCommonRequest()
{
}

void NetworkCommonRequest::start()
{
    __super::start();

    const QUrl& url = NetworkUtility::currentRequestUrl(m_request);
    if (!url.isValid())
    {
        m_strError = QStringLiteral("Error: Invaild Url -").arg(url.toString());
        emit requestFinished(false, QByteArray(), m_strError);
        return;
    }

    if (isFtpProxy(url.scheme()))
    {
        if (m_request.eType == eTypePost
            || m_request.eType == eTypeDelete
            || m_request.eType == eTypeHead)
        {
            const QString& strType = NetworkUtility::getTypeString(m_request.eType);
            m_strError = QStringLiteral("Unsupported FTP request type[%1], url: %2").arg(strType).arg(url.url());
            qDebug() << "[QMultiThreadNetwork]" << m_strError;

            emit requestFinished(false, QByteArray(), m_strError);
            return;
        }
    }

    if (nullptr == m_pNetworkManager)
    {
        m_pNetworkManager = new QNetworkAccessManager;
    }
    //m_pNetworkManager->connectToHost(url.host(), url.port());

    QNetworkRequest request(url);

    auto iter = m_request.mapRawHeader.cbegin();
    for (; iter != m_request.mapRawHeader.cend(); ++iter)
    {
        request.setRawHeader(iter.key(), iter.value());
    }

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

    if (m_request.eType == eTypeGet)
    {
        m_pNetworkReply = m_pNetworkManager->get(request);
    }
    else if (m_request.eType == eTypePost)
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;");

        const QByteArray& bytes = m_request.strReqArg.toUtf8();
        request.setHeader(QNetworkRequest::ContentLengthHeader, bytes.length());

        m_pNetworkReply = m_pNetworkManager->post(request, bytes);
    }
    else if (m_request.eType == eTypePut)
    {
        const QByteArray& bytes = m_request.strReqArg.toUtf8();
        request.setHeader(QNetworkRequest::ContentLengthHeader, bytes.length());

        m_pNetworkReply = m_pNetworkManager->put(request, bytes);
    }
    else if (m_request.eType == eTypeDelete)
    {
        m_pNetworkReply = m_pNetworkManager->deleteResource(request);
    }
    else if (m_request.eType == eTypeHead)
    {
        m_pNetworkReply = m_pNetworkManager->head(request);
    }

    connect(m_pNetworkReply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    connect(m_pNetworkManager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)),
        SLOT(onAuthenticationRequired(QNetworkReply *, QAuthenticator *)));
}

void NetworkCommonRequest::onFinished()
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
        if (bSuccess && m_request.eType == eTypeHead)
        {
            QString headers;
            foreach(const QByteArray& header, m_pNetworkReply->rawHeaderList())
            {
                headers.append(QString("%1 -> %2").arg(QString::fromUtf8(header))
                    .arg(QString::fromUtf8(m_pNetworkReply->rawHeader(header))));
                headers.append("\n");
            }
            bytes = headers.toUtf8();
        }
        else
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
    }
    emit requestFinished(bSuccess, bytes, m_strError);

    m_pNetworkReply->deleteLater();
    m_pNetworkReply = nullptr;
}
