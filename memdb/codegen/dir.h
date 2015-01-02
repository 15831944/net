#ifndef _DIR_H_
#define _DIR_H_
#include <string>
using namespace std;
#ifdef WIN32
//#include <winbase.h>
#include <windows.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#ifdef WIN32
#define DIR_HANDLE HANDLE
#define STAT_DATA WIN32_FIND_DATA
#else
#define DIR_HANDLE DIR*
#define STAT_DATA struct stat
#endif

#ifndef ulong64
typedef struct _tULong64
{
	unsigned long HighFileSize;
	unsigned long LowFileSize;
}ulong64;
#endif

enum EDIRTYPE
{
	DIR_INEXISTENCE,
	DIR_FILE,
	DIR_DIRECTORY,
};

//����Ŀ¼���࣬����ʵ�ֲ�ѯĳ��Ŀ¼�µ��ļ���
class CDir
{
private :
	bool bOpened;
	bool bNext;
	string strDirectory;
	string strFileName;
	DIR_HANDLE dp;
	STAT_DATA fp;

public :
	CDir();
	~CDir();
	//��ָ����Ŀ¼
	bool OpenDirectory(const char* pathname);
	//��ѯ�Ƿ����ļ�
	bool HasNextFile();
	//��ȡ��һ���ļ������֣�������·��
	string NextFile();
	//���ص�ǰ�ļ��Ƿ���Ŀ¼
	bool isDirectory();
	//���ص�ǰ�ļ��Ƿ����ļ�
	bool isFile();
	//���ص�ǰ�ļ�����Ŀ¼
	string GetDirectory();
	//��ȡ��ǰ�ļ��Ĵ�С
	ulong64 GetFileSize();
	//�ж�����
	static int GetNodeType(const char* pathname);
	//�ݹ鴴��Ŀ¼
	static void MakeDirectory(const char *pathname);
	void Close();
};

class CFileFilter
{
private :
	string filematchname;

public :
	CFileFilter(const string& filter);
	~CFileFilter();
	bool isMatch(const string& filename);
};
#endif
