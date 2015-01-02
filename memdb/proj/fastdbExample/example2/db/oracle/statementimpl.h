#ifndef _CSTATEMENTIMPL_H_
#define _CSTATEMENTIMPL_H_

#include <db/statement.h>

namespace avocado_oracle
{
class CConnectonImpl;
class CRefStatement;
class CResultSetImpl;
	// �������������sql���
	//
	class CStatementImpl : public CStatement
	{
	friend class CConnectionImpl;

	//��CResultSetImpl����ʱ����Ҫ��CStatementImpl��ָ���Լ���ָ����գ����ԣ���������Ҫ����m_pStmt
	friend class CResultSetImpl;	
	  protected:
		CConnectionImpl* m_pConn;
		CRefStatement* m_pRefStmt;
		oracle::occi::Statement*  m_pStmt;
		std::string               m_strErrMsg;
		std::string m_Sql;
		CResultSetImpl* m_pRst;
		map<int,Clob> m_Clobs;

		//���캯��
		CStatementImpl();

	  public:
		//��������
		~CStatementImpl();

			//
		void close();

		int getTimeZoneOffset(bool Throw=false);

		// ��ȡ����ֵ
		//
		Long64 getInsertID(void* pParam);

		// ִ�в�ѯSQL�����ؽ����
		CAutoPtr<CResultSet> executeQuery(const char* pSQL);

		/**
		 * ִ�и��µ���䲢����Ӱ�������
		 * @param pSQL �ύ���е�sql���
		 * @throws CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
		 */
		int executeUpdate(const char* pSQL);

		//
		bool execute(const char* pSQL);    

		// ִ�е�ǰ��SQL���
		//
		bool execute();

		// ִ�е�ǰ��ѯ
		//
		CAutoPtr<CResultSet> executeQuery();

		// ִ�е�ǰ�ĸ���SQL����
		// ����ִ��Ӱ���������
		//
		int executeUpdate();

		// ������в�����ֵ
		//
		void clearParameters();

		//�˷���,oracle��Ŀǰ��֧��
		void setByte(int nParamIndex, char Value);

		void saveBytes(const char* strColumnName, const unsigned char* Value, int nLength,bool SetSequence=false,string SequenceName="",bool SetPkName=false,string PkName="");

		//���´�����ֶ�ǰ��δ��statementִ��executequery�����׳��쳣
		void setBytes(int nParamIndex, const unsigned char* Value, int nLength);

		//
		//void setBoolean(int nParamIndex, bool Value);

		//
		void setString(int nParamIndex, const std::string& Value);

		//
		void setInt(int nParamIndex, int Value);

		//
		void setDouble(int nParamIndex, double Value);

		//
		void setNull(int nParamIndex, EDATATYPE nEDATATYPE);

		//
		void setDate(int nParamIndex, CDateTime Value);

		//
		void setTime(int nParamIndex, CDateTime Value);

		//
		void setTimestamp(int nParamIndex, CDateTime Value);

		//
		void setLong64(int nParamIndex, Long64 Value); 	 

		void prepare(const char* pSql);

		const char* getSQL() const;

		string getInsertTableName();
	};
}// namespace avocado_oracle


#endif
