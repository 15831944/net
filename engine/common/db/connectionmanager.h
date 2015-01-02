#if !defined(_CCONNECTIONMANAGER_H_)
#define _CCONNECTIONMANAGER_H_

class CConnection;
class CConnectionPool;

/**
 * ���ݿ����ӹ�����
 * @doc ����ָ���������ַ�������ʵ�ʵ����ݿ�����
 * <br> �������ṩ�ķ������Ǿ�̬���������Բ���Ҫʵ���������캯��Ϊ˽��
 */
class CConnectionManager
{
private:
public:
	/**
	 * ��ȡ���ݿ�����
	 * @param pConnectionURL ���ݿ����ƻ����ӱ���
	 * @param UserName ��½�û���
	 * @param UserPwd ��½����
	 * @return CAutoPtr<CConnection> ���ݿ����ӵ�����ָ��
	 * @exception CSQLException ���ݿ�����ʧ�ܣ�����ԭ�������sqlerrcode�������ݿ�������˵��
	 */
	static CAutoPtr<CConnection> getConnection(const char* pConnectionURL,const char* UserName="",const char* UserPwd="");

	/**
	 * ��ȡ���ݿ����ӳ�
	 * @param pConnectionURL ���ݿ����ƻ����ӱ���
	 * @param nMaxConnections ���ӳ����������
	 * @param UserName ��½�û���
	 * @param UserPwd ��½����
	 * @return CAutoPtr<CConnection> ���ݿ����ӳص�����ָ��
	 */
	static CAutoPtr <CConnectionPool> getConnectionPool(const char* pConnectionURL,int nMaxConnections,const char* UserName="",const char* UserPwd="");

private:

	CConnectionManager();
};

#endif 
