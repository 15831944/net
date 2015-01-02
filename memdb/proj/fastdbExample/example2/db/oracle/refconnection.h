#ifndef _REFCONNECTION_H_
#define _REFCONNECTION_H_

#include <util/refobject.h>

class oracle::occi::Statement;
namespace avocado_oracle
{
/**
 * ����connection
 * ���ౣ��occi::Connection��ָ��
 * ��ά��occi::Connection���������ڣ�ֻ�������ö��������occi::Connection�Żᱻ�ر�
 */
class CRefConnection:public CRefObject
{
private:
	oracle::occi::Environment* m_pEnv;
	oracle::occi::Connection* m_pConn;		
public:
	//���캯��
	CRefConnection(oracle::occi::Environment* pEnv,oracle::occi::Connection* pConn);
	

	//��������
	virtual ~CRefConnection();
};
}

#endif
