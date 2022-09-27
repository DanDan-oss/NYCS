#ifndef SERVER_ROLE_H
#define SERVER_ROLE_H

#include <zinx.h>

class CServerProto;
class CSocketMSG;

/* ҵ���&& �����ɫ�̳���*/
// ҵ�����Э���ά�����ҳ�ʼ��init(),Э����ͷ�ʱ��ҵ����ͷ�Fini()
class CServerRole : public Irole
{
	friend class CServerProto;
public:

	// ͨ�� Irole �̳�
	virtual bool Init() override;
	virtual void Fini() override;
	// �߼�����
	virtual UserData* ProcMsg(UserData& _poUserData) override;

	CServerRole(CServerProto* _proto);
	virtual ~CServerRole();
	void SetProto(CServerProto* _proto);	// ����Э����

private:
	CServerProto* m_Proto = nullptr;		// Э����
};

#endif
