#include <public.h>

#ifdef WINDOWS

#include "winreactorsocketimpl.h"
#include <mswsock.h>
#include "../../util/drivermanager.h"
#include "../socketdriver.h"
#include "../ip4address.h"
//#include "../../io/binlog.h"

/*

		Class	CWinReactorAcceptorImpl

*/
CWinReactorAcceptorImpl::CWinReactorAcceptorImpl(IAcceptorPtr pAcceptor) : m_pAcceptor(pAcceptor)
{
	m_nClientSocket = INVALID_SOCKET;
	m_pFunAcceptEx = NULL;
	//��������Ϊ�첽ģʽ
	//pAcceptor->setMode(ASYNC_MODE);
}

CWinReactorAcceptorImpl::~CWinReactorAcceptorImpl()
{
}

SOCKET CWinReactorAcceptorImpl::getHandle()
{
	return m_pAcceptor->getHandle();
}

int32 CWinReactorAcceptorImpl::listen(int BackLog)
{
	//m_pAcceptor->setMode(ASYNC_MODE);
	return m_pAcceptor->listen(BackLog);
}

int32 CWinReactorAcceptorImpl::setMode(ECOMMMODE Mode)
{
	//�������ٽ���������
	return -1;
}

ECOMMMODE CWinReactorAcceptorImpl::getMode()
{
	return m_pAcceptor->getMode();
}

void CWinReactorAcceptorImpl::close()
{
	m_pAcceptor->close();
}

IConnectionPtr CWinReactorAcceptorImpl::accept()
{
	SOCKET nclient = m_nClientSocket;
	//�ȴ���һ�ε�accept
	m_nClientSocket = INVALID_SOCKET;
	if ( nclient == INVALID_SOCKET )
		return NULL;
	SOCKET nlisten = getHandle();
	setsockopt(nclient,SOL_SOCKET,SO_UPDATE_ACCEPT_CONTEXT,(char*)&nlisten,sizeof(nlisten));
	//todo ���չٷ��ķ�����ȡ������Զ�̵�ַ
	struct sockaddr_in *localaddr,*remoteaddr;
	localaddr = (struct sockaddr_in*)(m_AcceptBuff+10);
	remoteaddr = (struct sockaddr_in*)(m_AcceptBuff+sizeof(SOCKADDR)+16+6);
//	pConnection->setHostAddress(new CIP4Address(inet_ntoa(localaddr->sin_addr), ntohs(localaddr->sin_port)));
//	pConnection->setRemoteAddress(new CIP4Address(inet_ntoa(remoteaddr->sin_addr), ntohs(remoteaddr->sin_port)));

	//ͨ��SocketDriver��ȡԭʼ��connection
	IConnectionPtr pConnection(static_cast<ISocketDriver*>(CDriverManager::getDriver("SocketDriver"))->createConnection(nclient,(IAddressPtr)this->m_pAcceptor->getListenAddress(),getPeerSockAddr(nclient)));
	if ( pConnection == NULL )
	{
		CLOSE_SOCKET(nclient);
		return NULL;
	}

	if ( 0 != pConnection->setMode(ASYNC_MODE) )
	{
		pConnection->close();
		return NULL;
	}

	if ( 0 != pConnection->setBufSize(64*1024) )
	{
		pConnection->close();
		return NULL;
	}
	return pConnection;
}

SOCKET CWinReactorAcceptorImpl::createClientSocket()
{
	if ( m_nClientSocket != INVALID_SOCKET )
	{
		SOCKET nlisten = getHandle();
		setsockopt(m_nClientSocket,SOL_SOCKET,SO_UPDATE_ACCEPT_CONTEXT,(char*)&nlisten,sizeof(nlisten));
		CLOSE_SOCKET(m_nClientSocket);
		m_nClientSocket = INVALID_SOCKET;
	}
	m_nClientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	return m_nClientSocket;
}

int32 CWinReactorAcceptorImpl::IocpAccept(LPOVERLAPPED pOverLapped)
{
	if ( m_pFunAcceptEx == NULL )
	{
		//Accept function GUID
		GUID guidAcceptEx = WSAID_ACCEPTEX;
		//get acceptex function pointer
		DWORD dwBytes = 0;
		if ( WSAIoctl(getHandle(),SIO_GET_EXTENSION_FUNCTION_POINTER,
		 &guidAcceptEx,sizeof(guidAcceptEx),&m_pFunAcceptEx,sizeof(m_pFunAcceptEx),
		 &dwBytes,NULL,NULL) != 0 )
		{
			return WSAGetLastError();
		}
	}
	DWORD dwBytes = 0;
	//����acceptex
	memset(pOverLapped,0,sizeof(OVERLAPPED));
	createClientSocket();
	if ( ! m_pFunAcceptEx(getHandle(),m_nClientSocket,m_AcceptBuff,0,sizeof(SOCKADDR_IN)+16,sizeof(SOCKADDR_IN)+16,&dwBytes,pOverLapped) )
	{
		DWORD errcode = WSAGetLastError();
		if ( errcode != ERROR_IO_PENDING )
			return errcode;
	}
	return 0;
}

const IAddressPtr& CWinReactorAcceptorImpl::getListenAddress() const
{
	return m_pAcceptor->getListenAddress();
}

/*

		Class	CWinReactorConnectionImpl

*/
CWinReactorConnectionImpl::CWinReactorConnectionImpl(IConnectionPtr pConnection,BIND_DATAPTR Key) : m_pConnection(pConnection),m_Key(Key)
{
	m_RecvBuffer = m_Key->RecvBuffer = CBytesBufferPtr::createInstance();
	m_SendBuffer = CBytesBufferPtr::createInstance();
	m_Key->SendBuffer = CBytesBufferPtr::createInstance();
	m_RecvIoData.opertype = PER_IO_DATA::OPER_RECV;
	m_SendIoData.opertype = PER_IO_DATA::OPER_SEND;
	m_bIsSending = false;
}

CWinReactorConnectionImpl::~CWinReactorConnectionImpl()
{
}

IInputStreamPtr CWinReactorConnectionImpl::getInputStream()
{
	return this->m_RecvBuffer.get();
}

IOutputStreamPtr CWinReactorConnectionImpl::getOutputStream()
{
	return this;
}

int32 CWinReactorConnectionImpl::writeBytes(const char* pBuf,uint32 &Buflen)
{
	//ֻд�뻺����
	CAutoLock lock(m_SendLock);
	m_SendBuffer->writeBytes(pBuf,Buflen);
	return IocpSend();
}

int32 CWinReactorConnectionImpl::IocpRecv()
{
	DWORD dwBytes = 0;
	memset(&this->m_RecvIoData.overlapped,0,sizeof(OVERLAPPED));
	//��Զʹ�ý��ջ�����
	this->m_RecvIoData.buffer.buf = (char*)m_RecvBuffer->c_str();
	//��һ���ֽ���������0ʹ��
	this->m_RecvIoData.buffer.len = m_RecvBuffer->getBufferSize() - 1;
	this->m_RecvIoData.dwFlags = 0;
	m_Key->incRef();
	if ( 0 != WSARecv(m_pConnection->getHandle(),&m_RecvIoData.buffer,1,&dwBytes,&m_RecvIoData.dwFlags,&m_RecvIoData.overlapped,NULL) )
	{
		DWORD errcode = WSAGetLastError();
		if ( errcode != WSA_IO_PENDING )
		{
			m_Key->decRef();
			return errcode;
		}
	}
	return 0;
}

void CWinReactorConnectionImpl::finishRecv(uint32 ReadSize)
{
	m_RecvBuffer->setDataSize(ReadSize);
}

int32 CWinReactorConnectionImpl::IocpSend()
{
	{
		CAutoLock lock(m_SendLock);
		if ( m_bIsSending )
			return 0;
		if ( m_Key->SendBuffer->getDataSize() == 0 )
		{
			if ( m_SendBuffer->getDataSize() == 0 )
				return 0;
			//�ѷ��ͻ�������ݶ�ȡ��KEY�Ļ�������
			m_SendBuffer->readBytes(*m_Key->SendBuffer.get());
		}
		m_bIsSending = true;
	}
	DWORD dwBytes = 0;
	memset(&this->m_SendIoData.overlapped,0,sizeof(OVERLAPPED));
	this->m_SendIoData.buffer.buf = (char*)m_Key->SendBuffer->c_str();
	//�ɿ��Ƿְ�����
	this->m_SendIoData.buffer.len = m_Key->SendBuffer->getDataSize();
	this->m_SendIoData.dwFlags = 0;
	m_Key->incRef();
	if ( 0 != WSASend(m_pConnection->getHandle(),&m_SendIoData.buffer,1,&dwBytes,m_SendIoData.dwFlags,&m_SendIoData.overlapped,NULL) )
	{
		DWORD errcode = WSAGetLastError();
		if ( errcode != WSA_IO_PENDING )
		{
			m_Key->decRef();
			return errcode;
		}
	}
	return 0;
}

void CWinReactorConnectionImpl::finishSend(uint32 SendSize)
{
	//�ɹ����Ͷ��ٸ��ֽ�
	CAutoLock lock(m_SendLock);
	m_bIsSending = false;
	m_Key->SendBuffer->popBytes(SendSize);
	m_Key->SendBuffer->compact();
}

SOCKET CWinReactorConnectionImpl::getHandle() const
{
	return m_pConnection->getHandle();
}

IAddressPtr CWinReactorConnectionImpl::getHostAddress()
{
	return m_pConnection->getHostAddress();
}

CBytesBufferPtr& CWinReactorConnectionImpl::getRecvBuffer()
{
	return this->m_SendBuffer;
}

IAddressPtr CWinReactorConnectionImpl::getRemoteAddress()
{
	return m_pConnection->getRemoteAddress();
}

void CWinReactorConnectionImpl::close()
{
	m_pConnection->close();
}

int32 CWinReactorConnectionImpl::setMode(ECOMMMODE Mode)
{
	//�������ٽ�������
	return -1;
	//return m_pConnection->setMode(Mode);
}

int32 CWinReactorConnectionImpl::setBufSize(int32 nBufSize)
{
	return m_pConnection->setBufSize(nBufSize);
}

void CWinReactorConnectionImpl::incRef()
{
	CRefShare::incRef();
}

bool CWinReactorConnectionImpl::decRef()
{
	return CRefShare::decRef();
}


/*

		Class	CWinReactorConnectorImpl

*/
CWinReactorConnectorImpl::CWinReactorConnectorImpl(IConnectorPtr pConnector) : m_pConnector(pConnector)
{
	//pConnector->setMode(ASYNC_MODE);
}

CWinReactorConnectorImpl::~CWinReactorConnectorImpl()
{
}

SOCKET CWinReactorConnectorImpl::getHandle()
{
	return m_pConnector->getHandle();
}

int32 CWinReactorConnectorImpl::setMode(ECOMMMODE Mode)
{
	//�������ٽ�������
	return -1;
	//return m_pConnector->setMode(Mode);
}

IConnectionPtr CWinReactorConnectorImpl::getConnection()
{
	return m_pConnector->getConnection();
}

int32 CWinReactorConnectorImpl::connect()
{
	//
	return reconnect();
}

void CWinReactorConnectorImpl::close()
{
	m_pConnector->close();
}

int32 CWinReactorConnectorImpl::reconnect()
{
	m_pConnector->reconnect();
	return 0;
}

void CWinReactorConnectorImpl::incRef()
{
	CRefShare::incRef();
}

bool CWinReactorConnectorImpl::decRef()
{
	return CRefShare::decRef();
}

const IAddressPtr& CWinReactorConnectorImpl::getConnectAddress() const
{
	return this->m_pConnector->getConnectAddress();
}


#endif
