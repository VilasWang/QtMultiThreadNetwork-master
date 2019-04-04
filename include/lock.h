#pragma once

#include <windows.h>
#include <memory>

namespace CVC
{
    class Lock
    {
    public:
        Lock();
        ~Lock();

    public:
        void lock();
        void unlock();

    private:
#if _MSC_VER >= 1700
        Lock(const Lock&) = delete;
        Lock& operator=(const Lock&) = delete;
#else
        Lock(const Lock&);
        Lock& operator=(const Lock&);
#endif

    private:
        CRITICAL_SECTION m_cs;
    };

    template<class _Lock>
    class Locker
    {
    public:
        explicit Locker(_Lock& lock)
            : m_lock(lock)
        {
            m_lock.lock();
        }

        Locker(_Lock& lock, bool bShared)
            : m_lock(lock)
        {
            m_lock.lock(bShared);
        }

#if _MSC_VER >= 1700
        ~Locker() _NOEXCEPT
#else
        ~Locker()
#endif
        {
            m_lock.unlock();
        }

    private:
#if _MSC_VER >= 1700
        Locker(const Locker&) = delete;
        Locker& operator=(const Locker&) = delete;
#else
        Locker(const Locker&);
        Locker& operator=(const Locker&);
#endif

    private:
        _Lock& m_lock;
    };
}
