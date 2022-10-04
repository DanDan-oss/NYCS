#ifndef CLIENG_MSG_H
#define CLIENG_MSG_H

#include <iostream>
#include <google/protobuf/message.h>
class CClientSockt;

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
	virtual std::string SerializeToString();


	// �����������л�,�ַ������ݷŽ�CClientSockt�Ļ�������������
	bool SerializeToMsg(struct SOCKT_MSG* _sokeMsg, CClientSockt* _pClientSock);

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


#endif // !CLIENG_MSG_H

