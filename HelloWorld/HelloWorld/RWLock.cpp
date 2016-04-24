//////////////////////////////////////////////////////////////////////////
//¶ÁÐ´Ëø

//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "RWLock.h"
//////////////////////////////////////////////////////////////////////////

#ifndef FALSE 
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

//Íê³É¶ÁÐ´Ëø³õÊ¼»¯
CRWLock::CRWLock()
{
	InitRWLock();
}

//
CRWLock::~CRWLock()
{
	DestroyRWLock();
}

BOOL  CRWLock::MyWaitForSingleObject(HANDLE hObject)
{
#ifdef WIN32

	DWORD result;

	result = WaitForSingleObject(hObject, INFINITE);

	return (result == WAIT_OBJECT_0);

#endif
}

BOOL  CRWLock::InitRWLock()
{
#ifdef WIN32

	m_RWLock.nReaderCount = 0;
	m_RWLock.hDataLock = CreateSemaphore(NULL, 1, 1, NULL);
	if (m_RWLock.hDataLock == NULL)
	{
		return FALSE;
	}

	m_RWLock.hMutex = CreateMutex(NULL, FALSE, NULL);
	if (m_RWLock.hMutex == NULL)
	{
		CloseHandle(m_RWLock.hDataLock);
		return FALSE;
	}

	return TRUE;

#else

	 pthread_rwlock_init(&m_RWLock, NULL);

	 return TRUE;
#endif

}

BOOL  CRWLock::DestroyRWLock()
{
#ifdef WIN32

	DWORD result = WaitForSingleObject(m_RWLock.hDataLock, INFINITE);
	if (result != WAIT_OBJECT_0)
	{
		return FALSE;
	}

	CloseHandle(m_RWLock.hMutex);
	CloseHandle(m_RWLock.hDataLock);
	return TRUE;

#else

	pthread_rwlock_destroy(&m_RWLock);

	return TRUE;

#endif
}

BOOL CRWLock::AcquireReadLock()
{
	BOOL result = TRUE;

#ifdef WIN32

	if (MyWaitForSingleObject(m_RWLock.hMutex) == FALSE)
	{
		return FALSE;
	}

	m_RWLock.nReaderCount ++;
	if(m_RWLock.nReaderCount == 1)
	{
		result = MyWaitForSingleObject(m_RWLock.hDataLock);
	}

	ReleaseMutex(m_RWLock.hMutex);

	return result;

#else
	pthread_rwlock_rdlock(&m_RWLock);

	return result;
#endif
}


BOOL CRWLock::ReleaseReadLock()
{
	int result = TRUE;

#ifdef WIN32

	LONG lPrevCount;

	if (MyWaitForSingleObject(m_RWLock.hMutex) == FALSE)
	{
		return FALSE;
	}

	--m_RWLock.nReaderCount;

	if (m_RWLock.nReaderCount == 0)
	{
		result = ReleaseSemaphore(m_RWLock.hDataLock, 1, &lPrevCount);
	}

	ReleaseMutex(m_RWLock.hMutex);

	return result;
#else

	return pthread_rwlock_unlock(&m_RWLock);	

#endif
}

BOOL CRWLock::AcquireWriteLock()
{

#ifdef WIN32

	return MyWaitForSingleObject(m_RWLock.hDataLock);

#else
	
	return pthread_rwlock_wrlock(&m_RWLock);

#endif
}


BOOL CRWLock::ReleaseWriteLock()
{
#ifdef WIN32

	int result = TRUE;
	LONG lPrevCount;

	result = ReleaseSemaphore(m_RWLock.hDataLock, 1, &lPrevCount);
	if (lPrevCount != 0)
	{
		return FALSE;
	}

	return result;

#else
	return  pthread_rwlock_unlock(&m_RWLock);
#endif
}
