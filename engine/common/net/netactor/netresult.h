#ifndef _NETRESULT_H_
#define _NETRESULT_H_

#include "../../public.h"
#include "../../io/bytesbuffer.h"
#include "../connection.h"
#include "../connector.h"
#include "../acceptor.h"
#include "../../util/refshare.h"
#include <string>

class CNetResult
{
public:
	/**
	 * ���캯��
	 */
	CNetResult();
	/**
	 * ��������
	 */
	virtual ~CNetResult();
	/**
	 * ��ȡ�����
	 */
	int32 getCode() const;
	/**
	 * ��ȡ�������
	 */
	const char* getDesc();
	/**
	 * ��ȡ��ǰ������
	 */
	IConnectionPtr getConnection() const;
	/**
	 * ��ȡ��ǰ��Զ������
	 */
	IConnectorPtr getConnector() const;
	/**
	 * ��ȡ��ǰ��acceptor
	 */
	IAcceptorPtr getAcceptor() const;
	/**
	 * ��ȡsocket���
	 */
	SOCKET getHandle() const;

	/**
	 * ���ý����
	 */
	void setCode(int32);
	/**
	 * ������������
	 */
	void setConnection(IConnectionPtr p);
	/**
	 * ����Զ������
	 */
	void setConnector(IConnectorPtr p);
	/**
	 * ����acceptor
	 */
	void setAcceptor(IAcceptorPtr p);
	/**
	 * ����socket���
	 */
	void setHandle(SOCKET Sock);

protected :
	int32       m_nCode;
	std::string  m_strDesc;
	IConnectionPtr m_pConnection;
	IConnectorPtr  m_pConnector;
	IAcceptorPtr   m_pAcceptor;
	SOCKET       m_Socket;
};

#endif

