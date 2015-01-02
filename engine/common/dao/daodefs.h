#ifndef _DAODEFS_H_
#define _DAODEFS_H_


enum PARAM_OPERMODE
{
	OPER_EQUAL= 0x01,
	OPER_NOEQL,
	OPER_LT,
	OPER_LET,
	OPER_BT,
	OPER_BET,
	OPER_IN,
	OPER_BETWEEN,
	OPER_LIKE,
	OPER_UNKNOW
};

enum VALUE_TYPE
{
	VALUE_STRING,
	VALUE_NUMBER,
	VALUE_PARAM,
};

enum ORDER_MODE
{
	ORDER_ASC,
	ORDER_DESC
};

enum EXPRESS_JOINMODE
{
	JOIN_AND = 1,
	JOIN_OR,
};

//��������
enum TRANSACTION_TYPE
{
	//����
	TRANSACTION_SYNC = 0x0001,
	//���ݿ�
	TRANSACTION_DB_ONLY = 0x0002,
	//���ݿ�
	TRANSACTION_CACHE_ONLY = 0x0003,
};
//���ݿ����ӵ�����
enum DBCONN_TYPE
{
	//����
	DBCONN_MASTER = 0,
	//�ӿ�
	DBCONN_SLAVE = 1
};

//��ǰ�����ݿ⻷��
enum DBENV
{
	//�����ڴ����ݿ� -- Ĭ��
	DBENV_MDB = 0,
	//����dbms���ݿ�
	DBENV_DB,
	//�����ӿ�����ݿ�
	DBENV_SDB,
	DBENV_END
};

//
enum EDAOACTION
{
	ACTION_UNKNOW = 0,
	ACTION_UPDATE,
	ACTION_DELETE,
	ACTION_INSERT,
	ACTION_END
};

#endif
