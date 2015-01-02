#ifndef _REACTORSOCKETIMPL_H_
#define _REACTORSOCKETIMPL_H_

#include "../../public.h"
#include "../netpublic.h"
#include "../acceptor.h"
#include "../connector.h"
#include <mswsock.h>
#include "../connection.h"
#include "nethandler.h"
#include "../../io/bytesbuffer.h"
#include "../../lock/lightlock.h"

//��IO��������
typedef struct _t_per_io_data
{
	//��ɶ˿ڽṹ
	OVERLAPPED overlapped;
	//һ�����ݻ�����,����WSASend/WSARecv�еĵڶ�������
	WSABUF buffer;
	//��������,����ΪSEND/RECV����
	enum OPER_TYPE
	{
		OPER_ACCEPT,
		OPER_SEND,
		OPER_RECV
	}opertype;
	DWORD dwFlags;
}PER_IO_DATA,*LPPER_IO_DATA;


typedef struct _t_binddata : public CRefShare
{
	enum
	{
		CONNECTOR_TYPE,
		ACCEPTOR_TYPE,
		CONNECTION_TYPE
	} BindDataType;
	//ʵ�ʵ�������
	IConnectorPtr pRawConnector;
	//������
	IConnectorPtr  pConnector;
	//Acceptor
	IAcceptorPtr   pAcceptor;
	//��������
	IConnectionPtr pConnection;
	//����ⲿ�Ĳ���
	void* pParam;
	//Net Handle��mask
	uint32 nMask;
	//�������ʵ��
	INetHandlerPtr pHandler;
	int32 nBeginConnect;
	//��ɶ˿ھ��
	HANDLE hCompletionHandle;
	//Io����
	PER_IO_DATA IoData;
	//���ջ���
	CBytesBufferPtr RecvBuffer;
	//���ͻ���
	CBytesBufferPtr SendBuffer;

	_t_binddata() : pConnector(NULL),pAcceptor(NULL),pConnection(NULL),pHandler(NULL)
	{
		nMask = 0;
		pParam = NULL;
		hCompletionHandle = INVALID_HANDLE_VALUE;
	}

	~_t_binddata()
	{

	}
}BIND_DATA,*LPBIND_DATA;

typedef CRefObject<BIND_DATA> BIND_DATAPTR;

class CWinReactorAcceptorImpl : public IAcceptor
{
protected :
	IAcceptorPtr m_pAcceptor;
	//Accept�����Ŀͻ���socket
	SOCKET m_nClientSocket;
	//��������ɶ˿ڵ�Accept����
	LPFN_ACCEPTEX m_pFunAcceptEx;
	//��ȡ��ַ�Ļ�����
	char m_AcceptBuff[100];
public :
	CWinReactorAcceptorImpl(IAcceptorPtr pAcceptor);
	~CWinReactorAcceptorImpl();
	SOCKET getHandle();
	int32 setMode(ECOMMMODE Mode);
	ECOMMMODE getMode();
	int32 listen(int BackLog);
	IConnectionPtr accept();
	void close();
	/*
	 * ����AcceptEx�����client socket
	 */
	SOCKET createClientSocket();
	/**
	 *
	 */
	int32 IocpAccept(LPOVERLAPPED pOverLapped);
	const IAddressPtr& getListenAddress() const;
};

class CWinReactorConnectionImpl : public IConnection,
								  public IOutputStream,
								  public CRefShare
{
protected :
	//������
	CLightLock m_SendLock;
	//���ջ���
	CBytesBufferPtr m_RecvBuffer;
	PER_IO_DATA m_RecvIoData;
	//���ͻ���
	CBytesBufferPtr m_SendBuffer;
	PER_IO_DATA m_SendIoData;
	//�ײ������
	IConnectionPtr m_pConnection;
	//��ȡ��KEY
	BIND_DATAPTR m_Key;
	//�Ƿ��ڷ����еı�ʶ
	bool m_bIsSending;
public :
	CWinReactorConnectionImpl(IConnectionPtr pConnection,BIND_DATAPTR Key);
	~CWinReactorConnectionImpl();
	int32 setMode(ECOMMMODE Mode);
	IInputStreamPtr  getInputStream();
	IOutputStreamPtr getOutputStream();
	void* getKey() { return m_Key; }
	/**
	 * д������
	 */
	int writeBytes( const char* pBuf, uint32& Buflen);
	/**
	 * ��ɷ���
	 */
	void finishSend(uint32 SendSize);
	/**
	 * ��ɶ�ȡ
	 */
	void finishRecv(uint32 SendSize);
	void close();
	/**
	 * ������ɶ˿ڵ�д�¼�����֤��˳��������
	 */
	int32 IocpSend();
	/**
	 * ������ɶ˿ڵĶ��¼�
	 */
	int32 IocpRecv();
	SOCKET getHandle() const;
	IAddressPtr getHostAddress();
	IAddressPtr getRemoteAddress();
	/**
	 * ��ȡ���ն��еĻ���
	 */
	CBytesBufferPtr& getRecvBuffer();
    //void setParam(void* pParam);
    //void* getParam();
	/**
	 * ����Socket�������Ĵ�С
	 */
	int32 setBufSize(int32 nBufSize);
	void incRef();
	bool decRef();
};

class CWinReactorConnectorImpl : public IConnector,
								 public CRefShare
{
protected :
	IConnectorPtr m_pConnector;
public :
	CWinReactorConnectorImpl(IConnectorPtr pConnector);
	~CWinReactorConnectorImpl();
	SOCKET getHandle();
	int32 setMode(ECOMMMODE Mode);
	IConnectionPtr getConnection();
	int32 connect();
	int32 reconnect();
	void close();
	void incRef();
	bool decRef();
	const IAddressPtr& getConnectAddress() const;
};


#endif
