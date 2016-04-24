//////////////////////////////////////////////////////////////////////////
//��д��

//////////////////////////////////////////////////////////////////////////
#ifndef  _RWMETUX_H_
#define  _RWMETUX_H_

//////////////////////////////////////////////////////////////////////////
#ifndef  WIN32
  #include <pthread.h>
#endif

//////////////////////////////////////////////////////////////////////////
// ��֤������д����
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

	BOOL  AcquireReadLock(void);    //�����������,���󲻳ɹ�������
	BOOL  ReleaseReadLock(void);    //�ͷŶ�������

	BOOL  AcquireWriteLock(void);   //����д������,���󲻳ɹ�������
	BOOL  ReleaseWriteLock(void);   //�ͷ�д������

private:
	BOOL  CRWLock::MyWaitForSingleObject(HANDLE hObject);
	BOOL  CRWLock::InitRWLock();
	BOOL  CRWLock::DestroyRWLock();
private:

#ifdef  WIN32
	RWLock	m_RWLock;   //��д��
#else
	pthread_rwlock_t m_RWLock;
#endif
	
};

#endif   //_RWMETUX_H_
