#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_

#include <net/acceptor.h>
#include <net/connector.h>
#include <net/connection.h>
#include <protocol/encryptor.h>
#include <protocol/compressor.h>
#include <util/refshare.h>
#include <io/bytesbuffer.h>
#include <framework/protocol.h>

#include <io/bytesinputstream.h>

/*
  �˵㣬���ڷ�װͨ�š�Э��Ĵ���
*/
class DLLAPI IEndPoint : public CRefShare
{
public:
	enum EENDPOINT_TYPE
	{
		//���ض˵�
		LOCAL_ENDPOINT,
		//Զ�̶˵�
		REMOTE_ENDPOINT
	};
	enum EENDPOINT_SOCKTYPE
	{
		//TCP����
		SOCKTYPE_TCP,
		//UDP����
		SOCKTYPE_UDP
	};

	enum ECONNECT_STATUS
	{
		//�ȴ�HTTP
		STATUS_WAITHTTP,
		//���ֳɹ�
		STATUS_SUCCESS,
	};
	/**
	 * ��̬��������ȡendpoint��Ψһ����
	 */
	static string getName(const string& Address,uint16 nPort,IEndPoint::EENDPOINT_TYPE nType);

	IEndPoint();
	virtual ~IEndPoint();

	/**
	 * ����ͨ�ŵļ��ܷ�ʽ
	 */
	void setEncryptor(IEncryptorPtr Encryptor) { m_pEncryptor = Encryptor; }
	IEncryptorPtr getEncryptor() { return m_pEncryptor; }

	/**
	 * ����ͨ�ŵ�ѹ��ģʽ
	 */
	void setComprossor(ICompressorPtr Compressor) { m_pCompressor = Compressor; }
	ICompressorPtr getComprossor() { return m_pCompressor; }
	/**
	 * ����Endpointʹ�õ�Э������
	 */
	void setProtocol(IProtocolPtr pProtocol) { m_pProtocol = pProtocol ; }
	IProtocolPtr getProtocol() { return m_pProtocol; }

	/**
	 * ��ȡEndpoint���ͣ�
	 */
	EENDPOINT_TYPE getType();
	/**
	 * ����Endpoint����
	 */
	void setType(EENDPOINT_TYPE Type);

	/**
	 * Endpoint����ʵͨ������
	 */
	void setConnection(IConnectionPtr Connection);
	/**
	 * ��ȡendpoint��ͨ������
	 */
	IConnectionPtr getConnection();
	/**
	 * ������Ϣ
	 * @param nSerialNo ��Ϣ��ˮ��
	 * @param nServantName Э������
	 * @param pBody ��Ϣ����
	 */
	void send(uint32 nSerialNo,uint32 nServantName,CBytesBuffer& pBody);
	/**
	 * ����������Ϣ
	 */
	void sendHeartBeat();
	/**
	 * Ӧ����Ϣ
	 * @param nSerialNo ������Ϣ����ˮ��
	 * @param nServantName ����Э������
	 * @param pBody Ӧ�����Ϣ����
	 */
	void response(uint32 nSerialNo,uint32 nServantName,CBytesBuffer& pBody);
	/**
	 * ��ȡ��Ϣ
	 * @param nSerialNo �����������Ϣ��ˮ��
	 * @param nServantName ���������Э������
	 * @return CBytesBufferPtr ��Ϣ���ݣ����û�пɶ���Ϣ���򷵻�NULL
	 */
	CBytesBufferPtr next(uint32 &nSerialNo,uint32 &nServantName);

	void setAcceptor(IAcceptorPtr Acceptor);
	IAcceptorPtr getAcceptor();

	void setConnector(IConnectorPtr Connector);
	IConnectorPtr getConnector();
	/**
	 * ���õ�ǰendpoint�����绺������С����λ��KB
	 */
	void setNetBufferSize(int NetBuffer);
	/**
	 * ��ȡ��ǰendpoint�����绺������С����λ��KB
	 */
	int getNetBufferSize() const;
	/**
	 * ��ȡ��ַ
	 */
	const char* getAddress() const;

	/**
	 * д���ȡ����Ϣ
	 * @param pData  ����
	 * @param DataLen ���ݳ���
	 */
	void writeRecvData(const char* pData,uint32 DataLen);

	/**
	 * ��ȡendpoint��Ψһ����
	 */
	const char* getName();

	/**
	 *
	 */
	void setParam( void* pParam );
	void* getParam();

private:
	IAcceptorPtr   m_pAcceptor;
	IConnectorPtr  m_pConnector;
	IConnectionPtr m_pConnection;
	//��ַ
	IAddressPtr m_pAddress;
	//����ʵ��
	IEncryptorPtr  m_pEncryptor;
	//ѹ��ʵ��
	ICompressorPtr m_pCompressor;
	//ҵ��Э��
	IProtocolPtr m_pProtocol;
	//��ȡ������
	CBytesBufferPtr m_pRecvBuf;
	//���ͻ�����
	CBytesBufferPtr m_pSendBuf;
	//HTTP���Ӵ�������
	CBytesBufferPtr m_pHttpBuf;
	//������
	CBytesInputStream m_InputStream;
	//��ǰ������״̬
	uint8 m_nStatus;
	//endpoint������
	EENDPOINT_TYPE m_nType;
	//����
	char m_Name[50];
	//���绺������С
	int m_nNetBufferSize;
	//
	CLightLock m_Lock;
	//
	void* m_pParam;
};

typedef CRefObject<IEndPoint> IEndPointPtr;

class DLLAPI IEndPointHandler : public IRefShare
{
public :
	enum
	{
//		MASK_READ = 0x01,
		MASK_OPEN = 0x02,
		MASK_CLOSE = 0x04,
		MASK_ACCEPT = 0x08,
//		MASK_WRITE = 0x10,
        MASK_NOTRECONNECT = 0x20
	};
	virtual ~IEndPointHandler() = 0;

	/**
	 * ���¼�
	 * @param RetCode int32 �¼��ķ���ֵ��0�����ɹ���-1����ʧ��
	 * @param EndPoint �������¼���EndPoint
	 * @return int32 ��������0��������ɹ���-1��������ʧ��
	 */
	virtual int32 onRead(int32 RetCode,IEndPointPtr EndPoint) = 0;
	/**
	 * д�¼�
	 * @param RetCode int32 �¼��ķ���ֵ��0�����ɹ���-1����ʧ��
	 * @param EndPoint �������¼���EndPoint
	 * @return int32 ��������0��������ɹ���-1��������ʧ��
	 */
	virtual int32 onWrite(int32 RetCode,IEndPointPtr EndPoint) = 0;
	/**
	 * �ر��¼�
	 * @param RetCode int32 �¼��ķ���ֵ��0�����ɹ���-1����ʧ��
	 * @param EndPoint �������¼���EndPoint
	 * @return int32 ��������0��������ɹ���-1��������ʧ��
	 */
	virtual int32 onClose(int32 RetCode,IEndPointPtr EndPoint) = 0;
	/**
	 * ���ӽ����¼�
	 * @param RetCode int32 �¼��ķ���ֵ��0�����ɹ���-1����ʧ��
	 * @param EndPoint �������¼���EndPoint
	 * @return int32 ��������0��������ɹ���-1��������ʧ��
	 */
	virtual int32 onOpen(int32 RetCode,IEndPointPtr EndPoint) = 0;
	/**
	 * ���ӽ����¼�
	 * @param RetCode int32 �¼��ķ���ֵ��0�����ɹ���-1����ʧ��
	 * @param EndPoint �������¼���EndPoint
	 * @return int32 ��������0��������ɹ���-1��������ʧ��
	 */
	virtual int32 onAccept(int32 RetCode,IEndPointPtr EndPoint) = 0;
};

typedef CRefObject<IEndPointHandler> IEndPointHandlerPtr;

template <typename T>
class TEndPointHandler : public IEndPointHandler, public CRefShare
{
public:
    TEndPointHandler(T* pClass, 
        int32 (T::*pfnOnRead)(int32 ,IEndPointPtr),
        int32 (T::*pfnOnWrite)(int32 ,IEndPointPtr),
        int32 (T::*pfnOnClose)(int32 ,IEndPointPtr),
        int32 (T::*pfnOnOpen)(int32 ,IEndPointPtr),
        int32 (T::*pfnOnAccept)(int32 ,IEndPointPtr)
        ) :
        m_pClass(pClass),
        m_pfnOnRead(pfnOnRead),
        m_pfnOnWrite(pfnOnWrite),
        m_pfnOnClose(pfnOnClose),
        m_pfnOnOpen(pfnOnOpen),
        m_pfnOnAccept(pfnOnAccept)
    {
    }

    virtual ~TEndPointHandler()
    {
    }

    virtual int32 onRead(int32 RetCode,IEndPointPtr EndPoint)
    {
        if ( m_pfnOnRead != NULL )
            return (m_pClass->*m_pfnOnRead)( RetCode, EndPoint );
        else
            return 0;
    }

    virtual int32 onWrite(int32 RetCode,IEndPointPtr EndPoint)
    {
        if ( m_pfnOnWrite != NULL )
            return (m_pClass->*m_pfnOnWrite)( RetCode, EndPoint );
        else
            return 0;
    }
	
    virtual int32 onClose(int32 RetCode,IEndPointPtr EndPoint)
    {
        if ( m_pfnOnClose != NULL )
            return (m_pClass->*m_pfnOnClose)( RetCode, EndPoint );
        else
            return 0;
    }
	
    virtual int32 onOpen(int32 RetCode,IEndPointPtr EndPoint)
    {
        if ( m_pfnOnOpen != NULL )
            return (m_pClass->*m_pfnOnOpen)( RetCode, EndPoint );
        else
            return 0;
    }

	virtual int32 onAccept(int32 RetCode,IEndPointPtr EndPoint)
    {
        if ( m_pfnOnAccept != NULL )
            return (m_pClass->*m_pfnOnAccept)( RetCode, EndPoint );
        else
            return 0;
    }

protected:
    void incRef()
    {
        CRefShare::incRef();
    }

    bool decRef()
    {
        return CRefShare::decRef();
    } 

private:
    T* m_pClass;

    int32 (T::*m_pfnOnRead)(int32,IEndPointPtr);
    int32 (T::*m_pfnOnWrite)(int32,IEndPointPtr);
    int32 (T::*m_pfnOnClose)(int32,IEndPointPtr);
    int32 (T::*m_pfnOnOpen)(int32,IEndPointPtr);
    int32 (T::*m_pfnOnAccept)(int32,IEndPointPtr);
};

#endif 

