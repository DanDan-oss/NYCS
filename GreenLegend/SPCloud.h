#pragma once

#pragma comment(lib, "SPCloud_A.lib")

extern "C" {

	/* 描述: 云计算初始化 */
	/* 参数: iTimeout; send/recv超时时间(毫秒) */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否初始化成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_CloudInit(int iTimeout, OUT OPTIONAL int* iError);

	/* 描述: 云计算初始化 (调试模式) */
	/* 参数: szSoftwareName; 软件名 */
	/* 参数: szCard; 测试卡密 */
	/* 参数: szIP; IP地址 */
	/* 参数: wPort; 网络验证端口号 */
	/* 参数: iTimeout; send/recv超时时间(毫秒) */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否初始化成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_CloudInit_Debug(char* szSoftwareName, char* szCard, char* szIP, int wPort, int iTimeout, OUT OPTIONAL int* iError);
	/* 描述: 和SP_CloudInit_Debug一样, 只是不会出现"调试模式"弹窗提示 */
	bool  __stdcall SP_CloudInit_Debug2(char* szSoftwareName, char* szCard, char* szIP, int wPort, int iTimeout, OUT OPTIONAL int* iError);

	/* 描述: 云计算初始化 */
	/* 参数: szUser; 测试账号 */
	/* 参数: szPassword; 测试密码 */
	/* 参数: szIP; IP地址 */
	/* 参数: wPort; 网络验证端口号 */
	/* 参数: iTimeout; send/recv超时时间(毫秒) */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否初始化成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_CloudInit_DebugUser(char* szSoftwareName, char* szUser, char* szPassword, char* szIP, int wPort, int iTimeout, OUT OPTIONAL int* iError);
	/* 描述: 和SP_CloudInit_DebugUser一样, 只是不会出现"调试模式"弹窗提示 */
	bool  __stdcall SP_CloudInit_DebugUser2(char* szSoftwareName, char* szUser, char* szPassword, char* szIP, int wPort, int iTimeout, OUT OPTIONAL int* iError);

	/* 描述: 云计算请求 (每次调用联网) */
	/* 参数: dwCloudID; 云计算ID; (必须大于0) */
	/* 参数: pData; 云计算数据包指针 */
	/* 参数: dwLength; 云计算数据包长度 */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 云计算是否成功; 如果出错, 可参考iError */
	bool  __stdcall SP_CloudComputing(int dwCloudID, unsigned char* pData, int dwLength, OUT OPTIONAL int* iError);

	/* 描述: 云计算返回数据 */
	/* 参数: dwYunID; 云计算ID; (必须大于0) */
	/* 参数: pData; 存放读取内容的缓冲区 */
	/* 参数: dwLength; 要读取的长度 */
	/* 返回: 读取到的长度 */
	int  __stdcall SP_CloudResult(int dwCloudID, unsigned char* pData, int dwLength);

	/* 描述: 云计算, 返回数据总长度 */
	/* 参数: dwCloudID; 云计算ID; (必须大于0) */
	/* 返回: 总长度 */
	int  __stdcall SP_CloudResultLength(int dwCloudID);

	/* 描述: 云计算, 返回数据剩余未读长度 */
	/* 参数: dwCloudID; 云计算ID; (必须大于0) */
	/* 返回: 读取到的长度 */
	int  __stdcall SP_CloudResultRest(int dwCloudID);

	/* 描述: 云计算, 频率验证 (建议创建一条线程来频繁调用, 比如30秒调用一次; 每次调用联网) */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否验证成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_Beat(OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密所属代理名 (每次调用联网) */
	/* 参数: szAgent[44] */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetCardAgent(char szAgent[44], OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的卡类型 (每次调用联网) */
	/* 参数: szCardType[36] */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetCardType(char szCardType[36], OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密登录时记录的IP地址 (每次调用联网) */
	/* 参数: szIPAddress[44] */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetIPAddress(char szIPAddress[44], OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的备注 (每次调用联网) */
	/* 参数: szRemarks[132] */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetRemarks(char szRemarks[132], OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的创建时间戳 (每次调用联网) */
	/* 参数: iCreatedTimeStamp */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetCreatedTimeStamp(__int64* iCreatedTimeStamp, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的激活时间戳 (每次调用联网) */
	/* 参数: iActivatedTimeStamp */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetActivatedTimeStamp(__int64* iActivatedTimeStamp, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的过期时间戳 (每次调用联网) */
	/* 参数: iExpiredTimeStamp */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetExpiredTimeStamp(__int64* iExpiredTimeStamp, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的最后登录时间戳 (每次调用联网) */
	/* 参数: iLastLoginTimeStamp */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetLastLoginTimeStamp(__int64* iLastLoginTimeStamp, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的剩余点数 (每次调用联网) */
	/* 参数: iFYI */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetFYI(__int64* iFYI, OUT OPTIONAL int* iError);

	/* 任务2-3 扣除当前卡密点数	用于用户使用了某些特殊功能需要额外扣费的场景*/
	/* 参数：iFYICount：需要扣除的点数数量*/
	/* 参数：iSurplusFYIOUT 剩下的点数  如果为-3为接受的扣除点数为负数；为-2为由于剩余点数不足导致扣除失败；为-1说明是没能找到目标卡密*/
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool __stdcall SP_Cloud_DeductFYI(__int64 iFYICount, __int64* iSurplusFYI, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的多开数量属性值 (每次调用联网) */
	/* 参数: iNum */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetOpenMaxNum(int* iNum, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的绑定机器属性值 (每次调用联网) */
	/* 参数: iBind; 是否绑机, 1/0 */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetBind(int* iBind, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的换绑周期 (每次调用联网) */
	/* 参数: iBindTime; (秒) */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetBindTime(__int64* iBindTime, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的解绑扣除属性值 (每次调用联网) */
	/* 参数: iDeductSec; (秒) */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetUnBindDeductTime(__int64* iDeductSec, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的最多解绑次数属性值 (每次调用联网) */
	/* 参数: iNum */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetUnBindMaxNum(int* iNum, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的累计解绑次数 (每次调用联网) */
	/* 参数: iCountTotal */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetUnBindCountTotal(int* iCountTotal, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆卡密的累计解绑扣除的时间 (每次调用联网) */
	/* 参数: iDeductTimeTotal; (秒) */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 是否获取成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetUnBindDeductTimeTotal(__int64* iDeductTimeTotal, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 移除当前云计算身份认证信息 (每次调用联网) */
	/* 参数: iError; 云计算错误码/状态码 */
	/* 返回: 下线是否成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_Offline(OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取公告内容 (每次调用联网) */
	/* 参数: szNoteices */
	/* 返回: 是否验证成功; 如果出错, 可参考iError  */
	bool  __stdcall SP_Cloud_GetNotices(char szNoteices[65535], OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前登陆的卡密 (SP_CloudInit 初始化成功后可用) */
	/* 参数: szCard */
	void  __stdcall SP_Cloud_GetCard(char szCard[41]);

	/* 描述: 云计算, 禁用当前登陆的卡密 (SP_CloudInit 初始化成功后可用; 每次调用联网) */
	/* 参数: iError; 云计算错误码/状态码 */
	void  __stdcall SP_Cloud_DisableCard(OUT OPTIONAL int* iError);

	/* 描述: 云计算, 获取当前客户端ID (SP_CloudInit 初始化成功后可用; 每次调用联网) */
	int  __stdcall SP_Cloud_GetCID();

	/* 描述: 云计算, 获取当前卡密在线客户端数量 (SP_CloudInit 初始化成功后可用; 每次调用联网) */
	/* 参数: iError; 云计算错误码/状态码 */
	bool  __stdcall SP_Cloud_GetOnlineCount(int* iCount, OUT OPTIONAL int* iError);

	/* 描述: 云计算, 设置云计算操作系统版本标识 (SP_CloudInit 初始化之前使用) */
	/* 参数: szWinVer; 自定义操作系统版本标识, 如果为空, 则为内置逻辑获取操作系统版本 */
	bool  __stdcall SP_Cloud_SetWinVer(char* szWinVer);
}

DWORD __stdcall Test(PVOID);

DWORD __stdcall Thread_HeartBeat(PVOID);

void TimeStamp2SystemTime(__int64 ts, SYSTEMTIME& st);

VOID ErrorCodeProc(int iError);

