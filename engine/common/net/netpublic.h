#ifndef _NETPUBLIC_H_
#define _NETPUBLIC_H_

#include <public.h>

enum EERRCODE
{
	//ϵͳ����
	E_SYSTEM = -2,
	E_INVAL = -3,
	E_NOMEM = -4,
};

enum ECOMMMODE
{
    ASYNC_MODE,
    SYNC_MODE
};

//��������
/**
 * ����socket������ģʽ
 */
int32 setSocketMode(SOCKET Sock, ECOMMMODE Mode);
/**
 * ��ӡ��������Ϣ
 * @param pBuf Ҫ��ӡ�Ļ�������ʼ��ַ
 * @param nSize �������Ĵ�С
 * @param pContext ��ӡ�ı�ʶ�����ڴ�ӡǰ�����α�ʶ��ӡ�������������
 */
void printfBinBuffer(const char* pBuf, uint32 nSize, const char* pContext = "");

#define DEFAULT_BACKLOG 1024

#ifdef WINDOWS
	#include <winsock2.h>
	typedef int socklen_t;
	#define RETURN_ERROR return WSAGetLastError()
	#define SHUT_RDWR SD_BOTH
#else
	#include <sys/types.h> // for sockaddr_in
	#include <arpa/inet.h> // for inet_addr()
	#include <netinet/in.h> // for netinet/in.h
	#include <sys/epoll.h>
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define RETURN_ERROR return errno
#endif

#endif
