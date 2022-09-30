#ifndef LIB_SOCKET_H
#define LIB_SOCKET_H

#include <iostream>
#include <google/protobuf/message.h>
#include <windows.h>

/* ���嶯̬����ת������������ͣ���ת��ʧ����ִ�з���NULL */
#define GET_REF2DATA(type, ref, orig)  type * pref = dynamic_cast<type *>(&orig); if (nullptr == pref) {return nullptr;} type &ref = dynamic_cast<type&>(orig)

enum NETWORK_MSGTYPE;
struct SOCKT_MSG;

DWORD WINAPI RevcMessageThread(LPVOID lpThreeParameter);


// ��Ϣ����
class UserData
{
public:
	UserData() {}
	virtual ~UserData() {}

};

// �ͻ����Լ�����Ϣ ���������Ϣ��ת��
class CClientMSG : public UserData
{
public:
	enum MSG_TYPE {		// �ͻ��˷�������������Ϣ����
		MSG_TYPE_PANT = 0,			// ��������Ϣ,������������Ϣ
		MSG_TYPE_LOG = 1,			// ��½��Ϣ
		MSG_TYPE_RESPOND = 3
	};

	CClientMSG(CClientMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg);
	CClientMSG(CClientMSG::MSG_TYPE _type, std::string _stream);
	CClientMSG(const struct SOCKT_MSG* msg);

	virtual ~CClientMSG();

	// �����������л����ݽ��ַ���
	std::string SerializeToString();

	// �����������л�,���ݽ�struct SOCKT_MSG����
	BOOL SerializeToMsg(struct SOCKT_MSG* _sokeMsg);

	// ��ȡprotobuf��Ϣ�ĸ���
	google::protobuf::Message* GetMessagePoint();

	// ��ȡ��ǰ��Ϣ������
	CClientMSG::MSG_TYPE GetMsgType();
private:
	MSG_TYPE m_msgType = MSG_TYPE::MSG_TYPE_PANT;			// ��Ϣ����
	google::protobuf::Message* m_pMsg = nullptr;	// protobuf������Ϣ�ĸ���

};

// ��Ϣ����
class CMultClientMSG
{
public:
	std::list<CClientMSG*> m_pMsg;
	virtual ~CMultClientMSG();
};

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
};





#endif // !LIB_SOCKET_H
