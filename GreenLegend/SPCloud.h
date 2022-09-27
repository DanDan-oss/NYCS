#pragma once

#pragma comment(lib, "SPCloud_A.lib")

extern "C" {

	/* ����: �Ƽ����ʼ�� */
	/* ����: iTimeout; send/recv��ʱʱ��(����) */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ʼ���ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_CloudInit(int iTimeout, OUT OPTIONAL int* iError);

	/* ����: �Ƽ����ʼ�� (����ģʽ) */
	/* ����: szSoftwareName; ����� */
	/* ����: szCard; ���Կ��� */
	/* ����: szIP; IP��ַ */
	/* ����: wPort; ������֤�˿ں� */
	/* ����: iTimeout; send/recv��ʱʱ��(����) */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ʼ���ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_CloudInit_Debug(char* szSoftwareName, char* szCard, char* szIP, int wPort, int iTimeout, OUT OPTIONAL int* iError);
	/* ����: ��SP_CloudInit_Debugһ��, ֻ�ǲ������"����ģʽ"������ʾ */
	bool  __stdcall SP_CloudInit_Debug2(char* szSoftwareName, char* szCard, char* szIP, int wPort, int iTimeout, OUT OPTIONAL int* iError);

	/* ����: �Ƽ����ʼ�� */
	/* ����: szUser; �����˺� */
	/* ����: szPassword; �������� */
	/* ����: szIP; IP��ַ */
	/* ����: wPort; ������֤�˿ں� */
	/* ����: iTimeout; send/recv��ʱʱ��(����) */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ʼ���ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_CloudInit_DebugUser(char* szSoftwareName, char* szUser, char* szPassword, char* szIP, int wPort, int iTimeout, OUT OPTIONAL int* iError);
	/* ����: ��SP_CloudInit_DebugUserһ��, ֻ�ǲ������"����ģʽ"������ʾ */
	bool  __stdcall SP_CloudInit_DebugUser2(char* szSoftwareName, char* szUser, char* szPassword, char* szIP, int wPort, int iTimeout, OUT OPTIONAL int* iError);

	/* ����: �Ƽ������� (ÿ�ε�������) */
	/* ����: dwCloudID; �Ƽ���ID; (�������0) */
	/* ����: pData; �Ƽ������ݰ�ָ�� */
	/* ����: dwLength; �Ƽ������ݰ����� */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƽ����Ƿ�ɹ�; �������, �ɲο�iError */
	bool  __stdcall SP_CloudComputing(int dwCloudID, unsigned char* pData, int dwLength, OUT OPTIONAL int* iError);

	/* ����: �Ƽ��㷵������ */
	/* ����: dwYunID; �Ƽ���ID; (�������0) */
	/* ����: pData; ��Ŷ�ȡ���ݵĻ����� */
	/* ����: dwLength; Ҫ��ȡ�ĳ��� */
	/* ����: ��ȡ���ĳ��� */
	int  __stdcall SP_CloudResult(int dwCloudID, unsigned char* pData, int dwLength);

	/* ����: �Ƽ���, ���������ܳ��� */
	/* ����: dwCloudID; �Ƽ���ID; (�������0) */
	/* ����: �ܳ��� */
	int  __stdcall SP_CloudResultLength(int dwCloudID);

	/* ����: �Ƽ���, ��������ʣ��δ������ */
	/* ����: dwCloudID; �Ƽ���ID; (�������0) */
	/* ����: ��ȡ���ĳ��� */
	int  __stdcall SP_CloudResultRest(int dwCloudID);

	/* ����: �Ƽ���, Ƶ����֤ (���鴴��һ���߳���Ƶ������, ����30�����һ��; ÿ�ε�������) */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ���֤�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_Beat(OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½�������������� (ÿ�ε�������) */
	/* ����: szAgent[44] */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetCardAgent(char szAgent[44], OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵĿ����� (ÿ�ε�������) */
	/* ����: szCardType[36] */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetCardType(char szCardType[36], OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵ�¼ʱ��¼��IP��ַ (ÿ�ε�������) */
	/* ����: szIPAddress[44] */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetIPAddress(char szIPAddress[44], OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵı�ע (ÿ�ε�������) */
	/* ����: szRemarks[132] */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetRemarks(char szRemarks[132], OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵĴ���ʱ��� (ÿ�ε�������) */
	/* ����: iCreatedTimeStamp */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetCreatedTimeStamp(__int64* iCreatedTimeStamp, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵļ���ʱ��� (ÿ�ε�������) */
	/* ����: iActivatedTimeStamp */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetActivatedTimeStamp(__int64* iActivatedTimeStamp, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵĹ���ʱ��� (ÿ�ε�������) */
	/* ����: iExpiredTimeStamp */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetExpiredTimeStamp(__int64* iExpiredTimeStamp, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵ�����¼ʱ��� (ÿ�ε�������) */
	/* ����: iLastLoginTimeStamp */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetLastLoginTimeStamp(__int64* iLastLoginTimeStamp, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵ�ʣ����� (ÿ�ε�������) */
	/* ����: iFYI */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetFYI(__int64* iFYI, OUT OPTIONAL int* iError);

	/* ����2-3 �۳���ǰ���ܵ���	�����û�ʹ����ĳЩ���⹦����Ҫ����۷ѵĳ���*/
	/* ������iFYICount����Ҫ�۳��ĵ�������*/
	/* ������iSurplusFYIOUT ʣ�µĵ���  ���Ϊ-3Ϊ���ܵĿ۳�����Ϊ������Ϊ-2Ϊ����ʣ��������㵼�¿۳�ʧ�ܣ�Ϊ-1˵����û���ҵ�Ŀ�꿨��*/
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool __stdcall SP_Cloud_DeductFYI(__int64 iFYICount, __int64* iSurplusFYI, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵĶ࿪��������ֵ (ÿ�ε�������) */
	/* ����: iNum */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetOpenMaxNum(int* iNum, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵİ󶨻�������ֵ (ÿ�ε�������) */
	/* ����: iBind; �Ƿ���, 1/0 */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetBind(int* iBind, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵĻ������� (ÿ�ε�������) */
	/* ����: iBindTime; (��) */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetBindTime(__int64* iBindTime, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵĽ��۳�����ֵ (ÿ�ε�������) */
	/* ����: iDeductSec; (��) */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetUnBindDeductTime(__int64* iDeductSec, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵ�������������ֵ (ÿ�ε�������) */
	/* ����: iNum */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetUnBindMaxNum(int* iNum, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵ��ۼƽ����� (ÿ�ε�������) */
	/* ����: iCountTotal */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetUnBindCountTotal(int* iCountTotal, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½���ܵ��ۼƽ��۳���ʱ�� (ÿ�ε�������) */
	/* ����: iDeductTimeTotal; (��) */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �Ƿ��ȡ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetUnBindDeductTimeTotal(__int64* iDeductTimeTotal, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, �Ƴ���ǰ�Ƽ��������֤��Ϣ (ÿ�ε�������) */
	/* ����: iError; �Ƽ��������/״̬�� */
	/* ����: �����Ƿ�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_Offline(OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ�������� (ÿ�ε�������) */
	/* ����: szNoteices */
	/* ����: �Ƿ���֤�ɹ�; �������, �ɲο�iError  */
	bool  __stdcall SP_Cloud_GetNotices(char szNoteices[65535], OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ��½�Ŀ��� (SP_CloudInit ��ʼ���ɹ������) */
	/* ����: szCard */
	void  __stdcall SP_Cloud_GetCard(char szCard[41]);

	/* ����: �Ƽ���, ���õ�ǰ��½�Ŀ��� (SP_CloudInit ��ʼ���ɹ������; ÿ�ε�������) */
	/* ����: iError; �Ƽ��������/״̬�� */
	void  __stdcall SP_Cloud_DisableCard(OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, ��ȡ��ǰ�ͻ���ID (SP_CloudInit ��ʼ���ɹ������; ÿ�ε�������) */
	int  __stdcall SP_Cloud_GetCID();

	/* ����: �Ƽ���, ��ȡ��ǰ�������߿ͻ������� (SP_CloudInit ��ʼ���ɹ������; ÿ�ε�������) */
	/* ����: iError; �Ƽ��������/״̬�� */
	bool  __stdcall SP_Cloud_GetOnlineCount(int* iCount, OUT OPTIONAL int* iError);

	/* ����: �Ƽ���, �����Ƽ������ϵͳ�汾��ʶ (SP_CloudInit ��ʼ��֮ǰʹ��) */
	/* ����: szWinVer; �Զ������ϵͳ�汾��ʶ, ���Ϊ��, ��Ϊ�����߼���ȡ����ϵͳ�汾 */
	bool  __stdcall SP_Cloud_SetWinVer(char* szWinVer);
}

DWORD __stdcall Test(PVOID);

DWORD __stdcall Thread_HeartBeat(PVOID);

void TimeStamp2SystemTime(__int64 ts, SYSTEMTIME& st);

VOID ErrorCodeProc(int iError);

