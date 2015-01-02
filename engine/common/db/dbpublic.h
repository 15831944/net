#ifndef _DBPUBLIC_H_
#define _DBPUBLIC_H_

//������붨��,�׳�CSQLExceptionʱ��ʹ�����´�����errcode

#include "../public.h"

#ifdef WIN32
#pragma warning(disable:4150) // Warning ****************
#endif
#include "../util/autoptr.h"
#include "../exception/myException.h"
#include "../util/datetime.h"
#include "sqlexception.h"

enum DATABSE_ERROR
{
	//�Ƿ�����
	ERROR_DATABASE_INVALIDOPER = 0x00010001,
	//��֧�ֵ�ǰ�ķ���
	ERROR_DATABASE_UNSUPPORT = 0x00010002, 
	//����Э����Ч
	ERROR_DATABASE_INVALIDPROTOCOL = 0x00010003,
	//�������ʹ���
	ERROR_DATABASE_INVALIDDATATYPE = 0x00010004, 
	//����Խ��
	ERROR_DATABASE_INDEXOUT = 0x00010005, 
	//���ݿ��ѹر�
	ERROR_DATABASE_CLOSED = 0x00010006, 
	//��������
	ERROR_DATABASE_COMMON = 0x00010007, 
	//��ͼ��ȡ��ֵ���ֶ�
	ERROR_DATABASE_NULLVALUE = 0x00010008,
	//�������ݿ����Ӳ��ɹ�
	ERROR_DATABASE_NOTCONNECT = 0x00010009,
	//���ݿ����ӳ���
	ERROR_DATABASE_POOLFULL = 0x00010010,
	//���´�����ֶ�ǰ��δ��statementִ��executequery
	ERROR_DATABASE_LOBUPDATE = 0x00010011,
	//��ȡ�����ʱ���������
	ERROR_GETBYTES_BUFFOVERFLOW = 0x00010012,
};

#ifdef _ORACLE_
	#define _SYSDATE_ "sysdate"
#elif _MYSQL_
	#define _SYSDATE_ "sysdate()"
#else
	#define _SYSDATE_ "sysdate()"
#endif

#endif

