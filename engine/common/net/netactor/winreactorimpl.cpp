#define FD_SETSIZE 10240
#include "../../public.h"

#ifdef WINDOWS
#include "winreactorsocketimpl.h"
#include "winreactorimpl.h"
#include "../../util/datetime.h"


/*

		Class	CWinReactorThread

*/
CWinReactorThread::CWinReactorThread(HANDLE& hCompletionHandle) : m_hCompletionHandle(hCompletionHandle)
{
	m_pThread = NULL;
	m_nListenSocket = INVALID_SOCKET;
}

CWinReactorThread::~CWinReactorThread()
{
	delete m_pThread;
}

void CWinReactorThread::run()
{
	m_bRunFlag = true;
	while ( m_bRunFlag )
	{
		fd_set writeset;
		FD_ZERO(&writeset);
		timeval timeout;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
		SOCKET maxfd = m_nListenSocket;
		FD_SET(m_nListenSocket,&writeset);
		{
			CAutoLock lock(m_ClientLock);
			if ( m_nListenSocket == INVALID_SOCKET )
			{
				m_nListenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			}
			for ( map<SOCKET,void*>::iterator i = m_Clients.begin() ; i != m_Clients.end() ;  i ++ )
			{
				FD_SET((*i).first,&writeset);
				if ( (*i).first > maxfd )
					maxfd = (*i).first;
			}
		}
		int32 current = CDateTime().getTotalSecs();
		int retval = select(maxfd+1,NULL,&writeset,NULL,&timeout);
		if ( retval < 0 )
			continue;
		{
			CAutoLock lock(m_ClientLock);
			if ( m_nListenSocket == INVALID_SOCKET )
			{
				m_nListenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			}
			for ( map<SOCKET,void*>::iterator i = m_Clients.begin() ; i != m_Clients.end() ;  )
			{
				if ( FD_ISSET((*i).first,&writeset) )
				{
					//���ӳɹ�
					PostQueuedCompletionStatus(m_hCompletionHandle,0,(ULONG_PTR)(*i).second,&((LPBIND_DATA)(*i).second)->IoData.overlapped);
					map<SOCKET,void*>::iterator  tmp = i;
					((LPBIND_DATA)tmp->second)->decRef();
					i ++;
					m_Clients.erase(tmp);
				}
				else
				{
					if ( current - ((LPBIND_DATA)(*i).second)->nBeginConnect >= 10 )
					{
						//timeout
						PostQueuedCompletionStatus(m_hCompletionHandle,-1,(ULONG_PTR)(*i).second,&((LPBIND_DATA)(*i).second)->IoData.overlapped);
						map<SOCKET,void*>::iterator  tmp = i;
						((LPBIND_DATA)tmp->second)->decRef();
						i ++;
						m_Clients.erase(tmp);
					}
					else
						i++;
				}
			}
		}
	}
}

void CWinReactorThread::addToListen(SOCKET nConnector,void* pClient)
{
	//�߳�ͬ��
	((LPBIND_DATA)pClient)->incRef();
	CAutoLock lock(m_ClientLock);
	m_Clients[nConnector] = pClient;
	SOCKET tmp = m_nListenSocket;
	m_nListenSocket = INVALID_SOCKET;
	CLOSE_SOCKET(tmp);
}

void CWinReactorThread::stop()
{
	m_bRunFlag = false;
	SOCKET tmp = m_nListenSocket;
	m_nListenSocket = INVALID_SOCKET;
	CLOSE_SOCKET(tmp);
}

void CWinReactorThread::start()
{
	m_pThread = CThreadFactory::createThread(*this);
	m_pThread->start();
}

bool CWinReactorThread::init()
{
	return m_ClientLock.initialize();
}

/*

		Class	CWinReactorImpl

*/
CWinReactorImpl::CWinReactorImpl() : m_Thread(m_hCompletionHandle),m_ThreadPool(this,&CWinReactorImpl::iocp,this)
{
	m_hCompletionHandle = INVALID_HANDLE_VALUE;
	m_bRunFlag = false;
	m_nThreadCount = 0;
}

CWinReactorImpl::~CWinReactorImpl()
{
	clear();
}

int32 CWinReactorImpl::init()
{
	this->m_hCompletionHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	if ( NULL == m_hCompletionHandle )
		return GetLastError();
	m_Thread.init();
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	//m_nThreadCount = SystemInfo.dwNumberOfProcessors - 1;
	//����ʵ�ʲ��ԣ����˶��̣߳�����������Ϊ������io���Ƶ������˶�CPU���ͬ����
	m_nThreadCount = 0;
	m_ThreadPool.init(m_nThreadCount);
	return 0;
}

void CWinReactorImpl::iocp(CWinReactorImpl* pReactor)
{
	while ( m_bRunFlag )
	{
		DWORD dwBytes = 0;
		LPBIND_DATA clientdata;
		LPPER_IO_DATA piodata;
		BOOL retval = GetQueuedCompletionStatus(m_hCompletionHandle,&dwBytes,(PULONG_PTR)&clientdata,(LPOVERLAPPED*)&(piodata),INFINITE);
		//ֹͣ�ź�
		if ( clientdata == 0 )
			break;
		switch ( clientdata->BindDataType )
		{
			case  BIND_DATA::ACCEPTOR_TYPE:
				{
					CWinReactorAcceptorImpl* pacceptor = (CWinReactorAcceptorImpl*)clientdata->pAcceptor.get();
					//�ж��Ƿ���Handler������handler����
					if ( clientdata->nMask & NETHANDLER_MASK_ACCEPT )
					{
						// accept
						if( !retval || clientdata->pAcceptor == NULL )
						{
							CNetResult Result;
							Result.setCode(-2);
							Result.setConnection(NULL);
							Result.setConnector(NULL);
							Result.setAcceptor(NULL);
							Result.setHandle(0); 
							clientdata->pHandler->onAccept(Result, clientdata->pParam);
						}
						else
						{
							CNetResult Result;
							Result.setCode(0);
							Result.setConnection(NULL);
							Result.setConnector(NULL);
							Result.setAcceptor(clientdata->pAcceptor);
							Result.setHandle(0);
							clientdata->pHandler->onAccept(Result, clientdata->pParam);
						}
					}
					//�ٴε���accept
					int32 myretval = pacceptor->IocpAccept(&clientdata->IoData.overlapped);
					if ( myretval != 0 )
					{
						//accept ʧ�ܣ�ɾ��acceptor
						clientdata->decRef();
					}
					continue;
				}
				break;
			case  BIND_DATA::CONNECTION_TYPE:
				{
					//����clientdata��IOCP�����л����INCREF�����������м�飬���������Ƿ��Ѿ�ʧЧ�ˡ�
					if ( clientdata->decRef() || clientdata->pConnection == NULL )
						continue;
					if ( !retval || ( dwBytes == 0 && retval ) )
					{
						//���ӹر�
						if ( clientdata->nMask & NETHANDLER_MASK_CLOSE )
						{
							CNetResult Result;
							Result.setCode(WSAGetLastError());
							Result.setConnection( clientdata->pConnection );
							Result.setConnector(clientdata->pRawConnector);
							Result.setAcceptor(NULL);
							Result.setHandle(0);
							clientdata->pHandler->onClose(Result,clientdata->pParam);
						}
						unregistry(clientdata->pConnection);
						continue;
					}
					switch ( piodata->opertype )
					{
						case PER_IO_DATA::OPER_RECV :
							{
								CWinReactorConnectionImpl* pconn = (CWinReactorConnectionImpl*)clientdata->pConnection.get();
								pconn->finishRecv(dwBytes);
								//���¼�
								if ( clientdata->nMask & NETHANDLER_MASK_READ )
								{
									CReadResult Result;
									Result.setCode(0);
									Result.setConnection( clientdata->pConnection );
									Result.setConnector(NULL);
									Result.setAcceptor(NULL);
									Result.setHandle(0);
									//
									Result.setData(*clientdata->RecvBuffer.get());
									clientdata->pHandler->onRead(Result,clientdata->pParam);

									clientdata->RecvBuffer->clear();
									//��ȡ��ϣ��������
								}
								if ( 0 != pconn->IocpRecv() )
								{
									if ( clientdata->nMask & NETHANDLER_MASK_CLOSE )
									{
										CNetResult Result;
										Result.setCode(WSAGetLastError());
										Result.setConnection( clientdata->pConnection );
										Result.setConnector(clientdata->pRawConnector);
										Result.setAcceptor(NULL);
										Result.setHandle(0);
										clientdata->pHandler->onClose(Result,clientdata->pParam);
									}
									unregistry(clientdata->pConnection);
								}
							}
							break;
						case PER_IO_DATA::OPER_SEND :
							{
								CWinReactorConnectionImpl* pconn = (CWinReactorConnectionImpl*)clientdata->pConnection.get();
								if ( pconn == NULL )
									continue;
								//��ȡ���ٸ���
								uint32 bytestosend = clientdata->SendBuffer->getDataSize();
								uint32 bytessent = dwBytes;
								pconn->finishSend(dwBytes);
								//���¼�
								if ( clientdata->nMask & NETHANDLER_MASK_WRITE )
								{
									CWriteResult result(bytestosend,bytessent);
									result.setCode(0);
									result.setConnection(clientdata->pConnection);
									result.setAcceptor(NULL);
									result.setConnector(NULL);
									result.setHandle(0);
									clientdata->pHandler->onWrite(result,clientdata->pParam);
								}
								//����Ƿ������ݣ������������
								pconn->IocpSend();
							}
							break;
					}
				}
				break;
			case  BIND_DATA::CONNECTOR_TYPE:
				{
					CAutoLock autolock( m_ClientDataLock );

					if ( dwBytes == 0 )
					{
						if ( clientdata->nMask & NETHANDLER_MASK_OPEN )
						{
							CNetResult Result;
							Result.setCode(0);
							Result.setConnection(clientdata->pConnection);
							Result.setConnector(clientdata->pRawConnector);
							Result.setAcceptor(NULL);
							Result.setHandle(0);
							clientdata->pHandler->onOpen(Result,clientdata->pParam);
						}
						//ע��ɹ���������clientdata
						m_ClientData.erase(m_ClientData.find(clientdata->pConnector->getHandle()));
					}
					else
					{
						//�����onOpen�¼��ٴν���ע�ᣬ�ᵼ��socket���á�������Ҫ�ڴ˾�ɾ��֮��
						m_ClientData.erase(m_ClientData.find(clientdata->pConnector->getHandle()));
						if ( clientdata->nMask & NETHANDLER_MASK_OPEN )
						{
							CNetResult Result;
							Result.setCode(-2);
							Result.setConnection(NULL);
							Result.setConnector(clientdata->pRawConnector);
							Result.setAcceptor(NULL);
							Result.setHandle(0);
							clientdata->pHandler->onOpen(Result,clientdata->pParam);
						}
					}
					//ɾ��addConnector����������
					clientdata->decRef();
				}
				break;
		}
	}
}

int32 CWinReactorImpl::run()
{
	//�������Ӽ���߳�
	m_Thread.start();
	m_bRunFlag = true;
	m_ThreadPool.start();
	iocp();
	return 0;
}

int32 CWinReactorImpl::stop()
{
	m_Thread.stop();
	m_bRunFlag = false;
	//֪ͨ�̣߳�ֹͣ������ɶ˿ڵ��¼�,+1��ԭ���ǣ���Ҫ�����̵߳�Ҳͣ����
	for ( int i = 0 ; i <= m_nThreadCount ; i ++ )
		PostQueuedCompletionStatus(m_hCompletionHandle,0,0,NULL);
	return 0;
}

int32 CWinReactorImpl::addAcceptor(IAcceptorPtr pAcceptor, INetHandlerPtr Handler, void *Param)
{
	if ( NULL == pAcceptor )
		return E_INVAL;
	BIND_DATAPTR clientdata = new BIND_DATA();
	if ( clientdata == NULL )
		return E_NOMEM;
	//����Ϊ�첽ģʽ
	pAcceptor->setMode(ASYNC_MODE);
	clientdata->incRef();
	//������ɶ˿�
	if ( NULL == ( clientdata->hCompletionHandle = CreateIoCompletionPort((HANDLE)pAcceptor->getHandle(),m_hCompletionHandle,(ULONG_PTR)clientdata.get(),0) ) )
	{
		clientdata->decRef();
		return WSAGetLastError();
	}
	//����handler
	clientdata->pHandler = Handler;
	clientdata->pParam = Param;
	CWinReactorAcceptorImpl* acceptorimpl = new CWinReactorAcceptorImpl(pAcceptor);
	clientdata->pAcceptor = acceptorimpl;
	if ( NULL != Handler )
		clientdata->nMask |= NETHANDLER_MASK_ACCEPT;
	clientdata->IoData.opertype = PER_IO_DATA::OPER_ACCEPT;
	clientdata->BindDataType = BIND_DATA::ACCEPTOR_TYPE;
	int32 retcode = acceptorimpl->IocpAccept(&clientdata->IoData.overlapped);
	//listen ʧ��
	if ( retcode != 0 )
		clientdata->decRef();
	return retcode;
}

int32 CWinReactorImpl::removeAcceptor(IAcceptorPtr pAcceptor)
{
	if ( NULL == pAcceptor )
		return E_INVAL;
	pAcceptor->close();
	return 0;
}

void CWinReactorImpl::clear()
{
	if ( m_hCompletionHandle != INVALID_HANDLE_VALUE )
		CloseHandle(m_hCompletionHandle);
	m_hCompletionHandle = INVALID_HANDLE_VALUE;
}

int32 CWinReactorImpl::addConnector(IConnectorPtr pConnector, INetHandlerPtr Handler, void *Param)
{
	if ( NULL == pConnector )
		return E_INVAL;
	BIND_DATAPTR clientdata = new BIND_DATA();
	if ( clientdata == NULL )
		return E_NOMEM;
	//����Ϊ�첽ģʽ
	pConnector->setMode(ASYNC_MODE);
	int32 retcode = pConnector->connect();
	//connect ʧ��
	if ( retcode != 0 )
		return retcode;
	//������ɶ˿�
	clientdata->incRef();
	if ( NULL == ( clientdata->hCompletionHandle = CreateIoCompletionPort((HANDLE)pConnector->getHandle(),m_hCompletionHandle,(ULONG_PTR)clientdata.get(),0) ) )
	{
		clientdata->decRef();
		return WSAGetLastError();
	}

	CAutoLock autolock( m_ClientDataLock );

	//���뻺��������registryʹ��ʱ������
	m_ClientData[pConnector->getHandle()] = clientdata.get();
	//����handler
	clientdata->pHandler = Handler;
	clientdata->pParam = Param;
	if ( NULL != Handler )
		clientdata->nMask |= NETHANDLER_MASK_OPEN;
	clientdata->BindDataType = BIND_DATA::CONNECTOR_TYPE;
	clientdata->nBeginConnect = CDateTime().getTotalSecs();
	clientdata->pRawConnector = pConnector;
	clientdata->pConnector = new CWinReactorConnectorImpl(pConnector);
	//�������Ӵ����̣߳��ж��Ƿ����ӳɹ�
	m_Thread.addToListen(pConnector->getHandle(),clientdata.get());
	return 0;
}

int32 CWinReactorImpl::removeConnector(IConnectorPtr pConnector)
{
	if ( NULL == pConnector )
		return E_INVAL;
	pConnector->close();
	return 0;
}

int CWinReactorImpl::registry(IConnectionPtr& pConnection,INetHandlerPtr Handler,uint32 nMask,void* Param)
{
	CAutoLock autolock( m_ClientDataLock );

	if ( NULL == pConnection )
		return E_INVAL;
	BIND_DATAPTR clientdata = NULL;
	map<SOCKET,void*>::iterator i = m_ClientData.find(pConnection->getHandle());
	if ( i == m_ClientData.end() )
	{
		clientdata = new BIND_DATA();
		if ( clientdata == NULL )
			return E_NOMEM;
		clientdata->incRef();
		//������ɶ˿�
		if ( NULL == ( clientdata->hCompletionHandle = CreateIoCompletionPort((HANDLE)pConnection->getHandle(),m_hCompletionHandle,(ULONG_PTR)clientdata.get(),0) ) )
		{
			clientdata->decRef();
			return WSAGetLastError();
		}
	}
	else
	{
		//����connector��data
		clientdata = (BIND_DATA*)(*i).second;
		clientdata->incRef();
	}
	//����Ϊ�첽ģʽ
	pConnection->setMode(ASYNC_MODE);
	//����handler
	clientdata->pHandler = Handler;
	clientdata->pParam = Param;
	if ( NULL != Handler )
		clientdata->nMask = nMask;
	CWinReactorConnectionImpl* pconn = new CWinReactorConnectionImpl(pConnection,clientdata.get());
	clientdata->pConnection = pconn;
	clientdata->BindDataType = BIND_DATA::CONNECTION_TYPE;
	pConnection = pconn;
	//����read�¼�
	return pconn->IocpRecv();
}

int CWinReactorImpl::unregistry(IConnectionPtr& pConnection)
{
	//must be CWinReactorConnectionImpl
	CWinReactorConnectionImpl* pconn = static_cast<CWinReactorConnectionImpl*>(pConnection.get());
	BIND_DATAPTR clientdata = static_cast<BIND_DATA*>(pconn->getKey());
	pConnection->close();
	//ɾ������
	clientdata->decRef();
	//���˫������
	clientdata->pConnection = NULL;
	return 0;
}


/*

		Class	CWinReactorDriver

*/
CWinReactorDriver::CWinReactorDriver()
{
}

CWinReactorDriver::~CWinReactorDriver()
{
}

const char* CWinReactorDriver::getClassName()
{
	return "CWinReactorDriver";
}

INetActorPtr CWinReactorDriver::getNetActorInstance()
{
	return CWinReactorImpl::getInstance();
}


#endif
