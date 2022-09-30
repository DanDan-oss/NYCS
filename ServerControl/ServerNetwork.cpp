#include "ServerNetwork.h"
#include "ServerProto.h"
#include <zinx.h>
//#define  GAME_TIMEOUT_EXIT 5
//
//// =============================
//// ʱ����
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
// TCP��Ϣͨ���㣬 socket


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
	* // �����һ���ͻ��˶Ͽ����Ӻ�,�رշ����
	if (ZinxKernel::Zinx_GetAllRole().size() < 1)
		ZinxKernel::Zinx_Exit();
		*/
	//if (ZinxKernel::Zinx_GetAllRole().size() < 1)
	//	TimerManage::GetInstance()->AddTask(&g_exit_time);
}

AZinxHandler* CServerChanelData::GetInputNextStage(BytesMsg& _oInput)
{
	// ���õ�ǰͨ���������һ����Ϣ�������Ϊ�󶨵�Э�����
	return this->m_proto;
	//return this->m_proto;
}

void CServerChanelData::SetProto(CServerProto* _proto)
{
	this->m_proto = _proto;
}

// ==============================
// �׽��ֹ�����
ZinxTcpData* CServerTcpFact::CreateTcpDataChannel(int _fd)
{
	CServerChanelData* pChanel = new CServerChanelData(_fd);
	CServerProto* pProto = new CServerProto(pChanel);
	pChanel->SetProto(pProto);
	std::cout << "new connect " << std::endl;
	// ��Э�������ӵ�Э������,�����Ժ����
	ZinxKernel::Zinx_Add_Channel(*pChanel);
	ZinxKernel::Zinx_Add_Proto(*pProto);
	return pChanel;
}


