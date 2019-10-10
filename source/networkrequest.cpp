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
#if _MSC_VER >= 1700
        pRequest = std::make_unique<NetworkDownloadRequest>();
#else
        pRequest.reset(new NetworkDownloadRequest());
#endif
    }
    break;
    case eTypeMTDownload:
    {
#if _MSC_VER >= 1700
        pRequest = std::make_unique<NetworkMTDownloadRequest>();
#else
        pRequest.reset(new NetworkMTDownloadRequest());
#endif
    }
    break;
    case eTypeUpload:
    {
#if _MSC_VER >= 1700
        pRequest = std::make_unique<NetworkUploadRequest>();
#else
        pRequest.reset(new NetworkUploadRequest());
#endif
    }
    break;
    case eTypePost:
    case eTypeGet:
    case eTypePut:
    case eTypeDelete:
    case eTypeHead:
    {
#if _MSC_VER >= 1700
        pRequest = std::make_unique<NetworkCommonRequest>();
#else
        pRequest.reset(new NetworkCommonRequest());
#endif
    }
    break;
    /*New type add to here*/
    default:
        break;
    }
    return pRequest;
}