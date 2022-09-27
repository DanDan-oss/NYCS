#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H

#include<ZinxTCP.h>

class CServerProto;

//class ExitTimer : public TimerOutProc
//{
//	// ͨ�� TimerOutProc �̳�
//	virtual void Proc() override;
//	virtual int GetTimeSec() override;
//};


/* TCP��Ϣͨ���㣬 socket
ͨ���㴴��ʱ����Э���,����д���Э�������Э��㻥���(�������²����ϲ㴫)
Э�����ά��, ͨ�����ͷ�ʱ,�Զ��ͷŰ󶨵�Э��� */
class CServerChanelData : public ZinxTcpData
{
	friend class CServerProto;
public:
	// ͨ�� ZinxTcpData �̳�

	//��ȡ��Ϣ��һ���������: Э������
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;

	CServerChanelData(int _fd);
	CServerChanelData(int _fd, CServerProto* _proto);	// ���봫���_proto�Զ���
	virtual  ~CServerChanelData();

	void SetProto(CServerProto* _proto);		/* �󶨵�ǰͨ������� Э����� */

private:
	CServerProto* m_proto = nullptr;	// Э�����
};

/* �׽��ֹ�����*/
class CServerTcpFact : public IZinxTcpConnFact
{
	// ͨ�� IZinxTcpConnFact �̳�
	virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};

#endif // !GAME_NETWORK_H





