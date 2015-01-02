#include <list>
#include <string>

#include "codegen.h"

extern std::list<std::string> g_lstPrecompile;

class CCPPGen : public ICodeGen
{
public :
	CCPPGen();
	virtual ~CCPPGen();

	//��ʼ��
	virtual void initialize(const char *desFile);
	//�ͷ���Դ
	virtual void release();
	//��������ʱ�ͷ���Դ
	virtual void releaseError();
	//��ʼ�����ļ�
	virtual void generateBegin();
	//�����ļ�
	virtual void generateFile();
	//���������ļ�
	virtual void generateEnd();

	//todo
	void writeEnumHead(const char *enumName);
	void writeEnumIV(const char *identifier, const char *value);
	void writeEnumIVI(const char *identifier, int value);
	void writeEnumEnd();

private :
	void writeVariable();
	void serialFunc();
	void unserialFunc();
	std::string getMarco();
	void generateConstuctor();
	void generateCopyConstructor();
	void generateAssignmentConstructor();
	void generateDestructor();

private :
	FILE* m_pDesFile;
	std::string m_strDesFile;
	//�ָ��
	char m_Seperator;
};
