#include <Winsock2.h>
#include <windows.h> 
#include "ClientSockt.h"
#include "dllmain.h"


DWORD WINAPI RevcMessageThread(LPVOID lpThreeParameter)
{
	//HANDLE hdThread = GetCurrentThread();		// ��ǰ���̵ľ��
	CClientSockt* pSocket = dynamic_cast<CClientSockt*>((CClientSockt*)lpThreeParameter);
	std::string strBuf;

	//if (!pSocket)
	//	return false();
	//std::cout << "������Ϣ�߳̿�ʼ����" << std::endl;
	//while (pSocket->GetFd())
	//{
	//	// socket ������û�ж�ȡ������ ���Ҵ�����ȡsocket���������ݵ��ڴ�û������ ���� Э����Ϣ����û������
	//	// (socket ������û�ж�ȡ ���ܵ�ǰû���յ����������͵����� || �������� || socket�Ѿ��ر� )
	//	if (FALSE == pSocket->ReadFd(strBuf) && strBuf.size() == 0 && pSocket->m_multMsg.m_pMsg.size())
	//	{
	//		std::cout << "��ǰsocket������û������,��Ĭ1��" << std::endl;
	//		Sleep(1000 * 1);		// ��ǰsocket������û������,��Ĭ1��
	//		continue;
	//	}
	//	int msgNum = pSocket->ResponseToRaw(strBuf, pSocket->m_multMsg);		// ���õ�������ת����Э����Ϣ,Ȼ����뵽��Ϣ����
	//}

	pSocket->ReadFd(strBuf);
	return NULL;
}

// ======================================> �������л���
CClientProto::CClientProto()
{
	
}

// TCP������Ϣ ת���� Э������ (waring: ע�ⷵ�ص� Э�����ݼ���������)
DWORD CClientProto::ResponseToRaw(std::string& _szInput, CMultClientMSG& _pMultMsg)
{
	int iMsgType = 0;
	int iMsgSize = 0;
	CClientMSG* pMsg = nullptr;
	std::string strCache;		// ����,��ʱ��Ž�ȡ����Ϣ

	// TODO: ��TCP��Ϣת������Ϸ�߼���Ϣ && TCPճ������
	this->m_MsgBuf.append(_szInput);
	while (this->m_MsgBuf.size() > 8)
	{
		// ���л����ݳ���
		iMsgSize = 0;
		iMsgSize |= this->m_MsgBuf[0] << 0;
		iMsgSize |= this->m_MsgBuf[1] << 0x8;
		iMsgSize |= this->m_MsgBuf[2] << 0x16;
		iMsgSize |= this->m_MsgBuf[3] << 0x24;

		// ��Ϣ����
		iMsgType = 0;
		iMsgType |= this->m_MsgBuf[4] << 0;
		iMsgType |= this->m_MsgBuf[5] << 0x8;
		iMsgType |= this->m_MsgBuf[6] << 0x16;
		iMsgType |= this->m_MsgBuf[7] << 0x24;

		int leng = this->m_MsgBuf.size() - 8;
		if(leng < iMsgSize)	// ��ǰ����ʵ�ʳ���С��Ӧ���յĳ���,˵��������Ϣû������
			break;

		// �а��������,�жϼ��϶�����û��new����,�����û��new����˵�������ǵ�һ����,���ɼ���
		pMsg = new CClientMSG( (CClientMSG::MSG_TYPE)iMsgType, this->m_MsgBuf.substr(8, iMsgSize));
		_pMultMsg.m_pMsg.push_back(pMsg);
		m_MsgBuf.erase(0, 8 + iMsgSize);
	}

	return _pMultMsg.m_pMsg.size();
}


std::string CClientProto::RawToReques(UserData& _oUserData)
{
	int iLength = 0;
	int iMsgType = 0;
	std::string pStrMsg;
	std::string strCache;				// ��ʱ������,���Ҫ�����������л�����ַ���

	GET_REF2DATA(CClientMSG, oOutput, _oUserData);
	strCache = oOutput.SerializeToString();
	iLength = strCache.size();
	iMsgType = oOutput.GetMsgType();

	// ���л����ݶγ���
	pStrMsg.push_back((iLength >> 0) & 0xFF);
	pStrMsg.push_back((iLength >> 8) & 0xFF);
	pStrMsg.push_back((iLength >> 16) & 0xFF);
	pStrMsg.push_back((iLength >> 24) & 0xFF);

	// ��Ϣ����
	pStrMsg.push_back((iMsgType >> 0) & 0xFF);
	pStrMsg.push_back((iMsgType >> 8) & 0xFF);
	pStrMsg.push_back((iMsgType >> 16) & 0xFF);
	pStrMsg.push_back((iMsgType >> 24) & 0xFF);

	std::cout << " size: " << iLength << "  iMsgType:" << iMsgType << " strSize: " << pStrMsg.size() << std::endl;
	pStrMsg.append(strCache);
	
	std::cout << "===================>  " << std::hex << pStrMsg.data() << std::endl;
	std::cout << std::dec;
	return pStrMsg;
}

// ===========================  �ͻ������紫����
CClientSockt::CClientSockt()
{
	
}


CClientSockt::CClientSockt(std::string& _serAdder, const unsigned int _serPort)
{
	if (false == this->Init(_serAdder, _serPort))
		std::cout << "�����ʼ��ʧ��" << std::endl;
}

CClientSockt::~CClientSockt()
{
	this->Fini();
}


BOOL CClientSockt::Init(std::string& _serAdder, const unsigned int _serPort)
{
	WSADATA wsaData;
	sockaddr_in  serSockAddr = { 0 };

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "socket WSAStartup init fail!" << std::endl;
		return false;
	}

	this->m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->m_sockFd)
		return FALSE;

	serSockAddr.sin_family = AF_INET;
	serSockAddr.sin_port = htons(_serPort);
	//inet_pton(AF_INET, _serAdder.data(), &(serSockAddr.sin_addr.S_un));
	serSockAddr.sin_addr.S_un.S_addr = inet_addr(_serAdder.data());
	if (-1 == connect(this->m_sockFd, (const struct sockaddr*)&serSockAddr, sizeof(struct sockaddr_in)))
	{
		std::cout << "socket connect server fail!" << std::endl;
		closesocket(this->m_sockFd);
		this->m_sockFd = 0;
		return FALSE;
	}

	CreateThread(NULL, 0, RevcMessageThread, this, 0, nullptr);
	return TRUE;
}

VOID CClientSockt::Fini()
{
	if (this->m_sockFd)
	{
		closesocket(this->m_sockFd);
		this->m_sockFd = 0;
	}
	std::cout << "����socket���ر�" << std::endl;
	WSACleanup();
}

int CClientSockt::GetFd()
{
	return this->m_sockFd;
}

BOOL CClientSockt::SendSockMsg(const struct SOCKT_MSG* _sokeMsg)
{
	CClientMSG msg(_sokeMsg);
	std::string  strBuf = this->RawToReques(msg);		// ��Э������ת����TCPЭ��Ȼ����
	return this->WriteFd(strBuf);
}


BOOL CClientSockt::RecvSockMsg(struct SOCKT_MSG* _sokeMsg)
{
	CClientMSG* msgPtr = nullptr;

	if (!this->m_multMsg.m_pMsg.size())
		return false;

	// ��ȡ��Ϣ������ĵ�һ����Ϣ
	msgPtr = this->m_multMsg.m_pMsg.front();

	// ת��Э����Ϣ��,�ͷ���Ϣ�ڴ�
	if (msgPtr->SerializeToMsg(_sokeMsg, this))
	{
		this->m_multMsg.m_pMsg.pop_front();
		delete msgPtr;
	}


	return TRUE;
}

bool CClientSockt::ReadFd(std::string& _input)
{
	bool bRet = false;
	int iReadLen = -1;
	char acBuff[1024] = { 0 };

	// �ж����socket���пɶ�����,��ȥ��,û�����ݾ��˳�
	while (this->GetFd())		
	{
		_input = "";
		iReadLen = recv(this->m_sockFd, acBuff, sizeof(acBuff), 0);
		if (iReadLen != SOCKET_ERROR)
		{
			_input.append(acBuff, iReadLen);
			this->ResponseToRaw(_input, this->m_multMsg);
			continue;
		}

		// socet��ȡ������
		iReadLen = WSAGetLastError();
		switch (iReadLen)
		{
		case WSAEWOULDBLOCK:	// socktû������
			std::cout << "recv from " << this->m_sockFd << ":" << _input.data() << std::endl;
			std::cout << "<----------------------------------------->" << std::endl;
			return true;

		case WSAEINVAL:
			std::cout << "socket WSAEINVAL" << std::endl;
		case WSAETIMEDOUT:		// socket��ʱ
			std::cout << "socket��ʱ" << std::endl;
			break;
		case WSAENETDOWN:		// socket���ӶϿ�
			std::cout << "socket���ӶϿ�" << std::endl;
			break;
		default:	// socket��������
			std::cout << "socket��������: " << iReadLen << std::endl;
			break;
		}
		this->Fini();
		std::cout << "<----------------------------------------->" << std::endl;
		return false;
	}
	//
	//std::cout << "recv from " << this->m_sockFd << ":" << _input.data() << std::endl;
	//std::cout << "<----------------------------------------->" << std::endl;
	return true;
}

bool CClientSockt::WriteFd(std::string& _output)
{
	bool bRet = false;
	// ����Ҫ���͵����ݵ���ʱ������
	char* pOut = (char*)calloc(1UL, _output.size());
	_output.copy(pOut, _output.size(), 0);

	if ((0 <= this->m_sockFd) && (_output.size() == send(this->m_sockFd, pOut, _output.size(), 0)))
	{
		bRet = true;
		std::cout << "<----------------------------------------->" << std::endl;
		std::cout << "send to " << this->m_sockFd << ":" << _output.size()<<"===>" << _output.data() << std::endl;
		std::cout << "<----------------------------------------->" << std::endl;
	}
	free(pOut);
	return bRet;
}

