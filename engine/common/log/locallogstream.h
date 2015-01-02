#ifndef _LOCALLOGSTREAM_H_
#define _LOCALLOGSTREAM_H_

#include "public.h"
#include "thread/thread.h"
#include  "lock/lightLock.h"
#include  "io/outputstream.h"
#include  "log.h"
#include "util/refshare.h"

class CLocalLogOutputStream : public IOutputStream,
							  public CRefShare,
							  public CRunnable
{
public :
	CLocalLogOutputStream(const char * pathname = "./log", const char *logfilename = "log");
	~CLocalLogOutputStream();
	int writeBytes(const char *pBuf, uint32 &Buflen);
	void incRef();
	bool decRef();
	/**
	 * �߳�����������
	 */
	void run();
	/*
	 * ��ʼ��
	 */
	bool init();
	/**
	 * ֹͣ
	 */
	void stop();
	/**
	 * �����߳�
	 */
	void start();

private :
	bool writeLog(LOGMSG& LogRequest);
	/**
	 * д����־�ļ�
	 */
	void flushFile();

private :
	std::string m_strFile;
	std::string m_strPath;
	//д��
	CLightLock m_WriteLock;
	//��־����
	list<string> m_LogList;
	//�߳�
	CThread* m_pThread;
	//���б�ʶ
	bool m_bRunFlag;
};

typedef CRefObject<CLocalLogOutputStream> CLocalLogOutputStreamPtr;
#endif

