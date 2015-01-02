#ifndef _CCALLABLESTATEMENTIMPL_H_
#define _CCALLABLESTATEMENTIMPL_H_

#include <db/callablestatement.h>
#include "connectionimpl.h"
namespace mysql
{
/**
 * ���ô洢���̵�statement
 * ����˵���μ��ӿ���
 */
	class CCallableStatementImpl : public CCallableStatement
	{
	friend class CConnectionImpl;
	//
	protected:
		//oracle::occi::Connection* m_pConn;
		CConnectionImpl* m_pConn;

	public:
		CCallableStatementImpl(CConnectionImpl& Conn);
		//
		~CCallableStatementImpl();

		bool execute();

		void close();

		bool registerReturn(EDATATYPE nSqlType);

		//
		void setByte(int nParamIndex, char Value, bool isRegOut=false);

		//
		//void setBoolean(int nParamIndex, bool Value, bool isRegOut=false);

		//
		void setBytes(int nParamIndex, const char* Value, int nLength, bool isRegOut=false);

		//
		void setString(int nParamIndex, const std::string& Value, bool isRegOut=false);

		//
		void setDate(int nParamIndex, CDateTime Value, bool isRegOut=false);

		//
		void setDouble(int nParamIndex, double Value, bool isRegOut=false);

		//
		void setInt(int nParamIndex, int Value, bool isRegOut=false);

		//
		void setNull(int nParamIndex, EDATATYPE nSqlType, bool isRegOut=false);

		//
		void setTime(int nParamIndex, CDateTime Value, bool isRegOut=false);

		//
		void setTimestamp(int nParamIndex, CDateTime Value, bool isRegOut=false);

		//
		void setInt64(int nParamIndex, int64 Value, bool isRegOut);

		//
		bool  registerResultset(int nIndex);	     

		// �Ƿ��и������
		//
		bool hasMoreResultsets();

		// ��ȡ��һ�������
		CAutoPtr<CResultSet> getNextResultSet();

		//
		char getByte(int nIndex);

		//
		char getByte(const char* pColumnName);

		//
		bool getBytes(int nIndex, char* pOutBuffer, int& nLength);

		//
		bool getBytes(const char* pColumnName, char* pOutBuffer, int& nLength);

		//
		bool getBoolean(int nIndex);

		//
		bool getBoolean(const char* pColumn);

		//
		std::string getString(int nIndex);

		//
		std::string getString(const char* pColumn);

		//
		int getInt(int nIndex);

		//
		int getInt(const char* pColumn);

		//
		double getDouble(int nIndex);

		//
		double getDouble(const char* pColumn);

		//
		bool isNull(int nIndex);

		//
		bool isNull(const char* pColumn);

		//
		CDateTime getDate(int nIndex);

		//
		CDateTime getDate(const char* pParamName);

		//
		CDateTime getTime(int nIndex);

		//
		CDateTime getTime(const char* pParamName);

		//
		CDateTime getTimestamp(int nIndex);

		//
		CDateTime getTimestamp(const char* pColumn);

		//
		int64 getInt64(int nIndex);

		//
		int64 getInt64(const char* pParamName);

		//
		void prepare(const char* pSql);

	};
}




#endif
