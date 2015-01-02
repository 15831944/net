#ifndef _GENGLOBAL_H_
#define _GENGLOBAL_H_

#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <string>

#include "codegen.h"

#ifdef __cplusplus
extern "C"{
#endif

//��ǰĿ¼
extern char pwd[256];

extern std::list<std::string> g_lstPrecompile;
extern std::vector<std::string> g_importVec;
extern std::list<ICodeGen*> g_lstCodeGen;

const char* transform(const char *keyword, typerecord *pTable);

//�ļ��Ѵ��ڣ��Ƿ񸲸�
void coverOrNot(const char *filename);

#ifdef __cplusplus
}
#endif

#endif