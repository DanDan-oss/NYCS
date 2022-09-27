#ifndef LIB_SOCKET_H
#define LIB_SOCKET_H
#include <iostream>
#include "msg.pb.h"
#pragma comment(lib, "ws2_32.lib")

enum NETWORK_MSGTYPE
{
	MSG_TYPE_PANT = 0,			// 心跳包消息,或者无意义消息
	MSG_TYPE_LOG = 1			// 登陆消息
};


class NetMSG
{
public:
	enum MSG_TYPE {		// 客户端服务器交互的消息类型
		MSG_TYPE_PANT = 0,			// 心跳包消息,或者无意义消息
		MSG_TYPE_LOG = 1			// 登陆消息
	};

	NetMSG(NetMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg);
	NetMSG(NetMSG::MSG_TYPE _type, std::string& _stream);
	virtual ~NetMSG();

	std::string Serialize();
	google::protobuf::Message* GetMessagePoint();
	NetMSG::MSG_TYPE GetMsgType();
private:
	MSG_TYPE m_msgType;			// 消息类型
	google::protobuf::Message* m_pMsg = nullptr;	// protobuf所有消息的父类
};




class CNetSockt
{
public:
	CNetSockt();
	CNetSockt(std::string& _serAdder, const unsigned int _serPort);
	~CNetSockt();

	BOOL InitWin32Socket(std::string& _serAdder, const unsigned int _serPort);
	BOOL SendNetMessage(std::string& _msg);
	DWORD RecvNetMessage(std::string& _recvStrMsg);

private:
	SOCKET m_sockFd = 0;
	char* m_recvBuf = nullptr;

};

#endif // !LIB_SOCKET_H
