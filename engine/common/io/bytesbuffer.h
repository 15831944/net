#ifndef _CBYTESBUFFER_H_
#define _CBYTESBUFFER_H_
#include "inputoutputstream.h"

#define  MEM_INCREASE_SIZE 1024
class CBytesBuffer :public CRefShare,public IInputOutputStream
{
public:
    /*
    *���캯��
    *@Buflen ���泤��
    */
    CBytesBuffer(int Buflen=0);
	/*
	 *�������캯��
	 *@other CBytesBuffer
	 */
	CBytesBuffer(const CBytesBuffer& other);
    /*
    *���캯��
    */
    virtual ~CBytesBuffer();
    /*
    *��ȡһ�λ���
    *@out:pBuf
    *@in: Buflen����
    *return :�����ĳ���
    */
    virtual int readBytes(char *pBuf,uint32& Buflen);
	/**
	 * ��ȡ���ݵ�BytesBuffer��
	 */
	int readBytes(CBytesBuffer& Buffer);
    /*
    *д��һ�λ���
    *@in: Buflen����
    *return :д��ĳ���
    */
    virtual int writeBytes(const char* pBuf, uint32& Buflen);
    /*
    *д��һ�λ���
    *@in: Buflen����
    *return :д��ĳ���
    */
    virtual int writeBytes(const CBytesBuffer& Buffer);
    /*
    *�Ƴ�һ�λ���
    */
    virtual int popBytes(uint32 BytesToPop);
    /*
    *���ػ�����Ч����
    */
    virtual uint32 getDataSize() const;
    /*
    *���ػ���
    */
    virtual const char* c_str();

	/**
	 * �����������
	 */
	virtual void clear();
	/**
	 * ������������
	 */
	void reset();
	/*
	*
	*/
	void incRef();
	/*
	*
	*/
	bool decRef();
    /**
     * �����ڴ�
     * @param IncrementSize �����ڴ��С��Ĭ��Ϊ0������ʹ��bytesbuffer�����Ĭ���������ȡ�
     */
    int increase(int32 IncrementSize = MEM_INCREASE_SIZE );
    /**
     * ���·����ڴ�
     * @param DataSize ���·�����ڴ��С
     */
    int resize(uint32 DataSize);
	/**
	 * ѹ���ڴ棬�ͷſ�����ڴ�
	 * @return int ʣ��Ļ������ռ�
	 */
	uint32 compact();
	/**
	 * ��ȡ��ǰ�ܵĻ�������С
	 * @return uint32 �������ܳ��ȣ���λ���ֽ�
	 */
	uint32 getBufferSize() const;

	/**
	 * ������Ч���ݳ���
	 * @param DataSize Ҫ���õ���Ч���ݳ���
	 */
	void setDataSize(uint32 DataSize);
	/*
	 * ��ȡ���ݵ�ԭʼ��ʼָ��
	 * @return void* ���ݵ�ԭʼ��ʼָ��
	 */
	void* getRawData();
	/**
	 * ��Other��BytesBuffer��������
	 * @param Other CBytesBuffer 
	 */
	void swap(CBytesBuffer& Other);

private :
	/**
	 * �����ֽڶ��룬���·����С������µĿռ�С����Ч���ݳ��ȣ���ᶪʧ����
	 * @param DataSize,Ҫ����Ĵ�С����λ�ֽ�
	 * @return uint32 ���·����Ŀռ��С
	 */
	uint32 resizeByAlign(uint32 DataSize);

private:
    /*
    *����ָ��
    */
    char *m_pBuf;
    /*
    *�����ܳ���
    */
    uint32 m_BufLen;
    /*
    *������ó���
    */
    uint32 m_freeLen;
};

#include "util/objectpool.h"
typedef CPoolObjectSharePtr<CBytesBuffer> CBytesBufferPtr;

#endif

