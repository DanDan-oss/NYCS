#ifndef LIB_SOCKET_H
#define LIB_SOCKET_H

#include <iostream>
#include <windows.h>
#include "ClientMSG.h"

class CClientMSG;
struct SOCKT_MSG;

/* ���嶯̬����ת������������ͣ���ת��ʧ����ִ�з���NULL */
#define GET_REF2DATA(type, ref, orig)  type * pref = dynamic_cast<type *>(&orig); if (nullptr == pref) {return nullptr;} type &ref = dynamic_cast<type&>(orig)



DWORD WINAPI RevcMessageThread(LPVOID lpThreeParameter);

// �ͻ�������ͨ��Э����-->�������л�
class CClientProto
{
public:
	CClientProto();
	// TCP������Ϣ ת���� Э������ (����|����), ����list�����л����ж�����Э������δ������
	virtual DWORD ResponseToRaw(std::string& _szInput, CMultClientMSG& _pMultMsg);

	// Э������(����) ת���� TCP������Ϣ
	virtual std::string RawToReques(UserData& _oUserData);
private:
	std::string m_MsgBuf;
};


// �ͻ������紫����
class CClientSockt :public CClientProto
{
	friend class CClientMSG;
	friend DWORD WINAPI RevcMessageThread(LPVOID lpThreeParameter);
public:
	CClientSockt();
	CClientSockt(std::string& _serAdder, const unsigned int _serPort);
	virtual ~CClientSockt();

	// socket��ʼ��
	virtual BOOL Init(std::string& _serAdder, const unsigned int _serPort);

	// sokct�ر�
	virtual VOID Fini();

	// ��ȡ��ǰͨ��Socket
	virtual int GetFd();

	virtual BOOL SendSockMsg(const struct SOCKT_MSG* _sokeMsg);
	virtual BOOL RecvSockMsg(struct SOCKT_MSG* _sokeMsg);

private:


	// ��������
	virtual bool ReadFd(std::string& _input);

	// ���� socket����
	virtual bool WriteFd(std::string& _output);

private:
	SOCKET m_sockFd = 0;		// ��ǰsocket ������--->socket���
	CMultClientMSG m_multMsg;
	std::string m_strCache[10];			// ��ʱ��ŷ������ظ���Ϣת������ַ�����
};


#endif // !LIB_SOCKET_H

