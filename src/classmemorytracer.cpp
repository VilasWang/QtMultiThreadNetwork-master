#include "classmemorytracer.h"
#include <sstream>
#include <vector>
#include <algorithm>

namespace VCUtil
{
    CSLock ClassMemoryTracer::m_lock;
    ClassMemoryTracer::TClassRefCount ClassMemoryTracer::s_mapRefCount;

    void LogDebug(const std::string& str)
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
        Locker<CSLock> locker(m_lock);
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
                        vecString.emplace_back(oss.str());
                    }
                }

                if (!vecString.empty())
                {
                    LogDebug("ClassMemoryTracer Detect Memory Leaks...\n");
                    std::for_each(vecString.cbegin(), vecString.cend(), [](const std::string& str) {
                        LogDebug(str);
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
            LogDebug(oss.str());
        }
        catch (...)
        {
            oss.str("");
            oss << "ClassMemoryTracer::checkMemoryLeaks() exception: "
                << intToString(GetLastError())
                << "\n";
            LogDebug(oss.str());
        }
    }
}

