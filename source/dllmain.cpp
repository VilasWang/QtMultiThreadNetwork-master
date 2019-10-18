// dllmain.cpp : Defines the entry point for the DLL application.

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef LOG_USELOG4CPLUS
#define IMPLEMENT_LOG4CPLUS
#include "Log4cplusWrapper.h"
#include <QDateTime>
#include <QDir>

///函数前置声明
/** @fn ModuleLogInit
@brief 模块日志初始化
*/
extern void ModuleLogInit();

/** @fn ModuleLogFini
@brief 模块日志扫尾
*/
extern void ModuleLogFini();
#endif //LOG_USELOG4CPLUS


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    (void)hModule;
    (void)lpReserved;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
#ifdef LOG_USELOG4CPLUS
        ///日志记录初始化
        ModuleLogInit();
#endif //LOG_USELOG4CPLUS
    }
    break;
    case DLL_THREAD_ATTACH:
    {
    }
    break;
    case DLL_THREAD_DETACH:
    {
    }
    break;
    case DLL_PROCESS_DETACH:
    {
#ifdef LOG_USELOG4CPLUS
        ///日志记录扫尾
        ModuleLogFini();
#endif //LOG_USELOG4CPLUS
    }
    break;
    default:
    {
        ///do nothing...
    }
    break;
    }
    return TRUE;
}

#ifdef LOG_USELOG4CPLUS
void ModuleLogInit()
{
    ///日志模块初始化代码
    ///1.要打开日志记录功能:工程中预定义宏定义中加上宏定义:LOG_USELOG4CPLUS
    ///2.关闭日志记录功能:工程中预定义宏定义中去除宏定义:LOG_USELOG4CPLUS

    ///设置日志配置文件名
    LOG_PARAM_LOG_CFG_FILE_NAME("QMultiThreadNetwork.log4cplus");
    ///设置存放日志文件的目录
    QString strPath = QString("log/%1").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    QDir dir;
    if (!dir.exists(strPath))
    {
        dir.mkpath(strPath);
    }
    LOG_PARAM_LOG_PATH(strPath.toStdString().c_str());
    ///设置生成日志文件名
    LOG_PARAM_LOG_FILE_NAME("QMultiThreadNetwork.log");
    ///设置日志级别
    LOG_PARAM_LOG_LEVEL("unknown");
    ///LOG_PARAM_LOG_LEVEL("INFO");
    ///设置log4cplus logger name
    LOG_PARAM_LOG_LOGGER_NAME("QMultiThreadNetwork");
    ///设置log4cplus App name,设置应用程序名字
    ///设置log4cplus App name的名字，若未设置日志文件的目录，则Vista以上版本操作系统创建的log目录可以放在应用程序名字所在的目录下
    LOG_PARAM_LOG_APP_NAME("QMultiThreadNetwork");
    ///带参数日志模块初始化，以上所有设置了的参数都将生效，没有设置的采用缺省值
    LOG_INIT_WITH_PARAM_MULTI_PROCESS();

    LOG_CRIT("初始化日志模块");
}

void ModuleLogFini()
{
    ///日志模块初扫尾代码
    LOG_CRIT("关闭日志模块");
    LOG_FINI();
}
#endif //LOG_USELOG4CPLUS

#endif //WIN32
