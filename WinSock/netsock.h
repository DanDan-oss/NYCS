#ifndef LIB_SOCKET_H
#define LIB_SOCKET_H
#include <iostream>
#include "msg.pb.h"
#pragma comment(lib, "ws2_32.lib")

enum NETWORK_MSGTYPE
{
	MSG_TYPE_PANT = 0,			// ��������Ϣ,������������Ϣ
	MSG_TYPE_LOG = 1			// ��½��Ϣ
};


class NetMSG
{
public:
	enum MSG_TYPE {		// �ͻ��˷�������������Ϣ����
		MSG_TYPE_PANT = 0,			// ��������Ϣ,������������Ϣ
		MSG_TYPE_LOG = 1			// ��½��Ϣ
	};

	NetMSG(NetMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg);
	NetMSG(NetMSG::MSG_TYPE _type, std::string& _stream);
	virtual ~NetMSG();

	std::string Serialize();
	google::protobuf::Message* GetMessagePoint();
	NetMSG::MSG_TYPE GetMsgType();
private:
	MSG_TYPE m_msgType;			// ��Ϣ����
	google::protobuf::Message* m_pMsg = nullptr;	// protobuf������Ϣ�ĸ���
};




class CNetSockt
{
public:
	CNetSockt();
	CNetSockt(std::string& _serAdder, const unsigned int _serPort);
	~CNetSockt();

	BOOL InitWin32Socket(std::string& _serAdder, const unsigned int _serPort);
	BOOL SendNetMessage(std::string& _msg);
	DWORD RecvNetMessage(std::string& _recvStrMsg);

private:
	SOCKET m_sockFd = 0;
	char* m_recvBuf = nullptr;

};

#endif // !LIB_SOCKET_H
