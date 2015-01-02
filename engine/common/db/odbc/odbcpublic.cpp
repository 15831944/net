#include "odbcpublic.h"
#include "../sqlexception.h"
const int g_AvoOdbcCTypeTable[][2] =
{
	{TYPE_INT,SQL_C_LONG},
	{TYPE_STRING,SQL_C_CHAR},
	{TYPE_DATE,SQL_C_TYPE_TIMESTAMP},
	{TYPE_TIME,SQL_C_TYPE_TIMESTAMP},
	{TYPE_TIMESTAMP,SQL_C_TYPE_TIMESTAMP},
	{TYPE_LONG64,SQL_C_SBIGINT},
	{TYPE_DOUBLE,SQL_C_DOUBLE},
	{TYPE_CHAR,SQL_C_CHAR},
	{TYPE_BLOB,SQL_C_BINARY},
	{-1,-1}
};
const int g_OdbcSqlAvoTable[][2] =
{
	{SQL_INTEGER,TYPE_INT},		//����
	{SQL_VARCHAR,TYPE_STRING},	//��ֵ��
	{SQL_FLOAT,TYPE_DOUBLE},	//�����ȸ�����
	{SQL_BIGINT,TYPE_LONG64},	//������
	{SQL_TYPE_TIMESTAMP,TYPE_DATE}, //������
	{SQL_DOUBLE,TYPE_DOUBLE},	//������
	{SQL_REAL,TYPE_DOUBLE},		//������
	{SQL_DECIMAL,TYPE_DOUBLE},	//��ֵ��
	{SQL_SMALLINT,TYPE_INT},	//С��ֵ��	
	{SQL_CHAR,TYPE_INT},		//�ֽ���
	{SQL_TYPE_DATE,TYPE_DATE},	//������
	{SQL_TYPE_TIME,TYPE_DATE},	//ʱ����
	{SQL_TINYINT,TYPE_INT},		//С��ֵ��
	{SQL_LONGVARCHAR,TYPE_BLOB}, //�������
	{SQL_BINARY,TYPE_BLOB},		//�����
	{-1,-1}
};
const int g_OdbcSQLTypeTable[][2]=
{
	{TYPE_INT,SQL_INTEGER},
	{TYPE_STRING,SQL_VARCHAR},
	{TYPE_DATE,SQL_TYPE_TIMESTAMP},
	{TYPE_TIME,SQL_TYPE_TIMESTAMP},
	{TYPE_TIMESTAMP,SQL_TYPE_TIMESTAMP},
	{TYPE_LONG64,SQL_C_SBIGINT},
	{TYPE_DOUBLE,SQL_C_DOUBLE},
	{TYPE_CHAR,SQL_C_CHAR},
	{TYPE_BLOB,SQL_C_BINARY},
	{-1,-1}
};
//��C++����ת�������ݿ���־
void convert(SQL_TIMESTAMP_STRUCT& SQLTime,const CDateTime& DateTime)
{
	memset(&SQLTime,0,sizeof(SQL_TIMESTAMP_STRUCT));
	SQLTime.year = DateTime.getYear();
	SQLTime.month = DateTime.getMonth();
	SQLTime.day = DateTime.getDay();
	SQLTime.hour = DateTime.getHour();
	SQLTime.minute = DateTime.getMinute();
	SQLTime.second = DateTime.getSecond();
}
//�����ݿ�����ת������ͨ����
void convert(CDateTime& DateTime,const SQL_TIMESTAMP_STRUCT& SQLTime)
{
	char buffer[40];
	if( SQLTime.year < 1970 )
	{
		DateTime = CDateTime( 1970 , 0 , 0 , 0 , 0 , 0 , 0 );
		return ;
	}
	sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",
		(INT)SQLTime.year,(INT)SQLTime.month,(INT)SQLTime.day,
		(INT)SQLTime.hour,(INT)SQLTime.minute,(INT)SQLTime.second);
	DateTime.parse(buffer,"YYYY-MM-DD hh:mm:ss");
}

//��ʱ����ת����������
void convert(TIME_STRUCT& SQLTime,const CDateTime& DateTime)
{
	memset(&SQLTime,0,sizeof(TIME_STRUCT));
	SQLTime.hour = DateTime.getHour();
	SQLTime.minute = DateTime.getMinute();
	SQLTime.second = DateTime.getSecond();
}
//�����ݿ�����ת����ʱ����
void convert(CDateTime& DateTime,const TIME_STRUCT& SQLTime)
{
	char buffer[40];
	sprintf(buffer,"%02d:%02d:%02d",
		(INT)SQLTime.hour,(INT)SQLTime.minute,(INT)SQLTime.second);
	DateTime.parse(buffer,"hh:mm:ss");
}

//��������ת����������
void convert(DATE_STRUCT& SQLTime,const CDateTime& DateTime)
{
	memset(&SQLTime,0,sizeof(DATE_STRUCT));
	SQLTime.year = DateTime.getYear();
	SQLTime.month = DateTime.getMonth();
	SQLTime.day = DateTime.getDay();
}
//�����ݿ�����ת����������
void convert(CDateTime& DateTime,const DATE_STRUCT& SQLTime)
{
	char buffer[40];
	sprintf(buffer,"%04d-%02d-%02d",(INT)SQLTime.year,(INT)SQLTime.month,(INT)SQLTime.day);
	DateTime.parse(buffer,"%04d-%02d-%02d");
}
//��鷵��ֵ
void getErrorMsgAndThrow( SQLSMALLINT nType, HANDLE hHand , const char * pMsg)
{
		UCHAR	szSqlState[SQL_MAX_MESSAGE_LENGTH] = "",
		szErrorMsg[SQL_MAX_MESSAGE_LENGTH] = "";
		SDWORD      pfNativeError = 0L;
		SWORD       pcbErrorMsg = 0;
		SQLSMALLINT cRecNmbr = 1;
		SQLGetDiagRec(nType, hHand,
         cRecNmbr, szSqlState, &pfNativeError,
         szErrorMsg, SQL_MAX_MESSAGE_LENGTH-1, &pcbErrorMsg); 
		ThrowSQLException<CSQLException>( pcbErrorMsg , 
					"[%s][%d][%d][%s][%s]", 
					pMsg ,
					pfNativeError , 
					(long)pcbErrorMsg , 
					szSqlState, 
					szErrorMsg );
}
//�õ�ODBC�������ݻ������� �ɹ�����>=0 ʧ�ܷ���-1
SQLSMALLINT getOdbcCType( const EDATATYPE type )
{
	const int(*p)[2] = g_AvoOdbcCTypeTable;
	for( ; (*p)[0] != -1 ; p++ )
	{
		if( type == (*p)[0] )
		{
			return (*p)[1];
		}
	}
	return -1;
}
//�õ�ODBC������չ����
SQLSMALLINT getOdbcSQLType( const EDATATYPE type )
{
	const int(*p)[2] = g_OdbcSQLTypeTable;
	for( ; (*p)[0] != -1 ; p++ )
	{
		if( type == (*p)[0] )
		{
			return (*p)[1];
		}
	}
	return -1;
}
//�õ��������Ͳ���throw
SQLSMALLINT getColDef( const EDATATYPE type )
{
	SQLSMALLINT ret = 0;
	switch( type )
	{
	case TYPE_STRING:
		ret = 1;
		break;
	case TYPE_DATE:
		ret = sizeof( DATE_STRUCT );
		break;
	case TYPE_TIME:
		ret = sizeof( TIME_STRUCT );
		break;
	case TYPE_TIMESTAMP:
		ret = sizeof( TIME_STRUCT );
		break;
	}
	return ret;
}
EDATATYPE getAvoDataTypeByOdbcType( const INT type)
{
	const int(*p)[2] = g_OdbcSqlAvoTable;
	for( ; (*p)[1] != -1 ; p++ )
	{
		if( type == (*p)[0] )
		{
			return (EDATATYPE)(*p)[1];
		}
	}
	return (EDATATYPE)-1;
}
