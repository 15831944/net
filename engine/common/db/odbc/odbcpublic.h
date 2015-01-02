#ifndef _MY_ODBCPUBLIC_H_
#define _MY_ODBCPUBLIC_H_
#include "../dbpublic.h"
#include "../dbinterface.h"

#define ODBCVER 0x0352
typedef unsigned char uchar_t;

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>
//����ͨ����ת�������ݿ�����
void convert(SQL_TIMESTAMP_STRUCT& SQLTime,const CDateTime& DateTime);
//�����ݿ�����ת������ͨ����
void convert(CDateTime& DateTime,const SQL_TIMESTAMP_STRUCT& SQLTime);

//��ʱ����ת����������
void convert(TIME_STRUCT& SQLTime,const CDateTime& DateTime);
//�����ݿ�����ת����ʱ����
void convert(CDateTime& DateTime,const TIME_STRUCT& SQLTime);

//��������ת����������
void convert(DATE_STRUCT& SQLTime,const CDateTime& DateTime);
//�����ݿ�����ת����������
void convert(CDateTime& DateTime,const DATE_STRUCT& SQLTime);
//�õ�ODBC�������ݻ������� �ɹ�����>=0 ʧ�ܷ���-1
SQLSMALLINT getOdbcCType( const EDATATYPE type );
//�õ�ODBC������չ����
SQLSMALLINT getOdbcSQLType( const EDATATYPE type );
//�õ�ODBC��NULLPoint����
SQLSMALLINT getColDef( const EDATATYPE type );
EDATATYPE getAvoDataTypeByOdbcType( const INT type);
//�õ��������Ͳ���throw
void getErrorMsgAndThrow( SQLSMALLINT nType, HANDLE hHand , const char * pMsg);

#define CHECK_CONN_AND_THROW( nCode , hHand , szMsg ) \
	if( ( SQL_SUCCESS != nCode ) && ( SQL_SUCCESS_WITH_INFO != nCode) ) \
	{	getErrorMsgAndThrow( SQL_HANDLE_DBC , hHand , szMsg); }

#define CHECK_EVN_AND_THROW( nCode , hHand , szMsg ) \
	if( ( SQL_SUCCESS != nCode ) && ( SQL_SUCCESS_WITH_INFO != nCode) ) \
	{	getErrorMsgAndThrow( SQL_HANDLE_ENV , hHand , szMsg); }

#define CHECK_STMT_AND_THROW( nCode , hHand , szMsg ) \
	if( ( SQL_SUCCESS != nCode ) && ( SQL_SUCCESS_WITH_INFO != nCode) ) \
	{	getErrorMsgAndThrow( SQL_HANDLE_STMT , hHand , szMsg); }

#define THROWEXCEPTION throw CSQLException(getErrNo(),getError());
#define EXECUTE(a) if ( SQL_NULL_HSTMT != a ) \
			THROWEXCEPTION;
#define VALIDSTMT if ( NULL == m_pStmt ) \
			throw CSQLException(ERROR_DATABASE_INVALIDOPER,"Not Prepared");

#define CHECK_PARAMS	\
	validIndex( nParamIndex ); \
	odbc_bind * pOdbcBind = NULL;	\
	map<int,odbc_bind *>::iterator itMap;	\
	itMap = m_mapParams.find( nParamIndex ); \
	if( itMap == m_mapParams.end() )\
	{\
		pOdbcBind =  new odbc_bind ;\
		m_mapParams[nParamIndex ] = pOdbcBind ;\
	}\
	else\
		pOdbcBind = itMap->second;

typedef struct odbc_bind
{
public:
	long nType;		//����
	long nScale;	//����
	long nLength;	//ʹ�ó���
	long nPcbValues; //ΪNULLʱʹ�ñ��
	long nInOut;	//�����������
	char szBuf[sizeof(TIMESTAMP_STRUCT) << 1 ]; //���ַ���String
private:
	long nSize;		//����������
	void * pBuf;	//���ݳ���
public:
	long getSize()
	{
		return nSize; 
	}
	void reset()
	{
		if( nSize >= sizeof( szBuf ) )
		{
			memset( pBuf , 0 , nSize );
		}
		else
		{
			memset( szBuf , 0 , nSize );
		}
	}
	void setSize( const long nLen )
	{
		int nlen = nLen + 1;
		if( nlen >= sizeof( szBuf ) )
		{
			pBuf = malloc( nlen + 1 );
			memset( pBuf , 0 , nlen + 1 );
		}
		nSize = nlen;
	}
	void setData(const void * pStr , const int nLen )
	{
		setString( pStr , nLen );
	}
	void * getData()
	{
		return getString();
	}
	void setString(const void * str , const int nLen)
	{ 
		nSize = 0 ; 
		if( NULL != pBuf )
		{
			::free( pBuf );
			pBuf = NULL;
		}
		if( nLen < sizeof( szBuf ) )
		{
			nSize = nLen;
			memcpy( szBuf , str , nLen );
		}
		else
		{
			nSize = nLen;
			::malloc( nLen + 1 );
			*( (char * )pBuf + nLen ) = 0;
			memcpy( pBuf , str , nLen );
		}
	}
	char * getString()
	{ 
		if( nSize < sizeof(szBuf) )
		{
			return (char *)szBuf;
		}  
		else
		{
			return (char *)pBuf;
		}
	};

	odbc_bind()
	{
		memset( this , 0 , sizeof( odbc_bind ) );
	}
	~odbc_bind()
	{
		if( NULL != pBuf )
		{
			::free(pBuf);
		}
	}
}ODBC_BIND,PODBC_BIND;

#define _M_MAX_FNAME_LEN_ 256

struct SODBCFieldInfo
{
	// meta data from ODBC
	SQLCHAR m_szName[_M_MAX_FNAME_LEN_];
	SWORD m_nSQLType;
	UDWORD m_nPrecision;
	SWORD m_nScale;
	SWORD m_nNullability;
	SODBCFieldInfo()
	{
		memset( this , 0 , sizeof( SODBCFieldInfo ));
	}
};
#endif