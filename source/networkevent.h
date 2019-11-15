#ifndef NETWORKEVENT_H
#define NETWORKEVENT_H

#include <QEvent>
#include <map>

namespace QMTNetwork {

    struct RequestTask;
    // 网络请求模块事件
    namespace NetworkEvent
    {
        template <typename T>
        int regiesterEvent(const T& eventName)
        {
            static_assert(std::is_integral<T>::value
                || std::is_base_of<std::string, T>::value
                || std::is_base_of<std::wstring, T>::value
                || std::is_base_of<QString, T>::value
                || std::is_base_of<QLatin1String, T>::value, "T is a unsupported Type.");

            typedef std::map<T, int> UserEventMap;
            static UserEventMap s_mapUserEvent;

            UserEventMap::const_iterator iter = s_mapUserEvent.find(eventName);
            if (iter != s_mapUserEvent.cend())
            {
                return iter->second;
            }

            int nEventType = QEvent::registerEventType();
            s_mapUserEvent[eventName] = nEventType;
            return nEventType;
        }

        const QEvent::Type WaitForIdleThread = (QEvent::Type)regiesterEvent(QLatin1String("WaitForIdleThread"));
        const QEvent::Type ReplyResult = (QEvent::Type)regiesterEvent(QLatin1String("ReplyResult"));
        const QEvent::Type NetworkProgress = (QEvent::Type)regiesterEvent(QLatin1String("NetworkProgress"));
    }

    //等待空闲线程事件
    class WaitForIdleThreadEvent : public QEvent
    {
    public:
        WaitForIdleThreadEvent() : QEvent(QEvent::Type(NetworkEvent::WaitForIdleThread)) {}
    };

    //通知结果事件
    class ReplyResultEvent : public QEvent
    {
    public:
        ReplyResultEvent() : QEvent(QEvent::Type(NetworkEvent::ReplyResult)), bDestroyed(true) {}

        RequestTask request;
        bool bDestroyed;
    };

    //下载/上传进度事件
    class NetworkProgressEvent : public QEvent
    {
    public:
        NetworkProgressEvent() : QEvent(QEvent::Type(NetworkEvent::NetworkProgress))
            , bDownload(true)
            , uiId(0)
            , uiBatchId(0)
            , iBtyes(0)
            , iTotalBtyes(0)
        {
        }

        bool bDownload;
        quint64 uiId;
        quint64 uiBatchId;
        qint64 iBtyes;
        qint64 iTotalBtyes;
    };
}

#endif ///NETWORKEVENT_H