#ifndef CLIENG_MSG_H
#define CLIENG_MSG_H

#include <iostream>
#include <google/protobuf/message.h>
class CClientSockt;

// 消息基类
class UserData
{
public:
	UserData() {}
	virtual ~UserData() {}

};

// 客户端自己的消息 与服务器消息的转换
class CClientMSG : public UserData
{
public:
	enum MSG_TYPE {		// 客户端服务器交互的消息类型
		MSG_TYPE_PANT = 0,			// 心跳包消息,或者无意义消息
		MSG_TYPE_LOG = 1,			// 登陆消息
		MSG_TYPE_RESPOND = 3
	};

	CClientMSG(CClientMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg);
	CClientMSG(CClientMSG::MSG_TYPE _type, std::string _stream);
	CClientMSG(const struct SOCKT_MSG* msg);

	virtual ~CClientMSG();

	// 进行数据序列化数据进字符串
	virtual std::string SerializeToString();


	// 进行数据序列化,字符串数据放进CClientSockt的缓冲区里面里面
	bool SerializeToMsg(struct SOCKT_MSG* _sokeMsg, CClientSockt* _pClientSock);

	// 获取protobuf消息的父类
	google::protobuf::Message* GetMessagePoint();

	// 获取当前消息的类型
	CClientMSG::MSG_TYPE GetMsgType();
private:
	MSG_TYPE m_msgType = MSG_TYPE::MSG_TYPE_PANT;			// 消息类型
	google::protobuf::Message* m_pMsg = nullptr;	// protobuf所有消息的父类

};

// 消息集合
class CMultClientMSG
{
public:
	std::list<CClientMSG*> m_pMsg;
	virtual ~CMultClientMSG();
};


#endif // !CLIENG_MSG_H

