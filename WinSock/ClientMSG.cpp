#include "ClientMSG.h"
#include "msg.pb.h"
#include "dllmain.h"
#include "ClientSockt.h"
#include <windows.h>
#include <iostream>
#include <memory>

// =============================== 消息类
CClientMSG::CClientMSG(CClientMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg)
	:m_msgType(_type)
{
	switch (_type)
	{
	case MSG_TYPE::MSG_TYPE_PANT:
		return;
		break;
	case MSG_TYPE::MSG_TYPE_LOG:
		this->m_pMsg = new PlayerLoginMsg(dynamic_cast<PlayerLoginMsg&>(*_pMsg));
		break;
	case MSG_TYPE::MSG_TYPE_RESPOND:
		this->m_pMsg = new ServerRespondMsg(dynamic_cast<ServerRespondMsg&>(*_pMsg));
	default:
		return;
	}
	return;
}

CClientMSG::CClientMSG(CClientMSG::MSG_TYPE _type, std::string _stream)
	:m_msgType(_type)
{
	switch (_type)
	{
	case MSG_TYPE::MSG_TYPE_PANT:
		return;
	case CClientMSG::MSG_TYPE_LOG:
		this->m_pMsg = new PlayerLoginMsg();
		break;
	case CClientMSG::MSG_TYPE_RESPOND:
		this->m_pMsg = new ServerRespondMsg();
		break;
	default:
		return;
	}
	this->m_pMsg->ParseFromString(_stream);
}

CClientMSG::CClientMSG(const struct SOCKT_MSG* msg)
{
	if (msg == nullptr)
		return;
	switch (msg->type)
	{
	case NETWORK_MSGTYPE::MSG_TYPE_PANT:
		return;
	case NETWORK_MSGTYPE::MSG_TYPE_LOG:
	{
		this->m_msgType = MSG_TYPE::MSG_TYPE_LOG;
		this->m_pMsg = new PlayerLoginMsg();
		auto pCache = dynamic_cast<PlayerLoginMsg*>(this->m_pMsg);
		pCache->set_msgtype(MSG_TYPE::MSG_TYPE_LOG);
		pCache->set_username(msg->PlayerLogin.userName);
		pCache->set_userpass(msg->PlayerLogin.userPass);
		return;
	}
	case NETWORK_MSGTYPE::MSG_TYPE_RESPOND:
	{
		this->m_msgType = MSG_TYPE::MSG_TYPE_RESPOND;
		this->m_pMsg = new ServerRespondMsg();
		auto pCache = dynamic_cast<ServerRespondMsg*>(this->m_pMsg);
		pCache->set_msgtype(MSG_TYPE::MSG_TYPE_RESPOND);
		pCache->set_rv(msg->ServerRecv.rv);
		pCache->set_data(msg->ServerRecv.recvData);
		return;
	}
	default:
		return;
	}
}

CClientMSG::~CClientMSG()
{
	if (this->m_pMsg)
		delete this->m_pMsg;
}



bool CClientMSG::SerializeToMsg(struct SOCKT_MSG* _sokeMsg, CClientSockt* _pClientSock)
{
	BOOL bRet = false;
	// 将存储的字符串结构放到CClientSockt的string缓冲区

	if ((!_sokeMsg) || (!_pClientSock))
		return FALSE;

	// 根据消息类型将传进来的参数赋值,将 参数成员变量是char*的赋值给_pClientSock的成员string缓冲区
	switch (this->m_msgType)
	{
	case MSG_TYPE::MSG_TYPE_PANT:
		_sokeMsg->type = NETWORK_MSGTYPE::MSG_TYPE_PANT;
		return TRUE;
	case MSG_TYPE::MSG_TYPE_LOG:
	{
		auto pCache = dynamic_cast<PlayerLoginMsg*>(this->m_pMsg);
		_sokeMsg->type = NETWORK_MSGTYPE::MSG_TYPE_LOG;

		_pClientSock->m_strCache[0] = pCache->username();
		_pClientSock->m_strCache[1] = pCache->userpass();

		_sokeMsg->PlayerLogin.userName = (char*)(_pClientSock->m_strCache[0].data());
		_sokeMsg->PlayerLogin.userPass = (char*)(_pClientSock->m_strCache[1].data());
		return TRUE;
	}
	case MSG_TYPE::MSG_TYPE_RESPOND:
	{

		auto pCache = dynamic_cast<ServerRespondMsg*>(this->m_pMsg);
		_sokeMsg->type = NETWORK_MSGTYPE::MSG_TYPE_RESPOND;
		_sokeMsg->ServerRecv.rv = pCache->rv();

		_pClientSock->m_strCache[0] = pCache->data();
		_sokeMsg->ServerRecv.recvData = (char*)(_pClientSock->m_strCache[0].data());
		return TRUE;
	}
	default:
		return FALSE;
	}
}

std::string CClientMSG::SerializeToString()
{
	std::string str;
	if (!this->m_pMsg)
		return str;
	this->m_pMsg->SerializeToString(&str);
	return str;
}




google::protobuf::Message* CClientMSG::GetMessagePoint()
{
	return this->m_pMsg;
}

CClientMSG::MSG_TYPE CClientMSG::GetMsgType()
{
	return this->m_msgType;
}


CMultClientMSG::~CMultClientMSG()
{
	// 释放消息集
	for (CClientMSG* pMsg : this->m_pMsg)
		delete pMsg;
}
