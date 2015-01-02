#include <framework/endpoint.h>
#include <framework/protocol.h>
#include <framework/debuginfo.h>
#include <xml/stringfun.h>

#include <net/driver/ip4address.h>

#define HTTP_END "\r\n\r\n"

/*

		Class	IEndPointHandler

*/
IEndPointHandler::~IEndPointHandler()
{
}

/*

		Class	IEndPoint

*/
string IEndPoint::getName(const string& Address,uint16 nPort,IEndPoint::EENDPOINT_TYPE nType)
{
	return CIP4Address(Address.c_str(),nPort).getString();
}

IEndPoint::IEndPoint() : m_InputStream(NULL,0)
{
	m_nType = LOCAL_ENDPOINT;
	m_nStatus = STATUS_WAITHTTP;
	m_pProtocol = NULL;
	this->m_pRecvBuf = CBytesBufferPtr::createInstance("ENDPOINT::RECVBUF");
	this->m_pSendBuf = CBytesBufferPtr::createInstance("ENDPOINT::SENDBUF");
	this->m_pHttpBuf = CBytesBufferPtr::createInstance("ENDPOINT::HTTPBUF");
	//��ֹ�����ü���ָ��ɾ��
	m_InputStream.setStackObject();
	m_Name[0] = 0;
	m_pParam = NULL;
}

IEndPoint::~IEndPoint()
{
}

void IEndPoint::writeRecvData(const char* pData,uint32 DataLen)
{
	if ( m_nStatus == STATUS_WAITHTTP )
	{
		int index = 0;
		//����http����״̬�������д���
		m_pHttpBuf->writeBytes(pData,DataLen);
		if ( m_pHttpBuf->getDataSize() < 3 )
			return ;
		//����Ƿ���HTTPͷ
		if ( strncasecmp(m_pHttpBuf->c_str(),"GET",3) == 0 )
		{
			//����Ƿ���\r\n\r\n
			index = CStrFun::find(m_pHttpBuf->c_str(),HTTP_END);
			if ( index == -1 )
				return ;
			index += sizeof(HTTP_END)-1;
			m_nStatus = STATUS_SUCCESS;
			if ( m_pHttpBuf->getDataSize() <= (uint32)index )
				return;
			//
		}
		m_nStatus = STATUS_SUCCESS;
		pData = m_pHttpBuf->c_str() + index;
		DataLen = m_pHttpBuf->getDataSize() - index;
	}
	else
	{
		//releae http buffer
		m_pHttpBuf = NULL;
	}

	uint32 buflen = DataLen;
	if ( this->m_pEncryptor != NULL )
	{
		m_InputStream.reset(pData,DataLen);
		this->m_pEncryptor->decrypt(&m_InputStream,m_pRecvBuf.get());
	}
	else
	{
		this->m_pRecvBuf->writeBytes(pData,buflen);
	}
	//ʹ�û�����д��������ڶ������ʱ�򣬿ɽ���һ���Ե�popup
	m_InputStream.reset(m_pRecvBuf->c_str(),m_pRecvBuf->getDataSize());
}

CBytesBufferPtr IEndPoint::next(uint32 &nSerialNo,uint32 &nServantName)
{
	if ( this->m_pConnection == NULL )
	{
		log_error("EndPoint|next|no connection[%s]",getAddress());
		return NULL;
	}
	if ( this->m_pProtocol == NULL )
	{
		log_error("EndPoint|next|no protocol[%s]",getAddress());
		return NULL;
	}
	CBytesBufferPtr pbuff = CBytesBufferPtr::createInstance(__FUNCTION__);
	switch ( m_pProtocol->next(m_InputStream,nSerialNo,nServantName,*pbuff.get()) )
	{
		case PRC_NOMOREPACK :
			{
				//��֮ǰ�İ���һ����popup�����ж������buffer�е�ʱ�򣬼���pop�Ĵ�����Ҳ������ڴ濽���Ĵ���
				this->m_pRecvBuf->popBytes(m_pRecvBuf->getDataSize()-m_InputStream.getDataSize());
				return NULL;
			}
		case PRC_ERROR :
			{
				//Э���д���������еĻ���
				m_pRecvBuf->clear();
				m_pRecvBuf->compact();
				return NULL;
			}
		case PRC_SUCCESS :
			{
				return pbuff;
			}
	}
	return NULL;
}


void IEndPoint::response(uint32 nSerialNo, uint32 nServantName, CBytesBuffer& pBody)
{
	send(nSerialNo,m_pProtocol->getResponseName(nServantName),pBody);
}

void IEndPoint::send(uint32 nSerialNo, uint32 nServantName, CBytesBuffer& pBody)
{
	CAutoLock lock(m_Lock);
	if ( this->m_pConnection == NULL )
	{
		log_error("EndPoint|send|no connection[%s]",getAddress());
		return ;
	}
	if ( this->m_pProtocol == NULL )
	{
		log_error("EndPoint|send|no protocol[%s]",getAddress());
		return ;
	}

	IOutputStreamPtr output = NULL;

	if ( m_pEncryptor != NULL )
	{
		m_pSendBuf->clear();
		output = m_pSendBuf.get();
	}
	else
	{
		output = m_pConnection->getOutputStream();
	}

	/**
	 * ���Э��
	 * @return int 0�����ɹ���-1����ʧ��
	 */
	if ( 0 != m_pProtocol->pack(nSerialNo,nServantName,pBody,*output.get()) )
	{
		DEBUG_LOG("IEndPoint::send protocol pack error return nonzero");
	}

	if ( m_pEncryptor != NULL )
	{
		//����
		m_pEncryptor->encrypt(m_pSendBuf.get(),m_pConnection->getOutputStream());
	}
}

void IEndPoint::sendHeartBeat()
{
	CAutoLock lock(m_Lock);
	if ( NULL == m_pConnection || NULL == m_pProtocol )
	{
		return ;
	}

	IOutputStreamPtr output = NULL;

	if ( m_pEncryptor != NULL )
	{
		m_pSendBuf->clear();
		output = m_pSendBuf.get();
	}
	else
	{
		output = m_pConnection->getOutputStream();
	}

	/**
	 * ���Э��
	 * @return int 0�����ɹ���-1����ʧ��
	 */
	if ( 0 != m_pProtocol->packHeartBeat(*output.get()) )
	{
		DEBUG_LOG("IEndPoint::send protocol pack error return nonzero");
	}

	if ( m_pEncryptor != NULL )
	{
		//����
		m_pEncryptor->encrypt(m_pSendBuf.get(),m_pConnection->getOutputStream());
	}
}


void IEndPoint::setConnection(IConnectionPtr Connection)
{
	m_pConnection = Connection;
	m_pAddress = m_pConnection->getRemoteAddress();
	this->m_pRecvBuf->clear();
}

IConnectionPtr IEndPoint::getConnection()
{
	return m_pConnection;
}

const char* IEndPoint::getName()
{
	return m_Name;
}

IEndPoint::EENDPOINT_TYPE IEndPoint::getType()
{
	return m_nType;
}

void IEndPoint::setType(EENDPOINT_TYPE Type)
{
	m_nType = Type;
}

void IEndPoint::setAcceptor(IAcceptorPtr Acceptor)
{
	m_pAcceptor = Acceptor;
	if ( Acceptor == NULL )
		return ;
	snprintf(m_Name,sizeof(m_Name),"%s",Acceptor->getListenAddress()->getString());
}

IAcceptorPtr IEndPoint::getAcceptor()
{
	return m_pAcceptor;
}

void IEndPoint::setConnector(IConnectorPtr Connector)
{
	m_pConnector = Connector;
	if ( Connector == NULL )
		return ;
	snprintf(m_Name,sizeof(m_Name),"%s",Connector->getConnectAddress()->getString());
}

IConnectorPtr IEndPoint::getConnector()
{
	return m_pConnector;
}

const char* IEndPoint::getAddress() const
{
	if ( m_pAddress == NULL )
		return "unknow";
	return m_pAddress->getString();
}

void IEndPoint::setNetBufferSize(int NetBuffer)
{
	m_nNetBufferSize = NetBuffer;
}

int IEndPoint::getNetBufferSize() const
{
	return m_nNetBufferSize;
}

void IEndPoint::setParam( void* pParam )
{
	m_pParam = pParam;
}

void* IEndPoint::getParam()
{
	return m_pParam;
}


