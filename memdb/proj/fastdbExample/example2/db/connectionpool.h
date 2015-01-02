#ifndef _CCONNECTIONPOOL_H_
#define _CCONNECTIONPOOL_H_

template <class T>
class CPoolConnAutoPtr;
class CConnection;
class CConnectionManager;
class CPoolConnection;

/**
 * ���ݿ����ӳ�
 * @doc �Լ��ϵķ�ʽά�����ݿ����ӡ�
 * <BR> ͨ�����м��������������ʱ���Ὠ��һ�����ݿ����ӳأ��м������DLL����Ҫʱ�������ڳ��л�ȡ���ݿ�������������ݿ������
 * <BR> ���ݿ����ӳز������²�������������
 * <BR> 
 */
class CConnectionPool
{
friend class CConnectionManager;
friend class CPoolConnection;
  public:
    /**
     * �����ӳ��л�ȡ���ݿ����ӣ�������ӳ����Ѿ�û�п��õ����ݿ����ӣ�������������������������׳��쳣
     * @param nWaitMillis �ȴ��ĺ�����������ȴ�ʱ�䵽����׳��쳣��=0��ʾ���Ϸ��أ�<0��ʾ���޵ȴ���>0��ʾ�ȴ��ĺ�����
	 * @return CAutoPtr<CPoolConnection> ���ݿ����ӵ�����ָ��
	 * @exception CSQLException �����ӳ��л�ȡ���ݿ�����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    CPoolConnAutoPtr<CPoolConnection> getConnection(long nWaitMillis);    

    /**
	 * ��������
	 */
    ~CConnectionPool();
	/**
	 * ��֤���ݿ����ӵļ���״̬
	 * 
	 */
	void keepActive();

  protected:

    //�ͷ�ָ��������
    void putConnection(CPoolConnection* pConn);

	//��ȡ���е�����
	const CPoolConnection* getIdleConnection();

	//ͨ��URL�������ӳ�
    CConnectionPool(const char* pURL, int nMaxConnections,const char* pUsr="",const char* pUsrPwd="");

  private:
    //
    CPoolConnection** m_pConnections;

    //
    //CConnectionManager* m_pConnectionManager;

    //����������
    int m_nMaxConnections;

	//��С������
	int m_nMinConnections;	

	//��ǰ��������
	int m_nConnectionCount;

	//��ǰ���е�������
	int m_nIdleConnectionCount;

	//���һ�α�ȡ���Ŀ������ӵ��±�
	int m_nLastConnID;

	//���ݿ����
	string m_Url;
	
	//�û���
	string m_Usr;

	//����
	string m_UsrPwd;	

	CBaseLock* m_BaseLock;
};
#endif
