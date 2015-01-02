#ifndef _BINLOG_H_
#define _BINLOG_H_

/*
��������־���������ݵĵ���
*/
class CBinLog
{
public :
	/**
	 * ���ö�������־��·������������ã�Ĭ��Ϊ���̵ĵ�ǰ����Ŀ¼
	 */
	static void setBinLogPath(const char* pPath);
	/**
	 * ����binlog��Ĭ����
	 */
	static void setBinLogName(const char* pName);
	/**
	 * д�˶������ļ�
	 * @param pData Ҫд�������
	 * @param DataSize Ҫд������ݴ�С
	 * @param pLogName д����ļ�������������ã���ȡsetBinLogName������
	 */
	static void writeBinLog(const char* pData,int DataSize,const char* pLogName = NULL);
	/**
	 * ���ı���ʽ��ӡ�������Ƶ�����
	 * @param pData Ҫ��ӡ�Ķ���������
	 * @param DataSize ���������ݵĴ�С����������1024���ֽ�
	 * @return const char* ת���������
	 */
	static const char* printBinText(const char* pData,int DataSize);
};

#endif
