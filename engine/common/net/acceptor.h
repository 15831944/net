#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "netpublic.h"
#include "connection.h"
#include "address.h"
#include  "../util/refshare.h"

class IAcceptor : public CRefShare
{
public:
    IAcceptor() {}
    virtual ~IAcceptor() {}

    virtual SOCKET getHandle() = 0;
    
    virtual int32 setMode(ECOMMMODE Mode) = 0;

	virtual ECOMMMODE getMode() = 0;
    
    virtual int32 listen(int BackLog = DEFAULT_BACKLOG) = 0;

    virtual IConnectionPtr accept() = 0;
    
    virtual void close() = 0;
	/**
	 * ��ȡ�����ĵ�ַ
	 * @return IAddressPtr �����ĵ�ַ
	 */
	virtual const IAddressPtr& getListenAddress() const = 0;
};

typedef CRefObject<IAcceptor> IAcceptorPtr;

#endif

