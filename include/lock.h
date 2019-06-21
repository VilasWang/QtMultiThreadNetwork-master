/*
@Brief:		windowsËø
@Author:	vilas wang
@Contact:	QQ451930733
*/

#pragma once
#include <windows.h>
#include <memory>

namespace VCUtil {

    //Class CSLock - ¹Ø¼ü¶ÎËø
    class CSLock
    {
    public:
        CSLock();
        ~CSLock();

        virtual void lock();
        virtual bool tryLock();
        virtual void unlock();

    private:
        CSLock(const CSLock &);
        CSLock &operator=(const CSLock &);

    private:
        CRITICAL_SECTION m_cs;
    };

    //Class SRWLock - slim ¶ÁÐ´Ëø
    class SRWLock
    {
    public:
        SRWLock();
        ~SRWLock();

        virtual void lock(bool bShared = false);
        virtual bool tryLock(bool bShared = false);
        virtual void unlock();

    private:
        SRWLock(const SRWLock &);
        SRWLock &operator=(const SRWLock &);

    private:
        SRWLOCK m_lock;
        long m_bSharedLocked;
        long m_bExclusiveLocked;
    };

    template<typename _Lock, typename Enable = void>
    class Locker
    {
    public:
        explicit Locker(_Lock&) {}
        ~Locker() {}

    private:
        Locker(const Locker&);
        Locker& operator=(const Locker&);
    };

    template<typename _Lock>
    class Locker<_Lock, typename std::enable_if<std::is_base_of<CSLock, _Lock>::value
        || std::is_base_of<SRWLock, _Lock>::value>::type>
    {
    public:
        explicit Locker(_Lock& lock) : m_lock(lock)
        {
            m_lock.lock();
        }

        Locker(_Lock& lock, bool bShared) : m_lock(lock)
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
        Locker(const Locker&);
        Locker& operator=(const Locker&);

    private:
        _Lock& m_lock;
    };
}