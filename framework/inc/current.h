#ifndef _CURRENT_H_
#define _CURRENT_H_

#include <framework/endpoint.h>
#include <framework/session.h>
#include <framework/response.h>

class CDAServer;
typedef CRefObject<CDAServer> CDAServerPtr;

class CCurrent : public CRefShare
{
public:
	/**
	 * ���캯��
	 */
	CCurrent();
	/**
	 * ��������
	 */
	virtual ~CCurrent();
	/**
	 * ��������ID
	 */
	void setRequestID(uint32 nRequestID);
	/**
	 * ��ȡ��ǰ�����id
	 */
	uint32 getRequestID();
	/**
	 *
	 */
	void setServantName(uint32 nServantName);
	/**
	 * ��ȡservant��
	 */
	uint32 getServantName();
	/**
	 * ��������˵��ʵ��
	 */
	void setEndPoint(IEndPoint* pEndPoint);
	/**
	 * �������Ӧ������˵�
	 */
	IEndPointPtr getEndPoint();
	/**
	 * ���ûỰ
	 */
	void setSession(CSession* pSession);
	/**
	 * �������Ӧ�ĻỰ��ʶ
	 */
	CSessionPtr getSession();
	/**
	 * ����Ӧ���
	 */
	void sendResponse(IResponsePtr pResponse);
	/**
	 * ֱ�ӷ������л���
	 */
	void sendSerial(uint32 nSerialNo,uint32 nServantName,ISerial& pSerial);
	/**
	 * �رյ�ǰ�Ự���Ͽ����ӣ�
	 * �ڵ�ǰҵ����Ϻ�ر�
	 */
	void closeSession();
	/**
	 * �Ƿ���Ҫ�رջỰ
	 */
	bool isCloseSession();
	/**
	 * ���÷�����
	 */
	void setDAServer(CDAServer* pServer);
	/**
	 * ��������
	 */
	void reset();
	/**
	 * ���ñ��ص���
	 */
	void setLocalCall();

private:
	bool m_bIsCallLocal;
	IEndPointPtr m_pEndPoint;
	CSessionPtr m_pSession;
	uint32 m_nRequestID;
	CDAServerPtr m_pDAServer;
	uint32 m_nServantName;
	bool m_bCloseSession;
};

#include <util/objectpool.h>
typedef CPoolObjectSharePtr<CCurrent> CCurrentPtr;

#endif

