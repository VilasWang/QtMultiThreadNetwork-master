#include "ClassMemoryTracer.h"
//#include "Log4cplusWrapper.h"
#include <sstream>

std::unique_ptr<Lock> ClassMemoryTracer::m_lock(new Lock);
TClassRefCount ClassMemoryTracer::s_mapRefConstructor;
TClassRefCount ClassMemoryTracer::s_mapRefDestructor;

void printLog(std::string str)
{
	OutputDebugStringA(str.c_str());
	//LOG_INFO(str);
}

std::string intToString(const int n)
{
	std::stringstream str;
	str << n;
	return str.str();
}

void ClassMemoryTracer::printInfo()
{
	m_lock->lock();
	std::string str = "ClassMemoryTracer[Constructor]\n";
	printLog(str);
	std::string str2;
	auto iter = s_mapRefConstructor.cbegin();
	for (; iter != s_mapRefConstructor.cend(); ++iter)
	{
		str2 = iter->first;
		str2 += ": ";
		str2 += intToString(iter->second);
		str2 += "\n";
		printLog(str2);

	}
	printLog(str);

	str = "ClassMemoryTracer[Destructor]\n";
	printLog(str);
	auto iter1 = s_mapRefDestructor.cbegin();
	for (; iter1 != s_mapRefDestructor.cend(); ++iter1)
	{
		str2 = iter1->first;
		str2 += ": ";
		str2 += intToString(iter1->second);
		str2 += "\n";
		printLog(str2);
	}
	printLog(str);
	m_lock->unLock();
}

Lock::Lock(void)
{
	InitializeCriticalSection(&m_cs);
}

Lock::~Lock(void)
{
	DeleteCriticalSection(&m_cs);
}

bool Lock::lock()
{
	EnterCriticalSection(&m_cs);
	return true;
}

bool Lock::unLock()
{
	LeaveCriticalSection(&m_cs);
	return true;
}