#include "ClassMemoryTracer.h"
#include <sstream>

using namespace CVC;

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
	Locker<Lock> locker(*m_lock.get());
	std::ostringstream oss;
	try
	{
		if (!s_mapRefConstructor.empty())
		{
			oss << "ClassMemoryTracer[Constructor]\n";

			auto iter = s_mapRefConstructor.cbegin();
			for (; iter != s_mapRefConstructor.cend(); ++iter)
			{
				oss << iter->first;
				oss << ": ";
				oss << intToString(iter->second);
				oss << "\n";
			}
			oss << "ClassMemoryTracer[Constructor]\n";
			Log_Debug(oss.str());
		}
		
		if (!s_mapRefDestructor.empty())
		{
			oss.str("");
			oss << "ClassMemoryTracer[Destructor]\n";

			auto iter1 = s_mapRefDestructor.cbegin();
			for (; iter1 != s_mapRefDestructor.cend(); ++iter1)
			{
				oss << iter1->first;
				oss << ": ";
				oss << intToString(iter1->second);
				oss << "\n";
			}
			oss << "ClassMemoryTracer[Destructor]\n";
			Log_Debug(oss.str());
		}
	}
	catch (std::exception* e)
	{
		oss.str("");
		oss << "ClassMemoryTracer::printInfo() exception: "
			<< std::string(e->what())
			<< "\n";
		Log_Debug(oss.str());
	}
	catch (...)
	{
		oss.str("");
		oss << "ClassMemoryTracer::printInfo() exception: "
			<< intToString(GetLastError())
			<< "\n";
		Log_Debug(oss.str());
	}
}