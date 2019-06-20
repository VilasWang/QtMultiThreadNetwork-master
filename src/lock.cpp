//#include "stdafx.h"
#include "Lock.h"
#include <winnt.h>

using namespace VCUtil;

CSLock::CSLock()
{
    InitializeCriticalSection(&m_cs);
}

CSLock::~CSLock()
{
    DeleteCriticalSection(&m_cs);
}

bool CSLock::tryLock()
{
    return TryEnterCriticalSection(&m_cs);
}

void CSLock::lock()
{
    EnterCriticalSection(&m_cs);
}

void CSLock::unlock()
{
    LeaveCriticalSection(&m_cs);
}


SRWLock::SRWLock()
    : m_bSharedLocked(FALSE)
    , m_bExclusiveLocked(FALSE)
{
    InitializeSRWLock(&m_lock);
}

SRWLock::~SRWLock()
{
    unlock();
}

bool SRWLock::tryLock(bool bShared)
{
    BOOL success = FALSE;
    if (bShared)
    {
        success = TryAcquireSRWLockShared(&m_lock);
        InterlockedExchange(&m_bSharedLocked, TRUE);
    }
    else
    {
        success = TryAcquireSRWLockExclusive(&m_lock);
        InterlockedExchange(&m_bExclusiveLocked, TRUE);
    }
    return (bool)success;
}

void SRWLock::lock(bool bShared)
{
    if (bShared)
    {
        AcquireSRWLockShared(&m_lock);
        InterlockedExchange(&m_bSharedLocked, TRUE);
    }
    else
    {
        AcquireSRWLockExclusive(&m_lock);
        InterlockedExchange(&m_bExclusiveLocked, TRUE);
    }
}

void SRWLock::unlock()
{
    if (TRUE == InterlockedExchange(&m_bSharedLocked, FALSE))
    {
        ReleaseSRWLockShared(&m_lock);
    }
    else if (TRUE == InterlockedExchange(&m_bExclusiveLocked, FALSE))
    {
        ReleaseSRWLockExclusive(&m_lock);
    }
}