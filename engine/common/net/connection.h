#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include  "netpublic.h"
#include  "../io/inputstream.h"
#include  "../io/outputstream.h"
#include  "address.h"
#include  "../util/refshare.h"

/*
IConnection���ܴ��ڶ�̳У�����ֻ�ܼ̳�IRefShare����ӿ�
*/
class IConnection : public IRefShare
{
public:
	IConnection() 
	{
		m_Socket = INVALID_SOCKET; 
		m_pHostAddr   = NULL;
		m_pRemoteAddr = NULL;
	}

	virtual ~IConnection() { }
	
	/**
	 * ����ͬ�����첽ģʽ
	 */
	virtual int32 setMode(ECOMMMODE Mode) = 0;
	/**
	 * ��ȡ������
	 */
	virtual IInputStreamPtr  getInputStream() = 0;  
	/**
	 * ��ȡ�����
	 */
	virtual IOutputStreamPtr getOutputStream() = 0;
	/**
	 * �ر�����
	 */
	virtual void close() = 0;
	/**
	 * ��ȡsocket���
	 */
	virtual SOCKET getHandle() const { return m_Socket; } 
	/**
	 * ��ȡ�����ĵ�ַ
	 */
	virtual IAddressPtr getHostAddress() { return m_pHostAddr; }
	/**
	 * ��ȡԶ�˵ĵ�ַ
	 */
	virtual IAddressPtr getRemoteAddress() { return m_pRemoteAddr; }
	/**
	 * ����Socket�������Ĵ�С
	 */
	virtual int32 setBufSize(int32 nBufSize) = 0;

protected :
	SOCKET      m_Socket;
	IAddressPtr m_pHostAddr;
	IAddressPtr m_pRemoteAddr;
};

typedef CRefObject<IConnection> IConnectionPtr;

#endif
