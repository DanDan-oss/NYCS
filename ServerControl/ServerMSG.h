#ifndef SERVER_MSG_H
#define SERVER_MSG_H

#include <zinx.h>
#include <google/protobuf/message.h>

class CSocketMSG :public UserData
{
public:
	enum MSG_TYPE {		// �ͻ��˷�������������Ϣ����
		MSG_TYPE_PANT = 0,			// ��������Ϣ,������������Ϣ
		MSG_TYPE_LOG = 1,			// ��½��Ϣ
		MSG_TYPE_Respond = 3
	};

public:
	CSocketMSG(CSocketMSG::MSG_TYPE _type, google::protobuf::Message* _pMsg);
	CSocketMSG(CSocketMSG::MSG_TYPE _type, std::string _stream);
	virtual ~CSocketMSG();

	// ���л����ַ���(����TCP��Ϣʱ���Թ�)
	std::string Serialize();
	google::protobuf::Message* GetMessagePoint();
	virtual CSocketMSG::MSG_TYPE GetMsgType();

private:
	MSG_TYPE m_msgType;			// ��Ϣ����
	google::protobuf::Message* m_pMsg = nullptr;	// protobuf������Ϣ�ĸ���
};

// ��Ϣ����
class CMultServerMSG :public UserData
{
public:
	std::vector<CSocketMSG*> m_pMsg;
	virtual ~CMultServerMSG();
};

#endif // SERVER_MSG_H

