#ifndef _CODEGEN_H_
#define _CODEGEN_H_

typedef struct _t_t_typerecord 
{
	const char *srctype;
	const char *destype;
}typerecord;

class ICodeGen
{
public :
	ICodeGen(){}
	virtual ~ICodeGen(){}

	//��ʼ��
	virtual void initialize(const char *desFile) = 0;
	//�ͷ���Դ
	virtual void release() = 0;
	//��������ʱ�ͷ���Դ
	virtual void releaseError() = 0;
	//��ʼ�����ļ�
	virtual void generateBegin() = 0;
	//�����ļ�
	virtual void generateFile() = 0;
	//���������ļ�
	virtual void generateEnd() = 0;

	//to do delete
	virtual void writeEnumHead(const char *enumName) = 0;
	virtual void writeEnumIV(const char *identifier, const char *value) = 0;
	virtual void writeEnumIVI(const char *identifier, int value) = 0;
	virtual void writeEnumEnd() = 0;
};

#endif
