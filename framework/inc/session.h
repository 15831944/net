#ifndef _SESSION_H_
#define _SESSION_H_

#include <framework/serial.h>
#include <util/refshare.h>
#include <util/objectpool.h>

struct _t_sessionvalue;

class CSession : public CRefShare
{
public:
    CSession();
    ~CSession();
	/**
	 * ����sessionֵ�����key�Ѵ��ڣ�����滻
	 * @
	 */
    void setInt(int Name, int32 Value);
	void setInt64(int Name, int64 Value);
	void setString(int Name, const char* Value);
	/**
	 * ����ָ�룬������֧�����ü�����ָ��,����sessionӵ�д�ָ��ʱ�����Ӷ��ڵ�����
	 */
	void setPtr(int Name,IRefShare* Value);
	/**
	 * �Ƿ�������key
	 */
	bool isExist(int Name);
    /**
	 * ��ȡsessionֵ
	 * @param Name key��
	 * @param bThrowable �������Ƿ����쳣��Ĭ��Ϊtrue
	 * @param bDefault Ĭ��ֵ
	 * @return int32 �����ֵ
	 */
    int32 getInt(int Name,bool bThrowable = true,int32 nDefault = 0);
    /**
	 * ��ȡsessionֵ
	 * @param Name key��
	 * @param bThrowable �������Ƿ����쳣��Ĭ��Ϊtrue
	 * @param bDefault Ĭ��ֵ
	 * @return int64 �����ֵ
	 */
    int64 getInt64(int Name,bool bThrowable = true,int64 nDefault = 0);
    /**
	 * ��ȡsessionֵ
	 * @param Name key��
	 * @param bThrowable �������Ƿ����쳣��Ĭ��Ϊtrue
	 * @param bDefault Ĭ��ֵ
	 * @return int32 �����ֵ
	 */
    const char* getString(int Name,bool bThrowable = true,const char* pDefault = "");
    /**
	 * ��ȡsessionֵ
	 * @param Name key��
	 * @param bThrowable �������Ƿ����쳣��Ĭ��Ϊtrue
	 * @param bDefault Ĭ��ֵ
	 * @return IRefShare* �����ֵ
	 */
    IRefShare* getPtr(int Name,bool bThrowable = true,IRefShare* pDefault = NULL);
	/**
	 * ɾ��sessionֵ
	 * @return bool �Ƿ���ڴ�ֵ
	 */
	bool remove(int Name);
    
    void setSessionID(uint32 nID);

    uint32 getSessionID();
	void reset();

private:
    uint32 m_nSessionID;
	//�ڲ�����
	map<int,struct _t_sessionvalue> m_Values;
};

typedef CPoolObjectSharePtr<CSession> CSessionPtr;

#endif 
