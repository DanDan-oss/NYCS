#ifndef DLL_EXPORT_H
#define DLL_EXPORT_H

enum NETWORK_MSGTYPE
{
	MSG_TYPE_PANT = 0,			// ��������Ϣ,������������Ϣ
	MSG_TYPE_LOG = 1,			// ��½��Ϣ
	MSG_TYPE_RESPOND = 3		// ���������ص���Ϣ

};

extern "C" __declspec(dllexport) struct SOCKT_MSG
{
	// ����ǽ��շ��������͵���Ϣ�����������ת����Ϣ�ṹ��, ����˲���Ϊ0��ʾ��ǰ��û��socket��Ϣ
	NETWORK_MSGTYPE type;			// ��Ϣ����
	union
	{
		struct  
		{
			char* userName;		// �û�����
			char* userPass;		// �û�����
		}PlayerLogin;

		struct
		{
			bool rv;					// ����������״̬
			unsigned int msgType;		// ��������
			char* recvData;		// ������������Ϣ
		} ServerRecv;
	};
};

extern "C" __declspec(dllexport) bool __stdcall InitSocketLib();
extern "C" __declspec(dllexport) bool __stdcall SendSockMessage(const struct SOCKT_MSG* _pSockMsg);
extern "C" __declspec(dllexport) bool __stdcall RecvSockMessage(struct SOCKT_MSG* _sockMsg);
extern "C" __declspec(dllexport) unsigned int __stdcall GetRecvOutstandMsg();		// ��ȡ��Ϣ�����л��ж�������Ϣδ������
extern "C" __declspec(dllexport) unsigned int __stdcall GetSockMsgError();		// ��ȡ�������

//extern "C" BOOL CALLBACK  SendSocketMsg(NETWORK_MSGTYPE _msgType,_In_ std::string& _strMsg);
//extern "C" DWORD CALLBACK  RecvSocketMsg(_Out_ NETWORK_MSGTYPE _msgType, _Out_ std::string& _recvStrMsg);

#endif 