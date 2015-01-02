#ifndef _ODBCCALLABLESTATEMENT_H_
#define _ODBCCALLABLESTATEMENT_H_
#include "../dbinterface.h"
#include "odbcpublic.h"
#include "odbccallablestatement.h"
class CCallableStatement;
namespace my_odbc
{
	class COdbcConnection;
	class COdbcStatement;
	class COdbcResultSet;
	class COdbcCallableStatement
		:public CCallableStatement
	{
	public:
		COdbcCallableStatement(COdbcConnection& Conn);
		virtual ~COdbcCallableStatement();
		
		/**
		* ע��洢���̷���ֵΪ�����
		* @param	nIndex: ������ID
		* @return	�����ͣ�ע��ɹ�Ϊtrue������Ϊfalse
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual bool registerResultset(int nIndex);
		
		/**
		* ע�ắ������ֵΪ�����
		* @param	nEDATATYPE: ����ֵ������
		* @return	�����ͣ�ע��ɹ�Ϊtrue������Ϊfalse
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual bool registerReturn(EDATATYPE nEDATATYPE);
		
		/**
		* ִ�д洢���̻���
		* @param  ��
		* @return �����ͣ��ɹ�ִ�з���true�����򷵻�false
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual bool execute();
		
		/**
		* �رյ�ǰִ��SQL����Statement��
		* �����Ҫִ��SQL��䣬��Ҫ���´���Statement��
		* @param  ��
		* @return ��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void close();
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��Ϊchar��
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setByte(int nParamIndex, char Value, bool isRegOut=false);
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��Ϊ������
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		//virtual void setBoolean(int nParamIndex, bool Value, bool isRegOut=false)=0;
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��Ϊ�ֽ�����ָ��
		*			nLength:		�ֽ����еĳ���
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setBytes(int nParamIndex, const char* Value, int nLength, bool isRegOut=false);
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��Ϊ�ַ���
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setString(int nParamIndex, const std::string& Value, bool isRegOut=false);
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��ΪCDateTime��
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setDate(int nParamIndex, CDateTime Value, bool isRegOut=false);
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��Ϊ������
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setDouble(int nParamIndex, double Value, bool isRegOut=false);
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��Ϊ����
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setInt(int nParamIndex, int Value, bool isRegOut=false);
		
		/**
		* ���SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			nEDATATYPE:		Ҫ��յĲ�������
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setNull(int nParamIndex, EDATATYPE nEDATATYPE, bool isRegOut=false);
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��ΪCDateTime��
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setTime(int nParamIndex, CDateTime Value, bool isRegOut=false);
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��ΪCDateTime��
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setTimestamp(int nParamIndex, CDateTime Value, bool isRegOut=false);
		
		/**
		* ����SQL����������ID��ֵ
		* @param	nParamIndex:	����ID
		*			Value:			Ҫ���õ�ֵ��ΪLong64�ͣ���64λ����
		*			isRegOut:		�Ƿ�ע��Ϊ���
		* @return	��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void setLong64(int nParamIndex, Long64 Value, bool isRegOut = false);
		
		/**
		* �Ƿ��и�������
		* �����жϴ洢���̺ͺ���
		* @param	��
		* @return	�����ͣ��н�����򷵻�true�����򷵻�false
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual bool hasMoreResultsets();
		
		/**
		* ��ȡ��һ�������
		* @param	��
		* @return	ָ����CResultSet��CAutoPtr����ָ��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual CAutoPtr<CResultSet> getNextResultSet();
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex: ����ID
		* @return	����Ϊchar
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual char getByte(int nIndex);
		
		/**
		* ������������ֵ	
		* @param	pColumnName: �������ֶ�����
		* @return	����Ϊchar
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual char getByte(const char* pColumnName);
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex:		����ID
		*			pOutBuffer: �����ֽ����еĵ�ַ
		*			nLength:	�ֽ����еĳ���
		* @return	����ֵ����Ϊ������
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual bool getBytes(int nIndex, char* pOutBuffer, int& nLength);
		
		/**
		* ������������ֵ
		* @param	pColumnName: �������ֶ�����
		*			pOutBuffer:  �����ֽ����еĵ�ַ
		*			nLength:	 �ֽ����еĳ���
		* @return	����ֵ����Ϊ������
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual bool getBytes(const char* pColumnName, char* pOutBuffer, int& nLength);
		
		/**
		* ���ز�������ID��ֵ
		* @param	nIndex: ����ID
		* @return	����ֵ����Ϊ�����ͣ��ɹ�Ϊtrue������Ϊfalse
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		//virtual bool getBoolean(int nIndex)=0;
		
		/**
		* ������������ֵ
		* @param	pColumn: �������ֶ�����
		* @return	����ֵ����Ϊ�����ͣ��ɹ�Ϊtrue������Ϊfalse
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		//virtual bool getBoolean(const char* pColumn)=0;
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex: ����ID
		* @return	����ֵ����Ϊ�ַ���
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual std::string getString(int nIndex);
		
		/**
		* ������������ֵ
		* @param	pColumn: �������ֶ�����
		* @return	����ֵ����Ϊ�ַ���
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual std::string getString(const char* pColumn);
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex: ����ID
		* @return	����ֵ����Ϊ����
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual int getInt(int nIndex);
		
		/**
		* ������������ֵ
		* @param	pColumn: �������ֶ�����
		* @return	����ֵ����Ϊ����
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual int getInt(const char* pColumn);
		
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex: ����ID
		* @return	����ֵ����Ϊ������
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual double getDouble(int nIndex);
		
		/**
		* ������������ֵ
		* @param	pColumn: �������ֶ�����
		* @return	����ֵ����Ϊ������
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual double getDouble(const char* pColumn);
		
		/**
		* �жϲ�������ID��ֵ�Ƿ�Ϊ�գ�NULL��
		* @param	nIndex: ����ID
		* @return	����ֵ����Ϊ�����ͣ����Ϊ���򷵻�true������Ϊfalse
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual bool isNull(int nIndex);
		
		/**
		* ���������ж�ֵ�Ƿ�Ϊ�գ�NULL��
		* @param	pColumn: �������ֶ�����
		* @return	����ֵ����Ϊ�����ͣ����Ϊ���򷵻�true������Ϊfalse
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual bool isNull(const char* pColumn);
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex: ����ID
		* @return	����ֵ����ΪCDateTime
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual CDateTime getDate(int nIndex);
		
		/**
		* ������������ֵ
		* @param	pParamName: �������ֶ�����
		* @return	����ֵ����ΪCDateTime
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual CDateTime getDate(const char* pParamName);
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex: ����ID
		* @return	����ֵ����ΪCDateTime
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual CDateTime getTime(int nIndex);
		
		/**
		* ������������ֵ
		* @param	pParamName: �������ֶ�����
		* @return	����ֵ����ΪCDateTime
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual CDateTime getTime(const char* pParamName);
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex: ����ID
		* @return	����ֵ����ΪCDateTime
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual CDateTime getTimestamp(int nIndex);
		
		/**
		* ������������ֵ
		* @param	pColumn: �������ֶ�����
		* @return	����ֵ����ΪCDateTime
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual CDateTime getTimestamp(const char* pColumn);
		
		/**
		* ���ز�������ID��ֵ	
		* @param	nIndex: ����ID
		* @return	����ֵ����ΪLong64��64λ����
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual Long64 getLong64(int nIndex);
		
		/**
		* ������������ֵ
		* @param	pParamName: �������ֶ�����
		* @return	����ֵ����ΪLong64
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual Long64 getLong64(const char* pParamName);
		
		/**
		* Ԥִ��SQL���
		* ִ�к�������ò���ֵ��
		* @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
		*/
		virtual void prepare(const char* pSql);
	public:
		HSTMT getStmt(){ return m_pStmt; }
		void clearResultSet();
	private:
		virtual void clearParameters();
		/**
		 * ����������Ƿ�Ϸ�
		 * @param nIndex int ������
		 */
		virtual void validIndex(int nIndex);
		//д������
		void writeBinaryData();
	private:
		HSTMT m_pStmt;
		SQLSMALLINT m_nParamCount;
		map<int,odbc_bind *> m_mapParams;//���ѡ��
		COdbcResultSet *m_pUsedResultSet;
		bool m_nResultsetCount;
	};
}
#endif