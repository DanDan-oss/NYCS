// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include "dllmain.h"
#include "ClientSockt.h"
#include "encry.h"
#include "msg.pb.h"
#include <windows.h>


#ifdef _DEBUG
#define WinPrintA OutputDebugStringFA
#define WinPrintW OutputDebugStringFW
#else
#define WinPrintA
#define WinPrintW
#endif
void  __stdcall OutputDebugStringFA(const char* format, ...);
void __stdcall OutputDebugStringFW(const wchar_t* format, ...);


#define SEVER_ADDRES "118.25.250.33"
#define SERVER_PORT 7788

CClientSockt* pSock = nullptr;

bool __stdcall InitSocketLib()
{
	std::string serverAddr = SEVER_ADDRES;
	unsigned int serverPort = SERVER_PORT;

	pSock = new CClientSockt();
	if (FALSE == pSock->Init(serverAddr, serverPort))
	{
		WinPrintA("[dbg]:服务器连接失败");
		delete pSock;
		pSock = nullptr;
		return FALSE;
	}

	WinPrintA("[dbg]:服务器连接成功");
	return TRUE;
}

unsigned int __stdcall GetSockMsgError()
{
	return 0;
}

bool __stdcall SendSockMessage(const struct SOCKT_MSG* _pSockMsg)
{
	if (!pSock)
		return FALSE;
	return pSock->SendSockMsg(_pSockMsg);
}

bool __stdcall RecvSockMessage(struct SOCKT_MSG* _sockMsg)
{
	if (!pSock)
		return FALSE;
	return pSock->RecvSockMsg(_sockMsg);
}

unsigned int __stdcall GetRecvOutstandMsg()
{
	return 0;
}





void __cdecl OutputDebugStringFA(const char* format, ...)
{
	va_list v1Args;
	char* strBuffer = (char*)GlobalAlloc(GPTR, 4009);

	va_start(v1Args, format);
	_vsnprintf(strBuffer, 4096 - 1, format, v1Args);
	va_end(v1Args);
	strcat(strBuffer, "\n");
	OutputDebugStringA(strBuffer);
	GlobalFree(strBuffer);
	return;
}

void __cdecl OutputDebugStringFW(const wchar_t* format, ...)
{
	va_list v1Args;
	wchar_t* strBuffer = (wchar_t*)GlobalAlloc(GPTR, 4009);

	va_start(v1Args, format);
	_vswprintf_c(strBuffer, 4096 - 1, format, v1Args);
	va_end(v1Args);
	wcscat(strBuffer, L"\n");
	OutputDebugStringW(strBuffer);
	GlobalFree(strBuffer);
	return;
}

