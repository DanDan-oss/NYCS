#include <Winsock2.h>  
#include <windows.h> 
#include "netsock.h" 


NetMSG::NetMSG(NetMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg)
	:m_msgType(_type)
{
	switch (_type)
	{
	case MSG_TYPE::MSG_TYPE_LOG:
		this->m_pMsg = new PlayerLoginMsg(dynamic_cast<PlayerLoginMsg&>(*_pMsg));
		break;
	}
}

NetMSG::NetMSG(NetMSG::MSG_TYPE _type, std::string& _stream)
{
	
}

NetMSG::~NetMSG()
{
	if (this->m_pMsg)
		delete this->m_pMsg;
}

std::string NetMSG::Serialize()
{
	std::string str;
	this->m_pMsg->SerializeToString(&str);
	return str;
}

google::protobuf::Message* NetMSG::GetMessagePoint()
{
	return this->m_pMsg;
}

NetMSG::MSG_TYPE NetMSG::GetMsgType()
{
	return this->m_msgType;
}

// ===========================
CNetSockt::CNetSockt()
{
	
}


CNetSockt::CNetSockt(std::string& _serAdder, const unsigned int _serPort)
{
	this->InitWin32Socket(_serAdder, _serPort);

}

CNetSockt::~CNetSockt()
{
	if (this->m_sockFd)
		::closesocket(this->m_sockFd);
	if (this->m_recvBuf)
		delete[] this->m_recvBuf;
	::WSACleanup();
}




BOOL CNetSockt::InitWin32Socket(std::string& _serAdder, const unsigned int _serPort)
{
	WSADATA wsaData;
	::WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockaddr_in  serSockAddr = { 0 };
	u_long iMode = 0;

	this->m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->m_sockFd)
		return FALSE;

	::ioctlsocket(this->m_sockFd, FIONBIO, &iMode);
	this->m_recvBuf = new char[1024];

	serSockAddr.sin_family = AF_INET;
	serSockAddr.sin_port = htons(_serPort);
	//inet_pton(AF_INET, _serAdder.data(), &(serSockAddr.sin_addr.S_un));
	if (-1 == ::connect(this->m_sockFd, (const struct sockaddr*)&serSockAddr, sizeof(struct sockaddr_in)))
		return FALSE;

	return TRUE;
}

BOOL CNetSockt::SendNetMessage(std::string& _msg)
{
	if(this->m_sockFd)
		return FALSE;
	if (-1 == send(this->m_sockFd, _msg.data(), _msg.size(), 0))
		return FALSE;
	return TRUE;
}

DWORD CNetSockt::RecvNetMessage(std::string& _recvStrMsg)
{
	int recvMsgLen = recv(this->m_sockFd, this->m_recvBuf, sizeof(this->m_recvBuf), 0);

	if (recvMsgLen > 0)
	{
		_recvStrMsg.clear();
		_recvStrMsg.append(this->m_recvBuf, recvMsgLen);
	}

	if (recvMsgLen == 0)
	{	// 对端的socket已正常关闭
		return -1;
	}

	if(0 >= recvMsgLen && errno == EAGAIN)
	{// 当前数据端没有数据读
			return 0;
	}

	return -1;
}