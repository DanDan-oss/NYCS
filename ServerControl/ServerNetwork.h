#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H

#include<ZinxTCP.h>

class CServerProto;

//class ExitTimer : public TimerOutProc
//{
//	// 通过 TimerOutProc 继承
//	virtual void Proc() override;
//	virtual int GetTimeSec() override;
//};


/* TCP消息通道层， socket
通道层创建时创建协议层,如果有传入协议层则与协议层互相绑定(数据由下层往上层传)
协议层由维护, 通道层释放时,自动释放绑定的协议层 */
class CServerChanelData : public ZinxTcpData
{
	friend class CServerProto;
public:
	// 通过 ZinxTcpData 继承

	//获取消息下一个处理对象: 协议层对象
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;

	CServerChanelData(int _fd);
	CServerChanelData(int _fd, CServerProto* _proto);	// 会与传入的_proto自动绑定
	virtual  ~CServerChanelData();

	void SetProto(CServerProto* _proto);		/* 绑定当前通道对象的 协议对象 */

private:
	CServerProto* m_proto = nullptr;	// 协议对象
};

/* 套接字工厂类*/
class CServerTcpFact : public IZinxTcpConnFact
{
	// 通过 IZinxTcpConnFact 继承
	virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};

#endif // !GAME_NETWORK_H





