#ifndef _CCONNECTION_H_
#define _CCONNECTION_H_

class CStatement;
class CCallableStatement;

/**
 * ���ݿ�������
 * @doc ʵ�����ݿ����ӹ��ܡ�
 * <BR> ���ڴ�������sql����CStatement�������ɵ��ô洢���̡�������CCallableStatement��
 * <BR> ����ά�������ṩ����Ľ������ύ���ع�������Ĭ������£�ÿ��sql������Լ��ύ�ġ�
 * <BR> ����Ϊ�����࣬����ʵ����������ͨ��CConnectionManager::getConnection������ȡ��������ָ�롣
 */
class CConnection
{
  public:
	  virtual ~CConnection(){};

    /**
	 * ����CStatement������ִ����ͨ��SQL���
	 * @return CAutoPtr<CStatement> Statement����ָ��	
	 * @exception CSQLException ����CStatementʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */    
    virtual CAutoPtr < CStatement > createStatement()=0;

	/**
	 * �����Զ��ύ��������
	 * @param bAutoCommit �Ƿ��Զ��ύ��true:�ǣ�false:��
	 */    
    virtual void setAutoCommit(bool bAutoCommit)=0;

    /**
	 * ��ȡ��ǰ�����Ƿ��Զ��ύ����
	 * @return �Ƿ��Զ��ύ��true:�ǣ�false:��
	 */
    virtual bool getAutoCommit()=0;

	/**    
	 * ��ʽ��������
	 * @return �����Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 */    
    virtual bool begin()=0;

	/**
	 * �ύ����
     * @return �ύ�Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 * @exception CSQLException �ύʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool commit()=0;

	/**
	 * �ع�����    
	 * @return �ع��Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 * @exception CSQLException �ع�ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool rollback()=0;

    /**
	 * �������ݿ�
	 * @return �����Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 * @exception CSQLException ���ݿ�����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool connect()=0;

    /**
	 * �ر����ݿ�����
	 * @return �ر��Ƿ�ɹ���true:�ɹ���false:ʧ�ܣ�
	 * @exception CSQLException ���ݿ����ӹر�ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual void close()=0;

    /**
	 * ������ݿ������Ƿ��ѹر�
	 * @return ���ݿ������Ƿ��ѹرգ�true:�ǣ�false:��
	 * @exception CSQLException ������ݿ����ӹر�ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */    
    virtual bool isClosed()=0;

	/**
	 * ����׼����SQL����CStatement����������Ҫ���ò�����SQL���    
	 * @return CAutoPtr<CStatement> CStatement����ָ��
	 * @exception CSQLException ����׼����SQL����CStatementʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CAutoPtr<CStatement> prepareStatement(const char* pSQL)=0;

    
	/**
	 * ����׼����SQL����CCallableStatement�������ڵ��ô洢���̡����������и������sql���    
	 * @return CAutoPtr<CCallableStatement> CCallableStatement����ָ��
	 * @exception CSQLException ����׼����SQL����CCallableStatementʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CAutoPtr< CCallableStatement > prepareCall(const char* pSQL)=0;

    // �������񼶱�
    // ���м���ο�TRANSACTION_XXX��ö��ֵ    
    virtual void setTransactionIsolation(int nLevel)=0;

	/**
	 * �������ӣ���������Ѿ��Ͽ������׳��쳣
	 * @exception CSQLException ��������ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */  
	virtual void keepActive()=0;

	/**
	 * ���ݿ����ӿ�¡������ȡ���Ӳ�����ͬ����һ������
	 * @return CAutoPtr<CConnection> CConnection����ָ��
	 * @exception CSQLException
	 */
	virtual CAutoPtr<CConnection> clone()=0;
	
	static const int TRANSACTION_NONE;
    
    static const int TRANSACTION_READ_COMMITTED;
    
    static const int TRANSACTION_READ_UNCOMMITTED;
    
    static const int TRANSACTION_REPEATABLE_READ;
    
    static const int TRANSACTION_SERIALIZABLE;
};

/*
const int CConnection::TRANSACTION_NONE=1;
const int CConnection::TRANSACTION_READ_COMMITTED=2;
const int CConnection::TRANSACTION_READ_UNCOMMITTED=3;
const int CConnection::TRANSACTION_REPEATABLE_READ=4;
const int CConnection::TRANSACTION_SERIALIZABLE=5;
*/

#endif 
