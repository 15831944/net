#include <public.h>
#include <framework/daserver.h>
#include <net/netactor/netactordriver.h>
#include <net/netactor/netactor.h>
#include <net/netactor/reactor.h>
#include <util/drivermanager.h>
#include <util/baselock.h>
#include <framework/responsetask.h>
#include <framework/requesttask.h>
#include <net/driver/tcpsocketdriver.h>
#include <net/driver/udpsocketdriver.h>
#include <net/driver/ip4address.h>
#include <util/getopt.h>
#include <util/profile.h>
#include <net/netactor/platform.h>
#include <framework/timerbusiness.h>
#include <framework/zwprotocol.h>
#include <framework/debuginfo.h>
#include <util/autoptr.h>
#include <util/datetime.h>
#include "sessionhandlebusiness.h"
#include <framework/commonfun.h>
#include <file/fileutil.h>
#include <util/memutil.h>

#include <zwencryptor.h>

#define DEFAULT_TIMEOUT 300
//��������֡���
#define TICK_INTERVAL 25
#define SECOND_COUNT (1000/TICK_INTERVAL)
#define MINUTE_COUNT (60*SECOND_COUNT)


//�ж��Ƿ�IP��ַ
bool isIP(const char* pHost)
{
    return ( INADDR_NONE != inet_addr(pHost) );
}

//�ж��Ƿ�˿�
bool isPort(const char* pPort)
{
    const char* tmp = pPort;

    if (*tmp == '-' )
        tmp++;
    else if(*tmp == '+' )
        tmp++;

    while( '\0' != *tmp)
    {
        if( (*tmp < '0') || (*tmp >'9') )
            return false;
        tmp++;
    }

    uint32 nPort = atoi( pPort );
    if ( nPort > MAX_UINT16 )
        return false;

    return true;
}

inline const char* getErrMsg(int32 ErrNo)
{
#ifdef WINDOWS
    static char errmsg[1024];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,ErrNo,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPSTR)&errmsg,sizeof(errmsg),NULL);
    return errmsg;
#else
    return strerror(ErrNo);
#endif
}


/*
inline std::string getEndPointName(IEndPointPtr pEndPoint)
{
if ( NULL != pEndPoint->getAcceptor() )
return pEndPoint->getAcceptor()->getListenAddress()->getString();
if ( NULL != pEndPoint->getConnector() )
return pEndPoint->getConnector()->getConnectAddress()->getString()+ "|" +pEndPoint->get;
return "";
}
*/

static uint32 g_nSessionID = 1;

/*
* min()/max() macros that also do
* strict type-checking.. See the
* "unnecessary" pointer comparison.
*/
/*
#define min(x,y) ({ \
typeof(x) _x = (x);	\
typeof(y) _y = (y);	\
(void) (&_x == &_y);		\
_x < _y ? _x : _y; })

#define max(x,y) ({ \
typeof(x) _x = (x);	\
typeof(y) _y = (y);	\
(void) (&_x == &_y);		\
_x > _y ? _x : _y; })
*/
//
#if (defined DEBUG) || (defined _DEBUG)
void printfandlog()
{

}

#else
int printfnon(const char *format, ...)
{
}

void log(const char *format, ...)
{
}
#endif

#define DBGPRINT printf
#define LOG printf 


/*

Class	CEndPointManager

*/
CEndPointManager::CEndPointManager()
{
}

CEndPointManager::~CEndPointManager()
{
}

IEndPointPtr CEndPointManager::find(const char* pEndPointName)
{
	CAutoLock lock(m_Lock);
	std::map<std::string, IEndPointPtr>::iterator iter_find;
	iter_find = m_mapServantInfo_ip.find( pEndPointName );

	if( iter_find != m_mapServantInfo_ip.end() )
	{
		return iter_find->second;
	}
	else
	{
		return NULL;
	} 
}

void CEndPointManager::unregistry(const char* pEndPointName)
{
	CAutoLock lock(m_Lock);
	std::map<std::string, IEndPointPtr>::iterator iter;
	iter = m_mapServantInfo_ip.find( pEndPointName );
	if ( iter == m_mapServantInfo_ip.end() )
		return ;
	IEndPointPtr pendpoint = iter->second;
	m_mapServantInfo_ip.erase(iter);

	for ( std::map<uint32,ENDPOINT_LIST>::iterator i = m_mapServantInfo_name.begin() ; i != m_mapServantInfo_name.end() ; i ++ )
	{
		for ( ENDPOINT_LIST::iterator j = i->second.begin() ;  j != i->second.end() ; )
		{
			if ( (*j).get() == pendpoint )
			{
				i->second.erase(j++);
				continue;
			}
			else
				++j;
		}
	}
}


IEndPointPtr CEndPointManager::find(uint32 nServantName)
{
	CAutoLock lock(m_Lock);
	std::map<uint32, ENDPOINT_LIST>::iterator iter_find;
	iter_find = m_mapServantInfo_name.find( nServantName );

	if( iter_find != m_mapServantInfo_name.end() )
	{
		if ( iter_find->second.size() > 0 )
			return iter_find->second.front().get();
		return NULL;
	}
	else
	{
		iter_find = m_mapServantInfo_name.find(GET_SERVANT_PREFIX(nServantName) );
		if( iter_find != m_mapServantInfo_name.end() )
		{
			if ( iter_find->second.size() > 0 )
				return iter_find->second.front().get();
			return NULL;
		}
		return NULL;
	}
}

bool CEndPointManager::isEndPointPrefix(uint32 nServantNamePrefix, IEndPointPtr pEndPoint)
{
    if ( pEndPoint == NULL )
        return false;

	CAutoLock lock(m_Lock);
    std::map<uint32, ENDPOINT_LIST>::iterator iter_find;
    iter_find = m_mapServantInfo_name.find( nServantNamePrefix );

    if( iter_find != m_mapServantInfo_name.end() )
    {
        for ( ENDPOINT_LIST::iterator i = iter_find->second.begin(); i != iter_find->second.end() ; i ++ )
        {
            if ( (*i) == pEndPoint )
                return true;
        }
    }
    return false;
}

void CEndPointManager::registry(IEndPointPtr pEndPoint)
{
	if ( pEndPoint == NULL )
		return;

	CAutoLock lock(m_Lock);
	string name = pEndPoint->getName();
	map<string,IEndPointPtr>::iterator i = m_mapServantInfo_ip.find(name);
	if ( i != m_mapServantInfo_ip.end() )
	{
		if ( (*i).second != pEndPoint )
		{
			throwDAException(-3,"registry same endpoint name but endpoint pointer not equal");
		}
		return ;
	}
	m_mapServantInfo_ip.insert( std::make_pair<string, IEndPointPtr>( name, pEndPoint ) );
}

void CEndPointManager::registry(IEndPointPtr pEndPoint, uint32 nServantNamePrefix)
{
	if ( pEndPoint == NULL )
		return;

	CAutoLock lock(m_Lock);
	string name = pEndPoint->getName();
	map<string,IEndPointPtr>::iterator i = m_mapServantInfo_ip.find(name);
	if ( i != m_mapServantInfo_ip.end() )
	{
		if ( (*i).second != pEndPoint )
		{
			throwDAException(-3,"registry same endpoint name but endpoint pointer not equal");
		}
	}
	else
		m_mapServantInfo_ip.insert( std::make_pair<string, IEndPointPtr>( name, pEndPoint ) );

	map<uint32,ENDPOINT_LIST>::iterator j = m_mapServantInfo_name.find(nServantNamePrefix);
	if ( j != m_mapServantInfo_name.end() )
	{
		for ( ENDPOINT_LIST::iterator i = j->second.begin(); i != j->second.end() ; i ++ )
		{
			if ( (*i) == pEndPoint )
				return ;
		}
	}
	m_mapServantInfo_name[nServantNamePrefix].push_back(pEndPoint);
}

/*

		Class	CDAServer

*/
bool CDAServer::getSessionID(uint32& nSessionID)
{
    //todo �����ظ�
    /*
    uint32 nbeginsessonid = g_nSessionID;
    while ( true )
    {
    //����ʹ��0
    if ( 0 == ++g_nSessionID )
    g_nSessionID = 1;
    //�ж�sessionid�Ƿ�����
    map<uint32,LPCONNECTION_DATA>::iterator i = m_Sessions.find(g_nSessionID);
    if ( i == m_Sessions.end() )
    break;
    if ( nbeginsessonid == g_nSessionID )
    {
    return false;
    }
    }
    */
    //����ʹ��0��0������connector���͵�endpointʹ��
    //	if ( 0 == ++g_nSessionID )
    //		g_nSessionID = 1;
    nSessionID = ++g_nSessionID;
    return true;
}

CDAServer::CDAServer() : m_Thread(this, &CDAServer::doBusiness)
{
	m_nSvrPortArgc  = 0;
	m_bIsRun = false;
	m_bIsInited = false;
	m_nSerialNo = 0;
	m_pReactor = NULL;
	m_nTimeoutCount = 0;
}

CDAServer::~CDAServer()
{
    for ( map<string,ENDPOINT_INFO>::iterator i = this->m_EndPoints.begin() ; i != m_EndPoints.end() ; i ++ )
    {
        delete (*i).second.pConnData;
    }
    m_EndPoints.clear();
}

bool CDAServer::init()
{
	g_Log->info("DAServer|initializing server");
	if ( m_bIsInited )
		throwDAException(-1,"DAServer|already init succesed");
	m_pProfile = getFrameWorkProf();

	if ( !this->loadProfile() )
		throwDAException(-1,"DAServer|load profile error!");

	//�̱߳��ر�����Դ��ʼ��
	if ( !m_Currents.initialize() )
		throwDAException(-1,"DAServer|Failed to init thread local object current");

	// ����ҵ�����߳�(ֻ����һ���߳�)
	if ( !m_BusinessLock.initialize() )
		throwDAException(-1,"DAServer|Failed to init business lock");
	if ( !m_Thread.init() )
		throwDAException(-1,"DAServer|Failed to init thread");
	if ( !m_TimeSvr.init() )
		throwDAException(-1,"DAServer|Failed to init time service");
	if ( !m_TimeoutLock.initialize() )
		throwDAException(-1,"DAServer|Failed to init timeout lock");
	m_bIsInited = true;
	// ��ʼ�����м�������(Acceptor) �� ����Զ�̷���(Connector)
	return initService();
}

bool CDAServer::initService()
{
	log_info("DAServer|initializing net service...");
	//ע�����Э��
	CProtocolFactory::regProtocol("zwprotocol",new CZWProtocol());

    // ���� TCPSocketDriver ��ע�ᵽ CDriverManager
    if( !CDriverManager::registry(new CTCPSocketDriver()) )
    {
        log_fatal("DAServer|registry TCPSocketDriver error!");
        return false;
    }

	// ���� TCPSocketDriver ��ע�ᵽ CDriverManager
	if( !CDriverManager::registryByClassName(new CUDPSocketDriver()) )
	{
		log_fatal("DAServer|registry UDPSocketDriver error!");
		return false;
	}

	// ���� LinuxNetActorDriver ��ע�ᵽ CDriverManager
	if( !CDriverManager::registry(CNetActorDriverFactory::createNetActorDriver(CNetActorDriverFactory::NETACTOR_REACTOR)) )
	{
		log_fatal("DAServer|registry NetActorDriver error!");
		return false;
	}

	// ��ȡ NetActor �� SocketDriver
	INetActorDriver* pdriver = static_cast<INetActorDriver*>( CDriverManager::getDriver("NetActorDriver") );
	if( NULL == pdriver )
	{
		log_fatal("DAServer|get NetActorDriver error!");
		return false;
	}

	m_pReactor = static_cast<IReactor*> ( pdriver->getNetActor().get());
	if( NULL == m_pReactor )
	{
		log_fatal("DAServer|get NetActor error!");
		return false;
	}

	ISocketDriver* psocketdriver = static_cast<ISocketDriver*>(CDriverManager::getDriver("SocketDriver"));
	if( NULL == psocketdriver )
	{
		log_fatal("DAServer|get SocketDriver error!");
		return false;
	}

	// ��ʼ�� NetActor
	if( 0 != m_pReactor->init() )
	{
		log_fatal("DAServer|init Reactor error!");
		return false;
	}

    std::vector<uint16>::iterator iter_t;
	int servicecount = m_pProfile->getInteger("control service settings", "control service count", 0);
	log_info("DAServer|Create Control Service Begin...|Count = %d", servicecount);
	//�������ƶ˿ڷ���
	for( int i = 1; i <= servicecount; i++)
	{
		char sectionname[255] = {0};
		sprintf(sectionname, "control service%02d", i);

		bool useencrypt = ( strcasecmp(m_pProfile->getString(sectionname,"use encrypt","false"),"true") == 0 ) ? true : false;
		
		std::string servicehost = m_pProfile->getString(sectionname, "servicehost", "0.0.0.0");
		std::vector<std::string> vecservicehost;
		splitString(servicehost, ",", vecservicehost);

		for(std::vector<std::string>::iterator iter_host = vecservicehost.begin(); iter_host != vecservicehost.end(); iter_host++)
		{
			if ( ! isIP(iter_host->c_str()) )
		    {
				string host = this->m_pProfile->getString("machine settings",iter_host->c_str(),"");
				if ( host == "" )
	            {
		            log_fatal("DAServer|no machine[%s]'s setting",iter_host->c_str());
			        return false;
				}
	            else
		            *iter_host = host;
			}

			std::string strport = m_pProfile->getString(sectionname, "serviceport", 0);
            std::vector<std::string> vecport;
			splitString(strport, ",", vecport);
			for(std::vector<std::string>::iterator iter_port = vecport.begin(); iter_port != vecport.end(); iter_port++)
			{
                uint16 finalport = 0;
                if( !isPort( iter_port->c_str() ) )
                {
                    string machine_port = m_pProfile->getString("machine settings", iter_port->c_str(), "");
                    if ( machine_port == "" )
                    {
                        log_fatal("DAServer|no machine[%s]'s setting", machine_port.c_str());
                        return false;
                    }

                    if ( !isPort( machine_port.c_str() ) )
                    {
                        log_fatal("DAServer|invalid machine[%s]'s setting", machine_port.c_str());
                        return false;
                    }

                    finalport = atoi( machine_port.c_str() );
                }
                else
                {
                    finalport = atoi( iter_port->c_str() );
                }

				log_info("DAServer|Create Servant: %s:%d", iter_host->c_str(), finalport);
				IEndPointPtr pendpoint = addEndpoint(iter_host->c_str(),finalport,IEndPoint::LOCAL_ENDPOINT,IEndPoint::SOCKTYPE_TCP,
					CProtocolFactory::createProtocol(m_pProfile->getString(sectionname,"protocol","zwprotocol")),(useencrypt?(new CZWEncryptor()):NULL));
				if ( pendpoint == NULL )
					return false;

				//�����˿�����ִ�е�servantǰ׺
				std::string servantlist = m_pProfile->getString(sectionname, "servantlist", "");//�˿ڴ����servant�б�ʮ�����Ʊ�ʾ�����ŷָ�
				if( servantlist != "" )
				{
					std::vector<uint16> servantprefixlist;
					splitString(servantlist, ",", servantprefixlist, 16);
					m_mapServantPrefixs.insert( make_pair(finalport, servantprefixlist) );
				}
			}//end of for finalport
		}//end of for host
	}//end of for servicecount

	int servantcount = m_pProfile->getInteger("servant service settings", "servant service count", 0);
    log_info("DAServer|Create Servant Server Begin..|Count = %d", servantcount);

	char sectionname[255] = {0};
    // ���� Servant ����
    for( int i = 1; i <= servantcount; i++)
    {
		sprintf(sectionname, "servant service%02d", i);

		bool useencrypt = ( strcasecmp(m_pProfile->getString(sectionname,"use encrypt","false"),"true") == 0 ) ? true : false;
		int netbuffer = m_pProfile->getInteger(sectionname,"netbuffer size",512);
		
		std::string servanthost = m_pProfile->getString(sectionname, "servanthost", "0.0.0.0");
		std::vector<std::string> vecservanthost;
		splitString(servanthost, ",", vecservanthost);

		for(std::vector<std::string>::iterator iter_host = vecservanthost.begin(); iter_host != vecservanthost.end(); iter_host++)
		{
	        if ( ! isIP(iter_host->c_str()) )
		    {
			    string hostip = m_pProfile->getString("machine settings",iter_host->c_str(),"");
				if ( hostip != "" )
					*iter_host = hostip;
				if ( !isIP(iter_host->c_str()) )
	            {
					//����������ת������
					struct hostent* hosts = gethostbyname(hostip.c_str());
					if ( hosts == NULL )
					{
						g_Log->fatal("DAServer|no machine[%s]'s setting",iter_host->c_str());
						return false;
					}
					else
					{
						hostip = inet_ntoa(*(struct in_addr *)hosts->h_addr);
						log_info("DAServer|convert hostname[%s] to ip[%s] success",iter_host->c_str(),hostip.c_str());
						*iter_host = hostip;
					}
				}
			}

			std::string strport = m_pProfile->getString(sectionname, "servantport", "");
            std::vector<std::string> vecport;
			splitString(strport, ",", vecport);
			for(std::vector<std::string>::iterator iter_port = vecport.begin(); iter_port != vecport.end(); iter_port++)
			{
                uint16 finalport = 0;
                if( !isPort( iter_port->c_str() ) )
                {
                    string machine_port = m_pProfile->getString("machine settings", iter_port->c_str(), "");
                    if ( machine_port == "" )
                    {
                        g_Log->fatal("DAServer|no machine[%s]'s setting", machine_port.c_str());
                        return false;
                    }

                    if ( !isPort( machine_port.c_str() ) )
                    {
                        g_Log->fatal("DAServer|invalid machine[%s]'s setting", machine_port.c_str());
                        return false;
                    }

                    finalport = atoi( machine_port.c_str() );
                }
                else
                {
                    finalport = atoi( iter_port->c_str() );
                }

				log_info("DAServer|Create Servant: %s:%d", iter_host->c_str(), finalport);
				IEndPointPtr pendpoint = addEndpoint(iter_host->c_str(),finalport,IEndPoint::LOCAL_ENDPOINT,IEndPoint::SOCKTYPE_TCP,
					CProtocolFactory::createProtocol(m_pProfile->getString(sectionname,"protocol","zwprotocol")),(useencrypt?new CZWEncryptor():NULL),netbuffer);
				if ( pendpoint == NULL )
					return false;

				//�����˿�����ִ�е�servantǰ׺
				std::string servantlist = m_pProfile->getString(sectionname, "servantlist", "");//�˿ڴ����servant�б�ʮ�����Ʊ�ʾ�����ŷָ�
				if( servantlist != "" )
				{
					std::vector<uint16> servantprefixlist;
					splitString(servantlist, ",", servantprefixlist, 16);
					m_mapServantPrefixs.insert( make_pair(finalport, servantprefixlist) );
				}
			}//end of for port
		}//end of for host
    }//end of for servantCount
    log_info("DAServer|Create Servant Server End");

    // ��ȡԶ�̷�����������
    int svrcount = m_pProfile->getInteger("server settings", "server count", 0);
    log_info("DAServer|Connect to Remote Server Begin Count = %d", svrcount);
    for( int i = 1; i <= svrcount; i++ )
    {
        snprintf(sectionname, sizeof(sectionname), "server%02d", i);

        std::string host  = m_pProfile->getString(sectionname, "serverhost", ""); // Զ�̷��������ַ��ֻ������һ��
        std::string port = m_pProfile->getString(sectionname, "serverport", "");  // Զ�̷�������˿ڣ�ֻ������һ��
        std::string servantlist = m_pProfile->getString (sectionname, "servantlist", "");// Զ��Servant�����б�, ʮ�����Ʊ�ʾ, ���ŷָ�
        bool useencrypt = ( strcasecmp(m_pProfile->getString(sectionname,"use encrypt","false"),"true") == 0  ) ? true : false; //�ж��Ƿ���ü���
		int netbuffer = m_pProfile->getInteger(sectionname,"netbuffer size",512);

		if ( !isIP(host.c_str()) )
		{
			string hostip = m_pProfile->getString("machine settings",host.c_str(),"");
			if ( hostip != "" )
				host = hostip;
			//machine config��Ҳ�п�����������
			if ( !isIP(host.c_str()) )
			{
				struct hostent* hosts = gethostbyname(host.c_str());
				if ( hosts == NULL )
				{
					g_Log->fatal("DAServer|no machine[%s]'s setting",host.c_str());
					return false;
				}
				else
				{
					hostip = inet_ntoa(*(struct in_addr *)hosts->h_addr);
					log_info("DAServer|convert hostname[%s] to ip[%s] success",host.c_str(),hostip.c_str());
					host = hostip;
				}
			}
		}

        uint16 finalport = 0;
        if( !isPort( port.c_str() ) )
        {
            string machine_port = m_pProfile->getString("machine settings", port.c_str(), "");
            if ( machine_port == "" )
            {
                log_fatal("DAServer|no machine[%s]'s setting", machine_port.c_str());
                return false;
            }

            if ( !isPort( machine_port.c_str() ) )
            {
                log_fatal("DAServer|invalid machine[%s]'s setting", machine_port.c_str());
                return false;
            }

            finalport = atoi( machine_port.c_str() );
        }
        else
        {
            finalport = atoi( port.c_str() );
        }
		IEndPoint::EENDPOINT_SOCKTYPE socktype = IEndPoint::SOCKTYPE_TCP;
		if ( strcasecmp("udp",m_pProfile->getString(sectionname,"socktype","tcp")) == 0 )
			socktype = IEndPoint::SOCKTYPE_UDP;
		log_info("DAServer|ServerHost = %s | ServerPort = %d | ServantList = %s | socktype = %s", host.c_str(), finalport, servantlist.c_str(),m_pProfile->getString(sectionname,"socktype","tcp"));
		IEndPointPtr pendpoint = addEndpoint(host.c_str(),finalport,IEndPoint::REMOTE_ENDPOINT,socktype,
				CProtocolFactory::createProtocol(m_pProfile->getString(sectionname,"protocol","zwprotocol")),
				(useencrypt?new CZWEncryptor():NULL),netbuffer);
		if ( pendpoint == NULL )
			return false;

        std::vector<uint32> servantprefixlist;
        splitString(servantlist, ",", servantprefixlist, 16);
		for( std::vector<uint32>::iterator iter_v = servantprefixlist.begin(); iter_v != servantprefixlist.end(); iter_v++ )
		{
			//ע��servant
			m_EndPointManager.registry(pendpoint,(*iter_v));
		}
    }
	log_info("DAServer|init service success.");
	return true;
}

bool CDAServer::loadProfile()
{
	m_nIdleTimeOut = atoi(m_pProfile->getString("app settings","idle timeout","20")) * 1000 ;     //�������ӳ�ʱ�ж�
	m_nServantExecuteTime = m_pProfile->getInteger("app settings", "servant execute time", 500); 	//servantִ��ʱ��
	log_info("DAServer|Idle Timeout is %d secs",m_nIdleTimeOut);
	return true;
}

bool CDAServer::uninit()
{
    return CThreadFactory::finalize();
}

void CDAServer::doBusiness(void* pParam)
{
	srand((unsigned int)time(NULL));

	while ( m_bIsRun )
    {
		BUSINESS_LIST listtemp;
		{
			CAutoLock lock(m_BusinessLock);
			if ( m_lstBusiness.empty() )
			{
				m_BusinessLock.wait();
				continue;
			}
			listtemp = m_lstBusiness;
			m_lstBusiness.clear();
		}

		BUSINESS_LIST::iterator iter_t;
		for( iter_t = listtemp.begin(); iter_t != listtemp.end(); iter_t++ )
		{
			CCurrentPtr pcurrent = (*iter_t).get()->getCurrent();
			CURRENT_STACK& currents = getCurrentStack();
			currents.push( pcurrent ); 
#ifndef SERVANTDEBUG
            try
            {
#endif
				uint64 begintime = CDateTime::current();
				(*iter_t).get()->execute();
				uint64 endtime = CDateTime::current();
				if ( endtime - begintime > m_nServantExecuteTime )
				{
					log_warning("running too slow|servant|0x%08x|esplased %dms", (*iter_t).get()->getServantName(), endtime - begintime);
				}
#ifndef SERVANTDEBUG
            }
            catch (CCException& e)
            {
				log_error("catch Exception[%s] while executing [%08X]business callstack : \n %s",e.what(),iter_t->get()->getServantName(),e.printStackTrace());
            }
            catch (exception& e)
            {
                log_error("catch Exception[%s] while executing [%08X]business ",e.what(),iter_t->get()->getServantName());
            }
#endif
			currents.pop();
        }
	}
	//end of while
	//�Ż��������ط�ȥ
	if ( this->m_pServerHandler != NULL )
	{
		m_pServerHandler->onServerStop();
	}
	//ִ��onStop�е��õ�һЩҵ��
	{
		m_BusinessLock.lock();
		if( m_lstBusiness.empty() )
			m_BusinessLock.wait( 2000 );
		BUSINESS_LIST listtemp;
		listtemp = m_lstBusiness;
		m_lstBusiness.clear();
		m_BusinessLock.unlock();

		BUSINESS_LIST::iterator iter_t;
		for( iter_t = listtemp.begin(); iter_t != listtemp.end(); iter_t++ )
		{
			try
			{
				CCurrentPtr pcurrent = (*iter_t).get()->getCurrent();
				CURRENT_STACK& currents = getCurrentStack();
				currents.push( pcurrent ); 
				uint64 begintime = CDateTime::current();
				(*iter_t).get()->execute();
				uint64 endtime = CDateTime::current();
				if ( endtime - begintime > m_nServantExecuteTime )
				{
					g_Log->warning("running too slow|servant|0x%08x|esplased %dms", (*iter_t).get()->getServantName(), endtime - begintime);
				}
				currents.pop();
			}
			catch (exception& e)
			{
				g_Log->error("catch Exception[%s] while executing business",e.what());
			}
		}
	}
	
	//ֹͣ����ģ�飬���ٴ�����������շ�
	if( 0 != m_pReactor->stop() )
	{
		g_Log->fatal("Failed to call Reactor stop!");
	}
	this->destoryCurrentStack();
}

void CDAServer::run()
{
	//������ʱ����
	ITimeTaskPtr ptimeouttask = new TTimeTask<CDAServer>(*this, &CDAServer::onTimeout,NULL);
	m_TimeSvr.addTask(ptimeouttask, 0, TICK_INTERVAL);
	m_bIsRun = true;
	//����ҵ���߳�
	m_Thread.start();
	//������ʱ�߳�
	m_TimeSvr.start();
	//��������container
	for ( int i = 0 ; i < m_ServantContainers.size() ; i ++ )
	{
		m_ServantContainers[i]->start();
	}
	if ( 0 != m_pReactor->run() )
	{
		throwDAException(-999, "Failed to call Reactor Run!");
	}
	this->destoryCurrentStack();
}

void CDAServer::registry(uint32 nServantNamePrefix,uint32 nMask,IEndPointHandlerPtr pEndPointHandler)
{
    HANDLE_INFO info;
    info.nMask = nMask;
    info.pEndPointHandler = pEndPointHandler;
    m_ServantHandlers.insert(map<uint32,HANDLE_INFO>::value_type(nServantNamePrefix,info));
}

void CDAServer::addServant(uint32 Name, IServantPtr Servant,int ContainerID)
{
	g_Log->info("DAServer|loading.. servant[0x%08X] container[%d]", Name,ContainerID);
	if ( ContainerID == 0 )
	{
		std::pair<std::map< uint32, SERVANTINFO >::iterator, bool> iter_ret;
		SERVANTINFO servantinfo;
		servantinfo.pServant = Servant;
		iter_ret = m_mapServants.insert( std::map< uint32, SERVANTINFO >::value_type(Name, servantinfo) );
		if ( !iter_ret.second )
			throwDAException(-2, "Failed to add Servant[%08X]! already Exists",Name);
		return;
	}

	if ( ContainerID > m_ServantContainers.size() )
		throwDAException(-2, "ContainerID[%d] Error",ContainerID);
	m_ServantContainers[ContainerID-1]->addServant(Name,Servant);
}

IServantPtr CDAServer::getServant(uint32 Name,int ContainerID/* = 0*/)
{
	if ( ContainerID == 0 )
	{
		std::map< uint32, SERVANTINFO >::iterator iter_ret = m_mapServants.find(Name);
		if (iter_ret != m_mapServants.end())
		{
			return iter_ret->second.pServant;
		}
	}

	if ( ContainerID > m_ServantContainers.size() )
	{
		return m_ServantContainers[ContainerID-1]->getServant(Name);
	}
	return NULL;
}

int CDAServer::addServantContainer(IServantContainerPtr pServantContainer)
{
	m_ServantContainers.push_back(pServantContainer);
	if ( ! pServantContainer->init(this) )
	{
		log_warning("DAServer|init container error");
		return -1;
	}
	DEBUG_LOG("DAServer|AddContainer|Container[%X] success , id=%d",pServantContainer.get(),m_ServantContainers.size());
	return m_ServantContainers.size();
}

typedef struct _t_timetask
{
    ITimeTaskPtr pTimeTask;
    CCurrentPtr pCurrent;
    void* pParam;
}TIMERTASK,*LPTIMERTASK;

TASKHANDLE CDAServer::addTimeTask(ITimeTaskPtr TimerTask, int32 nDelay, int32 nInterval, void *Param, int16 nRepeatTimes)
{
	LPTIMERTASK ptask = new TIMERTASK();
	ptask->pParam = Param;
	ptask->pTimeTask = TimerTask;
	CURRENT_STACK& currents = getCurrentStack();
	if ( !currents.empty() )
	{
		ptask->pCurrent = currents.top();
	}
	else
		ptask->pCurrent = NULL;
	ITimeTaskPtr pexectask = new TTimeTask<CDAServer>(*this, &CDAServer::executeTimeTask,&CDAServer::finishTimeout);
	return m_TimeSvr.addTask(pexectask, nDelay, nInterval, nRepeatTimes,ptask);
}

void CDAServer::send(uint32 SessionID, uint32 nServantName, ISerialPtr pMessage,IResponseHandlerPtr pResponseHandler,uint32 nTimeOut, void* Param)
{
	CONNECTION_DATA conndata;
	{
		CAutoLock lock(this->m_ConnDataLock);
		std::map<uint32, LPCONNECTION_DATA>::iterator i = m_Sessions.find(SessionID);
		if ( i == m_Sessions.end() )
		{
			g_Log->warning("DAServer|send by sessionid error -- session not found");
			return;
		}
		if ( (*i).second->pEndPoint == NULL )
		{
			g_Log->warning("DAServer|send by sessionid error -- EndPoint is NULL");
			return;
		}

		conndata = *(i->second);
	}

	int32 requestid = makeRequestID();
	if ( pResponseHandler != NULL )
	{
		//�����Ҫ����Ӧ��
		RESPONSE_DATA ResponseData;
		ResponseData.nServantName    = nServantName;
		ResponseData.pResponseHandle = pResponseHandler;
		ResponseData.pParam          = Param;
		if ( nTimeOut == 0 )
			nTimeOut = DEFAULT_TIMEOUT;
		ResponseData.nTimeOut = nTimeOut;
		CURRENT_STACK& currents = getCurrentStack();
		if ( !currents.empty() )
		{
			ResponseData.pCurrent = currents.top();
		}
		else
			ResponseData.pCurrent = NULL;
		DEBUG_LOG("DAServer|set response handler for request[%d]",requestid);
		m_TimeoutLock.lock();
		m_mapResponsees.insert( std::make_pair<uint32, RESPONSE_DATA>(requestid, ResponseData) );
		m_TimeoutLock.unlock();
	}
	CBytesBufferPtr body = CBytesBufferPtr::createInstance(__FUNCTION__);
	pMessage->serial(*body.get());
	conndata.pEndPoint->send(requestid,nServantName,*body.get());
}

void CDAServer::broadcast(uint32 nServantName, ISerialPtr Message)
{
	//Ϊ���÷���˳��������򣬷ŵ���ǰҵ��ĺ���ִ��
	BUSINESS_LIST tmplist;
	std::map<uint32, LPCONNECTION_DATA>::iterator iter_t;
	{
		CAutoLock lock(m_ConnDataLock);
		//�㲥�����пͻ���
		for( iter_t = this->m_Sessions.begin(); iter_t != m_Sessions.end(); iter_t++ )
		{
			IConnectionPtr pconn = iter_t->second->pConnection;
			CSessionPtr psession = iter_t->second->pSession;
			IEndPointPtr pendpoint = iter_t->second->pEndPoint;

			CCurrentPtr pcurrent = CCurrentPtr::createInstance();
			pcurrent->setServantName(nServantName);
			pcurrent->setRequestID(makeRequestID());
			pcurrent->setDAServer(this);
			pcurrent->setEndPoint(pendpoint.get());
			pcurrent->setSession(psession.get());

			CSendBusinessPtr pBusiness = CSendBusinessPtr::createInstance();
			pBusiness->setSerial( Message );
			pBusiness->setCurrent( pcurrent );
			pBusiness->setDAServer( this );

			tmplist.push_back( pBusiness.get() );
		}
	}
	this->putBusinessList(tmplist);
}

void CDAServer::putBusiness(IBussinessPtr pBusiness)
{
    m_BusinessLock.lock();
    m_lstBusiness.push_back( pBusiness );
    m_BusinessLock.notifyAll();
    m_BusinessLock.unlock();    
}

void CDAServer::putBusinessList(BUSINESS_LIST& BusinessList)
{
	m_BusinessLock.lock();
	for ( BUSINESS_LIST::iterator i = BusinessList.begin() ; i != BusinessList.end() ; i ++ )
		m_lstBusiness.push_back( (*i) );
	m_BusinessLock.notifyAll();
	m_BusinessLock.unlock();    
}

void CDAServer::stop()
{
	if ( !this->m_bIsInited )
		return ;
	//ֹͣҵ���߳�
	m_bIsRun = false;
	m_BusinessLock.lock();
	m_BusinessLock.notifyAll();
	m_BusinessLock.unlock();
	m_TimeSvr.stop();
	for ( int i = 0 ; i < m_ServantContainers.size() ; i ++ )
        {
                m_ServantContainers[i]->stop();
        }
}

CCurrentPtr CDAServer::createCurrent(uint32 nSessionID,uint32 nRequestID,uint32 nServantName)
{
	IEndPointPtr pendpoint;
	CSessionPtr psession;

	if ( nSessionID != 0 )
	{
		//���ָ����sessionid����ʹ��session����ֵ
		std::map<uint32, LPCONNECTION_DATA>::iterator iter = this->m_Sessions.find(nSessionID);
		if ( iter != m_Sessions.end() )
		{
			pendpoint = iter->second->pEndPoint;
			psession = iter->second->pSession;
		}
	}

	if ( pendpoint == NULL && psession == NULL )
	{
		CURRENT_STACK& currents = getCurrentStack();
		if ( !currents.empty() )
		{
			CCurrentPtr pcurrent = currents.top();
			if ( pcurrent != NULL )
			{
				pendpoint = pcurrent->getEndPoint();
				psession = pcurrent->getSession();
			}
		}
	}

	//����һ�����ص�CCurrent��Ϣ
	CCurrentPtr pcurrent = CCurrentPtr::createInstance();
	pcurrent->setDAServer(this);
	pcurrent->setRequestID(nRequestID);
	pcurrent->setServantName(nServantName);
	pcurrent->setEndPoint(pendpoint.get());
	pcurrent->setSession(psession.get());
	return pcurrent;
}

//�ڷ�ҵ���߳��У����ܵ���callservant
void CDAServer::callServant(uint32 nServantName, IRequestPtr pRequest, IResponseHandlerPtr pResponseHandle, uint32 nTimeOut, void* Param, const char* pszEndPoint,uint32 nSessionID)
{
#ifdef DEBUG_FRAMEWORK
	if ( ( ( 0x00050000 & nServantName ) == 0x00050000 ) &&  ( pszEndPoint == NULL ) )
		assert( false );
#endif

	int32 requestid = makeRequestID();
	DEBUG_LOG("DAServer|callservant|ServantName=0x%08X,serialno = 0x%08X,nTimeOut=%d,Param=0x%08X", nServantName, requestid, nTimeOut, Param);

	if ( pResponseHandle != NULL )
	{
		//�����Ҫ����Ӧ��
		RESPONSE_DATA responsedata;
		responsedata.nServantName    = nServantName;
		responsedata.pResponseHandle = pResponseHandle;
		responsedata.pParam          = Param;
		if ( nTimeOut == 0 )
			nTimeOut = DEFAULT_TIMEOUT;
		responsedata.nTimeOut = nTimeOut;
		CURRENT_STACK& currents = getCurrentStack();
		if ( !currents.empty() )
		{
			responsedata.pCurrent = currents.top();
		}
		else
			responsedata.pCurrent = NULL;
		DEBUG_LOG("DAServer|set response handler for request[%d]",requestid);
		m_TimeoutLock.lock();
		m_mapResponsees.insert( std::make_pair<uint32, RESPONSE_DATA>(requestid, responsedata) );
		m_TimeoutLock.unlock();
	}

	if ( pszEndPoint != NULL )
	{
		// ָ�� EndPoint ʱ����ֱ����ָ���ķ���������
		DEBUG_LOG("DAServer|callServant|Sent directly to the specified endpoint ");
		// ���� strEndPoint ����Endpoint, strEndPoint �ĸ�ʽΪ IP:Port
		IEndPointPtr pEndPoint = m_EndPointManager.find( pszEndPoint );
		if( NULL == pEndPoint )
		{
			//û�д�endpoint
			g_Log->error("DAServer|callServant|EndPoint is Null when call directly endpoint[%s]", pszEndPoint);
			return ;
		}
		CBytesBufferPtr body = CBytesBufferPtr::createInstance(__FUNCTION__);
		pRequest->serial(*body.get());
		pEndPoint->send(requestid,nServantName,*body.get());
		return ;
	}

	// û��ָ�� EndPoint ʱ������ұ��ص� Servant
	std::map<uint32, SERVANTINFO>::iterator iter = m_mapServants.find(nServantName);
	if ( iter != m_mapServants.end() )
	{
		DEBUG_LOG("DAServer|callServant|Call local endpoint ");
		//����һ�����ص�CCurrent��Ϣ
		CCurrentPtr pcurrent = this->createCurrent(nSessionID,requestid,nServantName);
		pcurrent->setLocalCall();
		//����ҵ���̴߳�ִ��
		CRequestTaskPtr prequest = CRequestTaskPtr::createInstance();
		prequest->setCurrent(pcurrent);
		prequest->setDAServer(this);
		prequest->setRequest(pRequest);
		prequest->setServant(iter->second.pServant);
		prequest->setServantName(nServantName);
		this->putBusiness(prequest.get());
		return;
	}

	//����container
	for ( int i = 0 ; i < m_ServantContainers.size() ; i ++ )
	{
		if ( m_ServantContainers[i]->hasServant(nServantName) )
		{
			DEBUG_LOG("DAServer|callServant|Call Servant Container[%d]",i+1);
			//����һ�����ص�CCurrent��Ϣ
			CCurrentPtr pcurrent = this->createCurrent(nSessionID,requestid,nServantName);
			pcurrent->setLocalCall();
			m_ServantContainers[i]->callServant(nServantName,pRequest,pcurrent);
			return;
		}
	}

	// ���ز����ҵ�ָ���� Servant (For �ֲ�ʽ����)
	DEBUG_LOG("DAServer|callServant|Call Remote endpoint ");
	IEndPointPtr pEndPoint = m_EndPointManager.find( nServantName );

	if ( NULL != pEndPoint )
	{
		CBytesBufferPtr body = CBytesBufferPtr::createInstance(__FUNCTION__);
		pRequest->serial(*body.get());
		pEndPoint->send(requestid,nServantName,*body.get());
	}
	else
	{
		g_Log->error("DAServer|callServant|EndPoint is Null when call remote servant! ");
		return ;
	}
}

bool CDAServer::response(uint32 nRequestID, IResponsePtr pResponse)
{
    std::map<uint32, RESPONSE_DATA>::iterator iter_find;
    CAutoLock lock(m_TimeoutLock);
	DEBUG_LOG("DAServer|get response for request[%d]",nRequestID);
    iter_find = m_mapResponsees.find( nRequestID );
    if( iter_find != m_mapResponsees.end() )
    {
		CResponseTaskPtr pbusiness = CResponseTaskPtr::createInstance();
		pbusiness->setParam(iter_find->second.pResponseHandle,pResponse,iter_find->second.pCurrent,iter_find->second.pParam,this);
		pbusiness->setServantName(iter_find->second.nServantName);
		m_BusinessLock.lock();
		m_lstBusiness.push_back( pbusiness.get() );
		m_BusinessLock.notifyAll();
		m_BusinessLock.unlock();
		m_mapResponsees.erase( iter_find );
		return true;
    }
	return false;
}

/*
void CDAServer::broadcastServant(uint32 Name, IRequestPtr pRequest, IResponseHandlerPtr Response)
{
//todo
}
*/

void CDAServer::onRead(CReadResult& Result, void* Param)
{
	DEBUG_BLOG(Result.c_str(),Result.getSize(),"read");

	int32 retcode = Result.getCode();
	if ( retcode != 0  )
	{
		log_error("DAServer|onRead|error[%d] connection[%s] handle[%d]",retcode,Result.getConnection()->getRemoteAddress()->getString(),Result.getConnection()->getHandle());
		return ;
	}

	assert( Result.getAcceptor() == NULL );
	//assert( Result.getConnector() == NULL );
	assert( Result.getConnection() != NULL );
	assert( Result.getConnection()->getOutputStream() != NULL );
	assert( Result.getConnection()->getInputStream() != NULL );

	//Param is LPCONNECTION_DATA, new CONNECTION_DATA in OnAccept()/onOpen()
	assert( Param != NULL ); 

	LPCONNECTION_DATA pconndata = (LPCONNECTION_DATA)Param;
	uint32 serialno,servantname;
	pconndata->pEndPoint->writeRecvData(Result.c_str(),Result.getSize());

	CBytesBufferPtr pbuffer;
	CBytesInputStream stream(NULL,0);
	stream.setStackObject();
	int packcount = 0;
	IProtocolPtr protocol = pconndata->pEndPoint->getProtocol();
	while ( NULL != ( pbuffer = pconndata->pEndPoint->next(serialno,servantname) ) )
	{
		packcount ++;
		//���һֱû�յ��Ϸ��İ���Ҳ�����ȡ������
		pconndata->nLastReadTime = CDateTime().getTotalMills();
		if ( protocol->isHeartBeat(servantname) )
		{
			continue;
		}
		//�������ݻ���
		stream.reset(pbuffer->c_str(),pbuffer->getDataSize());
		if ( protocol->isResponse( servantname ) )
		{
			//����ظ�
			processResponse(serialno, protocol->getServantName(servantname), pconndata, stream);
		} 
		else
		{
			//��������
			processRequest(serialno, protocol->getServantName(servantname), pconndata, stream);
		}
	}
}

void CDAServer::onWrite(CWriteResult& Result, void* Param)
{
    assert( Result.getAcceptor() == NULL );
    assert( Result.getConnector() == NULL );
    assert( Result.getConnection() != NULL );
}

void CDAServer::onOpen(CNetResult& Result, void* Param)
{
	//-----for debug begin-------//
	assert( Result.getAcceptor() == NULL );
	assert( Result.getConnector() != NULL ); 
	assert( Result.getConnection() == NULL );
	//-----for debug end-------//

	IConnectionPtr pConnection = Result.getConnector()->getConnection();
	log_info("Open Connection to Remote[%s]", pConnection->getRemoteAddress()->getString());
	IEndPointPtr endpoint = static_cast<IEndPoint*>(Param);
	pConnection->setBufSize(endpoint->getNetBufferSize()*1024);
	string name = endpoint->getName();
	int32 nErrCode = Result.getCode();
	if ( nErrCode != 0 )
	{
		log_error("Open Connection Error[%d] ", nErrCode);
		{
			CAutoLock lock(m_Lock);
			//����Ƿ���endpoint�����������������У��Ѿ�ɾ����endpoint
			map<string,ENDPOINT_INFO>::iterator i = m_EndPoints.find(name);
			if ( i == m_EndPoints.end() )
			{
				log_error("open remote connection error , but can't find endpoint");
				return;
			}
		}

		m_TimeoutLock.lock();
		this->m_ReConnList.push_back(endpoint);
		m_TimeoutLock.unlock();

		return ;
	}

	{
		ENDPOINT_INFO endpointinfo;
		{
			CAutoLock lock(m_Lock);
			//����Ƿ���endpoint��handler
			map<string,ENDPOINT_INFO>::iterator i = m_EndPoints.find(name);
			if ( i == m_EndPoints.end() )
			{
				log_error("open remote connection success , but can't find endpoint");
				return;
			}
			endpointinfo = (*i).second;
		}

		INetActorDriver::getNetActor()->registry(pConnection,this,NETHANDLER_MASK_READ | NETHANDLER_MASK_WRITE | NETHANDLER_MASK_CLOSE,endpointinfo.pConnData);
		endpointinfo.pConnData->pConnection = pConnection;
		endpointinfo.pConnData->pEndPoint->setConnection(pConnection);
		//���ӳɹ��������µ�session������ɵ�����
		(endpointinfo.pConnData->pSession = CSessionPtr::createInstance())->setSessionID(0);
		if ( endpointinfo.HandleInfo.nMask & IEndPointHandler::MASK_OPEN )
		{
			CCurrentPtr pcurrent = CCurrentPtr::createInstance();
			pcurrent->setDAServer(this);
			pcurrent->setRequestID(makeRequestID());
			pcurrent->setServantName(0);
			pcurrent->setEndPoint( endpoint.get() );
			pcurrent->setSession( endpointinfo.pConnData->pSession.get());

			CURRENT_STACK& currents = getCurrentStack();
			currents.push( pcurrent ); 

			//todo �޸�Ϊ����ҵ���߳��д���
			endpointinfo.HandleInfo.pEndPointHandler->onOpen(Result.getCode(),endpoint.get());

			currents.pop();
		}
	}

	//����Ƿ�����������
	for ( map<uint32,HANDLE_INFO>::iterator j = m_ServantHandlers.begin() ; j != m_ServantHandlers.end() ; j ++ )
	{
		//�Ƿ������endpoint��ӵ�е�servantname��Ҫ������
		if ( this->m_EndPointManager.isEndPointPrefix((*j).first,endpoint) )
		{
			if ( (*j).second.nMask & IEndPointHandler::MASK_OPEN )
			{
				//֪ͨopen�¼�
				(*j).second.pEndPointHandler->onOpen(Result.getCode(),endpoint);
			}
		}
	}
}

IEndPointPtr CDAServer::addEndpoint(const string& Address,uint16 nPort,IEndPoint::EENDPOINT_TYPE EndPointType,IEndPoint::EENDPOINT_SOCKTYPE SockType,
									IProtocolPtr pProtocol,IEncryptorPtr pEncryptor,int nNetBuffSize,uint32 nMask,IEndPointHandlerPtr pEndPointHandler)
{
	ISocketDriver* psocketdriver = NULL;
	switch ( SockType )
	{
		case IEndPoint::SOCKTYPE_UDP :
			psocketdriver = static_cast<ISocketDriver*>(CDriverManager::getDriverByClassName("CUDPSocketDriver"));
			break;
		default :
			psocketdriver = static_cast<ISocketDriver*>(CDriverManager::getDriver("SocketDriver"));
			break;
	}

	if ( NULL == psocketdriver )
	{
		log_fatal("DAServer|get SocketDriver error!");
		return NULL;
	}

	IEndPointPtr ep = new IEndPoint();
	if ( EndPointType == IEndPoint::REMOTE_ENDPOINT )
	{
		IConnectorPtr pconnector = psocketdriver->createConnector(new CIP4Address(Address.c_str(), nPort));
		if ( NULL == pconnector )
		{
			log_fatal("DAServer|Failed to init Service Host Connector %s:%d!", Address.c_str(), nPort);
			return NULL;
		}
		ep->setType(IEndPoint::REMOTE_ENDPOINT);
		ep->setConnector( pconnector );
	}
	else if ( EndPointType == IEndPoint::LOCAL_ENDPOINT )
	{
        IAcceptorPtr pacceptor = psocketdriver->createAcceptor(new CIP4Address(Address.c_str(), nPort));
		if ( NULL == pacceptor )
        {
		    log_error("DAServer|Failed to init Servant Host %s:%d!", Address.c_str(), nPort);
            return false;
		}
		int retval = pacceptor->listen();
		if ( 0 != retval )
		{
			ThrowException<CCException>("EndPoint[%s] Listen Error[%d--%s]",pacceptor->getListenAddress()->getString(),retval,getErrMsg(retval));
		}
		ep->setType(IEndPoint::LOCAL_ENDPOINT);
		ep->setAcceptor( pacceptor );
	}
	else
	{
		log_fatal("DAServer|addEndPoint|endpoint type[%d] error!",EndPointType);
		return NULL;
	}

	ep->setProtocol(pProtocol);
	ep->setEncryptor(pEncryptor);
	ep->setNetBufferSize(nNetBuffSize);

	//����EndPoint
	ENDPOINT_INFO info;
	info.HandleInfo.nMask = 0;
	if ( nMask != 0 && pEndPointHandler != NULL )
	{
		info.HandleInfo.pEndPointHandler = pEndPointHandler;
		info.HandleInfo.nMask = nMask;
	}
	if ( ep->getProtocol() == NULL )
		throwDAException(-1,"EndPoint's Protocol cannot be NULL");
	//�����;����ʧ�ܣ��˳�����ʱ�����Զ�ɾ��CONNECTION_DATA�Ķ���
	CAutoPtr<CONNECTION_DATA> pconndata = new CONNECTION_DATA();
	info.pConnData = pconndata.get();
	info.pConnData->pConnection = NULL;
	info.pConnData->pSession = CSessionPtr::createInstance();
	info.pConnData->pEndPoint = ep;
	//connector��sessionͳһΪ0��һ����Բ���ʹ��
	info.pConnData->pSession->setSessionID( 0 );
	{
		//����m_EndPoints��
		CAutoLock lock(m_Lock);
		std::map<string,ENDPOINT_INFO>::iterator iter = m_EndPoints.find(ep->getName());
		if ( iter == m_EndPoints.end() )
		{
			m_EndPoints.insert(std::make_pair<string,ENDPOINT_INFO>(ep->getName(),info));
			//���뵽map�ɹ���ɾ��
			pconndata.release();
		}
		else
		{
			log_fatal("DAServer|addEndPoint|endpoint name[%s] already exists!",ep->getName());
			return NULL;
		}
	}

	//���õ����紦����
	if ( EndPointType == IEndPoint::REMOTE_ENDPOINT )
	{
		IConnectorPtr pconnector = ep->getConnector();
		int retval = m_pReactor->addConnector( pconnector, this, ep );
		if ( 0 != retval )
		{
			this->removeEndpoint(Address,nPort,EndPointType);
			ThrowException<CCException>("EndPoint[%s] Connector regstry to netactor Error[%d--%s]",pconnector->getConnectAddress()->getString(),retval,getErrMsg(retval));
		}
		ep->setConnector( pconnector );
	}
	else if ( EndPointType == IEndPoint::LOCAL_ENDPOINT )
	{
		IAcceptorPtr pacceptor = ep->getAcceptor();
		int retval = m_pReactor->addAcceptor( pacceptor, this, ep);
		if ( 0 != retval )
		{
			this->removeEndpoint(Address,nPort,EndPointType);
			ThrowException<CCException>("EndPoint[%s] regstry to netactor Error[%d--%s]",pacceptor->getListenAddress()->getString(),retval,getErrMsg(retval));
		}
		ep->setAcceptor( pacceptor );
	}
	return ep;
}


IEndPointPtr CDAServer::removeEndpoint(const string& Address,uint16 nPort,IEndPoint::EENDPOINT_TYPE EndPointType)
{
	string name = IEndPoint::getName(Address,nPort,EndPointType);
	ENDPOINT_INFO endpointinfo;
	{
		CAutoLock lock(m_Lock);
		std::map<string,ENDPOINT_INFO>::iterator iter = m_EndPoints.find(name);
		if ( iter == m_EndPoints.end() )
		{
			log_warning("DAServer|removeEndpoint|endpoint name[%s] is not exists!",name.c_str());
			return NULL;
		}
		endpointinfo = iter->second;
		m_EndPoints.erase(iter);
	}
	//ɾ��CONNECTION_DATA�Ķ�����onClose�����л���У��������账��
	switch ( endpointinfo.pConnData->pEndPoint->getType() )
	{
		case IEndPoint::LOCAL_ENDPOINT :
			m_pReactor->removeAcceptor(endpointinfo.pConnData->pEndPoint->getAcceptor());
			break;
		case IEndPoint::REMOTE_ENDPOINT :
			m_pReactor->removeConnector(endpointinfo.pConnData->pEndPoint->getConnector());
			break;
		default :
			return endpointinfo.pConnData->pEndPoint;

	}
	return endpointinfo.pConnData->pEndPoint;
}

/*
void CDAServer::addEndpoint(IEndPointPtr pEndPoint,uint32 nMask,IEndPointHandlerPtr pEndPointHandler)
{
    //����EndPoint
    ENDPOINT_INFO info;
    info.HandleInfo.nMask = 0;
    if ( nMask != 0 && pEndPointHandler != NULL )
    {
        info.HandleInfo.pEndPointHandler = pEndPointHandler;
        info.HandleInfo.nMask = nMask;
    }
    IAcceptorPtr  pAcceptor  = pEndPoint->getAcceptor();
    IConnectorPtr pConnector = pEndPoint->getConnector();
    if ( pEndPoint->getProtocol() == NULL )
        throwDAException(-1,"EndPoint's Protocol cannot be NULL");
    info.pConnData = new CONNECTION_DATA();
    info.pConnData->pConnection = NULL;
    info.pConnData->pSession = CSessionPtr::createInstance();
    info.pConnData->pEndPoint = pEndPoint;
    //connector��sessionͳһΪ0��һ����Բ���ʹ��
    info.pConnData->pSession->setSessionID( 0 );

	if ( NULL != pAcceptor )
	{
		int retval = pAcceptor->listen();
		if ( 0 != retval )
		{
			ThrowException<CCException>("EndPoint[%s] Listen Error[%d--%s]",pAcceptor->getListenAddress()->getString(),retval,getErrMsg(retval));
		}
		retval = m_pReactor->addAcceptor( pAcceptor, this, pEndPoint );
		if ( 0 != retval )
		{
			ThrowException<CCException>("EndPoint[%s] regstry to netactor Error[%d--%s]",pAcceptor->getListenAddress()->getString(),retval,getErrMsg(retval));
		}
		pEndPoint->setAcceptor(pAcceptor);
	}

	if ( NULL != pConnector )
	{
		int retval = m_pReactor->addConnector( pConnector, this, pEndPoint );
		if ( 0 != retval )
		{
			ThrowException<CCException>("EndPoint[%s] Connector regstry to netactor Error[%d--%s]",pAcceptor->getListenAddress()->getString(),retval,getErrMsg(retval));
		}
		pEndPoint->setConnector( pConnector );
	}
	m_EndPoints[pEndPoint->getName()] = info;
}
*/

void CDAServer::onClose(CNetResult& Result, void* Param)
{
    log_info("DAServer|onClose|connection[%s] closing.. handle[%d]",Result.getConnection()->getRemoteAddress()->getString(),Result.getConnection()->getHandle());
    //��HOOK
    CAutoPtr<CONNECTION_DATA> pConnData = (LPCONNECTION_DATA)Param;
    //connector
    //����Զ�����ӶϿ�
    if ( Result.getConnector() != NULL )
    {
        string name = pConnData->pEndPoint->getName();
		{
			CAutoLock lock(m_Lock);
			map<string,ENDPOINT_INFO>::iterator i = this->m_EndPoints.find(name);
			if ( i == m_EndPoints.end() )
			{
				g_Log->error("DAServer|onClose|can't find endpoint");
				return ;
			}
			if ( (*i).second.HandleInfo.nMask & IEndPointHandler::MASK_CLOSE )
			{
				//֪ͨClose�¼�
				(*i).second.HandleInfo.pEndPointHandler->onClose(Result.getCode(),pConnData->pEndPoint);
			}
		}
        //����Ƿ�����������
        for ( map<uint32,HANDLE_INFO>::iterator j = m_ServantHandlers.begin() ; j != m_ServantHandlers.end() ; j ++ )
        {
            //�Ƿ������endpoint��ӵ�е�servantname��Ҫ������
            if ( this->m_EndPointManager.isEndPointPrefix((*j).first,pConnData->pEndPoint) )
            {
                if ( (*j).second.nMask & IEndPointHandler::MASK_CLOSE )
                {
                    //֪ͨclose�¼�
                    (*j).second.pEndPointHandler->onClose(Result.getCode(),pConnData->pEndPoint);
                }
            }
        }
        g_Log->info("DAServer|onClose|remote Connection Closed ");
        m_TimeoutLock.lock();
        this->m_ReConnList.push_back(pConnData->pEndPoint);
        m_TimeoutLock.unlock();
        //��ɾ������
        pConnData.release();
    }
    else
    {
        //��Client��������ɾ��
        CAutoLock lock(m_ConnDataLock);
        //��session������ɾ��
        map<uint32,LPCONNECTION_DATA>::iterator iter_sess = m_Sessions.find(pConnData->pSession->getSessionID());
        if ( iter_sess == m_Sessions.end() )
        {
            log_warning("DAServer|onClose|can't find session");
        }
        else
        {
            //֪ͨsession onClose�¼�
            for ( SESSIONHANDLER_LIST::iterator iter_hand = m_SessionHandlers.begin() ; iter_hand != m_SessionHandlers.end() ; iter_hand ++ )
            {
                //��ҵ���߳�ִ��
                this->putBusiness(new CSessionHandleBusiness((*iter_sess).second->pSession,(*iter_hand).get()));
            }
            m_Sessions.erase(iter_sess);
        }
    }
}

void CDAServer::onAccept(CNetResult& Result, void* Param)
{
	int32 retcode = Result.getCode();
	if ( retcode != 0 )
	{
		g_Log->error("DAServer|onAccept|error[%d]",retcode);
		return ;
	}
	// ���� Accept ����, Connector �� Connection ��ԶΪNULL
	IConnectionPtr pConnection = Result.getAcceptor()->accept(); 
	if ( NULL == pConnection )
	{
		g_Log->error("DAServer|onAccept|Failed to call accept! ErrCode = %d ", errno);
		return ;
	}

	INetActorPtr pNetActor = INetActorDriver::getNetActor();
	assert( pNetActor != NULL );
	IEndPointPtr pserverdata = (IEndPoint*)Param;
	//���û�������С
	retcode = pConnection->setBufSize(pserverdata->getNetBufferSize()*1024);
	g_Log->info("DAServer|onAccept|Client[%s] connect to listen[%s] success handle[%d] %d",pConnection->getRemoteAddress()->getString(),pConnection->getHostAddress()->getString(),pConnection->getHandle(),retcode);

	CAutoPtr<CONNECTION_DATA> pconndata = new CONNECTION_DATA();
	assert( NULL != pconndata );

	pconndata->pSession = CSessionPtr::createInstance();
	pconndata->pEndPoint = new IEndPoint();
	pconndata->pEndPoint->setType(IEndPoint::REMOTE_ENDPOINT);
	pconndata->pEndPoint->setAcceptor(Result.getAcceptor());
	pconndata->pEndPoint->setProtocol(pserverdata->getProtocol());
	pconndata->pEndPoint->setEncryptor(pserverdata->getEncryptor());
	pconndata->pEndPoint->setNetBufferSize(pserverdata->getNetBufferSize());
	pconndata->nLastReadTime = CDateTime().getTotalMills();
	uint32 nsessionid = 0;
	if ( !this->getSessionID(nsessionid) )
	{
		//id�þ�,�����ṩ����
		log_fatal("DAServer|onAccept|error sessionid all used");
		return ;
	}
	pconndata->pSession->setSessionID( nsessionid );

	int nRet = pNetActor->registry(pConnection, this, NETHANDLER_MASK_READ | NETHANDLER_MASK_WRITE | NETHANDLER_MASK_CLOSE, pconndata.get());
	if( nRet == 0 )
	{
		CAutoLock lock(m_ConnDataLock);
		pconndata->pConnection = pConnection;
		pconndata->pEndPoint->setConnection(pConnection);
		//����session�б�
		this->m_Sessions.insert( std::make_pair<uint32,LPCONNECTION_DATA>(pconndata->pSession->getSessionID(),pconndata.get()));
		g_Log->info("DAServer|onAccept|Client Session:%d ", pconndata->pSession->getSessionID());
		//�ѳɹ������б����ͷ�����ָ�������
		pconndata.release();
	}
	else
	{
		g_Log->info("DAServer|onAccept|Registry Client Connect Errno:%d ", nRet);
	}
}

int32 CDAServer::makeRequestID()
{
    return m_nSerialNo++;
}

bool CDAServer::isLegalServantPrefix(uint16 port, uint16 servantprefix)
{
	std::map<uint16, std::vector<uint16> >::iterator iter = m_mapServantPrefixs.find(port);
	if (iter != m_mapServantPrefixs.end() )
	{
		for (std::vector<uint16>::iterator vIter = iter->second.begin(); vIter != iter->second.end(); ++vIter)
		{
			if ( servantprefix == *vIter )
				return true;
		}
		return false;
	}
	else
		return true;
}

bool CDAServer::respondNotFoundServant(uint32 nSerialNo, uint32 nServantName,IEndPointPtr pEndPoint)
{
	//todo �޸�ΪЭ����ص�Ӧ��
	CBytesBufferPtr repbody = CBytesBufferPtr::createInstance(__FUNCTION__);
	// �ظ�ͷ
	writeuint32(*repbody.get(), SERVANT_NOTFOUND );
	pEndPoint->response(nSerialNo,nServantName,*repbody.get());
	return true;
}

bool CDAServer::processRequest(uint32 nSerialNo, uint32 nServantName, LPCONNECTION_DATA pConnData, IInputStream& pBody)
{
    DEBUG_LOG("DAServer|processRequest|serialno = 0x%08X, servantname = 0x%08X", nSerialNo, nServantName);

	if ( pConnData->pEndPoint->getAcceptor() != NULL )
	{
		if( !isLegalServantPrefix(pConnData->pEndPoint->getAcceptor()->getListenAddress()->getPort(), GET_SERVANT_PREFIX(nServantName)) )
		{
			g_Log->warning("DAServer|Warning : Port:%d doesn't process ServantPrefix:0x%04x servant_name(0x%08X)", pConnData->pEndPoint->getAcceptor()->getListenAddress()->getPort(), GET_SERVANT_PREFIX(nServantName), nServantName);
			return false;
		}
	}

    // ���� Servant Name ���Ҷ�Ӧ�� Servant
    std::map< uint32, SERVANTINFO >::iterator iter_find;
    iter_find = m_mapServants.find( nServantName );
    if ( iter_find != m_mapServants.end() )
    {
		//���Ӵ��������ڶ�ʱ�߳�����ԭ���Ǽ���ͬ��
		SERVANTINFO& servantinfo = iter_find->second;
		servantinfo.StatsticsInfo.incExecute();
		IServantPtr pservant = iter_find->second.pServant;

		//����request
		IRequestPtr prequest = pservant->createRequest();
		if ( NULL == prequest )
		{
			log_warning("DAServer|processRequest|pServant->createRequest() is NULL, nServantName:0x%08X ", nServantName);
			return false;
		}
		prequest->unserial(pBody);
		CCurrentPtr pcurrent = CCurrentPtr::createInstance();
		pcurrent->setDAServer(this);
		pcurrent->setRequestID(nSerialNo);
		pcurrent->setServantName(nServantName);
		pcurrent->setEndPoint(pConnData->pEndPoint.get());
		pcurrent->setSession(pConnData->pSession.get());
		//����ҵ����У���ҵ���߳�ִ��
		CRequestTaskPtr pBussiness =  CRequestTaskPtr::createInstance();
		pBussiness->setServant( pservant );
		pBussiness->setRequest( prequest );
		pBussiness->setCurrent( pcurrent );
		pBussiness->setDAServer( this );
		pBussiness->setServantName(nServantName);
		putBusiness(pBussiness.get());
		return true;
    }

	for ( int i = 0 ; i < m_ServantContainers.size() ; i ++ )
	{
		if ( m_ServantContainers[i]->hasServant(nServantName) )
		{
			DEBUG_LOG("DAServer|callServant|Call Servant Container[%d]",i+1);
			IRequestPtr prequest = m_ServantContainers[i]->createRequest(nServantName);
			if ( NULL == prequest )
			{
				log_warning("DAServer|processRequest|pServant->createRequest() is NULL, nServantName:0x%08X ", nServantName);
				return false;
			}
			prequest->unserial(pBody);
			//����һ�����ص�CCurrent��Ϣ
			CCurrentPtr pcurrent = CCurrentPtr::createInstance();
			pcurrent->setDAServer(this);
			pcurrent->setRequestID(nSerialNo);
			pcurrent->setServantName(nServantName);
			pcurrent->setEndPoint(pConnData->pEndPoint.get());
			pcurrent->setSession(pConnData->pSession.get());
			m_ServantContainers[i]->callServant(nServantName,prequest,pcurrent);
			return true;
		}
	}
	
    //�Ҳ���Servant
    log_error("DAServer|Process Request: Error Can't find Servantname:0x%08X from[%s] ", nServantName ,pConnData->pConnection->getRemoteAddress()->getString() );
    respondNotFoundServant(nSerialNo,nServantName,pConnData->pEndPoint);
    return false;
}

bool CDAServer::processResponse(uint32 nSerialNo, uint32 nServantName, LPCONNECTION_DATA pConnData, IInputStream& pBody)
{
	DEBUG_LOG("DAServer|Process Response: serialno = 0x%08X, servantname = 0x%08X", nSerialNo, nServantName);

    // ���� nSerialNo ������ RESPONSE_DATA, ����ҵ������֮ǰ������������������
    std::map<uint32, RESPONSE_DATA>::iterator iter_find;
    CAutoLock lock(m_TimeoutLock);
    iter_find = m_mapResponsees.find( nSerialNo );
    if( iter_find != m_mapResponsees.end() )
    {
		IResponsePtr pResponse = iter_find->second.pResponseHandle->createResponse();
		if( pResponse != NULL )
		{
			pResponse->unserial( pBody );
		}
		else
		{
			log_error("DAServer|pResponse == NULL");
			return false;
		}
		CResponseTaskPtr pbusiness = CResponseTaskPtr::createInstance();
		pbusiness->setParam(iter_find->second.pResponseHandle,pResponse,iter_find->second.pCurrent,iter_find->second.pParam,this);
		pbusiness->setServantName(nServantName);
		m_BusinessLock.lock();
		m_lstBusiness.push_back( pbusiness.get() );
		m_BusinessLock.notifyAll();
		m_BusinessLock.unlock();
		m_mapResponsees.erase( iter_find );
    }
    else
    {
        // �����ҵ���Ӧ SerialNo �� RESPONSE_DATA, ���ظ�
        log_error("DAServer|Process Response: Can't find the nSerialNo:0x%08X nServantName:0x%08X", nSerialNo,nServantName );
    }
    return true;
}

std::string CDAServer::getSetting(const std::string& strSessionName, const std::string& strKeyName, const std::string& strDefValue)
{
    return m_pProfile->getString(strSessionName.c_str(), strKeyName.c_str(), strDefValue.c_str());
}

uint32 CDAServer::getSettingUint32(const std::string& strSessionName, const std::string& strKeyName, const std::string& strDefValue)
{
    return atoi( m_pProfile->getString(strSessionName.c_str(), strKeyName.c_str(), strDefValue.c_str()) );
}

/*
std::string CDAServer::getSetting(const std::string& strSessionName, const std::string& strKeyName, const std::string& strDefValue)
{
    return m_MachineProfile.getString(strSessionName.c_str(), strKeyName.c_str(), strDefValue.c_str());
}
*/

int CDAServer::executeTimeTask( void *pParam )
{
	LPTIMERTASK ptask = (LPTIMERTASK)pParam;
	CTimerBusinessPtr pbusi = CTimerBusinessPtr::createInstance();
	pbusi->setDAServer(this);
	pbusi->setTimeTask(ptask->pTimeTask);
	pbusi->setParam(ptask->pParam);
	pbusi->setCurrent(ptask->pCurrent);
	this->putBusiness(pbusi.get());
	return 0;
}

void printBytesBufferInfo(CBytesBuffer* pBuffer,const char* pInfo)
{
	uint32 bufsize = pBuffer->getBufferSize();
	log_info("DAServer|mem statistics(bytesbuffer)|%X|%.2f%s|%s",pBuffer,GETSIZE(bufsize),GETUNITDESC(bufsize),pInfo);
}

int CDAServer::onTimeout(void *pParam)
{
	//ÿ25msִ��һ�Σ���ʱ����Ӿ�ȷ
	CDateTime::tickCount();
	m_nTimeoutCount ++ ;
	if ( m_nTimeoutCount % SECOND_COUNT == 0 )
	{
		//ÿ����еļ��

		//1.����Զ��endpoint������
		ENDPOINT_LIST tmplist;
		this->m_TimeoutLock.lock();
		tmplist = m_ReConnList;
		m_ReConnList.clear();
		this->m_TimeoutLock.unlock();
		for ( ENDPOINT_LIST::iterator i = tmplist.begin() ; i != tmplist.end() ; i ++ )
		{
			INetActorDriver::getNetActor()->addConnector((*i).get()->getConnector(),this,(*i));
		}
		tmplist.clear();

		//2.����Ƿ���response��ʱ--��ҵ���̴߳���
		CTimerBusinessPtr pbusi = CTimerBusinessPtr::createInstance();
		pbusi->setDAServer(this);
		pbusi->setTimeTask(new TTimeTask<CDAServer>(*this, &CDAServer::checkResponseTimeout,NULL));
		pbusi->setParam(NULL);
		pbusi->setCurrent(NULL);
		this->putBusiness(pbusi.get());
	}
	if ( m_nTimeoutCount % (10*SECOND_COUNT) == 0 )
	{
#ifndef DASERVER_NO_TIMEOUT
		//ÿ10����һ��
		uint64 now = CDateTime().getTotalMills();
		{
			//���ͻ�������
			CAutoLock lock(m_ConnDataLock);
			//���ͻ��������Ƿ�ʱ
			for( std::map<uint32, LPCONNECTION_DATA>::iterator iter_t = m_Sessions.begin(); iter_t != m_Sessions.end(); iter_t++ )
			{
				//����Ƿ�ʱ
				if ( ( now - (*iter_t).second->nLastReadTime ) >= m_nIdleTimeOut )
				{
					//�Ͽ�����
					iter_t->second->pConnection->close();
				}
			}
		}
#endif
		//����������
		{
			CAutoLock lock(m_Lock);
			for ( map<string,ENDPOINT_INFO>::iterator i = this->m_EndPoints.begin() ; i != m_EndPoints.end() ; i ++ )
			{
				IEndPointPtr endpoint = i->second.pConnData->pEndPoint;
				if ( endpoint->getType() == IEndPoint::LOCAL_ENDPOINT )
					continue;
				endpoint->sendHeartBeat();
			}
		}
		if ( isFrameWorkProfUpdated() )
		{
			this->onProfileUpdate();
		}
	}

	if ( this->m_nTimeoutCount % (10 * MINUTE_COUNT) == 0 )
	{
		//ÿ10���Ӵ�ӡһ��servant��ִ�����
		char buffer1[128];
		char buffer2[128];
		log_info("DAServer|statistics|begin");
		log_info("DAServer|statistics|servant name|exectimes(per sec.)|peek exectimes(per sec)|total exectimes|exectimes(per sec.)(network)|peek exectimes(per sec)(network)|total exectimes(network)");
		for ( std::map<uint32, SERVANTINFO>::iterator i = m_mapServants.begin() ; i != m_mapServants.end() ; i ++ )
		{
			//�����������Ϣ
			i->second.StatsticsInfo.serial(buffer1,sizeof(buffer1));
			//servant����ʵִ����Ϣ
			i->second.pServant->getStatsticsInfo().serial(buffer2,sizeof(buffer2));
			log_info("DAServer|statistics|%08X|%s|%s",i->first,buffer2,buffer1);
		}
		log_info("DAServer|statistics|end");
		log_info("DAServer|mem statistics|begin");
		//��ӡ���е�bytesbuffer��Ϣ
		CObjectPool<CBytesBuffer>::share_pool::getInstance()->foreach(printBytesBufferInfo);
		log_info("DAServer|mem statistics|end");
		//��ӡ��ǰ���̵��ڴ���Ϣ
		MEM_USAGE memusage;
		CMemUtil::getMemInfo(memusage);
		log_info("DAServer|process statistics|begin");
		log_info("DAServer|process statistics|mem|%s",CMemUtil::formatMemInfo(memusage));
		log_info("DAServer|process statistics|end");
	}
	return 0;
}

int CDAServer::finishTimeout(void *pParam)
{
    LPTIMERTASK ptask = (LPTIMERTASK)pParam;
    delete ptask;
    return 0;
}

int CDAServer::checkResponseTimeout(void *pParam)
{
	CDateTime current;
	CAutoLock lock(m_TimeoutLock);
	CURRENT_STACK& currents = getCurrentStack();
	for ( map<uint32, RESPONSE_DATA>::iterator i = m_mapResponsees.begin(); i != m_mapResponsees.end() ; )
	{
		//���ظ���ʱ
		if ( (current - (*i).second.BeginTime).getTotalSeconds() >= (int)(*i).second.nTimeOut )
		{
			DEBUG_LOG("DAServer|response timeout for request[%d]",(*i).first);
			currents.push((*i).second.pCurrent);

#ifndef SERVANTDEBUG
			try
			{
#endif
				(*i).second.pResponseHandle->onTimeout((*i).second.pParam);
#ifndef SERVANTDEBUG
			}
			catch (CCException& e)
			{
				g_Log->error("catch Exception[%s] while executing timeout callstack : \n %s",e.what(),e.printStackTrace());
			}
			catch (exception& e)
			{
				g_Log->error("catch Exception[%s] while executing timeout",e.what());
			}
#endif
			currents.pop();
			//��ʱɾ��
			m_mapResponsees.erase(i++);
		}
		else
			i++;
	}
	return 0;
}

void CDAServer::addSessionHandler(ISessionHandlerPtr pSessionHandler)
{
    this->m_SessionHandlers.push_back(pSessionHandler);
}

void CDAServer::closeSession(uint32 nSessionID)
{
    CAutoLock lock(this->m_ConnDataLock);
    std::map<uint32, LPCONNECTION_DATA>::iterator i = m_Sessions.find(nSessionID);
    if ( i == m_Sessions.end() )
    {
		g_Log->error("DAServer|closeSession|session(%d) not found",nSessionID);
		return;
    }
    if ( (*i).second->pEndPoint == NULL )
    {
		g_Log->error("DAServer|closeSession|session(%d) EndPoint is NULL",nSessionID);
		return;
    }
    CONNECTION_DATA conndata = *(i->second);
    conndata.pConnection->close();
}

void CDAServer::setServerHandler(IDAServerHandlerPtr pHandler)
{
    this->m_pServerHandler = pHandler;
}

CProfile& CDAServer::getProfile()
{
    return *m_pProfile;
}

CURRENT_STACK& CDAServer::getCurrentStack()
{
	CURRENT_STACK* pcurrent = (CURRENT_STACK*)m_Currents.getValue();
	if ( pcurrent == NULL )
	{
		pcurrent = new CURRENT_STACK();
		m_Currents.setValue(pcurrent);
	}
	return *pcurrent;
}

void CDAServer::destoryCurrentStack()
{
	CURRENT_STACK* pcurrent = (CURRENT_STACK*)m_Currents.getValue();
	if ( pcurrent != NULL )
	{
		delete pcurrent;
	}
	m_Currents.setValue(NULL);
}

void CDAServer::onProfileUpdate()
{
	reloadFrameWorkProf();
	this->loadProfile();
	if ( m_pServerHandler != NULL )
	{
		CDAServerBusinessPtr pbuss = CDAServerBusinessPtr::createInstance();
		pbuss->setParams(this,NULL,this->m_pServerHandler,DAOPER_ONPROFUPDATE);
		this->putBusiness(pbuss.get());
	}
}

bool CDAServer::addCallEndpoint(const string& Address,uint16 nPort,const string& ServantList,IProtocolPtr pProtocol,bool isUseEncrypt,int nNetBuffSize)
{
	if ( pProtocol == NULL )
		pProtocol = new CZWProtocol();
	IEncryptorPtr pencryptor = NULL;
	if ( isUseEncrypt )
	{
		pencryptor = new CZWEncryptor();
	}
	IEndPointPtr pendpoint = this->addEndpoint(Address,nPort,IEndPoint::REMOTE_ENDPOINT,IEndPoint::SOCKTYPE_TCP,
		pProtocol,pencryptor,nNetBuffSize);
	if ( pendpoint == NULL )
		return false;
	std::vector<uint16> servantprefixlist;
	splitString(ServantList, ",", servantprefixlist, 16);
	for( std::vector<uint16>::iterator iter_v = servantprefixlist.begin(); iter_v != servantprefixlist.end(); iter_v++ )
	{
		//ע��servant
		m_EndPointManager.registry(pendpoint,(*iter_v));
	}
	return true;
}

bool CDAServer::removeCallEndpoint(const string& Address,uint16 nPort)
{
	IEndPointPtr pendpoint = this->removeEndpoint(Address,nPort,IEndPoint::REMOTE_ENDPOINT);
	if ( NULL == pendpoint )
		return false;
	m_EndPointManager.unregistry(pendpoint->getName());
	return true;
}
//��ȡsessionID
bool CDAServer::getSessionIDByEndpointName(string strName,uint32& nSessionID)
{
	ENDPOINT_INFO endpointinfo;
	{
		CAutoLock lock(m_Lock);
		std::map<string,ENDPOINT_INFO>::iterator iter = m_EndPoints.find(strName);
		if ( iter == m_EndPoints.end() )
		{
			log_warning("DAServer|removeEndpoint|endpoint name[%s] is not exists!",strName.c_str());
			return false;
		}
		endpointinfo = iter->second;
		nSessionID = endpointinfo.pConnData->pSession->getSessionID();
	}
	return true;
}
