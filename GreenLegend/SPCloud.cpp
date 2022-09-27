#include "pch.h"

enum CloudID {
	CloudID_Test = 1			/* 测试指令 */
};
extern HANDLE hMainProcess;																				//游戏进程句柄

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
	//printf("登陆已达到上限.\n");
	
	
	switch (iError) {
	
	case -2:// -2 [错误码] WSAStartup函数返回失败; (可立即调用WSAGetLastError函数获取winsock错误码)
		LastError = WSAGetLastError();
		//printf("WSAStartup函数返回失败; 错误码:%d\n", LastError);
		// 结束游戏
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -3:// -3 [错误码] 尝试3次后, 网络通讯仍然出错! 请重新连接!; (可立即调用WSAGetLastError函数获取winsock错误码)
		LastError = WSAGetLastError();
		for (size_t i = 0; i < 30; i++) {
			Sleep(10 * 1000); // 间隔10s
			if (SP_CloudInit(300000, &LastError)) {
				return;
			}
		}
		//printf("网络通讯出错; 错误码:%d\n", LastError);
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -4:// -4 [错误码] 尝试3次后, 数据包仍然异常; (可能和服务端插件不匹配或封包被恶意修改)
		//printf("数据包异常!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -15:// -15 [错误码] 身份认证信息已过期! 请重新连接!;
		LastError = WSAGetLastError();
		for (size_t i = 0; i < 3; i++) {
			Sleep(10 * 1000); // 间隔10s
			if (SP_CloudInit(30000, &LastError)) {
				return;
			}
		}
		//printf("数据包异常!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -16:// -16 [错误码] 未知错误
		//printf("未知错误!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -21:// -21 [状态码] 卡密无效/账号密码错误;
		//printf("卡密无效或账号密码错误!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -22:// -22 [状态码] 卡密/账号已过期;
		//printf("卡密或账号已过期!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -23:// -23 [状态码] 卡密/账号被封停;
		//printf("卡密或账号被封停!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -24:// -24 [状态码] 代理商无效;
		//printf("代理商无效!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -25:// -25 [状态码] 代理商被停用;
		//printf("代理商被停用!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -26:// -26 [状态码] 已达到最大在线数量, 当前登陆将会失败. (服务器操作面板选择 维持在线用户)
		//printf("已达到最大在线数! 当前登陆无效!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -27:// -27 [状态码] 自身被挤下线, 当前客户端连接将在服务器被清除(强行掉线..), 服务器操作面板选择 踢掉在线用户)
		//printf("自身被挤下线!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	case -28:// -28 [状态码] 已换绑到其他设备, 当前登陆已失效! (建议客户端收到后结束进程)
		//printf("已换绑到其他设备, 当前登陆已失效!\n");
		::TerminateProcess(hMainProcess, 0);
		ExitProcess(0);
	default:
		//printf("错误码:%d\n", iError);
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
		printf("制卡人: %s\r\n", szAgent);
	}
	else {
		printf("制卡人获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetCardType(szCardType, &iError)) {
		printf("卡类型: %s\r\n", szCardType);
	}
	else {
		printf("卡类型获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetIPAddress(szIPAddress, &iError)) {
		printf("IP地址: %s\r\n", szIPAddress);
	}
	else {
		printf("IP地址获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetRemarks(szRemarks, &iError)) {
		printf("备注: %s\r\n", szRemarks);
	}
	else {
		printf("备注获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetCreatedTimeStamp(&iCreatedTimeStamp, &iError)) {
		SYSTEMTIME st;
		TimeStamp2SystemTime(iCreatedTimeStamp, st);
		printf("创建时间: %04d-%02d-%02d %02d:%02d:%02d \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else {
		printf("创建时间获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetActivatedTimeStamp(&iActivatedTimeStamp, &iError)) {
		SYSTEMTIME st;
		TimeStamp2SystemTime(iActivatedTimeStamp, st);
		printf("激活时间: %04d-%02d-%02d %02d:%02d:%02d \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else {
		printf("激活时间获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetExpiredTimeStamp(&iExpiredTimeStamp, &iError)) {
		SYSTEMTIME st;
		TimeStamp2SystemTime(iExpiredTimeStamp, st);
		printf("过期时间: %04d-%02d-%02d %02d:%02d:%02d \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else {
		printf("过期时间获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetLastLoginTimeStamp(&iLastLoginTimeStamp, &iError)) {
		SYSTEMTIME st;
		TimeStamp2SystemTime(iLastLoginTimeStamp, st);
		printf("最后登录时间: %04d-%02d-%02d %02d:%02d:%02d \r\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else {
		printf("最后登录时间获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetFYI(&iFYI, &iError)) {
		printf("点数: %lld \r\n", iFYI);
	}
	else {
		printf("点数获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetOpenMaxNum(&iNum, &iError)) {
		printf("多开数量属性值: %lld \r\n", iNum);
	}
	else {
		printf("多开数量属性值获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetBind(&iBind, &iError)) {
		printf("绑定机器属性值: %lld \r\n", iBind);
	}
	else {
		printf("绑定机器属性值获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetBindTime(&iBindTime, &iError)) {
		printf("换绑周期: %lld \r\n", iBindTime);
	}
	else {
		printf("换绑周期获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetUnBindDeductTime(&iDeductSec, &iError)) {
		printf("解绑扣除属性值: %lld \r\n", iDeductSec);
	}
	else {
		printf("解绑扣除属性值获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetUnBindMaxNum(&iNum, &iError)) {
		printf("最多解绑次数属性值: %d \r\n", iNum);
	}
	else {
		printf("最多解绑次数属性值获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetUnBindCountTotal(&iCountTotal, &iError)) {
		printf("累计解绑次数: %d \r\n", iCountTotal);
	}
	else {
		printf("累计解绑次数获取失败! 错误码: %d\r\n", iError);
	}

	if (SP_Cloud_GetUnBindDeductTimeTotal(&iDeductTimeTotal, &iError)) {
		printf("累计解绑扣除的时间: %lld \r\n", iDeductTimeTotal);
	}
	else {
		printf("累累计解绑扣除的时间获取失败! 错误码: %d\r\n", iError);
	}

	return 0;
}