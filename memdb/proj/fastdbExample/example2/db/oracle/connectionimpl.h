#ifndef _CCONNECTIONIMPL_H_
#define _CCONNECTIONIMPL_H_

#include <occi.h>
#include <db/connection.h>

class CConnectionManager;

namespace avocado_oracle
{
	class CConnectionImpl : public CConnection
	{
		friend class CConnectionManager;
	protected:
		//occi
		oracle::occi::Environment* m_pEnv;
		oracle::occi::Connection* m_pConn;
//		CRefConnection* m_pRefConn;
		OCCI_STD_NAMESPACE::string m_UsrName;
		OCCI_STD_NAMESPACE::string m_UsrPwd;
		OCCI_STD_NAMESPACE::string m_Database;
		bool m_AutoCommit;
		bool m_AutoCommitTmp;
		bool m_bConnected;
		int m_BeginCount;

	public:
		enum ETranctionLevel
		{
			TRANSACTION_NONE ,
			TRANSACTION_READ_COMMITTED , 
			TRANSACTION_READ_UNCOMMITTED ,
			TRANSACTION_REPEATABLE_READ ,
			TRANSACTION_SERIALIZABLE ,
		};
		
		//���캯��,����url��ȡ,�Ժ���չʱ��
		//CConnectionImpl(const char* pConnectionURL);
		//���캯��
		CConnectionImpl(string UsrName,string UsrPwd,string DataBase);

		//��������
		~CConnectionImpl();

		//���ݿ����ӿ�¡
		CAutoPtr<CConnection> clone();

		// ������ִ�е�SQLStatement������ִ����ͨ��SQL���
		CAutoPtr <CStatement> createStatement();

		// �����Զ��ύ��������
    
		void setAutoCommit(bool bAutoCommit);

		// ��ȡ��ǰ�����Ƿ��Զ��ύ����
    
		bool getAutoCommit();

		// ��ʽ��������
    
		bool begin();

		// ִ������
    
		bool commit();

		// �ع�����
    
		bool rollback();

		// �������ݿ�
		bool connect();

		// �ر����ݿ�����
    
		void close();

		bool checkConnect(std::string MethodName);

		// ������ݿ������Ƿ��ѹر�
    
		bool isClosed();

		// Ԥִ��SQL�������ڴ�����SQL�����
		CAutoPtr<CStatement> prepareStatement(const char* pSQL);

		//�������ݿ�����ר�÷���
		bool testStatement();
		

		// ���ô洢����    
		CAutoPtr<CCallableStatement> prepareCall(const char* pSQL);

		

		// �������񼶱�
		// ���м���ο�TRANSACTION_XXX��ö��ֵ
    
		void setTransactionIsolation(int nLevel);

		//��������
		void keepActive();

		// ��ֹSQL��䣬����ִ��SQL�����Ҫ���µ���createStatement

		bool terminateStatement(oracle::occi::Statement*  m_pStmt);

		//��ȡ������ǰConnection��Environment

		oracle::occi::Environment* getEnvironment();

		//��ȡConnection
		oracle::occi::Connection* getConnection();
	};
}// namespace avocado_oracle 


#endif 
