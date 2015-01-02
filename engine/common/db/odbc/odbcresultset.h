#ifndef _MY_ODBCRESULTSET_H_
#define _MY_ODBCRESULTSET_H_
namespace my_odbc
{
	class COdbcStatement;
	class COdbcCallableStatement;
	class COdbcConnection;
	//��ͨ�����
	class COdbcResultSet : public CResultSet
	{
		friend class my_odbc::COdbcStatement;
		friend class my_odbc::COdbcCallableStatement;
	private:
		COdbcStatement *m_pStmt;
		COdbcCallableStatement *m_pCallStmt;
		HSTMT m_Stmt;//��������STMT
		//
		int m_nFieldCount;
		//�ֶ�����������Ӧmap
		map<string,int> m_Fields;
		//MYSQL_ROW m_Row;
		map<int,odbc_bind *> m_mapResultData;//���ѡ��
	protected :
		/**
		 * ��������Ƿ�Խ�磬����ǣ����׳��쳣
		 */
		void validIndex(int nIndex);
		//��հ󶨵�ֵ
		void clear();
	private:
		/**
		* ���ɽ����
		*/
		void buildResultSet(void);
	public:
		COdbcResultSet(COdbcStatement& stmt);
		COdbcResultSet(COdbcCallableStatement& stmt);
		//���캯��

		//��������
		~COdbcResultSet();

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
		
		EDATATYPE getColumnDataType(int nIndex);
		/**
		 *
		 */
		string getColumnName(int nIndex) const;

		/**
		 * ������ƶ�����һ��
		 * �����Ƿ�����һ��
		 */
		bool next();

		// �������������е�����ID
		// ����Ҳ����򷵻�-1
		//
		virtual int findColumn(const char* pColumnName) const;

		/**
		 *
		 */
		virtual char getByte(int nIndex,bool bNullThrow = true,char Default = 0) const;

		/**
		 *
		 */
		virtual char getByte(const char* pColumnName,bool bNullThrow = true,char Default = 0) const;

		/**
		 *
		 */
		virtual int getBytes(int nIndex,unsigned char* pOutBuffer,
					int nBuffSize,int nCharCount,bool bNullThrow = true,
					const char* pDefault = DEFAULT_STRING) const;

		/**
		 *
		 */
		virtual int getBytes(const char* pColumnName,unsigned char* pOutBuffer,
					int nBuffSize,int nCharCount,bool bNullThrow = true,
					const char* pDefault = DEFAULT_STRING) const;
		
		//
		virtual bool getBoolean(int nIndex) const;

		//
		virtual bool getBoolean(const char* pColumn) const;
		
		/**
		 *
		 */
		virtual string getString(int nIndex,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING) const;

		/**
		 *
		 */
		virtual string getString(const char* pColumn,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING) const;

		/**
		 *
		 */
		virtual int getInt(int nIndex,bool bNulLThrow = true,int nDefault= DEFAULT_INT) const;

		/**
		 *
		 */
		virtual int getInt(const char* pColumn,bool bNulLThrow = true,int nDefault= DEFAULT_INT) const;

		/**
		 *
		 */
		virtual double getDouble(int nIndex,bool bNulLThrow = true,double dDefault= DEFAULT_DOUBLE) const;

		/**
		 *
		 */
		virtual double getDouble(const char* pColumn,bool bNulLThrow = true,double dDefault= DEFAULT_DOUBLE) const;

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
		long64 getLong64(int nIndex,bool bNullThrow = true,long64 nDefault = DEFAULT_LONG64) const;

		/**
		 *
		 */
		long64 getLong64(const char* pParamName,bool bNullThrow = true,long64 nDefault = DEFAULT_LONG64) const;
	};
}
#endif