#include "../public.h"
#include "../thread/thread.h"
#include "../lock//baselock.h"
#include "dbpublic.h"
#include "sqlexception.h"
#include "poolconnautoptr.h"
#include "connection.h"
#include "connectionmanager.h"
#include "poolconnection.h"
#include "connectionpool.h"

/*
#include <public.h>
#include <util/thread.h>
#include <util/baselock.h>
#include <db/dbpublic.h>
#include <db/sqlexception.h>
#include <db/poolconnautoptr.h>
#include <db/connection.h>
#include <db/connectionmanager.h>
#include <db/poolconnection.h>
#include <db/connectionpool.h>
*/
//
CConnectionPool::CConnectionPool(const char* pURL, int nMaxConnections,const char* pUsr,const char* pUsrPwd)
:m_nMaxConnections(nMaxConnections),m_nConnectionCount(0),m_nIdleConnectionCount(0),
m_Url(pURL),m_Usr(pUsr),m_UsrPwd(pUsrPwd),m_nLastConnID(0)
{	
	m_pConnections = new CPoolConnection*[nMaxConnections];
	for ( unsigned short i = 0 ; i < nMaxConnections ; i ++ )
	{
		m_pConnections[i] = NULL;
	}
	m_nMaxConnections = nMaxConnections;
	m_nConnectionCount = 0;
	m_nIdleConnectionCount= 0;
	this->m_BaseLock = new CBaseLock();
	m_BaseLock->initialize();
}

//
CConnectionPool::~CConnectionPool()
{
	for ( unsigned short i = 0 ; i < this->m_nMaxConnections; i ++ )
	{
		if ( m_pConnections[i] != NULL )
			delete m_pConnections[i];
	}
	delete[] m_pConnections;

	if (this->m_BaseLock != NULL)
	{
		delete m_BaseLock;
	}
}

//
void CConnectionPool::putConnection(CPoolConnection* pConn)
{
	CAutoLock lock(*this->m_BaseLock);

	if ( ( pConn->m_pConnectionPool == this )&&(pConn->m_bIsIdle==false) )
	{
		pConn->m_bIsIdle = true;		
		m_nIdleConnectionCount ++;
		pConn->m_nOwnThreadId = 0;
		this->m_BaseLock->notifyAll();	
	}	
}

const CPoolConnection* CConnectionPool::getIdleConnection()
{
	CPoolConnection* pPoolConn = NULL;
	if ( m_nIdleConnectionCount > 0 )
	{
		/*
		for ( unsigned short i = 0 ; i < this->m_nConnectionCount ; i ++ )
		{
			pPoolConn = m_pConnections[i];
			if ((pPoolConn==NULL)||( !pPoolConn->m_bIsIdle ))
				continue;
			pPoolConn->m_bIsIdle = false;
			m_nIdleConnectionCount--;			
			break;
		}
		*/

		int count=0;

		int i=m_nLastConnID+1;
		if (i==m_nConnectionCount)
		{
			i=0;
		}		

		pPoolConn = m_pConnections[i];

		//��������ȡ�������ӣ���֤���ӳ��е����ӱ�����ʹ��
		while((pPoolConn==NULL)||(!pPoolConn->m_bIsIdle))
		{	
			//printf("getIdleConn m_nLastConnID=%d i=%d\n",m_nLastConnID,i);

			i++;
			count++;

			//���ﵽ��������±꣬��0��ʼ�ҿ�������
			if (i==m_nConnectionCount)
			{
				i=0;
			}

			//���Ȿ�������bug���µ���ѭ�������е����Ӷ�������󣬻���û�ҵ���������������ѭ��
			if (count>m_nConnectionCount)
			{
				break;
			}
			
			pPoolConn = m_pConnections[i];
		}

		//��ȡ�����е�����
		if (pPoolConn!=NULL)
		{
			pPoolConn->m_bIsIdle = false;
			m_nIdleConnectionCount--;
			m_nLastConnID = i;
			
			//printf("%s m_nLastConnID=%d\n","getIdleConn",m_nLastConnID);
		}
	}

	//����Ϊ�Զ�commit
	if ( pPoolConn != NULL )
	{
		if( !pPoolConn->isClosed() )
			pPoolConn->setAutoCommit(true);
	}

	return pPoolConn;
}

//
CPoolConnAutoPtr<CPoolConnection> CConnectionPool::getConnection(long nWaitMillis)
{
	long tmp =0;
	bool bHasIdleConn=false;
	CPoolConnection* pPoolConn = NULL;
	
	{
		CAutoLock lock(*this->m_BaseLock);		
		pPoolConn = (CPoolConnection*)getIdleConnection();
		if ( pPoolConn == NULL )
		{
			//��������ȴ�
			if ((this->m_nConnectionCount >= this->m_nMaxConnections) && nWaitMillis)
			{
				//printf("wait for conn\n");
				while ( (pPoolConn == NULL)||(0<nWaitMillis)||(tmp < nWaitMillis) )
				{					
					if (nWaitMillis>0)
					{
						//printf("%s ThreadId=%d tmp=%d nWaitMillis=%d\n","nWaitMillis>0",CThread::getCurrentThreadId(),tmp,nWaitMillis);
						//nWaitMillis>0,��ʱ���ȴ���ÿ����ȡһ�ο�������
						tmp += 500;
						this->m_BaseLock->wait(500);
					}
					else
					{
						//nWaitMillis<0,���޵ȴ���ÿ��ȡһ�ο�������
						this->m_BaseLock->wait(1000);
					}

					pPoolConn = (CPoolConnection*)getIdleConnection();

					if (pPoolConn != NULL)
					{
						//printf("wait but got idle conn\n");
						bHasIdleConn = true;
						break;
						//CPoolConnAutoPtr<CPoolConnection> poolconn(pPoolConn);
						//this->m_BaseLock->unlock();	
						//return poolconn;
					}

					if ((tmp >= nWaitMillis)&&(nWaitMillis>0))
					{
						break;
					}
				}

				if ((tmp >= nWaitMillis)&&(!bHasIdleConn))
				{
					//this->m_BaseLock->unlock();	
					throw CSQLException(ERROR_DATABASE_NOTCONNECT,"timeout when getConnection by CConnectionPool!");
				}
			}
			//�����Ӳ��ȴ�
			else 
			{
				//���ӳ�δ��������һ���µ�����
				if ( this->m_nConnectionCount < this->m_nMaxConnections )
				{
					try
					{
						CAutoPtr<CConnection> ptr = CConnectionManager::getConnection(this->m_Url.c_str(),this->m_Usr.c_str(),this->m_UsrPwd.c_str());
						CConnection* pConn = ptr.release();					
						pPoolConn = new CPoolConnection(pConn,this);
						m_pConnections[m_nConnectionCount] = pPoolConn;
						m_nConnectionCount ++;
						pPoolConn->m_bIsIdle = false;				
					}
					catch (CSQLException& e)
					{
						//this->m_BaseLock->unlock();
						string errmsg = e.what();
						errmsg = "getConnection by CConnectionPool error! " + errmsg;
						throw CSQLException(ERROR_DATABASE_NOTCONNECT,errmsg);
					}
					
					//printf("getConn m_nConnectionCount=%d\n",m_nConnectionCount);
					CPoolConnAutoPtr<CPoolConnection> poolconn(pPoolConn);
					//this->m_BaseLock->unlock();	
					return poolconn;					
				}
				else
				{
					//���ӳ�����				
					throw CSQLException(ERROR_DATABASE_POOLFULL,"getConnection by CConnectionPool error! no connection available!");
				}
			}
		}
		//�п�������
		else
		{
			bHasIdleConn = true;		
		}
	}

	if (bHasIdleConn)
	{
		//������ݿ������Ƿ��ڻ״̬�����ǵĻ���Ҫ�����������������ɹ�����Ҫ��ȡ�����ӵķ�����
		try
		{
			if (pPoolConn->isClosed())
			{
				pPoolConn->close();
				pPoolConn->connect();
			}	
		}
		catch (CSQLException& )
		{
			pPoolConn->putConnection();
			throw;
			return NULL;
		}

		CPoolConnAutoPtr<CPoolConnection> poolconn(pPoolConn);
		return poolconn;
	}	
	
	return NULL;
}

void CConnectionPool::keepActive()
{
	//��С�������ж�
	{
		
	}

	//��������
	if ( m_nIdleConnectionCount > 0 )
	{
		CPoolConnection* pPoolConn = NULL;
		for ( unsigned short i = 0 ; i < this->m_nConnectionCount ; i ++ )
		{
			//ȡ����������
			{
				CAutoLock lock(*this->m_BaseLock);	
			
				pPoolConn = m_pConnections[i];
				if ((pPoolConn==NULL)||( !pPoolConn->m_bIsIdle ))
				{
					continue;
				}
				else
				{
					pPoolConn = (CPoolConnection*)getIdleConnection();		
				}
			}
			try
			{
				//�ѶϿ�������������������������
				if ( pPoolConn->isClosed() )
					pPoolConn->connect();
				else
					pPoolConn->keepActive();
				//�Żؿ�������
				pPoolConn->putConnection();
			}
			catch (exception& e)
			{	}
		}
	}
}

