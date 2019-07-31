/***********************************************************

1缺省日志初始化

    ///下面为main.cpp文件中内容
    ///LOG4CPLUS_ENABLE最好在工程预定义宏定义中定义,下面只是测试用
    #define LOG4CPLUS_ENABLE

    ///IMPLEMENT_LOG4CPLUS必须有一个cpp文件定义该宏定义
    #define IMPLEMENT_LOG4CPLUS
    #include "log4c_amalgamation.h"

    int main(int argc, char **argv)
    {
        LOG4C_INIT_DEFAULT();
        LOG_NOTICE("HELLO");
    }

2.带参数设置日志初始化

    ///加入日志模块,包含日志封装代码
    #define IMPLEMENT_LOG4CPLUS
    #include "Log4cplusWrapper.h"

    void ModuleLogInit()
    {
        ///日志模块初始化代码
        ///1.要打开日志记录功能:工程中预定义宏定义中加上宏定义:LOG4CPLUS_ENABLE
        ///2.关闭日志记录功能:工程中预定义宏定义中去除宏定义:LOG4CPLUS_ENABLE

        ///设置日志配置文件名
        LOG_PARAM_LOG_CFG_FILE_NAME("Net.log4cplus");
        ///设置生成日志文件名
        LOG_PARAM_LOG_FILE_NAME("NetLog.log");
        ///设置存放日志文件的目录
        LOG_PARAM_LOG_PATH("C:\Temp");
        ///设置日志级别
        ///LOG_PARAM_LOG_LEVEL("unknown");
        LOG_PARAM_LOG_LEVEL("INFO");
        ///设置是否立刻Flush
        LOG_PARAM_LOG_IMMEDIATE_FLUSH(FALSE);
        ///设置文件输出缓冲区
        LOG_PARAM_LOG_BUFFER_SIZE(1024*1024);
        ///设置log4cplus logger name
        LOG_PARAM_LOG_LOGGER_NAME("Net");
        ///设置log4cplus APP name,设置应用程序名字
        ///设置log4cplus App name的名字，Vista以上版本操作系统创建的log目录可以放在应用程序名字所在的目录下
        LOG_PARAM_LOG_APP_NAME("KeepInTouch");
        ///带参数日志模块初始化，以上所有设置了的参数都将生效，没有设置的采用缺省值
        LOG_INIT_WITH_PARAM_MULTI_PROCESS();

        LOG_CRIT("初始化日志模块");
        LOG_CRIT("KIT IM版本号:" << FileHelper::GetModuleFileVersionAsStringEfficiently()
            << ", 版本创建时间:" << FileHelper::GetModuleFileCreateTimeEfficiently()
            << ", 编译日期:" << __DATE__
            << ", 编译时间:" << __TIME__
        );
    }

**********************************************************/

#if !defined(AFX_LOG4CPLUSWRAPPER_H__DD58A78D_C125_410F_B4C8_F0067B797121__INCLUDED_)
#define AFX_LOG4CPLUSWRAPPER_H__DD58A78D_C125_410F_B4C8_F0067B797121__INCLUDED_

///禁用一些编译警告信息
#pragma warning(push)  
#pragma warning(disable:4996 4090 4018) 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///下面定义编译时温馨提示
#if !defined(LOG4CPLUS_ENABLE)

#define PTODO_LINENUMBER_TO_STRING(x) #x
#define PTODO_LINENUMBER(x) PTODO_LINENUMBER_TO_STRING(x)
#define INCLUDE_FILE_AND_LINE(string) __FILE__"(" PTODO_LINENUMBER(__LINE__) "): "string

///#pragma message (INCLUDE_FILE_AND_LINE("温馨提示:只有定义了预编译开关定义了LOG4CPLUS_ENABLE宏命令才使用log4cplus日志记录系统"))
#ifdef __cplusplus
#pragma message (INCLUDE_FILE_AND_LINE("温馨提示:只有定义了预编译开关定义了LOG4CPLUS_ENABLE宏命令才使用log4cplus日志记录系统"))
#else
#pragma message (INCLUDE_FILE_AND_LINE("温馨提示:只有定义了预编译开关定义了LOG4CPLUS_ENABLE宏命令才使用log4cplus日志记录系统"))
#endif

#endif ///!defined(LOG4CPLUS_ENABLE)


//////////////////////下面为log4cplus日志记录头文件包含////////////////////
#if defined(LOG4CPLUS_ENABLE)
///头文件包含log4cplus基本框架

///Include .h file 
#include <log4cplus/logger.h> 
#include <log4cplus/fileappender.h> 
#include <log4cplus/consoleappender.h> 
#include <log4cplus/layout.h> 
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/version.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

///Link Lib
#ifdef LOG4CPLUS_STATIC

#ifdef _DEBUG 
#ifdef UNICODE
#pragma comment(lib, "log4cplusMTd.lib")
#else
#pragma comment(lib, "log4cplusMTd.lib")
#endif //UNICODE
#else 
#ifdef UNICODE
#pragma comment(lib, "log4cplusMT.lib") 
#else
#pragma comment(lib, "log4cplusMT.lib")		
#endif //UNICODE
#endif //_DEBUG 

#else

#ifdef _DEBUG 
#ifdef UNICODE
#pragma comment(lib, "log4cplusd.lib")
#else
#pragma comment(lib, "log4cplusd.lib")
#endif //UNICODE
#else 
#ifdef UNICODE
#pragma comment(lib, "log4cplus.lib") 
#else
#pragma comment(lib, "log4cplus.lib")		
#endif //UNICODE
#endif //_DEBUG 

#endif //LOG4CPLUS_STATIC


///disable macros
///设置可以将某个级别级别以下的LOG代码全部不生成

#if defined(LOG_DISABLE_FATAL) && !defined(LOG_DISABLE_ERROR)
#define LOG_DISABLE_ERROR
#endif

#if defined(LOG_DISABLE_ERROR) && !defined(LOG_DISABLE_WARN)
#define LOG_DISABLE_WARN
#endif

#if defined(LOG_DISABLE_WARN) && !defined(LOG_DISABLE_INFO)
#define LOG_DISABLE_INFO
#endif

#if defined(LOG_DISABLE_INFO) && !defined(LOG_DISABLE_DEBUG)
#define LOG_DISABLE_DEBUG
#endif

#if defined(LOG_DISABLE_DEBUG) && !defined(LOG_DISABLE_TRACE)
#define LOG_DISABLE_TRACE
#endif

///兼容log4cplus的一些宏定义
#if defined(LOG4CPLUS_DISABLE_FATAL) && !defined(LOG_DISABLE_ERROR)
#define LOG_DISABLE_ERROR
#endif

#if defined(LOG4CPLUS_DISABLE_ERROR) && !defined(LOG_DISABLE_WARN)
#define LOG_DISABLE_WARN
#endif

#if defined(LOG4CPLUS_DISABLE_WARN) && !defined(LOG_DISABLE_INFO)
#define LOG_DISABLE_INFO
#endif

#if defined(LOG4CPLUS_DISABLE_INFO) && !defined(LOG_DISABLE_DEBUG)
#define LOG_DISABLE_DEBUG
#endif

#if defined(LOG4CPLUS_DISABLE_DEBUG) && !defined(LOG_DISABLE_TRACE)
#define LOG_DISABLE_TRACE
#endif

#endif ///#if defined(LOG4CPLUS_ENABLE)


//////////////////////下面为C++日志记录头文件包含///////////////////////////

#if defined(__cplusplus)
/************** Begin of config_log.h *******************************************/
/*
* config_log.h
*
* 下面两个目录各有本文件的一个拷贝,两个文件是一样的
*		apache-log4cplus-0.10.0\apache-log4cplus-0.10.0\src\main\include\
*		apache-log4cplus-0.10.0\apache-log4cplus-0.10.0\src\main\Log4cxxWrapper\
* 日志配置头文件
* 通过预编译头文件LOG4CPLUS_ENABLE来控制是否使用log4cplus日志记录系统,
* 若没有定义LOG4CPLUS_ENABLE预编译开关的话则不适用log4cplus的日志记录系统
* 要使用log4cplus日志记录系统时，可在本文件中定义LOG4CPLUS_ENABLE宏定义，
*/
#if !defined(AFX_CONFIG_LOG_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_)
#define AFX_CONFIG_LOG_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///只有定义了LOG4CPLUS_ENABLE宏命令才使用log4cplus日志记录系统
#if defined(LOG4CPLUS_ENABLE)  && defined(__cplusplus)
///#include "log4c_cxxWrapper.h"

///log4cplus缺省Logger名, loggerNameDefault 简写为LD
#ifndef LOGGER_DEFAULT_NAME
#define LOGGER_DEFAULT_NAME		LOG4CPLUS_TEXT("LD")
#endif

#ifndef APPENDER_DEFAULT_NAME
#define APPENDER_DEFAULT_NAME	("AppenderNameRollingFile")
#endif

#ifndef LOG4C_BUFFER_SIZE_DEFAULT
#define LOG4C_BUFFER_SIZE_DEFAULT  (1024*9)
#endif

#ifndef LOG4C_BUFFER_SIZE_MAX
#define LOG4C_BUFFER_SIZE_MAX	(1024*10)
#endif

#ifndef DEFAULT_LOG_CATEGORY_NAME
#define DEFAULT_LOG_CATEGORY_NAME "root"
#endif

/**
* Predefined Levels of priorities. These correspond to the priority levels
* used by syslog(3).
**/
enum ELog4cplusLevelType
{
    /** fatal */	LOG_LEVEL_FATAL = log4cplus::FATAL_LOG_LEVEL,
    /** alert */	LOG_LEVEL_ALERT = LOG_LEVEL_FATAL - 1000,
    /** crit */	    LOG_LEVEL_CRIT = LOG_LEVEL_ALERT - 1000,
    /** error */	LOG_LEVEL_ERROR = log4cplus::ERROR_LOG_LEVEL,
    /** warn */	    LOG_LEVEL_WARN = log4cplus::WARN_LOG_LEVEL,
    /** notice */	LOG_LEVEL_NOTICE = LOG_LEVEL_WARN - 1000,
    /** info */	    LOG_LEVEL_INFO = log4cplus::INFO_LOG_LEVEL,
    /** debug */	LOG_LEVEL_DEBUG = log4cplus::DEBUG_LOG_LEVEL,
    /** trace */	LOG_LEVEL_TRACE = log4cplus::TRACE_LOG_LEVEL,
    /** all */		LOG_LEVEL_ALL = LOG_LEVEL_TRACE,
    /** notset */	LOG_LEVEL_NOTSET = log4cplus::NOT_SET_LOG_LEVEL,
    /** unknown */	LOG_LEVEL_UNKNOWN = LOG_LEVEL_NOTSET,
};

/** 输出文件名和行号的日志输出宏
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG4CPLUS_LOG宏定义中
*/
#define LOG4CPLUS(level, message) \
do \
{ \
	if( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		using namespace std; \
		std::ostrstream ostrs; \
		ostrs << message << ends; \
		CLogWrapper::Instance()->LogMsg(__FILE__, __LINE__, level, ostrs.str(), CLogWrapper::GetDefaultLogger()); \
		ostrs.rdbuf()->freeze(0); \
	} \
} while (0);

/** 输出文件名和行号的日志输出宏
方便用户使用再定义一个新的宏定义
*/
#define LOG LOG4CPLUS

/** 不输出文件名和行号的日志输出宏
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG4CPLUS_LOG宏定义中
*/
#define LOG4CPLUS_NO_FILENUM(level, message) \
do \
{ \
	if( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		using namespace std; \
		std::ostrstream ostrs; \
		ostrs << message << ends; \
		CLogWrapper::Instance()->LogMsg(level, ostrs.str(), CLogWrapper::GetDefaultLogger()); \
		ostrs.rdbuf()->freeze(0); \
	} \
} while (0);

/** 不输出文件名和行号的日志输出宏
方便用户使用再定义一个新的宏定义
*/
#define LOG_NO_FILENUM LOG4CPLUS_NO_FILENUM	

/** 不输出文件名和行号和不进行任何格式转换(换行也没有)字符串原样输出的日志输出宏
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG4CPLUS_LOG宏定义中
*/
#define LOG_NO_FILENUM_NO_LAYOUT(level, message) \
do \
{ \
	if( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		using namespace std; \
		std::ostrstream ostrs; \
		ostrs << message << ends; \
		CLogWrapper::Instance()->LogMsgNoFileNumAndNoLayout(level, ostrs.str(), CLogWrapper::GetDefaultLogger()); \
		ostrs.rdbuf()->freeze(0); \
	} \
} while (0);

/** 不输出文件名和行号和不进行任何格式转换(换行也没有)字符串原样输出的日志输出宏
方便用户使用再定义一个新的宏定义
*/
#define LOG_ORIGIN LOG_NO_FILENUM_NO_LAYOUT	

/** Trace the execution of a line.
This macro outputs a trace of a source file line execution.
*/
#define LOG_LINE() \
do \
{ \
	LOG4CPLUS(LOG_LEVEL_TRACE, "line:" << __LINE__); \
} \
while(0);

/**将内存中数据以十六进制方式打印出来的一个宏定义
*/
#define LOG_HEX_DUMP(level, ptrBuffer, lBufferLen)	\
do \
{ \
	if( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{	\
		char *ptrStr = sprintf_data((char*)(ptrBuffer), (lBufferLen));\
		LOG(level, ptrStr );\
		FREEP(ptrStr);\
	} \
} \
while(0);


#define LOG_PRINTF(X) \
do \
{ \
	const int iLogLevel = CLogWrapper::Instance()->GetPriorityWrapper(X); \
	if((CLogWrapper::GetDefaultLogger()).isEnabledFor(iLogLevel)) \
	{ \
		char * strMsg = (char *)CLogWrapper::Instance()->LogVsnprintfWrapper(X); \
		CLogWrapper::Instance()->LogMsg( __FILE__, __LINE__, iLogLevel, strMsg, CLogWrapper::GetDefaultLogger()); \
		FREEP(strMsg); \
	}\
} \
while(0);

/** Output trace on condition.
This macro outputs a trace of any information needed, using standard stream
output operators. The output is only made if the conditional is TRUE.
*/
#define LOG_IF(level, cond, message) \
do \
{ \
	if (cond) \
	{ \
		LOG4CPLUS(level, message); \
	} \
} \
while(0);

/** 输出文件名和行号的日志输出宏,记录完日志直接return
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG宏定义中
*/
#define LOG_RETURN(level, message) \
{ \
	LOG(level, message); \
	return; \
}

/** 输出文件名和行号的日志输出宏,记录完日志直接return returnValue;
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG宏定义中
*/
#define LOG_RETURN_WITH_VALUE(level, message, returnValue) \
{ \
	LOG(level, message); \
	return (returnValue); \
}

/** 输出文件名和行号的日志输出宏,记录完日志直接break
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG宏定义中
*/
#define LOG_BREAK(level, message) \
{ \
	LOG(level, message); \
	break; \
}

/** 输出文件名和行号的日志输出宏,记录完日志直接continue
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG宏定义中
*/
#define LOG_CONTINUE(level, message) \
{ \
	LOG(level, message); \
	continue; \
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Trace an execution block.
This macro creates a trace variable for tracking the entry and exit of program
blocks. It creates an instance of the PTraceBlock class that will output a
trace message at the line PTRACE_BLOCK is called and then on exit from the
scope it is defined in.
*/
#define LOG_BLOCK(name) \
	using namespace std; \
	std::ostrstream ostrsLogBlock; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogBlock << name << ends; \
	} \
	CLogWrapper::CBlock __trace_block_instance(__FILE__, __LINE__, ostrsLogBlock.str(), ostrsLogBlock.str()); \
	ostrsLogBlock.rdbuf()->freeze(0); 

/** Trace an execution block.(可以设置Block的BeginName和EndName)
This macro creates a trace variable for tracking the entry and exit of program
blocks. It creates an instance of the PTraceBlock class that will output a
trace message at the line PTRACE_BLOCK is called and then on exit from the
scope it is defined in.
*/
#define LOG_BLOCK_EXT(BeginName, EndName) \
	using namespace std; \
	std::ostrstream ostrsLogBlockBegin; \
	std::ostrstream ostrsLogBlockEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogBlockBegin << BeginName << ends; \
		ostrsLogBlockEnd << EndName << ends; \
	} \
	CLogWrapper::CBlock __trace_block_ext_instance(__FILE__, __LINE__, ostrsLogBlockBegin.str(), ostrsLogBlockEnd.str()); \
	ostrsLogBlockBegin.rdbuf()->freeze(0); \
	ostrsLogBlockEnd.rdbuf()->freeze(0);

/** @macro LOG_BLOCK_B
@brief 记录函数进出的日志
@details 函数进入时为详细信息(带message)，函数退出时为简单信息(不带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_BLOCK_BEGIN这里简写为LOG_BLOCK_B
*/
#define LOG_BLOCK_B(message) \
	using namespace std; \
	std::ostrstream ostrsLogBlockBeginBegin; \
	std::ostrstream ostrsLogBlockBeginEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogBlockBeginBegin << message << ends; \
		ostrsLogBlockBeginEnd << "Block End" << ends; \
	} \
	CLogWrapper::CBlock __trace_block_begin_instance(__FILE__, __LINE__, ostrsLogBlockBeginBegin.str(), ostrsLogBlockBeginEnd.str()); \
	ostrsLogBlockBeginBegin.rdbuf()->freeze(0); \
	ostrsLogBlockBeginEnd.rdbuf()->freeze(0);

/** @macro LOG_BLOCK_E
@brief 记录函数进出的日志
@details 函数进入时为简单信息(不带message)，函数退出时为详细信息(带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_BLOCK_END这里简写为LOG_BLOCK_E
*/
#define LOG_BLOCK_E(message) \
	using namespace std; \
	std::ostrstream ostrsLogBlockEndBegin; \
	std::ostrstream ostrsLogBlockEndEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogBlockEndBegin << "Block Begin" << ends; \
		ostrsLogBlockEndEnd << message << ends; \
	} \
	CLogWrapper::CBlock __trace_block_end_instance(__FILE__, __LINE__, ostrsLogBlockEndBegin.str(), ostrsLogBlockEndEnd.str()); \
	ostrsLogBlockEndBegin.rdbuf()->freeze(0); \
	ostrsLogBlockEndEnd.rdbuf()->freeze(0);

/** Trace an execution block.
LOG_LIGHTBLOCK功能和LOG_BLOCK类似，只是不采用加锁为线程不安全的，但增加了效率
This macro creates a trace variable for tracking the entry and exit of program
blocks. It creates an instance of the PTraceBlock class that will output a
trace message at the line PTRACE_BLOCK is called and then on exit from the
scope it is defined in.
*/
#define LOG_LIGHTBLOCK(name) \
	using namespace std; \
	std::ostrstream ostrsLogLightBlock; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogLightBlock << name << ends; \
	} \
	CLogWrapper::CLightBlock __trace_lightblock_instance(__FILE__, __LINE__, ostrsLogLightBlock.str(), ostrsLogLightBlock.str()); \
	ostrsLogLightBlock.rdbuf()->freeze(0); 

/** Trace an execution block.(可以设置Block的BeginName和EndName)
LOG_LIGHTBLOCK_EXT功能和LOG_BLOCK_EXT类似，只是不采用加锁为线程不安全的，但增加了效率
This macro creates a trace variable for tracking the entry and exit of program
blocks. It creates an instance of the PTraceBlock class that will output a
trace message at the line PTRACE_BLOCK is called and then on exit from the
scope it is defined in.
*/
#define LOG_LIGHTBLOCK_EXT(BeginName, EndName) \
	using namespace std; \
	std::ostrstream ostrsLogLightBlockBegin; \
	std::ostrstream ostrsLogLightBlockEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogLightBlockBegin << BeginName << ends; \
		ostrsLogLightBlockEnd << EndName << ends; \
	} \
	CLogWrapper::CLightBlock __trace_light_block_ext_instance(__FILE__, __LINE__, ostrsLogLightBlockBegin.str(), ostrsLogLightBlockEnd.str()); \
	ostrsLogLightBlockBegin.rdbuf()->freeze(0); \
	ostrsLogLightBlockEnd.rdbuf()->freeze(0);

/** Trace an execution block.(若message中为变量时，支持进出时都输出变量最新的值)
This macro creates a trace variable for tracking the entry and exit of program
blocks. It creates an instance of the PTraceBlock class that will output a
trace message at the line PTRACE_BLOCK is called and then on exit from the
scope it is defined in.
*/
#define LOG_BLOCK_REF(message) \
	NSLog::CRefHolderBase refHolderBaseObjBlock; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjBlock << message; \
	} \
	NSLog::CBlockWithRef __trace_block_with_ref_instance(__FILE__, __LINE__, refHolderBaseObjBlock, refHolderBaseObjBlock);

/** Trace an execution block.(可以设置Block的BeginName和EndName)
(若message中为变量时，支持进出时都输出变量最新的值)
This macro creates a trace variable for tracking the entry and exit of program
blocks. It creates an instance of the PTraceBlock class that will output a
trace message at the line PTRACE_BLOCK is called and then on exit from the
scope it is defined in.
*/
#define LOG_BLOCK_REF_EXT(BeginName, EndName) \
	NSLog::CRefHolderBase refHolderBaseObjBlockRefExtBegin; \
	NSLog::CRefHolderBase refHolderBaseObjBlockRefExtEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjBlockRefExtBegin << BeginName; \
		refHolderBaseObjBlockRefExtEnd << EndName; \
	} \
	NSLog::CBlockWithRef __trace_block_with_ref_ext_instance(__FILE__, __LINE__, refHolderBaseObjBlockRefExtBegin, refHolderBaseObjBlockRefExtEnd);

/** @macro LOG_BLOCK_REF_B
@brief 记录函数进出的日志
@details 函数进入时为详细信息(带message)，函数退出时为简单信息(不带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_BLOCK_REF_BEGIN这里简写为LOG_BLOCK_REF_B
*/
#define LOG_BLOCK_REF_B(message) \
	NSLog::CRefHolderBase refHolderBaseObjBlockRefBeginBegin; \
	NSLog::CRefHolderBase refHolderBaseObjBlockRefBeginEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjBlockRefBeginBegin << message; \
		refHolderBaseObjBlockRefBeginEnd << "Block End"; \
	} \
	NSLog::CBlockWithRef __trace_block_with_ref_begin_instance(__FILE__, __LINE__, refHolderBaseObjBlockRefBeginBegin, refHolderBaseObjBlockRefBeginEnd);

/** @macro LOG_BLOCK_REF_E
@brief 记录函数进出的日志
@details 函数进入时为简单信息(不带message)，函数退出时为详细信息(带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_BLOCK_REF_END这里简写为LOG_BLOCK_REF_E
*/
#define LOG_BLOCK_REF_E(message) \
	NSLog::CRefHolderBase refHolderBaseObjBlockRefEndBegin; \
	NSLog::CRefHolderBase refHolderBaseObjBlockRefEndEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjBlockRefEndBegin << "Block Begin"; \
		refHolderBaseObjBlockRefEndEnd << message; \
	} \
	NSLog::CBlockWithRef __trace_block_with_ref_end_instance(__FILE__, __LINE__, refHolderBaseObjBlockRefEndBegin, refHolderBaseObjBlockRefEndEnd);

/** Trace an execution block.(若message中为变量时，支持进出时都输出变量最新的值)
LOG_LIGHTBLOCK_REF功能和LOG_BLOCK类似，只是不采用加锁为线程不安全的，但增加了效率
This macro creates a trace variable for tracking the entry and exit of program
blocks. It creates an instance of the PTraceBlock class that will output a
trace message at the line PTRACE_BLOCK is called and then on exit from the
scope it is defined in.
*/
#define LOG_LIGHTBLOCK_REF(name) \
	NSLog::CRefHolderBase refHolderBaseObjLightBlockRef; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjLightBlockRef << message; \
	} \
	NSLog::CLightBlockWithRef __trace_lightblock_with_ref_instance(__FILE__, __LINE__, refHolderBaseObjLightBlockRef, refHolderBaseObjLightBlockRef);

/** Trace an execution block.(可以设置Block的BeginName和EndName)
(若message中为变量时，支持进出时都输出变量最新的值)
LOG_LIGHTBLOCK_REF_EXT功能和LOG_BLOCK_REF_EXT类似，只是不采用加锁为线程不安全的，但增加了效率
This macro creates a trace variable for tracking the entry and exit of program
blocks. It creates an instance of the PTraceBlock class that will output a
trace message at the line PTRACE_BLOCK is called and then on exit from the
scope it is defined in.
*/
#define LOG_LIGHTBLOCK_REF_EXT(BeginName, EndName) \
	NSLog::CRefHolderBase refHolderBaseObjLightBlockRefExtBegin; \
	NSLog::CRefHolderBase refHolderBaseObjLightBlockRefExtEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjLightBlockRefExtBegin << BeginName; \
		refHolderBaseObjLightBlockRefExtEnd << EndName; \
	} \
	NSLog::CLightBlockWithRef __trace_lightblock_with_ref_ext_instance(__FILE__, __LINE__, refHolderBaseObjLightBlockRefExtBegin, refHolderBaseObjLightBlockRefExtEnd);


/**@macro LOG_FUN
@brief 记录函数进出的日志
@details 自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_FUN(message) \
	using namespace std; \
	std::ostrstream ostrsLogFun; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogFun << "FUN<" << __FUNCTION__ << ">:" << message << ends; \
	} \
	CLogWrapper::CBlock __trace_fun_block_instance(__FILE__, __LINE__, ostrsLogFun.str(), ostrsLogFun.str()); \
	ostrsLogFun.rdbuf()->freeze(0); 

/**@macro LOG_FUN_EXT
@brief 记录函数进出的日志(可以设置BeginName和EndName)
@details 自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_FUN_EXT(BeginName, EndName) \
	using namespace std; \
	std::ostrstream ostrsLogFunExtBegin; \
	std::ostrstream ostrsLogFunExtEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogFunExtBegin << "FUN<" << __FUNCTION__ << ">:" << BeginName << ends; \
		ostrsLogFunExtEnd << "FUN<" << __FUNCTION__ << ">:" << EndName << ends; \
	} \
	CLogWrapper::CBlock __trace_fun_ext_block_instance(__FILE__, __LINE__, ostrsLogFunExtBegin.str(), ostrsLogFunExtEnd.str()); \
	ostrsLogFunExtBegin.rdbuf()->freeze(0); \
	ostrsLogFunExtEnd.rdbuf()->freeze(0); 

/**@macro LOG_FUN_B
@brief 记录函数进出的日志
@details
函数进入时为详细信息(带message)，函数退出时为简单信息(不带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_FUN_BEGIN这里简写为LOG_FUN_B
*/
#define LOG_FUN_B(message) \
	using namespace std; \
	std::ostrstream ostrsLogFunBeginBegin; \
	std::ostrstream ostrsLogFunBeginEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogFunBeginBegin << "FUN<" << __FUNCTION__ << ">:" << message << ends; \
		ostrsLogFunBeginEnd << "FUN<" << __FUNCTION__ << ">:" << ends; \
	} \
	CLogWrapper::CBlock __trace_fun_block_begin_instance(__FILE__, __LINE__, ostrsLogFunBeginBegin.str(), ostrsLogFunBeginEnd.str()); \
	ostrsLogFunBeginBegin.rdbuf()->freeze(0); \
	ostrsLogFunBeginEnd.rdbuf()->freeze(0); 

/** 记录函数进出的日志
函数进入时为简单信息(不带message)，函数退出时为详细信息(带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_FUN_END这里简写为LOG_FUN_E
*/
#define LOG_FUN_E(message) \
	using namespace std; \
	std::ostrstream ostrsLogFunEndBegin; \
	std::ostrstream ostrsLogFunEndEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLogFunEndBegin << "FUN<" << __FUNCTION__ << ">:" << ends; \
		ostrsLogFunEndEnd << "FUN<" << __FUNCTION__ << ">:" << message << ends; \
	} \
	CLogWrapper::CBlock __trace_fun_block_end_instance(__FILE__, __LINE__, ostrsLogFunEndBegin.str(), ostrsLogFunEndEnd.str()); \
	ostrsLogFunEndBegin.rdbuf()->freeze(0); \
	ostrsLogFunEndEnd.rdbuf()->freeze(0); 

/**@macro LOG_FUN_WITH_LEVEL
@brief 记录函数进出的日志，这里可以指定日志输出的级别
@details 自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_FUN_WITH_LEVEL(level, message) \
	using namespace std; \
	std::ostrstream ostrsLogFunWithLevel; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		ostrsLogFunWithLevel << "FUN<" << __FUNCTION__ << ">:" << message << ends; \
	} \
	CLogWrapper::CBlock __trace_block_instance_with_level(__FILE__, __LINE__, ostrsLogFunWithLevel.str(), ostrsLogFunWithLevel.str(), level); \
	ostrsLogFunWithLevel.rdbuf()->freeze(0); 

/**@macro LOG_FUN_WITH_LEVEL_EXT
@brief 记录函数进出的日志，这里可以指定日志输出的级别(可以设置BeginName和EndName)
@details 自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_FUN_WITH_LEVEL_EXT(level, BeginName, EndName) \
	using namespace std; \
	std::ostrstream ostrsLogFunWithLevelExtBegin; \
	std::ostrstream ostrsLogFunWithLevelExtEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		ostrsLogFunWithLevelExtBegin << "FUN<" << __FUNCTION__ << ">:" << BeginName << ends; \
		ostrsLogFunWithLevelExtEnd << "FUN<" << __FUNCTION__ << ">:" << EndName << ends; \
	} \
	CLogWrapper::CBlock __trace_block_with_level_ext_instance(__FILE__, __LINE__, ostrsLogFunWithLevelExtBegin.str(), ostrsLogFunWithLevelExtEnd.str(), level); \
	ostrsLogFunWithLevelExtBegin.rdbuf()->freeze(0); \
	ostrsLogFunWithLevelExtEnd.rdbuf()->freeze(0); 

/** 记录函数进出的日志，这里可以指定日志输出的级别
函数进入时为详细信息(带message)，函数退出时为简单信息(只有函数名,不带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_FUN_BEGIN_WITH_LEVEL这里简写为LOG_FUN_WITH_LEVEL_B
*/
#define LOG_FUN_WITH_LEVEL_B(level, message) \
	using namespace std; \
	std::ostrstream ostrsLogFunBeginWithLevelBegin; \
	std::ostrstream ostrsLogFunBeginWithLevelEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		ostrsLogFunBeginWithLevelBegin << "FUN<" << __FUNCTION__ << ">:" << message << ends; \
		ostrsLogFunBeginWithLevelEnd << "FUN<" << __FUNCTION__ << ">:" << ends; \
	} \
	CLogWrapper::CBlock __trace_fun_block_end_with_level_instance(__FILE__, __LINE__, ostrsLogFunBeginWithLevelBegin.str(), ostrsLogFunBeginWithLevelEnd.str(), level); \
	ostrsLogFunBeginWithLevelBegin.rdbuf()->freeze(0); \
	ostrsLogFunBeginWithLevelEnd.rdbuf()->freeze(0); 

/** 记录函数进出的日志，这里可以指定日志输出的级别
函数进入时为简单信息(只有函数名,不带message)，函数退出时为详细信息(带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_FUN_END_WITH_LEVEL这里简写为LOG_FUN_WITH_LEVEL_E
*/
#define LOG_FUN_WITH_LEVEL_E(level, message) \
	using namespace std; \
	std::ostrstream ostrsLogFunEndWithLevelBegin; \
	std::ostrstream ostrsLogFunEndWithLevelEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		ostrsLogFunEndWithLevelBegin << "FUN<" << __FUNCTION__ << ">:" << ends; \
		ostrsLogFunEndWithLevelEnd << "FUN<" << __FUNCTION__ << ">:" << message << ends; \
	} \
	CLogWrapper::CBlock __trace_fun_block_end_with_level_instance(__FILE__, __LINE__, ostrsLogFunEndWithLevelBegin.str(), ostrsLogFunEndWithLevelEnd.str(), level); \
	ostrsLogFunEndWithLevelBegin.rdbuf()->freeze(0); \
	ostrsLogFunEndWithLevelEnd.rdbuf()->freeze(0);

/** 简炼日志记录宏定义
宏定义中直接对应了相应的日志级别
*/
#define LOG_FUN_FATAL(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_FATAL, message);
#define LOG_FUN_ALERT(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_ALERT, message);
#define LOG_FUN_CRIT(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_CRIT, message);
#define LOG_FUN_ERROR(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_ERROR, message);
#define LOG_FUN_WARN(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_WARN, message);
#define LOG_FUN_NOTICE(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_NOTICE, message);
#define LOG_FUN_INFO(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_INFO, message);
#define LOG_FUN_DEBUG(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_DEBUG, message);
#define LOG_FUN_TRACE(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_TRACE, message);
#define LOG_FUN_NOTSET(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_NOTSET, message);
#define LOG_FUN_UNKNOWN(message)	\
	LOG_FUN_WITH_LEVEL(LOG4C_PRIORITY_UNKNOWN, message);

#define LOG_FUN_FATAL_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_FATAL, BeginName, EndName);
#define LOG_FUN_ALERT_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_ALERT, BeginName, EndName);
#define LOG_FUN_CRIT_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_CRIT, BeginName, EndName);
#define LOG_FUN_ERROR_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_ERROR, BeginName, EndName);
#define LOG_FUN_WARN_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_WARN, BeginName, EndName);
#define LOG_FUN_NOTICE_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_NOTICE, BeginName, EndName);
#define LOG_FUN_INFO_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_INFO, BeginName, EndName);
#define LOG_FUN_DEBUG_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_DEBUG, BeginName, EndName);
#define LOG_FUN_TRACE_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_TRACE, BeginName, EndName);
#define LOG_FUN_NOTSET_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_NOTSET, BeginName, EndName);
#define LOG_FUN_UNKNOWN_EXT(BeginName, EndName)	\
	LOG_FUN_WITH_LEVEL_EXT(LOG4C_PRIORITY_UNKNOWN, BeginName, EndName);

#define LOG_FUN_FATAL_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_FATAL, message);
#define LOG_FUN_ALERT_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_ALERT, message);
#define LOG_FUN_CRIT_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_CRIT, message);
#define LOG_FUN_ERROR_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_ERROR, message);
#define LOG_FUN_WARN_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_WARN, message);
#define LOG_FUN_NOTICE_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_NOTICE, message);
#define LOG_FUN_INFO_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_INFO, message);
#define LOG_FUN_DEBUG_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_DEBUG, message);
#define LOG_FUN_TRACE_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_TRACE, message);
#define LOG_FUN_NOTSET_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_NOTSET, message);
#define LOG_FUN_UNKNOWN_B(message)	\
	LOG_FUN_WITH_LEVEL_B(LOG4C_PRIORITY_UNKNOWN, message);

#define LOG_FUN_FATAL_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_FATAL, message);
#define LOG_FUN_ALERT_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_ALERT, message);
#define LOG_FUN_CRIT_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_CRIT, message);
#define LOG_FUN_ERROR_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_ERROR, message);
#define LOG_FUN_WARN_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_WARN, message);
#define LOG_FUN_NOTICE_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_NOTICE, message);
#define LOG_FUN_INFO_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_INFO, message);
#define LOG_FUN_DEBUG_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_DEBUG, message);
#define LOG_FUN_TRACE_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_TRACE, message);
#define LOG_FUN_NOTSET_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_NOTSET, message);
#define LOG_FUN_UNKNOWN_E(message)	\
	LOG_FUN_WITH_LEVEL_E(LOG4C_PRIORITY_UNKNOWN, message);


/** 记录函数进出的日志
LOG_LIGHTFUN功能和LOG_FUN类似，只是不采用加锁为线程不安全的，但增加了效率
自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_LIGHTFUN(message) \
	using namespace std; \
	std::ostrstream ostrsLightFun; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		ostrsLightFun << "FUN<" << __FUNCTION__ << ">:" << message << ends; \
	} \
	CLogWrapper::CLightBlock __trace_fun_lightblock_instance(__FILE__, __LINE__, ostrsLightFun.str(), ostrsLightFun.str()); \
	ostrsLightFun.rdbuf()->freeze(0);

/** 记录函数进出的日志，这里可以指定日志输出的级别
自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_LIGHTFUN_WITH_LEVEL(level, message) \
	using namespace std; \
	std::ostrstream ostrsLogLightFunWithLevel; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		ostrsLogLightFunWithLevel << "FUN<" << __FUNCTION__ << ">:" << message << ends; \
	} \
	CLogWrapper::CLightBlock __trace_block_instance_with_level(__FILE__, __LINE__, ostrsLogLightFunWithLevel.str(), ostrsLogLightFunWithLevel.str(), level); \
	ostrsLogLightFunWithLevel.rdbuf()->freeze(0); 

/** 简炼日志记录宏定义
宏定义中直接对应了相应的日志级别
*/
#define LOG_LIGHTFUN_FATAL(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_FATAL, message);
#define LOG_LIGHTFUN_ALERT(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_ALERT, message);
#define LOG_LIGHTFUN_CRIT(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_CRIT, message);
#define LOG_LIGHTFUN_ERROR(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_ERROR, message);
#define LOG_LIGHTFUN_WARN(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_WARN, message);
#define LOG_LIGHTFUN_NOTICE(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_NOTICE, message);
#define LOG_LIGHTFUN_INFO(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_INFO, message);
#define LOG_LIGHTFUN_DEBUG(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_DEBUG, message);
#define LOG_LIGHTFUN_TRACE(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_TRACE, message);
#define LOG_LIGHTFUN_NOTSET(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_NOTSET, message);
#define LOG_LIGHTFUN_UNKNOWN(message)	\
	LOG_LIGHTFUN_WITH_LEVEL(LOG4C_PRIORITY_UNKNOWN, message);


/**@macro LOG_FUN_REF
@brief 记录函数进出的日志(若message中为变量时，支持进出时都输出变量最新的值)
@details 自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_FUN_REF(message) \
	NSLog::CRefHolderBase refHolderBaseObjFun; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjFun << "FUN<" << __FUNCTION__ << ">:" << message /*<< std::ends*/; \
	} \
	NSLog::CBlockWithRef __trace_fun_block_with_ref_instance(__FILE__, __LINE__, refHolderBaseObjFun, refHolderBaseObjFun);

/**@macro LOG_FUN_REF_EXT
@brief 记录函数进出的日志(可以设置BeginName和EndName),若message中为变量时，支持进出时都输出变量最新的值
@details 自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_FUN_REF_EXT(BeginName, EndName) \
	using namespace std; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefExtBegin; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefExtEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjFunRefExtBegin << "FUN<" << __FUNCTION__ << ">:" << BeginName /*<< ends*/; \
		refHolderBaseObjFunRefExtEnd << "FUN<" << __FUNCTION__ << ">:" << EndName /*<< ends*/; \
	} \
	NSLog::CBlockWithRef __trace_fun_ref_ext_block_instance(__FILE__, __LINE__, refHolderBaseObjFunRefExtBegin, refHolderBaseObjFunRefExtEnd);

/**@macro LOG_FUN_REF_B
@brief 记录函数进出的日志(若message中为变量时，支持进出时都输出变量最新的值)
@details
函数进入时为详细信息(带message)，函数退出时为简单信息(不带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_FUN_REF_BEGIN这里简写为LOG_FUN_REF_B
*/
#define LOG_FUN_REF_B(message) \
	NSLog::CRefHolderBase refHolderBaseObjFunRefBeginBegin; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefBeginEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjFunRefBeginBegin << "FUN<" << __FUNCTION__ << ">:" << message /*<< std::ends*/; \
		refHolderBaseObjFunRefBeginEnd << "FUN<" << __FUNCTION__ << ">:" /*<< std::ends*/; \
	} \
	NSLog::CBlockWithRef __trace_fun_block_with_ref_begin_instance(__FILE__, __LINE__, refHolderBaseObjFunRefBeginBegin, refHolderBaseObjFunRefBeginEnd);

/**@macro LOG_FUN_REF_E
@brief 记录函数进出的日志(若message中为变量时，支持进出时都输出变量最新的值)
@details
函数进入时为为简单信息(不带message)，函数退出时详细信息(带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_FUN_REF_END这里简写为LOG_FUN_REF_E
*/
#define LOG_FUN_REF_E(message) \
	NSLog::CRefHolderBase refHolderBaseObjFunRefEndBegin; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefEndEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjFunRefEndBegin << "FUN<" << __FUNCTION__ << ">:"  /*<< std::ends*/; \
		refHolderBaseObjFunRefEndEnd << "FUN<" << __FUNCTION__ << ">:" << message /*<< std::ends*/; \
	} \
	NSLog::CBlockWithRef __trace_fun_block_with_ref_end_instance(__FILE__, __LINE__, refHolderBaseObjFunRefEndBegin, refHolderBaseObjFunRefEndEnd);

/**@macro LOG_FUN_REF_WITH_LEVEL
@brief 记录函数进出的日志(若message中为变量时，支持进出时都输出变量最新的值)，这里可以指定日志输出的级别
@details 自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_FUN_REF_WITH_LEVEL(level, message) \
	NSLog::CRefHolderBase refHolderBaseObjFunWithLevel; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		refHolderBaseObjFunWithLevel << "FUN<" << __FUNCTION__ << ">:" << message /*<< std::ends*/; \
	} \
	NSLog::CBlockWithRef __trace_fun_block_with_ref_with_level_instance(__FILE__, __LINE__, refHolderBaseObjFunWithLevel, refHolderBaseObjFunWithLevel, level);

/**@macro LOG_FUN_REF_WITH_LEVEL_EXT
@brief 记录函数进出的日志(可以设置BeginName和EndName),若message中为变量时，支持进出时都输出变量最新的值
@details 自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_FUN_REF_WITH_LEVEL_EXT(level, BeginName, EndName) \
	using namespace std; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefWithLevelExtBegin; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefWithLevelExtEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		refHolderBaseObjFunRefWithLevelExtBegin << "FUN<" << __FUNCTION__ << ">:" << BeginName /*<< ends*/; \
		refHolderBaseObjFunRefWithLevelExtEnd << "FUN<" << __FUNCTION__ << ">:" << EndName /*<< ends*/; \
	} \
	NSLog::CBlockWithRef __trace_fun_ref_ext_with_level_block_instance(__FILE__, __LINE__, refHolderBaseObjFunRefWithLevelExtBegin, refHolderBaseObjFunRefWithLevelExtEnd, level);

/**@macro LOG_FUN_REF_WITH_LEVEL_B
@brief 记录函数进出的日志(若message中为变量时，支持进出时都输出变量最新的值)
@details
支持设置日志级别
函数进入时为详细信息(带message)，函数退出时为简单信息(不带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_FUN_REF_WITH_LEVEL_BEGIN这里简写为LOG_FUN_REF_WITH_LEVEL_B
*/
#define LOG_FUN_REF_WITH_LEVEL_B(level, message) \
	using namespace std; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefExtWithLevelBeginBegin; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefExtWithLevelBeginEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		refHolderBaseObjFunRefExtWithLevelBeginBegin << "FUN<" << __FUNCTION__ << ">:" << message /*<< ends*/; \
		refHolderBaseObjFunRefExtWithLevelBeginEnd << "FUN<" << __FUNCTION__ << ">:"  /*<< ends*/; \
	} \
	NSLog::CBlockWithRef __trace_fun_ref_with_level_begin_block_instance(__FILE__, __LINE__, refHolderBaseObjFunRefExtWithLevelBeginBegin, refHolderBaseObjFunRefExtWithLevelBeginEnd, level);

/**@macro LOG_FUN_REF_WITH_LEVEL_E
@brief 记录函数进出的日志(若message中为变量时，支持进出时都输出变量最新的值)
@details
支持设置日志级别
函数进入时为简单信息(不带message)，函数退出时为详细信息(带message)
自动输出当前所在的函数进出和对应的消息到日志文件中去
LOG_FUN_REF_WITH_LEVEL_END这里简写为LOG_FUN_REF_WITH_LEVEL_E
*/
#define LOG_FUN_REF_WITH_LEVEL_E(level, message) \
	using namespace std; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefExtWithLevelEndBegin; \
	NSLog::CRefHolderBase refHolderBaseObjFunRefExtWithLevelEndEnd; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		refHolderBaseObjFunRefExtWithLevelEndBegin << "FUN<" << __FUNCTION__ << ">:" /*<< ends*/; \
		refHolderBaseObjFunRefExtWithLevelEndEnd << "FUN<" << __FUNCTION__ << ">:" << message /*<< ends*/; \
	} \
	NSLog::CBlockWithRef __trace_fun_ref_with_level_end_block_instance(__FILE__, __LINE__, refHolderBaseObjFunRefExtWithLevelEndBegin, refHolderBaseObjFunRefExtWithLevelEndEnd, level);

/** 简炼日志记录宏定义
宏定义中直接对应了相应的日志级别
*/
#define LOG_FUN_REF_FATAL(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_FATAL, message);
#define LOG_FUN_REF_ALERT(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_ALERT, message);
#define LOG_FUN_REF_CRIT(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_CRIT, message);
#define LOG_FUN_REF_ERROR(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_ERROR, message);
#define LOG_FUN_REF_WARN(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_WARN, message);
#define LOG_FUN_REF_NOTICE(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_NOTICE, message);
#define LOG_FUN_REF_INFO(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_INFO, message);
#define LOG_FUN_REF_DEBUG(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_DEBUG, message);
#define LOG_FUN_REF_TRACE(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_TRACE, message);
#define LOG_FUN_REF_NOTSET(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_NOTSET, message);
#define LOG_FUN_REF_UNKNOWN(message)	\
	LOG_FUN_REF_WITH_LEVEL(LOG4C_PRIORITY_UNKNOWN, message);

#define LOG_FUN_REF_FATAL_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_FATAL, BeginName, EndName);
#define LOG_FUN_REF_ALERT_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_ALERT, BeginName, EndName);
#define LOG_FUN_REF_CRIT_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_CRIT, BeginName, EndName);
#define LOG_FUN_REF_ERROR_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_ERROR, BeginName, EndName);
#define LOG_FUN_REF_WARN_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_WARN, BeginName, EndName);
#define LOG_FUN_REF_NOTICE_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_NOTICE, BeginName, EndName);
#define LOG_FUN_REF_INFO_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_INFO, BeginName, EndName);
#define LOG_FUN_REF_DEBUG_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_DEBUG, BeginName, EndName);
#define LOG_FUN_REF_TRACE_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_TRACE, BeginName, EndName);
#define LOG_FUN_REF_NOTSET_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_NOTSET, BeginName, EndName);
#define LOG_FUN_REF_UNKNOWN_EXT(BeginName, EndName)	\
	LOG_FUN_REF_WITH_LEVEL_EXT(LOG4C_PRIORITY_UNKNOWN, BeginName, EndName);

#define LOG_FUN_REF_FATAL_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_FATAL, message);
#define LOG_FUN_REF_ALERT_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_ALERT, message);
#define LOG_FUN_REF_CRIT_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_CRIT, message);
#define LOG_FUN_REF_ERROR_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_ERROR, message);
#define LOG_FUN_REF_WARN_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_WARN, message);
#define LOG_FUN_REF_NOTICE_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_NOTICE, message);
#define LOG_FUN_REF_INFO_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_INFO, message);
#define LOG_FUN_REF_DEBUG_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_DEBUG, message);
#define LOG_FUN_REF_TRACE_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_TRACE, message);
#define LOG_FUN_REF_NOTSET_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_NOTSET, message);
#define LOG_FUN_REF_UNKNOWN_B(message)	\
	LOG_FUN_REF_WITH_LEVEL_B(LOG4C_PRIORITY_UNKNOWN, message);

#define LOG_FUN_REF_FATAL_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_FATAL, message);
#define LOG_FUN_REF_ALERT_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_ALERT, message);
#define LOG_FUN_REF_CRIT_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_CRIT, message);
#define LOG_FUN_REF_ERROR_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_ERROR, message);
#define LOG_FUN_REF_WARN_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_WARN, message);
#define LOG_FUN_REF_NOTICE_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_NOTICE, message);
#define LOG_FUN_REF_INFO_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_INFO, message);
#define LOG_FUN_REF_DEBUG_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_DEBUG, message);
#define LOG_FUN_REF_TRACE_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_TRACE, message);
#define LOG_FUN_REF_NOTSET_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_NOTSET, message);
#define LOG_FUN_REF_UNKNOWN_E(message)	\
	LOG_FUN_REF_WITH_LEVEL_E(LOG4C_PRIORITY_UNKNOWN, message);


/** 记录函数进出的日志(若message中为变量时，支持进出时都输出变量最新的值)
LOG_LIGHTFUN_REF功能和LOG_FUN_REF类似，只是不采用加锁为线程不安全的，但增加了效率
自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_LIGHTFUN_REF(message) \
	NSLog::CRefHolderBase refHolderBaseObjFun; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(LOG_LEVEL_TRACE) ) \
	{ \
		refHolderBaseObjFun << "FUN<" << __FUNCTION__ << ">:" << message /*<< std::ends*/; \
	} \
	NSLog::CLightBlockWithRef __trace_fun_lightblock_with_ref_instance(__FILE__, __LINE__, refHolderBaseObjFun);

/** 记录函数进出的日志(若message中为变量时，支持进出时都输出变量最新的值)，这里可以指定日志输出的级别
自动输出当前所在的函数进出和对应的消息到日志文件中去
*/
#define LOG_LIGHTFUN_REF_WITH_LEVEL(level, message) \
	NSLog::CRefHolderBase refHolderBaseObjFunWithLevel; \
	if ( (CLogWrapper::GetDefaultLogger()).isEnabledFor(level) ) \
	{ \
		refHolderBaseObjFunWithLevel << "FUN<" << __FUNCTION__ << ">:" << message /*<< std::ends*/; \
	} \
	NSLog::CLightBlockWithRef __trace_fun_lightblock_with_ref_with_level_instance(__FILE__, __LINE__, refHolderBaseObjFunWithLevel, level);

/** 简炼日志记录宏定义
宏定义中直接对应了相应的日志级别
*/
#define LOG_LIGHTFUN_REF_FATAL(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_FATAL, message);
#define LOG_LIGHTFUN_REF_ALERT(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_ALERT, message);
#define LOG_LIGHTFUN_REF_CRIT(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_CRIT, message);
#define LOG_LIGHTFUN_REF_ERROR(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_ERROR, message);
#define LOG_LIGHTFUN_REF_WARN(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_WARN, message);
#define LOG_LIGHTFUN_REF_NOTICE(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_NOTICE, message);
#define LOG_LIGHTFUN_REF_INFO(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_INFO, message);
#define LOG_LIGHTFUN_REF_DEBUG(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_DEBUG, message);
#define LOG_LIGHTFUN_REF_TRACE(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_TRACE, message);
#define LOG_LIGHTFUN_REF_NOTSET(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_NOTSET, message);
#define LOG_LIGHTFUN_REF_UNKNOWN(message)	\
	LOG_LIGHTFUN_REF_WITH_LEVEL(LOG4C_PRIORITY_UNKNOWN, message);


/** 输出文件名和行号的日志输出宏,new 操作失败则记录完日志直接return returnValue;
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG宏定义中
*/
#define LOG_NEW_RETURN_WITH_VALUE(POINTER, CONSTRUCTOR, RET_VAL, level, message) \
do \
{ \
	POINTER = new CONSTRUCTOR; \
	if (POINTER == 0) \
	{ \
		LOG(level, message); \
		return RET_VAL; \
	} \
} while (0)

/** 输出文件名和行号的日志输出宏,new 操作失败则记录完日志直接return ;
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG宏定义中
*/
#define LOG_NEW_RETURN(POINTER, CONSTRUCTOR, level, message) \
do \
{ \
	POINTER = new CONSTRUCTOR; \
	if (POINTER == 0) \
	{ \
		LOG(level, message); \
		return ; \
	} \
} while (0);

/** 输出文件名和行号的日志输出宏,new 操作失败则记录完日志直接break ;
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG宏定义中
*/
#define LOG_NEW_BREAK(POINTER, CONSTRUCTOR, level, message) \
{ \
	POINTER = new CONSTRUCTOR; \
	if (POINTER == 0) \
	{ \
		LOG(level, message); \
		break ; \
	} \
} 

/** 输出文件名和行号的日志输出宏,new 操作失败则记录完日志直接continue ;
下面的宏定义采用了ostrstream类型的对象进行中转,可以输出对象如PTIME和CORBA::Exception& e等到输出流中
然后再输入到LOG宏定义中
*/
#define LOG_NEW_CONTINUE(POINTER, CONSTRUCTOR, level, message) \
{ \
	POINTER = new CONSTRUCTOR; \
	if (POINTER == 0) \
	{ \
		LOG(level, message); \
		continue ; \
	} \
} 

/**
LOG初始化函数,传入配置文件名
*/
#define LOG_INIT(strConfigFile) \
	CLogWrapper::Instance()->Init(strConfigFile);

/**
LOG初始化函数,传入配置文件的字符串内容
*/
#define LOG_INIT_WITH_STRING(X) \
	CLogWrapper::Instance()->InitWithString X;

/**日志记录模块初始化宏定义
LOG缺省初始化函数
注意事项: 必须初始化日志模块后才能正确记录日志
本初始化不要求应用程序所在目录有日志配置文件log4crc，会自动生成一个缺省的log4crc文件
使用例子: LOG_INIT_DEFAULT();
*/
#define LOG_INIT_DEFAULT() \
	CLogWrapper::Instance()->InitWithString("");

/************************************************************************
下面的为通过设置参数设置日志模块参数的宏定义(功能和LOG_INIT和LOG_INIT_DEFAULT类似):
1.必须先调用LOG_PARAM_XXXX系列宏定义设置参数,
1.1.如设置日志记录级别、生成日志文件名等。
1.2.若不LOG_PARAM_XXXX系列宏定义则将使用默认设置参数。
2.然后调用LOG_INIT_WITH_PARAM宏定义来初始化日志模块。
3.此时就可以记录日志了。
*/
#define LOG_PARAM_LOG_CFG_FILE_NAME(strCfgFileName) \
	LOG4C_PARAM_CFG_FILE_NAME(strCfgFileName) 
#define LOG_PARAM_LOG_LEVEL(strLogLevel) \
	LOG4C_PARAM_LOG_LEVEL(strLogLevel) 
#define LOG_PARAM_LOG_PATH(strLogPath) \
	LOG4C_PARAM_LOG_PATH(strLogPath) 
#define LOG_PARAM_LOG_FILE_NAME(strLogFileName) \
	LOG4C_PARAM_LOG_FILE_NAME(strLogFileName) 
#define LOG_PARAM_LOG_FILE_SIZE(iFileSize) \
	LOG4C_PARAM_LOG_FILE_SIZE(iFileSize) 
#define LOG_PARAM_LOG_FILE_NUM(iFileNum) \
	LOG4C_PARAM_LOG_FILE_NUM(iFileNum)
#define LOG_PARAM_LOG_REREAD_CFG_FILE(bReReadLogCfgFile) \
	LOG4C_PARAM_REREAD_LOG_CFG_FILE(bReReadLogCfgFile)
#define LOG_PARAM_LOG_REREAD_CFG_FILE_ELAPSE(dwReReadMilliseconds) \
	LOG4C_PARAM_REREAD_LOG_CFG_FILE_ELAPSE(dwReReadMilliseconds)
#define LOG_PARAM_LOG_LOGGER_NAME(strLoggerName) \
	LOG4C_PARAM_LOG_LOGGER_NAME(strLoggerName)
#define LOG_PARAM_LOG_APP_NAME(strAppName) \
	LOG4C_PARAM_LOG_APP_NAME(strAppName)
#define LOG_PARAM_LOG_IMMEDIATE_FLUSH(bImmediateFlush) \
	LOG4C_PARAM_LOG_IMMEDIATE_FLUSH(bImmediateFlush)
#define LOG_PARAM_LOG_BUFFER_SIZE(iBufferSize) \
	LOG4C_PARAM_LOG_BUFFER_SIZE(iBufferSize)
#define LOG_INIT_WITH_PARAM() \
	LOG4C_INIT_WITH_PARAM()

#define LOG_INIT_WITH_PARAM_MULTI_PROCESS() \
	LOG4C_INIT_WITH_PARAM_MULTI_PROCESS()

/**
LOG结束清理函数
*/
#define LOG_FINI() \
	CLogWrapper::DestroyInstance()

#else

#define LOG4CPLUS(level, message)
#define LOG LOG4CPLUS
#define LOG4CPLUS_NO_FILENUM(level, message)
#define LOG_NO_FILENUM LOG4CPLUS_NO_FILENUM	
#define LOG_NO_FILENUM_NO_LAYOUT(level, message)
#define LOG_ORIGIN LOG_NO_FILENUM_NO_LAYOUT
#define LOG_IF(level, cond, message)
#define LOG_LINE()
#define LOG_HEX_DUMP(level, ptrBuffer, lBufferLen)
#define LOG_PRINTF(X)
#define LOG_RETURN(level, message)  do { return ; } while (0)
#define LOG_RETURN_WITH_VALUE(level, message, returnValue) do { return (returnValue); } while (0)
#define LOG_BREAK(level, message)  { break; }
#define LOG_CONTINUE(level, message)  { continue; }
#define LOG_BLOCK(name)
#define LOG_BLOCK_EXT(BeginName, EndName)
#define LOG_FUN(message)
#define LOG_FUN_B(message)
#define LOG_FUN_E(message)
#define LOG_FUN_WITH_LEVEL(level, message) 
#define LOG_FUN_WITH_LEVEL_B(level, message) 
#define LOG_FUN_WITH_LEVEL_E(level, message) 
#define LOG_FUN_FATAL(message)	
#define LOG_FUN_ALERT(message)
#define LOG_FUN_CRIT(message)
#define LOG_FUN_ERROR(message)
#define LOG_FUN_WARN(message)
#define LOG_FUN_NOTICE(message)
#define LOG_FUN_INFO(message)
#define LOG_FUN_DEBUG(message)
#define LOG_FUN_TRACE(message)
#define LOG_FUN_NOTSET(message)	
#define LOG_FUN_UNKNOWN(message)

#define LOG_FUN_FATAL_B(message)	
#define LOG_FUN_ALERT_B(message)
#define LOG_FUN_CRIT_B(message)
#define LOG_FUN_ERROR_B(message)
#define LOG_FUN_WARN_B(message)
#define LOG_FUN_NOTICE_B(message)
#define LOG_FUN_INFO_B(message)
#define LOG_FUN_DEBUG_B(message)
#define LOG_FUN_TRACE_B(message)
#define LOG_FUN_NOTSET_B(message)	
#define LOG_FUN_UNKNOWN_B(message)

#define LOG_FUN_FATAL_E(message)	
#define LOG_FUN_ALERT_E(message)
#define LOG_FUN_CRIT_E(message)
#define LOG_FUN_ERROR_E(message)
#define LOG_FUN_WARN_E(message)
#define LOG_FUN_NOTICE_E(message)
#define LOG_FUN_INFO_E(message)
#define LOG_FUN_DEBUG_E(message)
#define LOG_FUN_TRACE_E(message)
#define LOG_FUN_NOTSET_E(message)	
#define LOG_FUN_UNKNOWN_E(message)

#define LOG_LIGHTBLOCK(name)
#define LOG_LIGHTFUN(message)
#define LOG_LIGHTFUN_WITH_LEVEL(level, message) 
#define LOG_LIGHTFUN_FATAL(message)	
#define LOG_LIGHTFUN_ALERT(message)	
#define LOG_LIGHTFUN_CRIT(message)	
#define LOG_LIGHTFUN_ERROR(message)	
#define LOG_LIGHTFUN_WARN(message)	
#define LOG_LIGHTFUN_NOTICE(message)	
#define LOG_LIGHTFUN_INFO(message)	
#define LOG_LIGHTFUN_DEBUG(message)	
#define LOG_LIGHTFUN_TRACE(message)	
#define LOG_LIGHTFUN_NOTSET(message)
#define LOG_LIGHTFUN_UNKNOWN(message)

#define LOG_BLOCK_REF(message)
#define LOG_FUN_REF(message)
#define LOG_FUN_REF_WITH_LEVEL(level, message) 
#define LOG_FUN_REF_FATAL(message)	
#define LOG_FUN_REF_ALERT(message)
#define LOG_FUN_REF_CRIT(message)
#define LOG_FUN_REF_ERROR(message)
#define LOG_FUN_REF_WARN(message)
#define LOG_FUN_REF_NOTICE(message)
#define LOG_FUN_REF_INFO(message)
#define LOG_FUN_REF_DEBUG(message)
#define LOG_FUN_REF_TRACE(message)
#define LOG_FUN_REF_NOTSET(message)	
#define LOG_FUN_REF_UNKNOWN(message)

#define LOG_LIGHTBLOCK_REF(name)
#define LOG_LIGHTFUN_REF(message)
#define LOG_LIGHTFUN_REF_WITH_LEVEL(level, message) 
#define LOG_LIGHTFUN_REF_FATAL(message)	
#define LOG_LIGHTFUN_REF_ALERT(message)	
#define LOG_LIGHTFUN_REF_CRIT(message)	
#define LOG_LIGHTFUN_REF_ERROR(message)	
#define LOG_LIGHTFUN_REF_WARN(message)	
#define LOG_LIGHTFUN_REF_NOTICE(message)	
#define LOG_LIGHTFUN_REF_INFO(message)	
#define LOG_LIGHTFUN_REF_DEBUG(message)	
#define LOG_LIGHTFUN_REF_TRACE(message)	
#define LOG_LIGHTFUN_REF_NOTSET(message)
#define LOG_LIGHTFUN_REF_UNKNOWN(message)

#define LOG_NEW_RETURN_WITH_VALUE(POINTER, CONSTRUCTOR, RET_VAL, level, message) \
	do \
	{ \
		POINTER = new CONSTRUCTOR; \
		if (POINTER == 0) \
		{ \
			return RET_VAL; \
		} \
	} while (0)

#define LOG_NEW_RETURN(POINTER, CONSTRUCTOR, level, message) \
	do \
	{ \
		POINTER = new CONSTRUCTOR; \
		if (POINTER == 0) \
		{ \
			return ; \
		} \
	} while (0)

#define LOG_NEW_BREAK(POINTER, CONSTRUCTOR, level, message) \
{ \
	POINTER = new CONSTRUCTOR; \
	if (POINTER == 0) \
	{ \
		break ; \
	} \
} 

#define LOG_NEW_CONTINUE(POINTER, CONSTRUCTOR, level, message) \
{ \
	POINTER = new CONSTRUCTOR; \
	if (POINTER == 0) \
	{ \
		continue ; \
	} \
} 

#define LOG_INIT(strConfigFile)
#define LOG_INIT_WITH_STRING(X)
#define LOG_INIT_DEFAULT()
#define LOG_FINI() 

#define LOG_PARAM_LOG_CFG_FILE_NAME(strCfgFileName) 
#define LOG_PARAM_LOG_LEVEL(iLogLevel) 
#define LOG_PARAM_LOG_FILE_NAME(strLogFileName) 
#define LOG_PARAM_LOG_PATH(strLogPath) 
#define LOG_PARAM_LOG_FILE_SIZE(iFileSize) 
#define LOG_PARAM_LOG_FILE_NUM(iFileNum)
#define LOG_PARAM_LOG_REREAD_CFG_FILE(bReReadLogCfgFile)
#define LOG_PARAM_LOG_LOGGER_NAME(strLoggerName)
#define LOG_PARAM_LOG_IMMEDIATE_FLUSH(bImmediateFlush)
#define LOG_PARAM_LOG_BUFFER_SIZE(iBufferSize)
#define LOG_PARAM_LOG_APP_NAME(strAppName)
#define LOG_INIT_WITH_PARAM()
#define LOG_INIT_WITH_PARAM_MULTI_PROCESS()

#endif ///defined(LOG4CPLUS_ENABLE)  && defined(__cplusplus)

#endif // !defined(AFX_CONFIG_LOG_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_)
/************** End of config_log.h *******************************************/
#endif /// defined(__cplusplus)



//////////////////////下面为C日志记录头文件包含////////////////////////////

/************** Begin of Log.h *******************************************/
/************************************************************************
 * Log.h
 * 日志相关代码的头文件
 ************************************************************************/
#if !defined(AFX_LOG_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_)
#define AFX_LOG_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>

 ///用户不需要使用日志,不要定义LOG4CPLUS_ENABLE宏命令就可以了
#if !defined(LOG4CPLUS_ENABLE)

#define LOG4C(X) 
#define LOG4C_INIT()

#define LOG4C_PARAM_CFG_FILE_NAME(strCfgFileName) 
#define LOG4C_PARAM_LOG_LEVEL(strLogLevel) 
#define LOG4C_PARAM_LOG_FILE_NAME(strLogFileName) 
#define LOG4C_PARAM_LOG_FILE_SIZE(iFileSize) 
#define LOG4C_PARAM_LOG_FILE_NUM(iFileNum)
#define LOG4C_PARAM_LOG_PATH(strLogPath) 
#define LOG4C_PARAM_REREAD_LOG_CFG_FILE(bReReadLogCfgFile)
#define LOG4C_PARAM_REREAD_LOG_CFG_FILE_ELAPSE(dwReReadMilliseconds)
#define LOG4C_PARAM_LOG_LOGGER_NAME(strLoggerName)
#define LOG4C_PARAM_LOG_APP_NAME(strAppName)
#define LOG4C_PARAM_LOG_IMMEDIATE_FLUSH(bImmediateFlush)
#define LOG4C_PARAM_LOG_BUFFER_SIZE(iBufferSize)
#define LOG4C_INIT_WITH_PARAM()
#define LOG4C_INIT_WITH_PARAM_MULTI_PROCESS()

#define LOG4C_FINI()
#define LOG4C_INIT_DEFAULT()

#define LOG4C_NO_FILENUM(X) 
#define LOG4C_NO_FILENUM_NO_LAYOUT(X)
#define LOG4C_ORIGIN LOG4C_NO_FILENUM_NO_LAYOUT
#define LOG4C_BLOCK_BEGIN(X)
#define LOG4C_BLOCK_END(X) 
#define LOG4C_FUN(X) 
#define LOG4C_IF(X)
#define LOG4C_LINE()
#define LOG4C_HEX_DUMP(X)
#define LOG4C_RETURN(X)
#define LOG4C_RETURN_WITH_VALUE(X, reurnValue)

#define LOG_FATAL(message)
#define LOG_ALERT(message)
#define LOG_CRIT(message)
#define LOG_ERROR(message)
#define LOG_WARN(message)
#define LOG_NOTICE(message)
#define LOG_INFO(message)
#define LOG_DEBUG(message)
#define LOG_TRACE(message)
#define LOG_NOTSET(message)
#define LOG_UNKNOWN(message)	

#else

/**日志记录模块初始化宏定义
注意事项: 必须初始化日志模块后才能正确记录日志
本初始化不要求应用程序所在目录有日志配置文件log4crc，会自动生成一个缺省的log4crc文件
使用例子: LOG4C_INIT_DEFAULT();
*/
#define LOG4C_INIT_DEFAULT() \
	log_init_with_string("", "")

/**日志记录模块初始化宏定义
注意事项: 必须初始化日志模块后才能正确记录日志
本初始化要求应用程序所在目录有日志配置文件log4crc才行
使用例子: LOG4C_INIT();
*/
#define LOG4C_INIT() \
	log_init()

/************************************************************************
下面的为通过设置参数设置日志模块参数的宏定义(功能和LOG4C_INIT和LOG4C_INIT_DEFAULT类似):
1.必须先调用LOG_PARAM_XXXX系列宏定义设置参数,
1.1.如设置日志记录级别、生成日志文件名等。
1.2.若不LOG_PARAM_XXXX系列宏定义则将使用默认设置参数。
2.然后调用LOG_INIT_WITH_PARAM宏定义来初始化日志模块。
3.此时就可以记录日志了。
*/
#define LOG4C_PARAM_CFG_FILE_NAME(strCfgFileName) \
	log_set_log_cfg_file_name(strCfgFileName)

#define LOG4C_PARAM_LOG_LEVEL(strLogLevel) \
	log_set_log_level(strLogLevel)

#define LOG4C_PARAM_LOG_PATH(strLogPath) \
	log_set_log_path(strLogPath)

#define LOG4C_PARAM_LOG_FILE_NAME(strLogFileName) \
	log_set_log_file_name(strLogFileName)

#define LOG4C_PARAM_LOG_FILE_SIZE(iFileSize) \
	log_set_log_file_size(iFileSize)

#define LOG4C_PARAM_LOG_FILE_NUM(iFileNum) \
	log_set_log_file_num(iFileNum)

#define LOG4C_PARAM_REREAD_LOG_CFG_FILE(bReReadLogCfgFile) \
	log_set_reread_log_cfg_file(bReReadLogCfgFile)

#define LOG4C_PARAM_REREAD_LOG_CFG_FILE_ELAPSE(dwReReadMilliseconds) \
	log_set_reread_log_cfg_file_elapse(dwReReadMilliseconds)

#define LOG4C_PARAM_LOG_LOGGER_NAME(strLoggerName) \
	log_set_log_logger_name(strLoggerName)

#define LOG4C_PARAM_LOG_APP_NAME(strAppName) \
	log_set_log_app_name(strAppName)

#define LOG4C_PARAM_LOG_IMMEDIATE_FLUSH(bImmediateFlush) \
	log_set_log_immediate_flush(bImmediateFlush)

#define LOG4C_PARAM_LOG_BUFFER_SIZE(iBufferSize) \
	log_set_log_buffer_size(iBufferSize)

#define LOG4C_INIT_WITH_PARAM() \
	log_init_with_param()

#define LOG4C_INIT_WITH_PARAM_MULTI_PROCESS() \
	log_init_with_param_multi_process()


/**日志记录模块结束宏定义
注意事项: 必须正确结束日志模块后才不会造成内存、资源泄漏
使用例子: LOG4C_FINI();
*/
#define LOG4C_FINI() \
	log_fini()

/**日志记录宏定义
注意事项: 使用时参数必须使用两个括号括起来，LOG4C((X));见下面例子
使用例子: LOG4C((LOG_ERROR, "Hello World! My Name is %s, and my age is %d ", "Jess", "28" ));
*/
#define LOG4C(X) \
{ \
	const struct ST_LogParam st_LogParam = log_vsnprintf_wrapper X; \
	log_msg( \
		__FILE__, \
		__LINE__, \
		DEFAULT_LOG_CATEGORY_NAME, \
		st_LogParam.iLogLevel, \
		st_LogParam.strMsg \
	); \
	free((char *)st_LogParam.strMsg); \
}

#define LOG4C_NO_FILENUM(X) \
{ \
	const struct ST_LogParam st_LogParam = log_vsnprintf_wrapper X; \
	log_msg_no_file_num( \
	DEFAULT_LOG_CATEGORY_NAME, \
	st_LogParam.iLogLevel, \
	st_LogParam.strMsg \
	); \
	free((char *)st_LogParam.strMsg); \
}

#define LOG4C_NO_FILENUM_NO_LAYOUT(X) \
{ \
	const struct ST_LogParam st_LogParam = log_vsnprintf_wrapper X; \
	log_msg_no_file_num_no_layout( \
	DEFAULT_LOG_CATEGORY_NAME, \
	st_LogParam.iLogLevel, \
	st_LogParam.strMsg \
	); \
	free((char *)st_LogParam.strMsg); \
}

#define LOG4C_ORIGIN LOG4C_NO_FILENUM_NO_LAYOUT

#define LOG4C_BLOCK_BEGIN(X) \
{ \
	const struct ST_LogParam st_LogParam = log_vsnprintf_wrapper_msg X; \
	log4c_block_begin(__FILE__, __LINE__, st_LogParam.strMsg);\
	free((char *)st_LogParam.strMsg); \
}

#define LOG4C_BLOCK_END(X) \
{	\
	const struct ST_LogParam st_LogParam = log_vsnprintf_wrapper_msg X; \
	log4c_block_end(__FILE__, __LINE__, st_LogParam.strMsg);\
	free((char *)st_LogParam.strMsg); \
}

#define LOG4C_FUN(X) \
{ \
	const struct ST_LogParam st_LogParam = log_vsnprintf_wrapper_msg X; \
	const char *ptrStrMsg = log_vsnprintf_wrapper_fun_msg(__FUNCTION__, st_LogParam.strMsg); \
	log_msg(\
		__FILE__, \
		__LINE__, \
		DEFAULT_LOG_CATEGORY_NAME, \
		LOG4C_PRIORITY_TRACE, \
		ptrStrMsg \
		);\
	free((char *)st_LogParam.strMsg); \
	free((char *)ptrStrMsg); \
}

/** Output trace on condition.
This macro outputs a trace of any information needed, using standard stream
output operators. The output is only made if the conditional is TRUE.
example: LOG4C_IF((4>3, LOG4C_PRIORITY_TRACE, "HELLO"));
*/
#define LOG4C_IF(X) \
{ \
	const struct ST_LogParam st_LogParam = log_condition_vsnprintf_wrapper X; \
	if ( st_LogParam.iCondition ) \
	{ \
		log_msg( \
			__FILE__, \
			__LINE__, \
			DEFAULT_LOG_CATEGORY_NAME, \
			st_LogParam.iLogLevel, \
			st_LogParam.strMsg \
		); \
	} \
	free((char *)st_LogParam.strMsg); \
}

/** Trace the execution of a line.
This macro outputs a trace of a source file line execution.
*/
#define LOG4C_LINE() \
{ \
	char strBuffer[128] = {0}; \
	sprintf_s(strBuffer, sizeof(strBuffer), "line:%d", __LINE__);\
	log_msg( \
		__FILE__, \
		__LINE__, \
		DEFAULT_LOG_CATEGORY_NAME, \
		LOG4C_PRIORITY_TRACE, \
		strBuffer \
		); \
}

/**将内存中数据以十六进制方式打印出来的一个宏定义
example:LOG4C_HEX_DUMP((LOG4C_PRIORITY_TRACE, strHexBuf, iHexBufLen));
*/
#define LOG4C_HEX_DUMP(X)	\
{	\
	const struct ST_LogParam st_LogParam = log_hex_dump_vsnprintf_wrapper X; \
	char *ptrStr = log4c_sprintf_data((char*)(st_LogParam.strHexBuf), (st_LogParam.iHexBufLen));\
	log_msg( \
		__FILE__, \
		__LINE__, \
		DEFAULT_LOG_CATEGORY_NAME, \
		st_LogParam.iLogLevel, \
		ptrStr \
		); \
	free(ptrStr);\
}

/** 输出文件名和行号的日志输出宏,记录完日志直接return
然后再输入到LOG宏定义中
*/
#define LOG4C_RETURN(X) \
{ \
	LOG4C(X); \
	return; \
}

/** 输出文件名和行号的日志输出宏,记录完日志直接return returnValue;
然后再输入到LOG宏定义中
example:LOG4C_RETURN_WITH_VALUE(( LOG4C_PRIORITY_TRACE, "message%d", 1), returnValue);
*/
#define LOG4C_RETURN_WITH_VALUE(X, returnValue) \
{ \
	LOG4C(X); \
	return returnValue; \
}

#endif ///!defined(LOG4CPLUS_ENABLE)


#if defined(LOG4CPLUS_ENABLE)

/************** Begin of log4c/defs.h *******************************************/
/* $Id$
*
* Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef __log4c_defs_h
#define __log4c_defs_h

/**
* @file defs.h
*
* @brief types and declarations enclosures for C++.
*
**/

#ifdef  __cplusplus
# define __LOG4C_BEGIN_DECLS  extern "C" {
# define __LOG4C_END_DECLS    }
#else
# define __LOG4C_BEGIN_DECLS
# define __LOG4C_END_DECLS
#endif

#define LOGWRAPPER_INLINE inline
#define LOGWRAPPER_API    extern
#define LOGWRAPPER_DATA   extern

#ifdef __HP_cc
#define inline __inline
#endif

#ifdef _WIN32
///# include <log4c/config-win32.h>
#endif

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif /* GCC_VERSION */

#if GCC_VERSION < 2009
#define OLD_VARIADIC_MACRO 1
#endif

#endif /* __log4c_defs_h */

/************** End of log4c/defs.h *******************************************/

#ifdef _WIN32
/************** Begin of log4c/config-win32.h *******************************************/

/* $Id$
*
* See the COPYING file for the terms of usage and distribution.
*/

/* This file defines some labels as required for
compiling with Microsoft Visual C++ 6
*/

#ifndef __log4c_config_win32_h
#define __log4c_config_win32_h

#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif 
#include <time.h>
#include <windows.h>
///#include <winsock.h>


/// 生成和使用dll时使用
#ifndef LOGWRAPPER_DATA
#define LOGWRAPPER_DATA
#endif

#ifndef LOGWRAPPER_API
#define LOGWRAPPER_API
#endif

#ifndef LOG4C_CLASS_API
#define LOG4C_CLASS_API
#endif

/* This is defined to be 'inline' by default,
but with msvc6 undef it so that inlined
functions are just normal functions.
*/
#undef LOGWRAPPER_INLINE
#define LOGWRAPPER_INLINE

#endif /* __log4c_config_win32_h */

/************** End of log4c/config-win32.h *******************************************/
#endif

/************** Begin of log4c/priority.h *******************************************/
/* $Id$
*
* priority.h
*
* Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef log4c_priority_h
#define log4c_priority_h

/**
* @file priority.h
*
* @brief The priority class provides importance levels with which one can
* categorize log messages.
**/

///#include <log4c/defs.h>

__LOG4C_BEGIN_DECLS

/**
* Predefined Levels of priorities. These correspond to the priority levels
* used by syslog(3).
**/
typedef enum {
    /** fatal */	LOG4C_PRIORITY_FATAL = log4cplus::FATAL_LOG_LEVEL,
    /** alert */	LOG4C_PRIORITY_ALERT = LOG4C_PRIORITY_FATAL - 1000,
    /** crit */	    LOG4C_PRIORITY_CRIT = LOG4C_PRIORITY_ALERT - 1000,
    /** error */	LOG4C_PRIORITY_ERROR = log4cplus::ERROR_LOG_LEVEL,
    /** warn */	    LOG4C_PRIORITY_WARN = log4cplus::WARN_LOG_LEVEL,
    /** notice */	LOG4C_PRIORITY_NOTICE = LOG4C_PRIORITY_WARN - 1000,
    /** info */	    LOG4C_PRIORITY_INFO = log4cplus::INFO_LOG_LEVEL,
    /** debug */	LOG4C_PRIORITY_DEBUG = log4cplus::DEBUG_LOG_LEVEL,
    /** trace */	LOG4C_PRIORITY_TRACE = log4cplus::TRACE_LOG_LEVEL,
    /** all */		LOG4C_PRIORITY_ALL = LOG4C_PRIORITY_TRACE,
    /** notset */	LOG4C_PRIORITY_NOTSET = log4cplus::NOT_SET_LOG_LEVEL,
    /** unknown */	LOG4C_PRIORITY_UNKNOWN = LOG4C_PRIORITY_NOTSET
} log4c_priority_level_t;

/**
* @param a_priority a numeric value of the priority.
* @returns the given priority string name.
**/
LOGWRAPPER_API const char* log4c_priority_to_string(int a_priority);

/**
* @param a_priority_name a priority string name.
* @returns the given numeric value of the priority.
**/
LOGWRAPPER_API int log4c_priority_to_int(const char* a_priority_name);

__LOG4C_END_DECLS

#endif
/************** End of log4c/priority.h *******************************************/
#endif ///defined(LOG4CPLUS_ENABLE)


#if (defined(LOG4CPLUS_ENABLE) && defined(__cplusplus))

#ifdef _WIN32
///log4c正确使用必须的一些宏定义
#define snprintf _snprintf

#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H
#endif
///#define LOG4C_EXPORTS
#endif

///定义linux下使用到而不存在的类型
#ifdef linux
    ///typedef int BOOL;
#ifndef BOOL 
#define BOOL int
#define FALSE 0
#define TRUE 1
#endif
#define sprintf_s snprintf
///#define strcpy_s strncpy
#endif

/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,_CRT_PACKING)

 ///进行LOG宏定义参数获取的结果
    struct ST_LogParam
{
    char *strMsg;
    int iLogLevel;

    ///LOG4C_IF时的条件参数
    int iCondition;

    ///LOG4C_HEX_DUMP时的参数
    char *strHexBuf;
    int iHexBufLen;
};

#pragma pack(pop)

/// 日志输出缺省category
#ifndef DEFAULT_LOG_CATEGORY_NAME
#define DEFAULT_LOG_CATEGORY_NAME "root"
#endif

#if 1
/** 简炼日志记录宏定义
宏定义中直接对应了相应的日志级别
*/
#if 1
#if !defined(LOG_DISABLE_FATAL)
#define LOG_FATAL(message)  LOG(LOG4C_PRIORITY_FATAL, message)
#else
#define LOG_FATAL(message) do { } while (0)
#endif

#if !defined(LOG_DISABLE_ALERT)
#define LOG_ALERT(message)  LOG(LOG4C_PRIORITY_ALERT, message)
#else
#define LOG_ALERT(message) do { } while (0)
#endif

#if !defined(LOG_DISABLE_CRIT)
#define LOG_CRIT(message)  LOG(LOG4C_PRIORITY_CRIT, message)
#else
#define LOG_CRIT(message) do { } while (0)
#endif
#if !defined(LOG_DISABLE_ERROR)
#define LOG_ERROR(message)  LOG(LOG4C_PRIORITY_ERROR, message)
#else
#define LOG_ERROR(message) do { } while (0)
#endif

#if !defined(LOG_DISABLE_WARN)
#define LOG_WARN(message)  LOG(LOG4C_PRIORITY_WARN, message)
#else
#define LOG_WARN(message) do { } while (0)
#endif
#if !defined(LOG_DISABLE_NOTICE)
#define LOG_NOTICE(message)  LOG(LOG4C_PRIORITY_NOTICE, message)
#else
#define LOG_NOTICE(message) do { } while (0)
#endif

#if !defined(LOG_DISABLE_INFO)
#define LOG_INFO(message)  LOG(LOG4C_PRIORITY_INFO, message)
#else
#define LOG_INFO(message) do { } while (0)
#endif
#if !defined(LOG_DISABLE_DEBUG)
#define LOG_DEBUG(message)  LOG(LOG4C_PRIORITY_DEBUG, message)
#else
#define LOG_DEBUG(message) do { } while (0)
#endif

#if !defined(LOG_DISABLE_TRACE)
#define LOG_TRACE(message)  LOG(LOG4C_PRIORITY_TRACE, message)
#else
#define LOG_TRACE(message) do { } while (0)
#endif

#if !defined(LOG_DISABLE_NOTSET)
#define LOG_NOTSET(message)  LOG(LOG4C_PRIORITY_NOTSET, message)
#else
#define LOG_NOTSET(message) do { } while (0)
#endif

#if !defined(LOG_DISABLE_UNKNOWN)
#define LOG_UNKNOWN(message)  LOG(LOG4C_PRIORITY_UNKNOWN, message)
#else
#define LOG_UNKNOWN(message) do { } while (0)
#endif

#else
#define LOG_FATAL(message)		LOG(LOG4C_PRIORITY_FATAL, message)
#define LOG_ALERT(message)		LOG(LOG4C_PRIORITY_ALERT, message)
#define LOG_CRIT(message)		LOG(LOG4C_PRIORITY_CRIT, message)
#define LOG_ERROR(message)		LOG(LOG4C_PRIORITY_ERROR, message)
#define LOG_WARN(message)		LOG(LOG4C_PRIORITY_WARN, message)
#define LOG_NOTICE(message)		LOG(LOG4C_PRIORITY_NOTICE, message)
#define LOG_INFO(message)		LOG(LOG4C_PRIORITY_INFO, message)
#define LOG_DEBUG(message)		LOG(LOG4C_PRIORITY_DEBUG, message)
#define LOG_TRACE(message)		LOG(LOG4C_PRIORITY_TRACE, message)
#define LOG_NOTSET(message)		LOG(LOG4C_PRIORITY_NOTSET, message)
#define LOG_UNKNOWN(message)	LOG(LOG4C_PRIORITY_UNKNOWN	, message)
#endif

#else

/** 日志记录级别
*/
/** fatal */	#define LOG_FATAL		LOG4C_PRIORITY_FATAL	
/** alert */	#define LOG_ALERT		LOG4C_PRIORITY_ALERT	
/** crit */	    #define LOG_CRIT		LOG4C_PRIORITY_CRIT		
/** error */	#define LOG_ERROR		LOG4C_PRIORITY_ERROR	
/** warn */	    #define LOG_WARN		LOG4C_PRIORITY_WARN		
/** notice */	#define LOG_NOTICE		LOG4C_PRIORITY_NOTICE	
/** info */	    #define LOG_INFO		LOG4C_PRIORITY_INFO		
/** debug */	#define LOG_DEBUG		LOG4C_PRIORITY_DEBUG	
/** trace */	#define LOG_TRACE		LOG4C_PRIORITY_TRACE	
/** notset */	#define LOG_NOTSET		LOG4C_PRIORITY_NOTSET	
/** unknown */	#define LOG_UNKNOWN		LOG4C_PRIORITY_UNKNOWN
#endif

///#include <log4c/defs.h>

__LOG4C_BEGIN_DECLS

/** 日志模块初始化
@return int:return 0 for success
*/
LOGWRAPPER_API int log_init();

/** 日志模块初始化,指定配置文件名称
@return int:return 0 for success
*/
LOGWRAPPER_API int log_init_with_cfg_file(const char *strCfgFileName);

/** 日志模块清理
@return int:return 0 for success
*/
LOGWRAPPER_API int log_fini();

/**宏参数抽取priority函数
本函数接受LOG_DEBUG(X)的参数，并从该宏定义的参数中返回priority的值
@return const int : iLogLevel.
*/
LOGWRAPPER_API const int log_get_priority_wrapper(
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
);

/**
将大小为count的缓冲区中内容按字节以16进制字符串打印出来,
返回值即为指向相应的字符串，
该返回指向的存储区域要调用本函数的用户显示的进行删除
*/
LOGWRAPPER_API char * log4c_sprintf_data(char *buff, int count);

/**vsnprintf函数的wrapper函数
本函数只使用第二个及以后的参数,使用vsnprintf函数将strFormat和后面的不定参数va_list va;以
vsnprintf(strMsg, sizeof(strMsg), strFormat, va);
形式格式化输出到一缓冲区中并以字符串的形式返回
*/
LOGWRAPPER_API const struct ST_LogParam log_vsnprintf_wrapper(
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
);

/**vsnprintf函数的wrapper函数
本函数只使用第二个及以后的参数,使用vsnprintf函数将strFormat和后面的不定参数va_list va;以
vsnprintf(strMsg, sizeof(strMsg), strFormat, va);
形式格式化输出到一缓冲区中并以字符串的形式返回
*/
LOGWRAPPER_API const struct ST_LogParam log_condition_vsnprintf_wrapper(
    const int iCondition,	///条件
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
);

LOGWRAPPER_API const struct ST_LogParam log_hex_dump_vsnprintf_wrapper(
    const int iLogLevel,	///日志记录级别
    const char* strHexBuf,	///缓冲区首地址
    const int iHexBufLen,	///缓冲区长度
    ...						///日志内容
);

/**vsnprintf函数的wrapper函数
本函数只使用第二个及以后的参数,使用vsnprintf函数将strFormat和后面的不定参数va_list va;以
vsnprintf(strMsg, sizeof(strMsg), strFormat, va);
形式格式化输出到一缓冲区中并以字符串的形式返回
*/
LOGWRAPPER_API const struct ST_LogParam log_vsnprintf_wrapper_msg(
    const char* strFormat,	///日志内容格式
    ...						///日志内容
);

/**vsnprintf函数的wrapper函数
本函数只使用第二个及以后的参数,使用vsnprintf函数将strFormat和后面的不定参数va_list va;以
vsnprintf(strMsg, sizeof(strMsg), strFormat, va);
形式格式化输出到一缓冲区中并以字符串的形式返回
*/
LOGWRAPPER_API const char * log_vsnprintf_wrapper_fun_msg(
    const char* strFunName,	///函数名
    const char* strFormat,	///日志内容格式
    ...						///日志内容
);

/** 日志记录
日志记录为一个字符串指针指向的内容
@return void
*/
LOGWRAPPER_API void log_msg(
    const char *strFile,				///文件名
    const int iLineNum,					///行号
    ///const log4cplus::Logger &loggerObj,	///Logger对象引用
    const char *strCatName,				///category名
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
);

/** 日志记录,不记录文件名和行号
日志记录为一个字符串指针指向的内容
@return void
*/
LOGWRAPPER_API void log_msg_no_file_num(
    ///const log4cplus::Logger &loggerObj,	///Logger对象引用
    const char *strCatName,				///category名
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
);

/** 日志记录,不记录文件名和行号,没有任何layout转换，直接输出相应的字符文本到日志中
此条记录没有行号，也没有线程号，也没有回车等
日志记录为一个字符串指针指向的内容
@return void
*/
LOGWRAPPER_API void log_msg_no_file_num_no_layout(
    ///const log4cplus::Logger &loggerObj,	///Logger对象引用
    const char *strCatName,				///category名
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
);


/** 日志模块初始化
以日志配置文件字符串内容和日志配置文件命作为参数
若相应配置文件存在则使用，否则按照给定内容和文件名创建日志配置文件并使用之
@return int，0 表成功
*/
LOGWRAPPER_API int log_init_with_string(
    const char *strFileContent,
    const char *strFileName
);

/** 日志模块初始化
以日志配置文件命作为参数
@return int，0 表成功
*/
LOGWRAPPER_API int log_init_with_cfg_file_wrapper(
    const char * strConfigFile
);

/** 日志模块初始化-日志配置文件的文件名设置
以日志配置文件命作为参数
@return void
*/
LOGWRAPPER_API void log_set_log_cfg_file_name(const char *strFileName);

/** 日志模块初始化-设置存放日志文件的目录
@return void
*/
LOGWRAPPER_API void log_set_log_path(const char *strLogPath);

/** 日志模块初始化-生成的日志文件的文件名设置
以日志文件命作为参数
@return void
*/
LOGWRAPPER_API void log_set_log_file_name(const char *strFileName);

/** 日志模块初始化-日志记录级别设置
@return void
*/
LOGWRAPPER_API void log_set_log_level(const char *strLogLevel);

/** 日志模块初始化-日志记录文件的大设置
@return void
*/
LOGWRAPPER_API void log_set_log_file_size(const char *strFileSize);

/** 日志模块初始化-日志记录文件的个数设置
记录到最大个数后将回滚重复覆盖第一个日志文件，依次类推
@return void
*/
LOGWRAPPER_API void log_set_log_file_num(const int iFileNum);

/** 日志模块初始化-是否实时读取日志配置文件
设置是否每次记录时都读取日志配置文件
@param:const BOOL m_bReReadLogCfgFile
@return void
*/
LOGWRAPPER_API void log_set_reread_log_cfg_file(const BOOL bReReadLogCfgFile);

/** 日志模块初始化-实时读取日志配置文件的时间,单位(毫秒)
设置每次记录时都读取日志配置文件的时间
@param:const DWORD& dwReReadMilliseconds
@return void
*/
LOGWRAPPER_API void log_set_reread_log_cfg_file_elapse(const DWORD& dwReReadMilliseconds);

/** 日志模块初始化-设置log4cplus logger name
@param:const char *strLoggerName
@return void
*/
LOGWRAPPER_API void log_set_log_logger_name(const char *strLoggerName);

/** 日志模块初始化-设置log4cplus app name
@param:const char *strAppName
@return void
*/
LOGWRAPPER_API void log_set_log_app_name(const char *strAppName);

/** 日志模块初始化-设置log4cplus immediateFlush
@param:const BOOL bImmediateFlush
@return void
*/
LOGWRAPPER_API void log_set_log_immediate_flush(const BOOL bImmediateFlush);

/** 日志模块初始化-设置log4cplus BufferSize
@param:const int iBufferSize
@return void
*/
LOGWRAPPER_API void log_set_log_buffer_size(const int iBufferSize);

/** 日志模块初始化-带参数进行日志模块初始化
这里的参数则为上面的几个API设置的参数，
所以本函数一定要在日志配置参数设置好之后调用设置的参数才能生效的
@return int,RESULT_OK,成功;RESULT_FAILURE,失败
*/
LOGWRAPPER_API int log_init_with_param();

/** 日志模块初始化-带参数进行日志模块初始化,程序的多个运行实例产生的日志不互相冲突
这里的参数则为上面的几个API设置的参数，
所以本函数一定要在日志配置参数设置好之后调用设置的参数才能生效的
@return int,OK,成功;FAILURE,失败
*/
LOGWRAPPER_API int log_init_with_param_multi_process();

/** 日志记录Block方式能清楚的表明调用层次
类似下面这种B-Entry类型日志:
main.cpp(87) B-Entry	==> main
main.cpp(90) B-Entry	====> main_01
main.cpp(92) B-Entry	======> main_02
main.cpp(92) B-Exit	<====== main_02
main.cpp(90) B-Exit	<==== main_01
@return void
*/
LOGWRAPPER_API void log4c_block_begin(const char * fileName, int lineNum, const char * traceName);

/** 日志记录Block方式能清楚的表明调用层次
类似下面这种B-Exit类型日志:
main.cpp(87) B-Entry	==> main
main.cpp(90) B-Entry	====> main_01
main.cpp(92) B-Entry	======> main_02
main.cpp(92) B-Exit	<====== main_02
main.cpp(90) B-Exit	<==== main_01
@return void
*/
LOGWRAPPER_API void log4c_block_end(const char * fileName, int lineNum, const char * traceName);

/**检测配置文件是否存在
@return const int : iLogLevel.
*/
extern const int log_check(void);

/**检测配置文件是否存在,只检测传入的配置文件名
@return const int : iLogLevel.
*/
extern const int log_check_with_cfg_file(const char *strCfgFileName);

/**宏参数抽取format函数
本函数接受LOG_DEBUG(X)的参数，并从该宏定义的参数中返回format的值
@return const char* : strFormat
*/
extern const char* log_get_format_wrapper(
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
);

/** 日志记录
支持类似printf函数的带格式输出
@return void
*/
extern void log_log(
    const log4cplus::Logger &loggerObj,	///Logger对象引用
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
);

/** 日志模块初始化-日志模块使用实例个数自增
@return void
*/
extern void IncreaseLogModuleUsage();

/** @fn GetLogAppName
@brief 获取记录日志的应用程序名字
@return const char *
*/
extern const char * GetLogAppName();

/** @fn GetReReadLogCfgFile
@brief 获取是否要重新读取配置文件
@return const char *
*/
extern BOOL GetReReadLogCfgFile();

/** @fn GetReReadMilliseconds
@brief 获取重新读取配置文件的时间,单位(毫秒)
@return const char *
*/
extern DWORD GetReReadMilliseconds();

__LOG4C_END_DECLS

///#include "log4c.h"
#endif ///(defined(LOG4CPLUS_ENABLE) && defined(__cplusplus))

#endif // !defined(AFX_LOG_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_)
/************** End of Log.h *******************************************/


/************** Begin of GlobalMacro.h *******************************************/
/**********************************************************
* 版权所有 (C)2008, woyo.com
*
* 文件名称：GlobalMacro.h
* 内容摘要：全局宏定义的头文件
*			本文件包含了所有要用到的全局宏的定义
**********************************************************/
#if !defined(AFX_GLOBALMACRO_H__DD58A78D_C125_410F_B4C8_F0067B797121__INCLUDED_)
#define AFX_GLOBALMACRO_H__DD58A78D_C125_410F_B4C8_F0067B797121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push)  
#pragma warning(disable:4996 4090 4018) 
///#pragma warning(once:4996)

/// 解决VC6下无__FUNCTION__宏定义的问题
#if _MSC_VER <= 1200 ///vs 2005
#ifndef __FUNCTION__
#define __FUNCTION__ ""
#endif	
#else
    /// do nothing...
#endif

/* 函数返回基本状态信息  */
#ifdef RESULT_OK
#undef RESULT_OK
#endif /* #ifdef RESULT_OK */
#ifndef RESULT_OK
#define RESULT_OK               (0)    /* 成功返回	*/
#endif /* #ifndef OK */

/* 函数返回基本状态信息  */
#ifdef RESULT_FAILURE
#undef RESULT_FAILURE
#endif /* #ifdef RESULT_FAILURE */
#ifndef RESULT_FAILURE
#define RESULT_FAILURE           (-1)    /* 失败返回	*/
#endif /* #ifndef RESULT_FAILURE */


///一些宏定义

///删除一个数组指针的宏定义
//lint -emacro(774, DELETEA)
#ifndef DELETEA
#define DELETEA(ptr)	\
	if (NULL != ptr)	\
	{ \
		delete[] ptr;	\
		ptr = NULL;		\
	}
#endif

///删除一个指针的宏定义
#ifndef FREEP
#define FREEP(ptr)		\
	if (NULL != ptr)	\
	{					\
		free(ptr) ;		\
		ptr = NULL;		\
	}
#endif

///删除一个指针的宏定义
#ifndef GLOBALFREEP
#define GLOBALFREEP(ptr)	\
	if(NULL != ptr)			\
	{						\
		GlobalFree(ptr) ;	\
		ptr = NULL;			\
	}
#endif

///删除一个指针的宏定义
//lint -emacro(774, DELETEP)
#ifndef DELETEP
#define DELETEP(ptr)	\
	if (NULL != (ptr))	\
	{					\
		delete (ptr);	\
		(ptr) = NULL;	\
	}
#endif

///删除一个GDI对象的宏定义
//lint -emacro(774, DELETEOBJECT)
#ifndef DELETEOBJECT
#define DELETEOBJECT(ptr)		\
	if (NULL != (ptr))			\
	{							\
		::DeleteObject (ptr);	\
		(ptr) = NULL;			\
	}
#endif

///Destroy一个Window
//lint -emacro(774, DESTROYWINDOW)
#ifndef DESTROYWINDOW
#define DESTROYWINDOW(hWnd)	\
	if (IsWindow(hWnd)) \
	{ \
		DestroyWindow(hWnd); \
	}
#endif

///关闭一个Handle
//lint -emacro(774, CLOSEHANDLE)
#ifndef CLOSEHANDLE
#define CLOSEHANDLE(handle)		\
	if (NULL != (handle))		\
	{							\
		::CloseHandle (handle);	\
		(handle) = NULL;		\
	}
#endif

///使用FreeLibrary卸载一个库的宏定义
#ifndef FREELIBRARY
#define FREELIBRARY(hModule)	\
	if (NULL != hModule)		\
	{							\
		FreeLibrary(hModule) ;	\
		hModule = NULL;			\
	}
#endif

///使用AfxFreeLibrary卸载一个库的宏定义
#ifndef AFXFREELIBRARY
#define AFXFREELIBRARY(hModule)	\
	if (NULL != hModule)		\
	{							\
		AfxFreeLibrary(hModule);\
		hModule = NULL;			\
	}
#endif

///删除一个指针的宏定义
//lint -emacro(774, FREEP)
#ifndef FREEP
#undef  FREEP
#define FREEP(ptr)		\
	if (NULL != ptr)	\
	{					\
		free(ptr) ;		\
		ptr = NULL;		\
	}
#endif

/** 定义的根据输入类型来删除不同类型的指针的宏定义
*/
#ifndef DELETE_TYPE_P
#define DELETE_TYPE_P(Type, ptrEvent) \
	{\
		Type *ptrEventLocal = (Type *)ptrEvent; \
		DELETEP(ptrEventLocal); \
		ptrEvent = NULL; \
	}
#endif

/** 定义的根据输入类型来删除不同类型的指针的宏定义
*/
#ifndef DELETE_TYPE_A
#define DELETE_TYPE_A(Type, ptrEvent) \
	{\
		Type *ptrEventLocal = (Type *)ptrEvent; \
		DELETEA(ptrEventLocal); \
		ptrEvent = NULL; \
	}
#endif

#ifndef VOS_DELETE_SEM
#define VOS_DELETE_SEM(semId)	\
	if (NULL != semId)			\
	{							\
		VOS_DeleteSem(semId);	\
		semId = NULL;			\
	}
#endif

#ifndef VOS_DELETE_THREAD_MUTEX
#define VOS_DELETE_THREAD_MUTEX(semId)	\
	if (NULL != semId)					\
	{									\
		VOS_DeleteThreadMutex(semId);	\
		semId = NULL;					\
	}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后以return_value值return
*/
#ifndef PAssert_ReturnValue
#define PAssert_ReturnValue(condition, return_value) \
	{  \
		if (!(condition)) \
		{ \
			return (return_value); \
		} \
	}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后return
*/
#ifndef PAssert_Return
#define PAssert_Return(condition) \
	{  \
		if (!(condition)) \
		{ \
			return ; \
		} \
	}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后以return_value值return
*/
#ifndef PAssert_Statement_ReturnValue
#define PAssert_Statement_ReturnValue(condition, statement, return_value) \
	{  \
		if (!(condition)) \
		{ \
			statement; \
			return (return_value); \
		} \
	}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后return
*/
#ifndef PAssert_Statement_Return
#define PAssert_Statement_Return(condition, statement) \
	{  \
		if (!(condition)) \
		{ \
			statement; \
			return ; \
		} \
	}
#endif

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement语句，然后以return_value值return
*/
#ifndef PAssertNotNull_Statement_ReturnValue
#define PAssertNotNull_Statement_ReturnValue(ptr, statement, return_value) \
	{ \
		if ( (ptr) == NULL) \
		{ \
			statement; \
			return (return_value); \
		} \
	}
#endif

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement语句，然后return
*/
#ifndef PAssertNotNull_Statement_Return
#define PAssertNotNull_Statement_Return(ptr, statement) \
	{ \
		if ( (ptr) == NULL) \
		{ \
			statement; \
			return ; \
		} \
	}
#endif

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement语句，然后以return_value值return
*/
#ifndef PAssertNotNull_ReturnValue
#define PAssertNotNull_ReturnValue(ptr, return_value) \
	{ \
		if ( (ptr) == NULL) \
		{ \
			return (return_value); \
		} \
	}
#endif

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement语句，然后return
*/
#ifndef PAssertNotNull_Return
#define PAssertNotNull_Return(ptr) \
	{ \
		if ( (ptr) == NULL) \
		{ \
			return ; \
		} \
	}
#endif

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行continue语句
*/
#define PAssertNotNull_Continue(ptr) \
	{ \
	if ( (ptr) == NULL) \
		{ \
			continue ; \
		} \
	}

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement,然后执行continue语句
*/
#define PAssertNotNull_Statement_Continue(ptr, statement) \
	{ \
	if ( (ptr) == NULL) \
		{ \
			statement; \
			continue ; \
		} \
	}

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行break语句
*/
#define PAssertNotNull_Break(ptr) \
	{ \
		if( (ptr) == NULL) \
		{ \
			break ; \
		} \
	}

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement,然后执行break语句
*/
#define PAssertNotNull_Statement_Break(ptr, statement) \
	{ \
		if( (ptr) == NULL) \
		{ \
			statement; \
			break ; \
		} \
	}

/** This macro is used to do something and return
执行一个语句statement，然后return return_value
*/
#ifndef PStatement_Return
#define PStatement_Return(statement) \
{ \
	statement; \
	return ; \
}
#endif

/** This macro is used to do something and return
执行一个语句statement，然后return return_value
*/
#ifndef PStatement_ReturnValue
#define PStatement_ReturnValue(statement, return_value) \
{ \
	statement; \
	return return_value; \
}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行break语句
*/
#ifndef PAssert_Break
#define PAssert_Break(condition) \
{  \
	if (!(condition)) \
	{ \
		break ; \
	} \
}
#endif

/** This macro is used to do something and break
执行一个语句statement，然后break
*/
#ifndef PStatement_Break
#define PStatement_Break(statement) \
{ \
	statement; \
	break; \
}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后执行break语句
*/
#ifndef PAssert_Statement_Break
#define PAssert_Statement_Break(condition, statement) \
{  \
	if (!(condition)) \
	{ \
		statement; \
		break ; \
	} \
}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行continue语句
*/
#ifndef PAssert_Continue
#define PAssert_Continue(condition) \
{  \
	if (!(condition)) \
	{ \
		continue ; \
	} \
}
#endif

/** This macro is used to do something and continue
执行一个语句statement，然后continue
*/
#ifndef PStatement_Continue
#define PStatement_Continue(statement) \
{ \
	statement; \
	continue; \
}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后执行continue语句
*/
#ifndef PAssert_Statement_Continue
#define PAssert_Statement_Continue(condition, statement) \
{  \
	if (!(condition)) \
	{ \
		statement; \
		continue ; \
	} \
}
#endif

/** 空操作
*/
#ifndef NULL_OPERATION
#define NULL_OPERATION
#endif

///memset缺省构造函数
#ifndef MEMSET_CONSTRUCTOR
#define MEMSET_CONSTRUCTOR(ClassType) \
	ClassType() \
	{ \
		memset(this, 0, sizeof(ClassType)); \
	}
#endif

#if 1
#ifdef __cplusplus
/**
按照特定数据类型删除该数据类型的数组指针
*/
#ifndef DELETE_ARRAY_TEMPLATE
#define DELETE_ARRAY_TEMPLATE
template <class classType>
void DeleteArray(void *&ptr)
{
    classType *ptrClassType = (classType *)ptr;
    DELETEA(ptrClassType);
    ptr = NULL;
}
#endif

/**
按照特定数据类型删除该数据类型的指针
*/
#ifndef DELETE_TEMPLATE
#define DELETE_TEMPLATE
template <class classType>
void Delete(void *&ptr)
{
    classType *ptrClassType = (classType *)ptr;
    DELETEP(ptrClassType);
    ptr = NULL;
}
#endif
#endif
#endif

///左移宏定义
#ifndef LEFT_SHIFT
#define LEFT_SHIFT(uiData) \
	( (uiData) * 2)
#endif


///NEW系列宏定义
/** @macro MACRO_NEW_RETURNVALUE
@brief new CONSTRUCTOR对象给相应指针POINTER，失败则return RET_VAL
@details 参照ACE 中ACE_NEW_RETURN宏定义
    1.POINTER指针必须在本宏定义调用时先定义好。
    2.根据构造函数CONSTRUCTOR new一个对象赋给POINTER。
    3.失败return RET_VAL.
*/
#ifndef MACRO_NEW_RETURNVALUE
#define MACRO_NEW_RETURNVALUE(POINTER, CONSTRUCTOR, RET_VAL) \
	do \
	{ \
		POINTER = new  CONSTRUCTOR; \
		if (POINTER == 0) \
		{ \
			return RET_VAL; \
		} \
	} while (0);
#endif

#ifndef MACRO_NEW_RETURN
#define MACRO_NEW_RETURN(POINTER, CONSTRUCTOR) \
	do \
	{ \
		POINTER = new CONSTRUCTOR; \
		if (POINTER == 0) \
		{ \
			return; \
		} \
	} while (0);
#endif

#ifndef MACRO_NEW_NORETURN
#define MACRO_NEW_NORETURN(POINTER, CONSTRUCTOR) \
	do \
	{ \
		POINTER = new CONSTRUCTOR; \
		if (POINTER == 0) \
		{ } \
	} while (0);
#endif

/** @macro CHECK_RUN_ONCE_BEGIN
@brief 保证代码只运行一次
@details 保证嵌在CHECK_RUN_ONCE_BEGIN和CHECK_RUN_ONCE_END之间的代码只运行一次
@example
    CHECK_RUN_ONCE_BEING();
    {
        ///下面添加只执行一次的代码
        int i++;
        ...
    }
    CHECK_RUN_ONCE_BEGIN();
*/
#ifndef CHECK_RUN_ONCE_BEGIN
#define CHECK_RUN_ONCE_BEGIN() \
	do \
	{ \
		static UINT s_uiNumRun##NAME = 0; \
		if (0 == s_uiNumRun##NAME) \
		{ \
			s_uiNumRun##NAME ++;
#endif

#ifndef CHECK_RUN_ONCE_END
#define CHECK_RUN_ONCE_END() \
		} \
	} \
	while (0);
#endif

/** @macro CHECK_RUN_COUNT_BEGIN
@brief CHECK_RUN_COUNT_BEGIN()和CHECK_RUN_COUNT_END之间代码保证执行次数至多为max_count次
@details
    1.每次运行到CHECK_RUN_COUNT_BEGIN和CHECK_RUN_COUNT_END之间代码时，若执行次数没达到max_count次则执行一次。
    2.保证CHECK_RUN_COUNT_BEGIN和CHECK_RUN_COUNT_END之间代码运行次数不超过max_count次。
@param max_count 执行的最大次数
@example
    CHECK_RUN_COUNT_BEGIN(max_count);
    {
        ///下面的代码将至多执行max_count次
        ///do something...
    }
    CHECK_RUN_COUNT_END();
*/
#ifndef CHECK_RUN_COUNT_BEGIN
#define CHECK_RUN_COUNT_BEGIN(max_count) \
		do \
		{ \
			static UINT s_uiCount = 0; \
			if (s_uiCount < max_count) \
			{
#endif

#ifndef CHECK_RUN_COUNT_END
#define CHECK_RUN_COUNT_END() \
				s_uiCount ++; \
			} \
		} \
		while (0);
#endif

/** @macro CHECK_CONDITION_RUN_ONCE_BEGIN
@brief 带条件执行保证满足条件时,嵌在CHECK_CONDITION_RUN_ONCE_BEGIN和CHECK_CONDITION_RUN_ONCE_END之间的代码只运行一次
@param condition 满足的条件表达式
@example
    CHECK_CONDITION_RUN_ONCE_BEGIN(8>7);
    {
        ///必须上面的表达式(8>7)为true才会执行本段代码
        ///下面添加只执行一次的代码
        int i++;
        ...
    }
    CHECK_CONDITION_RUN_ONCE_END();
*/
//#ifndef CHECK_CONDITION_RUN_ONCE_BEGIN
//#define CHECK_CONDITION_RUN_ONCE_BEGIN(condition) \
//		do \
//		{ \
//			if (condition) \
//			{ \
//				CHECK_RUN_ONCE_BEGIN(); \
//				{
//#endif
//
//#ifndef CHECK_CONDITION_RUN_ONCE_END
//#define CHECK_CONDITION_RUN_ONCE_END() \
//				} \
//				CHECK_RUN_ONCE_END(); \
//			} \
//		} while (0);
//#endif

#ifndef CHECK_CONDITION_RUN_ONCE_BEGIN
#define CHECK_CONDITION_RUN_ONCE_BEGIN(condition) \
		do \
		{ \
			static UINT s_uiRunCount = 0; \
			if (0 == s_uiRunCount) \
			{ \
				if (condition) \
				{
#endif

#ifndef CHECK_CONDITION_RUN_ONCE_END
#define CHECK_CONDITION_RUN_ONCE_END() \
					s_uiRunCount ++;\
				} \
			} \
		} while (0);
#endif

/** @macro CHECK_CONDITION_RUN_COUNT_BEGIN
@brief 带条件执行保证满足条件时,嵌在CHECK_CONDITION_RUN_COUNT_BEGIN和CHECK_CONDITION_RUN_COUNT_END之间的代码只运行一次
@param condition 满足的条件表达式
@example
    CHECK_CONDITION_RUN_COUNT_BEGIN(8>7, 5);
    {
        ///必须上面的表达式(8>7)为true才会执行本段代码
        ///下面添加只执行5次的代码
        int i++;
        ...
    }
    CHECK_CONDITION_RUN_COUNT_END();
*/
//#ifndef CHECK_CONDITION_RUN_COUNT_BEGIN
//#define CHECK_CONDITION_RUN_COUNT_BEGIN(condition, max_count) \
//		do \
//		{ \
//			if (condition) \
//			{ \
//				CHECK_RUN_COUNT_BEGIN(max_count); \
//				{
//#endif
//
//#ifndef CHECK_CONDITION_RUN_COUNT_END
//#define CHECK_CONDITION_RUN_COUNT_END() \
//				} \
//				CHECK_RUN_COUNT_END(); \
//			} \
//		} while (0);
//#endif

#ifndef CHECK_CONDITION_RUN_COUNT_BEGIN
#define CHECK_CONDITION_RUN_COUNT_BEGIN(condition, max_count) \
		do \
		{ \
			static UINT s_uiRunCount = 0; \
			if (s_uiRunCount < max_count ) \
			{ \
				if (condition) \
				{
#endif

#ifndef CHECK_CONDITION_RUN_COUNT_END
#define CHECK_CONDITION_RUN_COUNT_END() \
					s_uiRunCount ++;\
				} \
			} \
		} while (0);
#endif

/** @macro DEBUG_ENABLE_ONLY_BEGIN
@brief DEBUG_ENABLE_ONLY_BEGIN()和DEBUG_ENABLE_ONLY_END()之间代码只在DEBUG下才有效
@details
    1.通过debug下if(1){...}来包含所处理代码.其实可以考虑#if 1
    2.通过release下if(0){...}来包含所处理代码.其实可以考虑#if 0
    3.具体的代码优化就看编译器的处理了。
@example
    DEBUG_ENABLE_ONLY_BEGIN();
    {
        printf("I just work in debug mode.");
    }
    DEBUG_ENABLE_ONLY_END();
*/
#ifdef _DEBUG
#ifndef DEBUG_ENABLE_ONLY_BEGIN
#define DEBUG_ENABLE_ONLY_BEGIN() \
		if(1) \
		{ 
#endif

#ifndef DEBUG_ENABLE_ONLY_END
#define DEBUG_ENABLE_ONLY_END() \
		}
#endif
#else
#ifndef DEBUG_ENABLE_ONLY_BEGIN
#define DEBUG_ENABLE_ONLY_BEGIN() \
		if(0) \
		{ 
#endif

#ifndef DEBUG_ENABLE_ONLY_END
#define DEBUG_ENABLE_ONLY_END() \
		}
#endif
#endif

/** @macro TIMER_TRIGGER_BEGIN
@brief 每隔millionsecondsInterval毫秒时间间隔,嵌在TIMER_TRIGGER_BEGIN和TIMER_TRIGGER_END之间的代码运行一次
@details
    1.首先TIMER_TRIGGER_BEGIN和TIMER_TRIGGER_END代码所在函数要能不断被调用到。
    2.调用到后，每隔millionsecondsInterval毫秒才会进入执行TIMER_TRIGGER_BEGIN和TIMER_TRIGGER_END之间代码。
@param millionsecondsInterval 毫秒数
@example
    TIMER_TRIGGER_BEGIN(2000);
    {
        ///下面添加每隔2000毫秒执行一次的代码
        int i++;
        ...
    }
    TIMER_TRIGGER_END();
*/
#ifndef TIMER_TRIGGER_BEGIN
#define TIMER_TRIGGER_BEGIN(millionsecondsInterval) \
	do \
	{ \
		static UINT s_uiTickCount = GetTickCount(); \
		UINT uiTickCount = GetTickCount(); \
		if ( (uiTickCount - s_uiTickCount) > millionsecondsInterval ) \
		{
#endif

#ifndef TIMER_TRIGGER_END
#define TIMER_TRIGGER_END() \
			s_uiTickCount = uiTickCount; \
		} \
	} while (0);
#endif

/** @macro OUTPUTDEBUGSTRING_DEBUG
@brief 只在DEBUG版本起作用的OutputDebugString输出宏定义
*/
#ifndef OUTPUTDEBUGSTRING_DEBUG
#ifdef _DEBUG
#define OUTPUTDEBUGSTRING_DEBUG(str) OutputDebugString(str)
#else
#define OUTPUTDEBUGSTRING_DEBUG(str)
#endif
#endif

///获取数组的维数，等同于winnt.h中_ARRAYSIZE/ARRAYSIZE
#ifndef PARRAYSIZE
#define PARRAYSIZE(array) ((sizeof(array)/sizeof(array[0])))
#endif

/** Declare all the standard PWlib class information.
This macro is used to provide the basic run-time typing capability needed
by the library. All descendent classes from the #PObject# class require
these functions for correct operation. Either use this macro or the
#PDECLARE_CLASS# macro.

The use of the #PDECLARE_CLASS# macro is no longer recommended for reasons
of compatibility with documentation systems.
*/
#ifndef CLASSINFO
#define CLASSINFO(cls, par) \
  public: \
	static const char * Class() \
	{ return #cls; } \
	virtual const char * GetClass(unsigned ancestor = 0) const \
	{ return ancestor > 0 ? par::GetClass(ancestor-1) : cls::Class(); } \
	virtual BOOL IsClass(const char * clsName) const \
	{ return strcmp(clsName, cls::Class()) == 0; } \
	virtual BOOL IsDescendant(const char * clsName) const \
	{ return strcmp(clsName, cls::Class()) == 0 || \
	  par::IsDescendant(clsName); } 
#endif

/** Declare all the standard PWlib class information.
This macro is used to provide the basic run-time typing capability needed
by the library. All descendent classes from the #PObject# class require
these functions for correct operation. Either use this macro or the
#PDECLARE_CLASS# macro.

The use of the #PDECLARE_CLASS# macro is no longer recommended for reasons
of compatibility with documentation systems.
*/
#ifndef CLASSINFO_BASE
#define CLASSINFO_BASE(cls) \
  public: \
	static const char * Class() \
	{ return #cls; } \
	virtual const char * GetClass(unsigned ancestor = 0) const \
	{ return cls::Class(); } \
	virtual BOOL IsClass(const char * clsName) const \
	{ return strcmp(clsName, cls::Class()) == 0; } \
	virtual BOOL IsDescendant(const char * clsName) const \
	{ return strcmp(clsName, cls::Class()) == 0; } 
#endif

#pragma warning (pop)

#endif // !defined(AFX_GLOBALMACRO_H__DD58A78D_C125_410F_B4C8_F0067B797121__INCLUDED_)
/************** End of GlobalMacro.h *******************************************/

//////////////////////下面为log4cplusWrapper的头文件包含///////////////////

#if (defined(LOG4CPLUS_ENABLE) && defined(__cplusplus))

#if !defined(AFX_LOG_MSG_CPP_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_)
#define AFX_LOG_MSG_CPP_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_

/** 日志记录
日志记录为一个字符串指针指向的内容
@return void
*/
LOGWRAPPER_API void log_msg_cpp(
    const char *strFile,				///文件名
    const int iLineNum,					///行号
    const log4cplus::Logger &loggerObj,	///Logger对象引用
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
);

/** 日志记录,不记录文件名和行号
日志记录为一个字符串指针指向的内容
@return void
*/
LOGWRAPPER_API void log_msg_no_file_num_cpp(
    const log4cplus::Logger &loggerObj,	///Logger对象引用
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
);

/** 日志记录,不记录文件名和行号,没有任何layout转换，直接输出相应的字符文本到日志中
此条记录没有行号，也没有线程号，也没有回车等
日志记录为一个字符串指针指向的内容
@return void
*/
LOGWRAPPER_API void log_msg_no_file_num_no_layout_cpp(
    const log4cplus::Logger &loggerObj,	///Logger对象引用
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
);

#endif // !defined(AFX_LOG_MSG_CPP_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_)

#endif /// (defined(LOG4CPLUS_ENABLE) && defined(__cplusplus))


#if (defined(LOG4CPLUS_ENABLE) && defined(__cplusplus))

/************** Begin of logWrapper.h *******************************************/
/*
* logWrapper.h
*
* 日志相关代码的头文件,CLogWrapper类的声明头文件
*/
#if !defined(AFX_LOGWRAPPER_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_)
#define AFX_LOGWRAPPER_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_


#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable: 4231 4251 4275 4786 )
#endif

///#include "config-win32.h"
///#include "log.h"
///#include "..\src\log.h"
#include <vector>
#include <wtypes.h>

/** 下面的宏定义参考自 "..\src\log4c\config-win32.h"
本来要#include "..\src\log4c\config-win32.h"
为避免包含该都文件，使用起来方便定义config-win32.h中下面的一系列宏定义
*/
#if defined(LOG4CPLUS_BUILD_DLL)
#undef LOGWRAPPER_DATA
#undef LOGWRAPPER_API
#undef LOGWRAPPER_CLASS_API

#define LOGWRAPPER_DATA			extern __declspec(dllexport)
#define LOGWRAPPER_API			__declspec(dllexport)
#define LOGWRAPPER_CLASS_API     __declspec(dllexport)

#elif defined(LOG4CPLUS_DLL)
#undef LOGWRAPPER_DATA
#undef LOGWRAPPER_API
#undef LOGWRAPPER_CLASS_API

#define LOGWRAPPER_DATA      	__declspec(dllimport)
#define LOGWRAPPER_API       	extern __declspec(dllimport)
#define LOGWRAPPER_CLASS_API 	__declspec(dllimport)
#else
#ifndef LOGWRAPPER_DATA
#define LOGWRAPPER_DATA
#endif

#ifndef LOGWRAPPER_API
#define LOGWRAPPER_API
#endif

#ifndef LOGWRAPPER_CLASS_API
#define LOGWRAPPER_CLASS_API
#endif
#endif

#include <strstream>

/**记录日志的宏定义
*/

/**本宏定义由于无法输出对象如PTIME和CORBA::Exception& e等到输出流中，所以该用下面的宏定义
#define PTRACE(level, message) \
LOG4CPLUS_LOG(CLogWrapper::m_logger, CLogWrapper::toLevel(level), message)
*/


///全局常量定义
///缺省log配置文件名
///const char g_strLogCfgFileDefault[] = "log4cplus_default.properties";

#ifndef LOG_CFG_FILENAME_DEFAULT
#define LOG_CFG_FILENAME_DEFAULT ("log4cplus_default.properties")
#endif
//////////////////////////////////////////////////////////////////////////

/*
* Currently, all MS C compilers for Win32 platforms default to 8 byte
* alignment.
*/
#pragma pack(push,_CRT_PACKING)


#ifndef TASK_ID
#ifdef _WIN32
#define TASK_ID HANDLE
#else
#define TASK_ID pthread_t
#endif
#endif

/** Class to trace Execution blocks.
This class is used for tracing the entry and exit of program blocks. Upon
construction it outputs an entry trace message and on destruction outputs an
exit trace message. This is normally only used from in the PTRACE_BLOCK macro.
*/
class LOGWRAPPER_CLASS_API CBlockBase
{
public:
    struct STBlockInfo
    {
        TASK_ID threadId;
        int iBlockLevel;
    };
    typedef STBlockInfo BlockInfo;

    ///存储各线程的Block层次的Vector
    typedef std::vector<BlockInfo> BlockInfoVector;

    /**BlockInfo类的比较函数对象，以成员变量threadId作为比较标准
    重载的()运算符接收两个BlockInfo类的对象,比较两对象大小，
    若第一个参数小余第二个参数则返回TRUE，否则返回FALSE。
    @return
    TRUE                         :第一个参数小余第二个参数
    FALSE						:第一个参数大于、等于第二个参数
    */
    class CBLockInfoComparedByThreadId
    {
    public:
        /**CContactManAscendingSortByTerState类的重载()运算符接收两个CSSModule::ContactMan的结构,比较两结构大小，
        若第一个参数等于第二个参数则返回TRUE，否则返回FALSE。
        @return
        TRUE							:第一个参数等于第二个参数
        FALSE							:第一个参数大于、等于第二个参数
        */
        bool operator()(
            const BlockInfo rpStart,
            const TASK_ID threadId) const
        {
            return (rpStart.threadId == threadId);
        }

        typedef BlockInfo first_argument_type;
        typedef bool result_type;
        typedef const TASK_ID second_argument_type;
    };

public:
    /** Output entry trace message. */
    CBlockBase(
        const char * fileName, /// Filename of source m_strFileName being traced
        int lineNum,           /// Line number of source m_strFileName being traced.
        const char * traceBeginName,/// String to be output with trace, typically it is the function m_strBlockName.
        const char * traceEndName,/// String to be output with trace, typically it is the function m_strBlockName.
        const int iLogLevel = LOG_LEVEL_TRACE, ///TRACE_LOG_LEVEL
        const bool bMultiThreadFlag = true
    );

    /// Output exit trace message.
    ~CBlockBase();

    /** @fn EnterBlock
    @brief 进入Block
    @return int 进入Block后Block的深度
    */
    int EnterBlock();

    /** @fn ExitBlock
    @brief 退出Block
    @return int 退出Block前的Block的深度
    */
    int ExitBlock();

    /** @fn GetBlockEnterBlock
    @brief 获取进入Block时Block要输出的字符串
    @return std::string.
    */
    std::string GetBlockEnterString(const int& iBlockLevel);

    /** @fn GetBlockExitString
    @brief 获取退出Block时Block要输出的字符串
    @return std::string.
    */
    std::string GetBlockExitString(const int& iBlockLevel);

    /** @fn Lock
    @brief 加锁
    @details 多线程访问mm_blockVector时需要加锁/解锁
    @retufn void.
    */
    void Lock();

    /** @fn Unlock
    @brief 解锁
    @details 多线程访问mm_blockVector时需要加锁/解锁
    @return voi.
    */
    void Unlock();

protected:
    ///检测threadId是的Block是否已经存在了
    BlockInfoVector::iterator FindThreadBlockInfo(const TASK_ID& threadId);
    BlockInfoVector::iterator FindThreadBlockInfo();
    BlockInfo GetThreadBlockInfo();

protected:
    const char * m_strFileName;
    int          m_iLineNum;
    const char * m_strBlockBeginName;
    const char * m_strBlockEndName;
    ///该Block的日志记录级别
    int			 m_iLogLevel;
    ///日志级别是否允许输出
    bool		 m_bIsEnabledForLog;
    ///是否支持多线程
    bool		 m_bMultiThreadEnabled;

    ///存储各线程的Block层次的Vector
    static BlockInfoVector m_blockVector;

    ///用于进行访问m_lockedInfoVector线程互斥用的
    ///static CSyncPoint_Thread m_sync;
};


/**
日志类
为了在不同的日志记录系统之间切换定义了本日志类
*/
///class LOGWRAPPER_CLASS_API CLogWrapper
class LOGWRAPPER_CLASS_API CLogWrapper
{
public:
    /** 实现Singleton模式的方法
    @return CLogWrapper*;
    */
    static CLogWrapper* Instance();

    /** 销毁单态对象
    @return void
    */
    static void DestroyInstance();

    /** @fn IsInstantiated
    @brief 判断是否实例化过了
    @return bool.
    @retval true 实例化过来。
    @retval false 没有实例化过。
    */
    static bool IsInstantiated();

    /** @fn GetDefaultLogger
    @breif 获取缺省的Logger
    @param [in] const bool &bReset = false 标志是否要重新重置Default Logger
    @return log4cplus::Logger
    */
    static const log4cplus::Logger &GetDefaultLogger(const bool &bReset = false);

    /** CLogWrapper扫尾函数
    @return bool,true:成功,false:失败。
    */
    bool Fini();

    /** CLogWrapper初始化函数
    这里传入一个日志配置文件名来进行初始化
    @return bool,true:成功,false:失败。
    */
    bool Init(const std::string& strConfigFile);

    /** 静态函数,判断当前日志系统是否OK
    是否可以正常记录日志,
    1.正常进行了初始化返回true
    2.没有进行初始化返回false
    3.进行了初始化但又调用了Fini函数将返回false
    @return bool,true:成功,false:失败。
    */
    static bool IsOK();

    /** CLogWrapper初始化函数
    这里传入一个字符串内容，根据这字符串内容来进行日志模块的初始化
    本函数的初始化过程如下：
    1.首先根据strFileContent内容生成一个日志配置文件。
    1.1.若同名配置文件存在则不作任何操作使用原来的配置文件。
    1.2.strFileName为生成的日志配置文件的名字，用户可以指定也可以采用缺省的文件名。
    2.然后调用Init函数来读取日志配置文件进行日志模块初始化工作。
    @return bool,true:成功,false:失败。
    */
    bool InitWithString(
        const std::string &strFileContent = "",
        const std::string &strFileName = LOG_CFG_FILENAME_DEFAULT
    );

    /** @fn PreProcessLogMsg
    @brief 记录消息日志前预处理
    @param const std::string &strMsg
    @return std::string 预处理后的消息
    */
    std::string PreProcessLogMsg(const std::string &strMsg);

public:
    /**@name Construction */
    //@{
    /** CLogWrapper类中定义的枚举常量。
    */

    ///用来存储logLevel数组的最大小标值，在init()函数中初始化
    static int m_iLogLevelMaxIndex;

    ///ostratream对象,PTRACE宏定义中用
    static std::ostrstream m_ostrs;

    /** 日志记录
    日志记录为一个字符串指针指向的内容
    @return void
    */
    void LogMsg(
        const char *strFile,
        const int &iLineNum,
        const int &iLogLevel,
        const char *strMsg,
        const log4cplus::Logger &loggerObj = GetDefaultLogger()
    );

    /** 日志记录,不带文件名和行号的
    日志记录为一个字符串指针指向的内容
    @return void
    */
    void LogMsg(
        const int &iLogLevel,
        const char *strMsg,
        const log4cplus::Logger &loggerObj = GetDefaultLogger()
    );

    /** 日志记录,不带文件名和行号的同时不附加回车换行，字符串原样输出到日志中去
    日志记录为一个字符串指针指向的内容
    @return void
    */
    void LogMsgNoFileNumAndNoLayout(
        const int &iLogLevel,
        const char *strMsg,
        const log4cplus::Logger &loggerObj = GetDefaultLogger()
    );

    /**获取宏定义中传入的参数形如(int , const char *, ...)的第一个iPriority的参数
    */
    const int GetPriorityWrapper(
        const int iLogLevel,		///日志记录级别
        const char* strFormat,	///日志内容格式
        ...						///日志内容
    );

    /**vsnprintf函数的wrapper函数
    本函数只使用第二个及以后的参数,使用vsnprintf函数将strFormat和后面的不定参数va_list va;以
    vsnprintf(strMsg, sizeof(strMsg), strFormat, va);
    形式格式化输出到一缓冲区中并以字符串的形式返回
    */
    const char *LogVsnprintfWrapper(
        const int iLogLevel,	///日志记录级别
        const char* strFormat,///日志内容格式
        ...					///日志内容
    );

    ///CLogWrapper类中的内部定义类

    /** Class to trace Execution blocks.
    This class is used for tracing the entry and exit of program blocks. Upon
    construction it outputs an entry trace message and on destruction outputs an
    exit trace message. This is normally only used from in the PTRACE_BLOCK macro.
    */
    class LOGWRAPPER_CLASS_API CBlock :public CBlockBase
    {
    public:
        /** Output entry trace message. */
        CBlock(
            const char * fileName, /// Filename of source m_strFileName being traced
            int lineNum,           /// Line number of source m_strFileName being traced.
            const char * traceBeginName,/// String to be output with trace, typically it is the function m_strBlockName.
            const char * traceEndName,/// String to be output with trace, typically it is the function m_strBlockName.
            const int iLogLevel = LOG_LEVEL_TRACE, ///TRACE_LOG_LEVEL
            const bool bMultiThreadEnabled = true
        );
        /// Output exit trace message.
        ~CBlock();
    };

    /** Class to trace Execution blocks.
    CLightBlock功能和CBlock类似，只是不采用加锁为线程不安全的，但增加了效率
    This class is used for tracing the entry and exit of program blocks. Upon
    construction it outputs an entry trace message and on destruction outputs an
    exit trace message. This is normally only used from in the PTRACE_BLOCK macro.
    */
    class LOGWRAPPER_CLASS_API CLightBlock :public CBlock
    {
    public:
        /** Output entry trace message. */
        CLightBlock(
            const char * fileName, /// Filename of source m_strFileName being traced
            int lineNum,           /// Line number of source m_strFileName being traced.
            const char * traceBeginName,/// String to be output with trace, typically it is the function m_strBlockName.
            const char * traceEndName,/// String to be output with trace, typically it is the function m_strBlockName.
            const int iLogLevel = TRACE_LOG_LEVEL
        );
        /// Output exit trace message.
        ~CLightBlock();
    };

    /**日志记录函数，
    记录日志
    为了提高记录日志的效率本函数为内联函数
    */
    /*目前无法使用由于message参数可能为 ("string1" << "string2")的形式而函数参数不支持这种形式
    inline static void log( const int& iLevel, ostrstream ostrs)
    {
    //ostrstream ostrs;
    ostrs << message << ends;
    LOG4CPLUS_LOG(CLogWrapper::m_logger, CLogWrapper::toLevel(iLevel), ostrs.str() )
    ostrs.rdbuf()->freeze(0);
    }
    */
private:
    /**私有缺省构造函数。
    本缺省构造函数不让使用
    */
    CLogWrapper()
    {

    };

    /**@name Access functions */
    //@{

    /**构造函数
    创建一个CLog对象。
    */
    CLogWrapper(const std::string& strConfigFile);
    //@}

    /**析构函数
    CLog对象销毁时调用。
    */
    ~CLogWrapper();

    static CLogWrapper* m_ptrInstance;
    //@}    
};

#pragma pack(pop)

///要导出的全局函数
LOGWRAPPER_API std::ostream& operator <<(std::ostream& stream, const WCHAR *ptrWCHAR);
LOGWRAPPER_API std::ostream& operator <<(std::ostream& stream, const std::string& strMsgAnsi);
LOGWRAPPER_API std::ostream& operator <<(std::ostream& stream, const std::wstring& strMsgUnicode);

/**
将大小为count的缓冲区中内容按字节以16进制字符串打印出来,
返回值即为指向相应的字符串，
该返回指向的存储区域要调用本函数的用户显示的进行删除
@return char *.
*/
extern char * sprintf_data(char *buff, int count);

/**将大小为count的缓冲区中内容按字节以16进制字符串打印出来,
str缓冲区的大小应大于等于count+1,因为还要一个字节存放字符串结束符
@return void.
*/
extern void sprintf_data(char *str, char *buff, int count);

#if defined(_MSC_VER)
#pragma warning (pop)
#endif

#endif // !defined(AFX_LOGWRAPPER_H__4735DC75_04BF_445A_9FB0_4146D6333F85__INCLUDED_)
/*************************** End of logWrapper.h ******************************/


/************** Begin of RefHolderBase.h ********************************************/
#pragma once
#pragma pack(push, _CRT_PACKING)

///类型前置声明
//namespace NSLog
//{
//	class CRefHolderBase;
//}

/** @fn operator <<
@brief CRefHolderBase类型的std::ostream 流输出操作符
@param std::ostream& stream
@param const CRefHolderBase& refHolderBaseObj
@return std::ostream&.
*/
///extern std::ostream& operator <<(std::ostream& stream, const NSLog::CRefHolderBase& refHolderBaseObj);

namespace NSLog
{
    class LOGWRAPPER_CLASS_API CRefHolderBase
    {
    public:
        typedef std::vector<CRefHolderBase *> TCRefHolderBasePtrVector;
    public:
        virtual ~CRefHolderBase();

        virtual std::ostream& Output2Stream(std::ostream& stream) const;

        ///virtual std::ostream& operator <<(std::ostream& stream) const;

        template<class CDataType>
        CRefHolderBase& operator <<(const CDataType& data)
        {
            CRefHolderBase *ptrRefHolderBase = new CRefHolder<CDataType>(data);
            m_vecPtrRefHolderBase.push_back(ptrRefHolderBase);
            return *this;
        }
    private:
        TCRefHolderBasePtrVector m_vecPtrRefHolderBase;
    };

    /** @class CRefHolder
    @brief 保存特定类型引用值的模板类
    */
    template<class CDataType>
    class LOGWRAPPER_CLASS_API CRefHolder :public CRefHolderBase
    {
    public:
        CRefHolder(const CDataType& data)
            :m_refData(data)
        {
        }

        virtual std::ostream& Output2Stream(std::ostream& stream) const
        {
            stream << m_refData;
            ///stream ::operator<< m_refData;
            ///::operator<< (stream , m_refData);
            return stream;
        }

        //virtual std::ostream& operator <<(std::ostream& stream) const
        //{
        //	stream << m_refData;
        //	return stream;
        //}
    protected:
        const CDataType& m_refData;
    };


    /** Class to trace Execution blocks.
    This class is used for tracing the entry and exit of program blocks. Upon
    construction it outputs an entry trace message and on destruction outputs an
    exit trace message. This is normally only used from in the PTRACE_BLOCK macro.
    */
    class LOGWRAPPER_CLASS_API CBlockWithRef :public CBlockBase
    {
    public:
        /** Output entry trace message. */
        CBlockWithRef(
            const char * fileName, /// Filename of source m_strFileName being traced
            int lineNum,           /// Line number of source m_strFileName being traced.
            const CRefHolderBase& refHolderBaseBegin,/// CRefHolderBase to be output with trace, typically it is the function m_refHolderBase.
            const CRefHolderBase& refHolderBaseEnd,/// CRefHolderBase to be output with trace, typically it is the function m_refHolderBase.
            const int iLogLevel = LOG_LEVEL_TRACE, ///TRACE_LOG_LEVEL
            const bool bMultiThreadEnabled = true
        );

        /// Output exit trace message.
        ~CBlockWithRef();

        /** @fn GetBlockEnterBlock
        @brief 获取进入Block时Block要输出的字符串
        @return std::string.
        */
        std::string GetBlockEnterString(const int& iBlockLevel);

        /** @fn GetBlockExitString
        @brief 获取退出Block时Block要输出的字符串
        @return std::string.
        */
        std::string GetBlockExitString(const int& iBlockLevel);
    private:
        const CRefHolderBase& m_refHolderBaseBegin;
        const CRefHolderBase& m_refHolderBaseEnd;
    };

    /** Class to trace Execution blocks.
    CLightBlockWithRef功能和CBlockWithRef类似，只是不采用加锁为线程不安全的，但增加了效率
    This class is used for tracing the entry and exit of program blocks. Upon
    construction it outputs an entry trace message and on destruction outputs an
    exit trace message. This is normally only used from in the PTRACE_BLOCK macro.
    */
    class LOGWRAPPER_CLASS_API CLightBlockWithRef :public CBlockWithRef
    {
    public:
        /** Output entry trace message. */
        CLightBlockWithRef(
            const char * fileName, /// Filename of source m_strFileName being traced
            int lineNum,           /// Line number of source m_strFileName being traced.
            const CRefHolderBase& refHolderBaseBegin,/// CRefHolderBase to be output with trace, typically it is the function m_refHolderBase.
            const CRefHolderBase& refHolderBaseEnd,/// CRefHolderBase to be output with trace, typically it is the function m_refHolderBase.
            const int iLogLevel = TRACE_LOG_LEVEL
        );
        /// Output exit trace message.
        ~CLightBlockWithRef();
    };
}

#pragma pack(pop)
/************** end of RefHolderBase.h ************************************/

#endif // (defined(LOG4CPLUS_ENABLE) && defined(__cplusplus))
//////////////////////////////////////////////////////////////////////////

//////下面为log4cplusWrapper的实现代码（包含c和c++的实现代码）///////////////

#if ( defined(LOG4CPLUS_ENABLE) && defined(__cplusplus) ) && defined(IMPLEMENT_LOG4CPLUS)

/************** Begin of voscfg.h *******************************************/
/**********************************************************
* 文件名称： voscfg.h
* 文件标识：
* 内容摘要： VOS的系统配置文件
**********************************************************/

#ifndef _VOSCFG_H
#define _VOSCFG_H

#ifdef __cplusplus
extern "C" {
#endif

    /*******************************************************
    *                       基本配置
    *******************************************************/

    /* CPU类型 */
#define   CPU_X86             0x1
#define   CPU_PPC             0x2
#define   CPU_ARM             0x3
#define   CPU_TYPE            CPU_X86

#undef	OS_LINUX	
#undef  OS_WINNT
/* 操作系统OS类型 */
#define   OS_VXWORKS          0x1
#define   OS_WINNT            0x2
#define   OS_PSOS             0x3
#define   OS_LINUX            0x4
#define   OS_KLINUX           0x5

///added by wenhm in 2005.08.25
#ifdef _WIN32
#define   OS_TYPE             OS_WINNT
#else #ifdef   linux
#define   OS_TYPE			  OS_LINUX
#endif


/* linux适配 */
#define    VXWORKS_TO_LINUX
#ifndef    VXWORKS_TO_LINUX
#define    PSOS_TO_LINUX
#endif
#if ( OS_TYPE == OS_KLINUX)
#define  _USE_TIMER_IRQ
#endif

/*********************************************************
基本包含文件
**********************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#if OS_TYPE == OS_PSOS
#include <psos.h>
#include <phile.h>
#include <pna.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys_conf.h>
#include <types.h>
#include <socket.h>
#endif

#if  OS_TYPE == OS_VXWORKS
#include <vxWorks.h>
#include <sysLib.h>
#include <taskLib.h>    
#include <msgQLib.h>
#include <semLib.h>
#include <vmLib.h>
#include <dosfslib.h>
#include <iolib.h>
#include <dirent.h>
#include <stat.h>
#include <errnoLib.h>
#include <types.h>
#include <socket.h>
#include <socklib.h>
#include <logLib.h>
#include <assert.h>
#include <symlib.h>
#include <sysSymTbl.h>
#include <excLib.h>
#include <esf.h>
#include <memLib.h>
#include <tickLib.h>
#include <private/memPartLibP.h>
#include <in.h>
#include <ramDrv.h>
#include <ftpdLib.h>
#include <cachelib.h>
#include <signal.h>
#include <sigLib.h>
#include <time.h>
#include <wdLib.h>
#include <intLib.h>
#include <timers.h>
#include "inetLib.h"
#include "netinet/tcp.h"
#include "fcntl.h"
#include "netinet/in.h"
#include "end.h"
#include "zbufSockLib.h"
#include <routeLib.h>
#include <usrLib.h>
#endif

#if OS_TYPE == OS_WINNT
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <imagehlp.h>
#include <io.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <direct.h>
///#include <Winsock.h>
#include <time.h>
#include <Winsock2.h>
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "Winmm.lib")
#endif

#if OS_TYPE == OS_LINUX
#ifdef VXWORKS_TO_LINUX
    ///#include "vxw_hdrs.h"
#else
    ///#include "psos_hdrs.h"
#endif
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <asm/ioctls.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <asm/errno.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <netinet/in.h>    
#include <signal.h>
#include <stdio.h>
#include <dirent.h>
#include <semaphore.h>    
#endif

#if OS_TYPE == OS_KLINUX
#include "kth.h"
    /*	#include "ksocket.h"*/
#include <stdio.h>
#include "ktm.h"
#include <netinet/tcp.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
/*	#include "kfile.h"*/
/*	#include <linux/vmalloc.h>*/
#endif

/*********************************************************
基本数据类型
**********************************************************/
#if OS_TYPE != OS_WINNT
    typedef  unsigned char    BYTE;
#endif

    typedef  unsigned char    BOOLEAN;
    typedef  char             CHAR;
    typedef  void            *LPVOID;
    typedef  unsigned short   WORD16;
    typedef  unsigned long    WORD32;
    typedef  signed short     SWORD16;
    ///typedef  signed long      SWORD32;

#ifndef SWORD32
#ifdef  WIN32
    typedef long                SWORD32;
#else
#define SWORD32            long
#endif		
#endif

#define  INVALID_BYTE     (unsigned char)0xff
#define  INVALID_WORD     (unsigned short)0xffff
#define  INVALID_DWORD    (unsigned long)0xffffffff 

    typedef  long             OSS_STATUS;
#define  OSS_OK           0
#define  OSS_ERROR        -1

#ifdef __cplusplus
}
#endif

#endif

/************** End of voscfg.h *******************************************/

/************** Begin of vos.h *******************************************/
/**********************************************************
* 文件名称： vos.h
* 文件标识：
* 内容摘要： 操作系统封装层头文件（主要是所包含头文件、数据结构、接口等）
* 其它说明： 无
* 当前版本： V1.0
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
**********************************************************/
#ifndef _VOS_H_
#define _VOS_H_

#ifdef __cplusplus
extern "C" {
#endif

    ///#include "voscfg.h"
    /**********************************************************
    *                          宏定义                         *
    **********************************************************/

#define    VOS_NULL                      0x0

#define    VOS_TRUE                      1
#define    VOS_FALSE                     0

#define    VOS_SUCCESS                   0     /* PSOS返回值                    */
#define    VOS_PCBINDEXNO                0     /* 保存pcbindex的便条寄存器号    */
#define    VOS_TASKPRINO                 1     /* 保存任务优先级的便条寄存器号  */
#define    VOS_TICKNUM                   1     /* 中断的tick数                  */
#define    TIMER_SCAN_EVENT              (WORD32)(1<<15)
#define    MEMLEAK_SCAN_EVENT            (WORD32)(1<<16)
#define    MAX_LENGTH_BUF                512

#define    VOS_NO_WAIT                   0x0
#define    VOS_WAIT_FOREVER              -1

#define    VOS_SEM_DELETE_SAFE           0x04
#define    VOS_SEM_INVERSION_SAFE        0x08

#define    VOS_VM_STATE_MASK_VALID       0x03
#define    VOS_VM_STATE_MASK_WRITABLE    0x0c
#define    VOS_VM_STATE_MASK_CACHEABLE   0x30
#define    VOS_VM_STATE_MASK_WBACK       0x40
#define    VOS_VM_STATE_MASK_GLOBAL      0x80
#define    VOS_VM_STATE_MASK_GUARDED     0x80

#define    VOS_VM_STATE_VALID            0x01
#define    VOS_VM_STATE_VALID_NOT        0x00
#define    VOS_VM_STATE_WRITABLE         0x04
#define    VOS_VM_STATE_WRITABLE_NOT     0x00
#define    VOS_VM_STATE_CACHEABLE        0x10
#define    VOS_VM_STATE_CACHEABLE_NOT    0x00
#define    VOS_VM_STATE_WBACK			 0x40
#define    VOS_VM_STATE_WBACK_NOT		 0x00
#define    VOS_VM_STATE_GLOBAL			 0x80
#define    VOS_VM_STATE_GLOBAL_NOT		 0x00
#define    VOS_VM_STATE_GUARDED			 0x80
#define    VOS_VM_STATE_GUARDED_NOT		 0x00

#define    MAXCOUNTSEM                   10000
#define    VOS_MAX_PATH_LEN              50
#define    VOS_MAX_FILENAME_LEN          60
#define    VOS_MAX_EXP_FILENAME_LEN      8

#if OS_TYPE == OS_VXWORKS
#define    NULL_TASKID               ERROR
#define    NULL_SEMID                NULL
#define    NULL_MSGQID               NULL
#define    NULL_MSGGET               0
#define    DIR_CODE                  0x2f    /* 定义符号 "/" */
#define    DIR_CONCATE_CODE            "/"
#endif

#if OS_TYPE == OS_WINNT
#define    NULL_TASKID               NULL
#define    NULL_SEMID                NULL
#define    NULL_MSGQID               NULL
#define    NULL_MSGGET               0
#define    DIR_CODE                  0x5c    /* 定义符号" \" */
#define    DIR_CONCATE_CODE            "\\"
#define    RDONLY                    0
#define    WRONLY                    1
#define    RDWR                      2
#define    WINNT_ERROR               -1    
#endif

#if OS_TYPE == OS_PSOS
#define    NULL_TASKID               0xffffffff
#define    NULL_SEMID                0xffffffff
#define    NULL_MSGQID               0xffffffff
#define    NULL_MSGGET               0xffffffff
#define    MSG_Q_FIFO                0x00
#define    MSG_Q_PRIORITY            0x01
#define    MSG_PRI_NORMAL            0x00
#define    MSG_PRI_URGENT            0x01
#define    RAM_BLOCKS                2*360  /* RAM disk块大小 */
#define    RAM_PATH_LENGTH           80
#define    MIN_EXPAN_BLOCKS          10
#endif

#if OS_TYPE == OS_LINUX
#ifdef VXWORKS_TO_LINUX
#define    NULL_TASKID               ERROR
#define    NULL_SEMID                NULL
#define    NULL_MSGQID               NULL
#else
#define    NULL_TASKID               0xffffffff
#define    NULL_SEMID                0xffffffff
#define    NULL_MSGQID               0xffffffff
#endif
#define    NULL_MSGGET               0
#define    DIR_CODE                  0x2f  /* 定义符号 "/" */

#endif

#if OS_TYPE == OS_KLINUX
#define    NULL_TASKID               ERROR
#define    NULL_SEMID                 NULL
#define    NULL_MSGQID              NULL
#define    TRUE   1
#define    FALSE 0
#define    DIR_CONCATE_CODE        "/"
#define    DIR_CODE                0x2f
#define    VOS_ISDIR(mode)    ((mode & S_IFMT) == S_IFDIR)    /* 判断是否是目录 */
    // #define    malloc(size) __vmalloc(size,GFP_KERNEL |__GFP_HIGHMEM, PAGE_KERNEL)
    //#define    free(buf)      vfree(buf)
#define    printf  printk
#endif


    /************** SOCKET & TCP ***************/
#define NELEMENTS(array)  (sizeof (array) / sizeof ((array) [0]))

#ifndef HASMAX
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#endif

#ifndef HASMIN
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#endif 


#if (OS_TYPE == OS_VXWORKS)
#define SOCKET_ERROR     ERROR
#define SOMAXCONN        5
#define VOS_EWOULDBLOCK  EWOULDBLOCK
#define VOS_ECONNRESET   ECONNRESET
#endif

#if (OS_TYPE == OS_WINNT)
    /* 数组中元素个数 */
    ///(pclint检测注释掉该行)#define NELEMENTS(array)  (sizeof (array) / sizeof ((array) [0]))
#define CloseSocket closesocket
#define bcopy(src,dst,size) memcpy(dst,src,size)
#define errnoGet         WSAGetLastError
    typedef long int STATUS;
#define VOS_EWOULDBLOCK WSAEWOULDBLOCK
#define VOS_ECONNRESET WSAECONNRESET
#endif


#if ((OS_TYPE == OS_LINUX) ||(OS_TYPE == OS_KLINUX))
    typedef unsigned long ULONG;
#define errnoGet() errno
#define SOCKET_ERROR     ERROR
#define SOMAXCONN	128
#define VOS_EWOULDBLOCK  EWOULDBLOCK
#define VOS_ECONNRESET ECONNRESET

#endif

#if ((OS_TYPE == OS_VXWORKS) || (OS_TYPE == OS_LINUX)||(OS_TYPE == OS_KLINUX))
    typedef int SOCKET;
#endif

    /**************** VOS TIMER *******************/

    /* 时钟中断精度10ms */
#define  TIMER_RESOLUTION    10 


#if (OS_TYPE == OS_WINNT)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <mmsystem.h>
#define  VOS_TIMERCALLBACK     LPTIMECALLBACK  
#define  VOS_TIMER_ID          WORD32 
#define  INVALID_VOS_TIMERID   0
#endif

#if (OS_TYPE == OS_VXWORKS)
#define  VOS_TIMERCALLBACK     FUNCPTR  
#define  VOS_TIMER_ID          WDOG_ID 
#define  INVALID_VOS_TIMERID   NULL

#define  TIMER_ONESHOT         0
#define  TIMER_PERIODIC        1
#endif

#if (OS_TYPE == OS_PSOS)
#define  VOS_TIMERCALLBACK     LPVOID  
#define  VOS_TIMER_ID          WORD32 
#define  INVALID_VOS_TIMERID   0xffffffff
#endif

#if (OS_TYPE == OS_LINUX)
    typedef  void(*VOS_TIMERCALLBACK)(int);
#define  VOS_TIMER_ID          WORD32 
#define  INVALID_VOS_TIMERID   0
#endif

#if (OS_TYPE == OS_KLINUX)
#ifndef HZ
#define HZ 100
#endif     
    extern unsigned long volatile jiffies;

    typedef  void(*VOS_TIMERCALLBACK)(WORD32);
    typedef  struct timer_list    *VOS_TIMER_ID;
#define  INVALID_VOS_TIMERID   NULL    
#endif

    /**************** VOS VOLUME *******************/
#define        VOLNAME_LENGTH            10


/**********************************************************
*                          类型定义                       *
**********************************************************/

#if    OS_TYPE == OS_VXWORKS
#define    VOS_ISDIR(mode)    ((mode & S_IFMT) == S_IFDIR)    /* 判断是否是目录 */
#define    VOS_ISCHR(mode)    ((mode & S_IFMT) == S_IFCHR)    /* 是否 是特殊字符 */
#define    VOS_ISBLK(mode)    ((mode & S_IFMT) == S_IFBLK)    /* 判断是否是特殊块 */
#define    VOS_ISREG(mode)    ((mode & S_IFMT) == S_IFREG)    /* 判断是否正规块 */
#define    VOS_ISFIFO(mode)   ((mode & S_IFMT) == S_IFIFO)    /* 判断是否fifo  */
#elif OS_TYPE== OS_WINNT
#define    VOS_ISDIR(mode)    ((mode & _S_IFMT) == _S_IFDIR)    /* 判断是否是目录 */
#define    VOS_ISCHR(mode)    ((mode & _S_IFMT) == _S_IFCHR)    /* 是否 是特殊字符 */
#define    VOS_ISBLK(mode)    ((mode & _S_IFMT) == _S_IFBLK)    /* 判断是否是特殊块 */
#define    VOS_ISREG(mode)    ((mode & _S_IFMT) == _S_IFREG)      /* 判断是否正规块 */
#define    VOS_ISFIFO(mode)   ((mode & _S_IFMT) == _S_IFIFO)    /* 判断是否fifo  */
#endif


/* ID类型定义 */
#if OS_TYPE == OS_VXWORKS
    typedef SWORD32  TASK_ID;
    typedef PART_ID  MEMPART_ID;
    typedef FUNCPTR  VOS_FUNCPTR;
    typedef WIND_TCB TASK_TCB;
#endif

#if OS_TYPE == OS_PSOS
    typedef WORD32  TASK_ID;
    typedef VOS_VOIDFUNCPTR  VOS_FUNCPTR;
    typedef WORD32  SEM_ID;
    typedef WORD32  MSG_Q_ID;
    typedef WORD32  MEMPART_ID;
#endif

#if OS_TYPE == OS_WINNT
    typedef HANDLE   TASK_ID;
    typedef HANDLE   SEM_ID;
    typedef HANDLE   MEMPART_ID;
    typedef HANDLE   MSG_Q_ID;
    typedef LPTHREAD_START_ROUTINE VOS_FUNCPTR;
#endif

#if OS_TYPE == OS_LINUX
    typedef void        VOID;
    typedef pthread_t	TASK_ID;
    typedef sem_t *		SEM_ID;
#ifndef PART_ID
#define PART_ID int
#endif /*wenhm,050826*/
    typedef PART_ID     MEMPART_ID;
    typedef void*       VOS_FUNCPTR;
    ///typedef void (*AutoRunHandler)(void);
    typedef WORD32      MSG_RECEIVE;/*yangyp*/
    typedef WORD32      MSG_Q_ID;

    ///typedef FUNCPTR     VOS_FUNCPTR; 
    ///typedef WIND_TCB    TASK_TCB;
    ///typedef WORD16      VOS_TIMER_ID;/*yangyp*/
    ///typedef WORD32		SEM_ID;
    ///typedef SWORD32     TASK_ID;
#endif
#if OS_TYPE == OS_KLINUX
    typedef void        VOID;
    typedef pthread_t	TASK_ID;
    typedef sem_t*		SEM_ID;
    /*typedef PART_ID     MEMPART_ID;
    typedef FUNCPTR     VOS_FUNCPTR;
    typedef WIND_TCB    TASK_TCB;*/
    typedef WORD32      MEMPART_ID;
    typedef void*       VOS_FUNCPTR;
    typedef WORD32      MSG_RECEIVE;/*yangyp*/
    typedef WORD16      VOS_TIMER_ID;/*yangyp*/
    typedef WORD32      MSG_Q_ID;
#endif


    /* VOS_STATUS: VOS层函数返回值 */
    typedef enum
    {
        VOS_ERROR = -1,
        VOS_OK = 0,
    }VOS_STATUS;

    /* VOS_SEM_STATE  二进制信号量初始状态 */
    typedef enum
    {
        VOS_SEM_EMPTY,
        VOS_SEM_FULL
    }VOS_SEM_STATE;

    /* 信号量队列类型:先进先出或优先级 */
    typedef enum
    {
        VOS_SEM_Q_FIFO,
        VOS_SEM_Q_PRIORITY
    }VOS_SEM_QUEUE_TYPE;

    /* 消息优先级 */
    typedef enum
    {
        VOS_MSG_PRI_NORMAL,
        VOS_MSG_PRI_URGENT
    }VOS_MSG_TYPE;

    /* 消息排队类型:先进先出或优先级 */
    typedef enum
    {
        VOS_MSG_Q_FIFO,
        VOS_MSG_Q_PRIORITY
    }VOS_MSGQUE_TYPE;

    /* 任务状态 */
    typedef enum
    {
        VOS_TASK_STATUS_NULL,
        VOS_TASK_STATUS_RUNNING,
        VOS_TASK_STATUS_READY,
        VOS_TASK_STATUS_PEND,
        VOS_TASK_STATUS_SUSPEND,
        VOS_TASK_STATUS_DEBUG,
        VOS_TASK_STATUS_DEAD
    }VOS_TASK_STATUS;

    /* 参数和返回值为VOID的函数定义 */
    typedef   VOID(*VOS_VOIDFUNCPTR)(VOID);

    /**********************************************************
    *                          数据结构定义                   *
    **********************************************************/

    /*
     * Currently, all MS C compilers for Win32 platforms default to 8 byte
     * alignment.
     */
#pragma pack(push,_CRT_PACKING)

     /* 系统时钟 */
    typedef struct tagSYS_CLOCK
    {
        WORD16             wSysYear;
        WORD16             wSysMon;
        WORD16             wSysDay;
        WORD16             wSysHour;
        WORD16             wSysMin;
        WORD16             wSysSec;
        WORD16             wSysWeek;
        WORD16             wMilliSec;
    }T_SYS_CLOCK;

#undef st_atime
#undef st_mtime
#undef st_ctime
    /* 文件状态描述 */
    typedef struct tagFileStat
    {
        WORD32  st_dev;
        WORD32  st_ino;
        WORD16  st_mode;
        SWORD16 st_nlink;
        SWORD16 st_uid;
        SWORD16 st_gid;
        WORD32  st_rdev;
        SWORD32 st_size;
        SWORD32 st_atime;
        SWORD32 st_mtime;
        SWORD32 st_ctime;
    }T_FileStat;


    /*文件的访问属性*/
    enum Permissions {
        /// File has world execute permission
        WorldExecute = 1,
        /// File has world write permission
        WorldWrite = 2,
        /// File has world read permission
        WorldRead = 4,
        /// File has group execute permission
        GroupExecute = 8,
        /// File has group write permission
        GroupWrite = 16,
        /// File has group read permission
        GroupRead = 32,
        /// File has owner execute permission
        UserExecute = 64,
        /// File has owner write permission
        UserWrite = 128,
        /// File has owner read permission
        UserRead = 256,
        /// All possible permissions.
        AllPermissions = 0x1ff,
        /// Owner read & write plus group and world read permissions.
        DefaultPerms = UserRead | UserWrite | GroupRead | WorldRead,
        /// Owner read & write & execute plus group and world read & exectute permissions.
        DefaultDirPerms = DefaultPerms | UserExecute | GroupExecute | WorldExecute
    };

#if OS_TYPE == OS_WINNT
    /**
    下面结构当和MINIAce一起使用时,
    os_dirent.h中相应的结构产生的冲突,而os_dirent.h结构中采用了ACE_LACKS_STRUCT_DIR才会生效,
    因此可以设定vos.h中不采用ACE_LACKS_STRUCT_DIR宏定义时它的结构dirent才生效.
    added by wenhm in 2007.05.16
    */
#ifndef ACE_LACKS_STRUCT_DIR
    /* 路径入口 */
    struct  dirent
    {
        unsigned short d_ino;
        unsigned short d_off;
        unsigned short d_reclen;
        CHAR     d_name[VOS_MAX_FILENAME_LEN + 1];
    };
#endif

    /* 目录项的定义 */
    typedef struct
    {
        SWORD32       dd_fd;        /* 打开目录的文件描述 */
        SWORD32       dd_cookie;    /* 目录中的文件特定标示filesys-specific marker within dir */
        struct dirent dd_dirent;    /* 路径入口 */
    }DIR;
#endif

#pragma pack(pop)

    /**********************************************************
    *                          函数原型                       *
    **********************************************************/

    /******************  任务管理模块 ************************/
    TASK_ID       VOS_CreateTask(CHAR *pTaskName, SWORD32 priority,
        SWORD32 options, SWORD32 stackSize,
        VOS_FUNCPTR pTaskEntry, SWORD32 arg1,
        SWORD32 arg2, SWORD32 arg3, SWORD32 arg4,
        SWORD32 arg5, SWORD32 arg6, SWORD32 arg7,
        SWORD32 arg8, SWORD32 arg9, SWORD32 arg10);

    VOS_STATUS    VOS_DeleteTask(TASK_ID taskId);
    VOS_STATUS    VOS_SuspendTask(TASK_ID taskId);
    VOS_STATUS    VOS_ResumeTask(TASK_ID  taskId);
    VOS_STATUS    VOS_SetPriority(TASK_ID TaskId, SWORD32 newPriority);
    SWORD32       VOS_GetPriority(TASK_ID TaskId);

    VOS_STATUS    VOS_DelayTask(SWORD32 delayTime);
    TASK_ID       VOS_GetSelfTaskID(VOID);
    VOS_STATUS    VOS_SuspendSelf(VOID);

    VOS_TASK_STATUS VOS_GetTaskStatus(TASK_ID taskId);
    VOID          VOS_CheckStack(TASK_ID taskId);


    /************** 二进制信号量与计数信号量 *****************/
    SEM_ID        VOS_CreateBSem(SWORD32 options, VOS_SEM_STATE initialState);
    SEM_ID        VOS_CreateCSem(SWORD32 options, SWORD32 initialState);
    SEM_ID        VOS_CreateMSem(SWORD32 options);
    VOS_STATUS    VOS_DeleteSem(SEM_ID semId);
    VOS_STATUS    VOS_TakeSem(SEM_ID semId, SWORD32 timeout);
    VOS_STATUS    VOS_GiveSem(SEM_ID semId);

    /** @fn VOS_CreateThreadMutex
    @brief 创建线程互斥用的临界区
    @details 内部采用CRITICAL_SECTION来实现的
        1.CRITICAL_SECTION用于线程间互斥，效率比较高.
        2.返回时SEM_ID保存CRITICAL_SECTION的指针
    @return SEM_ID.
    @retval 成功，返回信号量标识符
    @retval NULL, 失败，返回NULL
    */
    SEM_ID VOS_CreateThreadMutex();

    /** @fn VOS_TakeThreadMutex
    @brief 获得线程临界区信号量
    @details 内部采用CRITICAL_SECTION来实现的
        1.CRITICAL_SECTION用于线程间互斥，效率比较高.
        2.SEM_ID保存CRITICAL_SECTION的指针
    @param [in] SEM_ID semId 信号量标识符
    @param [in] SWORD32 timeout 超时毫秒数
    @return VOS_STATUS.
    @retval VOS_OK.
    @retval VOS_ERROR, 失败.
    */
    VOS_STATUS VOS_TakeThreadMutex(SEM_ID semId, SWORD32 timeout);

    /** @fn VOS_GiveThreadMutex
    @brief 释放线程临界区信号量
    @details 内部采用CRITICAL_SECTION来实现的
        1.CRITICAL_SECTION用于线程间互斥，效率比较高.
        2.SEM_ID保存CRITICAL_SECTION的指针
    @param [in] SEM_ID semId 信号量标识符
    @return VOS_STATUS.
    @retval VOS_OK.
    @retval VOS_ERROR, 失败.
    */
    VOS_STATUS VOS_GiveThreadMutex(SEM_ID semId);

    /** @fn VOS_DeleteThreadMutex
    @brief 删除线程临界区信号量
    @details 内部采用CRITICAL_SECTION来实现的
        1.CRITICAL_SECTION用于线程间互斥，效率比较高.
        2.SEM_ID保存CRITICAL_SECTION的指针
    @param [in] SEM_ID semId 信号量标识符
    @return VOS_STATUS.
    @retval VOS_OK.
    @retval VOS_ERROR, 失败.
    */
    VOS_STATUS VOS_DeleteThreadMutex(SEM_ID semId);

    /**************** 内存管理模块 ***************************/
    MEMPART_ID    VOS_CreateMemPart(CHAR *pPool, WORD32 poolSize);
    VOID         *VOS_AllocMemPart(MEMPART_ID memPartId, WORD32 size);
    VOS_STATUS    VOS_FreeMemPart(MEMPART_ID memPartId, CHAR *pBlock);

    CHAR         *VOS_GetPhysMemTop(VOID);
    WORD32        VOS_GetRnFreeInfo(WORD32 dwRnId);

    /********************* 中断处理 ******* ******************/
    SWORD32       VOS_LockInt();
    VOID          VOS_UnlockInt(SWORD32 dwLevel);


    /********************* 消息队列 **************************/
    MSG_Q_ID      VOS_CreateMsgQue(WORD32 dwMaxMsgNum,
        WORD32 dwMaxMsgLength,
        BYTE ucOptions);
    VOS_STATUS    VOS_DeleteMsgQue(MSG_Q_ID wMsgId);
    VOS_STATUS    VOS_SendMsg(MSG_Q_ID wMsgId,
        CHAR *pcMsgData,
        WORD32 dwlen,
        SWORD32 dwtimeout,
        BYTE ucPrior);
    WORD32        VOS_ReceiveMsg(MSG_Q_ID wMsgId,
        CHAR  *pcMsgDataBuffer,
        WORD32 dwlen,
        SWORD32 iTimeout);
    WORD32        VOS_GetCurMsgs(MSG_Q_ID wMsgId);


    /********************* 时钟模块 **************************/
    VOS_STATUS    VOS_ConnectClk(VOS_VOIDFUNCPTR clkIntEntry);
    SWORD32       VOS_GetSysClkRate(VOID);
    WORD32        VOS_GetTick();
    VOS_STATUS    VOS_IniSysClk(T_SYS_CLOCK *pSysSoftClk);
    VOS_STATUS    VOS_SetSysClkRate(SWORD32 sdTicPerSend);

    VOS_STATUS    VOS_TimeSet(T_SYS_CLOCK *pSysSoftClk);
    VOS_STATUS    VOS_TimeGet(T_SYS_CLOCK *pSysSoftClk);

    VOS_TIMER_ID VOS_RelTimer(
        WORD32 dwTimeLength,
        VOS_TIMERCALLBACK lpTimeProc,
        WORD32 dwUser,
        WORD32 dwEvents
    );

    VOS_TIMER_ID VOS_AbsTimer(
        T_SYS_CLOCK tRelTimerSet,
        VOS_TIMERCALLBACK lpTimeProc,
        WORD32 dwUser,
        WORD32 dwEvents
    );

    VOS_TIMER_ID VOS_PerTimer(
        WORD32 dwTimeLength,
        VOS_TIMERCALLBACK lpTimeProc,
        WORD32 dwUser,
        WORD32 dwEvents
    );

    VOS_STATUS   VOS_TimerKill(VOS_TIMER_ID TimerId);


    /********************* 错误异常 **************************/
    SWORD32       VOS_GetErrno(VOID);
    VOID          VOS_AddExcHook(VOS_FUNCPTR excHookEntry);

    /********************* 初始化VOS *************************/
    VOS_STATUS    VOS_InitVOS(VOID);


    /****************** 当前进程PCB操作 **********************/
    VOS_STATUS    VOS_SetCurPCBIndex(WORD16  wIndex);
    WORD16        VOS_GetCurPCBIndex(VOID);
    VOS_STATUS    VOS_FreeTlsData(VOID);

    /******************      事件模块   **********************/
    VOS_STATUS    VOS_SendEvent(TASK_ID taskId, WORD32 dwEvents);
    VOS_STATUS    VOS_ReceiveEvent(WORD32 dwEvents, WORD32 dwTimeout,
        WORD32 *ptEventsR);


    /******************  调试打印函数  **********************/
    VOID          VOS_Display(const CHAR *fmt, ...);


    /********************文件系统模块**********************/
    SWORD32       VOS_Create(CHAR *pcFileName);
    VOS_STATUS    VOS_Delete(CHAR *pcFileName);
    SWORD32       VOS_Open(CHAR *pcFileName, SWORD32 sdwFlag, SWORD32 sdwMode);
    VOS_STATUS    VOS_Close(SWORD32 sdwFileHandle);
    VOS_STATUS    VOS_Rename(CHAR *pcOldFileName, CHAR *pcNewFileName);
    SWORD32       VOS_Read(SWORD32 sdFileHandle, VOID *pBuffer, SWORD32 sdCount);
    SWORD32       VOS_Write(SWORD32 sdFileHandle, VOID *pBuffer, SWORD32 sdCount);
    SWORD32       VOS_FileLength(CHAR *pcFileName);
    SWORD32       VOS_Lseek(SWORD32 sdFileHandle, SWORD32 sdOffset, SWORD32 sdOrigin);
    VOS_STATUS    VOS_CreateDir(CHAR *pcDirName);
    VOS_STATUS    VOS_DeleteDir(CHAR *pcDirName);
    VOS_STATUS    VOS_RenameDir(CHAR *pcOldDirName, CHAR *pcNewDirName);
    DIR           *VOS_OpenDir(CHAR *pcDirName);
    struct dirent *VOS_ReadDir(DIR *pDir);
    VOS_STATUS    VOS_CloseDir(DIR *pDir);
    VOS_STATUS    VOS_Stat(CHAR *pcFileName, T_FileStat *pStat);
    VOS_STATUS    VOS_CreateMemDev(CHAR      *pcRamDiskName,
        CHAR      *pcRamAddr,
        SWORD32   sdwCHARsPerBlk,
        SWORD32   sdwBlksPerTrack,
        SWORD32   sdwNBlocks,
        SWORD32   sdwBlkOffset);
    VOS_STATUS    VOS_StartFtpServer(LPVOID   pLoginRtn,
        SWORD32  sdwStackSize);

    VOS_STATUS    VOS_StopFtpServer(VOID);

#if OS_TYPE == OS_PSOS
    VOS_STATUS    VOS_FileSystemInit(VOID);
#endif

    /****************** SOCKET **********************/
    VOS_STATUS  VOS_InitSocketLib(VOID);
    SOCKET      VOS_Socket(SWORD32 family, SWORD32 type, SWORD32 protocol);
    VOS_STATUS  VOS_Bind(SWORD32 s, const struct sockaddr * name, SWORD32 namelen);
    VOS_STATUS  VOS_Accept(SOCKET s, struct sockaddr * addr, SWORD32* addrlen);
    VOS_STATUS  VOS_Listen(SOCKET s, SWORD32 backlog);
    SWORD32     VOS_Send(SOCKET s, const char * buffer, SWORD32 len, SWORD32 flags);
    SWORD32     VOS_Recv(SOCKET s, const char * buffer, SWORD32 len, SWORD32 flags);
    VOS_STATUS  VOS_CloseSocket(SOCKET s);
    VOS_STATUS  VOS_CleanUp(void);
    VOS_STATUS  VOS_ConnectWithTimeout(SOCKET s, struct sockaddr * adrs, int adrsLen,
        struct timeval *  timeVal);
    SWORD32     VOS_SendTo(SOCKET s, const char *buffer, SWORD32 len, SWORD32 flags,
        struct sockaddr *to, int tolen);
    SWORD32     VOS_SendSocketMsg(SOCKET s, struct msghdr  *mp, SWORD32 flags);
    SWORD32     VOS_RecvFrom(SOCKET s, const char *buffer, SWORD32 len, SWORD32 flags,
        struct sockaddr *from, int  * pFromLen);
    SWORD32     VOS_RecvSocketMsg(SOCKET s, struct msghdr  *mp, SWORD32 flags);
    VOS_STATUS  VOS_Shutdown(SOCKET s, int how);
    VOS_STATUS  VOS_SetSockOpt(SOCKET s, int level, int  optname, char * optval, int optlen);
    VOS_STATUS  VOS_GetSockOpt(SOCKET s, int level, int  optname, char * optval, int *optlen);
    VOS_STATUS  VOS_GetSockName(SOCKET s, struct sockaddr * name, int * namelen);
    VOS_STATUS  VOS_GetPeerName(SOCKET s, struct sockaddr * name, int * namelen);
    SWORD32     VOS_Select(int width, fd_set  *pReadFds, fd_set  * pWriteFds,
        fd_set * pExceptFds, struct timeval * pTimeOut);
    SWORD32     VOS_Ioctl(int fd, int function, int arg);

    /****************** Exec **********************/
    VOS_STATUS VOS_Exec(
        const CHAR * strFile,		///要执行的程序名称
        const CHAR * strArgs		///要传入的命令行参数
    );

    VOS_STATUS VOS_ExecByFork(
        const CHAR * strFile,	///要执行的程序名称
        const CHAR * strArgs	///要传入的命令行参数
    );

    VOS_STATUS VOS_ForceDelete(CHAR *pcFileName);

    SWORD32 VOS_Chmod(
        const CHAR * pcFileName, ///文件名
        int iPermissions         ///要进行设置的文件属性
    );

    VOS_STATUS VOS_SetRLimit(const int iCoreFileSize);

    VOS_STATUS VOS_GetTimeOfDay(struct timeval *tv, struct timezone *tz);

#ifdef __cplusplus
}
#endif

#endif
/************** End of vos.h *******************************************/


/************** Begin of config.h *******************************************/

/* $Id$
*
* config.h created by wenhm in 2006-11-9
*
* Copyright 2001-2002, Meiosys (www.meiosys.com). All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef log4c_config_h
#define log4c_config_h

///#ifdef _WIN32
#define LOG4C_RCPATH	"."
#define VERSION			"1.2.1"
///#endif

#define WITH_ROLLINGFILE 1
#endif
/************** End of config.h *******************************************/

/************** Begin of vosnt.c *******************************************/
/**********************************************************
* 文件名称： vosnt.c
* 文件标识：
* 内容摘要： VOS在WINNT下的实现，包括调度模块、通信模块、
定时器模块、内存模块,主要实现NT下相应API函
数的封装
* 其它说明： 无
* 当前版本： V 0.1
**********************************************************/
/**********************************************************
* 函数名称： VOS_DeleteSem
* 功能描述： 删除二进制或计数信号量
* 输入参数： semId : 信号量标识符
* 输出参数： 无
* 返 回 值： VOS_OK       成功
VOS_ERROR    失败
* 其它说明： 无
* 修改日期    版本号     修改人      修改内容
* -----------------------------------------------
* 02/08/01     V0.1       杨新        XXXX
* 03/08/20     V1.0       王泽民      版本整合
**********************************************************/
#ifdef WIN32
VOS_STATUS VOS_DeleteSem(SEM_ID semId)
{
    BOOLEAN status;

    status = CloseHandle(semId);
    if (status != 0)
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERROR;
    }
}

/**********************************************************
* 函数名称： VOS_GiveSem
* 功能描述： 释放信号量
* 输入参数： semId : 信号量标识符
* 输出参数： 无
* 返 回 值： VOS_OK       成功
VOS_ERROR    失败
* 其它说明： 无
* 修改日期    版本号     修改人      修改内容
* -----------------------------------------------
* 02/08/01     V0.1       杨新        XXXX
* 03/08/20     V1.0       王泽民      版本整合
**********************************************************/
VOS_STATUS VOS_GiveSem(SEM_ID semId)
{
    BOOLEAN status;

    status = ReleaseSemaphore(semId, 1, NULL);

    if (status != 0)
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERROR;
    }
}

/**********************************************************
* 函数名称： VOS_TakeSem
* 功能描述： 获得信号量
* 输入参数： semId    : 信号量标识符
timeout  : 超时毫秒数
* 输出参数： 无
* 返 回 值： VOS_OK       成功
VOS_ERROR    失败
* 其它说明： 无
* 修改日期    版本号     修改人      修改内容
* -----------------------------------------------
* 02/08/01     V0.1       杨新        XXXX
* 03/08/20     V1.0       王泽民      版本整合
**********************************************************/
VOS_STATUS VOS_TakeSem(SEM_ID semId, SWORD32 timeout)
{
    WORD32 status;
    WORD32 timeLimit;

    if (timeout == -1)
    {
        timeLimit = INFINITE;
    }
    else
    {
        timeLimit = (WORD32)timeout;
    }

    status = WaitForSingleObject(semId, timeLimit);

    if (status == WAIT_OBJECT_0)
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERROR;
    }
}

/**********************************************************
* 函数名称： VOS_CreateMSem
* 功能描述： 创建互斥信号量
* 输入参数： 阻塞任务移出阻塞队列方式
VOS_SEM_Q_PRIORITY
VOS_SEM_Q_FIFO
VOS_SEM_DELETE_SAFE
VOS_SEM_INVERSION_SAFE
* 输出参数： 无
* 返 回 值： MSEM_ID        成功，返回互斥信号量标识符
NULL           失败，返回NULL
* 其它说明： 无
* 修改日期    版本号     修改人      修改内容
* -----------------------------------------------
* 02/08/01     V0.1       杨新        XXXX
* 03/08/20     V1.0       王泽民      版本整合
**********************************************************/
SEM_ID VOS_CreateMSem(SWORD32 options)
{
    SEM_ID semId;

    semId = VOS_CreateBSem(0, (VOS_SEM_STATE)1);

    return semId;
}

/**********************************************************
* 函数名称： VOS_CreateBSem
* 功能描述： 创建二进制信号量
* 输入参数： options       : 阻塞任务移出阻塞队列方式
VOS_SEM_Q_PRIORITY
VOS_SEM_Q_FIFO
initialState  : 信号量的初始状态
VOS_SEM_FULL
VOS_SEM_EMPTY
* 输出参数： 无
* 返 回 值： SEM_ID   成功，返回信号量标识符
VOS_ERROR    失败，返回NULL
* 其它说明： 无
* 修改日期    版本号     修改人      修改内容
* -----------------------------------------------
* 02/08/01     V0.1       杨新        XXXX
* 03/08/20     V1.0       王泽民      版本整合
**********************************************************/
SEM_ID VOS_CreateBSem(SWORD32 options, VOS_SEM_STATE initialState)
{
    HANDLE semId;

    semId = CreateSemaphore(NULL, (int)initialState, 1, NULL);

    return semId;
}

/**********************************************************
* 函数名称： VOS_CreateCSem
* 功能描述： 创建计数信号量
* 输入参数： options       : 阻塞任务移出阻塞队列方式
                                VOS_SEM_Q_PRIORITY
                                VOS_SEM_Q_FIFO
             initialState  : 信号量的初始状态
                                VOS_SEM_FULL
                                VOS_SEM_EMPTY
* 输出参数： 无
* 返 回 值： SEM_ID       成功，返回信号量标识符
             VOS_ERROR    失败，返回NULL
* 其它说明： 无
* 修改日期    版本号     修改人      修改内容
* -----------------------------------------------
* 02/08/01     V0.1       杨新        XXXX
* 03/08/20     V1.0       王泽民      版本整合
**********************************************************/
SEM_ID VOS_CreateCSem(SWORD32 options, SWORD32 initialState)
{
    HANDLE semId;

    semId = CreateSemaphore(NULL, initialState, MAXCOUNTSEM, NULL);

    return semId;
}

/** @fn VOS_CreateThreadMutex
@brief 创建线程互斥用的临界区
@details 内部采用CRITICAL_SECTION来实现的
    1.CRITICAL_SECTION用于线程间互斥，效率比较高.
    2.返回时SEM_ID保存CRITICAL_SECTION的指针
@return SEM_ID.
@retval 成功，返回信号量标识符
@retval NULL, 失败，返回NULL
*/
SEM_ID VOS_CreateThreadMutex()
{
    HANDLE semId;

    // Windows specific critical section implementation
    ///mutable CRITICAL_SECTION criticalSection;
    PCRITICAL_SECTION ptrCriticalSection = new CRITICAL_SECTION;

    ///	::InitializeCriticalSection(ptrCriticalSection);
    InitializeCriticalSectionAndSpinCount(ptrCriticalSection, 4000);

    semId = ptrCriticalSection;

    return semId;
}

/** @fn VOS_TakeThreadMutex
@brief 获得线程临界区信号量
@details 内部采用CRITICAL_SECTION来实现的
    1.CRITICAL_SECTION用于线程间互斥，效率比较高.
    2.SEM_ID保存CRITICAL_SECTION的指针
@param [in] SEM_ID semId 信号量标识符
@param [in] SWORD32 timeout 超时毫秒数
@return VOS_STATUS.
@retval VOS_OK.
@retval VOS_ERROR, 失败.
*/
VOS_STATUS VOS_TakeThreadMutex(SEM_ID semId, SWORD32 timeout)
{
    VOS_STATUS statusReturn = VOS_ERROR;
    PCRITICAL_SECTION ptrCriticalSection = (PCRITICAL_SECTION)semId;
    BOOL bTryEnterCriticalSection = FALSE;

    if (timeout == VOS_WAIT_FOREVER)
    {
        ::EnterCriticalSection(ptrCriticalSection);
        statusReturn = VOS_OK;
    }
    else if (timeout == 0)
    {
        bTryEnterCriticalSection = TryEnterCriticalSection(ptrCriticalSection);
        statusReturn = (TRUE == bTryEnterCriticalSection ? VOS_OK : VOS_ERROR);
    }
    else
    {
        if (TryEnterCriticalSection(ptrCriticalSection))
        {
            //do something that must be synh LeaveCriticalSection(&cs); 
            statusReturn = VOS_OK;
        }
        else
        {
            //do other job
            DWORD dwTickCount = GetTickCount();
            DWORD dwCountTry = 0;
            do
            {
                bTryEnterCriticalSection = TryEnterCriticalSection(ptrCriticalSection);
                dwCountTry++;
                if ((dwCountTry % 1000) == 0)
                {
                    /**
                    A value of zero causes the thread to relinquish the remainder of its time slice to any other thread that is ready to run.
                    If there are no other threads ready to run, the function returns immediately, and the thread continues execution.
                    */
                    Sleep(0);
                }
            } while (!bTryEnterCriticalSection && ((GetTickCount() - dwTickCount) < timeout));
            statusReturn = (TRUE == bTryEnterCriticalSection ? VOS_OK : VOS_ERROR);
        }
    }

    return statusReturn;
}

/** @fn VOS_GiveThreadMutex
@brief 释放线程临界区信号量
@details 内部采用CRITICAL_SECTION来实现的
    1.CRITICAL_SECTION用于线程间互斥，效率比较高.
    2.SEM_ID保存CRITICAL_SECTION的指针
@param [in] SEM_ID semId 信号量标识符
@return VOS_STATUS.
@retval VOS_OK.
@retval VOS_ERROR, 失败.
*/
VOS_STATUS VOS_GiveThreadMutex(SEM_ID semId)
{
    PCRITICAL_SECTION ptrCriticalSection = (PCRITICAL_SECTION)semId;
    ::LeaveCriticalSection(ptrCriticalSection);
    return VOS_OK;
}

/** @fn VOS_DeleteThreadMutex
@brief 删除线程临界区信号量
@details 内部采用CRITICAL_SECTION来实现的
    1.CRITICAL_SECTION用于线程间互斥，效率比较高.
    2.SEM_ID保存CRITICAL_SECTION的指针
@param [in] SEM_ID semId 信号量标识符
@return VOS_STATUS.
@retval VOS_OK.
@retval VOS_ERROR, 失败.
*/
VOS_STATUS VOS_DeleteThreadMutex(SEM_ID semId)
{
    PCRITICAL_SECTION ptrCriticalSection = (PCRITICAL_SECTION)semId;
    ::DeleteCriticalSection(ptrCriticalSection);
    DELETEP(ptrCriticalSection);
    return VOS_OK;
}

/**********************************************************
* 函数名称： VOS_GetSelfTaskID
* 功能描述： 获得当前任务ID
* 输入参数： 无
* 输出参数： 无
* 返 回 值： TASK_ID － 任务号
* 其它说明： 无
**********************************************************/
TASK_ID      VOS_GetSelfTaskID(VOID)
{
    return (TASK_ID)GetCurrentThreadId();
    //return GetCurrentThread();
}
#else
/**********************************************************
* 函数名称： VOS_DeleteSem
* 功能描述： 删除二进制或计数信号量
* 输入参数： semId : 信号量标识符
* 输出参数： 无
* 返 回 值： VOS_OK       成功
VOS_ERROR    失败
* 其它说明： 无
**********************************************************/
VOS_STATUS VOS_DeleteSem(SEM_ID semId)
{
    VOS_STATUS iResult = VOS_OK;
#ifdef PTHREAD
    if (semId != NULL)
    {
        iResult = (VOS_STATUS)sem_destroy(semId);
        free(semId);
    }
#endif

    return iResult;
}

/**********************************************************
* 函数名称： VOS_GiveSem
* 功能描述： 释放信号量
* 输入参数： semId : 信号量标识符
* 输出参数： 无
* 返 回 值： VOS_OK       成功
VOS_ERROR    失败
* 其它说明： 无
**********************************************************/
VOS_STATUS VOS_GiveSem(SEM_ID semId)
{
#ifdef PTHREAD
    if (sem_post(semId) == 0)
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERROR;
    }
#else
    return VOS_OK;
#endif
}

/**********************************************************
* 函数名称： VOS_TakeSem
* 功能描述： 获得信号量
* 输入参数： semId    : 信号量标识符
timeout  : 超时毫秒数
* 输出参数： 无
* 返 回 值： VOS_OK       成功
VOS_ERROR    失败
* 其它说明： 无
**********************************************************/
VOS_STATUS VOS_TakeSem(SEM_ID semId, SWORD32 timeout)
{
#ifdef PTHREAD
    WORD32 timeLimit;

    ///printf("VOS_TakeSem begin semId:%u, timeout:%d \n", semId,  timeout);

    if (timeout == VOS_WAIT_FOREVER)
    {
        sem_wait(semId);
        return VOS_OK;
    }
    else if (timeout == 0)
    {
        if (0 == sem_trywait(semId))
        {
            return VOS_OK;
        }
        else
        {
            return VOS_ERROR;
        }
    }
    else
    {
        // use proper timed spinlocks if supported.
        // http://www.opengroup.org/onlinepubs/007904975/functions/sem_timedwait.html
        struct timeval tv;
        struct timespec absTime;
        gettimeofday(&tv, 0);
        absTime.tv_sec = tv.tv_sec + timeout / 1000;
        absTime.tv_nsec = tv.tv_usec * 1000 + (timeout % 1000) * 1000000;

        ///printf("VOS_TakeSem semId: %u, absTime.tv_sec:%d absTime.tv_nsec:%d \n", semId, absTime.tv_sec, absTime.tv_nsec);

        if (sem_timedwait(semId, &absTime) == 0)
        {
            printf("VOS_OK \n");
            return VOS_OK;
        }
        else
        {
            printf("VOS_ERROR \n");
            return VOS_ERROR;
        }

        printf("VOS_TakeSem semId: %u, absTime.tv_sec:%d absTime.tv_nsec:%d \n", semId, absTime.tv_sec, absTime.tv_nsec);
    }
#else
    return VOS_OK;
#endif
}

/**********************************************************
* 函数名称： VOS_CreateMSem
* 功能描述： 创建互斥信号量
* 输入参数： 阻塞任务移出阻塞队列方式
VOS_SEM_Q_PRIORITY
VOS_SEM_Q_FIFO
VOS_SEM_DELETE_SAFE
VOS_SEM_INVERSION_SAFE
* 输出参数： 无
* 返 回 值： MSEM_ID 	成功，返回互斥信号量标识符
NULL       失败，返回NULL
* 其它说明： 无
**********************************************************/
SEM_ID VOS_CreateMSem(SWORD32 options)
{
#ifdef PTHREAD
    SEM_ID semId = (SEM_ID)malloc(sizeof(sem_t));

    if (0 != sem_init(semId, 0, 1))
    {
        return NULL;
    }

    return semId;
#else
    return 1;
#endif
}

/**********************************************************
* 函数名称： VOS_CreateBSem
* 功能描述： 创建二进制信号量
* 输入参数： options       : 阻塞任务移出阻塞队列方式
VOS_SEM_Q_PRIORITY
VOS_SEM_Q_FIFO
initialState  : 信号量的初始状态
VOS_SEM_FULL
VOS_SEM_EMPTY
* 输出参数： 无
* 返 回 值： SEM_ID   成功，返回信号量标识符
VOS_ERROR    失败，返回NULL
* 其它说明： 无
* 修改日期    版本号     修改人      修改内容
**********************************************************/
SEM_ID VOS_CreateBSem(SWORD32 options, VOS_SEM_STATE initialState)
{
    SEM_ID semId = (SEM_ID)malloc(sizeof(sem_t));

    if (0 != sem_init(semId, 0, initialState))
    {
        return NULL;
    }

    return semId;
}

/**********************************************************
* 函数名称： VOS_CreateCSem
* 功能描述： 创建计数信号量
* 输入参数： options       : 阻塞任务移出阻塞队列方式
                                VOS_SEM_Q_PRIORITY
                                VOS_SEM_Q_FIFO
             initialState  : 信号量的初始状态
                                VOS_SEM_FULL
                                VOS_SEM_EMPTY
* 输出参数： 无
* 返 回 值： SEM_ID	         成功，返回信号量标识符
             VOS_ERROR	     失败，返回NULL
* 其它说明： 无
**********************************************************/
SEM_ID VOS_CreateCSem(SWORD32 options, SWORD32 initialState)
{
    SEM_ID semId;

    semId = semCCreate(options, initialState);

    return semId;
}

/**********************************************************
* 函数名称： VOS_GetSelfTaskID
* 功能描述： 获得任务ID
* 输入参数： 无
* 输出参数： 无
* 返 回 值： TASK_ID － 任务号
* 其它说明： 无
**********************************************************/
TASK_ID    VOS_GetSelfTaskID(VOID)
{
    TASK_ID  dwID;

    dwID = pthread_self();

    return dwID;
}
#endif
/************** End of vosnt.c *******************************************/

/************** Begin of sharedmemory.h *******************************************/
#if defined(linux)  && defined(HAVE_SHM)
/***************************************************************************
|sharedmemory.h|  -  sharedmemory类的头文件-声明sharedmemory类
***************************************************************************/

#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/types.h>

//创建或关键字为Key的含有大小为size的共享内存对象，并将共享内存对象id放于shmid中返回。
//若共享内存对象已存在则取得该对象
extern int create_sharedmemory(key_t key, int size);

//创建关键字Key为IPC_PRIVATE(即关键字由系统选定)的含有大小为size的共享内存对象，并将共享内存对象id放于shmid中返回。
//因使用IPC_PRIVATE创建的共享内存的key都一样，所以key就不要了。
extern int create_sharedmemory_private(int size);

//将共享内存attach到进程自己得空间内，函数返回指向映射内存的指针。
extern void *attach_sharedmemory(int shmid);

//将共享内存disattach，唯一的参数是共享内存映射的指针
//返回值： -1 on error: errno = EINVAL (Invalid attach address passed)
extern int disattach_sharedmemory(void* shm);

//获取共享内存对象shmid的大小
extern int getsize_sharedmemory(int shmid);

//删除共享内存对象
extern int del_sharedmemory(int shmid);
#endif	/* SHAREDMEMORY_H */  

#endif
/************** Begin of sharedmemory.h *******************************************/

/************** Begin of sharedmemory.c *******************************************/
#if defined(linux)  && defined(HAVE_SHM)
/***************************************************************************
|saharedmemory.c|  -  saharedmemory共享内存对象 自己定义的一些操作函数的实现文件
***************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
///#include "sharedmemory.h"

//创建或关键字为Key的含有大小为size的共享内存对象，并将共享内存对象id放于shmid中返回。
//若共享内存对象已存在则取得该对象
int create_sharedmemory(key_t key, int size)
{
    int shmid;

    printf("Attempting to create new sharde memory object whose size is %d\n", size);

    //int shmget(key_t key,int size,int shmflg); 
    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) == -1)
    {
        fprintf(stderr, "shared memory object apply error:%s!\n", strerror(errno));
    }

    return shmid;
}

//创建关键字Key为IPC_PRIVATE(即关键字由系统选定)的含有大小为size的共享内存对象，并将共享内存对象id放于shmid中返回。
//因使用IPC_PRIVATE创建的共享内存的key都一样，所以key就不要了。
int create_sharedmemory_private(int size)
{
    int shmid;
    struct shmid_ds buf;

    printf("Attempting to create new sharde memory object whose size is %d\n", size);

    //int shmget(key_t key,int size,int shmflg); 
    if ((shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666)) == -1)
    {
        fprintf(stderr, "sharde memory object already exists!\n");
    }

    if (shmctl(shmid, IPC_STAT, &buf) == -1)
    {
        fprintf(stderr, "shmctl(shmid, IPC_STAT, buf) error!\n");//int shmget(key_t key,int size,int shmflg); 
    };

    return shmid;
}

//将共享内存attach到进程自己得空间内，函数返回指向映射内存的指针。
void *attach_sharedmemory(int shmid)
{
    return(shmat(shmid, 0, 0));
}

//将共享内存disattach，唯一的参数是共享内存映射的指针
//返回值： -1 on error: errno = EINVAL (Invalid attach address passed)
int disattach_sharedmemory(void* shm)
{
    return(shmdt((char*)shm));
}

//获取共享内存对象shmid的大小
int getsize_sharedmemory(int shmid)
{
    struct shmid_ds buf;

    if (shmctl(shmid, IPC_STAT, &buf) == -1)
    {
        fprintf(stderr, "shmctl(shmid, IPC_STAT, buf) error!\n");
    };

    return buf.shm_segsz;
}

//删除共享内存对象
int del_sharedmemory(int shmid)
{
    /* 共享内存对象并不随程序的结束而被删除,如果我们没删除的话(将1改为0)
    可以用ipcs命令查看到信号灯,用ipcrm可以删除共享内存对象的*/
    return shmctl(shmid, IPC_RMID, 0);
}
#endif
/** 获取模块使用次数
@return long.
*/
long GetModuleUsage()
{
    long lModuleUsage = -1;
#if defined(linux ) && defined(HAVE_SHM)
    //以8个8来作为共享内存的key
#define  MODULE_USAGE_SHMKEY    88888888
    gs_iModuleUsage_shmid = create_sharedmemory(MODULE_USAGE_SHMKEY, sizeof(long));
    void *ptrModuleUsageBuffer = attach_sharedmemory(gs_iModuleUsage_shmid);

    if (NULL != ptrModuleUsageBuffer)
    {
        lModuleUsage = *(long *)ptrModuleUsageBuffer;

        ///释放共享内存
        disattach_sharedmemory(ptrModuleUsageBuffer);
        ///del_sharedmemory(gs_iModuleUsage_shmid);
    }
#endif
    return lModuleUsage;
}

/** 设置模块使用次数
@return void.
*/
void SetModuleUsage(long lModuleUsage)
{
#if defined(linux ) && defined(HAVE_SHM)
    gs_iModuleUsage_shmid = create_sharedmemory(MODULE_USAGE_SHMKEY, sizeof(long));
    void *ptrModuleUsageBuffer = attach_sharedmemory(gs_iModuleUsage_shmid);

    if (NULL != ptrModuleUsageBuffer)
    {
        *(long *)ptrModuleUsageBuffer = lModuleUsage;

        ///释放共享内存
        disattach_sharedmemory(ptrModuleUsageBuffer);
        ///del_sharedmemory(gs_iModuleUsage_shmid);
    }
#endif
}

/** 释放使用的共享内存
@return void.
*/
void ReleaseModuleUsage()
{
#if defined(linux ) && defined(HAVE_SHM)
    long lModuleUsage = 0;
    gs_iModuleUsage_shmid = create_sharedmemory(MODULE_USAGE_SHMKEY, sizeof(long));
    void *ptrModuleUsageBuffer = attach_sharedmemory(gs_iModuleUsage_shmid);

    if (NULL != ptrModuleUsageBuffer)
    {
        lModuleUsage = *(long *)ptrModuleUsageBuffer;

        ///释放共享内存
        disattach_sharedmemory(ptrModuleUsageBuffer);

        if (lModuleUsage == 0)
        {
            del_sharedmemory(gs_iModuleUsage_shmid);
        }
    }
#endif
}
/************** Begin of sharedmemory.c *******************************************/

/************** Begin of CommonUtils.h ********************************************/
namespace NSLog
{
    namespace  NSCommonUtils
    {
#ifndef TEXT_SLASH
#define TEXT_SLASH L"\\"
#endif
#ifndef LOG_DIR
#define LOG_DIR  L"Log"
#endif
        ///枚举类型:操作系统版本号
        enum EOperationVersion
        {
            ///Unknown Operation System
            eUnKnown = -1,
            ///Win32
            eWin32 = 0,
            ///Win95
            eWin95,
            ///Win98
            eWin98,
            ///WinME
            eWinME,
            ///WinNT
            eWinNT,
            ///Win2000
            eWin2000,
            ///WinXP
            eWinXP,
            ///Win2003
            eWin2003,
            ///Win Vista
            eWinVista,
            ///Win2008
            eWin2008,
            ///Win7
            eWin7,
            ///Win2008R2
            eWin2008R2,
            ///Win8
            eWin8,
            ///Win8.1
            eWin8_1,
            ///Unknown high
            eUnKnownHigh
        };

        /** 获取操作系统版本
        返回操作系统的枚举类型EOperationVersion
        本函数的代码改造自
        MSDN:
        http://msdn.microsoft.com/en-us/library/ms724429.aspx.
        ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.WIN32COM.v10.en/sysinfo/base/getting_the_system_version.htm
        @return E_OperationVersion.具体的操作系统枚举
        */
        static EOperationVersion GetOperationVersion();

        /**获取日志配置文件的目录
        将自动获取彩虹的工作目录加上log子目录就是日志配置文件的目录
        @return std::string.
        */
        static std::wstring  GetLogCfgFilePath();

        /**获取日志配置文件的目录
        将自动获取彩虹的工作目录加上log子目录就是日志配置文件的目录
        @return std::string.
        */
        static std::wstring  GetDefaultLogParentPath();
    }
}

/** 将Unicode字符转换为Ansi字符
这个指定源和目的的长度,是安全版本
@return int.errcode of wcsncpy_s
*/
static int Unicode2Ansi(
    const WCHAR *strSrc,
    const UINT &uiSrcLen,	///size in words
    char *strDst,
    const UINT &uiDstLen	///size in byte
);

/** 将Unicode字符转换为std::string
这个指定源的长度,是安全版本
@return int.errcode of wcsncpy_s
*/
static int Unicode2AnsiString(
    const WCHAR *strSrc,
    const UINT &uiSrcLen,	///size in words
    std::string &strDst
);

/** 对std::wstring进行Replace操作
@return void.
*/
static void StringUtils_Replace(
    std::string &str,
    const std::string &strSrc,
    const std::string &strDst
);

/** 对std::wstring进行路径字符串调整操作
1.将/替换成\
2.将两个连续的分隔符\\替换成\
@return void.
*/
static void AdjustFilePathSlash(
    std::wstring & strPath
);

/** 对std::string进行路径字符串调整操作
1.将/替换成\
2.将两个连续的分隔符\\替换成\
@return void.
*/
static void AdjustFilePathSlash(
    std::string & strPath
);

/** 对std::wstring进行Replace操作
@return void.
*/
static void StringUtils_Replace(
    std::wstring &str,
    const std::wstring& strSrc,
    const std::wstring& strDst
);

/**
将大小为count的缓冲区中内容按字节以16进制字符串打印出来,
返回值即为指向相应的字符串，
该返回指向的存储区域要调用本函数的用户显示的进行删除
@return char *.
*/
/*static*/ char * sprintf_data(char *buff, int count);

/**将大小为count的缓冲区中内容按字节以16进制字符串打印出来,
str缓冲区的大小应大于等于count+1,因为还要一个字节存放字符串结束符
@return void.
*/
static void sprintf_data(char *str, char *buff, int count);

/** @fn GetFileNameByStripPath
@brief 去除文件路径得到纯文件名
@param [in] const std::string &strFilePath
@return std::string.
*/
static std::string GetFileNameByStripPath(const std::string &strFilePath);

///const  char *转换为 log4cplus::tstring
const log4cplus::tstring ToLog4cplus_TString(const char * str);

namespace NSLog
{
    /*
    * 从宽字符串转为ansi字符串
    * @param [in] in_str 为输入的源ansi字符
    * @return 返回值为宽字符串
    */
    std::string UnicodeToAscii(const std::wstring& in_str);

    std::wstring AsciiToUnicode(const std::string& in_str);

    std::wstring UTF8ToUnicode(const std::string& in_utf8Str);
}

/** @fn CreateMultipleDirectory
@brief 创建多级目录
@param [out] char *strPath 存放要创建目录的路径
@return int,返回路径字符串的长度
*/
static int CreateMultipleDirectory(const char* strPath);

/** 创建多级目录
*/
static bool CreateMultipleDirectory(const std::wstring& strPath);

/************** end of CommonUtils.h ************************************/


/************** Begin of sd/defs.h *******************************************/
/* $Id$
*
* Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef __sd_defs_h
#define __sd_defs_h

#ifdef  __cplusplus
# define __SD_BEGIN_DECLS  extern "C" {
# define __SD_END_DECLS    }
#else
# define __SD_BEGIN_DECLS
# define __SD_END_DECLS
#endif

#endif

/************** End of sd/defs.h *******************************************/


/************** Begin of sd/malloc.h *******************************************/
/* $Id$
*
* Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef __sd_malloc_h
#define __sd_malloc_h

#include <stddef.h>
#include <stdlib.h>
///#include <sd/defs.h>

/**
* @file malloc.h
*/

__SD_BEGIN_DECLS

typedef void(*sd_malloc_handler_t)();

extern sd_malloc_handler_t sd_malloc_set_handler(void(*a_handler)());

#ifndef __SD_DEBUG__
extern void *sd_malloc(size_t n);
extern void *sd_calloc(size_t n, size_t s);
extern void *sd_realloc(void *p, size_t n);
extern char *sd_strdup(const char *__str);
#else
#define sd_malloc	malloc
#define sd_calloc	calloc
#define sd_realloc	realloc
#define sd_strdup	strdup
#endif

__SD_END_DECLS

#endif
/************** End of sd/malloc.h *******************************************/

/************** Begin of malloc.c *******************************************/
/* xmalloc.c -- malloc with out of memory checking
Copyright (C) 1990,91,92,93,94,95,96,97 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with the GNU C Library; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.  */

#ifdef HAVE_CONFIG_H
///#include "config.h"
#endif
#include <stdio.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <string.h>
#include <stdlib.h>
///#include <sd/error.h>

#if defined(__APPLE__)
    ///# include <sys/time.h>
    ///# include <crt_externs.h>
# define environ (*_NSGetEnviron())
#endif /* __APPLE__ */

///typedef void (*sd_malloc_handler_t)();

static char*  first_break = NULL;
static sd_malloc_handler_t handler = NULL;

//static void *
//fixup_null_alloc (n)
//size_t n;
static void * fixup_null_alloc(size_t n)
{
    void* p = 0;

#ifdef HAVE_SBRK
    if (n == 0)
        p = malloc((size_t)1);

    if (p == 0) {
        extern char **environ;
        size_t allocated;

        if (first_break != NULL)
            allocated = (char *)sbrk(0) - first_break;
        else
            allocated = (char *)sbrk(0) - (char *)&environ;
        sd_error("\nCannot allocate %lu bytes after allocating %lu bytes\n",
            (unsigned long)n, (unsigned long)allocated);

        if (handler)
            handler();
        else {
            sd_error("\n\tMemory exhausted !! Aborting.\n");
            abort();
        }
    }
#endif
    return p;
}

//sd_malloc_handler_t
//sd_malloc_set_handler(a_handler)
//sd_malloc_handler_t a_handler;
sd_malloc_handler_t sd_malloc_set_handler(sd_malloc_handler_t a_handler)
{
    sd_malloc_handler_t previous = handler;

    handler = a_handler;
    return previous;
}

/* Allocate N bytes of memory dynamically, with error checking.  */

//void *
//sd_malloc (n)
//size_t n;
void *sd_malloc(size_t n)
{
    void *p = NULL;

    p = malloc(n);
    if (p == 0)
        p = fixup_null_alloc(n);
    return p;
}

/* Allocate memory for N elements of S bytes, with error checking.  */

//void *
//sd_calloc (n, s)
//size_t n, s;
void *sd_calloc(size_t n, size_t s)
{
    void *p = NULL;

    p = calloc(n, s);
    if (p == 0)
        p = fixup_null_alloc(n * s);
    return p;
}

/* Change the size of an allocated block of memory P to N bytes,
with error checking.
If P is NULL, run sd_malloc.  */

//void *
//sd_realloc (p, n)
//void *p;
//size_t n;
void *sd_realloc(void *p, size_t n)
{
    if (p == 0)
        return sd_malloc(n);
    p = realloc(p, n);
    if (p == 0)
        p = fixup_null_alloc(n);
    return p;
}

/* Return a newly allocated copy of STRING.  */

//char *
//sd_strdup (string)
//const char *string;
char *sd_strdup(const char *string)
{
    return strcpy((char *)sd_malloc(strlen(string) + 1), string);
}
/************** End of malloc.c *******************************************/

/************** Begin of sd/sd_xplatform.h *******************************************/
/* $Id$
*
* sd_xplatform.h
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef __sd_xplatform_h
#define __sd_xplatform_h

#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#else
#include <time.h>
#include <io.h> /* needed for _access  */
#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif 
#include <windows.h>
///#include <winsock.h>
#include <process.h>
#endif


#ifdef HAVE_STDINT_H
#       include <stdint.h>
#define  XP_UINT64 uint64_t
#define  XP_INT64 int64_t
#else
#ifndef _WIN32
#define  XP_UINT64 unsigned long long
#define  XP_INT64 long long
#else
#define  XP_UINT64 DWORD64
#define  XP_INT64 __int64
#endif
#endif

///#include <log4c/defs.h>
///#include <sd/defs.h>

/*extern int sd_optind; */
LOGWRAPPER_DATA int sd_optind;

extern void getopt_reset(void);

extern int sd_getopt(int argc, char *const *argv, const char *opts);

#ifdef _WIN32
#define SD_GETOPT(a,b,c) sd_getopt(a,b,c)
#define SD_OPTIND sd_optind
#else
#define SD_GETOPT(a,b,c) getopt(a,b,c)
#define SD_OPTIND optind
#endif


#ifdef _WIN32
#define SD_GETTIMEOFDAY(a,b) sd_gettimeofday(a,b)
extern int sd_gettimeofday(LPFILETIME lpft, void* tzp);
#else
#define SD_GETTIMEOFDAY(a,b) gettimeofday(a,b)
extern int sd_gettimeofday(struct timeval* tp, void* tzp);
#endif

#ifdef _WIN32
#define FILE_SEP "\\"
#else
#define FILE_SEP "/"
#endif

#ifdef _WIN32
#define SD_ACCESS_READ(a) _access(a,04)
#else
#define SD_ACCESS_READ(a) access(a,R_OK)
#endif

int sd_stat_ctime(const char* path, time_t* time);
#define SD_STAT_CTIME(path, time) sd_stat_ctime(path, time)

#ifndef _WIN32
#define DIFF_CMD  "/usr/bin/diff -q"
#else
#define DIFF_CMD  "comp.exe"
#endif

#ifdef _WIN32
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define alloca _alloca
#define strncasecmp strnicmp
#define strcasecmp stricmp
#define YY_NO_UNISTD_H
#define sleep(x) Sleep(x*1000)
#endif


/* Maybe should be using this for to mean
* MS compiler #if defined(_MSC_VER)
*/
#ifdef _WIN32
#define pthread_t HANDLE
#define pthread_mutex_t HANDLE
#define pthread_attr_t DWORD
#define THREAD_FUNCTION DWORD (WINAPI *)(void *)

/*
* This one not obvious: you would have naturally thought of mapping to
* CreateThread()--turns out that to be safe using CRT functions
* you need to use _begintheadex().
* cf. http://msdn2.microsoft.com/en-us/library/7t9ha0zh.aspx
*  http://groups.google.com/group/comp.os.ms-windows.programmer.win32/browse_thread/thread/86d8624e7ee38c5d/f947ac76cd10f397?lnk=st&q=when+to+use+_beginthreadex&rnum=1#f947ac76cd10f397
*
*/
#define pthread_create(thhandle,attr,thfunc,tharg) \
	(int)((*thhandle=(HANDLE)_beginthreadex(NULL,0,(THREAD_FUNCTION)thfunc,tharg,0,NULL))==NULL)
#define pthread_join(thread, result) \
	((WaitForSingleObject((thread),INFINITE)!=WAIT_OBJECT_0) || !CloseHandle(thread))
#define pthread_exit() _endthreadex(0)
#define pthread_cancel(thread) TerminateThread(thread,0)

#define pthread_mutex_init(pobject,pattr) (*pobject=CreateMutex(NULL,FALSE,NULL))
#define pthread_mutex_lock(pobject) WaitForSingleObject(*pobject,INFINITE)
#define pthread_mutex_unlock(pobject) ReleaseMutex(*pobject)

#define pthread_mutex_destroy(pobject) CloseHandle(*pobject)

#endif


#ifdef __HP_cc
#define inline __inline
#endif 

#endif
/************** End of sd/sd_xplatform.h *******************************************/


/************** Begin of sd/sprintf.h *******************************************/
/* $Id$
*
* Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef __sd_sprintf_h
#define __sd_sprintf_h

/**
* @file sprintf.h
*
* @brief Formatted output conversion
*
* These functions write the output under the control of a format
* string that specifies how subsequent arguments (or arguments
* accessed via the variable-length argument facilities of stdarg(2))
* are converted for output.
*
* They do not write more than \a size bytes, including the trailing
* \c '\0'.
*
* These functions return the number of characters printed (not
* including the trailing \c `\0' used to end output to strings). They
* return -1 if the output was truncated due to the @a size limit.
*
*/

#include <stdarg.h>
#include <stddef.h>
///#include <sd/defs.h>

__SD_BEGIN_DECLS

/**
* Same as fprintf(3) with auto-allocation of the resulting buffer,
* and output directly in a file, not a stream.
*/
extern int sd_fprintf(int fd, const char *fmt, ...);

/**
* Same as sprintf(3) with auto-allocation of the resulting buffer.
*/
extern char* sd_sprintf(const char* a_fmt, ...);

/**
* Same as vsprintf(3) with auto-allocation of the resulting buffer.
*/
extern char* sd_vsprintf(const char* a_fmt, va_list a_arg);

#if defined(__osf__)
extern int snprintf(char* str, size_t size, const char* fmt, ...);
extern int vsnprintf(char* str, size_t size, const char* fmt, va_list arg);
#endif

__SD_END_DECLS

#endif
/************** End of sd/sprintf.h *******************************************/


/************** Begin of log4c/init.h *******************************************/
/* $Id$
*
* Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef __log4c_init_h
#define __log4c_init_h

///#include <log4c/defs.h>
#include <stdio.h>

/**
* @file init.h
*
* @brief log4c constructors and destructors
*
**/

/**
* constructor
*
* @returns 0 for success
**/
LOGWRAPPER_API int log4c_init(void);

/**
* another constructor
*
* 带有配置文件名的初始化函数
* 完成功能和log4c_init完全一致，只是这里的配置文件名以参数形式传入的
* @returns 0 for success .
*/
LOGWRAPPER_API int log4c_init_with_cfg_file(const char *strCfgFileName);

/**
* destructor
*
* @returns 0 for success
**/
LOGWRAPPER_API int log4c_fini(void);

/*
* Dumps all the current appender, layout and rollingpolicy types
* known by log4c.
* @param stream to write to
*/
LOGWRAPPER_API void log4c_dump_all_types(FILE *fp);

/*
* Dumps all the current instances of categories, appenders, layouts
* and rollingpolicy objects.
* An instances of a type consists of the base
* type information (name plus function table) and an instance name and
* configuration.  For example one can have an instance of the rollingfile
* appender which logs to /var/tmp and another instance which logs to
* /usr/tmp.  They are both of type rollingfile, but are distinct instances of
* it
* @param stream to write t
*/
LOGWRAPPER_API void log4c_dump_all_instances(FILE *fp);

#endif

/************** End of log4c/init.h *******************************************/

/************** Begin of log4c/buffer.h *******************************************/
/* $Id$
*
* buffer.h
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef __log4c_buffer_h
#define __log4c_buffer_h

/**
* @file buffer.h
*
* @brief log4c buffer
*
**/

///#include <log4c/defs.h>
#include <stddef.h>

__LOG4C_BEGIN_DECLS

/**
* @brief buffer object
*
* Attributes description:
*
* @li @c size current size of the buffer
* @li @c maxsize maximum size of the buffer. 0 means no limitation.
* @li @c data raw data
**/
typedef struct
{
    size_t buf_size;
    size_t buf_maxsize;
    char*  buf_data;

} log4c_buffer_t;


__LOG4C_END_DECLS

#endif
/************** End of log4c/buffer.h *******************************************/

/************** Begin of error.c *******************************************/
///static const char version[] = "$Id$";

/*
* See the COPYING file for the terms of usage and distribution.
*/

#ifdef HAVE_CONFIG_H
///#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_STDARG_H
#  include <stdarg.h>
#else
#  ifdef HAVE_VARARGS_H
#    include <varargs.h>
#  endif
#endif

///#include <sd/error.h>
///#include <sd_xplatform.h>

int sd_debug(const char *fmt, ...)
{
    va_list args;
    int r;

    if (!getenv("SD_DEBUG"))
        return 0;

    r = fprintf(stderr, "[DEBUG] ");
    va_start(args, fmt);
    r += vfprintf(stderr, fmt, args);
    va_end(args);
    r += fprintf(stderr, "\n");

    return r;
}

int sd_error(const char *fmt, ...)
{
    va_list args;
    int r;

    if (!getenv("SD_ERROR"))
        return 0;

    r = fprintf(stderr, "[ERROR] ");
    va_start(args, fmt);
    r += vfprintf(stderr, fmt, args);
    va_end(args);
    r += fprintf(stderr, "\n");

    return r;
}

/************** End of error.c *******************************************/


/****************************自己写的嵌入到log4c中的代码************************************************/
//////////////////////////////////////////////////////////////////////////
///下面函数为自己写的一些函数，为使用log4c起来方便而设置的

static int log4c_is_init = 0;

/** @fn log4c_get_log4c_is_init
@brief 获取log4c的初始化标志变量
@return int.
*/
extern int log4c_get_log4c_is_init()
{
    return log4c_is_init;
}

/** @fn is_log4c_init
@brief 判断log4c是否进行了初始化
@return bool.
@retval true 进行了初始化
@retval false 没有进行初始化
*/
extern BOOL is_log4c_init()
{
    BOOL bResult = (log4c_is_init > 0);
    return bResult;
}

/** 带有配置文件名的初始化函数
完成功能和log4c_init完全一致，只是这里的配置文件名以参数形式传入的
@returns 0 for success .
*/
static ConfigureAndWatchThread *gs_ptrConfigureAndWatchThread = NULL;
extern int log4c_init_with_cfg_file(const char *strCfgFileName)
{
    size_t i = 0;
    int ret = 0;

    if (log4c_is_init)
    {
        sd_debug("log4c already initialized ]");
        return 0;
    }

    log4c_is_init++;

    ///log4cplus原始的日志模块初始化代码
    //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    //Logger root = Logger::getRoot();
    //LOG4CPLUS_WARN(root, "Testing....");
    //Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("testlogger"));
    //LOG4CPLUS_WARN(logger, "Testing....");

    ///log4cplus 日志配置文件初始化	

    /** 设置正确的本地locale，以让log4cplus在unicode下支持中文路径
    @ref http://hi.baidu.com/whysnl/blog/item/7ef48e308da3cd5cad4b5fcf.html
    */
    if (TRUE == GetReReadLogCfgFile())
    {
        ///启动线程对配置脚本进行监控，一旦发现配置脚本被更新则立刻重新加载配置
        if (NULL == gs_ptrConfigureAndWatchThread)
        {
            gs_ptrConfigureAndWatchThread = new ConfigureAndWatchThread(ToLog4cplus_TString(strCfgFileName), GetReReadMilliseconds());
        }
    }
    else
    {
        ///C库设置本地locale
        ///std::setlocale("");
        ///setlocale( LC_ALL, "" ); 
        ///C++库设置本地locale
        ///std::locale::global(std::locale(""));
        PropertyConfigurator::doConfigure(ToLog4cplus_TString(strCfgFileName));
        ///std::locale::global(std::locale("C"));			
    }

    Logger root = Logger::getRoot();
    LOG4CPLUS_WARN(root, "PropertyConfigurator::doConfigure....");

    ///loggerNameDefault 简写为LD
    Logger loggerNameDefault = Logger::getInstance(LOGGER_DEFAULT_NAME);
    ///LOG4CPLUS_WARN(loggerNameDefault, "PropertyConfigurator::doConfigure...., File:" << GetFileNameByStripPath(strCfgFileName).c_str());
    ///LOG_WARN("PropertyConfigurator::doConfigure...., File:" << GetFileNameByStripPath(strCfgFileName).c_str());

    ///重置CLogWrapper::GetDefaultLogger(true);
    CLogWrapper::Instance()->GetDefaultLogger(true);

    ///记录第一条日志
    LOG_CRIT("LOG4CPLUS_MAKE_VERSION_STR:" << LOG4CPLUS_VERSION_STR);
    LOG_CRIT("log4cplus PropertyConfigurator::doConfigure...., Log Config File:" << strCfgFileName);

#if 0
    for (int i = 0; i < 1024 * 10; i++)
    {
        LOG4CPLUS_WARN(loggerNameDefault, "PropertyConfigurator::doConfigure....");
        LOG4CPLUS_ERROR(loggerNameDefault, "PropertyConfigurator::doConfigure....");
    }
#endif

    return ret;
}

/*********************************************************************************/


/******************************************************************************/
extern int log4c_init(void)
{
    size_t i = 0;
    int ret = 0;

    sd_debug("log4c_init[");

    if (log4c_is_init) {
        sd_debug("log4c already initialized ]");
        return 0;
    }

    log4c_is_init++;


    sd_debug("]");
    return ret;
}

/******************************************************************************/
void __log4c_reread(void)
{
    time_t file_ctime = 0;
    int i = 0;
}

/******************************************************************************/
/*
* Rereads any log4crc files that have changed
*/
void log4c_reread(void)
{
#ifdef __ENABLE_REREAD__
    if (0 != log4c_rc->config.reread) {
        __log4c_reread();
    }
#endif
}

/******************************************************************************/
extern int log4c_fini(void)
{
    int rc = 0;

    if (!log4c_is_init) {
        rc = 0;
        sd_debug("not cleaning up--log4c not initialized");
        goto log4c_fini_exit;
    }

    log4c_is_init--;/* Some acceptable use of goto here to avoid lots of nested ifs
    * when we need a quick exit
    */
    sd_debug("log4c_fini[");

log4c_fini_exit:
    sd_debug("]");

    //////////////////////////////////////////////////////////////////////////
    ///销毁动态分配的对象
    DELETEP(gs_ptrConfigureAndWatchThread)

        ///std::locale::global(std::locale("C"));

        return 0;
}

/******************************************************************************/
#ifdef __GNUC__
extern void __attribute__((constructor)) __log4c_init(void)
{
#ifdef WITH_CONSTRUCTORS
    log4c_init();
#endif
}

extern void __attribute__((destructor)) __log4c_fini(void)
{
#ifdef WITH_CONSTRUCTORS
    log4c_fini();
#endif
}
#endif

/******************************************************************************/

///////////////////////////////////////////////////////////////////////////////

/********************** Begin of CommonUtils.c ********************************/
/*
* CommonUtils.c
*
* 通用Utiltity帮助代码的定义文件
*/
#include <ShlObj.h>
namespace NSLog
{
    namespace  NSCommonUtils
    {
        EOperationVersion GetOperationVersion()
        {
            const int iBufLen = 80;
            EOperationVersion eOperationVersion = eUnKnown;
            OSVERSIONINFOEXW osvi;
            BOOL bOsVersionInfoEx;

            // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
            // If that fails, try using the OSVERSIONINFO structure.

            ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
            osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

            bOsVersionInfoEx = GetVersionExW((OSVERSIONINFO *)&osvi);
            if (!bOsVersionInfoEx)
            {
                osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                if (!GetVersionExW((OSVERSIONINFO *)&osvi))
                {
                    return eOperationVersion;
                }
            }

            switch (osvi.dwPlatformId)
            {
                // Test for the Windows NT product family.
            case VER_PLATFORM_WIN32_NT:
                if (osvi.dwMajorVersion == 6)
                {
                    if (osvi.dwMinorVersion == 0)
                    {
                        if (osvi.wProductType == VER_NT_WORKSTATION)
                        {
                            eOperationVersion = eWinVista;
                            ///StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
                        }
                        else
                        {
                            eOperationVersion = eWin2008;
                            ///StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
                        }
                    }
                    else if (osvi.dwMinorVersion == 1)
                    {
                        if (osvi.wProductType == VER_NT_WORKSTATION)
                        {
                            eOperationVersion = eWin7;
                            ///StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
                        }
                        else
                        {
                            eOperationVersion = eWin2008R2;
                            ///StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
                        }
                    }
                    else if (osvi.dwMinorVersion == 2)
                    {
                        eOperationVersion = eWin8;
                    }
                    else if (osvi.dwMinorVersion == 3)
                    {
                        eOperationVersion = eWin8_1;
                    }
                    else if (osvi.dwMinorVersion > 3)
                    {
                        eOperationVersion = eUnKnownHigh;
                        ///StringCchCat(pszOS, BUFSIZE, TEXT("Unkonwn high " ));
                    }
                }
                else if (osvi.dwMajorVersion > 6)
                {
                    eOperationVersion = eUnKnownHigh;
                    ///StringCchCat(pszOS, BUFSIZE, TEXT("Unkonwn high " ));
                }

                // Test for the specific product.
                if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
                {
                    eOperationVersion = eWin2003;
                    ///printf ("Microsoft Windows Server 2003, ");
                }

                if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
                {
                    eOperationVersion = eWinXP;
                    ///printf ("Microsoft Windows XP ");
                }

                if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
                {
                    eOperationVersion = eWin2000;
                    ///printf ("Microsoft Windows 2000 ");
                }

                if (osvi.dwMajorVersion <= 4)
                {
                    eOperationVersion = eWinNT;
                    ///printf("Microsoft Windows NT ");
                }

                // Test for specific product on Windows NT 4.0 SP6 and later.
                if (bOsVersionInfoEx)
                {
                    // Test for the workstation type.
                    if (osvi.wProductType == VER_NT_WORKSTATION)
                    {
                        if (osvi.dwMajorVersion == 4)
                        {
                            ///printf ( "Workstation 4.0 " );
                        }
                        else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
                        {
                            ///printf ( "Home Edition " );
                        }
                        else
                        {
                            ///printf ( "Professional " );
                        }
                    }

                    // Test for the server type.
                    else if (osvi.wProductType == VER_NT_SERVER ||
                        osvi.wProductType == VER_NT_DOMAIN_CONTROLLER)
                    {
                        if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
                        {
                            if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                                printf("Datacenter Edition ");
                            else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                                printf("Enterprise Edition ");
                            else if (osvi.wSuiteMask == VER_SUITE_BLADE)
                                printf("Web Edition ");
                            else printf("Standard Edition ");
                        }
                        else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
                        {
                            if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                                printf("Datacenter Server ");
                            else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                                printf("Advanced Server ");
                            else printf("Server ");
                        }
                        else  // Windows NT 4.0 
                        {
                            if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                                printf("Server 4.0, Enterprise Edition ");
                            else printf("Server 4.0 ");
                        }
                    }
                }
                // Test for specific product on Windows NT 4.0 SP5 and earlier
                else
                {
                    HKEY hKey;
                    WCHAR szProductType[iBufLen];
                    DWORD dwBufLen = iBufLen;
                    LONG lRet;

                    lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                        L"SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
                        0, KEY_QUERY_VALUE, &hKey);
                    if (lRet != ERROR_SUCCESS)
                    {
                        return eOperationVersion;
                    }

                    lRet = RegQueryValueExW(hKey, L"ProductType", NULL, NULL,
                        (LPBYTE)szProductType, &dwBufLen);
                    if ((lRet != ERROR_SUCCESS) || (dwBufLen > iBufLen))
                    {
                        return eOperationVersion;
                    }

                    RegCloseKey(hKey);

                    if (lstrcmpi(L"WINNT", szProductType) == 0)
                        printf("Workstation ");
                    if (lstrcmpi(L"LANMANNT", szProductType) == 0)
                        printf("Server ");
                    if (lstrcmpi(L"SERVERNT", szProductType) == 0)
                        printf("Advanced Server ");
                    printf("%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
                }

                // Display service pack (if any) and build number.

                if (osvi.dwMajorVersion == 4 &&
                    lstrcmpiW(osvi.szCSDVersion, L"Service Pack 6") == 0)
                {
                    HKEY hKey;
                    LONG lRet;

                    // Test for SP6 versus SP6a.
                    lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
                        0, KEY_QUERY_VALUE, &hKey);
                    if (lRet == ERROR_SUCCESS)
                        printf("Service Pack 6a (Build %d)\n",
                            osvi.dwBuildNumber & 0xFFFF);
                    else // Windows NT 4.0 prior to SP6a
                    {
                        /*printf( "%s (Build %d)\n",
                            osvi.szCSDVersion,
                            osvi.dwBuildNumber & 0xFFFF);
                            */
                    }

                    RegCloseKey(hKey);
                }
                else // not Windows NT 4.0 
                {
                    /*
                    printf( "%s (Build %d)\n",
                        osvi.szCSDVersion,
                        osvi.dwBuildNumber & 0xFFFF);
                    */
                }

                break;

                // Test for the Windows Me/98/95.
            case VER_PLATFORM_WIN32_WINDOWS:

                if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
                {
                    eOperationVersion = eWin95;
                    printf("Microsoft Windows 95 ");
                    if (osvi.szCSDVersion[1] == L'C' || osvi.szCSDVersion[1] == L'B')
                    {
                        ///printf("OSR2 " );
                    }
                }

                if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
                {
                    eOperationVersion = eWin98;
                    ///printf ("Microsoft Windows 98 ");
                    if (osvi.szCSDVersion[1] == L'A')
                    {
                        ///printf("SE " );
                    }
                }

                if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
                {
                    eOperationVersion = eWinME;
                    ///printf ("Microsoft Windows Millennium Edition\n");
                }
                break;

            case VER_PLATFORM_WIN32s:
            {
                eOperationVersion = eWin32;
                ///printf ("Microsoft Win32s\n");
            }
            break;
            }
            return eOperationVersion;
        }

        /** 获取用户MyDocument目录
        采用windows api SHGetSpecialFolderPath来获取
        */
        std::wstring GetMyDocumentDir(void)
        {
            std::wstring strMyDocumentPath;

            ///获取用户对应的 application data 目录,例:C:\Documents and Settings\wenhm\My Documents
            WCHAR strPath[MAX_PATH] = { 0 };
            BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_PERSONAL, FALSE);
            if (TRUE == bResult)
            {
                strMyDocumentPath = strPath;
            }

            return strMyDocumentPath;
        }

        /** 获取用户Application Data目录
        采用windows api SHGetSpecialFolderPath来获取
        */
        std::wstring GetAppDataDir(void)
        {
            std::wstring strAppDataPath;

            ///获取用户对应的 application data 目录,例:C:\Documents and Settings\username\Application Data
            WCHAR strPath[MAX_PATH] = { 0 };
            BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_APPDATA, FALSE);

            if (TRUE == bResult)
            {
                strAppDataPath = strPath;
            }

            return strAppDataPath;
        }

        /** 获取所有用户的Application Data目录
        采用windows api SHGetSpecialFolderPath来获取
        */
        std::wstring GetCommonAppDataDir(void)
        {
            std::wstring strAppDataPath;

            /**获取the file system directory containing application data for all users目录,
            例:C:\Documents and Settings\All Users.WINDOWS\Application Data
            */
            WCHAR strPath[MAX_PATH] = { 0 };
            BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_COMMON_APPDATA, FALSE);

            if (TRUE == bResult)
            {
                strAppDataPath = strPath;
            }

            return strAppDataPath;
        }

        std::wstring GetModulePath()
        {
            std::wstring strModulePath;
            WCHAR szModulePath[MAX_PATH + 1] = { 0 };
            std::wstring::size_type index;

            /*DWORD nSize =  */::GetModuleFileNameW(NULL, szModulePath, MAX_PATH);
            strModulePath = szModulePath;

            index = strModulePath.rfind(TEXT_SLASH);
            if (index != std::wstring::npos)
            {
                strModulePath = strModulePath.substr(0, index) + (TEXT_SLASH);
            }

            return strModulePath;
        }

        void AdjustFilePathSlash(std::wstring & strPath)
        {
            std::wstring::size_type iPos = std::wstring::npos;

            StringUtils_Replace(strPath, L"/", L"\\");
            do
            {
                StringUtils_Replace(strPath, L"\\\\", L"\\");
                iPos = strPath.find(L"\\\\");
            } while (std::wstring::npos != iPos);
        }

        void AdjustFilePathSlash(std::string & strPath)
        {
            std::string::size_type iPos = std::string::npos;

            StringUtils_Replace(strPath, "/", "\\");
            do
            {
                StringUtils_Replace(strPath, "\\\\", "\\");
                iPos = strPath.find("\\\\");
            } while (std::string::npos != iPos);
        }

        /** @fn GetAppLogParentDir
        @brief 获取存放应用程序LOG数据的Parent目录
        @details
            1.若是vista以上版本则获取操作系统获取Document 下的那个目录(类似C:\Documents and Settings\wenhm\Application Data\51\Rainbow)
            2.其它的全部建相应目录在exe当前目录下.
        */
        std::wstring GetAppLogParentDir()
        {
            std::wstring strAppLogParentDir;
            {
                //////////////////////////////////////////////////////////////////////////
                ///1.Vista 操作系统获取Document 下的那个目录(类似C:\Documents and Settings\wenhm\Application Data\51\Rainbow)
                ///2.其它的全部建相应目录在exe当前目录下.

#if 0///暂时去掉不同操作系统下存放Log目录不一致的功能
                EOperationVersion eOperationVersion = GetOperationVersion();
                if (eOperationVersion < eWinVista)
                {
                    strAppLogParentDir = GetModulePath();
                }
                else
#endif
                {
                    /*
                    例子:C:\Documents and Settings\wenhm\Application Data\51\Rainbow
                    strAppDataPath = CUtility::GetAppLogParentDir();
                    例子:C:\Documents and Settings\wenhm\My Documents\51\Rainbow
                    */
                    ///strAppLogParentDir = GetMyDocumentDir();						
                    ///strAppLogParentDir = GetAppDataDir();						
                    strAppLogParentDir = GetCommonAppDataDir();

                    ///设置应用程序对应目录
                    if (strlen(GetLogAppName()) > 0)
                    {
                        strAppLogParentDir += TEXT_SLASH;
                        strAppLogParentDir += AsciiToUnicode(GetLogAppName());
                    }
                }

                //strAppDataPath += TEXT_SLASH;
                //strAppDataPath += LOG_DIR;
            }

            AdjustFilePathSlash(strAppLogParentDir);

            return strAppLogParentDir;
        }

        extern char *GetLogPath();
        std::wstring GetLogCfgFilePath()
        {
            std::wstring strLogCfgFilePath;
            if (strlen(GetLogPath()) == 0)
            {
                strLogCfgFilePath = GetAppLogParentDir();
                strLogCfgFilePath += TEXT_SLASH;
                strLogCfgFilePath += LOG_DIR;
            }

            ///创建多级目录
            CreateMultipleDirectory(strLogCfgFilePath);

            return strLogCfgFilePath;
        }

        std::wstring GetDefaultLogParentPath()
        {
            std::wstring strLogParentPath = GetAppLogParentDir();

            ///创建多级目录
            CreateMultipleDirectory(strLogParentPath);
            return strLogParentPath;
        }

        bool RemoveMultipleDirectory(const std::wstring& strPath)
        {
            PAssert_ReturnValue(!strPath.empty(), false);

            //存放要删除的目录字符串
            std::wstring strDir(strPath);

            //确保以'\'结尾以删除最后一个目录
            if (strDir[strDir.length() - 1] != (L'\\'))
            {
                strDir += L'\\';
            }

            //存放每一层目录字符串
            std::vector<std::wstring> vecPath;
            //一个临时变量,存放目录字符串
            std::wstring strTemp;
            //成功标志
            bool bSuccess = false;

            //遍历要删除的字符串
            for (UINT i = 0; i < strDir.length(); ++i)
            {
                if (strDir[i] != L'\\')
                {
                    //如果当前字符不是'\\'
                    strTemp += strDir[i];
                }
                else
                {
                    //如果当前字符是'\\',将当前层的字符串添加到数组中
                    vecPath.push_back(strTemp);
                    strTemp += strDir[i];
                }
            }

            //遍历存放目录的数组,删除每层目录,从最深的目录开始删除,进行逆向访问
            std::vector<std::wstring>::const_reverse_iterator iter;
            for (iter = vecPath.rbegin(); iter != vecPath.rend(); iter++)
            {
                //如果RemoveDirectory执行成功,返回true,否则返回false
                ///bool bResult = iter->find('\\');
                std::wstring::size_type iFindPos = iter->find(L'\\');
                if (std::wstring::npos != iFindPos)
                {
                    bSuccess = RemoveDirectoryW(iter->c_str()) ? true : false;
                }
            }

            return bSuccess;
        }
    };
}

/** @fn GetModuleFilePathA
@brief 获取当前执行文件所在的路径
@param [out] char *strPath 存放路径的缓冲区
@param [in] int iSize 缓冲区大小
@return int,返回路径字符串的长度
*/
static int GetModuleFilePathA(char *strPath, int iSize)
{
    int iPathLen = 0;
    const char * ptrStrSlashPos = NULL;

#ifdef WIN32
    DWORD dwSize = 0;
    char strFileName[MAX_PATH + 1] = { 0 };
    PAssertNotNull_ReturnValue(strPath, 0);

    dwSize = ::GetModuleFileNameA(NULL, strFileName, MAX_PATH);

    ///开始从文件名尾部逆向解析出除去文件名后的路径来
    ptrStrSlashPos = strrchr(strFileName, '/');
    if (NULL == ptrStrSlashPos)
    {
        ptrStrSlashPos = strrchr(strFileName, '\\');
    }

    if (NULL != ptrStrSlashPos)
    {
        iPathLen = ptrStrSlashPos - strFileName;
        if (iSize > iPathLen)
        {
            strncpy(strPath, strFileName, iPathLen);
        }
        else
        {
            strncpy(strPath, strFileName, iSize);
        }
    }
#endif

    return iPathLen;
}

/** @fn CreateMultipleDirectory
@brief 创建多级目录
@param [out] char *strPath 存放要创建目录的路径
@return int,返回路径字符串的长度
*/
static int CreateMultipleDirectory(const char* strPath)
{
#ifdef WIN32
    //成功标志
    BOOL bSuccess = FALSE;
    char aStrSubPath[64][MAX_PATH + 1] = { 0 };
    int iSubPathNum = 0;
    char strPathLocal[MAX_PATH + 1] = { 0 };
    char strTemp[MAX_PATH + 1] = { 0 };
    char strTemp2[MAX_PATH + 1] = { 0 };
    PAssertNotNull_ReturnValue(strPath, FALSE);

    //存放要创建的目录字符串
    strcpy(strPathLocal, strPath);

    //确保以'\'结尾以创建最后一个目录
    if (strPathLocal[strlen(strPathLocal) - 1] != ('\\'))
    {
        strcat_s(strPathLocal, sizeof(strPathLocal), "\\");
    }

    //遍历要创建的字符串
    for (int i = 0; i < strlen(strPathLocal); ++i)
    {
        memset(strTemp2, 0, sizeof(strTemp2));
        strTemp2[0] = strPathLocal[i];

        if (strPathLocal[i] != '\\')
        {
            //如果当前字符不是'\\'
            strcat_s(strTemp, sizeof(strTemp), strTemp2);
        }
        else
        {
            //如果当前字符是'\\',将当前层的字符串添加到数组中
            strcpy(aStrSubPath[iSubPathNum], strTemp);
            iSubPathNum++;
            strcat_s(strTemp, sizeof(strTemp), strTemp2);
        }
    }

    //遍历存放目录的数组,创建每层目录
    for (int i = 0; i < iSubPathNum; i++)
    {
        memset(strTemp, 0, sizeof(strTemp));
        strcpy(strTemp, aStrSubPath[i]);
        //如果CreateDirectory执行成功,返回true,否则返回false
        bSuccess = CreateDirectoryA(strTemp, NULL) ? true : false;
    }
#endif

    return bSuccess;
}

bool CreateMultipleDirectory(const std::wstring& strPath)
{
    PAssert_ReturnValue(!strPath.empty(), false);

    //存放要创建的目录字符串
    std::wstring strDir(strPath);

    //确保以'\'结尾以创建最后一个目录
    if (strDir[strDir.length() - 1] != (L'\\'))
    {
        strDir += L'\\';
    }

    //存放每一层目录字符串
    std::vector<std::wstring> vecPath;
    //一个临时变量,存放目录字符串
    std::wstring strTemp;
    //成功标志
    bool bSuccess = false;

    //遍历要创建的字符串
    for (UINT i = 0; i < strDir.length(); ++i)
    {
        if (strDir[i] != L'\\')
        {
            //如果当前字符不是'\\'
            strTemp += strDir[i];
        }
        else
        {
            //如果当前字符是'\\',将当前层的字符串添加到数组中
            vecPath.push_back(strTemp);
            strTemp += strDir[i];
        }
    }

    //遍历存放目录的数组,创建每层目录
    std::vector<std::wstring>::const_iterator iter;
    for (iter = vecPath.begin(); iter != vecPath.end(); iter++)
    {
        //如果CreateDirectory执行成功,返回true,否则返回false
        bSuccess = CreateDirectoryW(iter->c_str(), NULL) ? true : false;
    }

    return bSuccess;
}

/** @fn IsCurDirModuleFileDirSame
@brief 检测当前目录和exe所处目录是不是相同
@return BOOL.
@retval TRUE 相同
@retval FALSE 不相同
*/
static BOOL IsCurDirModuleFileDirSame()
{
    BOOL bResult = TRUE;

#ifdef WIN32
    DWORD dwSize = 0;
    char strModuleFilePath[MAX_PATH + 1] = { 0 };
    int iModuleFilePathLen = 0;
    char strCurrentPath[MAX_PATH + 1] = { 0 };
    int iResult = 0;

    iModuleFilePathLen = GetModuleFilePathA(strModuleFilePath, sizeof(strModuleFilePath));

    dwSize = GetCurrentDirectoryA(MAX_PATH, strCurrentPath);

    iResult = stricmp(strCurrentPath, strModuleFilePath);

    bResult = (iResult == 0 ? TRUE : FALSE);
#endif

    return bResult;
}

/** @fn SetModuleFilePathAsCurrentDirectoryA
@brief 设置当前exe所在路径为当前路径
@return void.
*/
void SetModuleFilePathAsCurrentDirectoryA()
{
    DWORD dwSize = 0;
    char strModuleFilePath[MAX_PATH + 1] = { 0 };
    int iModuleFilePathLen = 0;
    char strCurrentPath[MAX_PATH + 1] = { 0 };
    int iResult = 0;

    iModuleFilePathLen = GetModuleFilePathA(strModuleFilePath, sizeof(strModuleFilePath));

    SetCurrentDirectoryA(strModuleFilePath);
}

/** @fn GetDefaultLogPath
@brief 获取当前日志文件存放的缺省路径
@param [out] char *strLogPath 存放路径的缓冲区
@param [in] int iSize 缓冲区大小
@return int,返回路径字符串的长度
*/
static int GetDefaultLogPath(char *strLogPath, int iSize)
{
    int iLogPathLen = 0;
    const char * ptrStrSlashPos = NULL;
    BOOL bIsCurDirModuleFileDirSame = FALSE;

#ifdef WIN32
    DWORD dwSize = 0;
    char strLogParentPath[MAX_PATH + 1] = { 0 };
    int iLogParentPathLen = 0;
    PAssertNotNull_ReturnValue(strLogPath, 0);
#if 1
    std::string strLogParentPathLocal = NSLog::UnicodeToAscii(NSLog::NSCommonUtils::GetDefaultLogParentPath());
    snprintf(strLogParentPath, iSize, "%s", strLogParentPathLocal.c_str());
    iLogParentPathLen = strLogParentPathLocal.length();
#else
    iLogParentPathLen = GetModuleFilePathA(strLogParentPath, sizeof(strLogParentPath));
#endif
    bIsCurDirModuleFileDirSame = IsCurDirModuleFileDirSame();
    if (FALSE == bIsCurDirModuleFileDirSame)
    {
        if (strlen(strLogParentPath) > 0)
        {
            snprintf(strLogPath, iSize, "%s\\%s", strLogParentPath, "Log");
        }
        else
        {
            snprintf(strLogPath, iSize, "%s\\", "Log");
        }
    }
    else
    {
        snprintf(strLogPath, iSize, "%s\\%s", strLogParentPath, "Log");
    }
#endif
    ///调整路径中的多余分隔符
    std::string strLogPathLocal = strLogPath;
    AdjustFilePathSlash(strLogPathLocal);
    snprintf(strLogPath, iSize, "%s", strLogPathLocal.c_str());

    iLogPathLen = strlen(strLogPath);

    return iLogPathLen;
}

/** @fn IsLogPathInModulePath
@brief 检测Log目录是否在exe所在目录下
@return bool.
*/
bool IsLogPathInModulePath(const std::string strLogPath)
{
    bool bResult = false;
    std::string strModulePath = NSLog::UnicodeToAscii(NSLog::NSCommonUtils::GetModulePath());
    std::string::size_type iPos = strLogPath.find(strModulePath);
    if (std::string::npos != iPos)
    {
        bResult = true;
    }

    return bResult;
}

int Unicode2Ansi(
    const WCHAR *strSrc,
    const UINT &uiSrcLen,	///size in words
    char *strDst,
    const UINT &uiDstLen	///size in byte
)
{
    ///输入参数有效性检查
    PAssertNotNull_Statement_ReturnValue(strSrc, assert(0), RESULT_FAILURE);
    PAssertNotNull_Statement_ReturnValue(strDst, assert(0), RESULT_FAILURE);

    ///清零先
    memset(strDst, 0, uiDstLen);

    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK,
        strSrc, uiSrcLen, strDst, uiDstLen, NULL, NULL);

    return RESULT_OK;
}

int Unicode2AnsiString(
    const WCHAR *strSrc,
    const UINT &uiSrcLen,	///size in words
    std::string &strDst
)
{
    ///输入参数有效性检查
    PAssertNotNull_Statement_ReturnValue(strSrc, assert(0), RESULT_FAILURE);

    ///unicode一个字符转换为UTF8时最多可能转换为6个字节,这里虽然转换为Ansi但也采用6个字节
    UINT uiDstLen = 6 * uiSrcLen + 1;
    char *ptrBuffer = new char[uiDstLen];
    memset(ptrBuffer, 0, uiDstLen);

    Unicode2Ansi(strSrc, uiSrcLen, ptrBuffer, uiDstLen);

    strDst = ptrBuffer;

    DELETEA(ptrBuffer);

    return RESULT_OK;
}

namespace NSLog
{
    //unicode=>ansi
    std::string UnicodeToAscii(const std::wstring& wide)
    {
        int nNeedChars = WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, 0, 0, 0, 0);
        if (nNeedChars > 0)//再次判断一下
        {
            std::vector<char> temp(nNeedChars);
            ::WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, &temp[0], nNeedChars, 0, 0);
            return std::string(&temp[0]);
        }

        return std::string();
    }

    std::wstring AsciiToUnicode(const std::string& in_str)
    {
        int nNeedWchars = MultiByteToWideChar(CP_ACP, 0, in_str.c_str(), -1, NULL, 0);
        if (nNeedWchars > 0)
        {
            std::vector<wchar_t> temp(nNeedWchars);
            ::MultiByteToWideChar(CP_ACP, 0, in_str.c_str(), -1, &temp[0], nNeedWchars);
            return std::wstring(&temp[0]);
        }

        return std::wstring();
    }

    std::wstring UTF8ToUnicode(const std::string& in_utf8Str)
    {
        int nNeedWchars = MultiByteToWideChar(CP_UTF8, 0, in_utf8Str.c_str(), -1, NULL, 0);
        if (nNeedWchars > 0)
        {
            std::vector<wchar_t> temp(nNeedWchars);
            ::MultiByteToWideChar(CP_UTF8, 0, in_utf8Str.c_str(), -1, &temp[0], nNeedWchars);
            return std::wstring(&temp[0]);
        }

        return std::wstring();
    }
}

void StringUtils_Replace(
    std::string &str,
    const std::string &strSrc,
    const std::string &strDst
)
{
    std::wstring::size_type iPos = 0;

    while ((iPos = str.find(strSrc, iPos)) != std::string::npos)
    {
        str.replace(iPos, strSrc.size(), strDst);
        iPos += strDst.size();
    }
}

void StringUtils_Replace(
    std::wstring &str,
    const std::wstring& strSrc,
    const std::wstring& strDst
)
{
    std::wstring::size_type iPos = 0;

    while ((iPos = str.find(strSrc, iPos)) != std::wstring::npos)
    {
        str.replace(iPos, strSrc.size(), strDst);
        iPos += strDst.size();
    }
}

void AdjustFilePathSlash(std::wstring & strPath)
{
    std::wstring::size_type iPos = std::wstring::npos;

    StringUtils_Replace(strPath, L"/", L"\\");
    do
    {
        StringUtils_Replace(strPath, L"\\\\", L"\\");
        iPos = strPath.find(L"\\\\");
    } while (std::wstring::npos != iPos);
}

void AdjustFilePathSlash(std::string & strPath)
{
    std::string::size_type iPos = std::string::npos;

    StringUtils_Replace(strPath, "/", "\\");
    do
    {
        StringUtils_Replace(strPath, "\\\\", "\\");
        iPos = strPath.find("\\\\");
    } while (std::string::npos != iPos);
}

/**
将大小为count的缓冲区中内容按字节以16进制字符串打印出来,
返回值即为指向相应的字符串，
该返回指向的存储区域要调用本函数的用户显示的进行删除
*/
char * sprintf_data(char *buff, int count)
{
    int i = 0;
    int j = 0;
    int c = 0;
    int printnext = 1;
    char str[10] = { 0 };///由于这里要进行格式输出如"%.4x "等，这里给出10个字符作为格式输出冗余
    //使用ostrstream的代码，但ostrstream.str()传出去要使用者负责清除ostrstream.str()指向的内存了
    std::ostrstream ostr;
    if (count)
    {
        if (count % 16)
        {
            c = count + (16 - count % 16);
        }
        else
        {
            c = count;
        }
    }
    else
    {
        c = count;
    }

    for (i = 0; i < c; i++)
    {
        if (printnext)
        {
            printnext--;
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
            sprintf_s(str, sizeof(str), "%.4x ", i & 0xffff);
#else
            snprintf(str, sizeof(str), "%.4x ", i & 0xffff);
#endif
#else
            snprintf(str, sizeof(str), "%.4x ", i & 0xffff);
#endif
            ostr << str;
        }

        if (i < count)
        {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
            sprintf_s(str, sizeof(str), "%3.2x", buff[i] & 0xff);
#else
            snprintf(str, sizeof(str), "%3.2x", buff[i] & 0xff);
#endif
#else
            snprintf(str, sizeof(str), "%3.2x", buff[i] & 0xff);
#endif
            ostr << str;
        }
        else
        {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
            sprintf_s(str, sizeof(str), "   ");
#else
            snprintf(str, sizeof(str), "   ");
#endif
#else
            snprintf(str, sizeof(str), "   ");
#endif
            ostr << str;
        }

        if (!((i + 1) % 8))
        {
            if ((i + 1) % 16)
            {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                sprintf_s(str, sizeof(str), " -");
#else
                snprintf(str, sizeof(str), " -");
#endif
#else
                snprintf(str, sizeof(str), " -");
#endif
                ostr << str;
            }
            else
            {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                sprintf_s(str, sizeof(str), "   ");
#else
                snprintf(str, sizeof(str), "   ");
#endif
#else
                snprintf(str, sizeof(str), "   ");
#endif
                ostr << str;
                for (j = i - 15; j <= i; j++)
                {
                    if (j < count)
                    {
                        if ((buff[j] & 0xff) >= 0x20 &&
                            (buff[j] & 0xff) <= 0x7e)
                        {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                            sprintf_s(str, sizeof(str), "%c", buff[j] & 0xff);
#else
                            snprintf(str, sizeof(str), "%c", buff[j] & 0xff);
#endif
#else
                            snprintf(str, sizeof(str), "%c", buff[j] & 0xff);
#endif
                            ostr << str;
                        }
                        else
                        {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                            sprintf_s(str, sizeof(str), ".");
#else
                            snprintf(str, sizeof(str), ".");
#endif
#else
                            snprintf(str, sizeof(str), ".");
#endif
                            ostr << str;
                        }
                    }
                    else
                    {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                        sprintf_s(str, sizeof(str), " ");
#else
                        snprintf(str, sizeof(str), " ");
#endif
#else
                        snprintf(str, sizeof(str), " ");
#endif
                        ostr << str;
                    }
                }
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                sprintf_s(str, sizeof(str), "\n");
#else
                snprintf(str, sizeof(str), "\n");
#endif
#else
                snprintf(str, sizeof(str), "\n");
#endif
                ostr << str;
                printnext = 1;
            }
        }
    }


    /**
    ostrstreams 没有为我们插入一般在字符数组末尾所需要的零终止符，
    当我们准备好零终止符时，用特别操作算子ends。
    */
    ostr << std::ends;
    return ostr.str();
    /*
    strncpy(str, ostr.str(), count );
    str[count ] = '\0';
    ostr.rdbuf()->freeze(0);
    */
}

//将大小为count的缓冲区中内容按字节以16进制字符串打印出来,str缓冲区的大小应大于等于count+1,因为还要一个字节存放字符串结束符
void sprintf_data(char *str, char *buff, int count)
{
    char *ptrStr = sprintf_data(buff, count);

#if _MSC_VER >= 1400 ///vs 2005
    strncpy_s(str, count, ptrStr, count);
#else
    strncpy(str, ptrStr, count);
#endif
    str[count] = '\0';
    DELETEA(ptrStr);
}

std::string GetFileNameByStripPath(const std::string &strFilePath)
{
    ///下面从带路径的文件名解析出不带路径的纯文件名
    char strMsgBuffer[LOG4C_BUFFER_SIZE_MAX] = { 0 };
    const char * strFileName = strrchr(strFilePath.c_str(), '/');

    if (strFileName != NULL)
    {
        strFileName++;
    }
    else
    {
        strFileName = strrchr(strFilePath.c_str(), '\\');
        if (strFileName != NULL)
        {
            strFileName++;
        }
        else
        {
            strFileName = strFilePath.c_str();
        }
    }

    return strFileName;
}

///const  char *转换为 log4cplus::tstring
const log4cplus::tstring ToLog4cplus_TString(const char * str)
{
    log4cplus::tstring strReturn;
#ifdef UNICODE
    strReturn = NSLog::AsciiToUnicode(str);
#else
    strReturn = str;
#endif
    return strReturn;
}

/************** End of CommonUtils.c ************************************/


/************** Begin of log.c *******************************************/
/*
* log.cpp
*
* 日志相关代码的定义文件
*/

#ifdef HAVE_CONFIG_H
///#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#ifdef  HAVE_ALLOCA_H
#include <alloca.h>
#endif
///#include <sd/sprintf.h>
///#include <sd/malloc.h>
///#include <sd/factory.h>
///#include <log4c/appender.h>
///#include <log4c/priority.h>
///#include <log4c/logging_event.h>
///#include <log4c/category.h>
///#include <log4c/rc.h>
///#include <sd/error.h>
///#include <sd/sd_xplatform.h>
///#include <sd/sd_xplatform.h>
///#include "../vos/include/vos.h"
///#include "log.h"
///#include "GlobalMacro.h"

///下面括起来代码和log_check拷贝自init.c
/*
* Currently, all MS C compilers for Win32 platforms default to 8 byte
* alignment.
*/
#pragma pack(push,_CRT_PACKING)

typedef struct log_c_rcfile
{
    char name[256];
    time_t ctime;
    int    exists;
} log_c_rcfile_t;

#pragma pack(pop)

static log_c_rcfile_t log_c_rcfiles[] =
{
#if 1
    { "$LOG4C_RCPATH/log4crc" },
    {  "$HOME/.log4crc" },
#endif 
    { "./log4crc" }
};

static const int log_c_nrcfiles = sizeof(log_c_rcfiles) / sizeof(log_c_rcfiles[0]);

///static SEM_ID gs_semphore = NULL;
static int gs_iModuleUsage_shmid = 0;

/** 日志模块初始化
@return int:return 0 for success
*/
int log_init()
{
    ///设置两个环境变量SD_DEBUG和SD_ERROR,使sd_debug和sd_error两个函数起作用
    ///putenv("SD_DEBUG=SD_DEBUG");
    ///putenv("SD_ERROR=SD_ERROR");
    log_check();
    return(log4c_init());
}

/** 日志模块初始化,指定配置文件名称
@return int:return 0 for success
*/
int log_init_with_cfg_file(const char *strCfgFileName)
{
    log_check_with_cfg_file(strCfgFileName);
    return(log4c_init_with_cfg_file(strCfgFileName));
}

///函数前置声明
extern void ReleaseModuleUsage();

/** 日志模块清理
@return int:return 0 for success
*/
int log_fini()
{
    int iResult = log4c_fini();

    // 解决资源泄漏
    ///VOS_DELETE_SEM(gs_semphore);
    ReleaseModuleUsage();

    return iResult;
}

/**检测配置文件是否存在
@return const int : iLogLevel.
*/
const int log_check(void)
{
    int iReturn = 0;
    BOOL bCfgFileExist = FALSE;
    int i = 0;

    printf("check configuration files...\n");

    /* check configuration files */
    printf("looking for conf files...\n");

#if 1
    snprintf(log_c_rcfiles[0].name, sizeof(log_c_rcfiles[0].name) - 1, "%s/log4crc",
        getenv("LOG4C_RCPATH") ? getenv("LOG4C_RCPATH") : LOG4C_RCPATH);
    snprintf(log_c_rcfiles[1].name, sizeof(log_c_rcfiles[1].name) - 1, "%s/.log4crc",
        getenv("HOME") ? getenv("HOME") : "");
#endif

    for (i = 0; i < log_c_nrcfiles; i++)
    {
        printf("checking for conf file at '%s'\n", log_c_rcfiles[i].name);
        if (SD_ACCESS_READ(log_c_rcfiles[i].name))
        {
            continue;
        }
        if (SD_STAT_CTIME(log_c_rcfiles[i].name, &log_c_rcfiles[i].ctime) != 0)
        {
            sd_error("sd_stat_ctime %s failed", log_c_rcfiles[i].name);
        }
        log_c_rcfiles[i].exists = 1;
        bCfgFileExist = TRUE;

#if 0
        if (log4c_load(log_c_rcfiles[i].name) == -1)
        {
            sd_error("loading %s failed", log_c_rcfiles[i].name);
            iReturn = -1;
        }
        else
        {
            printf("loading %s succeeded\n", log_c_rcfiles[i].name);
            break;
        }
#endif

    }


    if (TRUE == bCfgFileExist)
    {
        printf("find configuration file\n");
    }
    else
    {
        printf("can't find configuration file,\nplease put you conf file in the same dir of execute file.\n");
    }

    return iReturn;
}

/**检测配置文件是否存在,只检测传入的配置文件名
@return const int : iLogLevel.
*/
const int log_check_with_cfg_file(const char *strCfgFileName)
{
    int iReturn = 0;
    BOOL bCfgFileExist = FALSE;
    int i = 0;

    printf("check configuration files...\n");

    /* check configuration files */
    printf("looking for conf files...\n");

#if 1
    snprintf(log_c_rcfiles[0].name, sizeof(log_c_rcfiles[0].name) - 1, "%s/log4crc",
        getenv("LOG4C_RCPATH") ? getenv("LOG4C_RCPATH") : LOG4C_RCPATH);
    snprintf(log_c_rcfiles[1].name, sizeof(log_c_rcfiles[1].name) - 1, "%s/.log4crc",
        getenv("HOME") ? getenv("HOME") : "");
#endif

    if (NULL == strCfgFileName)
    {
        iReturn = -1;
    }
    else
    {
        printf("checking for conf file at '%s'\n", strCfgFileName);
        if (SD_ACCESS_READ(strCfgFileName))
        {
            iReturn = -1;
        }
        else
        {
            if (SD_STAT_CTIME(strCfgFileName, &log_c_rcfiles[i].ctime) != 0)
            {
                sd_error("sd_stat_ctime %s failed", strCfgFileName);
            }

            bCfgFileExist = TRUE;
#if 0
            if (log4c_load(strCfgFileName) == -1)
            {
                sd_error("loading %s failed", strCfgFileName);
                iReturn = -1;
            }
            else
            {
                printf("loading %s succeeded\n", strCfgFileName);
            }
#endif
        }
    }

    if (TRUE == bCfgFileExist)
    {
        printf("find configuration file\n");
    }
    else
    {
        printf("can't find configuration file,\nplease put you conf file in the same dir of execute file.\n");
    }

    return iReturn;
}

/** 日志记录
日志记录为一个字符串指针指向的内容
@return void
*/
void log_msg(
    const char *strFile,				///文件名
    const int iLineNum,					///行号
    ///const log4cplus::Logger &loggerObj,	///Logger对象引用
    const char *strCatName,				///category名
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
)
{
    do
    {
        const log4cplus::Logger &loggerObj = CLogWrapper::GetDefaultLogger();
        if ((loggerObj).isEnabledFor(iLogLevel))
        {
            log4cplus::tostringstream _log4cplus_buf;
            _log4cplus_buf << strFormat;

            ///去除文件名路径获取纯文件名
            std::string strFileName = GetFileNameByStripPath(strFile);

            (loggerObj).forcedLog(
                iLogLevel,
                _log4cplus_buf.str(),
                strFileName.c_str(),
                iLineNum
            );
        }
    } while (0);
}

/** 日志记录,不记录文件名和行号
日志记录为一个字符串指针指向的内容
@return void
*/
void log_msg_no_file_num(
    ///const log4cplus::Logger &loggerObj,	///Logger对象引用
    const char *strCatName,				///category名
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
)
{
    do
    {
        const log4cplus::Logger &loggerObj = CLogWrapper::GetDefaultLogger();
        if ((loggerObj).isEnabledFor(iLogLevel))
        {
            log4cplus::tostringstream _log4cplus_buf;
            _log4cplus_buf << strFormat;

            (loggerObj).forcedLog(
                iLogLevel,
                _log4cplus_buf.str()
            );

            ///这里更改Layout使输出的记录中没有文件名和行号
            ///输出日志后将原先的Layout再恢复即可		
            ///经测试上述两点无法做到
        }
    } while (0);
}

/** 日志记录,不记录文件名和行号,没有任何layout转换，直接输出相应的字符文本到日志中
此条记录没有行号，也没有线程号，也没有回车等
日志记录为一个字符串指针指向的内容
@return void
*/
void log_msg_no_file_num_no_layout(
    ///const log4cplus::Logger &loggerObj,	///Logger对象引用
    const char *strCatName,				///category名
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
)
{
    do
    {
        const log4cplus::Logger &loggerObj = CLogWrapper::GetDefaultLogger();
        if ((loggerObj).isEnabledFor(iLogLevel))
        {
            log4cplus::tostringstream _log4cplus_buf;
            _log4cplus_buf << strFormat;

            (loggerObj).forcedLog(
                iLogLevel,
                _log4cplus_buf.str()
            );

            ///这里更改Layout使输出的记录中没有文件名和行号
            ///输出日志后将原先的Layout再恢复即可		
            ///经测试上述两点无法做到
        }
    } while (0);
}

/** 日志记录
支持类似printf函数的带格式输出
@return void
*/
void log_log(
    ///const log4cplus::Logger &loggerObj,	///Logger对象引用
    const char *strCatName,				///category名
    const int iLogLevel,				///日志记录级别
    const char* strFormat,				///日志内容格式
    ...									///日志内容
)
{
    char * strMsg = NULL;
    va_list va;
    va_start(va, strFormat);
    {
        strMsg = sd_vsprintf(strFormat, va);
    }
    va_end(va);

    do
    {
        const log4cplus::Logger &loggerObj = CLogWrapper::GetDefaultLogger();
        if ((loggerObj).isEnabledFor(iLogLevel))
        {
            log4cplus::tostringstream _log4cplus_buf;
            _log4cplus_buf << strMsg;

            (loggerObj).forcedLog(
                iLogLevel,
                _log4cplus_buf.str()
            );

            ///这里更改Layout使输出的记录中没有文件名和行号
            ///输出日志后将原先的Layout再恢复即可		
            ///经测试上述两点无法做到
        }
    } while (0);

    ///释放动态分配内存
    FREEP(strMsg);
}

const struct ST_LogParam log_vsnprintf_wrapper(
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
)
{
    struct ST_LogParam st_LogParam = { 0 };
    va_list va;
    va_start(va, strFormat);
    {
        st_LogParam.strMsg = sd_vsprintf(strFormat, va);
    }
    va_end(va);

    st_LogParam.iLogLevel = iLogLevel;
    return st_LogParam;
}

const struct ST_LogParam log_condition_vsnprintf_wrapper(
    const int iCondition,	///条件
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
)
{
    struct ST_LogParam st_LogParam = { 0 };
    va_list va;
    va_start(va, strFormat);
    {
        st_LogParam.strMsg = sd_vsprintf(strFormat, va);
    }
    va_end(va);

    st_LogParam.iLogLevel = iLogLevel;
    st_LogParam.iCondition = iCondition;

    return st_LogParam;
}

const struct ST_LogParam log_hex_dump_vsnprintf_wrapper(
    const int iLogLevel,	///日志记录级别
    const char* strHexBuf,		///缓冲区首地址
    const int iHexBufLen,		///缓冲区长度
    ...						///日志内容
)
{
    struct ST_LogParam st_LogParam = { 0 };

    st_LogParam.iLogLevel = iLogLevel;
    st_LogParam.strHexBuf = (char *)strHexBuf;
    st_LogParam.iHexBufLen = iHexBufLen;

    return st_LogParam;
}

const struct ST_LogParam log_vsnprintf_wrapper_msg(
    const char* strFormat,	///日志内容格式
    ...						///日志内容
)
{
    struct ST_LogParam st_LogParam = { 0 };
    va_list va;
    va_start(va, strFormat);
    {
        st_LogParam.strMsg = sd_vsprintf(strFormat, va);
    }
    va_end(va);
    return st_LogParam;
}

const char * log_vsnprintf_wrapper_fun_msg(
    const char* strFunName,	///函数名
    const char* strFormat,	///日志内容格式
    ...						///日志内容
)
{
    char *ptrStrMsg = NULL;
    char *ptrStrMsg2 = NULL;

    struct ST_LogParam st_LogParam = { 0 };
    va_list va;
    va_start(va, strFormat);
    {
        ptrStrMsg = sd_vsprintf(strFormat, va);
    }
    va_end(va);

    ptrStrMsg2 = sd_sprintf("FUN<%s>:%s", strFunName, ptrStrMsg);

    free(ptrStrMsg);

    return ptrStrMsg2;
}

void log_msg_wrapper(
    const char *strFileName,///文件名
    const int iLineNum,		///行号
    const char *strLoggerName,	///Logger名
    const int iLogLevel,	///日志记录级别
    const char *strFormat,	///日志内容格式
    ...						///日志内容
)
{
#if 0
    const log4c_category_t* a_category = log4c_category_get(strLoggerName);
    if (log4c_category_is_priority_enabled(a_category, iLogLevel))
    {
        va_list va;
        va_start(va, strFormat);
        log4c_category_vlog(a_category, iLogLevel, strFormat, va);
        va_end(va);
    }
#endif
}

/**宏参数抽取format函数
本函数接受LOG_DEBUG(X)的参数，并从该宏定义的参数中返回format的值
@return const char* : strFormat
*/
const char* log_get_format_wrapper(
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
)
{
    return strFormat;
#if 0
    char* buffer = strdup(strFormat);
    return buffer;
#endif
}

/**宏参数抽取priority函数
本函数接受LOG_DEBUG(X)的参数，并从该宏定义的参数中返回priority的值
@return const int : iLogLevel.
*/
const int log_get_priority_wrapper(
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
)
{
    return iLogLevel;
}

/**
将大小为count的缓冲区中内容按字节以16进制字符串打印出来,
返回值即为指向相应的字符串，
该返回指向的存储区域要调用本函数的用户显示的进行删除
*/
char * log4c_sprintf_data(char *buff, int count)
{
    int i, j, c;
    int printnext = 1;
    char str[10] = { 0 };///由于这里要进行格式输出如"%.4x "等，这里给出10个字符作为格式输出冗余
    char *strBuffer = (char *)malloc(LOG4C_BUFFER_SIZE_MAX);
    memset(strBuffer, 0, LOG4C_BUFFER_SIZE_MAX);

    if (count)
    {
        if (count % 16)
        {
            c = count + (16 - count % 16);
        }
        else
        {
            c = count;
        }
    }
    else
    {
        c = count;
    }

    for (i = 0;i < c;i++)
    {
        if (printnext)
        {
            printnext--;
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
            sprintf_s(str, sizeof(str), "%.4x ", i & 0xffff);
            strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
            _snprintf(str, sizeof(str), "%.4x ", i & 0xffff);
            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
            snprintf(str, sizeof(str), "%.4x ", i & 0xffff);
            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
            ///ostr << str;
        }

        if (i < count)
        {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
            sprintf_s(str, sizeof(str), "%3.2x", buff[i] & 0xff);
            strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
            _snprintf(str, sizeof(str), "%3.2x", buff[i] & 0xff);
            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
            snprintf(str, sizeof(str), "%3.2x", buff[i] & 0xff);
            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
            ///ostr << str;
        }
        else
        {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
            sprintf_s(str, sizeof(str), "   ");
            strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
            _snprintf(str, sizeof(str), "   ");
            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
            snprintf(str, sizeof(str), "   ");
            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
            ///ostr << str;
        }

        if (!((i + 1) % 8))
        {
            if ((i + 1) % 16)
            {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                sprintf_s(str, sizeof(str), " -");
                strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
                _snprintf(str, sizeof(str), " -");
                strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
                snprintf(str, sizeof(str), " -");
                strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
                ///ostr << str;
            }
            else
            {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                sprintf_s(str, sizeof(str), "   ");
                strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
                _snprintf(str, sizeof(str), "   ");
                strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
                snprintf(str, sizeof(str), "   ");
                strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
                ///ostr << str;
                for (j = i - 15;j <= i;j++)
                {
                    if (j < count)
                    {
                        if ((buff[j] & 0xff) >= 0x20 &&
                            (buff[j] & 0xff) <= 0x7e)
                        {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                            sprintf_s(str, sizeof(str), "%c", buff[j] & 0xff);
                            strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
                            _snprintf(str, sizeof(str), "%c", buff[j] & 0xff);
                            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
                            snprintf(str, sizeof(str), "%c", buff[j] & 0xff);
                            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
                            ///ostr << str;
                        }
                        else
                        {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                            sprintf_s(str, sizeof(str), ".");
                            strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
                            _snprintf(str, sizeof(str), ".");
                            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
                            snprintf(str, sizeof(str), ".");
                            strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
                            ///ostr << str;
                        }
                    }
                    else
                    {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                        sprintf_s(str, sizeof(str), " ");
                        strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
                        _snprintf(str, sizeof(str), " ");
                        strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
                        snprintf(str, sizeof(str), " ");
                        strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
                        ///ostr << str;
                    }
                }
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
                sprintf_s(str, sizeof(str), "\n");
                strcat_s(strBuffer, LOG4C_BUFFER_SIZE_MAX, str);
#else
                _snprintf(str, sizeof(str), "\n");
                strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
#else
                snprintf(str, sizeof(str), "\n");
                strncat(strBuffer, str, LOG4C_BUFFER_SIZE_MAX);
#endif
                ///ostr << str; 
                printnext = 1;
            }
        }
    }

    return strBuffer;
}

//////////////////////////////////////////////////////////////////////////
static char gs_strFileContent_InC[] =
"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
"<!DOCTYPE log4c SYSTEM \"\">\n"
"\n"
"<log4c version=\"1.2.1\">\n"
"	<config>\n"
"		<bufsize>0</bufsize>\n"
"		<debug level=\"2\"/>\n"
"		<nocleanup>0</nocleanup>\n"
"		<reread>1</reread>\n"
"	</config>\n"
"\n"
"	<category name=\"root\" priority=\"notice\"  appender=\"aname\"/>\n"
"\n"
"	<rollingpolicy name=\"a_policy_name\" type=\"sizewin\" maxsize=\"1048576\" maxnum=\"15\" />\n"
"	<appender name=\"aname\" type=\"rollingfile\"  logdir=\".\" prefix=\"log\" layout=\"dated_threadid\" rollingpolicy=\"a_policy_name\" />\n"
"\n"
"	<appender name=\"stdout\" type=\"stream\" layout=\"dated_threadid\"/>\n"
"	<appender name=\"stderr\" type=\"stream\" layout=\"dated\"/>\n"
"	<appender name=\"syslog\" type=\"syslog\" layout=\"basic\"/>\n"
"</log4c>\n";

static char gs_strDefualtFileName[] = "log4crc";

int log_init_with_string(
    const char *strFileContent,
    const char *strFileName
)
{
    int iResult = RESULT_FAILURE;
    const char * strLocalFileContent = NULL;
    const char * strLocalFileName = NULL;

    PAssertNotNull_ReturnValue(strFileContent, RESULT_FAILURE);
    PAssertNotNull_ReturnValue(strFileName, RESULT_FAILURE);

    strLocalFileContent = (strlen(strFileContent) > 0 ? strFileContent : gs_strFileContent_InC);
    strLocalFileName = (strlen(strFileName) > 0 ? strFileName : gs_strDefualtFileName);

    /**
    若文件已存在则不重新生成配置文件了
    否则生成新的配置文件
    */
    if (RESULT_OK != SD_ACCESS_READ(strLocalFileName))
    {
        ///文件不存在新建
        FILE *file = fopen(strLocalFileName, "wb");
        if (NULL != file)
        {
            fwrite(strLocalFileContent, 1, strlen(strLocalFileContent), file);
            fclose(file);
        }
    }
    else
    {
        ///文件存在,do nothing，使用原来的配置文件就OK了。
    }

    ///调用Init函数进行实际的日志模块初始化工作
    iResult = log_init_with_cfg_file_wrapper(strLocalFileName);

    return iResult;
}

int log_init_with_cfg_file_wrapper(const char * strConfigFile)
{
    int iResult = RESULT_FAILURE;

    PAssertNotNull_ReturnValue(strConfigFile, RESULT_FAILURE);

    ///调用log4c的函数来初始化日志
    if (strlen(strConfigFile) > 0)
    {
        iResult = log_init_with_cfg_file(strConfigFile);
    }

    if (RESULT_OK != iResult)
    {
        iResult = log_init();
    }

    return iResult;
}

/*****************************新增的一些日志模块配置属性设置类****************/
/*
    * Currently, all MS C compilers for Win32 platforms default to 8 byte
    * alignment.
    */
#pragma pack(push,_CRT_PACKING)

#define MAX_FILE_NAME_LEN 1024
struct ST_LogProperty
{
    char m_strLogPath[MAX_FILE_NAME_LEN];
    char m_strLogCfgFileName[MAX_FILE_NAME_LEN];
    char m_strLogFileName[MAX_FILE_NAME_LEN];
    char m_strLogFileSize[128];
    char m_strLogLevel[128];
    int m_iLogFileNum;
    ///标志是否要重新读取配置文件
    BOOL m_bReReadLogCfgFile;
    ///重新读取配置文件的时间ms
    DWORD m_dwReReadMilliseconds;
    ///Logger名称，不同的dll可以用不同的Logger名称
    char m_strLoggerName[128];
    ///应用程序名字，主要用于定位或创建应用程序对应的log目录
    char m_strAppName[128];
    ///是否立刻Flush到文件中
    BOOL m_bImmediateFlush;
    ///文件缓冲区大小，m_bImmediateFlush为FALSE时起作用
    int m_iBufferSize;
};
typedef struct ST_LogProperty LogProperty;

#pragma pack(pop)

static LogProperty gs_st_LogProperty =
{
    "",
    "log4cplus_default.properites",
    "log.log",
    "1024KB",
    "INFO",
    15,
    FALSE,
    (60 * 1000),
    "LD",
    "CommonLog",
    TRUE,
    0
};

static char gs_strFileContentFormat[] =
"######################################################################## \n"
"#rootLogger \n"
"#	logger \n"
"#		appender \n"
"#			layout \n"
"#				ConversionPattern \n"
"#			filters \n"
"######################################################################## \n"
"log4cplus.rootLogger=TRACE, STDOUT \n"
"\n"
"######################################################################## \n"
"#Logger Default这里简写为LD \n"
"#这样日志记录中记录Logger名时可以节省日志文件大小 \n"
"######################################################################## \n"
"#log4cplus.logger.LD=TRACE, AppenderNameRollingFile, AppenderNameDefault \n"
"log4cplus.logger.LD=%s, AppenderNameRollingFile \n"
"log4cplus.additivity.LD=FALSE \n"
"\n"
"log4cplus.appender.STDOUT=log4cplus::ConsoleAppender \n"
"log4cplus.appender.STDOUT.layout=log4cplus::PatternLayout \n"
"log4cplus.appender.STDOUT.layout.ConversionPattern=%%D{%%m/%%d/%%y %%H:%%M:%%S} [%%t] %%-5p %%c - %%m %%n \n"
"log4cplus.appender.STDOUT.logToStdErr=true \n"
"log4cplus.appender.STDOUT.ImmediateFlush=true \n"
"\n"
"log4cplus.appender.AppenderNameRollingFile=log4cplus::RollingFileAppender \n"
"log4cplus.appender.AppenderNameRollingFile.File=%s\\%s \n"
"#log4cplus.appender.AppenderNameRollingFile.MaxFileSize=5MB \n"
"log4cplus.appender.AppenderNameRollingFile.MaxFileSize=%s \n"
"log4cplus.appender.AppenderNameRollingFile.MaxBackupIndex=%d \n"
"#log4cplus.appender.AppenderNameRollingFile.layout=log4cplus::TTCCLayout \n"
"log4cplus.appender.AppenderNameRollingFile.layout=log4cplus::PatternLayout \n"
"log4cplus.appender.AppenderNameRollingFile.layout.ConversionPattern=%%D{%%m/%%d/%%y %%H:%%M:%%S:%%q} [%%t] %%-5p %%c - %%F(%%L) %%m %%n \n"
"log4cplus.appender.AppenderNameRollingFile.ImmediateFlush=%s \n"
"log4cplus.appender.AppenderNameRollingFile.BufferSize=%d \n"
"#log4cplus.appender.AppenderNameRollingFile.Append=TRUE \n"
"#log4cplus.appender.AppenderNameRollingFile.Threshold=ERROR \n"
"#log4cplus.appender.AppenderNameRollingFile.ReopenDelay=5 \n"
"\n"
"\n"
"#log4cplus.appender.AppenderNameDefault=log4cplus::RollingFileAppender \n"
"#log4cplus.appender.AppenderNameDefault=log4cplus::FileAppender \n"
"#log4cplus.appender.AppenderNameDefault.File=test_output.log \n"
"#log4cplus.appender.AppenderNameDefault=log4cplus::NullAppender \n"
"#log4cplus.appender.AppenderNameDefault.layout=log4cplus::TTCCLayout \n"
"#log4cplus.appender.AppenderNameDefault.layout=log4cplus::PatternLayout \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%%D{%%y-%%m-%%d %%H:%%M:%%S,%%q} [%%t] %%-5p %%c <%%x> - %%m%%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%%D{%%y-%%m-%%d %%H:%%M:%%S,%%q} %%-5p %%c <%%x> - %%m%%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%%p - %%m%%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%%-5p - %%m%%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%%l - %%m%%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%%C.%%M.%%L - %%m%%n \n"
"\n";

static char gs_strFileContentBuffer[4096] = { 0 };

const char * GetLogAppName()
{
    return gs_st_LogProperty.m_strAppName;
}

BOOL GetReReadLogCfgFile()
{
    return gs_st_LogProperty.m_bReReadLogCfgFile;
}

DWORD GetReReadMilliseconds()
{
    return gs_st_LogProperty.m_dwReReadMilliseconds;
}

const char * GetLogPath()
{
    return gs_st_LogProperty.m_strLogPath;
}

void log_set_log_cfg_file_name(const char *strFileName)
{
    PAssertNotNull_Return(strFileName);
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
    strcpy_s(gs_st_LogProperty.m_strLogCfgFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName), strFileName);
#else
    strncpy(gs_st_LogProperty.m_strLogCfgFileName, strFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName));
#endif
#else
    strncpy(gs_st_LogProperty.m_strLogCfgFileName, strFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName));
#endif
}

void log_set_log_path(const char *strLogPath)
{
    PAssertNotNull_Return(strLogPath);
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
    strcpy_s(gs_st_LogProperty.m_strLogPath, sizeof(gs_st_LogProperty.m_strLogPath), strLogPath);
#else
    strncpy(gs_st_LogProperty.m_strLogPath, strLogPath, sizeof(gs_st_LogProperty.m_strLogPath));
#endif
#else
    strncpy(gs_st_LogProperty.m_strLogPath, strLogPath, sizeof(gs_st_LogProperty.m_strLogPath));
#endif
}

void log_set_log_file_name(const char *strFileName)
{
    PAssertNotNull_Return(strFileName);
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
    strcpy_s(gs_st_LogProperty.m_strLogFileName, sizeof(gs_st_LogProperty.m_strLogFileName), strFileName);
#else
    strncpy(gs_st_LogProperty.m_strLogFileName, strFileName, sizeof(gs_st_LogProperty.m_strLogFileName));
#endif
#else
    strncpy(gs_st_LogProperty.m_strLogFileName, strFileName, sizeof(gs_st_LogProperty.m_strLogFileName));
#endif
}

void log_set_log_level(const char *strLogLevel)
{
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
    strcpy_s(gs_st_LogProperty.m_strLogLevel,
        sizeof(gs_st_LogProperty.m_strLogLevel), strLogLevel);
#else
    strncpy(gs_st_LogProperty.m_strLogLevel,
        strLogLevel, sizeof(gs_st_LogProperty.m_strLogLevel));
#endif
#else
    strncpy(gs_st_LogProperty.m_strLogLevel, strLogLevel,
        sizeof(gs_st_LogProperty.m_strLogLevel));
#endif
}

void log_set_log_file_size(const char *strFileSize)
{
    strcpy_s(gs_st_LogProperty.m_strLogFileSize,
        sizeof(gs_st_LogProperty.m_strLogFileSize), strFileSize);
}

void log_set_log_file_num(const int iFileNum)
{
    gs_st_LogProperty.m_iLogFileNum = iFileNum;
}

void log_set_reread_log_cfg_file(const BOOL bReReadLogCfgFile)
{
    gs_st_LogProperty.m_bReReadLogCfgFile = bReReadLogCfgFile;
}

void log_set_reread_log_cfg_file_elapse(const DWORD& dwReReadMilliseconds)
{
    gs_st_LogProperty.m_dwReReadMilliseconds = dwReReadMilliseconds;
}

void log_set_log_logger_name(const char *strLoggerName)
{
    PAssertNotNull_Return(strLoggerName);
    strcpy_s(gs_st_LogProperty.m_strLoggerName, sizeof(gs_st_LogProperty.m_strLoggerName), strLoggerName);
}

void log_set_log_app_name(const char *strAppName)
{
    PAssertNotNull_Return(strAppName);
    strcpy_s(gs_st_LogProperty.m_strAppName, sizeof(gs_st_LogProperty.m_strAppName), strAppName);
}

void log_set_log_immediate_flush(const BOOL bImmediateFlush)
{
    gs_st_LogProperty.m_bImmediateFlush = bImmediateFlush;
}

void log_set_log_buffer_size(const int iBufferSize)
{
    gs_st_LogProperty.m_iBufferSize = iBufferSize;
}

int log_init_with_param()
{
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
    sprintf_s(gs_strFileContentBuffer, sizeof(gs_strFileContentBuffer),
        gs_strFileContentFormat,
        ///gs_st_LogProperty.m_bReReadLogCfgFile,
        gs_st_LogProperty.m_strLogLevel,
        gs_st_LogProperty.m_strLogPath,
        gs_st_LogProperty.m_strLogFileName,
        gs_st_LogProperty.m_strLogFileSize,
        gs_st_LogProperty.m_iLogFileNum,
        (gs_st_LogProperty.m_bImmediateFlush ? "true" : "false"),
        gs_st_LogProperty.m_iBufferSize
    );
#else
    _snprintf(gs_strFileContentBuffer, sizeof(gs_strFileContentBuffer),
        gs_strFileContentFormat,
        ///gs_st_LogProperty.m_bReReadLogCfgFile,
        gs_st_LogProperty.m_strLogLevel,
        gs_st_LogProperty.m_iLogFileSize,
        gs_st_LogProperty.m_iLogFileNum,
        gs_st_LogProperty.m_strLogPath,
        gs_st_LogProperty.m_strLogFileName
    );
#endif
#else
    snprintf(gs_strFileContentBuffer, sizeof(gs_strFileContentBuffer),
        gs_strFileContentFormat,
        gs_st_LogProperty.m_bReReadLogCfgFile,
        gs_st_LogProperty.m_strLogLevel,
        gs_st_LogProperty.m_iLogFileSize,
        gs_st_LogProperty.m_iLogFileNum,
        gs_st_LogProperty.m_strLogPath,
        gs_st_LogProperty.m_strLogFileName
    );
#endif

    /** 保证生成每个log 配置文件中的logger和appender name都不一样
    不然多个模块(dll)中同时使用logcplus的dll的方式会导致一样名字的logger的日志内容互相串掉
    让logger name和appender name都带上相应日志配置文件名称作为名字的一部分
    */
    std::string strLogCfgFileContent = gs_strFileContentBuffer;
    std::string strLoggerName = std::string("") + gs_st_LogProperty.m_strLoggerName;
    StringUtils_Replace(strLogCfgFileContent, "LD", strLoggerName);

    std::string strAppenderName = std::string("AppenderNameRollingFile_") + gs_st_LogProperty.m_strLoggerName;
    StringUtils_Replace(strLogCfgFileContent, "AppenderNameRollingFile", strAppenderName);
    strcpy_s(gs_strFileContentBuffer, sizeof(gs_strFileContentBuffer) - 1, strLogCfgFileContent.c_str());

    return log_init_with_string(gs_strFileContentBuffer, gs_st_LogProperty.m_strLogCfgFileName);
}
/************************************************************************/

/************************************************************************/
// Instruct the compiler to put the g_lModuleUsage data
// variable in its own data section, called Shared. We
// then instruct the linker that we want the data in this
// section to be shared by all instances of this application.
#ifdef WIN32
#pragma data_seg("LogShared")
/** 数据库日志模块运行的实例个数
避免多个日志模块的运行实例使用同一个配置文件，避免使用同一个log文件来记录日志而产生冲突
*/
long g_lLogModuleUsage = -1;
#pragma data_seg()
// Instruct the linker to make the Shared section 
// readable, writable, and shared.
#pragma comment(linker, "/section:LogShared,rws")
#else 
// long g_lLogModuleUsage __attribute__((section ("shared"), shared)) = -1;
// int g_lLogModuleUsage __attribute__((section ("shared"), shared)) = -1;
// extern long GetModuleUsage();
// long g_lLogModuleUsage = GetModuleUsage();
long g_lLogModuleUsage = -1;
#endif

int log_init_with_param_multi_process()
{
    char strLogCfgFileName[MAX_FILE_NAME_LEN] = { 0 };
    char strLogFileName[MAX_FILE_NAME_LEN] = { 0 };
    char strLogModuleUsageNum[MAX_FILE_NAME_LEN] = { 0 };
    BOOL bIsCurDirModuleFileDirSame = TRUE;
    int iLogPathLen = 0;

    IncreaseLogModuleUsage();

    if (1)
    {
#if 1
        if (strlen(gs_st_LogProperty.m_strLogPath) == 0)
        {
            iLogPathLen = GetDefaultLogPath(gs_st_LogProperty.m_strLogPath, sizeof(gs_st_LogProperty.m_strLogPath));

            if (true == IsLogPathInModulePath(gs_st_LogProperty.m_strLogPath))
            {
                ///设置当前exe所在路径为当前路径,防止log4cplus无法打开带中文的路径下日志配置文件和日志记录文件
                SetModuleFilePathAsCurrentDirectoryA();
                /************************************************************************/
                /*下面代码设定日志文件一直生成在exe所在目录的Log目录下，而不依赖工作目录     */
                /************************************************************************/
                snprintf(gs_st_LogProperty.m_strLogPath, sizeof(gs_st_LogProperty.m_strLogPath), ".\\%s", "Log");
            }
        }

        CreateMultipleDirectory(gs_st_LogProperty.m_strLogPath);
#else
        /************************************************************************/
        /*下面代码设定日志文件一直生成在exe所在目录的Log目录下，而不依赖工作目录     */
        /************************************************************************/
        iLogPathLen = GetDefaultLogPath(gs_st_LogProperty.m_strLogPath, sizeof(gs_st_LogProperty.m_strLogPath));
        CreateMultipleDirectory(gs_st_LogProperty.m_strLogPath);
#endif

#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
        sprintf_s(strLogCfgFileName, sizeof(strLogCfgFileName),
            "%s\\%s%d", gs_st_LogProperty.m_strLogPath, gs_st_LogProperty.m_strLogCfgFileName, g_lLogModuleUsage);
        strcpy_s(gs_st_LogProperty.m_strLogCfgFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName), strLogCfgFileName);

        sprintf_s(strLogFileName, sizeof(strLogFileName),
            "%s%d", gs_st_LogProperty.m_strLogFileName, g_lLogModuleUsage);
        strcpy_s(gs_st_LogProperty.m_strLogFileName, sizeof(gs_st_LogProperty.m_strLogFileName), strLogFileName);
#else
        _snprintf(strLogCfgFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName),
            "%s\\%s%d", gs_st_LogProperty.m_strLogPath, gs_st_LogProperty.m_strLogCfgFileName, g_lLogModuleUsage);
        strcpy(gs_st_LogProperty.m_strLogCfgFileName, strLogCfgFileName);

        _snprintf(strLogFileName, sizeof(gs_st_LogProperty.m_strLogFileName),
            "%s%d", gs_st_LogProperty.m_strLogFileName, g_lLogModuleUsage);
        strcpy(gs_st_LogProperty.m_strLogFileName, strLogFileName);
#endif
#else
        //snprintf(gs_st_LogProperty.m_strLogCfgFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName), 
        //	"%s%d", gs_st_LogProperty.m_strLogCfgFileName, g_lLogModuleUsage);
        //snprintf(gs_st_LogProperty.m_strLogFileName, sizeof(gs_st_LogProperty.m_strLogFileName), 
        //	"%s%d", gs_st_LogProperty.m_strLogFileName, g_lLogModuleUsage);
        snprintf(strLogModuleUsageNum, sizeof(strLogModuleUsageNum), "%d", g_lLogModuleUsage);
        strcat(gs_st_LogProperty.m_strLogCfgFileName, strLogModuleUsageNum);
        strcat(gs_st_LogProperty.m_strLogFileName, strLogModuleUsageNum);
#endif
    }
    else
    {
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
        sprintf_s(gs_st_LogProperty.m_strLogCfgFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName),
            "%s%d", gs_st_LogProperty.m_strLogCfgFileName, g_lLogModuleUsage);
        sprintf_s(gs_st_LogProperty.m_strLogFileName, sizeof(gs_st_LogProperty.m_strLogFileName),
            "%s%d", gs_st_LogProperty.m_strLogFileName, g_lLogModuleUsage);
#else
        _snprintf(gs_st_LogProperty.m_strLogCfgFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName),
            "%s%d", gs_st_LogProperty.m_strLogCfgFileName, g_lLogModuleUsage);
        _snprintf(gs_st_LogProperty.m_strLogFileName, sizeof(gs_st_LogProperty.m_strLogFileName),
            "%s%d", gs_st_LogProperty.m_strLogFileName, g_lLogModuleUsage);
#endif
#else
        //snprintf(gs_st_LogProperty.m_strLogCfgFileName, sizeof(gs_st_LogProperty.m_strLogCfgFileName), 
        //	"%s%d", gs_st_LogProperty.m_strLogCfgFileName, g_lLogModuleUsage);
        //snprintf(gs_st_LogProperty.m_strLogFileName, sizeof(gs_st_LogProperty.m_strLogFileName), 
        //	"%s%d", gs_st_LogProperty.m_strLogFileName, g_lLogModuleUsage);
        snprintf(strLogModuleUsageNum, sizeof(strLogModuleUsageNum), "%d", g_lLogModuleUsage);
        strcat(gs_st_LogProperty.m_strLogCfgFileName, strLogModuleUsageNum);
        strcat(gs_st_LogProperty.m_strLogFileName, strLogModuleUsageNum);
#endif
    }

    return log_init_with_param();
}

extern long GetModuleUsage();
extern void SetModuleUsage(long lModuleUsage);
void IncreaseLogModuleUsage()
{
#ifdef WIN32
    InterlockedIncrement((long*)&g_lLogModuleUsage);
#else
    g_lLogModuleUsage = GetModuleUsage();
    g_lLogModuleUsage++;
    SetModuleUsage(g_lLogModuleUsage);
#endif
}
/************************************************************************/


/************************************************************************/

/*
* Currently, all MS C compilers for Win32 platforms default to 8 byte
* alignment.
*/
#pragma pack(push,_CRT_PACKING)

///下面为LOG4C_BLOCK_XXX相关宏定义的实现代码
///采用了类C++的思想来写C的代码
///进行LOG4C_BLOCK_BEGIN和LOG4C_BLOCK_END宏定义使用的Block结构
struct ST_Block
{
#define BLOCK_FREE 0
#define BLOCK_BUSY 1
    ///标志是否被使用
    int iUseStatus;
    ///线程ID
    TASK_ID threadId;
    ///Block的层次
    int iBlockLevel;

    void(*Init)(struct ST_Block *ptrThis);
    void(*Reset)(struct ST_Block *ptrThis);

    int(*GetUseStatus)(struct ST_Block *ptrThis);
    TASK_ID(*GetThreadId)(struct ST_Block *ptrThis);
    int(*GetBlockLevel)(struct ST_Block *ptrThis);

    void(*SetUseStatus)(struct ST_Block *ptrThis, int iUseStatus);
    void(*SetThreadId)(struct ST_Block *ptrThis, TASK_ID threadId);
    void(*SetBlockLevel)(struct ST_Block *ptrThis, int iBlockLevel);
};
typedef struct ST_Block Block;

/**一个ST_BlockGroup包含5个ST_Block
ST_BlockGroup中作为ST_BlockManager中管理的节点
即ST_BlockManager中没有空闲空间时，将分配一个ST_BlockGroup(BLOCK_GROUP_BLOCK_NUM个ST_Block)
*/
struct ST_BlockGroup
{
#define BLOCK_GROUP_BLOCK_NUM (5)
    Block aBlock[BLOCK_GROUP_BLOCK_NUM];
    struct ST_BlockGroup *ptrBlockGroupPre;
    struct ST_BlockGroup *ptrBlockGroupNext;

    void(*Init)(struct ST_BlockGroup *ptrThis);
    Block *(*GetThreadBlock)(struct ST_BlockGroup *ptrThis, TASK_ID threadId);
    Block *(*GetFreeBlock)(struct ST_BlockGroup *ptrThis);
};
typedef struct ST_BlockGroup BlockGroup;

struct ST_BlockManager
{
    BlockGroup *ptrBlockGroupHeader;
    BlockGroup *ptrBlockGroupTail;

    Block *(*GetThreadBlock)(struct ST_BlockManager *ptrThis, TASK_ID threadId);
    Block *(*GetFreeBlock)(struct ST_BlockManager *ptrThis);
};
typedef struct ST_BlockManager BlockManager;

#pragma pack(pop)

extern Block *BlockManager_GetThreadBlock(BlockManager *ptrThis, TASK_ID threadId);
extern Block *BlockManager_GetFreeBlock(BlockManager *ptrThis);

BlockManager g_blockManager =
{
    NULL,
    NULL,
    BlockManager_GetThreadBlock,
    BlockManager_GetFreeBlock
};

extern void Block_Reset(Block *ptrThis);
extern int Block_GetUseStatus(Block *ptrThis);
extern TASK_ID Block_GetThreadId(Block *ptrThis);
extern int Block_GetBlockLevel(Block *ptrThis);
extern void Block_SetUseStatus(Block *ptrThis, int iUseStatus);
extern void Block_SetThreadId(Block *ptrThis, TASK_ID threadId);
extern void Block_SetBlockLevel(Block *ptrThis, int iBlockLevel);

/**ST_Block的初始化代码
@param :Block *ptrThis
@rerurn void.
*/
void Block_Init(Block *ptrThis)
{
    PAssert_Return(ptrThis);
    memset(ptrThis, 0, sizeof(*ptrThis));

    ///设置本结构的成员函数
    ptrThis->Init = Block_Init;
    ptrThis->Reset = Block_Reset;

    ptrThis->SetBlockLevel = Block_SetBlockLevel;
    ptrThis->GetBlockLevel = Block_GetBlockLevel;
    ptrThis->SetThreadId = Block_SetThreadId;
    ptrThis->GetThreadId = Block_GetThreadId;
    ptrThis->SetUseStatus = Block_SetUseStatus;
    ptrThis->GetUseStatus = Block_GetUseStatus;
    return;
}

/**ST_Block的重置代码
@param :Block *ptrThis
@rerurn void.
*/
void Block_Reset(Block *ptrThis)
{
    PAssert_Return(ptrThis);
    Block_Init(ptrThis);
    return;
}

/**ST_Block的获取成员变量iUseStatus的函数
其实直接引用成员变量就可以了，这里为了练习C实现C++而特意实现的
@param :int
@rerurn void.
*/
int Block_GetUseStatus(Block *ptrThis)
{
    PAssert_ReturnValue(ptrThis, 0);
    return ptrThis->iUseStatus;
}

/**ST_Block的获取成员变量threadId的函数
其实直接引用成员变量就可以了，这里为了练习C实现C++而特意实现的
@param :TASK_ID.
@rerurn void.
*/
TASK_ID Block_GetThreadId(Block *ptrThis)
{
    PAssert_ReturnValue(ptrThis, 0);
    return ptrThis->threadId;
}

/**ST_Block的获取成员变量iBlockLevel的函数
其实直接引用成员变量就可以了，这里为了练习C实现C++而特意实现的
@param :int.
@rerurn void.
*/
int Block_GetBlockLevel(Block *ptrThis)
{
    PAssert_ReturnValue(ptrThis, 0);
    return ptrThis->iBlockLevel;
}

/**ST_Block的设置成员变量iUseStatus的函数
其实直接引用成员变量就可以了，这里为了练习C实现C++而特意实现的
@param :Block *ptrThis
@param :int
@rerurn void.
*/
void Block_SetUseStatus(Block *ptrThis, int iUseStatus)
{
    PAssert_Return(ptrThis);
    ptrThis->iUseStatus = iUseStatus;
}

/**ST_Block的设置成员变量threadId的函数
其实直接引用成员变量就可以了，这里为了练习C实现C++而特意实现的
@param :Block *ptrThis
@param :TASK_ID.
@rerurn void.
*/
void Block_SetThreadId(Block *ptrThis, TASK_ID threadId)
{
    PAssert_Return(ptrThis);
    ptrThis->threadId = threadId;
}

/**ST_Block的设置成员变量iBlockLevel的函数
其实直接引用成员变量就可以了，这里为了练习C实现C++而特意实现的
@param :Block *ptrThis
@param :int.
@rerurn void.
*/
void Block_SetBlockLevel(Block *ptrThis, int iBlockLevel)
{
    PAssert_Return(ptrThis);
    ptrThis->iBlockLevel = iBlockLevel;
}

extern Block *BlockGroup_GetThreadBlock(BlockGroup *ptrThis, TASK_ID threadId);
extern Block *BlockGroup_GetFreeBlock(BlockGroup *ptrThis);

/**ST_BlockGroup的初始化代码
@param :Block *ptrThis
@rerurn void.
*/
void BlockGroup_Init(BlockGroup *ptrThis)
{
    int i = 0;
    PAssert_Return(ptrThis);
    memset(ptrThis, 0, sizeof(*ptrThis));

    ptrThis->Init = BlockGroup_Init;
    ptrThis->GetThreadBlock = BlockGroup_GetThreadBlock;
    ptrThis->GetFreeBlock = BlockGroup_GetFreeBlock;

    ///初始化每个Block
    for (i = 0; i < BLOCK_GROUP_BLOCK_NUM; i++)
    {
        Block *ptrBlock = &ptrThis->aBlock[i];
        ptrBlock->Init = Block_Init;

        ptrBlock->Init(ptrBlock);
    }

    return;
}

/**从ST_BlockGroup中获取给定线程的Block的指针
@param :Block *ptrThis
@param :TASK_ID threadId
@rerurn Block *.
*/
Block *BlockGroup_GetThreadBlock(BlockGroup *ptrThis, TASK_ID threadId)
{
    Block *ptrBlockReturn = NULL;
    int i = 0;
    PAssert_ReturnValue(ptrThis, NULL);

    for (i = 0; i < BLOCK_GROUP_BLOCK_NUM; i++)
    {
        Block *ptrBlock = &ptrThis->aBlock[i];

        if (ptrBlock->GetThreadId(ptrBlock) == threadId)
        {
            ptrBlockReturn = ptrBlock;
            break;
        }
    }

    return ptrBlockReturn;
}

/**从ST_BlockGroup中获取空闲的Block的指针
@param :BlockGroup *ptrThis
@rerurn Block *.
*/
Block *BlockGroup_GetFreeBlock(BlockGroup *ptrThis)
{
    Block *ptrBlockReturn = NULL;
    int i = 0;
    PAssert_ReturnValue(ptrThis, NULL);

    for (i = 0; i < BLOCK_GROUP_BLOCK_NUM; i++)
    {
        Block *ptrBlock = &ptrThis->aBlock[i];

        if (ptrBlock->GetUseStatus(ptrBlock) == BLOCK_FREE)
        {
            ptrBlockReturn = ptrBlock;
            break;
        }
    }

    return ptrBlockReturn;
}

Block *BlockManager_GetThreadBlock(BlockManager *ptrThis, TASK_ID threadId)
{
    Block *ptrBlockReturn = NULL;
    BlockGroup *ptrBlockGroup = NULL;
    PAssert_ReturnValue(ptrThis, NULL);

    if (NULL == ptrThis->ptrBlockGroupHeader)
    {
        BlockGroup *ptrGroupBlock = (BlockGroup *)malloc(sizeof(BlockGroup));
        ptrGroupBlock->Init = BlockGroup_Init;
        ptrGroupBlock->Init(ptrGroupBlock);
        ptrThis->ptrBlockGroupHeader = ptrGroupBlock;
        ptrThis->ptrBlockGroupTail = ptrGroupBlock;
    }

    PAssert_ReturnValue(ptrThis->ptrBlockGroupHeader, NULL);

    ptrBlockGroup = ptrThis->ptrBlockGroupHeader;
    do
    {
        ptrBlockReturn = ptrBlockGroup->GetThreadBlock(ptrBlockGroup, threadId);
        if (NULL != ptrBlockReturn)
        {
            break;
        }
        else
        {
            ptrBlockGroup = ptrBlockGroup->ptrBlockGroupNext;
        }
    } while (NULL != ptrBlockGroup);

    ///此时找遍所有的链表都没有找到相应的Block，则重新找块没用的Block
    if (NULL == ptrBlockReturn)
    {
        ptrBlockReturn = ptrThis->GetFreeBlock(ptrThis);
    }

    return ptrBlockReturn;
}

Block *BlockManager_GetFreeBlock(BlockManager *ptrThis)
{
    Block *ptrBlockReturn = NULL;
    PAssert_ReturnValue(ptrThis, NULL);

    if (NULL == ptrThis->ptrBlockGroupHeader)
    {
        ///原来没有空间则动态分配
        BlockGroup *ptrGroupBlock = (BlockGroup *)malloc(sizeof(BlockGroup));
        ptrGroupBlock->Init = BlockGroup_Init;
        ptrGroupBlock->Init(ptrGroupBlock);
        ptrThis->ptrBlockGroupHeader = ptrGroupBlock;
        ptrThis->ptrBlockGroupTail = ptrGroupBlock;

        ptrBlockReturn = ptrGroupBlock->GetFreeBlock(ptrGroupBlock);
    }
    else
    {
        BlockGroup *ptrBlockGroup = ptrThis->ptrBlockGroupHeader;
        do
        {
            ptrBlockReturn = ptrBlockGroup->GetFreeBlock(ptrBlockGroup);
            if (NULL != ptrBlockReturn)
            {
                break;
            }
            else
            {
                ptrBlockGroup = ptrBlockGroup->ptrBlockGroupNext;
            }
        } while (NULL != ptrBlockGroup);

        ///此时找遍所有的链表都没有找到空闲空间,则进行动态分配
        if (NULL == ptrBlockReturn)
        {
            BlockGroup *ptrBlockGroup = (BlockGroup *)malloc(sizeof(BlockGroup));
            ptrBlockGroup->Init = BlockGroup_Init;
            ptrBlockGroup->Init(ptrBlockGroup);

            ptrThis->ptrBlockGroupTail->ptrBlockGroupNext = ptrBlockGroup;
            ptrBlockGroup->ptrBlockGroupPre = ptrThis->ptrBlockGroupTail;
            ptrThis->ptrBlockGroupTail = ptrBlockGroup;

            ptrBlockReturn = ptrBlockGroup->GetFreeBlock(ptrBlockGroup);
        }
    }

    return ptrBlockReturn;
}

void log4c_block_begin(const char * fileName, int lineNum, const char * traceName)
{
    TASK_ID threadId = VOS_GetSelfTaskID();
    int i = 0;
    Block *ptrBlock = NULL;
    PAssert_Return(fileName);
    PAssert_Return(traceName);

    ptrBlock = g_blockManager.GetThreadBlock(&g_blockManager, threadId);
    if (NULL != ptrBlock)
    {
        int iBlockLevel = 0;
        char strMsgBuffer[LOG4C_BUFFER_SIZE_MAX] = { 0 };

        if (ptrBlock->GetUseStatus(ptrBlock) == BLOCK_FREE)
        {
            ptrBlock->SetUseStatus(ptrBlock, BLOCK_BUSY);
            ptrBlock->SetThreadId(ptrBlock, threadId);
            ptrBlock->SetBlockLevel(ptrBlock, 0);
        }

        if (ptrBlock->GetThreadId(ptrBlock) != threadId)
        {
            ptrBlock->SetThreadId(ptrBlock, threadId);
            ptrBlock->SetBlockLevel(ptrBlock, 0);
        }

        ///ptrBlock->SetThreadId(ptrBlock, threadId);
        ///ptrBlock->SetBlockLevel(ptrBlock, 0);

        iBlockLevel = ++(ptrBlock->iBlockLevel);
#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
        strcat_s(strMsgBuffer, sizeof(strMsgBuffer), "B-Entry\t");
        for (i = 0; i < iBlockLevel; i++)
        {
            strcat_s(strMsgBuffer, sizeof(strMsgBuffer), "==");
        }
        strcat_s(strMsgBuffer, sizeof(strMsgBuffer), "> ");
        strcat_s(strMsgBuffer, sizeof(strMsgBuffer), traceName);
#else 
        strncat(strMsgBuffer, "B-Entry\t", sizeof(strMsgBuffer));
        for (i = 0; i < iBlockLevel; i++)
        {
            strncat(strMsgBuffer, "==", sizeof(strMsgBuffer));
        }
        strncat(strMsgBuffer, "> ", sizeof(strMsgBuffer));
        strncat(strMsgBuffer, traceName, sizeof(strMsgBuffer));
#endif
#else
        strncat(strMsgBuffer, "B-Entry\t", sizeof(strMsgBuffer));
        for (i = 0; i < iBlockLevel; i++)
        {
            strncat(strMsgBuffer, "==", sizeof(strMsgBuffer));
        }
        strncat(strMsgBuffer, "> ", sizeof(strMsgBuffer));
        strncat(strMsgBuffer, traceName, sizeof(strMsgBuffer));
#endif

        ///调用log4c的函数来记录日志
        log_msg(fileName, lineNum, DEFAULT_LOG_CATEGORY_NAME, LOG4C_PRIORITY_TRACE, strMsgBuffer);
    }
}

void log4c_block_end(const char * fileName, int lineNum, const char * traceName)
{
    TASK_ID threadId = VOS_GetSelfTaskID();
    int i = 0;
    Block *ptrBlock = NULL;
    PAssert_Return(fileName);
    PAssert_Return(traceName);

    ptrBlock = g_blockManager.GetThreadBlock(&g_blockManager, threadId);
    if (NULL != ptrBlock)
    {
        int iBlockLevel = ptrBlock->GetBlockLevel(ptrBlock);
        char strMsgBuffer[LOG4C_BUFFER_SIZE_MAX] = { 0 };

#ifdef WIN32
#if _MSC_VER >= 1400 ///vs 2005
        strcat_s(strMsgBuffer, sizeof(strMsgBuffer), "B-Exit\t");
        strcat_s(strMsgBuffer, sizeof(strMsgBuffer), "<");
        for (i = 0; i < iBlockLevel; i++)
        {
            strcat_s(strMsgBuffer, sizeof(strMsgBuffer), "==");
        }
        strcat_s(strMsgBuffer, sizeof(strMsgBuffer), " ");
        strcat_s(strMsgBuffer, sizeof(strMsgBuffer), traceName);
#else
        strncat(strMsgBuffer, "B-Exit\t", sizeof(strMsgBuffer));
        strncat(strMsgBuffer, "<", sizeof(strMsgBuffer));
        for (i = 0; i < iBlockLevel; i++)
        {
            strncat(strMsgBuffer, "==", sizeof(strMsgBuffer));
        }
        strncat(strMsgBuffer, " ", sizeof(strMsgBuffer));
        strncat(strMsgBuffer, traceName, sizeof(strMsgBuffer));
#endif
#else
        strncat(strMsgBuffer, "B-Exit\t", sizeof(strMsgBuffer));
        strncat(strMsgBuffer, "<", sizeof(strMsgBuffer));
        for (i = 0; i < iBlockLevel; i++)
        {
            strncat(strMsgBuffer, "==", sizeof(strMsgBuffer));
        }
        strncat(strMsgBuffer, " ", sizeof(strMsgBuffer));
        strncat(strMsgBuffer, traceName, sizeof(strMsgBuffer));
#endif
        ///调用log4c的函数来记录日志
        log_msg(fileName, lineNum, DEFAULT_LOG_CATEGORY_NAME, LOG4C_PRIORITY_TRACE, strMsgBuffer);

        iBlockLevel = --ptrBlock->iBlockLevel;
        if (ptrBlock->iBlockLevel == 0)
        {
            ptrBlock->SetThreadId(ptrBlock, 0);
            ptrBlock->SetBlockLevel(ptrBlock, 0);
            ptrBlock->SetUseStatus(ptrBlock, BLOCK_FREE);
        }
    }
}
/************************************************************************/
/************** End of log.c *******************************************/


/************** Begin of sd_xplatform.c *******************************************/
///static const char version[] = "$Id$";

/*
* sd_xplatform.c
*
* See the COPYING file for the terms of usage and distribution.
*/

#include <stdio.h>
#include <string.h>
///#include "log4c/defs.h"
///#include "sd_xplatform.h"

/****************** getopt *******************************/

#define	EOF	(-1)

int sd_opterr = 1;
int sd_optind = 1;
int sd_optopt = 0;
char *sd_optarg = NULL;
int _sp = 1;

#define warn(a,b,c)fprintf(stderr,a,b,c)

void
getopt_reset(void)
{
    sd_opterr = 1;
    sd_optind = 1;
    sd_optopt = 0;
    sd_optarg = NULL;
    _sp = 1;
}

int
sd_getopt(int argc, char *const *argv, const char *opts)
{
    char c;
    const char *cp;

    if (_sp == 1) {
        if (sd_optind >= argc || argv[sd_optind][0] != '-' ||
            argv[sd_optind] == NULL || argv[sd_optind][1] == '\0')
            return (EOF);
        else if (strcmp(argv[sd_optind], "--") == 0) {
            sd_optind++;
            return (EOF);
        }
    }
    sd_optopt = c = (unsigned char)argv[sd_optind][_sp];
    if (c == ':' || (cp = strchr(opts, c)) == NULL) {
        if (opts[0] != ':')
            warn("%s: illegal option -- %c\n", argv[0], c);
        if (argv[sd_optind][++_sp] == '\0') {
            sd_optind++;
            _sp = 1;
        }
        return ('?');
    }

    if (*(cp + 1) == ':') {
        if (argv[sd_optind][_sp + 1] != '\0')
            sd_optarg = &argv[sd_optind++][_sp + 1];
        else if (++sd_optind >= argc) {
            if (opts[0] != ':') {
                warn("%s: option requires an argument"
                    " -- %c\n", argv[0], c);
            }
            _sp = 1;
            sd_optarg = NULL;
            return (opts[0] == ':' ? ':' : '?');
        }
        else
            sd_optarg = argv[sd_optind++];
        _sp = 1;
    }
    else {
        if (argv[sd_optind][++_sp] == '\0') {
            _sp = 1;
            sd_optind++;
        }
        sd_optarg = NULL;
    }
    return (c);
}

/*****************************  gettimeofday *******************/
#ifdef _WIN32

#if 0 /* also in winsock[2].h */

/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,_CRT_PACKING)
#define _TIMEVAL_DEFINED
struct timeval {
    long tv_sec;
    long tv_usec;
    long tv_usec;
};
#pragma pack(pop)

#endif /* _TIMEVAL_DEFINED */

int sd_gettimeofday(LPFILETIME lpft, void* tzp) {

    if (lpft) {
        GetSystemTimeAsFileTime(lpft);
    }
    /* 0 indicates that the call succeeded. */
    return 0;
}
#endif /* _WIN32 */

/*
* Placeholder for WIN32 version to get last changetime of a file
*/
#ifdef WIN32
int sd_stat_ctime(const char* path, time_t* time)
{
    return -1;
}
#else
int sd_stat_ctime(const char* path, time_t* time)
{
    struct stat astat;
    int statret = stat(path, &astat);
    if (0 != statret)
    {
        return statret;
    }
#ifdef __USE_MISC
    *time = astat.st_ctim.tv_sec;
#else
    *time = astat.st_ctime;
#endif
    return statret;
}
#endif
/************** End of sd_xplatform.c *******************************************/

/************** Begin of sprintf.c *******************************************/
///static const char version[] = "$Id$";

/*
* Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
///#include <sd/sprintf.h>
///#include <sd/malloc.h>
///#include <sd/sd_xplatform.h>

/******************************************************************************/
extern char* sd_sprintf(const char* a_fmt, ...)
{
    char*	buffer;
    va_list	args;

    va_start(args, a_fmt);
    buffer = sd_vsprintf(a_fmt, args);
    va_end(args);

    return buffer;
}

/******************************************************************************/
extern char* sd_vsprintf(const char* a_fmt, va_list a_args)
{
    int		size = 1024;
    char*	buffer = (char*)sd_calloc(size, sizeof(char));

    while (1)
    {
        int n = vsnprintf(buffer, size, a_fmt, a_args);

        /* If that worked, return */
        if (n > -1 && n < size)
        {
            return buffer;
        }

        /* Else try again with more space. */
        if (n > -1)     /* ISO/IEC 9899:1999 */
        {
            size = n + 1;
        }
        else            /* twice the old size */
        {
            size *= 2;

            /** 为避免空间无限申请下面break出去
            1.当a_fmt中的格式有问题时，可能会导致这种情况的发生，此时应当break防止无限申请内存。
            added by wenhm in 2010.12.24
            */
            if (size > LOG4C_BUFFER_SIZE_DEFAULT)
            {
                return buffer;
            }
        }

        buffer = (char*)sd_realloc(buffer, size);
    }
}

#if defined(__osf__)
#	ifndef snprintf
#		include "sprintf.osf.c"
#	endif
#endif
/************** End of sprintf.c *******************************************/

//////下面为log4cplusWrapper的实现代码（只包含c++的实现代码）///////////////

//////////////////////////////////////////////////////////////////////////
#if (defined(LOG4CPLUS_ENABLE) && defined(__cplusplus))&& defined(IMPLEMENT_LOG4CPLUS)
#pragma warning(push) 

/************** Begin of logWrapper.cpp *******************************************/
/*
* logWrapper.cpp
*
* 日志相关代码的定义文件,CLog类的实现文件
*/
///#include "config_log.h"

///#include "GlobalMacro.h"
///#include "../vos/include/vos.h"
///#include "log.h"
///#include "log4c_cxxWrapper.h"
///#include "sd\sd_xplatform.h"
///#include "sd\sprintf.h"
#include <algorithm>
#include <functional>
#include <assert.h>

///static SEM_ID g_semphore = VOS_CreateMSem(VOS_SEM_Q_FIFO);
static SEM_ID g_semphore = VOS_CreateThreadMutex();

///静态成员变量定义
std::ostrstream CLogWrapper::m_ostrs;

CLogWrapper* CLogWrapper::m_ptrInstance = NULL;

CLogWrapper* CLogWrapper::Instance()
{
    /// Perform Double-Checked Locking Optimization.懒汉模式
    if (NULL != m_ptrInstance)
    {
        return m_ptrInstance;
    }
    else
    {
        ///static SEM_ID semphore = NULL;

        ///为线程安全这里加锁
        ///VOS_TakeSem(g_semphore, VOS_WAIT_FOREVER);
        VOS_TakeThreadMutex(g_semphore, VOS_WAIT_FOREVER);

        ///CLockMgr<CCSWrapper> guard(m_lock, FALSE);
        if (NULL != m_ptrInstance)
        {
            // 解锁
            ///VOS_GiveSem(g_semphore);
            VOS_GiveThreadMutex(g_semphore);
            return m_ptrInstance;
        }
        else
        {
            m_ptrInstance = new CLogWrapper();
            // m_ptrInstance->Init("");
            // 解锁
            ///VOS_GiveSem(g_semphore);
            VOS_GiveThreadMutex(g_semphore);
            return m_ptrInstance;
        }
    }
}

void CLogWrapper::DestroyInstance()
{
    PAssert_Return(NULL != m_ptrInstance && NULL != g_semphore);

    ///CLockMgr<CCSWrapper> guard(m_lock, TRUE);
    ///为线程安全这里加锁
    ///VOS_TakeSem(g_semphore, VOS_WAIT_FOREVER);
    VOS_TakeThreadMutex(g_semphore, VOS_WAIT_FOREVER);

    DELETEP(m_ptrInstance);

    // 解锁
    ///VOS_GiveSem(g_semphore);
    VOS_GiveThreadMutex(g_semphore);

    // 删除资源
    ///VOS_DELETE_SEM(g_semphore)
    VOS_DELETE_THREAD_MUTEX(g_semphore);
    ///VOS_DeleteSem(g_semphore);
}

bool CLogWrapper::IsInstantiated()
{
    bool bResult = (m_ptrInstance != NULL);

    return bResult;
}

const log4cplus::Logger &CLogWrapper::GetDefaultLogger(const bool &bReset)
{
    ///loggerNameDefault 简写为LD
#if 1
    log4cplus::tstring strLoggerName = ToLog4cplus_TString(gs_st_LogProperty.m_strLoggerName);
    static log4cplus::Logger loggerNameDefault = log4cplus::Logger::getInstance(strLoggerName);
    if (true == bReset)
    {
        loggerNameDefault = log4cplus::Logger::getInstance(strLoggerName);
    }
#else
    static log4cplus::Logger loggerNameDefault = log4cplus::Logger::getInstance(LOGGER_DEFAULT_NAME);
    if (true == bReset)
    {
        loggerNameDefault = log4cplus::Logger::getInstance(LOGGER_DEFAULT_NAME);
    }
#endif
    return loggerNameDefault;
}

std::string CLogWrapper::PreProcessLogMsg(const std::string &strMsg)
{
#if 1
    return strMsg;
#else
    /** 由于log4c中内部会将msg作为sprintf的format格式
    1.为防止msg中本身带有%导致log4c中出错，这里统一将%替换为%%
    */
    std::string strMsgLocal = strMsg;
    StringUtils_Replace(strMsgLocal, "%", "%%%%");

    return strMsgLocal;
#endif
}

void CLogWrapper::LogMsg(
    const char *strFile,
    const int &iLineNum,
    const int &iLogLevel,
    const char *strMsg,
    const log4cplus::Logger &loggerObj
)
{
    PAssertNotNull_Return(strMsg);
    ///PAssertNotNull_Return(strFile);

    /** 由于log4c中内部会将msg作为sprintf的format格式
    1.为防止msg中本身带有%导致log4c中出错，这里统一将%替换为%%
    */
    std::string strMsgLocal = PreProcessLogMsg(strMsg);

    ///调用log4c的函数来记录日志
    log_msg_cpp(strFile, iLineNum, loggerObj, iLogLevel, strMsgLocal.c_str());
}

void CLogWrapper::LogMsg(
    const int &iLogLevel,
    const char *strMsg,
    const log4cplus::Logger &loggerObj
)
{
    PAssertNotNull_Return(strMsg);
    ///PAssertNotNull_Return(strFile);
    ///PAssertNotNull_Return(strLoggerName);

    /** 由于log4c中内部会将msg作为sprintf的format格式
    1.为防止msg中本身带有%导致log4c中出错，这里统一将%替换为%%
    */
    std::string strMsgLocal = PreProcessLogMsg(strMsg);

    ///调用log4c的函数来记录日志
    log_msg_no_file_num_cpp(loggerObj, iLogLevel, strMsgLocal.c_str());
}

void CLogWrapper::LogMsgNoFileNumAndNoLayout(
    const int &iLogLevel,
    const char *strMsg,
    const log4cplus::Logger &loggerObj
)
{
    PAssertNotNull_Return(strMsg);
    ///PAssertNotNull_Return(strFile);
    ///PAssertNotNull_Return(strLoggerName);

    /** 由于log4c中内部会将msg作为sprintf的format格式
    1.为防止msg中本身带有%导致log4c中出错，这里统一将%替换为%%
    */
    std::string strMsgLocal = PreProcessLogMsg(strMsg);

    ///调用log4c的函数来记录日志
    log_msg_no_file_num_no_layout_cpp(loggerObj, iLogLevel, strMsgLocal.c_str());
}

CLogWrapper::CLogWrapper(const std::string& strConfigFile)
{
    Init(strConfigFile);
}

bool CLogWrapper::Init(const std::string& strConfigFile)
{
    bool bResult = false;
    int iResult = -1;

    ///调用log4c的函数来初始化日志
    if (strConfigFile.length() > 0)
    {
        iResult = log_init_with_cfg_file(strConfigFile.c_str());
    }

    if (0 != iResult)
    {
        iResult = log_init();
    }

    bResult = (0 == iResult);

    return bResult;
}

bool CLogWrapper::Fini()
{
    int iResult = log_fini();

    return (0 == iResult);
}

CLogWrapper::~CLogWrapper()
{
    Fini();
}

bool CLogWrapper::IsOK()
{
    bool bResult = (NULL != m_ptrInstance) && (is_log4c_init());

    return bResult;
}

//////////////////////////////////////////////////////////////////////////
static char gs_strFileContentInCxx[] =
"######################################################################## \n"
"#rootLogger \n"
"#	logger \n"
"#		appender \n"
"#			layout \n"
"#				ConversionPattern \n"
"#			filters \n"
"######################################################################## \n"
"log4cplus.rootLogger=TRACE, STDOUT  \n"
"\n"
"######################################################################## \n"
"#Logger Default这里简写为LD \n"
"#这样日志记录中记录Logger名时可以节省日志文件大小 \n"
"######################################################################## \n"
"#log4cplus.logger.LD=TRACE, AppenderNameRollingFile, AppenderNameDefault \n"
"log4cplus.logger.LD=INFO, AppenderNameRollingFile \n"
"log4cplus.additivity.LD=FALSE \n"
"\n"
"log4cplus.appender.STDOUT=log4cplus::ConsoleAppender \n"
"log4cplus.appender.STDOUT.layout=log4cplus::PatternLayout \n"
"log4cplus.appender.STDOUT.layout.ConversionPattern=%D{%m/%d/%y %H:%M:%S} [%t] %-5p %c - %m %n \n"
"\n"
"log4cplus.appender.AppenderNameRollingFile=log4cplus::RollingFileAppender \n"
"log4cplus.appender.AppenderNameRollingFile.File=log.log \n"
"#log4cplus.appender.AppenderNameRollingFile.Append=TRUE \n"
"#log4cplus.appender.AppenderNameRollingFile.MaxFileSize=5MB \n"
"log4cplus.appender.AppenderNameRollingFile.MaxFileSize=1024KB \n"
"log4cplus.appender.AppenderNameRollingFile.MaxBackupIndex=10 \n"
"#log4cplus.appender.AppenderNameRollingFile.layout=log4cplus::TTCCLayout \n"
"log4cplus.appender.AppenderNameRollingFile.layout=log4cplus::PatternLayout \n"
"log4cplus.appender.AppenderNameRollingFile.layout.ConversionPattern=%D{%m/%d/%y %H:%M:%S:%q} [%t] %-5p %c - %F(%L) %m %n \n"
"#log4cplus.appender.AppenderNameRollingFile.ImmediateFlush=true \n"
"#log4cplus.appender.AppenderNameRollingFile.Threshold=ERROR \n"
"#log4cplus.appender.AppenderNameRollingFile.BufferSize=0 \n"
"\n"
"\n"
"#log4cplus.appender.AppenderNameDefault=log4cplus::RollingFileAppender \n"
"#log4cplus.appender.AppenderNameDefault=log4cplus::FileAppender \n"
"#log4cplus.appender.AppenderNameDefault.File=test_output.log \n"
"#log4cplus.appender.AppenderNameDefault=log4cplus::NullAppender \n"
"#log4cplus.appender.AppenderNameDefault.layout=log4cplus::TTCCLayout \n"
"#log4cplus.appender.AppenderNameDefault.layout=log4cplus::PatternLayout \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%D{%y-%m-%d %H:%M:%S,%q} [%t] %-5p %c <%x> - %m%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%D{%y-%m-%d %H:%M:%S,%q} %-5p %c <%x> - %m%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%p - %m%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%-5p - %m%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%l - %m%n \n"
"#log4cplus.appender.AppenderNameDefault.layout.ConversionPattern=%C.%M.%L - %m%n \n"
"\n";

static char gs_strDefualtFileNameInCxx[] = LOG_CFG_FILENAME_DEFAULT;

bool CLogWrapper::InitWithString(
    const std::string &strFileContent,
    const std::string &strFileName
)
{
    std::string strLocalFileContent = (strFileContent.length() > 0 ? strFileContent : gs_strFileContentInCxx);
    std::string strLocalFileName = (strFileName.length() > 0 ? strFileName : gs_strDefualtFileNameInCxx);

    /**
    若文件已存在则不重新生成配置文件了
    否则生成新的配置文件
    */
    if (RESULT_OK != SD_ACCESS_READ(strLocalFileName.c_str()))
    {
        ///文件不存在新建
        FILE *file = fopen(strLocalFileName.c_str(), "wb");
        if (NULL != file)
        {
            fwrite(strLocalFileContent.c_str(), 1, strLocalFileContent.length(), file);
            fclose(file);
        }
    }
    else
    {
        ///文件存在,do nothing，使用原来的配置文件就OK了。
    }

    ///调用Init函数进行实际的日志模块初始化工作
    bool bResult = Init(strLocalFileName.c_str());

    return bResult;
}

const int CLogWrapper::GetPriorityWrapper(
    const int iLogLevel,		///日志记录级别
    const char* strFormat,		///日志内容格式
    ...						///日志内容
)
{
    return iLogLevel;
}

const char *CLogWrapper::LogVsnprintfWrapper(
    const int iLogLevel,	///日志记录级别
    const char* strFormat,	///日志内容格式
    ...						///日志内容
)
{
    const char * strMsg = NULL;
    va_list va;
    va_start(va, strFormat);
    {
        strMsg = sd_vsprintf(strFormat, va);
    }
    va_end(va);

    return strMsg;
}

///CBlockBase的实现方法:
///存储各线程的Block层次的Vector
CBlockBase::BlockInfoVector CBlockBase::m_blockVector;

CBlockBase::CBlockBase(
    const char * fileName,
    int lineNum,
    const char * traceBeginName,
    const char * traceEndName,
    const int iLogLevel,
    const bool bMultiThreadEnabled
)
{
    m_iLogLevel = iLogLevel;
    m_bIsEnabledForLog = (CLogWrapper::GetDefaultLogger()).isEnabledFor(m_iLogLevel);

    ///成员变量初始化
    m_strFileName = fileName;
    m_iLineNum = lineNum;
    m_strBlockBeginName = ((NULL != traceBeginName) ? traceBeginName : "");
    m_strBlockEndName = ((NULL != traceEndName) ? traceEndName : "");
    m_bMultiThreadEnabled = bMultiThreadEnabled;
}

CBlockBase::~CBlockBase()
{
}

int CBlockBase::EnterBlock()
{
    /*要访问m_BlockInfoVector了，要加锁进行互斥访问,
    由于下面可能有wait操作可能会堵住，为防止死锁，
    所以这里只能分两部进行局部加锁，将wait的语句放于该锁管辖范围之外
    */
    Lock();

    int iBlockLevel = 0;
    ///if ((PTraceOptions&Blocks) != 0) 
    {
        TASK_ID threadId = VOS_GetSelfTaskID();

        ///查找waitThread线程是否已经对本信号量加过锁了
        BlockInfoVector::iterator iter = FindThreadBlockInfo(threadId);

        if (iter != m_blockVector.end())
        {
            ///本线程Block层次加2
            iter->iBlockLevel += 2;
            iBlockLevel = iter->iBlockLevel;
        }
        else
        {
            ///本线程Block层次还没有
            BlockInfo blockInfo = { 0 };
            blockInfo.threadId = threadId;
            blockInfo.iBlockLevel += 2;

            m_blockVector.push_back(blockInfo);

            iBlockLevel = blockInfo.iBlockLevel;
        }
    }

    ///释放信号量
    Unlock();

    return iBlockLevel;
}

int CBlockBase::ExitBlock()
{
    /*要访问m_BlockInfoVector了，要加锁进行互斥访问,
    由于下面可能有wait操作可能会堵住，为防止死锁，
    所以这里只能分两部进行局部加锁，将wait的语句放于该锁管辖范围之外
    */
    Lock();

    int iBlockLevel = 0;

    ///if ((PTraceOptions&Blocks) != 0)
    {
        ///    PThread * thread = PThread::Current();
        TASK_ID   threadId = VOS_GetSelfTaskID();

        ///查找waitThread线程是否已经对本信号量加过锁了
        BlockInfoVector::iterator iter = FindThreadBlockInfo(threadId);

        if (iter != m_blockVector.end())
        {
            iBlockLevel = iter->iBlockLevel;
            ///本线程Block层次减2
            iter->iBlockLevel -= 2;

            ///Block层次为0将该线程相关的Block信息删除
            if (iter->iBlockLevel == 0)
            {
                m_blockVector.erase(iter);
            }
        }
    }

    ///释放信号量
    Unlock();

    return iBlockLevel;
}

void CBlockBase::Lock()
{
    if (true == m_bMultiThreadEnabled)
    {
        ///CWaitAndSignal sync(m_sync);
        ///为线程安全这里加锁
        ///VOS_TakeSem(g_semphore, VOS_WAIT_FOREVER);
        VOS_TakeThreadMutex(g_semphore, VOS_WAIT_FOREVER);
    }
}

void CBlockBase::Unlock()
{
    if (true == m_bMultiThreadEnabled)
    {
        ///释放信号量
        ///VOS_GiveSem(g_semphore);
        VOS_GiveThreadMutex(g_semphore);
    }
}

std::string CBlockBase::GetBlockEnterString(const int& iBlockLevel)
{
    ///std::ostrstream ostrs;
    std::ostringstream ostrs;

    ostrs << "B-Entry\t";
    for (int i = 0; i < iBlockLevel; i++)
    {
        ostrs << "=";
    }

    ostrs << "> " << m_strBlockBeginName /*<< " ThreadId:" << threadId*/ << std::ends;

    return ostrs.str();
}

std::string CBlockBase::GetBlockExitString(const int& iBlockLevel)
{
    ///std::ostrstream ostrs;
    std::ostringstream ostrs;

    ostrs << "B-Exit \t<";
    for (int i = 0; i < iBlockLevel; i++)
    {
        ostrs << "=";
    }

    ostrs << " " << m_strBlockEndName /*<< " ThreadId:" << threadId */ << std::ends;

    return ostrs.str();
}

///检测threadId是的Block是否已经存在了
CBlockBase::BlockInfoVector::iterator CBlockBase::FindThreadBlockInfo(const TASK_ID& threadId)
{
    BlockInfoVector::iterator iter =
        std::find_if(m_blockVector.begin(), m_blockVector.end(),
            std::bind2nd(CBLockInfoComparedByThreadId(), threadId));

    return iter;
}

CBlockBase::BlockInfoVector::iterator CBlockBase::FindThreadBlockInfo()
{
    ///PThread * thread = PThread::Current();
    TASK_ID threadId = VOS_GetSelfTaskID();

    ///查找waitThread线程是否已经对本信号量加过锁了
    BlockInfoVector::iterator iter = FindThreadBlockInfo(threadId);

    return iter;
}

CBlockBase::BlockInfo CBlockBase::GetThreadBlockInfo()
{
    BlockInfo blockInfo = { 0 };

    BlockInfoVector::iterator iter = FindThreadBlockInfo();
    if (iter != m_blockVector.end())
    {
        blockInfo = *iter;
    }

    return blockInfo;
}

///CLogWrapper::CBlock的实现方法:
CLogWrapper::CBlock::CBlock(
    const char * fileName,
    int lineNum,
    const char * traceBeginName,
    const char * traceEndName,
    const int iLogLevel,
    const bool bMultiThreadEnabled
)
    :CBlockBase(fileName, lineNum, traceBeginName, traceEndName, iLogLevel, bMultiThreadEnabled)
{
    if (!m_bIsEnabledForLog)
    {
        return;
    }

    /**当在全局或main函数中定义的CBlock对象时
    执行CBlock析构函数时可能是在退出main函数时,而此时可能已经销毁了日志系统
    此时再执行log_msg函数将导致内存泄漏所以这里加上日志系统是否正常的判断代码
    */
    if (true == CLogWrapper::IsOK())
    {
        /** 2010-09-16 23:44 修正
        经过代码测试，发现日志模块还有个问题，就是
        LOG_FUN时，不会调用如下代码
        CLogWrapper::Instance()
        导致Clog4c对象一直没有实例化，这样
        CBlock的析构函数中就由于Clog4c::IsOk中判断Clog4c是否实例化过，若没实例化就不会输入LOG_FUN的后面<==的日志，
        也就是说在没有调用LOG()宏，实例化Clog4c之前的LOG_FUN都只能输出前面一半的===>的日志。
        修正代码在CBlock的构造函数中，自己去调用 Clog4c::Instance()函数就好了。

        2010-11-15 13:46 修正
        采用2010-09-16 23:44的修正发现会带来一个问题:
        就是日志模块卸载调后又调用了LOG_FUN等日志语句会导致重新调用CLog4c::Instance()
        导致内存泄露，所以现将下面的语句给重新注释掉
        */
#if 1
        CLogWrapper::Instance();
#endif
        int iBlockLevel = EnterBlock();

        std::ostrstream ostrs;
        ostrs << GetBlockEnterString(iBlockLevel) /*<< " ThreadId:" << threadId*/ << std::ends;

        ///调用log4c的函数来记录日志
        log_msg_cpp(m_strFileName, m_iLineNum, CLogWrapper::GetDefaultLogger(), m_iLogLevel, ostrs.str());

        ///释放内存
        ostrs.rdbuf()->freeze(0);
    }
}

CLogWrapper::CBlock::~CBlock()
{
    if (!m_bIsEnabledForLog)
    {
        return;
    }

    /**当在全局或main函数中定义的CBlock对象时
    执行CBlock析构函数时可能是在退出main函数时,而此时可能已经销毁了日志系统
    此时再执行log_msg函数将导致内存泄漏所以这里加上日志系统是否正常的判断代码
    */
    if (true == CLogWrapper::IsOK())
    {
        ///ExitBlock返回的为退出Block前的深度
        int iBlockLevel = ExitBlock();
        std::ostrstream ostrs;
        ostrs << GetBlockExitString(iBlockLevel) /*<< " ThreadId:" << threadId */ << std::ends;

        ///调用log4c的函数来记录日志
        log_msg_cpp(m_strFileName, m_iLineNum, CLogWrapper::GetDefaultLogger(), m_iLogLevel, ostrs.str());

        ///释放内存
        ostrs.rdbuf()->freeze(0);
    }
}

///CLogWrapper::CLightBlock的实现方法:
CLogWrapper::CLightBlock::CLightBlock(const char * fileName, int lineNum, const char * traceBeginName, const char * traceEndName, const int iLogLevel)
    :CBlock(fileName, lineNum, traceBeginName, traceEndName, iLogLevel, false)
{
}

CLogWrapper::CLightBlock::~CLightBlock()
{
}

/************** Begin of LogMsgCpp.cpp *******************************************/
/** 日志记录
日志记录为一个字符串指针指向的内容
@return void
*/
void log_msg_cpp(
    const char *strFile,				///文件名
    const int iLineNum,					///行号
    const log4cplus::Logger &loggerObj,	///Logger对象引用
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
)
{
    do
    {
        if ((loggerObj).isEnabledFor(iLogLevel))
        {
            log4cplus::tostringstream _log4cplus_buf;
            _log4cplus_buf << strFormat;

            ///去除文件名路径获取纯文件名
            std::string strFileName = GetFileNameByStripPath(strFile);

            (loggerObj).forcedLog(
                iLogLevel,
                _log4cplus_buf.str(),
                strFileName.c_str(),
                iLineNum
            );
        }
    } while (0);
}

/** 日志记录,不记录文件名和行号
日志记录为一个字符串指针指向的内容
@return void
*/
void log_msg_no_file_num_cpp(
    const log4cplus::Logger &loggerObj,	///Logger对象引用
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
)
{
    do
    {
        if ((loggerObj).isEnabledFor(iLogLevel))
        {
            log4cplus::tostringstream _log4cplus_buf;
            _log4cplus_buf << strFormat;

            (loggerObj).forcedLog(
                iLogLevel,
                _log4cplus_buf.str()
            );

            ///这里更改Layout使输出的记录中没有文件名和行号
            ///输出日志后将原先的Layout再恢复即可		
            ///经测试上述两点无法做到
        }
    } while (0);
}

/** 日志记录,不记录文件名和行号,没有任何layout转换，直接输出相应的字符文本到日志中
此条记录没有行号，也没有线程号，也没有回车等
日志记录为一个字符串指针指向的内容
@return void
*/
void log_msg_no_file_num_no_layout_cpp(
    const log4cplus::Logger &loggerObj,	///Logger对象引用
    const int iLogLevel,				///日志记录级别
    const char *strFormat,				///日志内容格式
    ...									///日志内容
)
{
    do
    {
        if ((loggerObj).isEnabledFor(iLogLevel))
        {
            log4cplus::tostringstream _log4cplus_buf;
            _log4cplus_buf << strFormat;

            (loggerObj).forcedLog(
                iLogLevel,
                _log4cplus_buf.str()
            );

            ///这里更改Layout使输出的记录中没有文件名和行号
            ///输出日志后将原先的Layout再恢复即可		
            ///经测试上述两点无法做到
        }
    } while (0);
}

/** 日志记录
支持类似printf函数的带格式输出
@return void
*/
void log_log_cpp(
    const log4cplus::Logger &loggerObj,	///Logger对象引用
    const int iLogLevel,				///日志记录级别
    const char* strFormat,				///日志内容格式
    ...									///日志内容
)
{
    char * strMsg = NULL;
    va_list va;
    va_start(va, strFormat);
    {
        strMsg = sd_vsprintf(strFormat, va);
    }
    va_end(va);

    do
    {
        if ((loggerObj).isEnabledFor(iLogLevel))
        {
            log4cplus::tostringstream _log4cplus_buf;
            _log4cplus_buf << strMsg;

            (loggerObj).forcedLog(
                iLogLevel,
                _log4cplus_buf.str()
            );

            ///这里更改Layout使输出的记录中没有文件名和行号
            ///输出日志后将原先的Layout再恢复即可		
            ///经测试上述两点无法做到
        }
    } while (0);

    ///释放动态分配内存
    FREEP(strMsg);
}

/************** End of LogMsgCpp.cpp *******************************************/

///对流输出ostream的<<运算符重载函数
std::ostream& operator <<(std::ostream& stream, const WCHAR *ptrWCHAR)
{
    PAssertNotNull_ReturnValue(ptrWCHAR, stream);

    std::string strMsg;

    Unicode2AnsiString(ptrWCHAR, wcslen(ptrWCHAR), strMsg);

    stream << strMsg.c_str();

    return stream;
}

std::ostream& operator <<(std::ostream& stream, const std::string &strMsg)
{
    stream << strMsg.c_str();

    return stream;
}

std::ostream& operator <<(std::ostream& stream, const std::wstring& strMsgUnicode)
{
    std::string strMsg;

    Unicode2AnsiString(strMsgUnicode.c_str(), strMsgUnicode.length(), strMsg);

    stream << strMsg.c_str();

    return stream;
}

/*************************** End of logWrapper.cpp ******************************/

/***************************  Begin of RefHolderBase.cpp ******************************/
namespace NSLog
{
    CRefHolderBase::~CRefHolderBase()
    {
        TCRefHolderBasePtrVector::const_iterator iter = m_vecPtrRefHolderBase.begin();
        while (iter != m_vecPtrRefHolderBase.end())
        {
            const CRefHolderBase *ptrRefHolder = *iter;
            iter = m_vecPtrRefHolderBase.erase(iter);
            ///attention:这里若用sharedptr就可以不用自己去主动delete了
            ///可以考虑用std::autoptr来自动析构对象
            //if (NULL != ptrRefHolder)
            //{
            //	delete ptrRefHolder;
            //	ptrRefHolder = NULL;
            //}
            DELETEP(ptrRefHolder);
        }
        m_vecPtrRefHolderBase.clear();
    }

    std::ostream& CRefHolderBase::Output2Stream(std::ostream& stream) const
    {
        TCRefHolderBasePtrVector::const_iterator iter = m_vecPtrRefHolderBase.begin();
        while (iter != m_vecPtrRefHolderBase.end())
        {
            const CRefHolderBase& refRefHolderBase = *(*iter);
            refRefHolderBase.Output2Stream(stream);
            iter++;
        }
        return stream;
    }

    //std::ostream& CRefHolderBase::operator <<(std::ostream& stream) const
    //{
    //	TCRefHolderBasePtrVector::const_iterator iter = m_vecPtrRefHolderBase.begin();
    //	while (iter != m_vecPtrRefHolderBase.end())
    //	{
    //		const CRefHolderBase& refRefHolderBase = *(*iter);
    //		///refRefHolderBase.Output2Stream(stream);
    //		stream << refRefHolderBase;
    //		iter ++;
    //	}
    //	return stream;
    //}

    std::ostream& operator <<(std::ostream& stream, const CRefHolderBase& refHolderBaseObj)
    {
        ///stream << refHolderBaseObj.Output2Stream(stream);
        refHolderBaseObj.Output2Stream(stream);
        return stream;
    }

    ///CBlockWithRef的实现方法:
    CBlockWithRef::CBlockWithRef(
        const char * fileName,
        int lineNum,
        const CRefHolderBase& refHolderBaseBegin,
        const CRefHolderBase& refHolderBaseEnd,
        const int iLogLevel,
        const bool bMultiThreadEnabled
    )
        :CBlockBase(fileName, lineNum, "", "", iLogLevel, bMultiThreadEnabled),
        m_refHolderBaseBegin(refHolderBaseBegin),
        m_refHolderBaseEnd(refHolderBaseEnd)
    {
        if (!m_bIsEnabledForLog)
        {
            return;
        }

        /**当在全局或main函数中定义的CBlock对象时
        执行CBlock析构函数时可能是在退出main函数时,而此时可能已经销毁了日志系统
        此时再执行log_msg函数将导致内存泄漏所以这里加上日志系统是否正常的判断代码
        */
        if (true == CLogWrapper::IsOK())
        {
            /** 2010-09-16 23:44 修正
            经过代码测试，发现日志模块还有个问题，就是
            LOG_FUN时，不会调用如下代码
            CLogWrapper::Instance()
            导致Clog4c对象一直没有实例化，这样
            CBlock的析构函数中就由于Clog4c::IsOk中判断Clog4c是否实例化过，若没实例化就不会输入LOG_FUN的后面<==的日志，
            也就是说在没有调用LOG()宏，实例化Clog4c之前的LOG_FUN都只能输出前面一半的===>的日志。
            修正代码在CBlock的构造函数中，自己去调用 Clog4c::Instance()函数就好了。

            2010-11-15 13:46 修正
            采用2010-09-16 23:44的修正发现会带来一个问题:
            就是日志模块卸载调后又调用了LOG_FUN等日志语句会导致重新调用CLog4c::Instance()
            导致内存泄露，所以现将下面的语句给重新注释掉
            */
#if 1
            CLogWrapper::Instance();
#endif
            int iBlockLevel = EnterBlock();

            std::ostrstream ostrs;
            ostrs << GetBlockEnterString(iBlockLevel) /*<< " ThreadId:" << threadId*/ << std::ends;

            ///调用log4c的函数来记录日志
            log_msg_cpp(m_strFileName, m_iLineNum, CLogWrapper::GetDefaultLogger(), m_iLogLevel, ostrs.str());

            ///释放内存
            ostrs.rdbuf()->freeze(0);
        }
    }

    CBlockWithRef::~CBlockWithRef()
    {
        if (!m_bIsEnabledForLog)
        {
            return;
        }

        /**当在全局或main函数中定义的CBlock对象时
        执行CBlock析构函数时可能是在退出main函数时,而此时可能已经销毁了日志系统
        此时再执行log_msg函数将导致内存泄漏所以这里加上日志系统是否正常的判断代码
        */
        if (true == CLogWrapper::IsOK())
        {
            ///ExitBlock返回的为退出Block前的深度
            int iBlockLevel = ExitBlock();
            std::ostrstream ostrs;
            ostrs << GetBlockExitString(iBlockLevel) /*<< " ThreadId:" << threadId */ << std::ends;

            ///调用log4c的函数来记录日志
            log_msg_cpp(m_strFileName, m_iLineNum, CLogWrapper::GetDefaultLogger(), m_iLogLevel, ostrs.str());

            ///释放内存
            ostrs.rdbuf()->freeze(0);
        }
    }

    std::string CBlockWithRef::GetBlockEnterString(const int& iBlockLevel)
    {
        ///std::ostrstream ostrs;
        std::ostringstream ostrs;

        ostrs << "B-Entry\t";
        for (int i = 0; i < iBlockLevel; i++)
        {
            ostrs << "=";
        }

        ostrs << "> " << m_refHolderBaseBegin /*<< " ThreadId:" << threadId*/ << std::ends;

        return ostrs.str();
    }

    std::string CBlockWithRef::GetBlockExitString(const int& iBlockLevel)
    {
        ///std::ostrstream ostrs;
        std::ostringstream ostrs;

        ostrs << "B-Exit \t<";
        for (int i = 0; i < iBlockLevel; i++)
        {
            ostrs << "=";
        }

        ostrs << " " << m_refHolderBaseEnd /*<< " ThreadId:" << threadId */ << std::ends;

        return ostrs.str();
    }

    ///CLightBlockWithRef的实现方法:
    CLightBlockWithRef::CLightBlockWithRef(const char * fileName, int lineNum, const CRefHolderBase& refHolderBaseBegin, const CRefHolderBase& refHolderBaseEnd, const int iLogLevel)
        :CBlockWithRef(fileName, lineNum, refHolderBaseBegin, refHolderBaseEnd, iLogLevel, false)
    {
    }

    CLightBlockWithRef::~CLightBlockWithRef()
    {
    }
}
/***************************  End of RefHolderBase.cpp ******************************/

///恢复禁用的编译警告信息
#pragma warning(pop) 

#endif ///(defined(LOG4CPLUS_ENABLE) && defined(__cplusplus))&& defined(IMPLEMENT_LOG4CPLUS)

#endif ///defined(LOG4CPLUS_ENABLE) && defined(__cplusplus)

///恢复禁用的编译警告信息
#pragma warning(pop) 

#endif ///!defined(AFX_LOG4CPLUSWRAPPER_H__DD58A78D_C125_410F_B4C8_F0067B797121__INCLUDED_)
