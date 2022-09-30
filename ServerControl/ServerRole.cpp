#include "ServerRole.h"
#include "ServerMSG.h"
#include "ServerProto.h"
#include "msg.pb.h"
#include "encry.h"

CEncryAES cAes("qiancheng2");

CServerRole::CServerRole(CServerProto* _proto)
	:m_Proto(_proto)
{

}

CServerRole::~CServerRole()
{
	
}

bool CServerRole::Init()
{
	return true;
}

UserData* CServerRole::ProcMsg(UserData& _poUserData)
{
	std::string strData;
	google::protobuf::Message* pMsg = nullptr;
	CSocketMSG* pSerMsg = nullptr;
	std::string userName;
	std::string userpass;
	ServerRespondMsg* pCacheMsg = nullptr;
	PlayerLoginMsg* pMsgPtr = nullptr;

	GET_REF2DATA(CMultServerMSG, input, _poUserData);
	for (CSocketMSG* pCache : input.m_pMsg)
	{
		switch (pCache->GetMsgType())
		{
		case CSocketMSG::MSG_TYPE::MSG_TYPE_LOG:
			// 判断当前消息是不是上线消息
			pMsgPtr = dynamic_cast<PlayerLoginMsg*>(pCache->GetMessagePoint());

			userName = pMsgPtr->username();
			userpass = pMsgPtr->userpass();
			//userName = cAes.AesCbcDecrypt(userName);
			//userpass = cAes.AesCbcDecrypt(userpass);
			std::cout << userName << "   " << userpass << std::endl;

			pCacheMsg = new ServerRespondMsg();
			pCacheMsg->set_rv(true);
			pCacheMsg->set_msgtype(CSocketMSG::MSG_TYPE::MSG_TYPE_LOG);
			
			//pCacheMsg->set_data(userName.data(), userpass.data());
			//pCacheMsg->set_data(cAes.AesCbcEncrypt(pCacheMsg->data()));
			userName.append(userpass);
			pCacheMsg->set_data(userName);
			std::cout << "add data: "<<userName<< std::endl;
			pSerMsg = new CSocketMSG(CSocketMSG::MSG_TYPE::MSG_TYPE_Respond, pCacheMsg);
			ZinxKernel::Zinx_SendOut(*pSerMsg, *(this->m_Proto));
			delete pCacheMsg;
			break;
		default:
			break;
		}
	}

	return nullptr;
}

void CServerRole::Fini()
{
	return;
}

void CServerRole::SetProto(CServerProto* _proto)
{
	this->m_Proto = _proto;
	return;
}
