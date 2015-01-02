#ifndef _IINPUTSTREAM_H_
#define _IINPUTSTREAM_H_

#include "public.h"
#include "util/refshare.h"

class IInputStream : public IRefShare
{
public:
	/**
	 * ��������
	 */
    virtual ~IInputStream() {}

	/**
	 * ��ȡ����
	 * @param pBuf char* �������ݵ�ָ��
	 * @param Buflen uint32 �����������������-���������ȣ����-�ַ�����
	 * @return int �ɹ����0--�ɹ��������������롣
	 */
    virtual int readBytes(char *pBuf, uint32& Buflen) = 0;

	/**
	 * ����ָ���ֽ���������
	 * @param BytesToPop uint32 Ҫ�������ֽ���
	 * @return int �ɹ����0--�ɹ��������������롣
	 */
    virtual int popBytes(uint32 BytesToPop) = 0;
	/**
	 * ��ȡ�ж��ٿɶ����ݴ�С����λ���ֽ�
	 */
    virtual uint32 getDataSize() const = 0;
	/**
	 * ��ȡ�ֽ�ָ��
	 */
    virtual const char* c_str() = 0;
};

typedef CRefObject<IInputStream> IInputStreamPtr;

#endif

