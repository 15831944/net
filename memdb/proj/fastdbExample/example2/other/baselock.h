#if !defined(_BASELOCK_H_)
#define _BASELOCK_H_

#ifdef WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x410
#endif
#include <windows.h>
#include <winbase.h>
#else
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef THREADHANDLE
#undef THREADHANDLE
#endif
#ifdef THREADID
#undef THREADID
#endif

#ifdef WIN32
#define MUTEXHANDLE HANDLE
#define CONDHANDLE HANDLE
#define THREADHANDLE HANDLE
#define THREADID unsigned int
#else
#define MUTEXHANDLE pthread_mutex_t
#define CONDHANDLE  pthread_cond_t
#define THREADHANDLE pthread_t
#define THREADID pthread_t
#endif

#include "tlobject.h"
#include "mylock.h"

/*
�߳���
*/
class CBaseLock : public ILock
{
private :
	//��������
	unsigned int m_nLockCount;
	static int serialno;
	MUTEXHANDLE m_mutexObj;
	CONDHANDLE  m_condObj;
	//Ψһ��ʶ������
	char name[100];
	//��ʶ��ǰ�߳��Ƿ��Ѿ����ͬ����Դ
	//�߳����ݣ�����ʶ��ǰ�߳��Ƿ��Ѿ����ͬ����Դ
	CTlObject m_Synchronizer;
	bool bInitialized ;
	CBaseLock(const CBaseLock& Other);
	CBaseLock& operator= (const CBaseLock& Other);
	THREADID m_OwnThread;

public :
	CBaseLock();
	virtual ~CBaseLock();
	//��ʼ��������Դ
	bool initialize();
	//��ͬ����Դ������������ÿ���̴߳���һ��ָ��
	bool lock();
	//��ͬ����Դ������������ÿ���߳��Լ���ָ�����ɾ��
	bool unlock();
	//�õ�ǰ�߳̽���ȴ�
	void wait();
	//���߳̽���ȴ�״̬������ʱ��(��λ������)���������
	void wait(long waitmillsseconds);
	//�����������ڵȴ����߳�
	void notifyAll();
	//����ָ�����߳�
	void notify();
};


#endif
