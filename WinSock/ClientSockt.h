#ifndef LIB_SOCKET_H
#define LIB_SOCKET_H

#include <iostream>
#include <windows.h>
#include "ClientMSG.h"

class CClientMSG;
struct SOCKT_MSG;

/* 定义动态类型转换后的引用类型，若转换失败则执行返回NULL */
#define GET_REF2DATA(type, ref, orig)  type * pref = dynamic_cast<type *>(&orig); if (nullptr == pref) {return nullptr;} type &ref = dynamic_cast<type&>(orig)



DWORD WINAPI RevcMessageThread(LPVOID lpThreeParameter);

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
	friend class CClientMSG;
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
	std::string m_strCache[10];			// 临时存放服务器回复消息转换后的字符串集
};


#endif // !LIB_SOCKET_H

