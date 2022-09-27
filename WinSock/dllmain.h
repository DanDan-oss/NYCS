#ifndef DLL_EXPORT_H
#define DLL_EXPORT_H
#include <windows.h>
#include <iostream>

extern "C" __declspec(dllexport) BOOL  InitNetLib();
extern "C" __declspec(dllexport) BOOL  PlayerLogin(_In_ std::string & _UserName, _In_ std::string & _Pass);

//extern "C" BOOL CALLBACK  SendSocketMsg(NETWORK_MSGTYPE _msgType,_In_ std::string& _strMsg);
//extern "C" DWORD CALLBACK  RecvSocketMsg(_Out_ NETWORK_MSGTYPE _msgType, _Out_ std::string& _recvStrMsg);

#endif 