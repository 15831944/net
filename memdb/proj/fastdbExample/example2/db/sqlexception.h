// SQLException.h: interface for the CSQLException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SQLEXCEPTION_H)
#define SQLEXCEPTION_H

#include <util/userexception.h>

class CSQLError
{
public:
	/**
	 * ��ʼ�� CSQLError
	 * @param nSQLErrCode DataBase�쳣�Ĵ�����, ��Χ: 0x00010000 ~ 0x00019999, ǰ׺: ERROR_DATABASE_
	 * @param strErrDesc   DataBase����ľ�������
	 */
	CSQLError (int nSQLErrCode, const std::string& strErrDesc)
		      : m_nSQLErrCode(nSQLErrCode),
		        m_strSQLDesc(strErrDesc)
	{

	}

	/**
	 * ��ȡDataBase������ 
	 * @return int DataBase�쳣�Ĵ�����, ��Χ: 0x00010000 ~ 0x00019999, ǰ׺: ERROR_DATABASE_
	 */
	int getSQLErrorCode()  const  { return m_nSQLErrCode; } 

	/**
	 * ��ȡDataBase����ľ������� 
	 * @return std::string DataBase����ľ�������
	 */
	std::string getSQLDesc() const  { return m_strSQLDesc; }

private:
	int         m_nSQLErrCode;    
	std::string m_strSQLDesc;    
};

/**
 * DBģ���쳣�࣬���ڶ���DB��ص��쳣
 * @doc DBģ����ʵ���ϣ��Ჶ���������ݿ��׳����쳣��Ȼ���׳�DBģ���ڲ�������쳣
 */
class CSQLException : public CCException
{
public:
	/**
	 * ��ʼ�� CSQLException
	 * @param nSQLErrCode DataBase�쳣�Ĵ�����, ��Χ: 0x00010000 ~ 0x00019999, ǰ׺: ERROR_DATABASE_
	 * @param strErrMsg   DataBase����ľ�������
	 */
	CSQLException(int nSQLErrCode, const std::string& strErrMsg);

	/**
	 * ��������
	 */
	virtual ~CSQLException() throw();
	
	/**
	 * ��ʼ�� CSQLException
	 * @return const char* DataBase����ľ�������
	 */
	const char* what();

	/*
	 *	����һ�� CSQLError, ���� DataBase������Ϣ
	 *  @return CSQLError ���� DataBase������Ϣ
	 */
	const CSQLError& getSQLError();

private:
	int m_nSQLErrCode;
	CSQLError m_SQLError;
};

template <class T>
void ThrowSQLException(int nSQLErrCode,const char* fmt,...)
{
	char buffer[1024];
	va_list list;
	va_start(list,fmt);
#ifdef WIN32
	int retval = _vsnprintf(buffer,sizeof(buffer)-1,fmt,list);
	if ( retval > 0 )
	{
		buffer[retval] = 0;
	}
	else if ( retval == -1 )
	{
		buffer[sizeof(buffer)-1] = 0;
	}
#elif defined GCC
	//linux���Զ�����0�������
	int retval = vsnprintf(buffer,sizeof(buffer),fmt, list);
#else
	int retval = vsnprintf(buffer,sizeof(buffer)-1,fmt,list);
	if ( retval > 0 && (retval< sizeof(buffer)-1) )
	{
		buffer[retval] = 0;
	}
	else
	{
		buffer[sizeof(buffer)-1] = 0;
	}
#endif
	buffer[sizeof(buffer)-1] = 0 ;
	va_end(list);
	throw T(nSQLErrCode,buffer);
}

#endif // !defined(SQLEXCEPTION_H)

