#ifndef _CRESULTSETIMPL_H_
#define _CRESULTSETIMPL_H_

#include <db/resultset.h>

// ��ѯ���صĽ����
namespace avocado_oracle
{
	class CConnectionImpl;
	class CStatementImpl;
	class CResultSetImpl : public CResultSet
	{
	private :
		//�ֶ�����������Ӧmap
		mutable map<string,int> m_Fields;

	friend class CStatementImpl;	
	friend class CCallableStatementImpl;
	protected:
		CResultSetImpl();
		CRefStatement* m_pRefStmt;
		CStatementImpl* m_pStmt;
		CConnectionImpl* m_pConn;
		//
		oracle::occi::ResultSet* m_pSet;

	private :
		/**
		 * ��ȡ���ؽ�������ֶζ�Ӧ����������
		 * @return bool �Ƿ�ɹ�
		 */
		bool getFields() const;
	public:
		//���캯��

		//��������
		~CResultSetImpl();
		

		// �رս������
		// �ͷ�������Դ
		//
		void close();

		//
		int getColumnCount() const;

		EDATATYPE getColumnDataType(int nIndex);

		//
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
		int getBytes(int nIndex,unsigned char* pOutBuffer,int nBuffSize,int nCharCount,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING,unsigned int nOffset = 1) const;

		//
		int getBytes(const char* pColumnName,unsigned char* pOutBuffer,int nBuffSize,int nCharCount,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING,unsigned int nOffset = 1) const;

		//
		//bool getBoolean(int nIndex);

		//
		//bool getBoolean(const char* pColumn);

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
		Long64 getLong64(int nIndex,bool bNullThrow = true,Long64 nDefault = DEFAULT_LONG64) const;

		//
		Long64 getLong64(const char* pParamName,bool bNullThrow = true,Long64 nDefault = DEFAULT_LONG64) const;

	};
}

#endif 
