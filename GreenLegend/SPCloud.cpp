#include "pch.h"

enum CloudID {
	CloudID_Test = 1			/* ����ָ�� */
};
extern HANDLE hMainProcess;																				//��Ϸ���̾��

void TimeStamp2SystemTime(__int64 ts, SYSTEMTIME& st) {
	SP_AntiDumpBegin()
	__int64 tmpTs = (ts + 8 * 60 * 60) * 10000000i64 + 116444736000000000i64;
	SP_AntiDumpEnd()
	FILETIME ft;
	SP_AntiDumpBegin()
	ft.dwLowDateTime = (DWORD)tmpTs;
	SP_AntiDumpEnd()
	SP_AntiDumpBegin()
	ft.dwHighDateTime = tmpTs >> 32;
	SP_AntiDumpEnd()
	SP_AntiDumpBegin()
	FileTimeToSystemTime(&ft, &st);
	SP_AntiDumpEnd()
}

VOID ErrorCodeProc(int iError) {
	SP_Begin(1);
	SP_AntiDumpBegin()
	int LastError = 0;
	SP_AntiDumpEnd()
	//printf("��½�Ѵﵽ����.\n");
	
	
	switch (iError) {
	
	case -2:// -2 [������] WSAStartup��������ʧ��; (����������WSAGetLastError������ȡwinsock������)
		LastError = WSAGetLastError();
		//printf("WSAStartup��������ʧ��; ������:%d\n", LastError);
		// ������Ϸ
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -3:// -3 [������] ����3�κ�, ����ͨѶ��Ȼ����! ����������!; (����������WSAGetLastError������ȡwinsock������)
		LastError = WSAGetLastError();
		for (size_t i = 0; i < 30; i++) {
			Sleep(10 * 1000); // ���10s
			if (SP_CloudInit(300000, &LastError)) {
				return;
			}
		}
		//printf("����ͨѶ����; ������:%d\n", LastError);
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -4:// -4 [������] ����3�κ�, ���ݰ���Ȼ�쳣; (���ܺͷ���˲����ƥ������������޸�)
		//printf("���ݰ��쳣!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -15:// -15 [������] �����֤��Ϣ�ѹ���! ����������!;
		LastError = WSAGetLastError();
		for (size_t i = 0; i < 3; i++) {
			Sleep(10 * 1000); // ���10s
			if (SP_CloudInit(30000, &LastError)) {
				return;
			}
		}
		//printf("���ݰ��쳣!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -16:// -16 [������] δ֪����
		//printf("δ֪����!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -21:// -21 [״̬��] ������Ч/�˺��������;
		//printf("������Ч���˺��������!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -22:// -22 [״̬��] ����/�˺��ѹ���;
		//printf("���ܻ��˺��ѹ���!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -23:// -23 [״̬��] ����/�˺ű���ͣ;
		//printf("���ܻ��˺ű���ͣ!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -24:// -24 [״̬��] ��������Ч;
		//printf("��������Ч!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -25:// -25 [״̬��] �����̱�ͣ��;
		//printf("�����̱�ͣ��!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -26:// -26 [״̬��] �Ѵﵽ�����������, ��ǰ��½����ʧ��. (�������������ѡ�� ά�������û�)
		//printf("�Ѵﵽ���������! ��ǰ��½��Ч!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -27:// -27 [״̬��] ����������, ��ǰ�ͻ������ӽ��ڷ����������(ǿ�е���..), �������������ѡ�� �ߵ������û�)
		//printf("����������!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -28:// -28 [״̬��] �ѻ��������豸, ��ǰ��½��ʧЧ! (����ͻ����յ����������)
		//printf("�ѻ��������豸, ��ǰ��½��ʧЧ!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	default:
		//printf("������:%d\n", iError);
		break;
	}
	
	SP_End();
}
DWORD __stdcall Thread_HeartBeat(PVOID) {
	SP_AntiDumpBegin()
	int iError = 0;
	SP_AntiDumpEnd()
	while (TRUE) {
		if (!SP_Cloud_Beat(&iError)) {
			SP_AntiDumpBegin()
			ErrorCodeProc(iError);
			SP_AntiDumpEnd()
		}

		Sleep(1000);
	}

	return 0;
}

DWORD __stdcall Test(PVOID) {
	int iError = 0;
	char szAgent[44];
	char szCardType[36];
	char szIPAddress[44];
	char szRemarks[132];
	__int64 iCreatedTimeStamp, iActivatedTimeStamp, iExpiredTimeStamp, iLastLoginTimeStamp, iFYI, iBindTime, iDeductSec, iDeductTimeTotal;
	int iNum, iBind, iCountTotal;
	//	CreateThread (0, 0, Test_GetCardAgent, 0, 0, 0);

	//	CreateThread (0, 0, Test_GetFYI, 0, 0, 0);
	if (SP_Cloud_GetCardAgent(szAgent, &iError)) {
		printf("�ƿ���: %s\r\n", szAgent);
	}
	else {
		printf("�ƿ��˻�ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetCardType(szCardType, &iError)) {
		printf("������: %s\r\n", szCardType);
	}
	else {
		printf("�����ͻ�ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetIPAddress(szIPAddress, &iError)) {
		printf("IP��ַ: %s\r\n", szIPAddress);
	}
	else {
		printf("IP��ַ��ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetRemarks(szRemarks, &iError)) {
		printf("��ע: %s\r\n", szRemarks);
	}
	else {
		printf("��ע��ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetCreatedTimeStamp(&iCreatedTimeStamp, &iError)) {
		SYSTEMTIME st;
		TimeStamp2SystemTime(iCreatedTimeStamp, st);
		printf("����ʱ��: %04d-%02d-%02d %02d:%02d:%02d \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else {
		printf("����ʱ���ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetActivatedTimeStamp(&iActivatedTimeStamp, &iError)) {
		SYSTEMTIME st;
		TimeStamp2SystemTime(iActivatedTimeStamp, st);
		printf("����ʱ��: %04d-%02d-%02d %02d:%02d:%02d \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else {
		printf("����ʱ���ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetExpiredTimeStamp(&iExpiredTimeStamp, &iError)) {
		SYSTEMTIME st;
		TimeStamp2SystemTime(iExpiredTimeStamp, st);
		printf("����ʱ��: %04d-%02d-%02d %02d:%02d:%02d \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else {
		printf("����ʱ���ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetLastLoginTimeStamp(&iLastLoginTimeStamp, &iError)) {
		SYSTEMTIME st;
		TimeStamp2SystemTime(iLastLoginTimeStamp, st);
		printf("����¼ʱ��: %04d-%02d-%02d %02d:%02d:%02d \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else {
		printf("����¼ʱ���ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetFYI(&iFYI, &iError)) {
		printf("����: %lld \r\n", iFYI);
	}
	else {
		printf("������ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetOpenMaxNum(&iNum, &iError)) {
		printf("�࿪��������ֵ: %lld \r\n", iNum);
	}
	else {
		printf("�࿪��������ֵ��ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetBind(&iBind, &iError)) {
		printf("�󶨻�������ֵ: %lld \r\n", iBind);
	}
	else {
		printf("�󶨻�������ֵ��ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetBindTime(&iBindTime, &iError)) {
		printf("��������: %lld \r\n", iBindTime);
	}
	else {
		printf("�������ڻ�ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetUnBindDeductTime(&iDeductSec, &iError)) {
		printf("���۳�����ֵ: %lld \r\n", iDeductSec);
	}
	else {
		printf("���۳�����ֵ��ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetUnBindMaxNum(&iNum, &iError)) {
		printf("������������ֵ: %d \r\n", iNum);
	}
	else {
		printf("������������ֵ��ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetUnBindCountTotal(&iCountTotal, &iError)) {
		printf("�ۼƽ�����: %d \r\n", iCountTotal);
	}
	else {
		printf("�ۼƽ�������ȡʧ��! ������: %d\r\n", iError);
	}

	if (SP_Cloud_GetUnBindDeductTimeTotal(&iDeductTimeTotal, &iError)) {
		printf("�ۼƽ��۳���ʱ��: %lld \r\n", iDeductTimeTotal);
	}
	else {
		printf("���ۼƽ��۳���ʱ���ȡʧ��! ������: %d\r\n", iError);
	}

	return 0;
}