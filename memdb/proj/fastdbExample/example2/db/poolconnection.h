#ifndef _CPOOLCONNECTION_H_
#define _CPOOLCONNECTION_H_

class CConnectionPool;
/**
 * �����ݿ�����
 * @doc ʵ�����ݿ����ӵ����й��ܣ������ṩ�����ݿ����ӳ���ά���Ĺ���
 * <br> ���಻�ܱ�ʵ����������ͨ��CConnectionPool��getConnection�����������������ָ��
 */
class CPoolConnection:public CConnection
{
public:

	/**
	 * ����CStatement������ִ����ͨ��SQL���
	 * @return CAutoPtr<CStatement> Statement����ָ��
	 * @exception CSQLException ����CStatementʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    CAutoPtr <CStatement> createStatement();

    /**
	 * �����Զ��ύ��������
	 * @param bAutoCommit �Ƿ��Զ��ύ��true:�ǣ�false:��
	 */    
    void setAutoCommit(bool bAutoCommit);

    /**
	 * ��ȡ��ǰ�����Ƿ��Զ��ύ����
	 * @return �Ƿ��Զ��ύ��true:�ǣ�false:��
	 */    
    bool getAutoCommit();

    /**    
	 * ��ʽ��������
	 * @return �����Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 */    
    bool begin();

    /**
	 * �ύ����
     * @return �ύ�Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 * @exception CSQLException �ύʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    bool commit();

    /**
	 * �ع�����    
	 * @return �ع��Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 * @exception CSQLException �ع�ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    bool rollback();

    /**
	 * �������ݿ�
	 * @return �����Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 * @exception CSQLException ���ݿ�����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    bool connect();

    /**
	 * �ر����ݿ�����
	 * @return �ر��Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 * @exception CSQLException ���ݿ����ӹر�ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */    
    void close();

    /**
	 * ������ݿ������Ƿ��ѹر�
	 * @return ���ݿ������Ƿ��ѹرգ�true:�ǣ�false:��
	 * @exception CSQLException ������ݿ����ӹر�ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    bool isClosed();

    /**
	 * ����׼����SQL����CStatement����������Ҫ���ò�����SQL���    
	 * @return CAutoPtr<CStatement> ׼����sql����Statement����ָ��
	 * @exception CSQLException ����׼����SQL����CStatementʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    CAutoPtr<CStatement> prepareStatement(const char* pSQL);

    /**
	 * ����׼����SQL����CCallableStatement�������ڵ��ô洢���̡����������и������sql���    
	 * @return CAutoPtr<CCallableStatement>
	 * @exception CSQLException ����׼����SQL����CCallableStatementʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    CAutoPtr<CCallableStatement> prepareCall(const char* pSQL);

    /**
	 * �������񼶱�,����occi��֧�֣�����λʵ��
	 */    
    void setTransactionIsolation(int nLevel);

	/**
	 * �������ӣ���������Ѿ��Ͽ������׳��쳣
	 * @exception CSQLException ��������ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */    
	void keepActive();

	/**
	 * ��������
	 */
    ~CPoolConnection();

	/**
	 * ��¡
	 * CPoolConnection�಻֧�ִ˷��������ô˷�����õ��쳣CSQLException 
	 */
	CAutoPtr<CConnection> clone();

protected:
friend class CConnectionPool;
friend class CPoolConnAutoPtr<CPoolConnection>;
	CPoolConnection(CConnection* pConnection,CConnectionPool* pConnectionPool);

	void putConnection();
	//
	CConnection* m_pConnection;

	//
	CConnectionPool* m_pConnectionPool;

	bool m_bIsIdle;
public : 
	//
	unsigned int m_nOwnThreadId;
};



#endif
