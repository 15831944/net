#ifndef _SERVANT_H_
#define _SERVANT_H_

#include <util/refshare.h>
#include <framework/request.h>
#include <framework/response.h>
#include <framework/current.h>


/*
ͳ����Ϣ
*/
typedef struct _t_statstics_info
{
	//��ִ�д���
	uint64 TotalExecTimes;
	//���µ�ÿ��ִ�д���
	uint32 ExecTimesPerSecond;
	//��ֵ��ÿ��ִ�д���
	uint32 PeekExecTimesPerSecond;

	//��ǰ������
	uint32 CurrentExecTimes;
	//��һ��ִ��ʱ��
	uint32 LastExecTime;

	_t_statstics_info()
	{
		memset(this,0,sizeof(struct _t_statstics_info));
	}

	/**
	 * ����ִ�д���
	 */
	void incExecute();
	/**
	 * ���л�ͳ����Ϣ
	 */
	void serial(char* pBuff,int BufLen);
}STATISTICS_INFO;

class IServant : public IRefShare
{
public:
	/**
	 * ���캯��
	 */
    IServant() 
    {
    }
	/**
	 * ��������
	 */
    virtual ~IServant() {  }
    virtual IResponsePtr execute(IRequestPtr Request, CCurrentPtr pCurrent) = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual IRequestPtr createRequest() = 0;
    virtual void onOpen(IEndPointPtr pEndpoint) {  }
    virtual void onClose(IEndPointPtr pEndpoint, CSessionPtr Session) {  }

	/**
	 * ����ִ�д���
	 */
    void incExecuteTimes();
	/**
	 * ��ȡͳ����Ϣ
	 */
	STATISTICS_INFO& getStatsticsInfo();

private:
	STATISTICS_INFO m_StatisticsInfo;
};

typedef CRefObject<IServant> IServantPtr;

/*
ģ�巽�������԰�����������ⷽ����ΪServant
*/
template <typename T>
class TServant : public IServant
{
public:
    TServant(T* pClass, IResponsePtr (T::*pfnExecute)(IRequestPtr Request, CCurrentPtr pCurrent), IRequestPtr (T::*pfnCreateRequest)() ) :
    m_pfnExecute(pfnExecute),  m_pfnCreateRequest(pfnCreateRequest)   
    {
        
    }
    
    virtual ~TServant() 
    {
        
    }

    virtual IResponsePtr execute(IRequestPtr Request, CCurrentPtr pCurrent)
    {
        return (m_pClass->*m_pfnExecute)(Request, pCurrent);
    }

    virtual void pause() = 0;
    virtual void stop() = 0;

    virtual IRequestPtr createRequest()
    {
        return (m_pClass->*m_pfnCreateRequest)();
    }

private:
    T* m_pClass;
    IResponsePtr (T::*m_pfnExecute)(IRequestPtr Request, CCurrentPtr pCurrent);
    IRequestPtr (T::*m_pfnCreateRequest)();
};

typedef struct _t_servantinfo
{
	//ͳ������
	STATISTICS_INFO StatsticsInfo;
	//servant ��ָ��
	IServantPtr pServant;
}SERVANTINFO;

class CDAServer;

/*
Servant���������ڶ����ṩServant�����л���
*/
class IServantContainer : public IRefShare
{
public :
	virtual ~IServantContainer() { }
	/**
	 * ����servant
	 */
	virtual void addServant(uint32 Name, IServantPtr Servant) = 0;

	virtual IServantPtr getServant(uint32 Name) = 0;
	/**
	 * ��ʼ������
	 */
	virtual bool init(CDAServer* pServer) = 0;
	/**
	 * ��������
	 */
	virtual void start() = 0;
	/**
	 * ֹͣ����
	 */
	virtual void stop() = 0;
	/**
	 * �Ƿ������servant
	 */
	virtual bool hasServant(uint32 Name) = 0;
	/**
	 * �����������
	 */
	virtual IRequestPtr createRequest(uint32 nServantName) = 0;
	/**
	 * ����servant
	 */
	virtual void callServant(uint32 Name, IRequestPtr Request,CCurrentPtr pCurrent) = 0;
};

typedef CRefObject<IServantContainer> IServantContainerPtr;


#endif 

