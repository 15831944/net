#ifndef _DAODATABASE_H_
#define _DAODATABASE_H_
#include "dao.h"
#include "daoimpl.h"

class CDaoDatabase
{
protected  :
	//�̱߳��ر���
	CTlObject m_LocalTransaction;
	CTlObject m_LocalDBEnv;
public :
	virtual ~CDaoDatabase();
	/**
	 * ��������
	 * @param TransTye �������ͣ����ö��ֵ����daodefs.h
	 */
	virtual CDaoTransactionImpl* createTransaction(TRANSACTION_TYPE TransType) = 0;
	/**
	 * ��ȡ��ǰ������
	 */
	CDaoTransactionImpl* getLocalTransaction();
	/**
	 * ���õ�ǰ�̵߳����񣭣����ؾɵ��߳�����
	 */
	CDaoTransactionImpl* setLocalTransaction(CDaoTransactionImpl* pTransaction);
	/**
	 * �������ݿ⻷��
	 * @param EnvType Ҫ���õ����ݿ����ͣ����ö��ֵ����daodefs.h
	 */
	void setLocalDBEnv(DBENV EnvType);
	/**
	 * ��ȡ��ǰ�����ݿ�����
	 * @return DBENV ���ݿ�����
	 */
	DBENV getLocalDBEnv();
	/**
	 * ��ʼ����Ҫ����Դ
	 */
	bool initialize();

	/**
	 * ��ȡ���ݿ�ʵ��
	 * @return ���ݿ�ʵ��
	 */
	static CDaoDatabase& getInstanceImpl();
};

#endif
