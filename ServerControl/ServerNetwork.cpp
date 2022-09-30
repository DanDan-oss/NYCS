#include "ServerNetwork.h"
#include "ServerProto.h"
#include <zinx.h>
//#define  GAME_TIMEOUT_EXIT 5
//
//// =============================
//// 时间轮
//void ExitTimer::Proc()
//{
//	ZinxKernel::Zinx_Exit();
//}
//
//int ExitTimer::GetTimeSec()
//{
//	return GAME_TIMEOUT_EXIT;
//}

// ==============================
//static ExitTimer g_exit_time;
// TCP消息通道层， socket


CServerChanelData::CServerChanelData(int _fd)
	: ZinxTcpData(_fd)
{
	//if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
	//	TimerManage::GetInstance()->DelTask(&g_exit_time);
}


CServerChanelData::CServerChanelData(int _fd, CServerProto* _proto)
	: ZinxTcpData(_fd), m_proto(_proto)
{
	_proto->SetChanel(this);

	//if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
	//	TimerManage::GetInstance()->DelTask(&g_exit_time);
}

CServerChanelData::~CServerChanelData()
{
	if (this->m_proto)
	{
		ZinxKernel::Zinx_Del_Proto(*(this->m_proto));
		delete this->m_proto;
	}

	/*
	* // 当最后一个客户端断开链接后,关闭服务端
	if (ZinxKernel::Zinx_GetAllRole().size() < 1)
		ZinxKernel::Zinx_Exit();
		*/
	//if (ZinxKernel::Zinx_GetAllRole().size() < 1)
	//	TimerManage::GetInstance()->AddTask(&g_exit_time);
}

AZinxHandler* CServerChanelData::GetInputNextStage(BytesMsg& _oInput)
{
	// 设置当前通道对象的下一个消息处理对象为绑定的协议对象
	return this->m_proto;
	//return this->m_proto;
}

void CServerChanelData::SetProto(CServerProto* _proto)
{
	this->m_proto = _proto;
}

// ==============================
// 套接字工厂类
ZinxTcpData* CServerTcpFact::CreateTcpDataChannel(int _fd)
{
	CServerChanelData* pChanel = new CServerChanelData(_fd);
	CServerProto* pProto = new CServerProto(pChanel);
	pChanel->SetProto(pProto);
	std::cout << "new connect " << std::endl;
	// 将协议对象添加到协议链表,方便以后查找
	ZinxKernel::Zinx_Add_Channel(*pChanel);
	ZinxKernel::Zinx_Add_Proto(*pProto);
	return pChanel;
}


