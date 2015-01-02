#ifndef _CCALLABLESTATEMENT_H_
#define _CCALLABLESTATEMENT_H_

class CResultSet;
/**
 * ����������
 * @doc ʵ�ֵ������ݿ��еĴ洢���̡������Ĺ���
 * <br> ����oracle���ݿ⣬����Ҳ�������ύsql���
 */
class CCallableStatement
{
  public:

	  virtual ~CCallableStatement(){};

	/**
	 * ע��洢���̷���ֵΪ�����
	 * @param	nIndex: ������ID
	 * @return	�����ͣ�ע��ɹ�Ϊtrue������Ϊfalse
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool registerResultset(int nIndex)=0;

	/**
	 * ע�ắ������ֵΪ�����
	 * @param	nEDATATYPE: ����ֵ������
	 * @return	�����ͣ�ע��ɹ�Ϊtrue������Ϊfalse
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
	//virtual bool registerReturn(EDATATYPE nEDATATYPE)=0;

	/**
	 * ִ�д洢���̻���
	 * @param  ��
	 * @return �����ͣ��ɹ�ִ�з���true�����򷵻�false
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
	virtual bool execute()=0;

	/**
	 * �رյ�ǰִ��SQL����Statement��
	 * �����Ҫִ��SQL��䣬��Ҫ���´���Statement��
	  * @param  ��
	 * @return ��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
	virtual void close()=0;

    /**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊchar��
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual void setByte(int nParamIndex, char Value, bool isRegOut=false)=0;

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
    virtual void setBytes(int nParamIndex, const char* Value, int nLength, bool isRegOut=false)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ�ַ���
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
	virtual void setString(int nParamIndex, const CMYString& Value, bool isRegOut=false)=0;

    /**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��ΪCDateTime��
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual void setDate(int nParamIndex, CDateTime Value, bool isRegOut=false)=0;

    /**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ������
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual void setDouble(int nParamIndex, double Value, bool isRegOut=false)=0;

    /**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ����
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual void setInt(int nParamIndex, int Value, bool isRegOut=false)=0;

    /**
	 * ���SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			nEDATATYPE:		Ҫ��յĲ�������
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    //virtual void setNull(int nParamIndex, EDATATYPE nEDATATYPE, bool isRegOut=false)=0;

    /**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��ΪCDateTime��
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual void setTime(int nParamIndex, CDateTime Value, bool isRegOut=false)=0;

    /**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��ΪCDateTime��
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual void setTimestamp(int nParamIndex, CDateTime Value, bool isRegOut=false)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��ΪLong64�ͣ���64λ����
	 *			isRegOut:		�Ƿ�ע��Ϊ���
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
	virtual void setLong64(int nParamIndex, long64 Value, bool isRegOut = false)=0;

    /**
	 * �Ƿ��и�������
     * �����жϴ洢���̺ͺ���
	 * @param	��
	 * @return	�����ͣ��н�����򷵻�true�����򷵻�false
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool hasMoreResultsets()=0;

    /**
	 * ��ȡ��һ�������
     * @param	��
	 * @return	ָ����CResultSet��CAutoPtr����ָ��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual auto_ptr<CResultSet> getNextResultSet()=0;

    /**
	 * ���ز�������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @return	����Ϊchar
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual char getByte(int nIndex)=0;

    /**
	 * ������������ֵ	
	 * @param	pColumnName: �������ֶ�����
	 * @return	����Ϊchar
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual char getByte(const char* pColumnName)=0;

    /**
	 * ���ز�������ID��ֵ	
	 * @param	nIndex:		����ID
	 *			pOutBuffer: �����ֽ����еĵ�ַ
	 *			nLength:	�ֽ����еĳ���
	 * @return	����ֵ����Ϊ������
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool getBytes(int nIndex, char* pOutBuffer, int& nLength)=0;

    /**
	 * ������������ֵ
	 * @param	pColumnName: �������ֶ�����
	 *			pOutBuffer:  �����ֽ����еĵ�ַ
	 *			nLength:	 �ֽ����еĳ���
	 * @return	����ֵ����Ϊ������
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool getBytes(const char* pColumnName, char* pOutBuffer, int& nLength)=0;

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
    virtual CMYString getString(int nIndex)=0;

    /**
	 * ������������ֵ
	 * @param	pColumn: �������ֶ�����
	 * @return	����ֵ����Ϊ�ַ���
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CMYString getString(const char* pColumn)=0;

    /**
	 * ���ز�������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @return	����ֵ����Ϊ����
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual int getInt(int nIndex)=0;

    /**
	 * ������������ֵ
	 * @param	pColumn: �������ֶ�����
	 * @return	����ֵ����Ϊ����
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual int getInt(const char* pColumn)=0;

    
    /**
	 * ���ز�������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @return	����ֵ����Ϊ������
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual double getDouble(int nIndex)=0;

    /**
	 * ������������ֵ
	 * @param	pColumn: �������ֶ�����
	 * @return	����ֵ����Ϊ������
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual double getDouble(const char* pColumn)=0;

    /**
	 * �жϲ�������ID��ֵ�Ƿ�Ϊ�գ�NULL��
	 * @param	nIndex: ����ID
	 * @return	����ֵ����Ϊ�����ͣ����Ϊ���򷵻�true������Ϊfalse
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool isNull(int nIndex)=0;

    /**
	 * ���������ж�ֵ�Ƿ�Ϊ�գ�NULL��
	 * @param	pColumn: �������ֶ�����
	 * @return	����ֵ����Ϊ�����ͣ����Ϊ���򷵻�true������Ϊfalse
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool isNull(const char* pColumn)=0;

    /**
	 * ���ز�������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @return	����ֵ����ΪCDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getDate(int nIndex)=0;

    /**
	 * ������������ֵ
	 * @param	pParamName: �������ֶ�����
	 * @return	����ֵ����ΪCDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getDate(const char* pParamName)=0;

    /**
	 * ���ز�������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @return	����ֵ����ΪCDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getTime(int nIndex)=0;

    /**
	 * ������������ֵ
	 * @param	pParamName: �������ֶ�����
	 * @return	����ֵ����ΪCDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getTime(const char* pParamName)=0;

    /**
	 * ���ز�������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @return	����ֵ����ΪCDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getTimestamp(int nIndex)=0;

    /**
	 * ������������ֵ
	 * @param	pColumn: �������ֶ�����
	 * @return	����ֵ����ΪCDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getTimestamp(const char* pColumn)=0;

    /**
	 * ���ز�������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @return	����ֵ����ΪLong64��64λ����
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual long64 getLong64(int nIndex)=0;

    /**
	 * ������������ֵ
	 * @param	pParamName: �������ֶ�����
	 * @return	����ֵ����ΪLong64
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual long64 getLong64(const char* pParamName)=0;

	/**
	 * Ԥִ��SQL���
	 * ִ�к�������ò���ֵ��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
	virtual void prepare(const char* pSql)=0;

};

#endif 
