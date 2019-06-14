/*////////////////////////////////////////////////////////////////////
@Brief:		追踪C++类的内存分配和释放
@Author:	vilas wang
@Contact:	QQ451930733|451930733@qq.com

【用法】

1：预定义宏TRACE_CLASS_MEMORY_ENABLED

2：在需要追踪的类的构造函数和析构函数打标记
例：
Class A
{
public:
A() { TRACE_CLASS_CONSTRUCTOR(A); }
~A() { TRACE_CLASS_DESTRUCTOR(A); }
}

3: 最后等需要知道类内存分配和释放情况的时候(比如程序退出前)打印信息
TRACE_CLASS_CHECK_LEAKS();
///////////////////////////////////////////////////////////////////////////////////////*/

#pragma once
#include <windows.h>
#include <string>
#include <map>
#include "lock.h"


#ifndef TRACE_CLASS_CONSTRUCTOR
#ifdef TRACE_CLASS_MEMORY_ENABLED
#define TRACE_CLASS_CONSTRUCTOR(T) VCUtility::ClassMemoryTracer::addRef<T>()
#else
#define TRACE_CLASS_CONSTRUCTOR(T) __noop
#endif
#endif

#ifndef TRACE_CLASS_DESTRUCTOR
#ifdef TRACE_CLASS_MEMORY_ENABLED
#define TRACE_CLASS_DESTRUCTOR(T) VCUtility::ClassMemoryTracer::release<T>()
#else
#define TRACE_CLASS_DESTRUCTOR(T) __noop
#endif
#endif

#ifndef TRACE_CLASS_CHECK_LEAKS
#ifdef TRACE_CLASS_MEMORY_ENABLED
#define TRACE_CLASS_CHECK_LEAKS() VCUtility::ClassMemoryTracer::checkMemoryLeaks()
#else
#define TRACE_CLASS_CHECK_LEAKS() __noop
#endif
#endif

namespace VCUtility {

    class ClassMemoryTracer
    {
    private:
        typedef std::map<size_t, std::pair<std::string, int>> TClassRefCount;
        static TClassRefCount s_mapRefCount;
        static CSLock m_lock;

    public:
        template <typename T>
        static void addRef()
        {
            const size_t hashcode = typeid(T).hash_code();

            Locker<CSLock> locker(m_lock);
            auto iter = s_mapRefCount.find(hashcode);
            if (iter == s_mapRefCount.end())
            {
                const char *name = typeid(T).name();
                s_mapRefCount[hashcode] = std::make_pair<std::string, int>(std::string(name), 1);
            }
            else
            {
                ++iter->second.second;
            }
        }

        template <typename T>
        static void release()
        {
            const size_t hashcode = typeid(T).hash_code();

            Locker<CSLock> locker(m_lock);
            auto iter = s_mapRefCount.find(hashcode);
            if (iter != s_mapRefCount.end())
            {
                if (iter->second.second > 0)
                {
                    --iter->second.second;
                }
            }
        }

        static void checkMemoryLeaks();

    private:
        ClassMemoryTracer() {}
        ~ClassMemoryTracer() {}
        ClassMemoryTracer(const ClassMemoryTracer &);
        ClassMemoryTracer &operator=(const ClassMemoryTracer &);
    };
}
