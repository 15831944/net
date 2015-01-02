#ifndef _DAOIMPL_H_
#define _DAOIMPL_H_
#include "../db/dbinterface.h"
#include "../util/userexception.h"
#include "../util/datetime.h"

/*
��ѯ��������
*/
class IDaoQuery
{
public :
	virtual ~IDaoQuery() {}
	virtual void setQInt(int Index,int32 nValue) = 0;
	virtual void setQInt64(int Index,int64 nValue) = 0;
	virtual void setQInt16(int Index,int16 nValue) = 0;
	virtual void setQString(int Index,const string& Value) = 0;
	virtual void setQDateTime(int Index,const CDateTime& Value) = 0;
	virtual void setQBytes(int Index,const void* pValue,int nLength) = 0;
	virtual void setQUInt8(int Index,uint8 Value) = 0;
	virtual void setQUInt16(int Index,uint16 Value) = 0;
	virtual void setQUInt32(int Index,uint32 Value) = 0;
	virtual void setQUInt64(int Index,uint64 Value) = 0;
};

/*
���ݿ��Dao
*/
class CDBDaoImpl : public IDaoQuery
{
public :
	virtual ~CDBDaoImpl() { }
	/**
	 * ��������ѯ
	 */
	virtual bool prepareDB(const char* sql,CPoolConnectionPtr& Conn) = 0;
	//
	virtual void setQInt(int Index,int32 nValue) = 0;
	virtual void setQInt64(int Index,int64 nValue) = 0;
	virtual void setQInt16(int Index,int16 nValue) = 0;
	virtual void setQString(int Index,const string& Value) = 0;
	virtual void setQDateTime(int Index,const CDateTime& Value) = 0;
	virtual void setQBytes(int Index,const void* pValue,int nLength) = 0;
	virtual void setQUInt8(int Index,uint8 Value) = 0;
	virtual void setQUInt16(int Index,uint16 Value) = 0;
	virtual void setQUInt32(int Index,uint32 Value) = 0;
	virtual void setQUInt64(int Index,uint64 Value) = 0;
	//�ȽϺ����������ж��Ƿ���ͬ�Ķ���
	virtual bool compare(const CDBDaoImpl* pOther) const = 0;
	//���ݿ���صĲ�������
	virtual bool updateDB(CConnection& Conn) = 0;
	virtual bool insertDB(CConnection& Conn) = 0;
	virtual bool removeDB(CConnection& Conn) = 0;
	/**
	 * ִ�в�ѯ
	 * @return int ���ؽ����������
	 */
	virtual int findDB() = 0;
	virtual bool nextDB() = 0;
	virtual CDBDaoImpl* clone() = 0;
	//����SQL
	virtual string generateInsertSQL() = 0;
	virtual string generateUpdateSQL() = 0;
	virtual string generateRemoveSQL() = 0;
};

class CMDBDaoImpl : public IDaoQuery
{
public :
	virtual ~CMDBDaoImpl() { };
	virtual bool prepareMDB(const char* sql) = 0;
	//
	virtual void setQInt(int Index,int32 nValue) = 0;
	virtual void setQInt64(int Index,int64 nValue) = 0;
	virtual void setQInt16(int Index,int16 nValue) = 0;
	virtual void setQString(int Index,const string& Value) = 0;
	virtual void setQDateTime(int Index,const CDateTime& Value) = 0;
	virtual void setQBytes(int Index,const void* pValue,int nLength) = 0;
	virtual void setQUInt8(int Index,uint8 Value) = 0;
	virtual void setQUInt16(int Index,uint16 Value) = 0;
	virtual void setQUInt32(int Index,uint32 Value) = 0;
	virtual void setQUInt64(int Index,uint64 Value) = 0;
	virtual bool readMDB() = 0;
	/**
	 * ������ִ�в�ѯ
	 * @return int ���ؽ����������
	 */
	virtual int findMDB() = 0;
	virtual bool insertMDB() = 0;
	virtual bool removeMDB() = 0;
	virtual bool nextMDB() = 0;
	virtual bool updateMDB() = 0;
};

//DB����֧��
class CDBDaoTransactionImpl
{
public :
	virtual ~CDBDaoTransactionImpl() { } 
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
	/**
	 * ����
	 */
	virtual void update(CDBDaoImpl* pDao) = 0;
	/**
	 * ����
	 */
	virtual void insert(CDBDaoImpl* pDao) = 0;
	/**
	 * ɾ��
	 */
	virtual void remove(CDBDaoImpl* pDao) = 0;
	/**
	 * Ԥ��ѯ
	 */
	virtual void prepare(const char* Sql,CDBDaoImpl* pDao) = 0;
};


//�ڴ�DB����֧��
class CMDBDaoTransactionImpl
{
public :
	/**
	 *
	 */
	virtual ~CMDBDaoTransactionImpl() { };
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
	/**
	 * ����
	 */
	virtual void update(CMDBDaoImpl* pDao) = 0;
	/**
	 * ����
	 */
	virtual void insert(CMDBDaoImpl* pDao) = 0;
	/**
	 * ɾ��
	 */
	virtual void remove(CMDBDaoImpl* pDao) = 0;
};

#endif
