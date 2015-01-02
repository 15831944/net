#ifndef _ADDRESS_H_
#define _ADDRESS_H_

#include "netpublic.h"
#include "../util/refshare.h"

class IAddress
{
public:
    IAddress() {}
    virtual ~IAddress() {}
	/**
	 * ��ȡIP
	 */
    virtual const char* getIP() const = 0;
	/**
	 * ��ȡ�˿�
	 */
    virtual uint16 getPort() const = 0;
	/**
	 * ��ȡsockaddr�Ľṹ
	 */
    virtual const struct sockaddr_in* getsockaddr() const = 0;
	/**
	 * ��ȡһ������IP��PORT���ַ�������������Ψһ�ԣ�
	 * @return const char* �ַ�������
	 */
	virtual const char* getString() const = 0;
};

typedef TRefPointer<IAddress> IAddressPtr;

/**
 * ��ȡsocket�ı�����ַ
 */
IAddressPtr getSockAddr(SOCKET Sock);
/**
 * ��ȡsocket�ĶԶ˵�ַ
 */
IAddressPtr getPeerSockAddr(SOCKET Sock);

#endif

