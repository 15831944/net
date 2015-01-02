#ifndef _CSTATEMENTIMPL_H_
#define _CSTATEMENTIMPL_H_

#include <mysql.h>
#include <db/statement.h>
#include <util/refshare.h>

namespace mysql
{
	class CConnectionImpl;
	// �������������sql���
	//
	class CStatementImpl : public CStatement,public CRefShare
	{
	protected:
		friend class CPrepareResultSetImpl;
		//���Ӵ���ʵ��
		CConnectionImpl* m_pConn;
		//sql�����
		std::string m_strSql;
		//mysql��preparedstatement�ľ��
		MYSQL_STMT* m_pStmt;
		//���е�sql����
		//CMetaData* m_pParams;
		MYSQL_BIND* m_pParams;
		//�ܲ�������
		int m_nParamCount;
		//
		std::string m_SQL;

	protected :
		/**
		 * ����Ԥ�ύ��䣬���ò�������
		 */
		void prepareParams();
		/**
		 * ����������Ƿ�Ϸ�
		 * @param nIndex int ������
		 */
		void validIndex(int nIndex);
		/**
		 * ɾ�����еĲ���
		 */
		void destroyParams();
		/**
		 * ��ղ���
		 * @param nIndex �����ţ���1��ʼ�����뱣֤��������Ч
		 */
		void clearParam(int nIndex);
		/**
		 *
		 */
		void cleanParam(MYSQL_BIND& Param);

	public:
		/**
		 * ���캯��
		 */
		CStatementImpl(CConnectionImpl& Conn);
		/**
		 * ��������
		 */
		~CStatementImpl();
		/**
		 * ��ȡmysql auto_increment�Զ��������Զ�����ֵ
		 * @return uint64 ���ص�ֵ
		 */
		uint64 getLastInsertID();
		//*********************CStatement�Ľӿں���*******************************
		/**
		 * �ر���Դ
		 */
		void close();
		/**
		 * ͳһ�ӿڣ���ȡ�����idֵ
		 * @param pParam �����������mysqlû��
		 * @return int64 ���ص���ֵ
		 */
		uint64 getInsertID(void* pParam);
		/**
		 * ִ�в�ѯSQL�����ؽ����
		 */
		CAutoPtr<CResultSet> executeQuery(const char* pSQL);
		/**
		 * ִ�и��µ���䲢����Ӱ�������
		 * @param pSQL �ύ���е�sql���
		 * @throws CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
		 */
		int executeUpdate(const char* pSQL);
		/**
		 * ִ�е���䲢�����Ƿ�����ɹ�
		 * @param pSQL �ύ���е�sql���
		 * @throws CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
		 */
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

		//
		void setByte(int nParamIndex, char Value);

		//
		void setBytes(int nParamIndex, const char* Value, int nLength);

		//
		//void setBoolean(int nParamIndex, bool Value);

		//
		void setString(int nParamIndex, const std::string& Value);

		//
		void setInt(int nParamIndex, int Value);
		void setUInt(int nParamIndex, unsigned int Value);

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
		void setInt64(int nParamIndex,int64 Value);
		void setUInt64(int nParamIndex,uint64 Value);
		/**
		 * Ԥ����sql
		 */
		void prepare(const char* pSql);
		/**
		 * ��ȡ��ǰ�Ĵ����
		 * @return int �����
		 */
		int getErrNo();
		/**
		 * ��ȡ��ǰ�Ĵ�������
		 * @return char* �ַ����Ĵ�������
		 */
		const char* getError();
		
		/** 
		 * ��ȡsql���
		 * @return sql���
		 * @note ���뿪statement��������󣬷���ָ��ָ����ڴ�ᱻ�ͷ�
		 */
		const char* getSQL() const;
		int getTimeZoneOffset(bool Throw=false);
		void saveBytes(const char* strColumnName, const unsigned char* Value, int nLength,bool SetSequence=false,string SequenceName="",bool SetPkName=false,string PkName="");
		void setBytes(int nParamIndex, const unsigned char* Value, int nLength);
	};

	//�������ü����ķ�ʽ�����ڲ�����
	class CRefStatement : public CStatement
	{
	private :
		//proxyģʽ����
		friend class CPrepareResultSetImpl;
		CStatementImpl* m_pStmtImpl;
	public :
		CRefStatement(CStatementImpl& Stmt);
		~CRefStatement();
		/**
		 * �ر���Դ
		 */
		void close();
		/**
		 * ͳһ�ӿڣ���ȡ�����idֵ
		 * @param pParam �����������mysqlû��
		 * @return int64 ���ص���ֵ
		 */
		uint64 getInsertID(void* pParam);
		/**
		 * ִ�в�ѯSQL�����ؽ����
		 */
		CAutoPtr<CResultSet> executeQuery(const char* pSQL);
		/**
		 * ִ�и��µ���䲢����Ӱ�������
		 * @param pSQL �ύ���е�sql���
		 * @throws CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
		 */
		int executeUpdate(const char* pSQL);
		/**
		 * ִ�е���䲢�����Ƿ�����ɹ�
		 * @param pSQL �ύ���е�sql���
		 * @throws CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
		 */
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

		//
		void setByte(int nParamIndex, char Value);

		//
		void setBytes(int nParamIndex, const char* Value, int nLength);

		//
		//void setBoolean(int nParamIndex, bool Value);

		//
		void setString(int nParamIndex, const std::string& Value);

		//
		void setInt(int nParamIndex, int Value);
		void setUInt(int nParamIndex, unsigned int Value);

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
		void setInt64(int nParamIndex, int64 Value);
		void setUInt64(int nParamIndex, uint64 Value);
		/**
		 * Ԥ����sql
		 */
		void prepare(const char* pSql);
		/**
		 * ��ȡ��ǰ�Ĵ����
		 * @return int �����
		 */
		int getErrNo();
		/**
		 * ��ȡ��ǰ�Ĵ�������
		 * @return char* �ַ����Ĵ�������
		 */
		const char* getError();
		
		/** 
		 * ��ȡsql���
		 * @return sql���
		 * @note ���뿪statement��������󣬷���ָ��ָ����ڴ�ᱻ�ͷ�
		 */
		const char* getSQL() const;
		int getTimeZoneOffset(bool Throw=false);
		void saveBytes(const char* strColumnName, const unsigned char* Value, int nLength,bool SetSequence=false,string SequenceName="",bool SetPkName=false,string PkName="");
		void setBytes(int nParamIndex, const unsigned char* Value, int nLength);
	};

}// namespace avocado_mysql


#endif
