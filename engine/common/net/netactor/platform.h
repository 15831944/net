#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "netactordriver.h"
/*
  ʹ�ù���ģʽ����ƽ̨�Ĳ����Ժ͸���netactor
*/
class CNetActorDriverFactory
{
public :
	enum 	NETACTOR_TYPE
	{
		NETACTOR_REACTOR,
		NETACTOR_PROACTOR
	};
	static INetActorDriver* createNetActorDriver(NETACTOR_TYPE Type);
};

#endif
