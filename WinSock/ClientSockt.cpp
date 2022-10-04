#include <Winsock2.h>
#include <windows.h> 
#include "ClientSockt.h"
#include "dllmain.h"


DWORD WINAPI RevcMessageThread(LPVOID lpThreeParameter)
{
	//HANDLE hdThread = GetCurrentThread();		// 当前进程的句柄
	CClientSockt* pSocket = dynamic_cast<CClientSockt*>((CClientSockt*)lpThreeParameter);
	std::string strBuf;

	//if (!pSocket)
	//	return false();
	//std::cout << "接受消息线程开始运行" << std::endl;
	//while (pSocket->GetFd())
	//{
	//	// socket 缓存区没有读取到数据 并且传给接取socket缓冲区数据的内存没有数据 并且 协议消息队列没有数据
	//	// (socket 缓存区没有读取 可能当前没有收到服务器发送的数据 || 发生错误 || socket已经关闭 )
	//	if (FALSE == pSocket->ReadFd(strBuf) && strBuf.size() == 0 && pSocket->m_multMsg.m_pMsg.size())
	//	{
	//		std::cout << "当前socket缓冲区没有数据,静默1秒" << std::endl;
	//		Sleep(1000 * 1);		// 当前socket缓冲区没有数据,静默1秒
	//		continue;
	//	}
	//	int msgNum = pSocket->ResponseToRaw(strBuf, pSocket->m_multMsg);		// 将得到的数据转换成协议消息,然后插入到消息队列
	//}

	pSocket->ReadFd(strBuf);
	return NULL;
}

// ======================================> 数据序列化类
CClientProto::CClientProto()
{
	
}

// TCP数据消息 转换成 协议数据 (waring: 注意返回的 协议数据集合中数量)
DWORD CClientProto::ResponseToRaw(std::string& _szInput, CMultClientMSG& _pMultMsg)
{
	int iMsgType = 0;
	int iMsgSize = 0;
	CClientMSG* pMsg = nullptr;
	std::string strCache;		// 缓存,临时存放截取的消息

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
		if(leng < iMsgSize)	// 当前包的实际长度小于应该收的长度,说明包的消息没有收完
			break;

		// 有包的情况下,判断集合对象有没有new出来,如果还没有new出来说明组这是第一个包,生成集合
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
	std::string strCache;				// 临时缓冲区,存放要发送数据序列化后的字符串

	GET_REF2DATA(CClientMSG, oOutput, _oUserData);
	strCache = oOutput.SerializeToString();
	iLength = strCache.size();
	iMsgType = oOutput.GetMsgType();

	// 序列化数据段长度
	pStrMsg.push_back((iLength >> 0) & 0xFF);
	pStrMsg.push_back((iLength >> 8) & 0xFF);
	pStrMsg.push_back((iLength >> 16) & 0xFF);
	pStrMsg.push_back((iLength >> 24) & 0xFF);

	// 消息类型
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

// ===========================  客户端网络传输类
CClientSockt::CClientSockt()
{
	
}


CClientSockt::CClientSockt(std::string& _serAdder, const unsigned int _serPort)
{
	if (false == this->Init(_serAdder, _serPort))
		std::cout << "网络初始化失败" << std::endl;
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
	std::cout << "网络socket被关闭" << std::endl;
	WSACleanup();
}

int CClientSockt::GetFd()
{
	return this->m_sockFd;
}

BOOL CClientSockt::SendSockMsg(const struct SOCKT_MSG* _sokeMsg)
{
	CClientMSG msg(_sokeMsg);
	std::string  strBuf = this->RawToReques(msg);		// 将协议数据转换成TCP协议然后发送
	return this->WriteFd(strBuf);
}


BOOL CClientSockt::RecvSockMsg(struct SOCKT_MSG* _sokeMsg)
{
	CClientMSG* msgPtr = nullptr;

	if (!this->m_multMsg.m_pMsg.size())
		return false;

	// 获取消息队列里的第一个消息
	msgPtr = this->m_multMsg.m_pMsg.front();

	// 转换协议消息后,释放消息内存
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

	// 判断如果socket中有可读数据,就去读,没有数据就退出
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

		// socet读取错误处理
		iReadLen = WSAGetLastError();
		switch (iReadLen)
		{
		case WSAEWOULDBLOCK:	// sockt没有数据
			std::cout << "recv from " << this->m_sockFd << ":" << _input.data() << std::endl;
			std::cout << "<----------------------------------------->" << std::endl;
			return true;

		case WSAEINVAL:
			std::cout << "socket WSAEINVAL" << std::endl;
		case WSAETIMEDOUT:		// socket超时
			std::cout << "socket超时" << std::endl;
			break;
		case WSAENETDOWN:		// socket连接断开
			std::cout << "socket连接断开" << std::endl;
			break;
		default:	// socket其他错误
			std::cout << "socket其他错误: " << iReadLen << std::endl;
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
	// 拷贝要发送的数据到临时缓冲区
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

