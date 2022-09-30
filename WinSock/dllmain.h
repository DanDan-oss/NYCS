#ifndef DLL_EXPORT_H
#define DLL_EXPORT_H

enum NETWORK_MSGTYPE
{
	MSG_TYPE_PANT = 0,			// 心跳包消息,或者无意义消息
	MSG_TYPE_LOG = 1,			// 登陆消息
	MSG_TYPE_RESPOND = 3		// 服务器返回的消息

};

extern "C" __declspec(dllexport) struct SOCKT_MSG
{
	// 如果是接收服务器发送的消息根据这个参数转换消息结构体, 如果此参数为0表示当前还没有socket消息
	NETWORK_MSGTYPE type;			// 消息类型
	union
	{
		struct  
		{
			char* userName;		// 用户名称
			char* userPass;		// 用户密码
		}PlayerLogin;

		struct
		{
			bool rv;					// 服务器请求状态
			unsigned int msgType;		// 请求类型
			char* recvData;		// 服务器请求消息
		} ServerRecv;
	};
};

extern "C" __declspec(dllexport) bool __stdcall InitSocketLib();
extern "C" __declspec(dllexport) bool __stdcall SendSockMessage(const struct SOCKT_MSG* _pSockMsg);
extern "C" __declspec(dllexport) bool __stdcall RecvSockMessage(struct SOCKT_MSG* _sockMsg);
extern "C" __declspec(dllexport) unsigned int __stdcall GetRecvOutstandMsg();		// 获取消息队列中还有多少条消息未被处理
extern "C" __declspec(dllexport) unsigned int __stdcall GetSockMsgError();		// 获取错误代码

//extern "C" BOOL CALLBACK  SendSocketMsg(NETWORK_MSGTYPE _msgType,_In_ std::string& _strMsg);
//extern "C" DWORD CALLBACK  RecvSocketMsg(_Out_ NETWORK_MSGTYPE _msgType, _Out_ std::string& _recvStrMsg);

#endif 