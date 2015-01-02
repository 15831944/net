#ifndef _ZWPROTOCOL_H_
#define _ZWPROTOCOL_H_

#include <framework/protocol.h>

class CZWProtocol : public IProtocol,
					public CRefShare
{
protected :
	/**
	 * ��ȡ��ֵ
	 * @param pBuffer ����ָ��
	 * @param nDataSize Ҫ��ȡ�����ݴ�С
	 */
	int readInt(const char* pBuffer,int nReadSize);
	/**
	 * ���Э��
	 * @return int 0�����ɹ���-1����ʧ��
	 */
	PROTOCOL_RETCODE pack(uint32 nSerialNo,uint32 nServantName,const char* pBody,uint32 nBodySize,IOutputStream& Output);

public :
	/**
	 * ���캯��
	 */
	CZWProtocol();
	/**
	 * ��������
	 */
	~CZWProtocol();
	/**
	 * ��ʼ����Э�����
	 * @param Buffer CBytesBuffer��������ݣ�������е����ݶ���
	 * @return void �����κη���
	 */
	//void begin(CBytesBuffer& Buffer);
	/**
	 * �Ƿ�����һ����
	 * @param Input IInputStream ���������
	 * @param Body Э���������
	 * @return PROTOCOL_RETCODE PRC_SUCCESS-����һ������PRC_ERROR-����������PRC_NOPACK--û�������İ�
	 */
	PROTOCOL_RETCODE next(IInputStream& Input,uint32& nSerialNo,uint32& nServantName,IOutputStream& Body);
	/**
	 * ���Э��
	 * @return int 0�����ɹ���-1����ʧ��
	 */
	PROTOCOL_RETCODE pack(uint32 nSerialNo,uint32 nServantName,const CBytesBuffer& Body,IOutputStream& Output);
	/**
	 * ��ȡ����Э���Ӧ������
	 * @param nServantName ����Э���������
	 * @param uint32 Ӧ��Э���������
	 */
	uint32 getResponseName(uint32 nServantName);
	/**
	 * �Ƿ�Ӧ��Э��
	 */
	bool isResponse(uint32 nServantName);
	/**
	 * �Ƿ�����Э�飬����ǣ�ֱ�����������账��
	 */
	bool isHeartBeat(uint32 nServantName);
	/**
	 * ����Ӧ�����������������������
	 * @param nServantName ��������
	 * @return uint32 �����
	 */
	uint32 getServantName(uint32 nServantName);
	/**
	 * ����������Ϣ
	 */
	PROTOCOL_RETCODE packHeartBeat(IOutputStream& Output);
	/**
	 * ����ʵ��
	 */
	IProtocol* newInstance();


	/**
	 *
	 */
	void incRef();
	/**
	 *
	 */
	bool decRef();
};

#endif
