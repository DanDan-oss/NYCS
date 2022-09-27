#ifndef SERVER_PROTO_H
#define SERVER_PROTO_H

#include <zinx.h>

class CServerChanelData;
class CServerRole;
class CEncryAES;

/* 协议层
由于是通道层对象主动创建或者绑定协议层对象(数据由下层往上层传),
所有通道层和协议层对象之间的关系由通道层维护.
业务层由协议层维护并且初始化init(),协议层释放时将业务层释放Fini() */
class CServerProto : public Iprotocol
{
	friend class CServerChanelData;
	friend class CServerRole;

public:
	// 通过 Iprotocol 继承
	// 将客户端发送TCP消息转换成服务器要执行逻辑消息
	virtual UserData* raw2request(std::string _szInput) override;

	// 将服务器要执行完的逻辑消息转换成TCP消息(返回给客户端)
	virtual std::string* response2raw(UserData& _oUserData) override;

	// 处理客户端TCP消息逻辑的对象
	virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;

	// 返回客户端通道层对象
	virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

	CServerProto();
	CServerProto(CServerChanelData* _chanel);
	virtual ~CServerProto();

private:
	void SetChanel(CServerChanelData* _chanel);	// 绑定通道层对象

private:
	CServerChanelData* m_chanel = nullptr;		// 通道类
	CServerRole* m_pRole = nullptr;			// 业务类
	std::string m_MsgBuf;					// TCP消息缓存
};

#endif // !SERVER_PROTO_H