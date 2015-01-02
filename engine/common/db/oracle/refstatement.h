#ifndef _REFSTATEMENT_H_
#define _REFSTATEMENT_H_

#include <util/refobject.h>

namespace avocado_oracle
{
class CRefConnection;
/**
 * ����statemnet
 * ���ౣ��occi::Statement��ָ��
 * ��ά��occi::Statement���������ڣ�ֻ�������ö��������occi::Statement�Żᱻ�ر�
 */
class CRefStatement:public CRefObject
{
	//��CResultSetImpl����ʱ����Ҫ��CStatementImpl��ָ���Լ���ָ����գ����ԣ���������Ҫ����m_pStmt
	friend class CResultSetImpl;
private:	
	oracle::occi::Connection* m_pConn;	
	oracle::occi::Statement* m_pStmt;
	//CRefConnection* m_pRefConn;

	void close();
public:
	//���캯��
	CRefStatement(oracle::occi::Connection* pConn,oracle::occi::Statement* pStmt/*,CRefConnection* pRefConn*/);
	
	//��������
	virtual ~CRefStatement();

	//�������ø���
	unsigned long incCount();
	
	//�������ø���
	bool decCount();	
};
}

#endif