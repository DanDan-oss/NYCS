#ifndef SERVER_PROTO_H
#define SERVER_PROTO_H

#include <zinx.h>

class CServerChanelData;
class CServerRole;
class CEncryAES;

/* Э���
������ͨ������������������߰�Э������(�������²����ϲ㴫),
����ͨ�����Э������֮��Ĺ�ϵ��ͨ����ά��.
ҵ�����Э���ά�����ҳ�ʼ��init(),Э����ͷ�ʱ��ҵ����ͷ�Fini() */
class CServerProto : public Iprotocol
{
	friend class CServerChanelData;
	friend class CServerRole;

public:
	// ͨ�� Iprotocol �̳�
	// ���ͻ��˷���TCP��Ϣת���ɷ�����Ҫִ���߼���Ϣ
	virtual UserData* raw2request(std::string _szInput) override;

	// ��������Ҫִ������߼���Ϣת����TCP��Ϣ(���ظ��ͻ���)
	virtual std::string* response2raw(UserData& _oUserData) override;

	// ����ͻ���TCP��Ϣ�߼��Ķ���
	virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;

	// ���ؿͻ���ͨ�������
	virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

	CServerProto();
	CServerProto(CServerChanelData* _chanel);
	virtual ~CServerProto();

private:
	void SetChanel(CServerChanelData* _chanel);	// ��ͨ�������

private:
	CServerChanelData* m_chanel = nullptr;		// ͨ����
	CServerRole* m_pRole = nullptr;			// ҵ����
	std::string m_MsgBuf;					// TCP��Ϣ����
};

#endif // !SERVER_PROTO_H