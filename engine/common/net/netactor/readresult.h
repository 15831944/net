#ifndef _READRESULT_H_
#define _READRESULT_H_

#include "netresult.h"
#include "../../io/bytesbuffer.h"

class CReadResult : public CNetResult
{
public:
    CReadResult();
    virtual ~CReadResult();

    uint32 getSize();
    const char* c_str();
	
	/**
	 * ���ÿɶ�ȡ������
	 */
    void setData(const char* pData, uint32 nDataSize);

	/**
	 * ���ÿɶ�ȡ������
	 */
	void setData(CBytesBuffer& DataBuffer);

private:
	const char* m_pData;
	uint32 m_nDataSize;
};

#endif
