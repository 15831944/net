#include <vector>
#include <string>

#include "codegen.h"

extern std::vector<std::string> g_importVec;

class CASGen : public ICodeGen
{
public :
	CASGen();
	virtual ~CASGen();

	//��ʼ��
	virtual void initialize(const char *desFile);
	//�ͷ���Դ
	virtual void release();
	//��������ʱ�ͷ���Դ
	virtual void releaseError();
	//��ʼ�����ļ�
	virtual void generateBegin(){}
	//�����ļ�
	virtual void generateFile();
	//���������ļ�
	virtual void generateEnd(){}

	//todo
	void writeEnumHead(const char *enumName);
	void writeEnumIV(const char *identifier, const char *value);
	void writeEnumIVI(const char *identifier, int value);
	void writeEnumEnd();

private :
	void writeVariable();
	void serialFunc();
	void unserialFunc();

private :
	FILE* m_pDesFile;
	//·��
	std::string m_strPath;
	//�ָ���
	char m_cSeparator;
};