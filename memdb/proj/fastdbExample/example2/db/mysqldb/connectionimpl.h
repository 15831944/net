#ifndef _CCONNECTIONIMPL_H_
#define _CCONNECTIONIMPL_H_

#include <mysql.h>
#include <db/connection.h>

class CConnectionManager;

namespace mysql
{
	class CStatementImpl;
	class CResultsetImpl;
	class CConnectionImpl : public CConnection
	{
		friend class CStatementImpl;
		friend class CResultSetImpl;
		friend class CCallableStatementImpl;
	protected :
		//mysql c api
		MYSQL* m_pConn;
		//�û���
		string m_UserName;
		//����
		string m_PassWord;
		//���ݿ���
		string m_Database;
		//���ӵ�ַ
		string m_Host;
		//���Ӷ˿�
		unsigned short m_nPort;
		//���ӱ�ʶ
		bool m_bConnected;
		//�Զ��ύ��ʶ
		bool m_bAutoCommit;

	public :
		//���캯��,����url��ȡ,�Ժ���չʱ��
		//CConnectionImpl(const char* pConnectionURL);
		//���캯��
		/**
		 * ���캯��
		 * @param Host string ������ַ
		 * @param Port short �����˿�
		 * @param DataBase string ���ݿ���
		 * @param UserName �û���
		 * @param PassWord ����
		 */
		CConnectionImpl(const string& Host,unsigned short Port = 0,const string& DataBase = "",const string& UserName = "",const string& Password = "");

		//��������
		virtual ~CConnectionImpl();
		//��¡
		CAutoPtr <CConnection> clone();
		//������ִ�е�SQLStatement������ִ����ͨ��SQL��� 
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
		// ������ݿ������Ƿ��ѹر�
		bool isClosed();
		//Ԥִ��SQL�������ڴ�����SQL�����
		CAutoPtr<CStatement> prepareStatement(const char* pSQL);
		//���ô洢����    
		CAutoPtr<CCallableStatement> prepareCall(const char* pSQL);
		// �������񼶱�
		// ���м���ο�TRANSACTION_XXX��ö��ֵ
		void setTransactionIsolation(int nLevel);
		//��������
		void keepActive();
		//************
		/**
		 * ��ȡmysql�Ĵ����
		 * @return int �����
		 */
		int getErrNo();
		/**
		 * ��ȡmysql�Ĵ�����ַ�������
		 * @return char* �ַ�������
		 */
		const char* getError();
	};
}// namespace avocado_mysql

#endif
