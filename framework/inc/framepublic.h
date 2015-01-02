#ifndef _FRAMEPUBLIC_H_
#define _FRAMEPUBLIC_H_

#include <public.h>
#include <util/getopt.h>
#include <io/log.h>
#include <io/binlog.h>
#include <util/profile.h>

extern DLLAPI CLogPtr g_Log;
#ifdef WIN32
#define log_debug(fmt, ...) g_Log->debug(fmt, __VA_ARGS__)
#define log_error(fmt, ...) g_Log->error(fmt, __VA_ARGS__)
#define log_fatal(fmt, ...) g_Log->fatal(fmt, __VA_ARGS__)
#define log_info(fmt, ...) g_Log->info(fmt, __VA_ARGS__)
#define log_warning(fmt, ...) g_Log->warning(fmt, __VA_ARGS__)
#else
#define log_debug(fmt, ...) g_Log->debug(fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) g_Log->error(fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) g_Log->fatal(fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) g_Log->info(fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) g_Log->warning(fmt, ##__VA_ARGS__)
#endif

extern const struct option default_opts[];

ELOGLEVEL getLogLevel(const char* LogLevel);

/*
  ���������в���
  @return int ���ػ�ȡ�Ĳ�������
*/
int parseCmdArgs(int argc,char** argv,const struct option opts[]=default_opts);

/**
 * ��ȡ�����в���
 * @param nOpt �̲�����
 * @param Value ����ֵ���������
 * @return bool �Ƿ��ȡ�ɹ������ʧ�ܣ���Value��ֵΪNULL
*/
bool getCmdArgs(int nOpt,const char*& Value);

/**
 * ��ʼ��ҵ���ܰ�
 * ��ʼ����־(log)����ʼ�������ļ�(profile)
 */
bool initFrameWorkLib(const char* ProcessName);
/*
 ��ʼ����־��������parseCmdArgs����ã�����᷵��ʧ��
 ��ʹ�ò���-l��ָ������Ϊ��־·��
*/
bool initFrameWorkLog(const char* ProcessName);


/**
 * �ͷ�ҵ���ܰ�������������Դ
 */
void uninitFrameWorkLib();

/**
 * ��ȡ�����ļ�
 */
CProfile* getFrameWorkProf();
/**
 * ��ʼ�������ļ�
 */
bool initFrameWorkProf();
/**
 * ����װ�������ļ��������ȵ���initFrameWorkProf
 */
bool reloadFrameWorkProf();
/**
 * �Ƿ������ļ��и���
 */
bool isFrameWorkProfUpdated();

#endif
