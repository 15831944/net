#include "bytesbuffer.h"
#include <memory.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

inline uint32 DOALIGN(uint32 a,uint32 b)
{
	uint32 size = (a/b);
	size = ( size * b >= a ) ? size * b : (size+1) * b;
	return size;
}

#ifdef __cplusplus
}
#endif

//���캯��
CBytesBuffer::CBytesBuffer(int Buflen)
{
    if ( 0 != Buflen)
    {
        m_pBuf =  (char*)malloc(Buflen);
        m_BufLen = Buflen;
        m_freeLen = Buflen;
    }
    else
    {
        m_pBuf      = (char*)malloc(MEM_INCREASE_SIZE);
        m_BufLen    = MEM_INCREASE_SIZE;
        m_freeLen   = MEM_INCREASE_SIZE;
    }
}

//�������캯��
CBytesBuffer::CBytesBuffer(const CBytesBuffer& other)
{
	m_BufLen  = other.m_BufLen;
	m_freeLen = other.m_freeLen;
	m_pBuf = (char*)malloc(m_BufLen);
	memset(m_pBuf,0x00,m_BufLen);
	memcpy(m_pBuf,other.m_pBuf,m_BufLen  -  m_freeLen);
}

//���캯��
CBytesBuffer::~CBytesBuffer()
{
    if (m_pBuf != NULL)
    {
        free(m_pBuf);
    }
}

uint32 CBytesBuffer::getDataSize() const
{
	return m_BufLen - m_freeLen;
}

//�ɹ�����0��ʧ�ܷ��أ�1
int CBytesBuffer::readBytes(char *pBuf,uint32 &Buflen)
{
	if ( Buflen > getDataSize() )
	{
		Buflen = getDataSize();
	}
	memcpy(pBuf,m_pBuf,Buflen);
	popBytes(Buflen);
	return 0;
}

int CBytesBuffer::readBytes(CBytesBuffer& Buffer)
{
	if ( Buffer.getDataSize() == 0 ) 
		swap(Buffer);
	else
	{
		uint32 datasize = this->m_BufLen-this->m_freeLen;
		Buffer.writeBytes(this->m_pBuf,datasize);
	}
	m_freeLen = m_BufLen;
	return 0;
}

//�ɹ�����0��ʧ�ܷ���-1
int CBytesBuffer::writeBytes(const char* pBuf, uint32& Buflen)
{
	if ( Buflen > m_freeLen )
	{
		uint32 oldsize = m_BufLen;
		if ( this->resizeByAlign(Buflen+getDataSize()) == oldsize )
			return -1;
	}
	memcpy(m_pBuf+getDataSize(),pBuf,Buflen);
	m_freeLen -= Buflen;
	return 0;
}

//�ɹ�����0��ʧ�ܷ���-1
int CBytesBuffer::writeBytes(const CBytesBuffer& Buffer)
{
	uint32 len = ((CBytesBuffer&)Buffer).getDataSize();
	if ( len == 0 )
		return 0;
	return writeBytes(((CBytesBuffer&)Buffer).c_str(),len);
}

const char* CBytesBuffer::c_str()
{
    return m_pBuf;
}
//�ɹ�����ɾ�����ȣ�ʧ�ܷ��أ�1
int CBytesBuffer::popBytes(uint32 BytesToPop)
{
    if ( BytesToPop <= 0 )
    {
        return -1;
    }
    else if (BytesToPop >= getDataSize())
    {
        uint32 rec   = getDataSize();
        m_freeLen    = m_BufLen;
        return rec;
    }
    else
    {
        memmove(m_pBuf,m_pBuf + BytesToPop,getDataSize() - BytesToPop);
        m_freeLen  += BytesToPop;
        return BytesToPop;
    }
}

void CBytesBuffer::clear()
{
    m_freeLen = m_BufLen;
}

void CBytesBuffer::incRef()
{
     CRefShare::incRef();
}

bool CBytesBuffer::decRef()
{ 
    return CRefShare::decRef();
	//return;
}

int CBytesBuffer::increase(int32 IncrementSize)
{
    char *ptmpbuf = m_pBuf;

    m_pBuf    = (char*)realloc(m_pBuf, m_BufLen + IncrementSize );
    //����ʧ�ܣ�ָ��ԭ����λ�ã�ԭ���ݲ���ʧ
    if (NULL == m_pBuf)
    {
        m_pBuf  = ptmpbuf;
        return -1;
    }
    m_BufLen  += IncrementSize;
    m_freeLen += IncrementSize;
    return 0;
}

uint32 CBytesBuffer::compact()
{
	if ( m_freeLen <= MEM_INCREASE_SIZE )
		return this->m_BufLen;
	return resizeByAlign(getDataSize());
}

/**
* ���·����ڴ�
* @param DataSize ���·�����ڴ��С
*/

int CBytesBuffer::resize(uint32 DataSize)
{
	this->resizeByAlign(getDataSize() + DataSize);
	return 0;
}

/**
* ������Ч���ݳ���
* @param DataSize Ҫ���õ���Ч���ݳ���
*/
void CBytesBuffer::setDataSize(uint32 DataSize)
{
	if ( DataSize > m_BufLen )
		m_freeLen = 0;
	else
		m_freeLen = m_BufLen - DataSize;
	if ( m_freeLen != 0 )
		m_pBuf[DataSize] = 0;
}

/*
* ��ȡ���ݵ�ԭʼ��ʼָ��
* @return void* ���ݵ�ԭʼ��ʼָ��
*/
void* CBytesBuffer::getRawData()
{
    return m_pBuf;
}

void CBytesBuffer::swap(CBytesBuffer& Other)
{
	char* tmpptr = m_pBuf;
	this->m_pBuf =Other.m_pBuf;
	Other.m_pBuf = tmpptr;
	uint32 tmpvalue = m_BufLen;
	this->m_BufLen = Other.m_BufLen;
	Other.m_BufLen = tmpvalue;

	tmpvalue = m_freeLen;
	this->m_freeLen = Other.m_freeLen;
	Other.m_freeLen = tmpvalue;
}

uint32 CBytesBuffer::getBufferSize() const
{
	return this->m_BufLen;
}

uint32 CBytesBuffer::resizeByAlign(uint32 Size)
{
	if ( Size == 0 )
		Size = MEM_INCREASE_SIZE;
	else
		Size = DOALIGN(Size,MEM_INCREASE_SIZE);
	//����
	if ( Size == m_BufLen )
		return m_BufLen;
	char *ptmpbuf = m_pBuf;

	m_pBuf = (char*)realloc(m_pBuf,Size);
	//����ʧ�ܣ�ָ��ԭ����λ�ã�ԭ���ݲ���ʧ
	if ( NULL != m_pBuf )
	{
		//�п��ܼ���
		m_freeLen += (int)( Size - m_BufLen );
		m_BufLen = Size;
	} 
	else
		m_pBuf  = ptmpbuf;
	return m_BufLen;
}

void CBytesBuffer::reset()
{
	clear();
	compact();
}
