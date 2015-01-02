#ifndef _MYSQL_DAOIMPL_H_
#define _MYSQL_DAOIMPL_H_

#include <public.h>
#include <db/dbinterface.h>
#include <dao/daoimpl.h>

namespace dao_mysql
{
class CMysqlDaoImpl : public CDBDaoImpl
{
protected :
	//��ѯ��ϸ��¼
	CStatementPtr m_QueryStmt;
	//��ѯ������
	CStatementPtr m_QueryCountStmt;
	CResultSetPtr m_Resultset;
	CPoolConnectionPtr m_Conn;
	//������Ĭ���ڹ��캯����m_TableName = "tb_table"
	const char* m_TableName;

public :
	CMysqlDaoImpl();
	virtual ~CMysqlDaoImpl();
	/**
	 * ��������ѯ
	 * @return int ���ؽ����������
	 */
	bool prepareDB(const char* Sql,CPoolConnectionPtr& Conn);
	//
	void setQInt(int Index,int32 nValue);
	void setQInt64(int Index,int64 nValue);
	void setQInt16(int Index,int16 nValue);
	void setQString(int Index,const string& Value);
	void setQDateTime(int Index,const CDateTime& Value);
	void setQBytes(int Index,const void* pValue,int nLength);
	void setQUInt8(int Index,uint8 Value);
	void setQUInt16(int Index,uint16 Value);
	void setQUInt32(int Index,uint32 Value);
	void setQUInt64(int Index,uint64 Value);
	int findDB();
	bool nextDB();
	bool compare(const CDBDaoImpl* pOther) const;
	//----���������ʵ�ֵĺ���----
	//���ݿ���صĲ�������
	virtual void readDB(CResultSet& Set) = 0;
	/**
	 * ����Ƿ���ͬ�����ݿ��¼
	 * @return bool �������ͬ�����ݿ⣬�򷵻�true
	 */
	virtual bool check(const CMysqlDaoImpl* pOther) const = 0;
};

class CMysqlDBDaoTransactionImpl : public CDBDaoTransactionImpl
{
protected :
	//���ݿ����ӳ�
	CConnectionPool* m_pConnPool;
	//���µ�����
	CPoolConnectionPtr m_pUpdateConn;

public :
	CMysqlDBDaoTransactionImpl(CConnectionPool& ConnPool);
	~CMysqlDBDaoTransactionImpl();
	/**
	 * ����dao����
	 */
	bool begin();
	/**
	 * �ύdao����
	 */
	bool commit();
	/**
	 * �ع�dao����
	 */
	bool rollback();
	/**
	 */
	void update(CDBDaoImpl* pDao);
	void insert(CDBDaoImpl* pDao);
	void remove(CDBDaoImpl* pDao);
	void prepare(const char* Sql,CDBDaoImpl* pDao);
};

}

#endif
