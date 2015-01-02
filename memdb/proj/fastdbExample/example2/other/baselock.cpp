#include <util/baselock.h>

#include <util/thread.h>
#include <stdio.h>

#ifdef _WIN32
#pragma warning(disable:4996)
#endif

////////////////////////////////////////////////////////////////////////////
//
//
//				class CBaseLock
//
//
////////////////////////////////////////////////////////////////////////////
int CBaseLock::serialno = 0;
CBaseLock::CBaseLock()
{
	m_nLockCount = 0;
	this->bInitialized = false;
#ifdef WIN32
	sprintf(name,"PID[%d]::Synchronizer%d",GetCurrentProcessId(),serialno++);
#else
	sprintf(name,"PID[%d]::Synchronizer%d",getpid(),serialno++);
#endif
#ifdef WIN32
	this->m_condObj = NULL;
	this->m_mutexObj = NULL;
	this->m_OwnThread = NULL;
#endif
}

//��ʼ��������Դ�����������������������(����UNIXϵͳ��Ч)
bool CBaseLock::initialize()
{
	//�����߳�˽�б�������
	if ( ! m_Synchronizer.initialize() )
	{
#ifdef _THDEBUG
#ifdef WIN32
		xprint("Create TSD Object Failed[%d]",GetLastError());
#else
		xprint("Create TSD Object Failed");
#endif
#endif
		return false;
	}

#ifdef WIN32
	//����Ƿ��Ѿ�����ͬ����MUTEX����
	m_mutexObj = OpenMutex(MUTEX_ALL_ACCESS,false,name);
	if ( m_mutexObj != NULL )
	{
#ifdef _THDEBUG
		xprint("WINDOWX Mutex Object Already exists\n");
#endif
		CloseHandle(m_mutexObj);
		return false;
	}
	//����false �������������ϻ��Mutex����ķ���Ȩ��
	//�����߳���Ҫȡ�÷���Ȩ��ʱ�������wait function���ܻ��
	m_mutexObj = CreateMutex(NULL,false,name);
	if ( m_mutexObj == NULL )
	{
#ifdef _THDEBUG
		xprint("Create WINDOWS Mutex Object Failed[%d]",GetLastError());
#endif
		return false;
	}

	//�����µ�EVENTΨһ��ʶ��
	strcat(name,"EVENT");
	//����Ƿ��Ѿ�����ͬ����EVENT����
	m_condObj = OpenEvent(EVENT_ALL_ACCESS,false,name);
	if ( m_condObj != NULL )
	{
#ifdef _THDEBUG
		xprint("WINDOW Event Object Already exists\n");
#endif
		CloseHandle(m_condObj);
		return false;
	}

	//��һ����true ��������EVENT���󱻻�ȡ�󲻻����ϱ�ʶ��nonsignaled��Ҫ����ResetEvent()�ſ���
	//�ڶ�������false�����������״̬��nonsignaled
	m_condObj = CreateEvent(NULL,true,false,name);
	if ( m_condObj == NULL )
	{
#ifdef _THDEBUG
		xprint("Create WINDOWS Mutex Object Failed[%d]",GetLastError());
#endif
		return false;
	}

//end of WIN32 INITIALIZED
#else
	int retval = 0 ;
	if ( ( retval = pthread_mutex_init(&m_mutexObj,NULL) ) != 0 )
	{
#ifdef _THDEBUG
		xprint("Create UNIX Mutex Object Failed[%d]",retval);
#endif
		return false;
	}
	if ( ( retval = pthread_cond_init(&m_condObj,NULL) ) != 0 )
	{
#ifdef _THDEBUG
		xprint("Create UNIX Condition Object Failed[%d]",retval);
#endif
		pthread_mutex_destroy(&m_mutexObj);
		return false;
	}
#endif
	this->bInitialized = true;
	return true;
}

//�����������ر���Դ������
CBaseLock::~CBaseLock()
{
#ifdef WIN32
	if ( m_condObj != NULL )
		SetEvent(m_condObj);
#else
	pthread_cond_broadcast(&this->m_condObj);
#endif

#ifdef WIN32
	if ( m_condObj != NULL )
		CloseHandle(m_condObj);
	if ( m_mutexObj != NULL )
		CloseHandle(m_mutexObj);
#else
	pthread_cond_destroy(&m_condObj);
	pthread_mutex_destroy(&m_mutexObj);
#endif
}

//�Ա�����������������ʼ����ͬ������
bool CBaseLock::lock()
{
	if ( !this->bInitialized )
	{
		//printf("name lock error[%s]\n",name);
		throw CThreadException("No Initialize--Lock Error");
	}
	int state = 0;

	int* p = (int*)m_Synchronizer.getValue();
	//���������֤��������ȡ������Դ�����ٽ��л�ȡ
	if ( p )
	{
		//���Ӽ���
		m_nLockCount ++;
		return true;
	}

//*****************��ȡͬ������Դ***************************
#ifdef WIN32
	if ( WaitForSingleObject(this->m_mutexObj,INFINITE) == WAIT_FAILED )
	{
	#ifdef _THDEBUG
			xprint("Lock Error[%d]",GetLastError());
	#endif
		return false;
	}
	state = 1;
#else
		int retval = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&state);
		retval = pthread_mutex_lock(&this->m_mutexObj);
#endif

#ifdef _THDEBUG
	#ifdef WIN32
		xprint("Lock Mutex[%d]",CThread::getCurrentThreadId());
	#endif
#endif

//**********************���̱߳��ر������и�ֵ***********************
	p = (int*)(state+1);
	m_Synchronizer.setValue(p);
	m_nLockCount++;
#ifdef _DEBUG
	m_OwnThread = CThread::getCurrentThreadId();
#endif
	return true;
}

//�Ա��������������ͬ���������
bool CBaseLock::unlock()
{
	if ( !this->bInitialized )
		throw CThreadException("UnLock Error--Not Initialize");
//*****************��ȡ�̱߳��ر�����������Դ���************************
	int *p = (int*)m_Synchronizer.getValue();
	if ( !p )
	{
		return false;
	}
	long state = (long)p-1;
	//����Ϊ0���������
	if ( (--m_nLockCount) > 0 )
		return false;
	m_Synchronizer.setValue(0);

#ifdef _THDEBUG
	xprint("Release Mutex[%d]",CThread::getCurrentThreadId());
#endif

#ifdef WIN32
	if ( ! ReleaseMutex(this->m_mutexObj) )
	{
	#ifdef _THDEBUG
			xprint("Release Mutex Error[%d]",GetLastError());
	#endif
		return false;
	}
#else
	pthread_mutex_unlock(&this->m_mutexObj);
	pthread_setcancelstate(state,NULL);
	pthread_testcancel();
#endif

#ifdef _DEBUG
	#ifdef WIN32
		m_OwnThread = NULL;
	#else
		memset(&m_OwnThread,0,sizeof(m_OwnThread));
	#endif
#endif
	return true;
}

void  CBaseLock::notify()
{
	int *p = (int*)m_Synchronizer.getValue();
	if ( !p )
		throw CThreadException("Notify1 Wrong No in Synchronize Mode");

#ifdef WIN32
	PulseEvent(m_condObj);
#else
	pthread_cond_signal(&this->m_condObj);
#endif
}

void CBaseLock::notifyAll()
{
	int *p = (int*)m_Synchronizer.getValue();
	if ( !p )
		throw CThreadException("NotifyAll1 Wrong No in Synchronize Mode");

#ifdef WIN32
	SetEvent(m_condObj);
#else
	pthread_cond_broadcast(&this->m_condObj);
#endif
}

void CBaseLock::wait()
{
	if ( ! this->bInitialized )
		throw CThreadException("Wait Wrong No in Synchronize Mode");

	int *p = (int*)m_Synchronizer.getValue();
	if ( !p )
		throw CThreadException("Wait1 Wrong No in Synchronize Mode");

#ifdef WIN32
	HANDLE th = CThread::getCurrentThread();
	if ( th != NULL )
	{
	#ifdef _THDEBUG
			xprint("Current Handle[%d],Id[%d]",GetCurrentThread(),th);
	#endif
		if ( !ResetEvent(m_condObj) )
		{
	#ifdef _THDEBUG
			xprint("Reset Event Error[%d]",WSAGetLastError());
	#endif
		}
		//wait��ʱ�����ͳһ�Ľ���
		int waitcount = m_nLockCount;
		m_nLockCount = 0 ;
		//��Ϊ��������µ���lock������������Ҫ��tsd��Ϊ0
		m_Synchronizer.setValue(0);
		SignalObjectAndWait(m_mutexObj,m_condObj,INFINITE,false);
		//�رվ����Դ
		CloseHandle(th);
		//���»����Դ
		this->lock();
		m_nLockCount = waitcount;
	}
	#ifdef _THDEBUG
		else
			xprint("Duplite Handle Error[%d]",GetLastError());
	#endif
#else//end of ifdef WIN32
	int waitcount = m_nLockCount;
	m_nLockCount = 0 ;
	#ifdef _THDEBUG
		xprint("Condition Wait in UNIX ");
		if (pthread_cond_wait(&this->m_condObj,&this->m_mutexObj) == 0 )
			xprint("Wait Unix Error [%d] ",errno);
	#else
		pthread_cond_wait(&this->m_condObj,&this->m_mutexObj);
	#endif//end of ifdef _THDEBUG
	m_nLockCount = waitcount;
#endif
}

void CBaseLock::wait(long waitmillsseconds)
{
	if ( ! this->bInitialized )
		throw CThreadException("Wait Wrong No in Synchronize Mode");
	int *p = (int*)m_Synchronizer.getValue();
	if ( !p )
		throw CThreadException("WaitTime1 Wrong No in Synchronize Mode");
#ifdef WIN32
	HANDLE th = CThread::getCurrentThread();
	if ( th != NULL )
	{
	#ifdef _THDEBUG
			xprint("Current Handle[%d],Id[%d]",GetCurrentThread(),th);
	#endif
		if ( !ResetEvent(m_condObj) )
		{
	#ifdef _THDEBUG
			xprint("Reset Event Error[%d]",WSAGetLastError());
	#endif
		}
		int waitcount = m_nLockCount;
		//���Բ�����Ƕ�׵���lock/unlock��ʱ�������wait���ַ��������Σ�ա�
		m_nLockCount = 0 ;
		//
		m_Synchronizer.setValue(0);
		SignalObjectAndWait(m_mutexObj,m_condObj,waitmillsseconds,false);
		//�رվ����Դ
		CloseHandle(th);
		//���»����Դ
		this->lock();
		m_nLockCount = waitcount;
	}
	#ifdef _THDEBUG
		else
			xprint("Duplite Handle Error[%d]",GetLastError());
	#endif
#else//end of ifdef WIN32
	int waitcount = m_nLockCount;
	m_nLockCount = 0 ;
	struct timespec delay;
	//��ȡ��ǰ����ʱ��
	if ( !clock_gettime(CLOCK_REALTIME,&delay) )
	{
		delay.tv_sec += (int)(waitmillsseconds / 1000);
		delay.tv_nsec += (waitmillsseconds % 1000)*1000000;
	}
	else
	{
		delay.tv_sec = time(NULL) + (int)(waitmillsseconds / 1000);
		//����--10��9�η�
		delay.tv_nsec = (waitmillsseconds % 1000)*1000000;
	}
	#ifdef _THDEBUG
		xprint("Condition Wait in UNIX ");
		if (pthread_cond_timedwait(&this->m_condObj,&this->m_mutexObj,&delay) == 0 )
		{
			perror("test");
			xprint("Wait Unix Error [%d] ",errno);
		}
	#else
		pthread_cond_timedwait(&this->m_condObj,&this->m_mutexObj,&delay);
	#endif//end of ifdef _THDEBUG
	m_nLockCount = waitcount;
#endif
}

