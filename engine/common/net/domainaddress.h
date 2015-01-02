#ifndef _DOMAINADDRESS_H_
#define _DOMAINADDRESS_H_

#include "netpublic.h"
#include "address.h"

/*
������ַ
*/
class CDomainAddress : public IAddress
{
public:
	CDomainAddress();
	virtual ~CDomainAddress();
	/**
	 * ��ʼ��������ת��ΪIP
	 * @param Domain �������������199���ֽ�
	 * @param nPort �˿�
	 * @return int32 ת�������0����ɹ�������ֵ���������
	 */
	int32 init(const char* Domain,uint16 nPort);

	virtual const char* getIP() const;
	virtual uint16 getPort() const;
	virtual const struct sockaddr_in* getsockaddr() const;
	/**
	 * ��ȡһ������IP��PORT���ַ�������������Ψһ�ԣ�
	 * @return const char* �ַ�������
	 */
	const char* getString() const;

private:
	struct sockaddr_in m_addr;
	char m_Address[100];
	char m_IP[20];
	//���������199���ֽ�
	char m_Domain[200];
};


#endif
