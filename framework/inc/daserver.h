#ifndef _DASERVER_H_
#define _DASERVER_H_

#include <framework/framepublic.h>
#include <framework/context.h>
#include <framework/endpoint.h>
#include <net/netactor/nethandler.h>
#include <framework/request.h>
#include <framework/response.h>
#include <framework/servant.h>
#include <map>
#include <net/netactor/reactor.h>
#include <net/netactor/nethandler.h>
#include <net/netactor/netactordriver.h>
#include <event/timeservice.h>
#include <net/connection.h>
#include <framework/serial.h>
#include <framework/responsehandler.h>
#include <list>
#include <vector>
#include <framework/sessionhandler.h>
#include <framework/current.h>
#include <framework/bussiness.h>
#include <framework/commonptl.h>
#include <event/timeservice.h>
#include <util/activeobject.h> // TActiveObject
#include <framework/daexception.h>

#include <net/driver/tcpsocketdriver.h>
#include <net/driver/ip4address.h>
#include <net/netactor/reactor.h>
#include <util/profile.h>
#include <util/datetime.h>
#include <util/lightlock.h>
#include <util/tlobject.h>

// Ret Header Ret Code
#ifndef SERVANT_NOTFOUND
    #define SERVANT_NOTFOUND 0x00000004
#endif

#define HANDLER_TIMEOUT_INFINITE -1

#define PREFIX_RESPONSE 0X80000000
//��ȡ Servant Name ��ǰ׺
#define GET_SERVANT_PREFIX(ServantName) ( ( ( ServantName ) >> 16 ) & 0xFFFF )

/*

Class	CEmptyRequest

*/
class CEmptyRequest : public IRequest
{
public:

    virtual bool serial(char *pOut, int &Length){ return true; }
    virtual bool serial(IOutputStreamPtr Output) { return true;	}
    virtual bool unserial(const char *pIn, int Length) { return true; }
    virtual bool unserial(IInputStreamPtr Input) {	return true; }
};

/*
 EndPoint��Ϣ������
*/
class DLLAPI CEndPointManager 
{
private:
	typedef list<CSTLRefObject<IEndPoint> > ENDPOINT_LIST;
	// ���� map �����������ͬ, Ϊ���ڲ�ͬ���������߲��ҵ�Ч��
	std::map<uint32,ENDPOINT_LIST> m_mapServantInfo_name; // ���� ServantName  ��Ϊ��������� map
	std::map<std::string,IEndPointPtr> m_mapServantInfo_ip;   // ���� ip:port ��ʽ ��Ϊ��������� map
	CLightLock m_Lock;
public:
	CEndPointManager();
	~CEndPointManager();
	/**
	 * �����µ�EndPoint
	 * @param pEndPoint Ҫ�����EndPoint
	 */
	void registry(IEndPointPtr pEndPoint);
	/**
	 * ΪEndPointbע��ServantNameǰ׺
	 * @param pEndPoint �����EndPoint�����EndPoint�Ѵ��ڣ��򲻻��ظ�����
	 * @param nServantNamePrefix 
	 */
	void registry(IEndPointPtr pEndPoint,uint32 nServantNamePrefix);
	/**
	 * ɾ��ָ��endpoint����Ϣ
	 */
	void unregistry(const char* pEndPointName);

	/**
	 * ����EndPointName���Ҷ�Ӧ��EndPoint
	 */
	IEndPointPtr find(const char* pEndPointName);
	/**
	 * ����ServantName���Ҷ�Ӧ��EndPoint
	 */
	IEndPointPtr find(uint32 nServantNamePrefix);
	/**
	 * ������endpoint�Ƿ������prefix
	 * @retur bool �Ƿ����
	 */
	bool isEndPointPrefix(uint32 nServantNamePrefix,IEndPointPtr pEndPoint);
};

class CResponseTask;
//������ջ
typedef std::stack<CCurrentPtr> CURRENT_STACK;

//DAServer�Ĵ���
class IDAServerHandler : public IRefShare
{
public :
	/**
	 * ������ֹͣ
	 */
	virtual void onServerStop() = 0;
	/**
	 * �����ļ�����
	 */
	virtual void onProfileUpdate(CDAServer& Server) = 0;
};

typedef CRefObject<IDAServerHandler> IDAServerHandlerPtr;

class DLLAPI CDAServer : public INetHandler, public CRunnable
{
protected:
	friend class CResponseTask;
	//�Ƿ��������Դ�ĳ�ʼ��
	bool m_bIsInited;
	// Server�������ļ�
	CProfile* m_pProfile;
	//������ƶ˿�
	int m_nSvrPortArgc;
	//��ʱ����
	CTimeService m_TimeSvr;
	//��ˮ��
	int32 m_nSerialNo;
	//����ģ��
	IReactorPtr m_pReactor;
	//���ص�servant
	std::map<uint32, SERVANTINFO> m_mapServants;
	//�����˿�����ִ�е�servantǰ׺
	std::map<uint16, std::vector<uint16> > m_mapServantPrefixs;
	//servant������
	vector<IServantContainerPtr> m_ServantContainers;
	//���г�ʱ����λ������
	int32 m_nIdleTimeOut;
	//servantִ��ʱ�䣬��λ������
	uint64 m_nServantExecuteTime;

	//-------------��ʱ����--------------------
	//��ʱ������ص���
	CBaseLock m_TimeoutLock;
	//��ʱ���񣭣������б�
	typedef list<CSTLRefObject<IEndPoint> > ENDPOINT_LIST;
	ENDPOINT_LIST m_ReConnList;
	//��ʱ������
	uint32 m_nTimeoutCount;
	//��ǰʱ��
	uint32 m_nCurrentTime;

	//---------------������صı���-------------------
	// ������Ϣ, ÿ������ֻ��һ�� Session & RecvBuf
	typedef struct _t_connection_data
	{
		//��Ӧ����������
		IConnectionPtr pConnection;
		//�Ự��Ϣ
		CSessionPtr pSession;
		//����˵�
		IEndPointPtr pEndPoint;
		//�����ȡʱ�䣬��λ������
		uint64 nLastReadTime;
	}CONNECTION_DATA, *LPCONNECTION_DATA;
	//�������пͻ��˵����� <SOCKET, CONNECTION>
	//�������еĻỰ��Ϣ��key=SESSIONID
	std::map<uint32, LPCONNECTION_DATA> m_Sessions;
	//�Ự������
	typedef list<CSTLRefObject<ISessionHandler> > SESSIONHANDLER_LIST;
	SESSIONHANDLER_LIST m_SessionHandlers;

	//EndPoint�ı����Լ�������Ϣ
	CEndPointManager m_EndPointManager;

	typedef struct _t_handle_info
	{
		IEndPointHandlerPtr pEndPointHandler;
		uint32 nMask;
		_t_handle_info()
		{
			nMask = 0;
			pEndPointHandler = NULL;
		}
	}HANDLE_INFO;
	//�Բ�����ĳЩservant��
	std::map<uint32,HANDLE_INFO> m_ServantHandlers;
	typedef struct _t_endpoint_info
	{
		LPCONNECTION_DATA pConnData;
		HANDLE_INFO HandleInfo;
	}ENDPOINT_INFO,*LPENDPOINT_INFO;
	//��Դ������
	CLightLock m_Lock;
	//endpointname-handler
	std::map<string,ENDPOINT_INFO> m_EndPoints;

	//-------------ҵ���߳���ر���--------------
	//ҵ�����߳�
	TActiveObject<CDAServer, void*> m_Thread;
	//�����������ݵ���
	CLightLock m_ConnDataLock;
	//ҵ�����ͬ����
	CBaseLock m_BusinessLock;
	//ҵ�����(����ҵ������Ҫ����˶��У���ҵ���̴߳���)
	typedef std::list<CSTLRefObject<IBussiness> > BUSINESS_LIST;
	BUSINESS_LIST m_lstBusiness;
	//ҵ���߳����б�ʶ
	bool m_bIsRun;
	typedef struct _t_RESPONSE_DATA
	{
		uint32 nServantName;
		IResponseHandlerPtr	pResponseHandle;
		void* pParam;
		CCurrentPtr pCurrent;
		//��response�ĳ�ʱ���
		CDateTime BeginTime;
		//��ʱ���ƣ���λ����
		uint32 nTimeOut;
	}RESPONSE_DATA, *LPRESPONSE_DATA;
	//�������е�Ӧ�����Ӧ�Ĵ����� RequestID, LPRESPONSE_DATA
	std::map<uint32, RESPONSE_DATA> m_mapResponsees;
	//ʱ�䴦��
	IDAServerHandlerPtr m_pServerHandler;

	//servant��current��ջ
	// ���� Servant.execute() ֮ǰ��Current ���浽��ջ
	//������̵߳���callservant���ᵼ��current���ң���Ҫʹ�ñ��ر���������
	CTlObject m_Currents;

protected:
    //----------------INetHandler �ӿں���--------------------------
    virtual void onRead(CReadResult& result, void* Param);
    virtual void onWrite(CWriteResult& result, void* Param);
    virtual void onOpen(CNetResult& result, void* Param);
    virtual void onClose(CNetResult& result, void* Param);
    virtual void onAccept(CNetResult& result, void* Param);
    //---------------���ڲ����ú���---------------------
    /**
     * ҵ���̴߳�����
     */
    void doBusiness(void* pParam);
	/**
	 * ��ȡsessionid
	 * @param nSessionID ����������õ���sessionid
	 * @return bool �Ƿ��ȡ�ɹ�
	 */
	bool getSessionID(uint32& nSessionID);
	/**
	 * �����ļ��б��
	 */
	void onProfileUpdate();

protected:
	/**
	 * ����requestID
	 */
    int32 makeRequestID();
	/**
	 * �ж϶˿�port�Ƿ���servantǰ׺������
	 */
	bool isLegalServantPrefix(uint16 port, uint16 servantprefix);
    /**
     * Ӧ��servant�Ҳ����Ĵ�����Ϣ���ظ�������[�ظ�ͷ], [�ظ�ͷ]��ErrorCodeΪ NotFoundServant
     */
    bool respondNotFoundServant(uint32 nSerialno, uint32 nServantName, IEndPointPtr pEndPoint);
    /**
     * ����ظ���
     */ 
    virtual bool processResponse(uint32 nSerialNo, uint32 nServantName, LPCONNECTION_DATA pConnData, IInputStream& pBody);
    /**
     * ���������
     */
    bool processRequest(uint32 nSerialNo, uint32 nServantName, LPCONNECTION_DATA pConnData, IInputStream& pBody);
	/**
	 *  ��ʼ�� DAServer �����з��񣬰�����ʼ�������ķ���(Acceptor)��Զ�̷���(Connector)
	 * @return bool �Ƿ�ɹ�
	 */
	bool initService();
    /**
     * ִ�ж�ʱ�¼�
     */
    int executeTimeTask( void *pParam );
	/**
	 * ��ʱ����
	 */
	int onTimeout(void *pParam);
	/**
	 * ��ʱ�������
	 */
	int finishTimeout(void *pParam);
	/**
	 * ����Ƿ���ҵ��ʱ
	 */
	int checkResponseTimeout(void *pParam);
	/**
	 * ���뵽ҵ�����������
	 */
	void putBusiness(IBussinessPtr pBusiness);
	/**
	 * ����������ҵ������
	 */
	void putBusinessList(BUSINESS_LIST& BusinessList);
	/**
	 * ��ȡCurrent
	 * @param nSessionID ���Ϊ0��������ݵ�ǰ��ջ��Ϣ��ȡcurrent�������Ϊ0�����ȡָ��session����Ϣ������current
	 */
	CCurrentPtr createCurrent(uint32 nSessionID,uint32 nRequestID,uint32 nServantName);
	/**
	 * ���endpoint�������ö�Ӧ�Ĵ�����
	 * @param Address ��ַ
	 * @param nPort �˿�
	 * @param EndPointType endpoint���ͣ����IEndPoint::EENDPOINT_TYPE��ö��ֵ
	 * @param SockType endpoint���������ͣ����IEndPoint::EENDPOINT_SOCKTYPE��ö��ֵ
	 * @param isUseEncrypt �Ƿ�ʹ�ü���
	 * @return IEndPointPtr ���ӳɹ��󣬷��ض�Ӧ��ָ�룬�������NULL����֤������ʧ��
	 */
	IEndPointPtr addEndpoint(const string& Address,uint16 nPort,IEndPoint::EENDPOINT_TYPE EndPointType,
		IEndPoint::EENDPOINT_SOCKTYPE SockType,IProtocolPtr pProtocol,IEncryptorPtr pEncryptor = NULL,
		int nNetBuffSize = 512,uint32 nMask = 0,IEndPointHandlerPtr pEndPointHandler = NULL);
	/**
	 * �Ƴ�endpoint������ɾ����Ӧ�����д���
	 * @return IEndPointPtr ɾ���ɹ��󣬷��ض�Ӧ��ָ�룬�������NULL��֤��ɾ��ʧ�ܣ��޴�endpoint��
	 */
	IEndPointPtr removeEndpoint(const string& Address,uint16 nPort,IEndPoint::EENDPOINT_TYPE EndPointType);

	CDAServer();
	friend class CRefObject<CDAServer>;
	friend class CRefObject<CCurrent>;
	friend class CCurrent;
	friend class CRequestTask;
	friend class CDBServantService;

public:
	virtual ~CDAServer();

	//--------------------��ʼ���ӿ�-----------------------------
	/**
	 * ��ʼ������, ���к�������֮ǰ�����ȵ��� init()
	 */
	bool init();

	/**
	 *
	 */
	bool uninit();

	/**
	 * ��ʼ���� DAServer
	 */    
	virtual void run();

	/**
	 * ֹͣ���� DAServer
	 */
	virtual void stop();

	/**
	 * ��� Servant
	 * @param ContainerID ����id�����id��addServantContainer�ķ���ֵ��Ϊ0������뵽daserver�����servant������
	 */
	void addServant(uint32 Name, IServantPtr Servant,int ContainerID = 0);

	IServantPtr getServant(uint32 Name,int ContainerID = 0);

	/**
	 * ���servant��������
	 * @return int ���>0����Ϊcontainer��id�����<0������ʧ��
	 */
	int addServantContainer(IServantContainerPtr pServantContainer);

	/**
	 *  ��� EndPoint,�������Ӧ���¼�
	 * @param EndPoint ����˵�
	 * @param nMask ��Ҫ������¼���ʶ��
	 * @param pEndPointHandler �¼�������
	 */ 
	//void addEndpoint(IEndPointPtr EndPoint,uint32 nMask = 0,IEndPointHandlerPtr pEndPointHandler = NULL);

	/**
	 * ����ָ��servant��endpoint����
	 * @param nServantNamePrefix servantnameǰ׺
	 * @param IEndPointHandlerPtr ������
	 * @return 
	 */
	void registry(uint32 nServantNamePrefix,uint32 nMask,IEndPointHandlerPtr pEndPointHandler);

	/**
	 *  ��Ӷ�ʱ����
	 */
	TASKHANDLE addTimeTask(ITimeTaskPtr TimerTask, int32 nDelay, int32 nInterval, void *Param, int16 nRepeatTimes);    

	//------------------------------�ṩ��ҵ��Servant���õĽӿ�--------------------------------------
	/**
	 * ����ָ���Ŀͻ���
	 * 
	 */
	void send(uint32 SessionID, uint32 nServantName, ISerialPtr pMessage,IResponseHandlerPtr pResponseHandler = NULL,uint32 nTimeOut = 0, void* Param = NULL);

	/**
	 * �����пͻ��˹㲥��Ϣ
	 */
	void broadcast(uint32 nName, ISerialPtr Message);
	/**
	 * �㲥������ӵ�д�ServantName��EndPoint
	 */
	//void broadcastServant(uint32 nServantName, IRequestPtr Request, IResponseHandlerPtr Response);

	/**
	 * call by servant
	 * @param nTimeout ���� ��ʱʱ�䣬���Ϊ0����ΪĬ��ֵ��10
	 * @param nSessionID -- ʹ��ĳ��session��Ϊcallservant��session�����Ϊ0����ȡ��ǰ��session
	 */
	void callServant(uint32 Name, IRequestPtr Request, IResponseHandlerPtr Response, uint32 nTimeOut, void* Param, const char* pszEndPoint,uint32 nSessionID = 0);
	// �ṩ��ҵ��Servant���õĽӿ� -- end

	/**
	 * ��ȡstring���͵���������
	 */
	std::string getSetting(const std::string& strSessionName, const std::string& strKeyName, const std::string& strDefValue);
	/**
	 * ��ȡuint32���͵���������
	 */
    uint32 getSettingUint32(const std::string& strSessionName, const std::string& strKeyName, const std::string& strDefValue);

	/**
	 * ��ȡ��ǰ���������ļ����Ѿ��Ѷ���������ļ�Ҳһ��ϲ��ˡ�
	 */
	CProfile& getProfile();
	/**
	 * ע���session�Ĺ���
	 */
	void addSessionHandler(ISessionHandlerPtr pSessionHandler);
	/**
	 * �ر�session�����ӣ����Ϲرգ�
	 * @param nSessionID ���� �Ựid
	 */
	void closeSession(uint32 nSessionID);
	/**
	 * ���÷��������¼�������
	 */
	void setServerHandler(IDAServerHandlerPtr pHandler);
	/**
	 * ��ȡ�����ļ�
	 */
	bool loadProfile();
	//---------------servant container����----------
    /**
     * ���ص���ʱ����ظ�, �� CCurrent��ServantContainer����
     */
    bool response(uint32 nRequestID, IResponsePtr pResponse);
	/**
	 * ��ȡ��ǰ�̵߳�ջ����
	 */
	CURRENT_STACK& getCurrentStack();
	/**
	 * ɾ����ǰ�̵߳Ķ�ջ
	 */
	void destoryCurrentStack();

	/**
	 * ����Զ�̵��õĽڵ�
	 */
	bool addCallEndpoint(const string& Address,uint16 nPort,const string& ServantList,IProtocolPtr pProtocol = NULL,bool isUseEncrypt = false,int nNetBuffSize = 512);
	/**
	 * ɾ��Զ�̵��õĽڵ�
	 */
	bool removeCallEndpoint(const string& Address,uint16 nPort);

	//��ȡsessionID
	bool getSessionIDByEndpointName(string strName,uint32& nSessionID);
};

typedef CRefObject<CDAServer> CDAServerPtr;

/*

Class	CEmptyResponse

*/
class CEmptyResponse : public IResponse
{
public:
	uint32 m_nRetCode;

	virtual bool serial(char *Output, int &Length){return true;}
	virtual bool serial(IOutputStream& Output)
	{
		writeuint32(Output, m_nRetCode);
		return true;
	}
	virtual bool unserial(const char *Input, int Length){return true;}
	virtual bool unserial(IInputStream& Input)
	{
		readuint32(Input, m_nRetCode);
		return true;
	}
	CEmptyResponse()
	{
		m_nRetCode = -1;
	}
	CEmptyResponse(const CEmptyResponse &t)
	{
		m_nRetCode = t.m_nRetCode;
	}
	CEmptyResponse& operator = (const CEmptyResponse& t)
	{
		m_nRetCode = t.m_nRetCode;
		return *this;
	}
	virtual ~CEmptyResponse()
	{
	}
};


#endif 
