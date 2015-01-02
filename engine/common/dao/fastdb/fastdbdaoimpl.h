#ifndef _FASTDB_DAOIMPL_H_
#define _FASTDB_DAOIMPL_H_
#include <public.h>
#include <dao/daoimpl.h>
#include <dao/daodatabase.h>
#include <dao/daoparse.h>
#include <fastdb.h>
#include <string>
#include <vector>
#include <util/singleton.h>

const char* getJoinStr(int JoinMode);
const char* getCompStr(PARAM_OPERMODE OperMode);

namespace dao_fastdb
{
typedef struct _t_fdbparam
{
	string sql;
	dao_bind_param* Param;
}fdbparam;

class CFastDBDaoQueryImpl
{
protected :
	vector<fdbparam> m_Conditions;
	vector<dao_bind_param*> m_Params;
	bool m_isCompiled;
	string m_Order;
	dbQuery m_Query;
	//
	bool m_isFastDBCompiled;

protected :
	dao_bind_param* getBindParam(int nIndex);
public :
	CFastDBDaoQueryImpl();
	virtual ~CFastDBDaoQueryImpl();
	/**
	 * ��������
	 */
	void compile(const dao_statement& Statement,const char* pSQL);
	//���ò���
	void setInt(int Index,int32 nValue);
	void setInt64(int Index,int64 nValue);
	void setInt16(int Index,int16 nValue);
	void setString(int Index,const string& Value);
	void setDateTime(int Index,const CDateTime& Value);
	void setBytes(int Index,const void* pValue,int nLength);
	void setUInt8(int Index,uint8 Value);
	void setUInt16(int Index,uint16 Value);
	void setUInt32(int Index,uint32 Value);
	void setUInt64(int Index,uint64 Value);
	/**
	 * ��ȡ�󶨲����ĸ���
	 */
	int32 getBindParamCount();
	dbQuery& getQuery();
	void clear();
	void initBindParam(dao_expression& Expression);
};

class CFastDBDaoImpl : public CMDBDaoImpl
{
protected :
	const char* m_Name;
	CFastDBDaoQueryImpl* m_pQueryImpl;

public :
	CFastDBDaoImpl();
	virtual ~CFastDBDaoImpl() = 0;
	/**
	 * ���ز�ѯ������
	 */
	virtual int select(dbQuery& Query) = 0;
	int findMDB();
	bool prepareMDB(const char* sql);
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
	void clear();
};

class CFastDBDaoDatabaseImpl;

class CFastDBDaoTransactionImpl : public CMDBDaoTransactionImpl
{
protected :
	int m_nSessionID;
	bool m_bHasBegin;
	TRANSACTION_TYPE m_TransType;
public :
	CFastDBDaoTransactionImpl(CFastDBDaoDatabaseImpl &pDatabase);
	~CFastDBDaoTransactionImpl();
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
	void update(CMDBDaoImpl* pDao);
	void insert(CMDBDaoImpl* pDao);
	void remove(CMDBDaoImpl* pDao);
};

class CFastDBDaoDatabaseImpl
{
public :
	//dbDatabase m_DB;
	int m_nSessionID;
	bool m_bIsCloseDB;
public :
	CFastDBDaoDatabaseImpl();
	~CFastDBDaoDatabaseImpl();
	//��ȡָ��������
	CMDBDaoTransactionImpl* createTransaction(TRANSACTION_TYPE TransType);
	/**
	 * ��ʼ���ڴ����ݿ⣬���ԭ�е����ݶ�ɾ��
	 */
	bool init(const char* DBName,int64 InitSize=1024,int64 MaxSize=2048,int64 IndexSize=1024,int64 IncreSize=20);
    /**
	 * ��һ���ڴ����ݿ⣬����ɾ��ԭ�е�����
	 */
    bool open(const char* DBName,int64 InitSize=1024,int64 MaxSize=2048,int64 IndexSize=1024,int64 IncreSize=20);
    /**
     *@���FastDB�ڴ�
     */
    void cleanup( const char* DBName );

protected:
    /**
     *@
     */
    int hashFunction(char const* s);
    /**
     *@
     */
    int getKey(char const* databaseName, char const* suffix);
    /**
     *@
     */
    void removeSemaphore(char const* databaseName, char const* suffix);
    /**
     *@
     */
    void removeSharedMemory(char const* databaseName, char const* suffix);
};

typedef struct _t_compliedquery
{
	CFastDBDaoQueryImpl* query;
	uint32 usedtimes;

	_t_compliedquery()
	{
		memset(this,0,sizeof(struct _t_compliedquery));
	}

	_t_compliedquery(const struct _t_compliedquery &Other)
	{
		memcpy(this,&Other,sizeof(struct _t_compliedquery));
		((struct _t_compliedquery*)&Other)->query = NULL;
	}

	~_t_compliedquery()
	{
		if ( query != NULL )
			delete query;
	}
}compliedquery;
/*
Ԥ���������
*/
class CCompiledQueryManager : public CSingleton<CCompiledQueryManager>
{
private :
	friend class CSingleton<CCompiledQueryManager>;
	CCompiledQueryManager();
	map<string,compliedquery> m_CompiledQuerys;
public :
	bool getPreCompiledData(const string& SQL,const string& TableName,CFastDBDaoQueryImpl*& pQuery);
	bool setPreCompiledData(const string& SQL,const string& TableName,CFastDBDaoQueryImpl* pQuery);
};

}

#endif
