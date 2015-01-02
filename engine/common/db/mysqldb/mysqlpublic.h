#if !defined(_MYSQLPUBLIC_H_)
#define _MYSQLPUBLIC_H_

#include "../../public.h"
#include "../../exception/myException.h"
#include "../../util/datetime.h"
#include <mysql.h>
#include "metadata.h"

//����ͨ����ת�������ݿ�����
void convert(MYSQL_TIME& SQLTime,const CDateTime& DateTime);
//�����ݿ�����ת������ͨ����
void convert(CDateTime& DateTime,const MYSQL_TIME& SQLTime);
//��ȡ���ݿ�����ͱ�ʶ
enum_field_types getDBType(EDATATYPE nDataType);
//��ȡ��ͨ�����ͱ�ʶ
EDATATYPE getDataType(enum_field_types nSqlType);

#endif
