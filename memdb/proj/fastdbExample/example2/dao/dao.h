#ifndef _DAO_H_
#define _DAO_H_
#include "public.h"
#include "userexception.h"
#include "datetime.h"
#include <list>
using namespace std;
#include "daodefs.h"
#include "daoimpl.h"
#include "tlobject.h"
#include "dbinterface.h"

class CDaoTransactionImpl;

/*
  ���ݷ��ʶ���
  ��ѯ����
  ֧��name=? and param=? �ķ�ʽ
*/
class CDao
{
protected :
	CDaoTransactionImpl* m_pTranImpl;
	IDaoQuery* m_pQuery;
	int m_nRowCount;

public :
	CDao();
	virtual ~CDao();
	bool find();
	bool next();
	bool update();
	bool insert();
	bool remove();
	bool prepare(const char* sql);
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
	 * ��ȡ����
	 */
	int getRowCount() const;

	/**
	 * ��ȡ��Ӧ���ڴ�daoʵ��
	 * @return ��Ӧ��ʵ��ָ�룬����������CDao����
	 */
	virtual CMDBDaoImpl* getMDBDao() = 0 ;
	/**
	 * ��ȡ��Ӧ�ĳ־����ݿ�daoʵ��
	 * @return ��Ӧ��ʵ��ָ�룬����������CDao����
	 */
	virtual CDBDaoImpl* getDBDao() = 0;
};

class CDaoException : public CCException
{
public :
	CDaoException(const string& Message);
	virtual ~CDaoException() throw();
};

class CDaoTransaction
{
protected :
	//��ǰ��������
	CDaoTransactionImpl* m_pTranImpl;
	//��һ������
	CDaoTransactionImpl* m_pOldTranImpl;
	//��ʶ�Ƿ�������
	bool m_bHasBegin;

public :
	//Ĭ������cached������
	CDaoTransaction(TRANSACTION_TYPE TransType = TRANSACTION_SYNC);
	//
	~CDaoTransaction();
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
};

class CDaoTransactionImpl
{
protected :
	//��������
	TRANSACTION_TYPE m_TransType;

public :
	/**
	 * dao����Ĺ��캯��
	 * @param TransType �������ͣ�Ĭ��Ϊͬ��������
	 */
	CDaoTransactionImpl(TRANSACTION_TYPE TransType = TRANSACTION_SYNC);
	//
	virtual ~CDaoTransactionImpl();
	/**
	 * ����dao����
	 */
	virtual bool begin() = 0;
	/**
	 * �ύdao����
	 */
	virtual bool commit() = 0;
	/**
	 * �ع�dao����
	 */
	virtual bool rollback() = 0;
	virtual bool update(CDao* pDao) = 0;
	virtual bool insert(CDao* pDao) = 0;
	virtual bool remove(CDao* pDao) = 0;
	virtual IDaoQuery* prepare(const char* Sql,CDao* pDao) = 0;
	/**
	 * ����Ӱ������
	 */
	virtual int find(CDao* pDao) = 0;
	virtual bool next(CDao* pDao) = 0;
};

#endif

