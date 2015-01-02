#ifndef _NETACTOR_H_
#define _NETACTOR_H_

#include "../acceptor.h"
#include "../connector.h"
#include "nethandler.h"
#include "../util/refshare.h"

class INetActor : public CRefShare
{
public:
    INetActor()
    {

    }

    virtual ~INetActor() 
    {
    
    }

    /**
     *
     */
    virtual int32 init() = 0;
       
    /**
     *
     */
    virtual int32 run() = 0;

    /**
     *
     */
    virtual int32 stop() = 0;

    /**
     *
     */
    virtual int32 addAcceptor(IAcceptorPtr Acceptor, INetHandlerPtr Handler, void* Param) = 0;

    /**
     *
     */
    virtual int32 addConnector(IConnectorPtr Connector, INetHandlerPtr Handler, void* Param) = 0;
	/**
	 * ɾ��������
	 */
	virtual int32 removeConnector(IConnectorPtr Connector) = 0;
	/**
	 * ɾ��������
	 */
	virtual int32 removeAcceptor(IAcceptorPtr Acceptor) = 0;
    /**
     * ע��connection
	 * @param pConnection Ҫע������ӣ���������������ᱻ�޸�Ϊע���ĺϷ�����
	 * @return int32 �Ƿ�ɹ���0�����ɹ���������ϵͳ������
     */
    virtual int32 registry(IConnectionPtr& pConnection, INetHandlerPtr Handler, uint32 nMask, void* Param) = 0;
    /**
     *
     */
    virtual int32 unregistry(IConnectionPtr& pConnection) = 0;
};

typedef CRefObject<INetActor> INetActorPtr;


#endif
