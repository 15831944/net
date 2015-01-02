#ifndef _CRESULTSET_H_
#define _CRESULTSET_H_

/**
 * �������
 * @doc ά����ѯ���صĽ����
 */
class CResultSet
{
  public:
	virtual ~CResultSet(){};

    /**
	 * �رս������
     * ������resultsetʹ����Ϻ���ʽ���ô˷������ͷ���Դ����߳�������
	 * @param	��
	 * @return	��
	 * @exception CSQLException �ر�ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual void close()=0;

	/**
	 * ���ؽ�������еĸ�����
	 * @param	��
	 * @return	��������еĸ���
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual int getColumnCount() const=0;

	/**
	 * ���ؽ��������Ӧ������������
	 * ��������Ƿ����򷵻ؿ��ַ���
	 * @param	nIndex��������
	 * @return	�������ֶ�����,�Դ�д��ĸ����
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual std::string getColumnName(int nIndex) const=0;

	/**
	 * ���ؽ��������Ӧ����������������
	 * ��������Ƿ������׳��쳣��code:-6��
	 * @param	nIndex��������
	 * @return	�е���������
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
	virtual EDATATYPE getColumnDataType(int nIndex)=0;

    /**
	 * ������ƶ�����һ��
	 * @param	��
	 * @return	�����ͣ���true��������һ�м�¼��false������һ�м�¼��
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool next()=0;

	/**
	 * �������������е�����ID
	 * @param	pColumnName		�������ֶ�����
	 * @return	�����е�����ID��ʧ��ʱ����-1
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual int findColumn(const char* pColumnName) const =0;

   /**
	 * ����������ȡ�����ֵ	
	 * @param	pColumnName:Ҫ����ֵ������
	 * @param	pOutBuffer:	��Ŵ�����buff
	 * @param	nBuffSize:	��Ŵ�����buff�Ĵ�С
	 * @param   nCharCount  ��Ҫ�����ַ���
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	pDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @param   nOffset     ��ʼ��������λ��
	 * @return	�����Ĵ������ֽ���(�൱��strlen(char*),�����ȫ�����ģ�GBK������£�ʵ�ʵ��ַ�������return/2)
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 * @note �������ݿ����UTF8�ַ���������nBuffSize�Ĵ�С����>ʵ���ַ���*3�����nBuffSize��С���㣬���׳��쳣
	   ����ʾ���μ�teststatement.cpp testclob����
	    sql = "select id,value from testclob";
		stmt3->prepare(sql.c_str());
		CAutoPtr<CResultSet> rst = stmt3->executeQuery();

		unsigned char testclob[40960]="";
		memset(testclob,0,40960);
		int buffsize = sizeof(testclob);		
		rst->next();		
		int gotcount = rst->getBytes(2,testclob,buffsize,buffsize/3);
		//�����Ҫ���ƶ�ȡ���ַ�����������ô���ã�����ֻ��ǰ1000���ַ�
		//int gotcount = rst->getBytes(2,testclob,buffsize,1000);
		rst->close();
	 */
    virtual int getBytes(int nIndex,unsigned char* pOutBuffer,int nBuffSize,int nCharCount,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING,unsigned int nOffset = 1) const =0;

	/**
	 * ����������ȡ�����ֵ	
	 * @param	pColumnName:Ҫ����ֵ������
	 * @param	pOutBuffer:	��Ŵ�����buff
	 * @param	nBuffSize:	��Ŵ�����buff�Ĵ�С
	 * @param   nCharCount  ��Ҫ�����ַ���
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	pDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	�����Ĵ������ֽ���(�൱��strlen(char*),�����ȫ�����ģ�GBK������£�ʵ�ʵ��ַ�������return/2)
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��	 
	 */
    virtual int getBytes(const char* pColumnName,unsigned char* pOutBuffer,int nBuffSize,int nCharCount,bool bNullThrow = true,const char* pDefault = DEFAULT_STRING,unsigned int nOffset = 1) const =0;

    /**
	 * ���ؽ����������ID��ֵ	
	 * @param	nIndex:		����ID
	 * @return	������
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    //virtual bool getBoolean(int nIndex)=0;

    /**
	 * �����������ؽ������ֵ	
	 * @param	pColumn:	Ҫ����ֵ������
	 * @return	������
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    //virtual bool getBoolean(const char* pColumn)=0;

	/**
	 * ���ؽ����������ID��ֵ	
	 * @param	nIndex:		����ID
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	pDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	�ַ���
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual std::string getString(int nIndex,bool bNulLThrow = true,const char* pDefault= DEFAULT_STRING) const =0;

    /**
	 * �����������ؽ������ֵ	
	 * @param	pColumn:	Ҫ����ֵ������
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	pDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	�ַ���
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual std::string getString(const char* pColumn,bool bNulLThrow = true,const char* pDefault= DEFAULT_STRING) const =0;

    /**
	 * ����������ȡintֵ	
	 * @param	nIndex:		����ID
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	nDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	����
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual int getInt(int nIndex,bool bNulLThrow = true,int nDefault= DEFAULT_INT) const =0;

    /**
	 * ����������ȡintֵ	
	 * @param	pColumn:	Ҫ����ֵ������
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	nDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	����
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual int getInt(const char* pColumn,bool bNulLThrow = true,int nDefault= DEFAULT_INT) const =0;

    /**
	 * ����������ȡDoubleֵ	
	 * @param	nIndex:		����ID
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	dDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	double
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual double getDouble(int nIndex,bool bNulLThrow = true,double dDefault= DEFAULT_DOUBLE) const =0;

    /**
	 * �����������ؽ������ֵ	
	 * @param	pColumn:	Ҫ����ֵ������
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	dDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	double
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual double getDouble(const char* pColumn,bool bNulLThrow = true,double dDefault= DEFAULT_DOUBLE) const =0;

    /**
	 * �жϽ����������ID��Ӧ���ֶ��Ƿ�ΪNULL
	 * @param	nIndex: ����ID
	 * @return	�����ͣ�����Ϊ���򷵻�true������false
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool isNull(int nIndex) const =0;

    /**
	 * ���������жϽ�����ж�Ӧ���ֶ��Ƿ�ΪNULL
	 * @param	pColumn:	Ҫ�ж�ֵ������
	 * @return	�����ͣ�����Ϊ���򷵻�true������false
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual bool isNull(const char* pColumn) const =0;

    /**
	 * ���ؽ����������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	Default	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	CDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getDate(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const =0;

    /**
	 * �����������ؽ������ֵ	
	 * @param	pColumn:	Ҫ����ֵ������
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	Default	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	CDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getDate(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const =0;

    /**
	 * ���ؽ����������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	Default	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	CDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getTime(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const =0;

    /**
	 * �����������ؽ������ֵ	
	 * @param	pColumn:	Ҫ����ֵ������
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	Default	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	CDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getTime(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const =0;

    /**
	 * ���ؽ����������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	Default	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	CDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getTimestamp(int nIndex,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const =0;

    /**
	 * �����������ؽ������ֵ	
	 * @param	pColumn:	Ҫ����ֵ������
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	Default	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	CDateTime
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual CDateTime getTimestamp(const char* pColumn,bool bNullThrow = true,const CDateTime& Default = DEFAULT_DATETIME) const =0;

    /**
	 * ���ؽ����������ID��ֵ	
	 * @param	nIndex: ����ID
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	nDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	int64��64λ����ֵ
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual int64 getInt64(int nIndex,bool bNullThrow = true,int64 nDefault = DEFAULT_LONG64) const =0;

    /**
	 * �����������ؽ������ֵ	
	 * @param	pParamName:	Ҫ����ֵ������
	 * @param	bNullThrow	�����ǰֵΪ�գ��Ƿ��׳��쳣��Ĭ��ΪTRUE
	 * @param	nDefault	���bNullThrowΪfalse���򵱵�ǰֵΪ���򷵻ش�Ĭ��ֵ
	 * @return	int64��64λ����ֵ
	 * @exception CSQLException ����ʧ�ܣ�����ԭ�������sqlcode�������ݿ�������˵��
	 */
    virtual int64 getInt64(const char* pParamName,bool bNullThrow = true,int64 nDefault = DEFAULT_LONG64) const =0;

};



#endif /* CRESULTSET_H_HEADER_INCLUDED_BB3264A8 */
