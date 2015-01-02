#ifndef _DBINTERFACE_H_
#define _DBINTERFACE_H_

#include <db/dbpublic.h>

//������
#include <db/resultset.h>
#include <db/statement.h>
#include <db/callablestatement.h>
#include <db/connection.h>
#include <db/sqlexception.h>

//���ӹ���
#include <db/connectionmanager.h>

//���ӳ�
#include <db/poolconnautoptr.h>
#include <db/connectionpool.h>
#include <db/poolconnection.h>

typedef CAutoPtr<CStatement> CStatementPtr;
typedef CAutoPtr<CConnection> CConnectionPtr;
typedef CAutoPtr<CResultSet> CResultSetPtr;
typedef CPoolConnAutoPtr<CPoolConnection> CPoolConnectionPtr;

#endif
