#include "ClassMemoryTracer.h"
#include <sstream>

#if _MSC_VER >= 1700
std::unique_ptr<Lock> ClassMemoryTracer::m_lock(new Lock);
#else
std::shared_ptr<Lock> ClassMemoryTracer::m_lock(new Lock);
#endif
TClassRefCount ClassMemoryTracer::s_mapRefConstructor;
TClassRefCount ClassMemoryTracer::s_mapRefDestructor;

void Log_Debug(std::string str)
{
	OutputDebugStringA(str.c_str());
}

std::string intToString(const int n)
{
	std::stringstream str;
	str << n;
	return str.str();
}

void ClassMemoryTracer::printInfo()
{
	std::string str;
	Locker2<Lock> locker(*m_lock.get());

	try
	{
		str = "ClassMemoryTracer[Constructor]\n";
		Log_Debug(str);

		auto iter = s_mapRefConstructor.cbegin();
		for (; iter != s_mapRefConstructor.cend(); ++iter)
		{
			str = iter->first;
			str += ": ";
			str += intToString(iter->second);
			str += "\n";
			Log_Debug(str);

		}
		str = "ClassMemoryTracer[Constructor]\n";
		Log_Debug(str);

		str = "ClassMemoryTracer[Destructor]\n";
		Log_Debug(str);

		auto iter1 = s_mapRefDestructor.cbegin();
		for (; iter1 != s_mapRefDestructor.cend(); ++iter1)
		{
			str = iter1->first;
			str += ": ";
			str += intToString(iter1->second);
			str += "\n";
			Log_Debug(str);
		}
		str = "ClassMemoryTracer[Destructor]\n";
		Log_Debug(str);
	}
	catch (std::exception* e)
	{
		str = "ClassMemoryTracer::printInfo() exception: ";
		str += std::string(e->what());
		str += "\n";
		Log_Debug(str);
	}
	catch (...)
	{
		str = "ClassMemoryTracer::printInfo() exception: ";
		str += intToString(GetLastError());
		str += "\n";
		Log_Debug(str);
	}
}

Lock::Lock()
{
	InitializeCriticalSection(&m_cs);
}

Lock::~Lock()
{
	DeleteCriticalSection(&m_cs);
}

void Lock::lock()
{
	EnterCriticalSection(&m_cs);
}

void Lock::unlock()
{
	LeaveCriticalSection(&m_cs);
}