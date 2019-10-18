#include "networkrequest.h"
#include <QDebug>
#include "classmemorytracer.h"
#include "networkdownloadrequest.h"
#include "networkuploadrequest.h"
#include "networkcommonrequest.h"
#include "networkmtdownloadrequest.h"

using namespace QMTNetwork;

NetworkRequest::NetworkRequest(QObject *parent)
    : QObject(parent)
    , m_bAbortManual(false)
    , m_pNetworkManager(nullptr)
    , m_pNetworkReply(nullptr)
    , m_nProgress(0)
    , m_nRedirectionCount(0)
{
    TRACE_CLASS_CONSTRUCTOR(NetworkRequest);
}

NetworkRequest::~NetworkRequest()
{
    TRACE_CLASS_DESTRUCTOR(NetworkRequest);

    abort();
    if (m_pNetworkManager)
    {
        m_pNetworkManager->deleteLater();
        m_pNetworkManager = nullptr;
    }
}

void NetworkRequest::abort()
{
    m_bAbortManual = true;
    if (m_pNetworkReply)
    {
        if (m_pNetworkReply->isRunning())
        {
            m_pNetworkReply->abort();
        }
        m_pNetworkReply->deleteLater();
        m_pNetworkReply = nullptr;
    }
}

void NetworkRequest::start()
{
    m_bAbortManual = false;
    m_nProgress = 0;
}

void NetworkRequest::onError(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code);

    m_strError = m_pNetworkReply->errorString();
    qDebug() << "[QMultiThreadNetwork] Error" << QString("[%1]").arg(getRequestTypeString(m_request.eType)) << m_strError;
}

void NetworkRequest::onAuthenticationRequired(QNetworkReply *r, QAuthenticator *a)
{
    Q_UNUSED(a);
    qDebug() << "[QMultiThreadNetwork] Authentication Required." << r->readAll();
}


std::unique_ptr<NetworkRequest> NetworkRequestFactory::create(const RequestType& eType)
{
    std::unique_ptr<NetworkRequest> pRequest;
    switch (eType)
    {
    case eTypeDownload:
    {
#if defined(_MSC_VER) && _MSC_VER < 1700
        pRequest.reset(new NetworkDownloadRequest());
#else
        pRequest = std::make_unique<NetworkDownloadRequest>();
#endif
    }
    break;
    case eTypeMTDownload:
    {
#if defined(_MSC_VER) && _MSC_VER < 1700
        pRequest.reset(new NetworkMTDownloadRequest());
#else
        pRequest = std::make_unique<NetworkMTDownloadRequest>();
#endif
    }
    break;
    case eTypeUpload:
    {
#if defined(_MSC_VER) && _MSC_VER < 1700
        pRequest.reset(new NetworkUploadRequest());
#else
        pRequest = std::make_unique<NetworkUploadRequest>();
#endif
    }
    break;
    case eTypePost:
    case eTypeGet:
    case eTypePut:
    case eTypeDelete:
    case eTypeHead:
    {
#if defined(_MSC_VER) && _MSC_VER < 1700
        pRequest.reset(new NetworkCommonRequest());
#else
        pRequest = std::make_unique<NetworkCommonRequest>();
#endif
    }
    break;
    /*New type add to here*/
    default:
        break;
    }
    return pRequest;
}