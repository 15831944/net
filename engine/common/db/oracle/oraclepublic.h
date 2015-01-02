#ifndef _ORACLEDBPUBLIC_H_
#define _ORACLEDBPUBLIC_H_

#ifdef WIN32
#pragma warning(disable:4250)
#pragma warning(disable:4244)
#pragma warning(disable:4715)
#pragma warning(disable:4150) // Warning ****************
#endif

#include <common.h>
#include <util/autoptr.h>
#include <util/userexception.h>
#include <util/datetime.h>
#include <db/dbpublic.h>
#include <db/sqlexception.h>
#define WIN32COMMON
#include <occi.h>
using namespace oracle::occi;
#include <db/oracle/refstatement.h>
namespace avocado_oracle
{
//
#define BYTES_MAX_SIZE	512
#define STRING_MAX_SIZE 512

/*
 *	����OCCI DB ��װ��� ������
 *  @x ���Եĺ�����������ʱ��ʾ
 */
#define BEGIN_TEST_TRY(x) \
	try \
{ \

#define END_TEST_TRY(x) \
} \
catch (CSQLException& e) \
{ \
	printf("catch a CSQLException - Method:" #x "(), %s\n", e.what()); \
} \
catch (oracle::occi::SQLException& e) \
{ \
	printf("Error - catch a oracle::occi::SQLException - Method:" #x "(), %s;\n", e.what()); \
	CU_ASSERT(false); \
} \
catch (std::exception& e) \
{ \
	printf("Error - catch a std::exception - Method:" #x "(), %s\n", e.what()); \
	CU_ASSERT(false); \
} \
catch (...) \
{ \
    printf("Error - catch a unhandle exception - Method:" #x "();\n"); \
	CU_ASSERT(false); \
} \

/*
 *����oracle�쳣���׳��ڲ��쳣�ĺ�
 */
#define BEGIN_DBTRY \
try \
{\

#define END_DBTRY \
}\
catch (oracle::occi::SQLException& e)\
{\
	throw CSQLException(e.getErrorCode(), e.what());\
}\

// database������
/*
class SQLTYPE
{
public:
	static const int SQL_INT;
	static const int SQL_VARCHAR;
	static const int SQL_DATE;
	static const int SQL_TIME;
	static const int SQL_TIMESTAMP;
};
*/

//framework����������oracle�������͵�ת��
oracle::occi::Type toOracleType(EDATATYPE nType);

//OCCI������framework���͵�ת��
EDATATYPE toOracleType(oracle::occi::Type oraType);

//framework����������OCCI���͵�ת��
oracle::occi::Type toOcciType(EDATATYPE nType);

//OCCI������db�����͵�ת��
EDATATYPE toOcciType(oracle::occi::Type oraType);


//Bytes �� Long64 ������ת��
Long64 bytesToLong64(Bytes& bt);

/*
 *
 *�ַ�����Сдת��
 *
const char* strToUpper(const char* pSrc);
const char* strtoLower(const char* pSrc);
*/
}
#endif 
