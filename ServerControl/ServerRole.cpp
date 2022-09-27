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
	RespondMsg* pCacheMsg = nullptr;
	PlayerLoginMsg* pMsgPtr = nullptr;

	GET_REF2DATA(CMultServerMSG, input, _poUserData);
	for (CSocketMSG* pCache : input.m_pMsg)
	{
		switch (pCache->GetMsgType())
		{
		case CSocketMSG::MSG_TYPE::MSG_TYPE_LOG:

			pMsgPtr = dynamic_cast<PlayerLoginMsg*>(pCache->GetMessagePoint());
			userName = cAes.AesCbcDecrypt(pMsgPtr->username());
			userpass = cAes.AesCbcDecrypt(pMsgPtr->userpass());
			std::cout << userName << "   " << userpass << std::endl;
			pCacheMsg = new RespondMsg();
			//pCacheMsg->set_rv(true);
			//pCacheMsg->set_msgtype(CSocketMSG::MSG_TYPE::MSG_TYPE_LOG);
			//pCacheMsg->set_data(userName.data(), userpass.data());
			//pCacheMsg->set_data(cAes.AesCbcEncrypt(pCacheMsg->data()));

			pSerMsg = new CSocketMSG(CSocketMSG::MSG_TYPE::MSG_TYPE_Respond, pCacheMsg);
			ZinxKernel::Zinx_SendOut(*pSerMsg, *(this->m_Proto));
			delete pCacheMsg;
			break;
		default:
			break;
		}
		return pSerMsg;
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
