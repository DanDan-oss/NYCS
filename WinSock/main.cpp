#include "dllmain.h"
#include <iostream>
#include <windows.h>

int main()
{
	bool initNo = InitSocketLib();


	struct SOCKT_MSG msg;
	char* username = new char[500];
	char* userpass = new char[500];
	memset(username, 0, sizeof(username));
	memset(userpass, 0, sizeof(userpass));
	strcpy(username, "asdsadsaka");
	strcpy(userpass, "dsadsadsada");
	msg.type = NETWORK_MSGTYPE::MSG_TYPE_LOG;
	msg.PlayerLogin.userName = (char*)username;
	msg.PlayerLogin.userPass = (char*)userpass;

	bool sendNo = SendSockMessage(&msg);
	Sleep(10);

	system("pause");

	memset(&msg, 0, sizeof(msg));
	bool recvNo = RecvSockMessage(&msg);


	system("pause");
}