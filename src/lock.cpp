#include "lock.h"

namespace CVC {
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
}
