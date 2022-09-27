#include "ServerProto.h"
#include "ServerMSG.h"
#include "ServerRole.h"
#include "ServerNetwork.h"


CServerProto::CServerProto()
{
	this->m_pRole = new CServerRole(this);
	ZinxKernel::Zinx_Add_Role(*(this->m_pRole));
}

CServerProto::CServerProto(CServerChanelData* _chanel)
	: m_chanel(_chanel)
{
	this->m_pRole = new CServerRole(this);
	ZinxKernel::Zinx_Add_Role(*(this->m_pRole));
}

CServerProto::~CServerProto()
{
	if (this->m_pRole)
	{
		ZinxKernel::Zinx_Del_Role(*(this->m_pRole));
		delete this->m_pRole;
	}

}


UserData* CServerProto::raw2request(std::string _szInput)
{

	int iMsgType = 0;
	int iMsgSize = 0;
	CSocketMSG* pMsg = nullptr;;
	std::string strCache;		// 缓存,临时存放截取的消息
	CMultServerMSG* pMultMsg = nullptr;

	// TODO: 将TCP消息转换成游戏逻辑消息 && TCP粘包处理
	this->m_MsgBuf.append(_szInput);
	while (this->m_MsgBuf.size() > 8)
	{
		// 序列化数据长度
		iMsgSize = 0;
		iMsgSize |= this->m_MsgBuf[0] << 0;
		iMsgSize |= this->m_MsgBuf[1] << 0x8;
		iMsgSize |= this->m_MsgBuf[2] << 0x16;
		iMsgSize |= this->m_MsgBuf[3] << 0x24;

		// 消息类型
		iMsgType = 0;
		iMsgType |= this->m_MsgBuf[4] << 0;
		iMsgType |= this->m_MsgBuf[5] << 0x8;
		iMsgType |= this->m_MsgBuf[6] << 0x16;
		iMsgType |= this->m_MsgBuf[7] << 0x24;

		int leng = this->m_MsgBuf.size() - 8;
		if (leng < iMsgSize)	// 当前包的实际长度小于应该收的长度,说明包的消息没有收完
			break;
		// 有包的情况下,判断集合对象有没有new出来,如果还没有new出来说明组第一个包,生成集合
		if (!pMultMsg)
			pMultMsg = new CMultServerMSG();
		pMsg = new CSocketMSG((CSocketMSG::MSG_TYPE)iMsgType, this->m_MsgBuf.substr(8, iMsgSize));
		pMultMsg->m_pMsg.push_back(pMsg);
		this->m_MsgBuf.erase(0, 8 + iMsgSize);
	}

	return pMultMsg;
}

std::string* CServerProto::response2raw(UserData& _oUserData)
{	// TODO: 将游戏逻辑消息转换成TCP消息
	int iLength = 0;
	int iId = 0;
	std::string* pStrMsg = nullptr;
	std::string strCache;				// 临时缓冲区,存放要发送数据序列化后的字符串

	GET_REF2DATA(CSocketMSG, oOutput, _oUserData);
	strCache = oOutput.Serialize();
	iLength = strCache.size();
	iId = oOutput.GetMsgType();

	pStrMsg = new std::string();

	// 序列化数据段长度
	pStrMsg->push_back((iLength >> 0) & 0xFF);
	pStrMsg->push_back((iLength >> 8) & 0xFF);
	pStrMsg->push_back((iLength >> 16) & 0xFF);
	pStrMsg->push_back((iLength >> 24) & 0xFF);
	// 消息类型
	pStrMsg->push_back((iId >> 0) & 0xFF);
	pStrMsg->push_back((iId >> 8) & 0xFF);
	pStrMsg->push_back((iId >> 16) & 0xFF);
	pStrMsg->push_back((iId >> 24) & 0xFF);
	pStrMsg->append(strCache);

	return pStrMsg;
}

Irole* CServerProto::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
	return this->m_pRole;
}

Ichannel* CServerProto::GetMsgSender(BytesMsg& _oBytes)
{
	return this->m_chanel;
}


void CServerProto::SetChanel(CServerChanelData* _chanel)
{
	this->m_chanel = _chanel;
}
