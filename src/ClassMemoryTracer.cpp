#include "classmemorytracer.h"
#include <sstream>
#include <vector>
#include <algorithm>

using namespace CVC;

Lock ClassMemoryTracer::m_lock;
ClassMemoryTracer::TClassRefCount ClassMemoryTracer::s_mapRefCount;

void Log_Debug(std::string str)
{
    if (!str.empty())
    {
        OutputDebugStringA(str.c_str());
    }
}

std::string intToString(const int n)
{
	std::stringstream str;
	str << n;
	return str.str();
}

void ClassMemoryTracer::checkMemoryLeaks()
{
	Locker<Lock> locker(m_lock);
	std::ostringstream oss;
	try
	{
        std::vector<std::string> vecString;
		if (!s_mapRefCount.empty())
		{
			auto iter = s_mapRefCount.cbegin();
			for (; iter != s_mapRefCount.cend(); ++iter)
			{
                oss.str("");
                if (iter->second.second > 0)
                {
                    oss << iter->second.first;
                    oss << ": leak ";
                    oss << intToString(iter->second.second);
                    oss << " objects \n";
                    vecString.push_back(oss.str());
                }
			}

            if (!vecString.empty())
            {
                Log_Debug("ClassMemoryTracer Detect Memory Leaks...\n");
                std::for_each(vecString.cbegin(), vecString.cend(), [](const std::string& str) {
                    Log_Debug(str);
                });
            }
		}
	}
	catch (std::exception* e)
	{
		oss.str("");
		oss << "ClassMemoryTracer::checkMemoryLeaks() exception(std::exception): "
			<< std::string(e->what())
			<< "\n";
		Log_Debug(oss.str());
	}
	catch (...)
	{
		oss.str("");
		oss << "ClassMemoryTracer::checkMemoryLeaks() exception: "
			<< intToString(GetLastError())
			<< "\n";
		Log_Debug(oss.str());
	}
}