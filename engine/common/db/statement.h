#ifndef _CSTATEMENT_H_
#define _CSTATEMENT_H_

class CResultSet;
/**
 * SQL���������
 * @doc ʵ������SQL���Ĺ���
 */
class CStatement
{
public:
	virtual ~CStatement() {};
	/**
	 * �رյ�ǰִ��SQL����ʵ��
	 * @param	��
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void close()=0;

	/**
	 * ��ȡ���ݿ��ʱ��ƫ���������籱��ʱ�䷵��8,��������ʱ�䷵��-5
	 * @param Throw ��ȡ����ʱ��ʱ�Ƿ����쳣�������쳣�Ļ�����GMT/UTC(��������ʱ��)
	 * @return ��ǰʱ����UTC��ƫ����                                                                    
	 * @exception CAvoException ȡʱ��ƫ�����쳣
	 */	
	virtual int getTimeZoneOffset(bool Throw=false)=0;

	/**
	 * ��ȡ����ǰ������ֵ
	 * @param ����ڻ�ȡ����ֵʱ��Ҫ���������ʹ�ô˲���������oracle��Ҫ����������	
	 * @return	����ֵ
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual uint64 getInsertID(void* pParam)=0;

	/**
	 * ִ�в�ѯSQL�����ؽ����
	 * <br>����ʹ��ͬһ��Cstatement��ȡ�������Ͻ���������������һ�β�ѯprepareǰ����Ҫ��close��һ�������
	 * @param	pSQL �ύ���е�sql���
	 * @return	ָ������������ָ��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual CAutoPtr<CResultSet> executeQuery(const char* pSQL)=0;

	/**
	 * ִ�и��µ���䲢����Ӱ�������
	 * @param	pSQL �ύ���е�sql���
	 * @return	��Ӱ�������
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual int executeUpdate(const char* pSQL)=0;

	/**
	 * ִ��SQL���
	 * @param	pSQL �ύ���е�sql���
	 * @return	�����ͣ��ɹ�Ϊtrue������Ϊfalse
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual bool execute(const char* pSQL)=0;    

	/**
	 * ִ�е�ǰ��SQL���
	 * ���ڲ���������SQL���Ӧ����ǰ���prepare()�Ⱥ����趨
	 * @param	��
	 * @return	�����ͣ��ɹ�Ϊtrue������Ϊfalse
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual bool execute()=0;

	/**
	 * Ԥ����SQL���
	 * ͨ��SQL�����Ҫ�趨����ʱ�����ô˺���
	 * <br>����ʹ��ͬһ��Cstatement��ȡ�������Ͻ���������������һ�β�ѯprepareǰ����Ҫ��close��һ�������
	 * @param	pSQL �ύ���е�sql���
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void prepare(const char* pSql)=0;

	/** 
	 * ��ȡsql���
	 * @return sql���
	 * @note ���뿪statement��������󣬷���ָ��ָ����ڴ�ᱻ�ͷ�
	 */
	virtual const char* getSQL() const =0;

	/**
	 * ִ�е�ǰSQL��ѯ
	 * ���ؽ����
	 * @param	��
	 * @return	ָ������������ָ��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual CAutoPtr<CResultSet> executeQuery()=0;

	/**
	 * ִ�е�ǰ�ĸ���SQL����
	 * ����ִ��Ӱ���������
	 * @param	��
	 * @return	��Ӱ���������
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual int executeUpdate()=0;

	/**
	 * ������в�����ֵ
	 * ����ִ��SQL��䣬��Ҫ�����趨����ֵ
	 * @param	��
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void clearParameters()=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊchar��
	 * @return	��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setByte(int nParamIndex, char Value)=0;

	/**
	 * insert into table������±�������ֵ
	 * @param strColumnName ������ֶ���
	 * @param Value �����ֵ
	 * @param nLength �����ֵ�ĳ���
	 * @param SetSequence �Ƿ�����������������ȡ����ֵ��
	 * @param SequenceName ������
	 * @param SetPkName �Ƿ����������ֶ���������ȡ����ֵ��
	 * @param PkName �����ֶ���
	 * @exception CSQLException ��������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 * @note �μ�teststatement.cpp��testclob����
 		conn->begin();
 		sql = "insert into testclob (value) values (EMPTY_CLOB())";
		stmt->prepare(sql.c_str());
		stmt->executeUpdate();
		
		unsigned char tmp[100]="a very log string";
		int length = strlen((const char *)tmp);
		stmt->saveBytes("value",tmp,length);
		conn->commit();
	 */	
	virtual void saveBytes(const char* strColumnName, const unsigned char* Value, int nLength,bool SetSequence=false,string SequenceName="",bool SetPkName=false,string PkName="")=0;

	/**
	 * update table����������ô����ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ�ֽ�����ָ��
	 *			nLength:		�ֽ����еĳ���
	 * @return	��
	 * @exception	CSQLException ���ô����ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 * @note
	 * ���oracle�棬ʵ�ֶԴ�����ֶεĸ��£���Ҫʹ��select for update��������´����
	 * ���²��裺
	 * stmt->prepare("select ... for update");
	 * stmt->executeQuery();
	 * stmt->setBytes();
	 * stmt->executeUpdate();
	 * �����setBytesǰû����executeQuery,���׳�������ΪERROR_DATABASE_LOBUPDATE���쳣
	 */
	virtual void setBytes(int nParamIndex, const unsigned char* Value, int nLength)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ������
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	//virtual void setBoolean(int nParamIndex, bool Value)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ�ַ���
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setString(int nParamIndex, const std::string& Value)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ����
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setInt(int nParamIndex, int Value) = 0;
	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ����
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setUInt(int nParamIndex, unsigned int Value) = 0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊ������
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setDouble(int nParamIndex, double Value)=0;

	/**
	 * ���SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			nEDATATYPE:		Ҫ��յĲ�������
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setNull(int nParamIndex, EDATATYPE nEDATATYPE)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��ΪCDateTime��
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setDate(int nParamIndex, CDateTime Value)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��ΪCDateTime��
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setTime(int nParamIndex, CDateTime Value)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��ΪCDateTime��
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setTimestamp(int nParamIndex, CDateTime Value)=0;

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊint64�ͣ���64λ����
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setInt64(int nParamIndex, int64 Value)=0; 

	/**
	 * ����SQL����������ID��ֵ
	 * @param	nParamIndex:	����ID
	 *			Value:			Ҫ���õ�ֵ��Ϊint64�ͣ���64λ����
	 * @return	��
	 * @exception	CSQLException sql�������ʧ�ܣ�����ԭ�������errcode�������ݿ�������˵��
	 */
	virtual void setUInt64(int nParamIndex, uint64 Value)=0; 

};

#endif
