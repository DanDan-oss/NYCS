#ifndef SERVER_MSG_H
#define SERVER_MSG_H

#include <zinx.h>
#include <google/protobuf/message.h>

class CSocketMSG :public UserData
{
public:
	enum MSG_TYPE {		// 客户端服务器交互的消息类型
		MSG_TYPE_PANT = 0,			// 心跳包消息,或者无意义消息
		MSG_TYPE_LOG = 1,			// 登陆消息
		MSG_TYPE_Respond = 3
	};

public:
	CSocketMSG(CSocketMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg);
	CSocketMSG(CSocketMSG::MSG_TYPE _type, std::string _stream);
	virtual ~CSocketMSG();

	// 序列化成字符串(发送TCP消息时可以共)
	std::string Serialize();
	google::protobuf::Message* GetMessagePoint();
	virtual CSocketMSG::MSG_TYPE GetMsgType();

private:
	MSG_TYPE m_msgType;			// 消息类型
	google::protobuf::Message* m_pMsg = nullptr;	// protobuf所有消息的父类
};

// 消息集合
class CMultServerMSG :public UserData
{
public:
	std::vector<CSocketMSG*> m_pMsg;
	virtual ~CMultServerMSG();
};

#endif // SERVER_MSG_H

