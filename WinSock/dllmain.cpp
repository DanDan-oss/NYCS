// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include "dllmain.h"
#include "netsock.h"
#include "encry.h"

#define SEVER_ADDRES "118.25.250.33"
#define SERVER_PORT 7788

#ifdef _DEBUG
#define WinPrintA OutputDebugStringFA
#define WinPrintW OutputDebugStringFW
#else
#define WinPrintA
#define WinPrintW
#endif
void  __stdcall OutputDebugStringFA(const char* format, ...);
void __stdcall OutputDebugStringFW(const wchar_t* format, ...);



CNetSockt* pSock = nullptr;
CEncryAES* pAes = nullptr;;


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		pSock = new CNetSockt();
		pAes = new CEncryAES("qiancheng2");
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if (pSock)
			delete pSock;
		if (pAes)
			delete pAes;
		break;
	}
	return TRUE;
}



BOOL   InitNetLib()
{
	std::string serAddr = SEVER_ADDRES;
	unsigned int serPort = SERVER_PORT;
	if (FALSE == pSock->InitWin32Socket(serAddr, serPort))
	{
		WinPrintA("[dbg]:服务器连接失败");
		return FALSE;
	}
	WinPrintA("[dbg]:服务器连接成功");
	return TRUE;

}


BOOL  PlayerLogin(_In_ std::string& _UserName, _In_ std::string& _Pass)
{
	PlayerLoginMsg* pMsg = new PlayerLoginMsg();
	RespondMsg* pRecvMsg = nullptr;
	std::string strMsg;

	BOOL blStat = FALSE;
	if (!pMsg)
	{
		WinPrintA("[dbg]: 生成待发送的网络消息失败-->PlayerLoginMsg");
		return FALSE;
	}
	pMsg->set_msgtype(NETWORK_MSGTYPE::MSG_TYPE_LOG);
	pMsg->set_username(pAes->AesCbcEncrypt(_UserName));		// 账户名
	pMsg->set_userpass(pAes->AesCbcEncrypt(_Pass));			// 密码
	pMsg->SerializeToString(&strMsg);	// 序列化

	if (0 >= strMsg.size())
	{
		delete pMsg;
		WinPrintA("[dbg]: 序列化待发送的网络消息失败");
		return FALSE;
	}
	pSock->SendNetMessage(strMsg);
	for (int timeOut = 0; timeOut <= 10; ++timeOut)
	{
		if (0 != pSock->RecvNetMessage(strMsg))
		{
			Sleep(1000);
			continue;
		}
		pRecvMsg = new RespondMsg();
		pRecvMsg->ParseFromString(strMsg);
		if (TRUE == pRecvMsg->rv() && pRecvMsg->msgtype() == NETWORK_MSGTYPE::MSG_TYPE_LOG)
			blStat = TRUE;
		break;
	}
#ifdef _DEBUG
	if (blStat)
		WinPrintA("[dbg]: 网络验证成功,可以登录");
	else
		WinPrintA("[dbg]: 网络验证失败, 账号或者密码可能不对");
#endif
	if (pMsg)
		delete pMsg;
	if (pRecvMsg)
		delete pRecvMsg;
	return blStat;
}


BOOL CALLBACK  SendSocketMsg(NETWORK_MSGTYPE _msgType, _In_ std::string& _strMsg)
{
	return 0;
}

DWORD CALLBACK  RecvSocketMsg(_Out_ NETWORK_MSGTYPE _msgType, _Out_ std::string& _recvStrMsg)
{
	return 0;
}
DWORD CALLBACK  RecvSocketMsg(std::string& _recvStrMsg)
{

	//std::string recvMsg = cNetSock.RecvNetMessage();
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

