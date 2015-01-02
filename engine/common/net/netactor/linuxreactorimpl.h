#ifndef _LINUXREACTORIMPL_H_
#define _LINUXREACTORIMPL_H_

#include <net/netactor/reactor.h>
#include <net/netpublic.h>
#include <map>
#include <util/singleton.h>
#include <io/bytesbuffer.h>
#include <util/baselock.h>
#include <util/refshare.h>
#include <util/lightlock.h>
#include <util/thread.h>
#include <util/activeobject.h>

#define EVNET_COUNT 1024

class CEpollBindData : public CRefShare
{
public:
    CEpollBindData()
    {
		SendBuffer = CBytesBufferPtr::createInstance(__FUNCTION__);
		reset();
    }

    virtual ~CEpollBindData()
    {
		reset();
    }

    enum
    {
        CONNECTOR_TYPE,
        ACCEPTOR_TYPE,
        CONNECTION_TYPE
    } BindDataType;

	void reset()
	{
		pRawConnector = NULL;
		pAcceptor   = NULL;
		pConnection = NULL;
		pHandler = NULL;
		pParam = NULL;
		nEpollevents = 0;
		nNetHandleMask = 0;
		SendBuffer->reset();
	}

	//���Ӿ����Ҳ��Ψһ��ʶ
	SOCKET Sock;
	//
	IConnectorPtr pRawConnector;
	//acceptor
	IAcceptorPtr   pAcceptor;
	//ʵ�ʵ���������
	IConnectionPtr pConnection;
	//���͵�������
	CLightLock m_SendBufLock;
	//�����¼�������
	INetHandlerPtr pHandler;
	//�����¼������mask
	uint32 nNetHandleMask;
	//Я���Ĳ���
	void* pParam;
	//���͵�buffer
	CBytesBufferPtr SendBuffer;
	//epoll Ҫ������¼�
	uint32 nEpollevents;
};

typedef CRefObject<CEpollBindData> CEpollBindDataPtr;


class CLinuxReactorImpl : public IReactor,
                          public CSingleton<CLinuxReactorImpl>
{
protected :
	/**
	 * �����ķ����߳�
	 */
	void sendRun(CLinuxReactorImpl* pPointer);

public:
    CLinuxReactorImpl();
    virtual ~CLinuxReactorImpl();

    /**
     *
     */
    virtual int32 init();

    /**
     *
     */
    virtual int32 run();    

    /** 
     *
     */
    virtual int32 stop();

    /**
     *
     */
    virtual int32 addAcceptor(IAcceptorPtr Acceptor, INetHandlerPtr Handler, void* Param);
	/**
	 *
	 */
	virtual int32 removeAcceptor(IAcceptorPtr Acceptor);

    /**
     *
     */
    virtual int32 addConnector(IConnectorPtr Connector, INetHandlerPtr Handler, void* Param);
	/**
	 *
	 */
	virtual int32 removeConnector(IConnectorPtr Connector);

    /**
     *
     */
    virtual int32 registry(IConnectionPtr& pConnection, INetHandlerPtr Handler, uint32 nMask,void* Param);

	/**
	 *
	 */
	virtual int32 unregistry(IConnectionPtr& pConnection);
	/**
	 * ����epoll���첽д
	 */
    int32 asynWrite(IConnectionPtr& pConnection, CEpollBindData* pData, const char* pszBuf, uint32 nBufLen);
    
    //int32 AsynRead(IConnectionPtr& pConnection, char* pszBuf, uint32 nBufLen); 
protected:
	CLightLock m_BindDataLock;

	//����������ɶ˿�
	int m_EpollReadFd;
	//д��������ɶ˿�
	int m_EpollWriteFd;
	struct epoll_event m_events[EVNET_COUNT];
    std::map<SOCKET, CEpollBindData*> m_mapBindData;
	bool m_isLoop;
	typedef TActiveObject<CLinuxReactorImpl> THREAD_TYPE;
	//�����߳�
	THREAD_TYPE* m_pThread;
};

#endif

