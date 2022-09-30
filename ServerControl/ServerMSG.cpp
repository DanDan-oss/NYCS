#include "ServerMSG.h"
#include "msg.pb.h"

CSocketMSG::CSocketMSG(CSocketMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg)
	: m_msgType(_type)
{
	switch (_type)
	{
	case MSG_TYPE::MSG_TYPE_LOG:
		this->m_pMsg = new PlayerLoginMsg(dynamic_cast<PlayerLoginMsg&>(*_pMsg));
		break;
	case MSG_TYPE::MSG_TYPE_Respond:
		this->m_pMsg = new ServerRespondMsg(dynamic_cast<ServerRespondMsg&>(*_pMsg));
	default:
		break;
	}
}

CSocketMSG::CSocketMSG(CSocketMSG::MSG_TYPE _type, std::string _stream)
	:m_msgType(_type)
{
	switch (_type)
	{
	case MSG_TYPE::MSG_TYPE_LOG:
		this->m_pMsg = new PlayerLoginMsg();
		break;
	case MSG_TYPE::MSG_TYPE_Respond:
		this->m_pMsg = new ServerRespondMsg();
	default:
		return;
	}
	this->m_pMsg->ParseFromString(_stream);
}

CSocketMSG::~CSocketMSG()
{
	if (this->m_pMsg)
		delete this->m_pMsg;
}

std::string CSocketMSG::Serialize()
{
	std::string str;
	this->m_pMsg->SerializeToString(&str);
	return str;

}

google::protobuf::Message* CSocketMSG::GetMessagePoint()
{
	return this->m_pMsg;
}

CSocketMSG::MSG_TYPE CSocketMSG::GetMsgType()
{
	return this->m_msgType;
}

// ============================
CMultServerMSG::~CMultServerMSG()
{
	// 释放消息集
	for (CSocketMSG* pMsg : this->m_pMsg)
		delete pMsg;
}

