#ifndef _MY_ODBCCONNECTION_H_
#define _MY_ODBCCONNECTION_H_
#include "./odbcpublic.h"
#include <util/lightlock.h>
namespace my_odbc 
{
	class COdbcConnection:
		public CConnection
	{
		friend class COdbcStatement;
		friend class COdbcResultSet;
		friend class COdbcCallableStatement;
	protected :
		SQLHDBC m_pConn;
		//�û���
		std::string m_UserName;
		//����
		std::string m_PassWord;
		//���ӵ�ַ
		std::string m_ConnStr;
		//���Ӷ˿�
		unsigned short m_nPort;
		//���ӱ�ʶ
		bool m_bConnected;
		//�Զ��ύ��ʶ
		bool m_bAutoCommit;

	public :

		SQLHDBC getConnHandle(){ return m_pConn; }
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
		COdbcConnection(const std::string& Host,unsigned short Port = 0,
					const std::string& UserName = "",const std::string& Password = "");
		//���캯��
		COdbcConnection();
		//��������
		virtual ~COdbcConnection();
		//��¡
		virtual CAutoPtr<CConnection> clone();
		//������ִ�е�SQLStatement������ִ����ͨ��SQL��� 
		CAutoPtr<CStatement> createStatement();
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
		//�����������ݿ�
		bool reconnect();
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
		/**
		* �����ַ�������
		* @pEncode �ͻ���
		* @retrun �ַ������뷽ʽ
		*/
		virtual bool setEncoding(const char * pEncode = "gb2312");
		/**
		* @param pConnString�����ַ���
		* @param �ɹ�����TRUEʧ�ܷ���FALSE
		*/
		virtual bool setConnectString(const char * pConnString , const char * pUserName , const char * pUserPwd);
		//************
		/**
		 * ��ȡmysql�Ĵ����
		 * @return int �����
		 */
		virtual int getErrNo() const;
		/**
		 * ��ȡmysql�Ĵ�����ַ�������
		 * @return char* �ַ�������
		 */
		virtual const char* getError() const;
	public:
		//mysql c api
		static CLightLock m_CountLock;
		static int m_ConnCount;
		static SQLHENV m_pEven; 
		static void initialize();
		static void finalize();
	};
}
#endif