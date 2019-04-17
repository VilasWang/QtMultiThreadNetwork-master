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
TRACE_CLASS_PRINT();
///////////////////////////////////////////////////////////////////////////////////////*/

#pragma once
#include <windows.h>
#include <string>
#include <map>
#include "lock.h"


#ifndef TRACE_CLASS_CONSTRUCTOR
#ifdef TRACE_CLASS_MEMORY_ENABLED
#define TRACE_CLASS_CONSTRUCTOR(T) CVC::ClassMemoryTracer::addRef<T>()
#else
#define TRACE_CLASS_CONSTRUCTOR(T) __noop
#endif
#endif

#ifndef TRACE_CLASS_DESTRUCTOR
#ifdef TRACE_CLASS_MEMORY_ENABLED
#define TRACE_CLASS_DESTRUCTOR(T) CVC::ClassMemoryTracer::release<T>()
#else
#define TRACE_CLASS_DESTRUCTOR(T) __noop
#endif
#endif

#ifndef TRACE_CLASS_PRINT
#ifdef TRACE_CLASS_MEMORY_ENABLED
#define TRACE_CLASS_PRINT() CVC::ClassMemoryTracer::printInfo()
#else
#define TRACE_CLASS_PRINT __noop
#endif
#endif

namespace CVC {

    class ClassMemoryTracer
    {
    private:
        typedef std::map<std::string, int> TClassRefCount;
        static TClassRefCount s_mapRefConstructor;
        static TClassRefCount s_mapRefDestructor;
        static Lock m_lock;

    public:
        template <class T>
        static void addRef()
        {
            const char *name = typeid(T).name();
            std::string str(name);
            if (str.empty())
            {
                return;
            }

            Locker<Lock> locker(m_lock);
            auto iter = s_mapRefConstructor.find(str);
            if (iter == s_mapRefConstructor.end())
            {
                s_mapRefConstructor[str] = 1;
            }
            else
            {
                s_mapRefConstructor[str] = ++iter->second;
            }
        }

        template <class T>
        static void release()
        {
            const char *name = typeid(T).name();
            std::string str(name);
            if (str.empty())
            {
                return;
            }

            Locker<Lock> locker(m_lock);
            auto iter = s_mapRefDestructor.find(str);
            if (iter == s_mapRefDestructor.end())
            {
                s_mapRefDestructor[str] = 1;
            }
            else
            {
                s_mapRefDestructor[str] = ++iter->second;
            }
        }

        static void printInfo();

    private:
        ClassMemoryTracer() {}
        ~ClassMemoryTracer() {}
        ClassMemoryTracer(const ClassMemoryTracer &);
        ClassMemoryTracer &operator=(const ClassMemoryTracer &);
    };
}
