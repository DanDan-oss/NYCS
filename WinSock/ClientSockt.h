#ifndef LIB_SOCKET_H
#define LIB_SOCKET_H

#include <iostream>
#include <google/protobuf/message.h>
#include <windows.h>

/* 定义动态类型转换后的引用类型，若转换失败则执行返回NULL */
#define GET_REF2DATA(type, ref, orig)  type * pref = dynamic_cast<type *>(&orig); if (nullptr == pref) {return nullptr;} type &ref = dynamic_cast<type&>(orig)

enum NETWORK_MSGTYPE;
struct SOCKT_MSG;

DWORD WINAPI RevcMessageThread(LPVOID lpThreeParameter);


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
	std::string SerializeToString();

	// 进行数据序列化,数据进struct SOCKT_MSG里面
	BOOL SerializeToMsg(struct SOCKT_MSG* _sokeMsg);

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

// 客户端网络通信协议类-->数据序列化
class CClientProto
{
public:
	CClientProto();
	// TCP数据消息 转换成 协议数据 (多条|集合), 返回list集合中还有有多少条协议数据未被处理
	virtual DWORD ResponseToRaw(std::string& _szInput, CMultClientMSG& _pMultMsg);

	// 协议数据(单条) 转换成 TCP数据消息
	virtual std::string RawToReques(UserData& _oUserData);
private:
	std::string m_MsgBuf;
};


// 客户端网络传输类
class CClientSockt :public CClientProto
{
	friend DWORD WINAPI RevcMessageThread(LPVOID lpThreeParameter);
public:
	CClientSockt();
	CClientSockt(std::string& _serAdder, const unsigned int _serPort);
	virtual ~CClientSockt();

	// socket初始化
	virtual BOOL Init(std::string& _serAdder, const unsigned int _serPort);

	// sokct关闭
	virtual VOID Fini();

	// 获取当前通信Socket
	virtual int GetFd();

	virtual BOOL SendSockMsg(const struct SOCKT_MSG* _sokeMsg);
	virtual BOOL RecvSockMsg(struct SOCKT_MSG* _sokeMsg);

private:


	// 接收数据
	virtual bool ReadFd(std::string& _input);

	// 发送 socket数据
	virtual bool WriteFd(std::string& _output);

private:
	SOCKET m_sockFd = 0;		// 当前socket 描述符--->socket句柄
	CMultClientMSG m_multMsg;
};





#endif // !LIB_SOCKET_H
