#ifndef _LOG_H_
#define _LOG_H_

#include "io/outputstream.h"
#include "exception/myException.h"
#include "util/datetime.h"
#include "io/bytesbuffer.h"

enum ELOGLEVEL
{
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG,
};

typedef struct _t_logmsg
{
	//ʱ��
	char datetime[30];
	//������
	string process;
	//����ID
	int pid;
	//servername
	string servername;
	//��־�ȼ�
	int level;
	//��־����
	const char* message;
}LOGMSG,*PLOGMSG;

/*
���л���־����
*/
void serialLogMsg(const LOGMSG& LogMsg,CBytesBuffer& Buffer);
/*
�����л���־����
*/
bool unserialLogMsg(const char* pData,int DataSize,LOGMSG& LogMsg);

class CLog : public CRefShare
{
public :
	CLog(string processname, string servername);
	~CLog();
	void fatal(const char* Format,...);
	void error(const char* Format,...);
	void warning(const char* Format,...);
	void info(const char* Format,...);
	void debug(const char* Format,...);
	void addOutput(IOutputStreamPtr Output);
	void setLogLevel(ELOGLEVEL level);
	uint8 getLogLevel();

private:
	void writeMessage(ELOGLEVEL nLogLevel,const char* pMessage);

private :
	//CLightLock m_Lock;
	//char m_Buffer[2048];
	//CBytesBufferPtr m_pBuffer;
	LOGMSG m_LogMsg;
	ELOGLEVEL m_nLevel;
	typedef vector<IOutputStreamPtr> OUTPUT_VEC;
	OUTPUT_VEC m_vOutputStream;
};

typedef CRefObject<CLog> CLogPtr;

#endif
