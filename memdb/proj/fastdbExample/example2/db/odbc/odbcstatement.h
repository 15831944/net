#ifndef _MY_ODBCSTATEMENT_H_
#define _MY_ODBCSTATEMENT_H_
#include "odbcconnection.h"
namespace my_odbc
{
	class COdbcConnection;
	class COdbcStatement : public CStatement
	{
	protected:
		//���Ӵ���ʵ��
		COdbcConnection* m_pConn;
		//sql�����
		std::string m_strSql;
		//mysql��preparedstatement�ľ��
		HSTMT m_pStmt;

		map<int,odbc_bind *> m_mapParams;//���ѡ��
		//�ܲ�������
		int m_nParamCount;

	protected :
		/**
		 * ����Ԥ�ύ��䣬���ò�������
		 */
		virtual void prepareParams(){};
		/**
		 * ����������Ƿ�Ϸ�
		 * @param nIndex int ������
		 */
		virtual void validIndex(int nIndex);

	public:
		/**
		 * ���캯��
		 */
		COdbcStatement(COdbcConnection& Conn);
		/**
		 * ��������
		 */
		~COdbcStatement();

		virtual int getTimeZoneOffset(bool Throw=false);

		virtual const char* getSQL() const;
		/**
		 * ��ȡmysql auto_increment�Զ��������Զ�����ֵ
		 * @return ULong64 ���ص�ֵ
		 */
		virtual long64 getLastInsertID();
		//*********************CStatement�Ľӿں���*******************************
		/**
		 * �ر���Դ
		 */
		virtual void close();
		/**
		 * ͳһ�ӿڣ���ȡ�����idֵ
		 * @param pParam �����������mysqlû��
		 * @return Long64 ���ص���ֵ
		 */
		virtual long64 getInsertID(void* pParam);
		/**
		 * ִ�в�ѯSQL�����ؽ����
		 */
		virtual CAutoPtr<CResultSet> executeQuery(const char* pSQL);
		/**
		 * ִ�и��µ���䲢����Ӱ�������
		 * @param pSQL �ύ���е�sql���
		 * @throws CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
		 */
		virtual int executeUpdate(const char* pSQL);
		/**
		 * ִ�е���䲢�����Ƿ�����ɹ�
		 * @param pSQL �ύ���е�sql���
		 * @throws CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
		 */
		virtual bool execute(const char* pSQL);
		// ִ�е�ǰ��SQL���
		//
		virtual bool execute();
		// ִ�е�ǰ��ѯ
		//
		virtual CAutoPtr<CResultSet> executeQuery();

		// ִ�е�ǰ�ĸ���SQL����
		// ����ִ��Ӱ���������
		//
		virtual int executeUpdate();

		// ������в�����ֵ
		//
		virtual void clearParameters();

		//
		virtual void saveBytes(const char* strColumnName, const unsigned char* Value,
			int nLength,bool SetSequence=false,string SequenceName="",
			bool SetPkName=false,string PkName="");
		//

		virtual void setByte(int nParamIndex, char Value);
		//
		virtual void setBytes(int nParamIndex, const unsigned char* Value, int nLength);

		//
		virtual void setBoolean(int nParamIndex, bool Value);

		//
		virtual void setString(int nParamIndex, const std::string& Value);

		//
		virtual void setInt(int nParamIndex, int Value);

		//
		virtual void setDouble(int nParamIndex, double Value);

		//
		virtual void setNull(int nParamIndex, EDATATYPE nEDATATYPE);

		//
		virtual void setDate(int nParamIndex, CDateTime Value);

		//
		virtual void setTime(int nParamIndex, CDateTime Value);

		//
		virtual void setTimestamp(int nParamIndex, CDateTime Value);

		//
		virtual void setLong64(int nParamIndex, long64 Value); 	 
		/**
		 * Ԥ����sql
		 */
		virtual void prepare(const char* pSql);
		/**
		 * ��ȡ��ǰ�Ĵ����
		 * @return int �����
		 */
		virtual int getErrNo();
		/**
		 * ��ȡ��ǰ�Ĵ�������
		 * @return char* �ַ����Ĵ�������
		 */
		virtual const char* getError();


		HSTMT getStmt() const;

		void clearResultSet();
	private:
		//д������
		void writeBinaryData();
		COdbcResultSet *m_pUsedResultSet;
	};
}
#endif