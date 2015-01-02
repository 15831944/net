#ifndef _CRESULTSETIMPL_H_
#define _CRESULTSETIMPL_H_

#include <db/resultset.h>
#include "statementimpl.h"

// ��ѯ���صĽ����
namespace mysql
{
	class CStatementImpl;
	class CCallableStatementImpl;
	//��ͨ�����
	class CResultSetImpl : public CResultSet
	{
	protected :
		//
		MYSQL_RES* m_pMetaData;
		//
		CConnectionImpl* m_pConn;
		//
		int m_nFieldCount;
		//�ֶ�����������Ӧmap
		map<string,int> m_Fields;
		//����ֶμ�
		CResultData* m_pResultData;
		//���������
		MYSQL_ROW m_Row;
	protected :
		/**
		 * ��������Ƿ�Խ�磬����ǣ����׳��쳣
		 */
		void validIndex(int nIndex) const;
	public:
		CResultSetImpl(CConnectionImpl& Conn);
		//���캯��

		//��������
		~CResultSetImpl();

		// �رս������
		// �ͷ�������Դ
		//
		void close();
		/**
		 * ��ʼ������
		 * @return bool ��ʼ���ɹ���ʶ
		 */
		bool init();

		/**
		 *
		 */
		int getColumnCount() const;
		
		/**
		 *
		 */
		EDATATYPE getColumnDataType(int nIndex);

		/**
		 *
		 */
		std::string getColumnName(int nIndex) const;

		/**
		 * ������ƶ�����һ��
		 * �����Ƿ�����һ��
		 */
		bool next();

		// �������������е�����ID
		// ����Ҳ����򷵻�-1
		//
		int findColumn(const char* pColumnName) const;

		/**
		 *
		 */
		char getByte(int nIndex,bool bNullThrow = true,char Default = 0) const;

		/**
		 *
		 */
		char getByte(const char* pColumnName,bool bNullThrow = true,char Default = 0) const;

		/*
		//
		bool getBoolean(int nIndex);

		//
		bool getBoolean(const char* pColumn);
		*/
		/**
		 *
		 */
		std::string getString(int nIndex,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING) const;

		/**
		 *
		 */
		std::string getString(const char* pColumn,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING) const;

		/**
		 *
		 */
		int getInt(int nIndex,bool bNulLThrow = true,int nDefault= DEFAULT_INT) const;

		/**
		 *
		 */
		int getInt(const char* pColumn,bool bNulLThrow = true,int nDefault= DEFAULT_INT) const;

		/**
		 *
		 */
		double getDouble(int nIndex,bool bNulLThrow = true,double dDefault= DEFAULT_DOUBLE) const;

		/**
		 *
		 */
		double getDouble(const char* pColumn,bool bNulLThrow = true,double dDefault= DEFAULT_DOUBLE) const;

		/**
		 *
		 */
		bool isNull(int nIndex) const;

		/**
		 *
		 */
		bool isNull(const char* pColumn) const;

		/**
		 *
		 */
		CDateTime getDate(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		/**
		 *
		 */
		CDateTime getDate(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		/**
		 *
		 */
		CDateTime getTime(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		/**
		 *
		 */
		CDateTime getTime(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		/**
		 *
		 */
		CDateTime getTimestamp(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		/**
		 *
		 */
		CDateTime getTimestamp(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		/**
		 *
		 */
		int64 getInt64(int nIndex,bool bNullThrow = true,int64 nDefault = DEFAULT_LONG64) const;

		/**
		 *
		 */
		int64 getInt64(const char* pParamName,bool bNullThrow = true,int64 nDefault = DEFAULT_LONG64) const;
		int getBytes(int nIndex,unsigned char* pOutBuffer,int nBuffSize,int nCharCount,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING,unsigned int nOffset = 1) const;
		int getBytes(const char* pColumnName,unsigned char* pOutBuffer,int nBuffSize,int nCharCount,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING,unsigned int nOffset = 1) const;
	};
	/*
	  preparestatement�Ľ����
	*/
	class CPrepareResultSetImpl : public CResultSet
	{
	protected :
		MYSQL_RES* m_pMetaData;
		CRefStatement* m_pStmt;
		//����ֶμ�
		CResultData* m_pResultData;
		//����ֶμ�
		MYSQL_BIND* m_pResult;
		//�ֶ�����
		int m_nFieldCount;
		//�ֶ�����������Ӧmap
		map<string,int> m_Fields;

	protected :
		/**
		 * ��������Ƿ�Խ�磬����ǣ����׳��쳣
		 */
		void validIndex(int nIndex) const;
		/**
		 * �������ֵ
		 */
		void cleanParam(MYSQL_BIND& pBind);
	public:
		//���캯��
		CPrepareResultSetImpl(CStatementImpl& Stmt);
		//��������
		~CPrepareResultSetImpl();
		/**
		 * ��ʼ���������Դ
		 */
		bool init();
		// �رս������
		// �ͷ�������Դ
		//
		void close();

		/**
		 *
		 */
		int getColumnCount() const;
		/**
		 *
		 */
		EDATATYPE getColumnDataType(int nIndex);

		/**
		 *
		 */
		std::string getColumnName(int nIndex) const;

		// ������ƶ�����һ��
		// �����Ƿ�����һ��
		//
		bool next();

		// �������������е�����ID
		// ����Ҳ����򷵻�-1
		//
		int findColumn(const char* pColumnName) const;

		//
		char getByte(int nIndex,bool bNullThrow = true,char Default = 0) const;

		//
		char getByte(const char* pColumnName,bool bNullThrow = true,char Default = 0) const;

		/*
		//
		bool getBoolean(int nIndex);

		//
		bool getBoolean(const char* pColumn);
		*/
		//
		std::string getString(int nIndex,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING) const;

		//
		std::string getString(const char* pColumn,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING) const;

		//
		int getInt(int nIndex,bool bNulLThrow = true,int nDefault= DEFAULT_INT) const;

		//
		int getInt(const char* pColumn,bool bNulLThrow = true,int nDefault= DEFAULT_INT) const;

		//
		double getDouble(int nIndex,bool bNulLThrow = true,double dDefault= DEFAULT_DOUBLE) const;

		//
		double getDouble(const char* pColumn,bool bNulLThrow = true,double dDefault= DEFAULT_DOUBLE) const;

		//
		bool isNull(int nIndex) const;

		//
		bool isNull(const char* pColumn) const;

		//
		CDateTime getDate(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		//
		CDateTime getDate(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		//
		CDateTime getTime(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		//
		CDateTime getTime(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		//
		CDateTime getTimestamp(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		//
		CDateTime getTimestamp(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const;

		//
		int64 getInt64(int nIndex,bool bNullThrow = true,int64 nDefault = DEFAULT_LONG64) const;

		//
		int64 getInt64(const char* pParamName,bool bNullThrow = true,int64 nDefault = DEFAULT_LONG64) const;
		int getBytes(int nIndex,unsigned char* pOutBuffer,int nBuffSize,int nCharCount,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING,unsigned int nOffset = 1) const;
	    int getBytes(const char* pColumnName,unsigned char* pOutBuffer,int nBuffSize,int nCharCount,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING,unsigned int nOffset = 1) const;
	};
}

#endif
