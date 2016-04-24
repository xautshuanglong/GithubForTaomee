//////////////////////////////////////////////////////////////////////////
//读写锁

//////////////////////////////////////////////////////////////////////////
#ifndef  _RWMETUX_H_
#define  _RWMETUX_H_

//////////////////////////////////////////////////////////////////////////
#ifndef  WIN32
  #include <pthread.h>
#endif

//////////////////////////////////////////////////////////////////////////
// 多证书链读写锁类
#ifdef  WIN32

typedef struct _RWLock
{
	HANDLE	hMutex;    
	HANDLE	hDataLock;
	int		nReaderCount;
} RWLock;

#endif

class  CRWLock
{
public:
	CRWLock();
	~CRWLock();

	BOOL  AcquireReadLock(void);    //请求读操作锁,请求不成功则阻塞
	BOOL  ReleaseReadLock(void);    //释放读操作锁

	BOOL  AcquireWriteLock(void);   //请求写操作锁,请求不成功则阻塞
	BOOL  ReleaseWriteLock(void);   //释放写操作锁

private:
	BOOL  CRWLock::MyWaitForSingleObject(HANDLE hObject);
	BOOL  CRWLock::InitRWLock();
	BOOL  CRWLock::DestroyRWLock();
private:

#ifdef  WIN32
	RWLock	m_RWLock;   //读写锁
#else
	pthread_rwlock_t m_RWLock;
#endif
	
};

#endif   //_RWMETUX_H_
