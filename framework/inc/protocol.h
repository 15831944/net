#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <public.h>
#include <io/outputstream.h>
#include <io/bytesbuffer.h>
#include <util/refshare.h>

enum PROTOCOL_RETCODE
{
	PRC_ERROR = -1,
	PRC_SUCCESS = 0,
	PRC_NOMOREPACK
};

class IProtocol : public IRefShare
{
public :
	virtual ~IProtocol() {}
	/**
	 * ���Э��
	 * @return PROTOCOL_RETCODE PRC_SUCCESS�����ɹ���PRC_ERROR����ʧ��
	 */
	virtual PROTOCOL_RETCODE pack(uint32 nSerialNo,uint32 nServantName,const CBytesBuffer& Body,IOutputStream& Output) = 0;
	//���������ݿ������������
	/**
	 * �Ƿ�����һ����
	 * @param Input IInputStream ���������
	 * @param Body Э���������
	 * @return PROTOCOL_RETCODE PRC_SUCCESS-����һ������PRC_ERROR-����������PRC_NOPACK--û�������İ�
	 */
	virtual PROTOCOL_RETCODE next(IInputStream& Input,uint32& nSerialNo,uint32& nServantName,IOutputStream& Body) = 0;
	/**
	 * ��ȡ����Э���Ӧ������
	 * @param nServantName ����Э���������
	 * @param uint32 Ӧ��Э���������
	 */
	virtual uint32 getResponseName(uint32 nServantName) = 0;
	/**
	 * �Ƿ�Ӧ��Э��
	 */
	virtual bool isResponse(uint32 nServantName) = 0;
	/**
	 * �Ƿ�����Э�飬����ǣ�ֱ�����������账��
	 */
	virtual bool isHeartBeat(uint32 nServantName) = 0;
	/**
	 * ����������Ϣ
	 */
	virtual PROTOCOL_RETCODE packHeartBeat(IOutputStream& Output) = 0;
	/**
	 * ����Ӧ�����������������������
	 * @param nServantName ��������
	 * @return uint32 �����
	 */
	virtual uint32 getServantName(uint32 nServantName) = 0;
	//-----�����ϣ�Ӧ�÷��뵥����ʵ����������-----
	/**
	 * ����ʵ��
	 */
	virtual IProtocol* newInstance() = 0;
};

typedef CRefObject<IProtocol> IProtocolPtr;

class CProtocolFactory
{
private :
	//�洢���еĿռ�
	static map<string,IProtocolPtr> g_Protocols;
	CProtocolFactory();

public :
	/**
	 * ����Э������������Ӧ��Э��
	 */
	static IProtocolPtr createProtocol(const char* pProtocolName);
	/**
	 * ע��Э��
	 */
	static void regProtocol(const char* pProtocolName,IProtocolPtr pProtocol);
};

#endif
