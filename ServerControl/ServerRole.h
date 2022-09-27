#ifndef SERVER_ROLE_H
#define SERVER_ROLE_H

#include <zinx.h>

class CServerProto;
class CSocketMSG;

/* 业务层&& 人物角色继承类*/
// 业务层由协议层维护并且初始化init(),协议层释放时将业务层释放Fini()
class CServerRole : public Irole
{
	friend class CServerProto;
public:

	// 通过 Irole 继承
	virtual bool Init() override;
	virtual void Fini() override;
	// 逻辑处理
	virtual UserData* ProcMsg(UserData& _poUserData) override;

	CServerRole(CServerProto* _proto);
	virtual ~CServerRole();
	void SetProto(CServerProto* _proto);	// 设置协议类

private:
	CServerProto* m_Proto = nullptr;		// 协议类
};

#endif
