
// GreenLegendDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GreenLegend.h"
#include "GreenLegendDlg.h"
#include "afxdialogex.h"
#include<vector>
#include "haoi.h"
#include <fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <thread>
#include <string>
#include "SimpleIni.h"
#pragma comment(lib,"haoi.lib")

enum CloudID {
	CloudID_Test = 1			/* 测试指令 */
};

// CGreenLegendDlg 对话框
CTabPage1 TabPage1;																					//日志功能
CTabPage2 TabPage2;																					//基本功能
CTabPage3 TabPage3;																					//战斗功能
TabPAGE4 TabPage4;																					//进程功能
TabPAGE5 TabPage5;																					//账号功能
CPlayTabPage1 PlayTabPage1;																			//玩家信息
CPlayTabPage2 PlayTabPage2;																			//背包信息
CGreenLegendDlg* Main;																				//主窗口

HWND hGameWindow = NULL;																			//游戏窗口句柄
HANDLE hMainProcess{};																				//游戏进程句柄
HWND hClientWindow;																					//客户端窗口句柄

HANDLE hFitUser = NULL;																				//脚本打人线程
HANDLE hRunScript = NULL;																			//脚本运行线程
HANDLE hTime = NULL;																				//计时线程句柄
HANDLE hCheckProtectThread = NULL;																	//保护线程句柄
HANDLE hClickThread = NULL;																			//点击提示框句柄
HANDLE hReconnectionThread = NULL;																	//保护线程句柄
HANDLE hShowInfor = NULL;																			//显示个人信息线程句柄

DWORD ThreadidOnline = 0;																			//挂机时长线程id
bool OnlineFlag = true;																				//挂机时间计时开关
extern Idmsoft* pDM;

bool CheckFlag = true;																				//云计算检测
char WindowTitlebuffer[256]{};																		//游戏窗口标题缓冲区
char WindowClassbuffer[256]{};																		//游戏窗口类名缓冲区

User GameUser{};																					//玩家自己结构体
OtherUser GameOtherObj[0x1000]{};																	//周围对象结构体
Knapsack MyKnapsack[45]{};																			//自己背包结构体

DWORD GameBase = 0;

bool CheckHp = true;																				//检测血量开关
bool Rand = true;																					//随机石开关
bool Screenshot = true;																				//截图开关

bool EndFlag = true;																				//游戏全局线程标志
bool BlackCd = true;																				//黑名单CD
bool suspend = false;																				//暂停标志
bool Initflag = true;																				//初始化标志

bool WaitBackFlag = false;																			//回城等待开关

bool StartDartsContinueFlag = false;																//押镖任务没有完成继续标志

bool EliminateDemonContinueFlag = false;															//除魔任务没有完成继续标志

bool EliminateDemonOver = true;																		//除魔全部完成标志

bool StartDartsOver = true;																			//押镖完成标志

int PrivateMapOver = 1;																				//个人副本完成标志

bool bAutoFlag=true;																				//挂机线程开关
bool bFitFlag = true;																				//打人线程开关
//SYSTEMTIME sysTime2{};
DWORD dMinute = 0;																					//分钟计时


void* WayfindingAdress = NULL;																		//走路call内存地址
void* UseGoodsAdreress = NULL;																		//使用物品call内存地址
void* ChoseGoodsDisintegrateAdreress = NULL;														//选择装备分解call内存地址
void* ChoseGoodsSaveAdreress = NULL;																//选择物品存仓库call内存地址
void* KeyAdreress = NULL;																			//按键call内存地址
void* SkillAdreress = NULL;																			//技能call内存地址

CString On_lineMap;																					//挂机地图
wchar_t wMap[0x20][0x100]{};																		//list控件中的地图

wchar_t wOn_lineMap[0x20][0x20]{};																	//没到时间要挂机的图
bool isNow = false;																					//是否有到时间的流程
int index[0x40]{};																					//地图顺序索引

//wchar_t wLifeMap[0x20][0x20]{};																		//日常任务地图

int NoEquipCount;																					//回收错误次数

CRITICAL_SECTION cs;																				//互斥体
CRITICAL_SECTION cs2;																				//互斥体2
CRITICAL_SECTION cs3;																				//互斥体3

string exepath;
CSimpleIniA ini;
string iniPath;
bool  Stop = false;




DWORD WINAPI CheakHpAndProtect(_In_ LPVOID lpParameter);
DWORD WINAPI OnlineTime(LPVOID lpParam)
{
	CString tmpStr;
	

	//GetLocalTime(&sysTime);
	WORD Day = 0;
	WORD Hour = 0;
	WORD Minute = 0;
	WORD Second = 0;
	/*GetLocalTime(&sysTime2);
	Day = sysTime2.wDay - sysTime.wDay;
	Hour = sysTime2.wHour - sysTime.wHour;
	Minute = sysTime2.wMinute - sysTime.wMinute;
	Second = sysTime2.wSecond - sysTime.wSecond;*/
	while (OnlineFlag)
	{

		tmpStr.Format(_T("[%02d天:%02d:%02d:%02d]"), Day, Hour, Minute, Second++);
		if (Second>=60)
		{
			Minute++;
			dMinute++;
			Second = 0;
		}
		if (Minute >= 60)
		{
			Hour++;
			Minute = 0;
		}
		if (Hour >= 24)
		{
			Day++;
			Hour = 0;
		}
		PlayTabPage1.PlayInfoList.SetItemText(2, 1, tmpStr);
		Sleep(985);
	}
	return 0;
}

DWORD WINAPI ShowTime(DWORD flag)
{
	//设置列内容
	CString tmpStr;
	SYSTEMTIME sysTime{};
	GetLocalTime(&sysTime);
	tmpStr.Format(_T("[%02d:%02d:%02d]"), sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	if (flag==1)
	{
		//设置登录时间
		PlayTabPage1.PlayInfoList.SetItemText(1, 1, tmpStr);
		return 1;
	}
	if (flag == 2)
	{
		hTime=CreateThread(NULL, 0, OnlineTime, NULL, 0, &ThreadidOnline);
		return 2;
	}
	return 0;
}

CGreenLegendDlg::CGreenLegendDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GREENLEGEND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGreenLegendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, PlayerInfor);
	DDX_Control(pDX, IDC_TAB2, FuncTab);
	DDX_Control(pDX, IDC_BTN_END, EndScript);
	DDX_Control(pDX, IDC_BTN_Start, StartScript);
	DDX_Control(pDX, IDC_CMB_Config, m_cbCongfig);
}

BEGIN_MESSAGE_MAP(CGreenLegendDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CGreenLegendDlg::OnTcnSelchangeTab2)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CGreenLegendDlg::OnTcnSelchangeTab1)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_Start, &CGreenLegendDlg::OnBnClickedBtnStart)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_SaveConfig, &CGreenLegendDlg::OnBnClickedBtnSaveconfig)
	ON_BN_CLICKED(IDC_BTN_OpenGame, &CGreenLegendDlg::OnBnClickedBtnOpengame)
	ON_BN_CLICKED(IDC_BTN_END, &CGreenLegendDlg::OnBnClickedBtnEnd)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CMB_Config, &CGreenLegendDlg::OnCbnSelchangeCmbConfig)
	ON_EN_CHANGE(IDC_EDIT1, &CGreenLegendDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

vector<string> laoerarr;
// CGreenLegendDlg 消息处理程序
int iError = 0;
bool bInit = false;

void initDict() {
	//laoerarr.push_back("00003d_11|3|d171b5");//一级
	laoerarr.push_back("af003d_11|3|d171b5,19|7|ffb0ff,16|15|f23afa,19|20|580020,6|15|ffffff");//二级
	laoerarr.push_back("b56a01_8|5|ff000f,14|6|c77213,16|14|ff035d,6|14|ff3772,11|21|ff355b");//三级
	laoerarr.push_back("FFFFFF_8|-1|031561,13|4|5CFFDB,12|12|22EBCC,6|17|C1AEE9,0|12|0052AB,-4|17|0A009C");//四级
	//laoerarr.push_back("FFFFFF_9|0|7A0129,14|4|D46537,10|16|C10503,2|16|B4AB01,-3|19|B80001,-4|7|BF1A72");//五级
	//laoerarr.push_back("14CBFF_11|-2|121368,14|8|3579E1,11|19|0000E8,2|20|0072FF,-3|20|8D2ECA,15|21|7D03A7,6|12|005FF6");//七级

}
BOOL CGreenLegendDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	
	Main = this;
	
	initDict();


	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 创建标签页
	GetClientRect(&m_rect);

	TCITEM tabItem;

	tabItem.mask = TCIF_TEXT;

	tabItem.pszText = TEXT("日志");

	FuncTab.InsertItem(0, &tabItem);

	tabItem.pszText = TEXT("基本");

	FuncTab.InsertItem(1, &tabItem);

	tabItem.pszText = TEXT("战斗");

	FuncTab.InsertItem(2, &tabItem);

	tabItem.pszText = TEXT("进程");

	FuncTab.InsertItem(3, &tabItem);

	tabItem.pszText = TEXT("账号");

	FuncTab.InsertItem(4, &tabItem);

	//============================================================ 人物信息Tab控件
	tabItem.mask = TCIF_TEXT;

	tabItem.pszText = TEXT("信息");

	PlayerInfor.InsertItem(0, &tabItem);

	tabItem.pszText = TEXT("背包");

	PlayerInfor.InsertItem(1, &tabItem);
	

	// 创建标签页子窗口

	TabPage1.Create(IDD_TabPAGE1, &FuncTab);

	TabPage2.Create(IDD_TabPAGE2, &FuncTab);

	TabPage3.Create(IDD_TabPAGE3, &FuncTab);

	TabPage4.Create(IDD_TabPAGE4, &FuncTab);

	TabPage5.Create(IDD_TabPAGE5, &FuncTab);

	PlayTabPage1.Create(IDD_PlayTabPage1, &PlayerInfor);

	PlayTabPage2.Create(IDD_PlayTabPage2, &PlayerInfor);

	// 窗口布局

	CRect tabRect;
	FuncTab.GetClientRect(&tabRect);
	tabRect.left += 5;
	tabRect.right -= 5;
	tabRect.top += 25;
	tabRect.bottom -= 10;
	TabPage1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	TabPage2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	TabPage3.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	TabPage4.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	TabPage5.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

	PlayerInfor.GetClientRect(&tabRect);
	tabRect.left += 5;
	tabRect.right -= 5;
	tabRect.top += 25;
	tabRect.bottom -= 10;
	PlayTabPage1.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	PlayTabPage2.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	//把对话框对象指针保存起来
	pDialog[0] = &TabPage1;
	pDialog[1] = &TabPage2;
	pDialog[2] = &TabPage3;
	pDialog[3] = &TabPage4;
	pDialog[4] = &TabPage5;


	pPlayDialog[0] = &PlayTabPage1;
	pPlayDialog[1] = &PlayTabPage2;
	//显示初始页面

	//保存当前选择
	m_CurSelTab = 0;

	m_PlayCurSelTab = 0;


	//=====================================================================初始化人物信息
	//列表控件使用

	//标题所需字段
	CString head[] = { TEXT("项目"),TEXT("数值") };

	CString name[] = { TEXT("登录信息"),TEXT("上线时间"),TEXT("挂机计时"),
					TEXT("角色名称"),TEXT("血量状态"),TEXT("挂机状态"),TEXT("角色经验") ,TEXT("物品数量") ,TEXT("金币收益") ,TEXT("绑金收益"),TEXT("经验收益") ,TEXT("统计信息"),TEXT("统计信息"),TEXT("统计信息") };

	//插入列标题
	PlayTabPage1.PlayInfoList.InsertColumn(0, head[0], LVCFMT_LEFT, 75);
	PlayTabPage1.PlayInfoList.InsertColumn(1, head[1], LVCFMT_LEFT, 190);
	

	//插入正文内容
	for (int i = 0; i < 14; i++) {
		//	CString str;
		//  str.Format(TEXT("张三_%d"),i);
		//	str.Format(TEXT("name[i]_%d"),i);

		//确定行数
		PlayTabPage1.PlayInfoList.InsertItem(i, name[i]);

	}
	//设置风格样式
	//LVS_EX_GRIDLINES 网格
	//LVS_EX_FULLROWSELECT 选中整行
	PlayTabPage1.PlayInfoList.SetExtendedStyle(PlayTabPage1.PlayInfoList.GetExtendedStyle()
		| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//-------------------------------------------地图选项控件
	CString Map = L"烟花";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"沃玛暗殿";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"祖玛暗殿";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"赤月暗殿";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"封魔暗殿";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"幻境";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"节日活动地图";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"合服活动地图";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"秘之魔境";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	Map = L"其他";
	TabPage4.m_Cob_InHook_Map1.AddString(Map);
	//-------------------------------------------------日常任务

	Map = L"除魔";
	TabPage4.m_Cob_EveryDayTask.AddString(Map);
	Map = L"押镖";
	TabPage4.m_Cob_EveryDayTask.AddString(Map);
	Map = L"个人副本";
	TabPage4.m_Cob_EveryDayTask.AddString(Map);

	//-------------------------------------------------时间编辑框
	TabPage4.m_ETime.SetWindowTextW(L"12:00");
	//------------------------------------------------流程表
	TabPage4.m_LFlowPath.InsertColumn(0, L"类型", LVCFMT_LEFT, 40);
	TabPage4.m_LFlowPath.InsertColumn(1, L"时间" ,LVCFMT_LEFT, 50);
	TabPage4.m_LFlowPath.InsertColumn(2, L"地图", LVCFMT_LEFT, 550);

	TabPage4.m_LFlowPath.SetExtendedStyle(PlayTabPage1.PlayInfoList.GetExtendedStyle()
		| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//------------------------------------------------基本界面设置
	CString cInitShow;
	cInitShow = L"3-5";
	TabPage2.m_eRedNumb.SetWindowTextW(cInitShow);
	cInitShow = L"3-5";
	TabPage2.m_eBlueNumb.SetWindowTextW(cInitShow);
	cInitShow = L"1";
	TabPage2.m_eSnowNumb.SetWindowTextW(cInitShow);
	cInitShow = L"1";
	TabPage2.m_eCureNumb.SetWindowTextW(cInitShow);
	cInitShow = L"1";
	TabPage2.m_eFixWaterNumb.SetWindowTextW(cInitShow);
	TabPage2.m_cFixWaterStart.SetCheck(BST_CHECKED);
	cInitShow = L"1";
	TabPage2.m_eBackNumb.SetWindowTextW(cInitShow);
	cInitShow = L"2";
	TabPage2.m_eBackRollNumb.SetWindowTextW(cInitShow);
	cInitShow = L"1";
	TabPage2.m_eRandNumb.SetWindowTextW(cInitShow);
	cInitShow = L"0";
	TabPage2.m_ePackSurplus.SetWindowTextW(cInitShow);
	cInitShow = L"10";
	TabPage2.m_eDungeon.SetWindowTextW(cInitShow);
	

	cInitShow = L"3";
	TabPage2.m_eRedPoison.SetWindowTextW(cInitShow);
	cInitShow = L"3";
	TabPage2.m_eGreenPoison.SetWindowTextW(cInitShow);
	cInitShow = L"3";
	TabPage2.m_eAmulet.SetWindowTextW(cInitShow);

	cInitShow = L"300";
	TabPage2.m_eTimes.SetWindowTextW(cInitShow);

	TabPage2.m_cBackRollStart.SetCheck(BST_CHECKED);

	cInitShow = L"80";
	TabPage2.m_eFire.SetWindowTextW(cInitShow);


	//------------------------------------------------战斗界面设置
	//保护功能
	cInitShow = L"80";
	TabPage3.m_eLittleRedBack.SetWindowTextW(cInitShow);
	TabPage3.m_cLittleRedBack.SetCheck(BST_CHECKED);
	cInitShow = L"85";
	TabPage3.m_eLittleRedRand.SetWindowTextW(cInitShow);
	//TabPage3.m_cLittleRedRand.SetCheck(BST_CHECKED);
	cInitShow = L"20";
	TabPage3.m_eLittleRedDown.SetWindowTextW(cInitShow);
	//TabPage3.m_cLittleRedDown.SetCheck(BST_CHECKED);
	cInitShow = L"85";
	TabPage3.m_eLittleRedScreenshot.SetWindowTextW(cInitShow);
	cInitShow = L"30";
	TabPage3.m_eBackWait.SetWindowTextW(cInitShow);
	TabPage3.m_cBackWait.SetCheck(BST_CHECKED);

	cInitShow = L"全体模式";
	TabPage3.m_cPattern.AddString(cInitShow);
	cInitShow = L"和平模式";
	TabPage3.m_cPattern.AddString(cInitShow);
	cInitShow = L"编组模式";
	TabPage3.m_cPattern.AddString(cInitShow);
	cInitShow = L"行会模式";
	TabPage3.m_cPattern.AddString(cInitShow);
	cInitShow = L"善恶模式";
	TabPage3.m_cPattern.AddString(cInitShow);
	cInitShow = L"夫妻模式";
	TabPage3.m_cPattern.AddString(cInitShow);
	cInitShow = L"师徒模式";
	TabPage3.m_cPattern.AddString(cInitShow);
	


	//检查功能
	cInitShow = L"2";
	TabPage3.m_eTimeNoMasterRand.SetWindowTextW(cInitShow);
	cInitShow = L"5";
	TabPage3.m_eRoundNoMasterRand.SetWindowTextW(cInitShow);
	cInitShow = L"10";
	TabPage3.m_eKnapsackResidue.SetWindowTextW(cInitShow);
	cInitShow = L"10";
	TabPage3.m_eAdamasEquipment.SetWindowTextW(cInitShow);

	cInitShow = L"例如(张三|李四|王五)竖线隔开";
	TabPage3.m_eBlackList.SetWindowTextW(cInitShow);

	cInitShow = L"700";
	TabPage3.m_eFit.SetWindowTextW(cInitShow);
	//-------------------------挂机点
	cInitShow = L"0";
	TabPage3.m_eFitXY.SetWindowTextW(cInitShow);
	cInitShow = L"0";
	TabPage3.m_EDIT_Y.SetWindowTextW(cInitShow);
	cInitShow = L"3";
	TabPage3.m_eXYTime.SetWindowTextW(cInitShow);
	//-------------------------账号设置
	cInitShow = L"F:\\绿色传奇\\launcher.exe   按个人情况更改";
	TabPage5.m_eGamePath.SetWindowTextW(cInitShow);


	//----------------------------------------------------------
	//显示配置信息
	ShowGameConfig();

	//----------------------------------------------------------------------

	CHANGEFILTERSTRUCT p;
	p.cbSize = sizeof(CHANGEFILTERSTRUCT);
	HWND m_HWND = AfxGetMainWnd()->m_hWnd;
	ChangeWindowMessageFilterEx(m_HWND, WM_DROPFILES, MSGFLT_ADD, &p);
	ChangeWindowMessageFilterEx(m_HWND, 0x0049, MSGFLT_ADD, &p);

	ShowTime(1);
	
	
	//CString string = pDM->Ver();
	//TabPage1.LogOut(string);
	int dm_ret = pDM->RegEx("1515130550145c17e3007c7dcb824339787f1bdfd2c", "7.2211", "123.129.231.44|123.129.231.45|123.129.231.85|123.129.231.86");
	
	if (dm_ret>0)
	{
		TabPage1.LogOut(L"辅助初始化成功");
	}

	
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs2);
	InitializeCriticalSection(&cs3);
	

	HWND Is=FindWindowA(NULL, "DbgPlugin");
	CHAR lpFilename[200]{};
	if (Is==NULL)
	{

		char ExePath[200]{};
		STARTUPINFOA stStartUpInfo;
		PROCESS_INFORMATION ProcessInformation{};


		char EXE[]{ "\\DbgGreenLegend\\DbgPlugin.exe" };




		GetModuleFileNameA(NULL, lpFilename, 200);

		memcpy(ExePath, lpFilename, strlen(lpFilename));
		int i;
		for (i = 200; lpFilename[i] != '\\'; i--);
		lpFilename[i] = 0;
		int y;
		for (y = 200; ExePath[y] != '\\'; y--);

		exepath = lpFilename;

		memcpy(ExePath + y, EXE, sizeof(EXE));


		stStartUpInfo = { sizeof(stStartUpInfo) };
		bool start = CreateProcessA(ExePath, NULL, NULL, NULL, FALSE, NULL, NULL, lpFilename, &stStartUpInfo, &ProcessInformation);

		if (start == 0)
		{
			int x = GetLastError();
			cInitShow.Format(L"游戏启动失败%d", x);
			TabPage1.LogOut(cInitShow);
			return 0;
		}
		HWND  DBG = NULL;
		while (DBG == NULL)
		{
			DBG = FindWindowA(NULL, "DbgPlugin 商业版");
			Sleep(500);
		}


		HWND Hwnd = FindWindowExA(DBG, NULL, "Button", "登录");

		HWND Hwnd2 = NULL;
		while (Hwnd2 == NULL)
		{
			PostMessageA(Hwnd, WM_LBUTTONDOWN, 0, MAKELONG(11, 8));
			Sleep(1000);
			PostMessageA(Hwnd, WM_LBUTTONUP, 0, MAKELONG(11, 8));
			Sleep(1000);
			Hwnd2 = FindWindowA(NULL, "用户协议");
		}

		HWND Hprotect = NULL;
		while (Hprotect == NULL)
		{
			Sleep(1000);
			Hprotect = FindWindowExA(Hwnd2, NULL, "Button", "同意");
		}

		HWND Hwnd3 = NULL;
		while (Hwnd2 != NULL)
		{
			PostMessageA(Hprotect, WM_LBUTTONDOWN, 0, MAKELONG(11, 8));
			Sleep(200);
			PostMessageA(Hprotect, WM_LBUTTONUP, 0, MAKELONG(11, 8));
			Sleep(200);
			Hwnd2 = FindWindowA(NULL, "用户协议");
			
		}
		Sleep(200);
		Hwnd3 = FindWindowA(NULL, "DbgPlugin");

		PostMessageA(Hprotect, WM_LBUTTONDOWN, 0, MAKELONG(11, 8));
		Sleep(200);
		PostMessageA(Hprotect, WM_LBUTTONUP, 0, MAKELONG(11, 8));

		Sleep(1000 * 10);
		//::ShowWindow(Hwnd3, SW_HIDE);
		//pDM->SetWindowState((long)Hwnd3, 6);
	}

	//iniPath = exepath + "\\config.ini";
	//int res = ini.LoadFile(iniPath.c_str());
	添加保护();
	TabPage1.LogOut(L"可以启动了");

	 //bInit = SP_CloudInit_Debug2("测试","1AC82E417AD84B7F860F257721E4EF8E", "124.221.81.11", 8896, 30 * 1000, &iError);// 调试版用的初始化接口
	
	/*if (!bInit) {
		cInitShow.Format(L"软件初始化失败! iError:%d\r\n", iError);
		TabPage1.LogOut(cInitShow);
	}*/
	
	//Test(0);

	// 测试指令
	/*char szStr[] = "我是客户端字符串";
	if (!SP_CloudComputing(CloudID_Test, (unsigned char*)szStr, strlen(szStr), &iError)) {
		cInitShow.Format(L"iError:%d\r\n", iError);
		TabPage1.LogOut(cInitShow);
	}
	else {
		int len = SP_CloudResultLength(CloudID_Test);
		char* szTmp = (char*)malloc(len + 1);
		SP_CloudResult(CloudID_Test, (unsigned char*)szTmp, len);
		szTmp[len] = '\0';
	
		CStringA tmp;

		
		tmp.Format("szTmp:%s\r\n", szTmp);
		TabPage1.LogOut(tmp);
		free(szTmp);

	}*/

	//cInitShow.Format(L"CID:%d\r\n", SP_Cloud_GetCID());
	//TabPage1.LogOut(cInitShow);

	//SP_Cloud_Offline(0);
	




	//pDM->Reg(L"sn1515130550165cf4bbf606d556afe3fc34fa6aff116", L"");//收费版本需要注册后才能使用
	/*6.1538版本修改内存可直接调用*/
	//DWORD pid = GetCurrentProcessId();
	//int handle = (int)GetModuleHandleW(L"xm.dll");

	//string.Format(L"进程ID:%d模块句柄：%x dm_ret%d", pid, handle, dm_ret);rrR
	//TabPage1.LogOut(string);
	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	//int value = 1;
	//bool is = WriteProcessMemory(hProcess, LPVOID(handle + 1078240), &value, 1, NULL);
	///*调用大漠插件内函数*/
	//pDM->MoveTo(0, 0);
	//pDM->delay(3000);
	//pDM->KeyPressStr("1234567890", 10);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGreenLegendDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。

HCURSOR CGreenLegendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGreenLegendDlg::OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	//把当前的页面隐藏起来
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_CurSelTab = FuncTab.GetCurSel();
	//把新的页面显示出来
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);

	*pResult = 0;

}


void CGreenLegendDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	//把当前的页面隐藏起来
	pPlayDialog[m_PlayCurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_PlayCurSelTab = PlayerInfor.GetCurSel();
	//把新的页面显示出来
	pPlayDialog[m_PlayCurSelTab]->ShowWindow(SW_SHOW);

	*pResult = 0;
	
}


void CGreenLegendDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	
	// TODO: 在此处添加消息处理程序代码
}

void CGreenLegendDlg::ReSize(void)
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while (hwndChild) {
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x * fsp[0]);
		TLPoint.y = long(OldTLPoint.y * fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x * fsp[0]);
		BRPoint.y = long(OldBRPoint.y * fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}

void CGreenLegendDlg::ChangeSize(UINT nID,int x, int y)
{
	CWnd* pWnd = GetDlgItem(nID);
	
	if (pWnd != NULL)
	{
		CRect rec;
		pWnd->GetWindowRect(&rec);//获取控件变化前的大小
		ScreenToClient(rec);   //将控件大小装换位在对话框中的区域坐标
		// x / m_rect.Width(); 伸缩rate
		rec.left = rec.left * x / m_rect.Width();
		rec.top = rec.top * y / m_rect.Height();
		rec.bottom = rec.bottom * y / m_rect.Height();
		rec.right = rec.right * x / m_rect.Width();
		pWnd->MoveWindow(rec);//伸缩控件
	}
}

void  CGreenLegendDlg::ReSize(int  nID, int  cx, int  cy)
{
	CWnd* pWnd;
	pWnd = GetDlgItem(nID);      //获取控件句柄
	if (pWnd)
	{
		CRect rect;    //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect); //将控件大小转换为在对话框中的区域坐标
		rect.left = (int)(rect.left * ((float)cx / (float)m_rect.Width())); //调整控件大小
		rect.right = (int)(rect.right * ((float)cx / (float)m_rect.Width()));
		rect.top = (int)(rect.top * ((float)cy / (float)m_rect.Height()));
		rect.bottom = (int)(rect.bottom * ((float)cy / (float)m_rect.Height()));
		
		pWnd->MoveWindow(rect); //设置控件位置
	}
}

void CGreenLegendDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//if (nType != SIZE_MINIMIZED)  //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	//{
	//	//ReSize(IDC_TAB1, cx, cy);
	//	//ReSize(IDC_TAB2, cx, cy);
	//	ReSize();
	//	GetClientRect(&m_rect);
	//}
}

void CGreenLegendDlg::OnBnClickedBtnStart()
{

	Stop = true;
	return;

	//char Reply[512];//储存答案变量,使用完毕后需要程序自己释放
	//char TID[512];//储存题目编号变量,使用完毕后需要程序自己释放
	//LPVOID PicBytes;
	//LONG len;
	////SetQuality(100);
	////发送GIF或者PNG图片的时候需要设置为100
	//SetRebate("1001|9A42B0F1BD994C75");
	////设置作者返利，SoftKey为作者的软件Key
	//SendFileEx("a14015|C9CF041BAF555596", "5001", "D:\\DMPic\\111111.png", 100, 0, "", TID, Reply);
	//MessageBoxA(NULL,TID, NULL, MB_OK);
	//MessageBoxA(NULL, Reply, NULL, MB_OK);

	//if (IsRight(Reply)) {//判断返回值是否正常（必须）
	//	//输入答案到游戏、判断游戏是否有"验证码错误"的提示，如果有 应该在这里调用SendError
	//}
	//else {
	//	if (Reply == "#编号不存在") {};//一般不会出现，可能出现的原因是TID 没有加IsRight判断 发送过来错误的流水编号；或者获取到返回值后 仍然一直GetAnswer 此编号
	//		 //我们的GetAnswer 根据获取的时间 返回值如下
	//		 // ""	（还未获得到答案）
	//		 // 答案	（正常接受到答案）
	//		 // 超时	（超过设置的超时时间会变成此结果）
	//		 // 编号不存在	（10分钟后会变成此结果）

	//	if (Reply == "#答案不确定") {};//截的图无法识别出答案 应延迟1秒 换一个新的验证码重新截图 重发题目
	//	if (Reply == "#超时") {};//应延迟1秒 换一个新的验证码重新截图 重发题目
	//	if (Reply == "#网络错误") {};//应延迟1秒 换一个新的验证码重新截图 重发题目


	//}
	//return;
	//CString str;
	////SaveGameConfig();
	//auto hGameWindow = pDM->FindWindowW("FBClient", "");
	//str.Format(L"句柄%d", hGameWindow);
	//TabPage1.LogOut(str);

	//if (pDM->SetPath(_T(".\\")))
	//{
	//	auto value = pDM->SetDict(0, _T("libstr.txt"));
	//	str.Format(L"字库返回值:%d", value);
	//	TabPage1.LogOut(str);
	//	pDM->UseDict(0);
	//	TabPage1.LogOut(L"多开功能开启");
	//}

	////if (pDM->BindWindowEx((long)hGameWindow, "dx.graphic.3d", "dx.mouse.state.api|dx.mouse.api|dx.mouse.cursor|dx.mouse.raw.input", "dx.keypad.state.api|dx.keypad.api", "", 0) == 0)
	//if (pDM->BindWindowEx((long)hGameWindow, "dx.graphic.3d", "dx.mouse.position.lock.api|dx.mouse.clip.lock.api|dx.mouse.input.lock.api|dx.mouse.state.api|dx.mouse.api|dx.mouse.cursor", "dx.keypad.input.lock.api|dx.keypad.state.api|dx.keypad.api|dx.keypad.raw.input", "dx.public.active.api|dx.public.active.message|dx.public.graphic.protect|dx.public.anti.api|dx.public.km.protect", 0) == 0)
	//{
	//	TabPage1.LogOut(L"辅助绑定失败");
	//}
	//else
	//	TabPage1.LogOut(L"辅助绑定成功");

	//VARIANT x{};
	//VARIANT y{};

	//while (pDM->FindStrFast(798, 216, 977, 549, "军务官", "FFFF00-101010", 0.9, &x, &y) < 0)
	//{
	//	Sleep(3000);
	//	TabPage1.LogOut(L"没");
	//}
	//TabPage1.LogOut(L"收集完成");


	// TODO: 在此添加控件通知处理程序代码
	//启动辅助
	//Start();
}


void CGreenLegendDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	wchar_t fileName[MAX_PATH]{};
	if (DragQueryFileW(hDropInfo, 0, fileName, MAX_PATH) > 0)
	{
		TabPage5.m_eGamePath.SetWindowTextW(fileName);
	}
	CDialogEx::OnDropFiles(hDropInfo);




}


DWORD WINAPI CheakHpAndProtect(_In_ LPVOID lpParameter)
{

	DWORD time = 0;
	DWORD time2 = 0;
	DWORD time3 = 0;
	DWORD time4 = 0;
	DWORD time5 = 0;
	DWORD time6 = 0;
	CString sTimes;
	DWORD state = TabPage3.m_cBackWait.GetCheck();
	TabPage3.m_eBackWait.GetWindowTextW(sTimes);
	time3 = _ttoi(sTimes);
	int state1 = TabPage3.m_cBlackListBack.GetCheck();

	int state2 = TabPage3.m_cBlackListRand.GetCheck();


	Rand = true;
	BlackCd = true;
	Screenshot = true;
	int ret = 0;
	while (CheckHp)
	{
		if (time > 15)
		{
			if (Rand == false)
			{
				Rand = true;
				time = 0;
			}
			if (BlackCd == false)
			{
				BlackCd = true;
				time = 0;
			}
		}
		if (time2 > 1500)
		{
			if (Screenshot == false)
			{
				Screenshot = true;
				time2 = 0;
			}
		}

		if (time4 > 18000)
		{
			if (EliminateDemonOver == false)
			{
				EliminateDemonOver = true;
				time4 = 0;
			}

		}

		if (time5 > 18000)
		{
			if (StartDartsOver == false)
			{
				StartDartsOver = true;
				time5 = 0;
			}

		}

		if (time6 > 18000)
		{
			if (PrivateMapOver == 0)
			{
				PrivateMapOver = 1;
				time4 = 0;
			}

		}


		if (IsSafetyArea() == 1)
		{
			if (BlackCd)
			{

				if (state1 == BST_CHECKED || state2 == BST_CHECKED)
				{
					GetUserInMap();
					if (StrCmpW(L"盟重省", GameUser.InMap) != 0 && StrCmpW(L"苍月岛", GameUser.InMap) != 0)
					{
						//检测黑名单
						ret = BlackFunc();
						if (ret > 0)
						{
							if (StrCmpW(L"跨服龙城", GameUser.InMap) == 0)
							{
								WayfindingCall(239, 202);
							}
							else
							{
								WayfindingCall(338, 335);
							}

							Sleep(1000 + Random(1, 500));
							if (IsSafetyArea() == 2 && state == BST_CHECKED)
							{
								//EliminateDemonContinueFlag = true;
								for (size_t i = 0; i < 32; i++)
								{
									if (StrStrW(wOn_lineMap[i], L"押镖") != NULL)
									{
										StartDartsContinueFlag = true;
									}
								}
								TabPage1.LogOut(L"黑名单回城等待中");
								TabPage3.m_eBackWait.GetWindowTextW(sTimes);
								time3 = _ttoi(sTimes);
								if (time3 > 0)
								{
									for (size_t i = 0; i < time3 - 1; i++)
									{
										if (IsSafetyArea() != 2)
										{
											break;
										}
										Sleep(1000 + Random(1, 500));
									}
									WaitBackFlag = false;
								}

							}
							WaitBackFlag = false;
						}

					}
				}
			}

			if (GetUserHp_MaxHp() > 0)
			{
				if (LowRedFunc(GameUser.Hp, GameUser.MaxHp) == 2)
				{
					GetUserInMap();
					if (StrCmpW(L"跨服龙城", GameUser.InMap) == 0)
					{
						WayfindingCall(239, 202);
					}
					else
					{
						WayfindingCall(338, 335);
					}
					Sleep(1000 + Random(1, 500));
					//EliminateDemonContinueFlag = true;
					for (size_t i = 0; i < 32; i++)
					{
						if (StrStrW(wOn_lineMap[i], L"押镖") != NULL)
						{
							StartDartsContinueFlag = true;
						}
					}


					if (state == BST_CHECKED && IsSafetyArea() == 2)
					{
						TabPage1.LogOut(L"被打回城等待中");
						TabPage3.m_eBackWait.GetWindowTextW(sTimes);
						time3 = _ttoi(sTimes);
						if (time3 > 0)
						{
							for (size_t i = 0; i < time3 - 1; i++)
							{
								if (IsSafetyArea() != 2)
								{
									break;
								}
								Sleep(1000 + Random(1, 500));
							}
							WaitBackFlag = false;
						}

					}
					WaitBackFlag = false;
				}
			}

		}

		Sleep(200);
		if ((Rand == false || BlackCd == false) && time <= 16)
		{
			time++;
		}

		if (Screenshot == false && time2 <= 1501)
		{
			time2++;
		}
		if (EliminateDemonOver == false && time4 <= 18001)
		{
			time4++;
		}
		if (StartDartsOver == false && time5 <= 18001)
		{
			time5++;
		}
		if (PrivateMapOver == 0 && time6 <= 18001)
		{
			time6++;
		}

	}
	return 0;
}
DWORD WINAPI ShowInfor(_In_ LPVOID lpParameter)
{
	
	long long EXP= GetUserExp();
	long long EXP2{};
	long long EXP3{};
	CString tmpStr;
	tmpStr.Format(L"%s", GameUser.Name);
	PlayTabPage1.PlayInfoList.SetItemText(3, 1, tmpStr);
	int numb = 0;
	long long moneyBind = GetUserMoney(0);
	long long money = GetUserMoney(1);
	long long moneyBind2 = 0;
	long long moneyBind3 = 0;
	long long money2 = 0;
	long long money3 = 0;

	SYSTEMTIME sysTime{};
	GetLocalTime(&sysTime);

	while (CheckHp)
	{
		if (GetUserHp_MaxHp())
		{
			tmpStr.Format(L"%d/%d", GameUser.Hp, GameUser.MaxHp);
			PlayTabPage1.PlayInfoList.SetItemText(4, 1, tmpStr);
		}
		
		if (GetOn_lineState() == 0)
		{
			tmpStr = L"挂机停止";
			PlayTabPage1.PlayInfoList.SetItemText(5, 1, tmpStr);
		}
		else
		{
			tmpStr = L"挂机中";
			PlayTabPage1.PlayInfoList.SetItemText(5, 1, tmpStr);
		}
		
		EXP2= GetUserExp();
		if (EXP2)
		{
			tmpStr.Format(L"%lld", EXP2);
			PlayTabPage1.PlayInfoList.SetItemText(6, 1, tmpStr);
		}
		if (EXP2> EXP)
		{
			EXP3 = EXP2 - EXP;
		}
		tmpStr.Format(L"%lld", EXP3);
		PlayTabPage1.PlayInfoList.SetItemText(10, 1, tmpStr);

		for (size_t i = 0; i < GameGoodsMaxNumber; i++)
		{
			if (MyKnapsack[i].Numb != 0)
			{
				numb++;
			}

		}
		
		tmpStr.Format(L"[%d / %d]", numb, GameGoodsMaxNumber);
		PlayTabPage1.PlayInfoList.SetItemText(7, 1, tmpStr);
		numb = 0;

		moneyBind2=GetUserMoney(0);
		money2=GetUserMoney(1);
		money3=money2 - money;
		moneyBind3 =moneyBind2 - moneyBind;
		
		tmpStr.Format(L"%lld", money3);
		PlayTabPage1.PlayInfoList.SetItemText(8, 1, tmpStr);
		
		tmpStr.Format(L"%lld", moneyBind3);
		PlayTabPage1.PlayInfoList.SetItemText(9, 1, tmpStr);
		
		if (dMinute>0)
		{
			tmpStr.Empty();
			tmpStr.Format(L"%lld/经验收益每分钟", EXP3 / dMinute);
			PlayTabPage1.PlayInfoList.SetItemText(11, 1, tmpStr);
			tmpStr.Empty();
			tmpStr.Format(L"%lld/金币收益每分钟",money3 / dMinute);
			PlayTabPage1.PlayInfoList.SetItemText(12, 1, tmpStr);
			tmpStr.Empty();
			tmpStr.Format(L"%lld/绑金收益每分钟", moneyBind3 / dMinute);
			PlayTabPage1.PlayInfoList.SetItemText(13, 1, tmpStr);
		}
		else
		{
			tmpStr.Format(L"%d/经验收益每分钟", 0);
			PlayTabPage1.PlayInfoList.SetItemText(11, 1, tmpStr);
			tmpStr.Format(L"%d/金币收益每分钟", 0);
			PlayTabPage1.PlayInfoList.SetItemText(12, 1, tmpStr);
			tmpStr.Format(L"%d/绑金收益每分钟", 0);
			PlayTabPage1.PlayInfoList.SetItemText(13, 1, tmpStr);
		}
	
		

		Sleep(6000);
	}


	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	/*CString logText = L"这个是一条日志消息测试";
	while (true)
	{
		TabPage1.LogOut(logText);
		Sleep(1000 + Random(1, 500));
	}*/
	SP_Begin(99);
	DWORD state = TabPage3.m_cBlackFit.GetCheck();
	
	DWORD Threadid;
	if (Initflag)
	{
		hGameWindow = NULL;
		
		GameBase = GetGameBase(NULL);

		if (GameBase != 0)
		{
			
			WayfindingAdress = VirtualAllocEx(hMainProcess, NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			UseGoodsAdreress = VirtualAllocEx(hMainProcess, NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			ChoseGoodsDisintegrateAdreress = VirtualAllocEx(hMainProcess, NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			ChoseGoodsSaveAdreress = VirtualAllocEx(hMainProcess, NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			KeyAdreress = VirtualAllocEx(hMainProcess, NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			SkillAdreress = VirtualAllocEx(hMainProcess, NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			InitUserInfor(GameBase,3);
			InitKnapsackInfor(GameBase);
			
			//检测
			if (CheckFlag)
			{
				SP_AntiDumpBegin();
				bInit = SP_CloudInit(30 * 1000, &iError);// 发布版用的初始化接口
				SP_AntiDumpEnd()
				DWORD Thread = 0;
				SP_AntiDumpBegin();
				CreateThread(0, 0, Thread_HeartBeat, 0, 0, &Thread);
				SP_AntiDumpEnd()
				SP_AntiDumpBegin()
				CheckFlag = false;
				SP_AntiDumpEnd()
			}
			

			Initflag = false;
			
		}
		else
		{
			TabPage1.LogOut(L"辅助游戏初始化失败");
		}
		
		
	}
	if (GameBase != 0)
	{

		int state3 = TabPage5.m_cAutoContinue.GetCheck();
		int state4 = TabPage2.m_cPoisonMonster.GetCheck();
		int state5 = TabPage2.m_cFire.GetCheck();

		NoEquipCount = 0;
		CheckHp = true;					//检测hp标志
		EndFlag = true;					//挂机线程标志
		bAutoFlag = true;
		WaitBackFlag = false;
		if (state==BST_CHECKED)
		{
			if (hFitUser==NULL)
			{
				//bFitFlag = true;
				//hFitUser = CreateThread(NULL, 0, FitUser, NULL, NULL, &Threadid);
				
			}
			if (hCheckProtectThread == NULL)
			{
				hCheckProtectThread = CreateThread(NULL, 0, CheakHpAndProtect, NULL, NULL, &Threadid);
			}
			
		}
		else
		{
			if (state3 == BST_CHECKED&& hReconnectionThread==NULL)
			{
				hReconnectionThread=CreateThread(NULL, 0, Reconnection, NULL, NULL, &Threadid);

			};
			if (hRunScript == NULL)
			{
				hRunScript = CreateThread(NULL, 0, RunScript, NULL, NULL, &Threadid);
			}

			if ((state4 == BST_CHECKED|| state5 == BST_CHECKED)&& hClickThread == NULL)
			{
				hClickThread=CreateThread(NULL, 0, ClickError, NULL, NULL, &Threadid);

			};

		}
		

		if (hShowInfor==NULL)
		{
			hShowInfor = CreateThread(NULL, 0, ShowInfor, NULL, NULL, &Threadid);
		}
		else
		{
			ResumeThread(hShowInfor);
		}

		if (hTime == NULL)
		{
			ShowTime(2);
		}
	}

	SP_End();
	return 0;
}


DWORD WINAPI RunScript(_In_ LPVOID lpParameter)
{
	SP_Begin(99);

	DWORD Threadid = 0;
	HWND tmpHwnd = NULL;
	bool ReStart = true;																				//检测是否在挂机地图开关
	int Count = 0;
	int Now = 0;
	int Rand = 0;
	int state = 0;
	int state2 = 0;
	int Times = 0;
	bool Sign = false;
	DWORD Overtime = 0;
	CString sMap;
	int ret = 0;
	int ret2 = 0;
	RECT rect{};

	if (hGameWindow)
	{
		if (pDM->BindWindowEx((long)hGameWindow, "dx.graphic.3d", "dx.mouse.raw.input", "dx.keypad.state.api|dx.keypad.raw.input", "", 0) == 0)
		{
			TabPage1.LogOut(L"辅助绑定失败");
		}

	}


	//GetNowGotoMap(GetOn_lineMap());
	//开启保护线程
	if (hCheckProtectThread==NULL)
	{
		hCheckProtectThread = CreateThread(NULL, 0, CheakHpAndProtect, NULL, NULL, &Threadid);
		//pDM->SetShowAsmErrorMsg(0);
	}
	
	
	//清理窗口
	pDM->KeyPress(27);
	Sleep(1000 + Random(1, 500));
	pDM->KeyPress(27);
	Sleep(1000 + Random(1, 500));
	pDM->KeyPress(27);
	
	if (TabPage5.m_CloseTeam.GetCheck()==BST_CHECKED)
	{
		VARIANT intX, intY;
		for (size_t i = 0; i < 4; i++)
		{
			if (pDM->FindPic(825, 606, 1095, 740, "team.bmp", "000000", 0.9, 0, &intX, &intY) >= 0)
			{
				moveTo(intX.intVal + 3, intY.intVal + 3, 3);
				break;
			}
			Sleep(1000 + Random(1, 500));
		}
	}

	//切换攻击模式
	ChoseUseState();
	//关闭自动追敌
	//CloseAutoTrace(0);
	//关闭自动修理
	SetAutoFix(false);
	//

	while ( EndFlag)
	{
		if (hGameWindow)
		{
			GetWindowRect(hGameWindow, &rect);
			tmpHwnd=GetForegroundWindow();
			if (tmpHwnd== hGameWindow)
			{
				SetForegroundWindow(GetNextWindow(hGameWindow, GW_HWNDNEXT));
			}
			MoveWindow(hGameWindow, rect.left, rect.top, 0x00000462, 0x0000030c, false);
		}
		Sleep(1000 + Random(1, 500));
		SP_AntiDumpBegin()
		bInit = SP_CloudInit_Debug2("", "", "124.221.81.11", 8896, 15 * 1000, &iError);// 调试版用的初始化接口
		SP_AntiDumpEnd()
		
		if (iError==0)
		{
			SP_AntiDumpBegin()
			SP_Cloud_Offline(0);
			SP_AntiDumpEnd()
		}
		
		while (WaitBackFlag)
		{
			Sleep(1000 + Random(1, 500));
			Overtime++;
			if (Overtime > 60)
			{
				Overtime = 0;
				TabPage1.LogOut(L"等待中");
			}
		}

		//是否随机进图
		state = TabPage4.m_cRand.GetCheck();
		if (state == BST_CHECKED)
		{
			Count = GetNowGotoMap(GetOn_lineMap());
			if (Count <= 0)
			{
				continue;
			}
			RandFunc(Count, true);
		}
		else
		{
			Count = GetNowGotoMap(GetOn_lineMap());
			if (Count <= 0)
			{
				continue;
			}
			RandFunc(Count, false);
		}
		

		if (ReStart)
		{
			//ReStart = false;
			if (GetNowGotoMap(GetOn_lineMap()) > 0)
			{
				GetUserInMap();
				if (wcscmp(L"盟重省", GameUser.InMap) != 0)
				{
					
					for (size_t i = 0; i < Count; i++)
					{
						if (StrStrW(wOn_lineMap[i], L"活动地图") != NULL|| StrStrW(wOn_lineMap[i], L"神秘洞穴"))
						{
							if (wcsstr(GameUser.InMap, L"地图") != NULL || wcsstr(GameUser.InMap, L"豪华") != NULL||wcsstr(GameUser.InMap, L"合服") != NULL || wcsstr(GameUser.InMap, L"神秘洞穴") != NULL)
							{
								//直接开始挂机
								sMap = wOn_lineMap[i];
								Now = i;
								Sign = true;
								//ReStart = false;
								goto On_Line;

							}
						}						
						//在挂机地图的话
						else if (wcsstr(GameUser.InMap, wOn_lineMap[i]) != NULL)
						{
							//直接开始挂机
							sMap = wOn_lineMap[i];
							Now = i;
							Sign = true;
							//ReStart = false;
							goto On_Line;
						}
					}
					
				}
			}

		}

		//不在挂机地图先回城
		while (true && EndFlag)
		{
			GetUserInMap();
			if (wcscmp(L"盟重省", GameUser.InMap) != 0)
			{
				GetUserInMap();
				if (wcscmp(L"盟重省(跨服)", GameUser.InMap) == 0)
				{
					while (FindWay(334, 322) < 0)
					{

					}

					Sleep(1000 + Random(1, 500));
					
					Overtime = 0;
					while (GetObjAndClick(5, 31, 402, 186, "返回原服", "FFFF00-000000", 3,3, -1) < 0)
					{
						GetObjAndClick(850, 153, 1107, 436, "跨服使者", "00FF00-000000", 24, -36);
						FindWay(334, 322);
						Sleep(1000 + Random(1, 500));
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}
					}

					Overtime = 0;
					while (GetObjAndClick(5, 31, 402, 186, "返回原服", "FFFF00-000000") < 0)
					{

						Sleep(1000 + Random(1, 500));
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}
					}

					Sleep(2000);


				}
				else
				{
					if (wcscmp(L"盟重省", GameUser.InMap) != 0)
					{
						
						TabPage1.LogOut(L"人不在盟重省使用回城");
						UseBack();
						Sleep(3000 + Random(1, 500));
					}
				}

			}
			GetUserInMap();
			if (wcscmp(L"盟重省", GameUser.InMap) == 0)
			{
				break;
			}
		}


		if (ret == -99)
		{
			while (EndFlag)
			{
				Sleep(1000 + Random(1, 500));
			}
		}

		//清理窗口
		pDM->KeyPress(27);
		Sleep(500 + Random(1, 500));
		pDM->KeyPress(27);
		Sleep(500 + Random(1, 500));
		pDM->KeyPress(27);

		
		Sleep(500 + Random(1, 500));

		//隐藏人物
		HideUseAndMaster(1);
		Sleep(200);
		//关闭任务自动追踪
		TaskbarTraceSwitch(false);
		Sleep(200);
		//是否随机进图
		state = TabPage4.m_cRand.GetCheck();
		if (state == BST_CHECKED)
		{
			Count = GetNowGotoMap(GetOn_lineMap());
			RandFunc(Count, true);
		}
		else
		{
			Count = GetNowGotoMap(GetOn_lineMap());
			RandFunc(Count, false);
		}

		//判断地图下标是否超过地图最大数
		if (Rand >= Count)
		{
			Rand = 0;
		}


		//去回收
		if (EndFlag)
		{
			RecycleNpcFunc();
		}


		//去分解
		if (EndFlag)
		{
			ResolveNpcFun();
		}


		//去存厂库+买药+去买道具+商城补给
		if (EndFlag)
		{
			if (SupplyGoods() >= 0)
			{


				//打开任务自动追踪
				//TaskbarTraceSwitch(true);

			On_Line:
				//去挂机
				//wchar_t wNowOn_lineMap[0x20][0x20]{};																	//到时间要挂机的图
				//wchar_t wOn_lineMap[0x20][0x20]{};																	//没到时间要挂机的图
				Sleep(1000 + Random(1, 500));
				
				//如果登录上了在挂机地图
				if (Sign)
				{
					Sign = false;
					
					sMap.Empty();
					sMap += wOn_lineMap[Now];
					ret = Go_On_lineMap(sMap,true);
					if (ret <= 0)
					{
						CString str;
						if (EliminateDemonOver == false)
						{
							str.Format(L"屠魔挂机失败任务全部完成");
						}
						else if (StartDartsOver == false)
						{
							str.Format(L"押镖挂机失败任务全部完成");
						}
						else
						{
							str.Format(L"%s 挂机失败", wOn_lineMap[Now]);
						}

						TabPage1.LogOut(str);

					}

					
				}
				else
				{
					//去挂机地图
					
					sMap.Empty();
					sMap += wOn_lineMap[index[Rand]];

					if (EndFlag)
					{
						ret = Go_On_lineMap(sMap);
						if (ret <= 0)
						{
							CString str;
							if (EliminateDemonOver == false)
							{
								str.Format(L"屠魔挂机失败任务全部完成");
							}
							else if (StartDartsOver == false)
							{
								str.Format(L"押镖挂机失败任务全部完成");
							}
							else
							{
								str.Format(L"%s 挂机失败", wOn_lineMap[index[Rand]]);
							}

							TabPage1.LogOut(str);
								
						}

						if (ret != -18 && ret != -19)
						{
							Rand++;
						}
					}


					
				}
				

			}
			else
			{
				TabPage1.LogOut(L"补给出错");
			}
		}
	}
	
	SP_End();
	
	return 0;
}

void 启动游戏() {


	STARTUPINFO StartInfo;
	PROCESS_INFORMATION pinfo;
	memset(&StartInfo, 0, sizeof(STARTUPINFO));
	StartInfo.cb = sizeof(STARTUPINFO);
	auto gamepath = ini.GetValue("Setting", "gamepath");
	STARTUPINFOA stStartUpInfo;
	PROCESS_INFORMATION ProcessInformation{};
	stStartUpInfo = { sizeof(stStartUpInfo) };

	bool start = CreateProcessA(gamepath, NULL, NULL, NULL, FALSE, NULL, NULL, exepath.c_str(), &stStartUpInfo, &ProcessInformation);
	if (start == 0)
	{
		int x = GetLastError();



		return;
	}
}
void CGreenLegendDlg::OnBnClickedBtnOpengame()
{
	//启动游戏();
	return;
	//CSimpleIniA ini2;
	//auto gCloudIniPath = "E:\\P8\\c++\\GreenLegend\\GreenLegend\\Release\\config.ini";
	//int res = ini2.LoadFile(gCloudIniPath);
	//if (res < 0)
	//	return ;

	//读配置
	auto test = ini.GetLongValue("Setting", "test");
	auto roleInx = ini.GetLongValue("账号", "当前角色");
	auto accountName = ini.GetValue("账号", "账号名称");
	auto accountPWD = ini.GetValue("账号", "密码");

	//写配置
	ini.SetLongValue("账号", "当前角色", 3);
	ini.SetValue("账号", "账号名称", "admin1");
	ini.SetValue("账号", "密码", "pass1");
	ini.SaveFile(iniPath.c_str());


	// TODO: 在此添加控件通知处理程序代码

	pDM->MoveTo(100, 100);
	Sleep(100);

	pDM->LeftClick();
	//Login();


	//CreateThread(NULL, 0, GetGameBase, NULL, NULL, &Threadid);
	//ShowTime(2);
}
int Random(int minValue, int maxValue)
{
	srand(time(nullptr));
	return rand() % (maxValue - minValue + 1) + minValue;
}
TCHAR szMessage[500][200];
void 添加保护() {

	HWND Hwnd = FindWindowA(NULL, "DbgPlugin");
	HWND Hprotect = FindWindowExA(Hwnd, NULL, "Button", "保护进程");

	HWND Hwnd2 = NULL;
	while (Hwnd2 == NULL)
	{
		PostMessageA(Hprotect, WM_LBUTTONDOWN, 0, MAKELONG(50, 11));
		Sleep(1000);
		PostMessageA(Hprotect, WM_LBUTTONUP, 0, MAKELONG(50, 11));
		Sleep(1000);
		Hwnd2 = FindWindowA(NULL, "保护进程");
	}
	HWND Hprotect2 = FindWindowExA(Hwnd2, NULL, "ComboBox", NULL);
	HWND HADDprotect = FindWindowExA(Hwnd2, NULL, "Button", "添加保护");
	SendMessageA(Hprotect2, WM_LBUTTONDOWN, 0, MAKELONG(191, 9));
	Sleep(1000);
	SendMessageA(Hprotect2, WM_LBUTTONUP, 0, MAKELONG(191, 9));
	Sleep(1500);

	UINT uCount;

	uCount = SendMessage(Hprotect2, CB_GETCOUNT, 0, 0);


	memset(szMessage, 0, sizeof(szMessage));

	for (size_t i = 0; i < uCount; i++)
	{
		SendMessage(Hprotect2, CB_GETLBTEXT, i, (LPARAM)szMessage[i]);

		if (wcsstr(szMessage[i], L"GreenLegend.exe") != NULL)
		{
			SendMessage(Hprotect2, CB_SETCURSEL, i, 0);
			break;
		}

	}
	SendMessageA(HADDprotect, WM_LBUTTONDOWN, 0, MAKELONG(50, 11));
	Sleep(1000);
	SendMessageA(HADDprotect, WM_LBUTTONUP, 0, MAKELONG(50, 11));

	PostMessageA(HADDprotect, WM_LBUTTONDOWN, 0, MAKELONG(50, 11));
	Sleep(500);
	PostMessageA(HADDprotect, WM_LBUTTONUP, 0, MAKELONG(191, 9));
	Sleep(500);
	PostMessageA(HADDprotect, WM_LBUTTONDOWN, 0, MAKELONG(50, 11));
	Sleep(500);
	PostMessageA(HADDprotect, WM_LBUTTONUP, 0, MAKELONG(191, 9));
	Sleep(500);
	PostMessageA(HADDprotect, WM_LBUTTONDOWN, 0, MAKELONG(50, 11));
	Sleep(500);
	PostMessageA(HADDprotect, WM_LBUTTONUP, 0, MAKELONG(191, 9));
	Sleep(500);
	SendMessageA(Hwnd2, WM_CLOSE, 0, 0);
	PostMessageA(Hwnd2, WM_CLOSE, 0, 0);
	Hwnd2 = FindWindowA(NULL, "保护进程");
	if (Hwnd2 != NULL)
	{
		SendMessageA(Hwnd2, WM_CLOSE, 0, 0);
	}





}


bool isRun = false;





void CGreenLegendDlg::OnBnClickedBtnSaveconfig()
{
	// TODO: 在此添加控件通知处理程序代码


	CString str;	
	//auto restult = pDM->DmGuard(1, "f1");
	//SaveGameConfig();
	auto GameWindow = pDM->FindWindowW("FBClient", "");
	str.Format(L"句柄%d", hGameWindow);
	TabPage1.LogOut(str);

	if (pDM->SetPath(_T(".\\")))
	{
		auto value = pDM->SetDict(0, _T("libstr.txt"));
		str.Format(L"字库返回值:%d", value);
		TabPage1.LogOut(str);
		pDM->UseDict(0);
		TabPage1.LogOut(L"多开功能开启");
	}

	hGameWindow = (HWND)GameWindow;
	pDM->SetWindowState(GameWindow, 1);
	if (pDM->BindWindowEx((long)GameWindow, "dx.graphic.3d", "dx.mouse.position.lock.api|dx.mouse.clip.lock.api|dx.mouse.input.lock.api|dx.mouse.state.api|dx.mouse.api|dx.mouse.cursor", "dx.keypad.input.lock.api|dx.keypad.state.api|dx.keypad.api|dx.keypad.raw.input", "dx.public.active.api|dx.public.active.message|dx.public.graphic.protect|dx.public.anti.api|dx.public.km.protect", 0) == 0)
	//if (pDM->BindWindowEx((long)hGameWindow, "normal", "normal", "normal", "", 0) == 0)
	{
		TabPage1.LogOut(L"辅助绑定失败");
		return;
	}
	else
		TabPage1.LogOut(L"辅助绑定成功");

	pDM->EnableRealMouse(2, 20, 20);
	pDM->EnableRealKeypad(1);
	//pDM->SetMouseDelay("normal",30);
	pDM->SetMouseSpeed(6);
	//pDM->SetSimMode(1);
	pDM->EnableMouseAccuracy(0);

	if (isRun)
	{
		return;
	}
	isRun = true;
	TabPage1.LogOut("开始辅助线程");
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	std::thread t1(run);
	t1.detach();


	return;


	pDM->SendString(GameWindow,"cnmlnb123");
	Sleep(1000 + Random(1, 500));
	//进入游戏
	pDM->MoveTo(511, 535);
	Sleep(100);
	pDM->LeftClick();
	Sleep(2000);

	//选服务器
	moveTo(405, 187);
	Sleep(500);
	moveTo(503, 532);
	Sleep(2000);
	

	//选角色
	pDM->MoveTo(935, 608);
	Sleep(100);
	pDM->LeftClick();



}

void 装备弹出框处理() {
	int x1 = Random(1, 15);
	int y1 = Random(3, 8);

	if (GetObjAndClick(830, 620, 893, 647, "使用", "B2C8DD-4D3722", x1, y1, -1) >= 0) {
		GetObjAndClick(830, 620, 893, 647, "使用", "B2C8DD-4D3722", x1, y1);
		Sleep(1500 + Random(1, 500));
	}
	if (GetObjAndClick(788, 539, 940, 655, "装备确定", "B2C8DD-4D3722", x1, y1, -1) >= 0) {
		GetObjAndClick(788, 539, 940, 655, "装备确定", "B2C8DD-4D3722", x1, y1);
		Sleep(1000 + Random(1, 500));

	}
	if (GetObjAndClick(250, 517, 600, 630, "转移", "AF8950-1D2716", x1, y1, -1) >= 0) {
		GetObjAndClick(250, 517, 600, 630, "转移", "AF8950-1D2716", x1, y1);
		Sleep(1000 + Random(1, 500));
	}
}

int switch_on = 0;
bool 虫族已完成 = false;
void 虫族任务() {

	int x1 = Random(1, 15);
	int y1 = Random(3, 8);
	int 任务类型 = 0;
	VARIANT x{};
	VARIANT y{};
	VARIANT intX{};
	VARIANT intY{};
	_bstr_t Jewelry;
	CString str;

	Sleep(1000 + Random(1, 500));

	装备弹出框处理();
	//    83,108,595,473  R虫族任务  D5D65A-2A290C       781,222,978,517 任务栏虫族  FFCC00-101010                     387,202,647,526 对话框大小    对话框虫族  AF8950-1D2716    虫族确定      
	//	  825,1,1004,30  血池试炼场    D8D7D7-272828            39,127,750,509 虫族教官  D1A401-2B2201   23,44         等待5分钟       350,28,698,152  地图血池   D8D7D7-272828   46,142       开始挂机  等待出现离开副本                                   
	//    825,1,1004,30 母虫之穴试炼场 D8D7D7-272828        290,26,798,490   虫族教官  D1A401-2B2201   23,44     等待4分钟				350,28,698,152  地图母虫   D8D7D7-272828   -139,162	   开始挂机  等待出现离开副本
	if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0)
	{
		pDM->KeyPress(82);
		Sleep(1000 + Random(1, 500));
	}

	if (pDM->FindStrFast(781, 222, 978, 517, "任务栏虫族", "FFCC00-101010", 0.9, &x, &y) < 0|| GetObjAndClick(854, 1, 995, 48, "太阳城", "D8D7D7-272828", x1, y1, -1) >= 0)
	{
		TabPage1.LogOut(L"接取虫族任务");
		if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) < 0)
			pDM->KeyPress(82);
		Sleep(1000 + Random(1, 500));


		if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C", x1, y1, -1) >= 0)
		{
			GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C", x1, y1);
			Sleep(1000 + Random(1, 500));
			if (GetObjAndClick(387, 202, 647, 526, "对话框虫族", "AF8950-1D2716", x1, y1, -1) < 0)
			{
				TabPage1.LogOut(L"关闭R键");
				pDM->KeyPress(82);
			}

			Sleep(3000 + Random(1, 500));
		}

		

		if (GetObjAndClick(387, 202, 647, 526, "重返", "AF8950-1D2716", x1, y1, -1) >= 0)
		{
			TabPage1.LogOut(L"重反虫族副本");
			GetObjAndClick(387, 202, 647, 526, "重返", "AF8950-1D2716", x1, y1);
			Sleep(1000 + Random(1, 500));
			goto offset;
		}
		if (GetObjAndClick(387, 202, 647, 526, "对话框虫族", "AF8950-1D2716", x1, y1, -1) < 0) {
			TabPage1.LogOut(L"没找到对话框虫族");
			return;
		}

		TabPage1.LogOut(L"找到对话框虫族");
		GetObjAndClick(387, 202, 647, 526, "对话框虫族", "AF8950-1D2716", x1, y1);
		Sleep(500 + Random(1, 500));

		if (GetObjAndClick(387, 202, 647, 526, "离开", "AF8950-1D2716", x1, y1, -1) >= 0)
		{
			if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0)
			{
				pDM->KeyPress(82);
				Sleep(1000 + Random(1, 500));
			}
			虫族已完成 = true;
			return;
		}

		if (GetObjAndClick(387, 202, 647, 526, "虫族确定", "AF8950-1D2716", x1, y1, -1) < 0) {
			
			
				TabPage1.LogOut(L"没找到虫族确定");
				return;
			
		}
		else
		{
			TabPage1.LogOut(L"找到虫族确定");
			GetObjAndClick(387, 202, 647, 526, "虫族确定", "AF8950-1D2716", x1, y1);
		}
		
		Sleep(1000 + Random(1, 500));
		if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0)
		{
			pDM->KeyPress(82);
			Sleep(500 + Random(1, 500));
		}
		
	}
	offset:
	TabPage1.LogOut(L"执行虫族任务");
	Sleep(1000 + Random(1, 500));
	if (GetObjAndClick(854, 1, 995, 48, "血池试炼场", "D8D7D7-272828", x1, y1, -1) >= 0)
	{
		TabPage1.LogOut(L"血池试炼场");

		if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0)
		{
			pDM->KeyPress(82);
			Sleep(500 + Random(1, 500));
		}
		int x = 0;
		while (GetObjAndClick(39, 127, 750, 509, "虫族教官", "D1A401-2B2201", x1 + 23, y1 + 44)<0)
		{
			if (x>10)
			{
				break;
			}
			x++;
			Sleep(1000);
		}


		Sleep(5000);
		if (GetObjAndClick(387, 202, 647, 526, "对话框准备", "AF8950-1D2716", x1, y1, -1) >= 0)
		{
			GetObjAndClick(387, 202, 647, 526, "对话框准备", "AF8950-1D2716", x1, y1);
		}
		else
		{
			TabPage1.LogOut(L"没有找到虫族教官对话框准备");
			return;
		}

		

		CString str = pDM->Ocr(2, 3, 40, 46, "ffffff-606050", 0.95);
		TabPage1.LogOut(str);
		if (str.Find(L"43") >= 0)
		{
			TabPage1.LogOut(L"开始等待教官通关15分钟");
			Sleep(1000 * 60 * 5);
		}
		else
		{
			TabPage1.LogOut(L"开始等待教官通关5分钟");
			Sleep(1000 * 60 * 5);
		}
		

		
		pDM->KeyPress(9);
		Sleep(500 + Random(1, 500));
		//350,28,698,152  地图血池   D8D7D7-272828   46,142
		while(GetObjAndClick(350, 28, 698, 152, "地图血池", "D8D7D7-272828", x1+ 46, y1+ 142, -1) < 0)
		{
			TabPage1.LogOut(L"没有找到虫族地图血池");
			pDM->KeyPress(9);
			Sleep(1500);
			
		}
		GetObjAndClick(350, 28, 698, 152, "地图血池", "D8D7D7-272828", 46, 142);
		Sleep(1500);
		pDM->KeyPress(9);
		Sleep(1000 * 20);



	}

	if (GetObjAndClick(854, 1, 995, 48, "母虫之穴试炼场", "D8D7D7-272828", x1, y1, -1) >= 0)
	{
		TabPage1.LogOut(L"母虫之穴试炼场");
		if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0)
		{
			pDM->KeyPress(82);
			Sleep(500 + Random(1, 500));
		}

		int x = 0;
		while (GetObjAndClick(39, 127, 750, 509, "虫族教官", "D1A401-2B2201", x1 + 23, y1 + 44) < 0)
		{
			if (x > 10)
			{
				break;
			}
			x++;
			Sleep(1000);
		}
	
		Sleep(500 + Random(1, 500));
		if (GetObjAndClick(387, 202, 647, 526, "对话框准备", "AF8950-1D2716", x1, y1, -1) >= 0)
		{
			GetObjAndClick(387, 202, 647, 526, "对话框准备", "AF8950-1D2716", x1, y1);
		}
		else
		{
			TabPage1.LogOut(L"没有找到虫族教官对话框准备");
			return;
		}
		TabPage1.LogOut(L"开始等待教官通关");
		Sleep(1000 * 60 * 4);


		pDM->KeyPress(9);
		Sleep(500 + Random(1, 500));
		
		while (GetObjAndClick(350, 28, 698, 152, "地图母虫", "D8D7D7-272828", x1 + 46, y1 + 142, -1) < 0)
		{
			TabPage1.LogOut(L"没有找到虫族地图血池");
			pDM->KeyPress(9);
			Sleep(1500);

		}
		GetObjAndClick(350, 28, 698, 152, "地图母虫", "D8D7D7-272828", -139, 162);
		Sleep(1500);
		pDM->KeyPress(9);
		Sleep(1000 * 27);
	}


	if (GetObjAndClick(855, 4, 978, 27, "巨兽之窿", "D8D7D7-272828", x1, y1, -1) >= 0)
	{
		if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0)
		{
			pDM->KeyPress(82);
			Sleep(500 + Random(1, 500));
		}

		TabPage1.LogOut(L"巨兽之窿");

		int x = 0;
		while (GetObjAndClick(39, 127, 750, 509, "虫族教官", "D1A401-2B2201", x1 + 23, y1 + 44) < 0)
		{
			if (x > 10)
			{
				break;
			}
			x++;
			Sleep(1000);
		}

		Sleep(5000);
		if (GetObjAndClick(387, 202, 647, 526, "对话框准备", "AF8950-1D2716", x1, y1, -1) >= 0)
		{
			GetObjAndClick(387, 202, 647, 526, "对话框准备", "AF8950-1D2716", x1, y1);
		}
		else
		{
			TabPage1.LogOut(L"没有找到虫族教官对话框准备");
			return;
		}

		TabPage1.LogOut(L"开始等待教官通关");
		Sleep(1000 * 60 *4);


		pDM->KeyPress(9);
		Sleep(500 + Random(1, 500));
		//350,28,698,152  地图血池   D8D7D7-272828   46,142  12.23	
		while (GetObjAndClick(350, 28, 698, 179, "试炼场", "D8D7D7-272828", x1 + 46, y1 + 142, -1) < 0)
		{
			TabPage1.LogOut(L"没有找到虫族地图巨兽之窿");
			pDM->KeyPress(9);
			Sleep(1500);

		}
		GetObjAndClick(350, 28, 698, 179, "试炼场", "D8D7D7-272828", -117, 263);
		Sleep(1500);
		pDM->KeyPress(9);

		Sleep(1000 * 40);
	}

	int times = 0;
	while (pDM->FindMultiColor(348, 18, 596, 95, "7B0000", "3|1|7D0000,0|-2|7C0000", 0.98, 0, &x, &y) ==0) {
	
		
		pDM->KeyDown(18);
		Sleep(100);
		pDM->KeyPress(65);
		Sleep(100);
		pDM->KeyUp(18);
		
		Sleep(1500 + Random(1, 500));
		times++;
		if (times>30)
		{
			break;
		}
	}
	

	while (GetObjAndClick(393, 41, 672, 129, "离开副本", "CCB066-000000", x1, -y1, -1) < 0)
	{
		Sleep(3000 + Random(1, 500));
		TabPage1.LogOut(L"等待开启宝箱");

		if (GetObjAndClick(512, 513, 526, 533, "复活", "AA8750-212816", x1, y1, -1) >= 0) 
		{
			bool is回城复活 = false;
			x1 = Random(1, 15);
			y1 = Random(1, 8);
			if (!is回城复活) {
				y1 = Random(31, 38);
			}
			GetObjAndClick(512, 513, 526, 533, "复活", "AA8750-212816", x1, y1);
			TabPage1.LogOut(L"复活等待10秒");
			Sleep(10000 + Random(1, 500));
			if (GetObjAndClick(854, 1, 995, 48, "母虫之穴试炼场", "D8D7D7-272828", x1, y1, -1) >= 0)
			{
				TabPage1.LogOut(L"复活母虫之穴试炼场");
				pDM->KeyPress(9);
				Sleep(500 + Random(1, 500));

				while (GetObjAndClick(350, 28, 698, 152, "地图母虫", "D8D7D7-272828", x1 + 46, y1 + 142, -1) < 0)
				{
					TabPage1.LogOut(L"没有找到虫族地图血池");
					pDM->KeyPress(9);
					Sleep(1500);

				}
				GetObjAndClick(350, 28, 698, 152, "地图母虫", "D8D7D7-272828", -139, 162);
				Sleep(1500);
				pDM->KeyPress(9);
				Sleep(1000 * 27);
			}


			if (GetObjAndClick(854, 1, 995, 48, "血池试炼场", "D8D7D7-272828", x1, y1, -1) >= 0)
			{
				TabPage1.LogOut(L"复活血池试炼场");
				pDM->KeyPress(9);
				Sleep(500 + Random(1, 500));
				//350,28,698,152  地图血池   D8D7D7-272828   46,142
				while (GetObjAndClick(350, 28, 698, 152, "地图血池", "D8D7D7-272828", x1 + 46, y1 + 142, -1) < 0)
				{
					TabPage1.LogOut(L"没有找到虫族地图血池");
					pDM->KeyPress(9);
					Sleep(1500);

				}
				GetObjAndClick(350, 28, 698, 152, "地图血池", "D8D7D7-272828", 46, 142);
				Sleep(1500);
				pDM->KeyPress(9);
				Sleep(1000 * 20);

			}

			if (GetObjAndClick(855, 4, 978, 27, "巨兽之窿", "D8D7D7-272828", x1, y1, -1) >= 0)
			{
				TabPage1.LogOut(L"复活巨兽之窿");
				pDM->KeyPress(9);
				Sleep(500 + Random(1, 500));
				//350,28,698,152  地图血池   D8D7D7-272828   46,142  12.23	
				while (GetObjAndClick(350, 28, 698, 179, "试炼场", "D8D7D7-272828", x1 + 46, y1 + 142, -1) < 0)
				{
					TabPage1.LogOut(L"没有找到虫族地图巨兽之窿");
					pDM->KeyPress(9);
					Sleep(1500);

				}
				GetObjAndClick(350, 28, 698, 179, "试炼场", "D8D7D7-272828", -117, 263);
				Sleep(1500);
				pDM->KeyPress(9);

				Sleep(1000 * 40);

			}
			while (pDM->FindStrFast(414, 94, 619, 223, "开启", "AFAF01-505001", 0.9, &x, &y) < 0)
			{
				pDM->KeyDown(18);
				Sleep(200);
				pDM->KeyPress(65);
				Sleep(200);
				pDM->KeyUp(18);
				Sleep(1000);
			}

			TabPage1.LogOut(L"开启挂机");
		}

		

	}
	GetObjAndClick(393, 41, 672, 129, "离开副本", "CCB066-000000", x1, -y1);
	Sleep(1000 + Random(1, 500));
	return;

}

void 五色任务() {
	int x1 = Random(1, 15);
	int y1 = Random(3, 8);
	int 任务类型 = 0;
	VARIANT x{};
	VARIANT y{};
	VARIANT intX{};
	VARIANT intY{};
	_bstr_t Jewelry;
	CString str;
}


void 联邦任务() {
	int x1 = Random(1, 15);
	int y1 = Random(3, 8);
	int 任务类型 = 0;
	VARIANT x{};
	VARIANT y{};
	VARIANT intX{};
	VARIANT intY{};
	_bstr_t Jewelry;
	CString str;





	Sleep(1000 + Random(1, 500));

	装备弹出框处理();



	if (GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C", x1, y1, -1) >=0)
	{
		pDM->KeyPress(82);
		Sleep(500 + Random(1, 500));
		TabPage1.LogOut(L"R成长手册打开了");
	}


	if (pDM->FindStrFast(793, 222, 975, 373, "联邦任务", "FFCC00-101010", 0.9, &x, &y) < 0)
	{
		TabPage1.LogOut(L"接取联邦任务");
		if (GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C", x1, y1, -1) < 0)
		pDM->KeyPress(82);
		Sleep(1500 + Random(1, 500));

		if (GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C", x1, y1, -1) >= 0)
		{
			GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C", x1, y1);
			Sleep(300 + Random(1, 500));
			x1 = Random(1, 15);
			y1 = Random(3, 8);
			pDM->MoveTo(416+ x1, 129+ y1);
			Sleep(300 + Random(1, 500));
			if (GetObjAndClick(384, 202, 653, 532, "对话联邦任务", "9F8A50-2D2616", x1, y1, -1) < 0)
			{
				TabPage1.LogOut(L"关闭R键");
				pDM->KeyPress(82);
			}
				
			Sleep(3000 + Random(1, 500));
		}
		else
		{
			TabPage1.LogOut(L"没有找到R联邦任务");
		}

		if (GetObjAndClick(384, 202, 653, 532, "对话联邦任务", "9F8A50-2D2616", x1, y1, -1) < 0) {
			TabPage1.LogOut(L"没找到对话联邦任务");
			return;
		}

		TabPage1.LogOut(L"找到联邦任务对话");
		GetObjAndClick(384, 202, 653, 532, "对话联邦任务", "9F8A50-2D2616", x1, y1);

		Sleep(1000 + Random(1, 500));	
		if (GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C", x1, y1, -1) >= 0)
		{
			pDM->KeyPress(82);
			TabPage1.LogOut(L"R成长手册打开了2");
			Sleep(500 + Random(1, 500));
		}
		return;
	}
	
	TabPage1.LogOut(L"开始执行联邦任务");
	
	

	

	if (pDM->FindStrFast(x.intVal, y.intVal, x.intVal + 193, y.intVal + 58, "带给", "FF3333-000000", 0.9, &x, &y) >= 0) {

		while (GetObjAndClick(384, 202, 653, 532, "NPC联邦对话", "9F8A50-2D2616", x1, y1, -1) < 0)
		{
			moveTo(x.intVal + 25, y.intVal + 2);
			Sleep(5000);
		}
		GetObjAndClick(384, 202, 653, 532, "NPC联邦对话", "9F8A50-2D2616", x1, y1);
	}

	//4  82   5  120
	if (pDM->FindStrFast(793, 222, 975, 373, "购买三", "FF3333-000000", 0.96, &x, &y) >= 0) {

		while (GetObjAndClick(384, 202, 653, 532, "购买", "D8D7D7-272828", x1,y1, -1) < 0)
		{
			moveTo(x.intVal + 46, y.intVal + 38);
			Sleep(5000);
		}
		GetObjAndClick(384, 202, 653, 532, "购买", "D8D7D7-272828", x1, y1);
		Sleep(1000 + Random(1, 500));
		GetObjAndClick(311, 156, 663, 572, "商店枪械师", "D7D7AC-282820", x1, y1);
		Sleep(1000 + Random(1, 500));
		GetObjAndClick(311, 156, 663, 572, "商店枪械师", "D7D7AC-282820", x1, y1+ 41,3);
		Sleep(1000 + Random(1, 500));
		pDM->KeyPress(27);
		Sleep(300 + Random(1, 500));
		pDM->KeyPress(27);		
	}

	


	if (pDM->FindStrFast(793, 222, 975, 373, "购买四", "FF3333-000000", 0.96, &x, &y) >= 0) {

		while (GetObjAndClick(384, 202, 653, 532, "购买", "D8D7D7-272828", x1, y1, -1) < 0)
		{
			moveTo(x.intVal + 46, y.intVal + 38);
			Sleep(5000);
		}
		GetObjAndClick(384, 202, 653, 532, "购买", "D8D7D7-272828", x1, y1);
		Sleep(1000 + Random(1, 500));
		GetObjAndClick(311, 156, 663, 572, "商店枪械师", "D7D7AC-282820", x1, y1);
		Sleep(1000 + Random(1, 500));
		GetObjAndClick(311, 156, 663, 572, "商店枪械师", "D7D7AC-282820", x1, y1 + 83, 3);
		Sleep(1000 + Random(1, 500));
		pDM->KeyPress(27);
		Sleep(300 + Random(1, 500));
		pDM->KeyPress(27);
	}

	//4  82   5  120
	if (pDM->FindStrFast(793, 222, 975, 373, "购买五", "FF3333-000000", 0.96, &x, &y) >= 0) {

		while (GetObjAndClick(384, 202, 653, 532, "购买", "D8D7D7-272828", x1, y1, -1) < 0)
		{
			moveTo(x.intVal + 46, y.intVal + 38);
			Sleep(5000);
		}
		GetObjAndClick(384, 202, 653, 532, "购买", "D8D7D7-272828", x1, y1);
		Sleep(1000 + Random(1, 500));
		GetObjAndClick(311, 156, 663, 572, "商店枪械师", "D7D7AC-282820", x1, y1);
		Sleep(1000 + Random(1, 500));
		GetObjAndClick(311, 156, 663, 572, "商店枪械师", "D7D7AC-282820", x1, y1 + 126, 3);
		Sleep(1000 + Random(1, 500));
		pDM->KeyPress(27);
		Sleep(300 + Random(1, 500));
		pDM->KeyPress(27);
	}


	x1 = Random(5, 15);
	y1 = Random(2, 9);
	//采集
	if (pDM->FindStrFast(793, 222, 975, 373, "联邦任务", "FFCC00-101010", 0.9, &x, &y) >= 0)
	{
		str.Format(L"联邦任务: %d,%d", x.intVal, y.intVal);
		TabPage1.LogOut(str);
		if (pDM->FindStrFast(x.intVal, y.intVal, x.intVal + 193, y.intVal + 58, "采集", "FF3333-000000", 0.9, &x, &y) >= 0) 
		{
			str.Format(L"采集: %d,%d", x.intVal, y.intVal);
			TabPage1.LogOut(str);
			moveTo(x.intVal + 25, y.intVal + 2);

			pDM->MoveTo(x.intVal -60, y.intVal - 60);
			while (pDM->FindStrFast(798, 216, 977, 549, "尼尔", "FFFF00-101010", 0.9, &x, &y) < 0)
			{
					Sleep(3000);
			}
			TabPage1.LogOut(L"收集完成");
		}
	}

	x1 = Random(5, 15);
	y1 = Random(2, 9);
	//杀死
	if (pDM->FindStrFast(793, 222, 975, 373, "联邦任务", "FFCC00-101010", 0.9, &x, &y) >= 0)
	{
		str.Format(L"联邦任务: %d,%d", x.intVal, y.intVal);
		TabPage1.LogOut(str);
		if (pDM->FindStrFast(x.intVal, y.intVal, x.intVal + 193, y.intVal + 58, "杀死", "D72C2C-280808", 0.9, &x, &y) >= 0)
		{
			str.Format(L"杀死: %d,%d", x.intVal, y.intVal);
			TabPage1.LogOut(str);
			moveTo(x.intVal + 25+ x1, y.intVal + y1);

			pDM->MoveTo(x.intVal - 60, y.intVal - 60);
			while (pDM->FindStrFast(798, 216, 977, 549, "尼尔", "FFFF00-101010", 0.9, &x, &y) < 0)
			{
				Sleep(3000 + Random(1000, 5000));
				while (pDM->FindMultiColor(348, 18, 596, 95, "7B0000", "3|1|7D0000,0|-2|7C0000", 0.98, 0, &x, &y) == 1) {

					if (GetObjAndClick(370, 27, 447, 87, "系统", "D7D7D7-282828", x1, y1, -1) >= 0)
					{
						break;
					}

					TabPage1.LogOut(L"自动打怪中");
					Sleep(1000 + Random(1, 500));

				}
				moveTo(x.intVal + 25 + x1, y.intVal + y1);
			}
			TabPage1.LogOut(L"杀死完成");
		}
	}
	

	while (GetObjAndClick(384, 202, 653, 532, "任务完成", "9F8A50-2D2616", x1, y1, -1) < 0)
	{
		TabPage1.LogOut(L"完成联邦任务");

		if (GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C", x1, y1, -1) < 0)
		{
			pDM->KeyPress(82);
			Sleep(500 + Random(1, 500));
		}
	
		if (GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0) {
			GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C|07D707-082807", x1, y1);
			Sleep(2000 + Random(1, 500));
			x1 = Random(1, 15);
			y1 = Random(3, 8);
			pDM->MoveTo(416 + x1, 129 + y1);
			Sleep(2000+ Random(1, 500));

		}

		if (GetObjAndClick(384, 202, 653, 532, "检查", "9F8A50-2D2616", x1, y1, -1)>=0)
		{
			break;
		}
	}
	if (pDM->FindStrFast(384, 202, 653, 532, "对话框三等", "D72C2C-280808", 0.96, &x, &y) >= 0)
	{
		Jewelry = "threeJewelry.bmp";
		任务类型 = 1;
	}

	if (pDM->FindStrFast(384, 202, 653, 532, "对话框四等", "D72C2C-280808", 0.96, &x, &y) >= 0)
	{
		Jewelry = "fourJewelry.bmp";
		任务类型 = 1;
	}
	if (pDM->FindStrFast(384, 202, 653, 532, "对话框五等", "D72C2C-280808", 0.96, &x, &y) >= 0)
	{
		Jewelry = "fiveJewelry.bmp";
		任务类型 = 1;
	}

	if (GetObjAndClick(384, 202, 653, 532, "检查", "9F8A50-2D2616", x1, y1, -1) >= 0)
	{
		GetObjAndClick(384, 202, 653, 532, "检查", "9F8A50-2D2616", x1, y1);
	}
	else
	{
		GetObjAndClick(384, 202, 653, 532, "任务完成", "9F8A50-2D2616", x1, y1);
	}

	Sleep(1000 + Random(1, 500));

	int errorNum = 0;


	while (pDM->FindStrFast(875, 300, 1014, 395, "反外挂答题", "c4a765-101010", 0.98, &x, &y) >= 0) {

		TabPage1.LogOut("截图答题");
		pDM->CapturePng(596, 246, 1016, 620, "c:\\cap.png");
		char Reply[512];//储存答案变量,使用完毕后需要程序自己释放
		char TID[512];//储存题目编号变量,使用完毕后需要程序自己释放
		LPVOID PicBytes;
		LONG len;
		//SetQuality(100);
		//发送GIF或者PNG图片的时候需要设置为100
		SetRebate("1001|9A42B0F1BD994C75");
		//设置作者返利，SoftKey为作者的软件Key
		SendFileEx("a14015|C9CF041BAF555596", "5001", "c:\\cap.png", 100, 0, "", TID, Reply);


		if (IsRight(Reply)) {//判断返回值是否正常（必须）
			//输入答案到游戏、判断游戏是否有"验证码错误"的提示，如果有 应该在这里调用SendError

			int ans = atoi(Reply);
			str.Format(L"返回答案项:%d", ans);
			TabPage1.LogOut(str);

			moveTo(683, (ans - 1) * 30 + 409, 0);
			Sleep(1000 + Random(1, 500));
			moveTo(728, 555, 0);
			Sleep(1000 + Random(1, 500));
			errorNum = 0;
		}
		else {
			if (Reply == "#编号不存在") {};//一般不会出现，可能出现的原因是TID 没有加IsRight判断 发送过来错误的流水编号；或者获取到返回值后 仍然一直GetAnswer 此编号
				 //我们的GetAnswer 根据获取的时间 返回值如下
				 // ""	（还未获得到答案）
				 // 答案	（正常接受到答案）
				 // 超时	（超过设置的超时时间会变成此结果）
				 // 编号不存在	（10分钟后会变成此结果）

			if (Reply == "#答案不确定") {};//截的图无法识别出答案 应延迟1秒 换一个新的验证码重新截图 重发题目
			if (Reply == "#超时") {};//应延迟1秒 换一个新的验证码重新截图 重发题目
			if (Reply == "#网络错误") {};//应延迟1秒 换一个新的验证码重新截图 重发题目
			errorNum++;
			str.Format(L"答题失败次数:%d", errorNum);
			TabPage1.LogOut(str);
			moveTo(926, 556, 0);
		}

		Sleep(1000 + Random(1, 500));

	}


	if (任务类型==1)
	{
		if (pDM->FindPic(698, 250, 985, 536, Jewelry, "000000", 0.8, 0, &intX, &intY) >= 0)
		{
			moveTo(intX.intVal + 3, intY.intVal + 3, 3);
			str.Format(L"找到首饰%d:%d", intX.intVal, intX.intVal);
			TabPage1.LogOut(str);
		}
		TabPage1.LogOut(Jewelry.GetBSTR());
		Sleep(1000 + Random(1, 500));
		GetObjAndClick(440, 351, 655, 552, "首饰任务确定", "AF8950-1D2716", x1, y1);
		Sleep(500 + Random(1, 500));

		if (GetObjAndClick(714, 139, 952, 245, "物品栏", "D8D7D7-272828", x1, y1, -1) >= 0)
		{
			pDM->KeyPress(66);
			Sleep(500 + Random(1, 500));
		}

	}



	
	if (GetObjAndClick(61, 105, 583, 649, "联邦任务", "D5D65A-2A290C", x1, y1, -1) >= 0)
	{
		pDM->KeyPress(82);
		Sleep(500 + Random(1, 500));
	}

	if (GetObjAndClick(387, 202, 647, 526, "离开", "AF8950-1D2716", x1, y1, -1) >= 0)
	{
		GetObjAndClick(387, 202, 647, 526, "离开", "AF8950-1D2716", x1, y1);
	}

	return;

}



void 风暴试炼()
{
	VARIANT x{};
	VARIANT y{};



	pDM->KeyPress(9);
	Sleep(1000 + Random(300, 500));

	/*moveTo(187, 388);
	moveTo(405, 410);
	moveTo(638, 353);*/


	moveTo(188, 423);
	Sleep(15000 + Random(1, 500));
	pDM->KeyDown(18);
	Sleep(200);
	pDM->KeyPress(65);
	Sleep(200);
	pDM->KeyUp(18);
	Sleep(1000 * 60 + Random(1, 500));//4.30  7.46
		
	moveTo(406, 448	);
	Sleep(15000 + Random(1, 500));
	pDM->KeyDown(18);
	Sleep(200);
	pDM->KeyPress(65);
	Sleep(200);
	pDM->KeyUp(18);
	Sleep(1000 * 60 + Random(1, 500));

	moveTo(638, 353);
	Sleep(10000 + Random(1, 500));
	pDM->KeyDown(18);
	Sleep(200);
	pDM->KeyPress(65);
	Sleep(200);
	pDM->KeyUp(18);
	Sleep(1000 * 60 + Random(1, 500));
	pDM->KeyPress(9);
	return;
}

void 主线() {
	CString str;
	VARIANT x{};
	VARIANT y{};



	int x1 = Random(1, 15);
	int y1 = Random(3, 7);

	
	//if (pDM->FindMultiColor(348, 18, 596, 95, "7B0000", "3|1|7D0000,0|-2|7C0000", 0.98, 0, &x, &y) == 1) {
	//	TabPage1.LogOut(L"自动打怪中");
	//	Sleep(1000 + Random(1, 500));
	//	return;
	//}
	if (GetObjAndClick(725, 358, 876, 435, "继续对话|任务按钮", "B2C8DD-4D3722", x1, y1, -1) >= 0) {
		GetObjAndClick(725, 358, 876, 435, "继续对话|任务按钮", "B2C8DD-4D3722", x1 + 32, y1+6);
		pDM->MoveTo(617 + Random(0, 161), 16 + Random(0, 48));
		Sleep(200 + Random(1, 200));
		return;
	}

	if (GetObjAndClick(741, 405, 855, 445, "娱乐交友", "B2C8DD-4D3722", x1, y1, -1) >= 0) {
		GetObjAndClick(741, 405, 855, 445, "娱乐交友", "B2C8DD-4D3722", x1, y1);
		Sleep(1500 + Random(1, 500));
		GetObjAndClick(591, 382, 892, 471, "全天", "d8d7d7-272828", x1, y1);
		Sleep(1000 + Random(1, 500));
		return;
	}

	if (GetObjAndClick(741, 405, 855, 445, "继续对话|任务按钮", "B2C8DD-4D3722", x1, y1, -1) >= 0) {
		GetObjAndClick(741, 405, 855, 445, "继续对话|任务按钮", "B2C8DD-4D3722", x1 + 32, y1 + 6);
		pDM->MoveTo(617 + Random(0, 161), 16 + Random(0, 48));
		Sleep(200 + Random(1, 200));
		return;
	}	


	
	if (GetObjAndClick(419, 35, 644, 129, "离开副本", "ccb066-000000", x1, y1, -1) >= 0) {
		GetObjAndClick(419, 35, 644, 129, "离开副本", "ccb066-000000", x1, y1);
		Sleep(1000 + Random(1, 500));
		return;
	}

	//355, 141, 699, 583  458,344 亚伯任务
	if (GetObjAndClick(419, 332, 455, 361, "失之交臂", "c3a861-202020", x1, y1, -1) >= 0) {
		GetObjAndClick(419, 332, 455, 361, "失之交臂", "c3a861-202020", x1, y1);
		Sleep(1000 + Random(1, 500));
		return;
	}
	if (GetObjAndClick(831, 186, 908, 257, "关闭详情", "e5d5d9-101010", x1, y1, -1) >= 0) {
		GetObjAndClick(831, 186, 908, 257, "关闭详情", "e5d5d9-101010", x1, y1);
		Sleep(1000 + Random(1, 500));
		return;
	}

	if (GetObjAndClick(874, 114, 929, 175, "关闭赛事", "fff9d8-101010", x1, y1, -1) >= 0) {
		GetObjAndClick(874, 114, 929, 175, "关闭赛事", "fff9d8-101010", x1, y1);
		Sleep(1000 + Random(1, 500));
		return;
	}
	if (GetObjAndClick(512, 513, 526, 533, "复活", "AA8750-212816", x1, y1, -1) >= 0) {
		bool is回城复活 = false;
		x1 = Random(1, 15);
		y1 = Random(1, 8);
		if (!is回城复活) {
			y1 = Random(31, 38);
		}
		GetObjAndClick(512, 513, 526, 533, "复活", "AA8750-212816", x1, y1);
		Sleep(1000 + Random(1, 500));
		return;
	}

	if (GetObjAndClick(521, 419, 566, 438, "确定", "B2C8DD-4D3722", x1, y1, -1) >= 0) {
		GetObjAndClick(521, 419, 566, 438, "确定", "B2C8DD-4D3722", x1, y1);
		Sleep(1000 + Random(1, 500));
		return;
	}

	if (GetObjAndClick(387, 202, 647, 526, "重返", "AF8950-1D2716", x1, y1, -1) >= 0)
	{
		TabPage1.LogOut(L"重返副本");
		GetObjAndClick(387, 202, 647, 526, "重返", "AF8950-1D2716", x1, y1);
		Sleep(1000 + Random(1, 500));
	}

	装备弹出框处理();


	
	//特殊标记点掉
	if (pDM->FindMultiColor(464, 446, 560, 552, "3B2219", "21|-4|EEE000,15|9|DBCE00,7|20|9F960F,-10|20|DBB09A,-10|9|570F1E,3|10|6D4245", 0.98, 0, &x, &y) > 0) {
		moveTo(x.intVal + Random(1, 30), y.intVal + Random(1, 30), 0);
		Sleep(500 + Random(1, 500));
		return;
	}
	if (pDM->FindMultiColor(460, 446, 600, 584, "E3A7F3", "-19|24|C47D5D,14|28|8F4032", 0.98, 0, &x, &y) > 0) {
		moveTo(x.intVal + Random(1, 30), y.intVal + Random(1, 30), 0);
		Sleep(500 + Random(1, 500));
		return;
	}
	if (pDM->FindMultiColor(501, 498, 522, 521, "000000", "6|0|D28B23,5|5|C47A22,-1|7|000000,-9|7|CF9554,-5|15|D08125,1|15|CE7E25",  0.98, 0, &x, &y) > 0) {
		moveTo(x.intVal + Random(1, 30), y.intVal + Random(1, 30), 0); 
		Sleep(500 + Random(1, 500));
		return;
	}
	if (pDM->FindMultiColor(459, 454, 565, 545, "3E5776", "-8|-18|062D6A,32|-13|1948B7,17|17|151923", 0.98, 0, &x, &y) > 0) {
		moveTo(x.intVal + Random(1, 30), y.intVal + Random(1, 30), 0);
		Sleep(500 + Random(1, 500));
		return;
	}
	if (pDM->FindMultiColor(459, 454, 565, 545, "3E5776", "-8|-18|062D6A,32|-13|1948B7,17|17|151923", 0.98, 0, &x, &y) > 0) {
		moveTo(x.intVal + Random(1, 30), y.intVal + Random(1, 30), 0);
		Sleep(500 + Random(1, 500));
		return;
	}


	if (GetObjAndClick(780, 208, 992, 343, "武器架", "FFFF00-101010", 79, 23, -1) >= 0) {
		GetObjAndClick(780, 208, 992, 343, "武器架", "FFFF00-101010", 79, 23);
		Sleep(3000 + Random(1, 500));
		return;
	}

	if (GetObjAndClick(754, 549, 988, 671, "首饰任务确定", "af8950-1D2716", 79, 23, -1) >= 0) {
		GetObjAndClick(754, 549, 988, 671, "首饰任务确定", "af8950-1D2716", 5, -45);
		Sleep(1000 + Random(1, 500));
		GetObjAndClick(761, 532, 1022, 680, "取消", "af8950-1D2716", x1, y1);
		Sleep(1000 + Random(1, 500));

		return;
	}

	if (GetObjAndClick(798, 594, 986, 689, "ESC", "ffffff-303030", x1, y1, -1) >= 0) {
		GetObjAndClick(798, 594, 986, 689, "ESC", "ffffff-303030", x1, y1);
		TabPage1.LogOut(L"跳过剧情");
		pDM->KeyPress(27);
		Sleep(500 + Random(1, 500));
		return;
	}
	if (GetObjAndClick(798, 594, 986, 689, "跳过", "d8d7d7-272828", x1, y1, -1) >= 0) {
		GetObjAndClick(798, 594, 986, 689, "跳过", "d8d7d7-272828", x1, y1);
		Sleep(1000 + Random(1, 500));

		if (GetObjAndClick(542, 509, 700, 590, "继续任务", "917f59-3C353C", x1, y1, -1) >= 0) {
			GetObjAndClick(542, 509, 700, 590, "继续任务", "917f59-3C353C", x1, y1);
			Sleep(1000 *60*8);
			Sleep(1000*20);
		}
		moveTo(521, 511);
		return;
	}


	if (GetObjAndClick(803, 237, 981, 299, "合成金色武器", "d72c2c-280808", 79, 23, -1) >= 0) {
		Sleep(2000 + Random(300, 500));
		pDM->KeyPress(72);
		Sleep(500 + Random(1, 500));
		moveTo(398, 239);
		Sleep(1000 + Random(1, 500));



		if (pDM->FindMultiColor(716, 260, 995, 539, "000F16", "17|1|3D3D2F,4|11|4E4350,18|12|017997", 0.98, 0, &x, &y) > 0) {
			moveTo(x.intVal + Random(1, 10), y.intVal + Random(1, 10), 3);
			Sleep(1000 + Random(1, 500));
			TabPage1.LogOut(L"合成1");
		}

		if (pDM->FindMultiColor(716, 260, 995, 539, "6B6CB2", "17|1|ABADFE,1|15|8185D4,19|15|979AED", 0.98, 0, &x, &y) > 0) {
			moveTo(x.intVal + Random(1, 10), y.intVal + Random(1, 10), 3);
			Sleep(1000 + Random(1, 500));
			TabPage1.LogOut(L"合成2");
		}

		if (pDM->FindMultiColor(716, 260, 995, 539, "000F16", "17|1|3D3D2F,4|11|4E4350,18|12|017997", 0.98, 0, &x, &y) > 0) {
			moveTo(x.intVal + Random(1, 10), y.intVal + Random(1, 10), 3);
			Sleep(1000 + Random(1, 500));
			TabPage1.LogOut(L"合成3");
		}

		if (pDM->FindMultiColor(716, 260, 995, 539, "000F16", "17|1|3D3D2F,4|11|4E4350,18|12|017997", 0.98, 0, &x, &y) > 0) {
			moveTo(x.intVal + Random(1, 10), y.intVal + Random(1, 10), 3);
			Sleep(1000 + Random(1, 500));
			TabPage1.LogOut(L"合成4");
		}
		if (pDM->FindMultiColor(716, 260, 995, 539, "000F16", "17|1|3D3D2F,4|11|4E4350,18|12|017997", 0.98, 0, &x, &y) > 0) {
			moveTo(x.intVal + Random(1, 10), y.intVal + Random(1, 10), 3);
			Sleep(1000 + Random(1, 500));
			TabPage1.LogOut(L"合成5");
		}
		if (pDM->FindMultiColor(716, 260, 995, 539, "000F16", "17|1|3D3D2F,4|11|4E4350,18|12|017997", 0.98, 0, &x, &y) > 0) {
			moveTo(x.intVal + Random(1, 10), y.intVal + Random(1, 10), 3);
			Sleep(1000 + Random(1, 500));
			TabPage1.LogOut(L"合成6");
		}


		moveTo(432, 616);
		Sleep(500 + Random(1, 500));

		moveTo(461, 365);
		Sleep(1500 + Random(1, 500));
		moveTo(671, 212);
		Sleep(500 + Random(1, 500));

		if (pDM->FindMultiColor(716, 260, 995, 539, "086ED2", "16|5|000000,20|15|41E9FF,7|16|000F18", 0.98, 0, &x, &y) > 0) {
			moveTo(x.intVal + Random(1, 10), y.intVal + Random(1, 10), 3);
			Sleep(1000 + Random(1, 500));
			TabPage1.LogOut(L"使用7");
		}
		if (pDM->FindMultiColor(716, 260, 995, 539, "5971b7", "7|2|5374ad,6|3|4b6d9f,2|1|60f3ec,4|2|57b9b2,3|8|051118,8|8|010507,6|1|7af1eb,8|6|030c10,5|3|52759d,2|2|4e847d", 0.9, 0, &x, &y) > 0) {
			moveTo(x.intVal + Random(1, 10), y.intVal + Random(1, 10), 3);
			Sleep(1000 + Random(1, 500));
			TabPage1.LogOut(L"使用8");
		}

		Sleep(500 + Random(1, 500));
		moveTo(992, 210);
		Sleep(500 + Random(1, 500));
		return;
	}


	if (GetObjAndClick(859, 3, 981, 23, "迷雾湿地", "ffffff-101010", 1, 23, -1)>=0)
	{
		if (GetObjAndClick(63, 99, 674, 554, "格维恩", "D7A800-261E01", 1, 23, -1) >= 0)
		{
			GetObjAndClick(63, 99, 674, 554, "格维恩", "D7A800-261E01", 19, 64);
			TabPage1.LogOut(L"格维恩点击");
			return;
		}
	}
	
	if (GetObjAndClick(373, 293, 585, 344, "开始", "917f59-3C353C", 1, 23, -1) >= 0) {

		挂机设置(1);
		Sleep(1000 + Random(300, 500));
		提升战力(1);
		Sleep(1000 + Random(300, 500));
	
		Sleep(1000 + Random(300, 500));
		GetObjAndClick(373, 293, 585, 344, "开始", "917f59-3C353C", x1, y1);
		Sleep(1000 + Random(300, 500));
		GetObjAndClick(203, 431, 336, 500, "进入", "917f59-3C353C", x1, y1);
		Sleep(5000 + Random(300, 500));
		GetObjAndClick(681, 542, 853, 625, "开始关卡", "917f59-3C353C", x1, y1);
		Sleep(5000 + Random(300, 500));
		
		风暴试炼();
		return;
	}








	//新手试炼场 和一个望远镜的剧情任务冲突
	if (GetObjAndClick(801, 267, 972, 444, "刀锋", "ffff00-101010", x1, y1, -1) >= 0)
	{
		TabPage1.LogOut(L"新手试炼场开始挂机");
		pDM->KeyDown(18);
		Sleep(200);
		pDM->KeyPress(65);
		Sleep(200);
		pDM->KeyUp(18);
		Sleep(30000 + Random(1, 500));
		return;
	}

	int times = 0;
	while (pDM->FindMultiColor(348, 18, 596, 95, "7B0000", "3|1|7D0000,0|-2|7C0000", 0.98, 0, &x, &y) == 1) {

		if (GetObjAndClick(370, 27, 447, 87, "系统", "D7D7D7-282828", x1, y1, -1) >= 0)
		{
			break;
		}

		TabPage1.LogOut(L"自动打怪中");
		Sleep(1000 + Random(1, 500));
		if (times >60)
		{
			break;
		}
		times++;
	}
	Sleep(500 + Random(1, 500));


	int a = pDM->FindStrFast(775, 215, 986, 552, "剧情", "d3d300-2C2C01", 0.9, &x, &y);
	if (a == -1)
	{
		TabPage1.LogOut(L"没找到剧情");
		Sleep(1000 + Random(1, 500));
		return;
	}


	x1 = Random(5, 15);
	y1 = Random(2, 9);
	a = pDM->FindStrFast(x.intVal, y.intVal, x.intVal + 217, y.intVal + 115, "任务条", "FFFF00-101010", 0.98, &x, &y);
	if (a == -1)
	{
		TabPage1.LogOut(L"没找到任务条");
		Sleep(1000 + Random(1, 500));
		return;
	}

	str.Format(L"任务条 %d,%d--%d", x.intVal + x1, y.intVal, y.intVal - y1);
	TabPage1.LogOut(str);

	moveTo(x.intVal + x1, y.intVal - y1, 0);
	x1 = Random(16, 30);
	y1 = Random(9, 50);


	Sleep(4000 + Random(1, 2000));

	
	if (GetObjAndClick(106, 131, 285, 179,  "角色", "d8d7d7-272828", 79, 23, -1) >= 0) {
		pDM->KeyPress(67);
		return;
	}

	
	if (pDM->FindStrFast(518, 396, 883, 598, "劳尔", "ffffff-000000", 0.98, &x, &y) >= 0) {

		pDM->KeyPress(79);
		Sleep(1000 + Random(1, 500));
		pDM->KeyPress(79);
		Sleep(1000 + Random(1, 500));
	
		moveTo(704, 136);

		Sleep(1000 + Random(1, 500));
		
		if (pDM->FindStrFast(518, 396, 883, 598, "劳尔", "ffffff-000000", 0.98, &x, &y) >= 0)
		{
			//170 47
			moveTo(x.intVal, y.intVal +3);
			Sleep(1000 + Random(1, 500));
			moveTo(x.intVal + 170, y.intVal +3);
			Sleep(1000 + Random(1, 500));
			moveTo(x.intVal, y.intVal + 3+47);
			Sleep(1000 + Random(1, 500));
			moveTo(x.intVal + 170, y.intVal + 3+47);
			Sleep(1000 + Random(1, 500));
			moveTo(328, 618);
			Sleep(1000 + Random(1, 500));
			
		}
		
		
		pDM->KeyPress(79);
		return;

	}
	
}



void 挂机设置(bool x) {

	VARIANT intX{};
	VARIANT intY{};
	CString str;
	if (GetObjAndClick(180, 463, 571, 612, "取消药品", "D8D7D7-272828", -17, 5, -1) < 0)
		moveTo(861, 43, 0);
	Sleep(1500 + Random(1, 500));
	//取消药品17,5
	if (pDM->FindPic(185, 501, 311, 549, "true.bmp", "000000", 0.7, 0, &intX, &intY) >= 0)
	{
		moveTo(intX.intVal + 6, intY.intVal + 6);
		str.Format(L"取消药品%d:%d", intX.intVal, intX.intVal);
		TabPage1.LogOut(str);
		Sleep(1500 + Random(1, 500));
	}
	//取消紫水晶
	if (pDM->FindPic(183, 539, 326, 594, "true.bmp", "000000", 0.7, 0, &intX, &intY) >= 0)
	{
		moveTo(intX.intVal + 6, intY.intVal + 6);
		str.Format(L"取消紫水晶%d:%d", intX.intVal, intX.intVal);
		TabPage1.LogOut(str);
		Sleep(1500 + Random(1, 500));
	}
	//取消真爱材料
	if (pDM->FindPic(223, 542, 427, 593, "true.bmp", "000000", 0.7, 0, &intX, &intY) >= 0)
	{
		moveTo(intX.intVal + 6, intY.intVal + 6);
		str.Format(L"取消真爱材料%d:%d", intX.intVal, intX.intVal);
		TabPage1.LogOut(str);
		Sleep(1500 + Random(1, 500));
	}
	
	
	//生命值85%
	if (GetObjAndClick(568, 191, 867, 276, "药物", "D8D7D7-272828", 39, 23, -1) >= 0) {
	GetObjAndClick(568, 191, 867, 276, "药物", "D8D7D7-272828", 39, 23);
	Sleep(1000 + Random(1, 500));
	}
	//技能1,2
	if (GetObjAndClick(575, 401, 853, 539, "点击", "917F59-3C353C", 4, 52, -1) >= 0) {
		GetObjAndClick(575, 401, 853, 539, "点击", "917F59-3C353C", -26, 52); 
		Sleep(500 + Random(1, 500));
		moveTo(644, 497, 0);
		Sleep(1000 + Random(1, 500));
	}

	if (GetObjAndClick(575, 401, 853, 539, "点击", "917F59-3C353C", 67, 50, -1) >= 0) {
		GetObjAndClick(575, 401, 853, 539, "点击", "917F59-3C353C", 67, 50);
		Sleep(1000 + Random(1, 500));
		moveTo(759, 494, 0);
		Sleep(1000 + Random(1, 500)); 
	}

	if (GetObjAndClick(575, 401, 853, 539, "点击", "917F59-3C353C", 67, 50, -1) >= 0) {
		GetObjAndClick(575, 401, 853, 539, "点击", "917F59-3C353C", 116, 50);
		Sleep(1000 + Random(1, 500));
		moveTo(753, 502, 0);
		Sleep(1000 + Random(1, 500));
	}

	if (GetObjAndClick(180, 463, 571, 612, "取消药品", "D8D7D7-272828", -17, 5, -1) >= 0)
		pDM->KeyPress(27);



	if (x)
	{
		return;
	}



	Sleep(500 + Random(1, 500));
	//隐藏玩家
	if (GetObjAndClick(391, 203, 640, 521, "系统设置", "AF8950-1D2716", -17, 5, -1) < 0)
		pDM->KeyPress(27);
	GetObjAndClick(391, 203, 640, 521, "系统设置", "AF8950-1D2716", 3, 5);

	Sleep(800 + Random(1, 500));
	if (pDM->FindPic(271, 355, 348, 387, "close.bmp", "000000", 0.8, 0, &intX, &intY) >= 0)
	{
		moveTo(intX.intVal + 6, intY.intVal + 6);
		str.Format(L"隐藏玩家%d:%d", intX.intVal, intX.intVal);
		TabPage1.LogOut(str);
		Sleep(1500 + Random(1, 500));
	}
	else
	{
		TabPage1.LogOut(L"不需要隐藏玩家");
	}
	if (GetObjAndClick(306, 461, 421, 522, "首饰任务确定", "917F59-3C353C", -17, 5, -1) >= 0)
		GetObjAndClick(306, 461, 421, 522, "首饰任务确定", "917F59-3C353C");
	Sleep(500 + Random(1, 500));
	//隐藏各种特效
	if (GetObjAndClick(391, 203, 640, 521, "系统设置", "AF8950-1D2716", -17, 5, -1) < 0)
		pDM->KeyPress(27);
	GetObjAndClick(391, 203, 640, 521, "系统设置", "AF8950-1D2716", 3, 5);
	Sleep(500 + Random(1, 500));

	if (GetObjAndClick(237, 144, 777, 306, "性能优化", "D7D7AC-282820", -17, 5, -1) >= 0)
		GetObjAndClick(237, 144, 777, 306, "性能优化", "D7D7AC-282820");
	Sleep(1000 + Random(1, 500));


	while(pDM->FindPic(276, 239, 713, 401, "close.bmp", "000000", 0.8, 0, &intX, &intY) >= 0)
	{
		moveTo(intX.intVal + 6, intY.intVal + 6);
		str.Format(L"隐藏各种特效%d:%d", intX.intVal, intX.intVal);
		TabPage1.LogOut(str);
		Sleep(500 + Random(1, 500));
	}
	if (GetObjAndClick(306, 461, 421, 522, "首饰任务确定", "917F59-3C353C", -17, 5, -1) >= 0)
		GetObjAndClick(306, 461, 421, 522, "首饰任务确定", "917F59-3C353C");
	Sleep(500 + Random(1, 500));
	return;
}
void 自动加点设置() {
	VARIANT intX{};
	VARIANT intY{};
	CString str;
	moveTo(926, 110, 0);
	Sleep(500);
	pDM->KeyPress(67);
	Sleep(1500 + Random(1, 500));
	TabPage1.LogOut("自动加点设置");

	if (pDM->FindPic(347, 308, 392, 350, "true.bmp", "000000", 0.2, 0, &intX, &intY) < 0)
	{
		moveTo(485, 326, 0);
		Sleep(200 + Random(1, 500));
		moveTo(520, 341, 0);
		Sleep(200 + Random(1, 500));
		moveTo(567, 420, 0);
		Sleep(200 + Random(1, 500));
	}

	pDM->KeyPress(67);
	Sleep(300 + Random(1, 500));
}

void 领取福利() {

	VARIANT x{};
	VARIANT y{};

	if (pDM->FindColor(860,150,869,162, "327320-202020", 1, 0, &x, &y) < 0){
		TabPage1.LogOut("领取福利已完成");
		return;
	}
	

	moveTo(857, 167, 0);
	Sleep(100 + Random(1, 500));//打开
	//领取每日签到
	moveTo(222, 184, 0);
	Sleep(500 + Random(1, 500));
	TabPage1.LogOut("领取福利");
	//签到
	if (pDM->FindMultiColor(363, 192, 914, 495, "FFFF2A-202020", "31|-1|FFF924-202020,63|1|FFFF2A-202020,0|68|FFFF28-202020", 0.95, 0, &x, &y) > 0) {

		TabPage1.LogOut("签到");
		moveTo(x.intVal + 50, y.intVal + 50, 0); //点击第一个包裹项目
		Sleep(200 + Random(1, 500));
	}
	//领取签到奖励
	moveTo(839, 588, 0);
	Sleep(500 + Random(1, 500));
	//领取登录礼包
	moveTo(239, 253, 0);
	Sleep(500 + Random(1, 500));
	while (pDM->FindColor(607, 610, 615, 623, "740106-101010", 0.98, 0, &x, &y) > 0) {
		moveTo(622, 613, 0);
		Sleep(500 + Random(1, 500));
	}
	//关闭
	moveTo(933, 136, 0);
	Sleep(200 + Random(1, 500));
}

void 战斗技巧() {
	VARIANT x{};
	VARIANT y{};
	CString str;
	if (pDM->FindStrFast(793, 363, 918, 528, "战斗技巧", "f9d220-101010", 0.95, &x, &y) <= 0)
	{
		return;
	}
	TabPage1.LogOut("打开战斗技巧");
	moveTo(x.intVal+83, y.intVal+8, 0);
	Sleep(500 + Random(100, 500));
	moveTo(619, 552, 0);
	Sleep(500 + Random(100, 500));
	moveTo(437, 184, 0);
	Sleep(500 + Random(100, 500));
}

void 提升战力(bool open)
{
	VARIANT intX{};
	VARIANT intY{};
	CString str;
	int x1 = Random(1, 15);
	int y1 = Random(3, 8);

	
	int a = pDM->FindStrFast(778, 238, 907, 327, "叉", "B2C8DD-4D3722", 0.9, &intX, &intY);
	str.Format(L"a%d", a);
	TabPage1.LogOut(str);

	if (a >= 0) {
		TabPage1.LogOut(L"关闭任务框");
		moveTo(intX.intVal, intY.intVal);
		Sleep(1000 + Random(1, 500));
	}
	
	Sleep(1000 + Random(1, 500));
	//首先升级技能   86
	while (pDM->FindStrFast(342, 184, 481, 249, "战斗技巧", "D7D7AC-282820", 0.9, &intX, &intY) < 0)
	{
		pDM->KeyPress(86);
		Sleep(1000 + Random(1, 500));
	}
	
	Sleep(1500 + Random(1, 500));

	if (pDM->FindStrFast(342, 184, 481, 249, "猎魔人", "D1CFC8-2E3037", 0.9, &intX, &intY) >= 0)
	{
		while (pDM->FindPic(160, 235, 427, 546, "addskil.bmp", "000000", 0.85, 0, &intX, &intY) >= 0)
		{
			
			moveTo(intX.intVal + Random(1, 5), intY.intVal + Random(1, 5));
			str.Format(L"升级技能%d:%d", intX.intVal, intX.intVal);
			TabPage1.LogOut(str);
			Sleep(500 + Random(1, 500)); 
			if (GetObjAndClick(354, 274, 675, 398, "经验0", "ffffff-101010", -17, 5, -1) >= 0)
			{
				pDM->KeyPress(13);
				Sleep(500 + Random(1, 500));
			}
		}
	}
	else
	{
		while (pDM->FindPic(158, 234, 306, 292, "addskil.bmp", "000000", 0.85, 0, &intX, &intY) >= 0)
		{
				moveTo(intX.intVal + Random(1, 5), intY.intVal + Random(1, 5));
				str.Format(L"升级技能%d:%d", intX.intVal, intX.intVal);
				TabPage1.LogOut(str);
				Sleep(500 + Random(1, 500));
				if (GetObjAndClick(354, 274, 675, 398, "经验0", "ffffff-101010", -17, 5, -1) >= 0)
				{
					pDM->KeyPress(13);
					Sleep(500 + Random(1, 500));
				}
			
		}

		while (pDM->FindPic(166, 286, 422, 530, "addskil.bmp", "000000", 0.85, 0, &intX, &intY) >= 0)
		{
			
				moveTo(intX.intVal + Random(1, 5), intY.intVal + Random(1, 5));
				str.Format(L"升级技能%d:%d", intX.intVal, intX.intVal);
				TabPage1.LogOut(str);
				Sleep(500 + Random(1, 500));
				if (GetObjAndClick(354, 274, 675, 398, "经验0", "ffffff-101010", -17, 5, -1) >= 0)
				{
					pDM->KeyPress(13);
					Sleep(500 + Random(1, 500));
				}
			
		}
	}
	
	// 怀旧服
	/*while (pDM->FindPic(290, 236, 428, 340, "addskil.bmp", "000000", 0.85, 0, &intX, &intY) >= 0)
	{
		moveTo(intX.intVal + Random(1, 5), intY.intVal + Random(1, 5));
		str.Format(L"升级技能%d:%d", intX.intVal, intX.intVal);
		TabPage1.LogOut(str);
		Sleep(500 + Random(1, 500));
		if (GetObjAndClick(344, 275, 701, 430, "首饰任务确定", "AF8950-1D2716", -17, 5, -1) >= 0)
			GetObjAndClick(344, 275, 701, 430, "首饰任务确定", "AF8950-1D2716", x1, y1);
		Sleep(500 + Random(1, 500));

	}

	while (pDM->FindPic(158, 280, 295, 354, "addskil.bmp", "000000", 0.9, 0, &intX, &intY) >= 0)
	{
		moveTo(intX.intVal + Random(1, 5), intY.intVal + Random(1, 5));
		str.Format(L"升级技能%d:%d", intX.intVal, intX.intVal);
		TabPage1.LogOut(str);
		Sleep(500 + Random(1, 500));
		if (GetObjAndClick(344, 275, 701, 430, "首饰任务确定", "AF8950-1D2716", -17, 5, -1) >= 0)
			GetObjAndClick(344, 275, 701, 430, "首饰任务确定", "AF8950-1D2716", x1, y1);
		Sleep(500 + Random(1, 500));
	}
	*/


	if (pDM->FindStrFast(342, 184, 481, 249, "战斗技巧", "D7D7AC-282820", 0.9, &intX, &intY) >= 0)
	{
		moveTo(intX.intVal + Random(1, 5), intY.intVal + Random(1, 5));
		Sleep(1000 + Random(1, 500));

		moveTo(intX.intVal - 129, intY.intVal + 172);
		Sleep(1000 + Random(1, 500));

		if (GetObjAndClick(217, 106, 808, 143, "上限", "f4bf00-606060", 1, 1, -1) >= 0)//(50,110)
		{
			Sleep(7000 + Random(1, 500));
			moveTo(intX.intVal - 50, intY.intVal + 110);
			Sleep(500 + Random(1, 500));
			if (GetObjAndClick(217, 106, 808, 143, "上限", "f4bf00-606060", 1, 1, -1) >= 0)//(75,84)
			{
				Sleep(7000 + Random(1, 500));
				moveTo(intX.intVal - 75, intY.intVal + 84);
				Sleep(500 + Random(1, 500));
			}
		}



		if (GetObjAndClick(696, 382, 821, 564, "一键放入", "d8d7d7-272828", 1, 1, -1) >= 0) {
			GetObjAndClick(696, 382, 821, 564, "一键放入", "d8d7d7-272828", x1, y1);
			Sleep(1000 + Random(1, 500));
			moveTo(intX.intVal + 246, intY.intVal + 340);
			Sleep(500 + Random(1, 500));
			
		}


	}


	pDM->KeyPress(86);



	if (open)
	{
		return;
	}



	//改造+1  2个二级  +2 3个2级或者1个三级    +3 3个三级或者一个4级   +4 3个四级或者一个5级
	Sleep(1000 + Random(1, 500));
	pDM->KeyPress(79);
	Sleep(1000 + Random(1, 500));
	BOOL sign = true;
	for (size_t x = 0; x < 4; x++)
	{
		
		int y1 = 42 * x;
		for (size_t i = 0; i < 6; i++)
		{
			sign = true;
			if (GetObjAndClick(704, 414 + y1, 852, 634 + y1, "+6", "e9d70a-101010", 1, 1, -1) >= 0)
			{
				TabPage1.LogOut(L"+6了已经");
				break;
			}

			moveTo(704 + Random(1, 127), 136 + Random(1, 21) + y1);
			Sleep(1000 + Random(1, 500));
			if (GetObjAndClick(306, 92, 750, 182, "最高", "D3D300-2C2C01", 1, 1, -1) >= 0 || GetObjAndClick(306, 92, 750, 182, "失败", "D3D300-2C2C01", 1, 1, -1) >= 0)
			{
				TabPage1.LogOut(L"改造到最高或者失败了");
				Sleep(7000 + Random(1, 500));
				break;
			}
			int iNumb = 0;
			int iNumb2 = 0;
			int iNumb3 = 0;
			for (size_t z = 0; z < 2 && sign == true; z++)
			{
				
				int y2 = 42 * z;
				_bstr_t Numb;
				if (z==1)
				{
					
					Numb = pDM->Ocr(942, 513, 983, 555, "ffffff-101010", 0.97);
				}
				else
				{
					Numb = pDM->Ocr(925, 462, 968, 494, "ffffff-101010", 0.97);
				}
				
				iNumb = _ttoi(Numb.GetBSTR());
				str.Format(L"石头数量%d", iNumb);
				TabPage1.LogOut(str);
				for (size_t t = 0; t < iNumb; t++)
				{
					moveTo(927 + Random(1, 48), 450 + Random(1, 21) + y2);
					Sleep(1000 + Random(1, 500));

					
					if (z == 1)
					{

						Numb = pDM->Ocr(942, 513, 983, 555, "ffffff-101010", 0.97);
						iNumb3 = _ttoi(Numb.GetBSTR());
						if (iNumb3 >= iNumb)
						{
							TabPage1.LogOut(L"没有没有改造石了");
							sign = false;
							goto go1;
							
						}
					}
					else
					{
						Numb = pDM->Ocr(925, 462, 968, 494, "ffffff-101010", 0.97);
						iNumb3 = _ttoi(Numb.GetBSTR());
						if (iNumb3 >= iNumb)
						{
							TabPage1.LogOut(L"没有没有改造石了");
							sign = false;
							goto go1;
							
						}
					}

					
				}
				iNumb2 += iNumb;
			}
			if (iNumb2>0&&sign == true)
			{
				if (GetObjAndClick(210, 572, 454, 658, "改造", "AF8950-1D2716", -17, 5, -1) >= 0)
				{
					GetObjAndClick(210, 572, 454, 658, "改造", "AF8950-1D2716", 3, 3);
					Sleep(1000 + Random(1, 500));
					TabPage1.LogOut(L"改造");
					Sleep(700 + Random(1, 500));
					if (GetObjAndClick(368, 75, 674, 204, "最高", "D3D300-2C2C01", 1, 1, -1) >= 0 || GetObjAndClick(306, 92, 750, 182, "失败", "D3D300-2C2C01", 1, 1, -1) >= 0)
					{
						TabPage1.LogOut(L"改造到最高或者操作失败了");
						Sleep(7000 + Random(1, 500));
						break;
					}
				}
			}
			if (GetObjAndClick(518, 390, 870, 638, "劳尔", "ffffff-000000", 1, 1, -1) < 0)
			{
				TabPage1.LogOut(L"改造全部用光了");
				break;
			}
		}
		go1:
		sign = true;
	}
	
	for (size_t x = 0; x < 6; x++)
	{
		
		int y1 = 42 * x;
		for (size_t i = 0; i < 6; i++)
		{
			sign = true;
			if (GetObjAndClick(704, 414 + y1, 852, 634 + y1, "+6", "e9d70a-101010", 1, 1, -1) >= 0)
			{
				TabPage1.LogOut(L"+6了已经");
				break;
			}

			moveTo(532 + Random(1, 127), 136 + Random(1, 21) + y1);
			Sleep(1000 + Random(1, 500));

			if (GetObjAndClick(306, 92, 750, 182, "最高", "D3D300-2C2C01", 1, 1, -1) >= 0 || GetObjAndClick(306, 92, 750, 182, "失败", "D3D300-2C2C01", 1, 1, -1) >= 0)
			{
				TabPage1.LogOut(L"改造到最高或者失败了");
				Sleep(7000 + Random(1, 500));
				break;
			}
			int iNumb = 0;
			int iNumb2 = 0;
			int iNumb3 = 0;
			for (size_t z = 0; z < 2 && sign == true; z++)
			{
				int y2 = 42 * z;
				_bstr_t Numb;
				if (z == 1)
				{

					Numb = pDM->Ocr(942, 513, 983, 555, "ffffff-101010", 0.97);
				}
				else
				{
					Numb = pDM->Ocr(925, 462, 968, 494, "ffffff-101010", 0.97);
				}
				iNumb = _ttoi(Numb.GetBSTR());
				str.Format(L"石头数量%d", iNumb);
				TabPage1.LogOut(str);
				for (size_t t = 0; t < iNumb; t++)
				{
					moveTo(927 + Random(1, 48), 450 + Random(1, 21) + y2);
					Sleep(1000 + Random(1, 500));
					if (z == 1)
					{

						Numb = pDM->Ocr(942, 513, 983, 555, "ffffff-101010", 0.97);
						iNumb3 = _ttoi(Numb.GetBSTR());
						if (iNumb3 >= iNumb)
						{
							TabPage1.LogOut(L"没有没有改造石了");
							sign = false;
							goto go;

						}
					}
					else
					{
						Numb = pDM->Ocr(925, 462, 968, 494, "ffffff-101010", 0.97);
						iNumb3 = _ttoi(Numb.GetBSTR());
						if (iNumb3 >= iNumb)
						{
							TabPage1.LogOut(L"没有没有改造石了");
							sign = false;
							goto go;

						}
					}
				}
				iNumb2 += iNumb;
			}
			if (iNumb2 > 0 && sign == true)
			{
				if (GetObjAndClick(210, 572, 454, 658, "改造", "AF8950-1D2716", -17, 5, -1) >= 0)
				{
					GetObjAndClick(210, 572, 454, 658, "改造", "AF8950-1D2716", 3, 3);
					TabPage1.LogOut(L"改造");
					Sleep(1000 + Random(1, 500));
					if (GetObjAndClick(368, 75, 674, 204, "最高", "D3D300-2C2C01", 1, 1, -1) >= 0 || GetObjAndClick(306, 92, 750, 182, "失败", "D3D300-2C2C01", 1, 1, -1) >= 0)
					{
						TabPage1.LogOut(L"改造到最高或者操作失败了");
						Sleep(7000 + Random(1, 500));
						break;
					}
				}
			}
			if (GetObjAndClick(518, 390, 870, 638, "劳尔", "ffffff-000000", 1, 1, -1) < 0)
			{
				TabPage1.LogOut(L"改造全部用光了");
				break;
			}
		}
	go:
		sign = true;

	}
	
	pDM->KeyPress(79);
	Sleep(1000 + Random(1, 500));
}
std::vector<std::string> stringSplit(const std::string& str, char delim) {
	std::size_t previous = 0;
	std::size_t current = str.find(delim);
	std::vector<std::string> elems;
	while (current != std::string::npos) {
		if (current > previous) {
			elems.push_back(str.substr(previous, current - previous));
		}
		previous = current + 1;
		current = str.find(delim, previous);
	}
	if (previous != str.size()) {
		elems.push_back(str.substr(previous));
	}
	return elems;
}

/*
* 
* 21级时去做联盟任务
* 联盟任务完成后
* 出现包裹满的红字
*/

//void 清理背包() {
//	VARIANT x{};
//	VARIANT y{};
//	CString str;
//	//太阳城卖垃圾,野外丢装备  729 279  x+35  
//	int time = 0;
//	while(GetObjAndClick(775, 180, 926, 228, "物品栏", "D8D7D7-272828", 1, 1, -1) < 0)
//	{
//		if (time>4)
//		{
//			return;
//		}
//		pDM->KeyPress(66);
//		Sleep(1000 + Random(1, 500));
//		time++;
//
//	}
//	
//
//	//TabPage1.LogOut("使用新手礼包怀旧版");
//	//if (pDM->FindMultiColor(710, 262, 979, 531, "f5aa49", "14|1|ec8400,-1|10|b800db,18|10|a04a00,9|16|ffef5e", 0.95, 0, &x, &y) >= 0) {
//	//	pDM->MoveTo(x.intVal, y.intVal);
//	//	Sleep(200);
//	//	pDM->RightClick();
//	//	Sleep(1500);
//	//}
//	TabPage1.LogOut("整理");
//	moveTo(946, 599, 0);//整理
//	Sleep(500);
//
//	TabPage1.LogOut("扔装备");
//	//扔装备  五等装备不扔
//	//装备  fcfcfc-404040
//	int baseBagX = 726;
//	int baseBagY = 277;
//	int dist = 39;//两个包裹之间的宽高距离都是39
//
//	int  index = 21;
//	for (int i = 0; i < 21; i++) {
//		int clickX = baseBagX + (i % 7) * 39;
//		int clickY = baseBagY + (i / 7) * 39;
//		pDM->MoveTo(clickX, clickY);
//		Sleep(500 + Random(100, 500));
//
//		if (pDM->FindStrFast(608, 231, 993, 626, "劳尔晶体", "b49c5c-303030", 0.95, &x, &y) >= 0)
//		{
//			str.Format(L"第 %d 格是晶体,开始合成晶体", i + 1);
//			TabPage1.LogOut(str);
//			index = i;
//			break;
//		}
//		//五等装备不扔
//		if (pDM->FindStrFast(495, 311, 785, 661, "装备", "fcfcfc-404040", 0.95, &x, &y) >= 0)
//		{
//			str.Format(L"第 %d 格是装备,删除", i+1);
//			TabPage1.LogOut(str);			
//			moveTo(clickX, clickY, 0);
//			Sleep(200);
//			moveTo(696+ Random(1, 113), 110 + Random(1, 71), 0);//移出
//			Sleep(400 + Random(50, 150));
//			str.Format(L"确认删除");
//			pDM->KeyPress(13);//确认删除
//			Sleep(500);
//		}
//	}
//
//	TabPage1.LogOut("检查第2个包裹是否有数据");
//	//检查第2个包裹是否有数据
//	moveTo(683, 334, 0);//弹出第二包裹项目
//	Sleep(200);
//	if (pDM->FindMultiColor(713, 264, 743, 295, "362e2e", "5|2|0b0a0a,9|8|322b2b,0|9|372f2f", 0.98, 0, &x, &y) > 0) {
//
//		TabPage1.LogOut("第2个包裹无数据");
//		moveTo(684, 279, 0); //点击第一个包裹项目
//		Sleep(500 + Random(1, 500));
//	}
//	//幸运宝箱,目的是将包裹已满提示给顶到上面去
//	TabPage1.LogOut("查找幸运宝箱");
//	do {
//		if (pDM->FindMultiColor(710, 262, 979, 531, "c65ed1", "-10|0|afffff,-8|14|9a7d47,11|19|ffb52e,9|2|6b5bb7,6|4|959e99", 0.98, 0, &x, &y) > 0)
//		{
//			str.Format(L"使用坐标: %d,%d 的幸运宝箱", x.intVal, y.intVal);
//			TabPage1.LogOut(str);
//			pDM->MoveTo(x.intVal, y.intVal);
//			Sleep(200);
//			pDM->RightClick();
//			Sleep(1500);
//		}
//		else
//			break;
//	} while (true);
//	
//	//合石头
//	//int len = 49 - index;
//	
//
//	bool ishecheng = false;
//	for(int j=0;j< laoerarr.size();j++){
//		str.Format(L"开始合成 %d 级的晶体", j+2);
//		TabPage1.LogOut(str);
//		
//		if (GetObjAndClick(362, 141, 497, 211, "合成", "D8D7D7-272828", 1, 1, -1) < 0)
//		{
//			pDM->KeyPress(72);
//			Sleep(500 + Random(1, 500));
//		}
//		Sleep(800);
//
//		string laoercolor = laoerarr[j];//从2级劳尔晶体遍历		
//		auto vect = stringSplit(laoercolor, '_');
//		do
//		{		
//			bool ismatch = false;
//			//if (pDM->FindMultiColor(clickX - 18, clickY - 18, clickX + 18, clickY + 18, vect[0].c_str(), vect[1].c_str(), 0.98, 0, &x, &y) > 0) {
//			int x1 = 0;
//			int y1 = 0;
//
//			if(GetObjAndClick(362, 141, 497, 211, "合成", "D8D7D7-272828", x1, y1, -1) < 0)
//			{
//				pDM->KeyPress(72);
//				Sleep(500 + Random(1, 500));
//			}
//
//
//			if (pDM->FindMultiColor(709, 258, 984, 537, vect[0].c_str(), vect[1].c_str(), 0.98, 0, &x, &y) > 0) {
//				ismatch = true;
//				x1 = x.intVal;
//				y1 = y.intVal;
//			}
//			if (ismatch) {
//				//匹配当前查找的等级晶体,则右击
//				if (pDM->FindMultiColor(500, 311, 542, 357, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) <= 0) {
//					//第4个有晶体合成
//					TabPage1.LogOut("4个晶体合成");
//					moveTo(367 + Random(1, 8), 583 + Random(1, 8), 0);
//					Sleep(500 + Random(300, 500));
//				}
//				else {
//					str.Format(L"添加坐标: %d,%d 的晶体", x1, y1);
//					TabPage1.LogOut(str);
//					//添加到合成面板中
//					pDM->MoveTo(x1 + Random(1, 10), y1 + Random(1, 10));
//					Sleep(200);
//					pDM->RightClick();
//					Sleep(500 + Random(1, 500));
//				}
//			}
//			else {
//				if (pDM->FindMultiColor(432, 309, 478, 357, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) > 0) {
//					//<=2个晶体关闭合成窗口,让晶体归位
//					
//					if (pDM->FindMultiColor(306, 315, 346, 354, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) <= 0) {
//						//如果第一个格子没有添加晶体						
//						TabPage1.LogOut("低于3个晶体合成,放弃");
//						if (GetObjAndClick(362, 141, 497, 211, "合成", "D8D7D7-272828", 1, 1, -1) >= 0)
//						{
//							pDM->KeyPress(72);
//							Sleep(500 + Random(1, 500));
//						}
//						Sleep(800);
//					}					
//					break;
//				}
//				else {
//					TabPage1.LogOut("高于3个晶体合成");
//					//第三个格子有晶体,且无第4个了需合成
//					moveTo(367 + Random(1, 8), 583 + Random(1, 8), 0);
//					Sleep(500 + Random(1, 500));
//
//					if (GetObjAndClick(362, 141, 497, 211, "合成", "D8D7D7-272828", 1, 1, -1) >= 0)
//					{
//						pDM->KeyPress(72);
//						Sleep(500 + Random(1, 500));
//					}
//					
//					Sleep(800);
//					break;
//				}
//			}
//		} while (true);
//
//		Sleep(800);
//	} 
//	TabPage1.LogOut("晶体合成完毕");
//
//	Sleep(1500 + Random(1, 500));
//	
//	if (GetObjAndClick(359, 157, 488, 195, "合成", "D8D7D7-272828", 1, 1, -1) >= 0)
//	{
//		pDM->KeyPress(72);
//		Sleep(500 + Random(1, 500));
//	}
//	if (GetObjAndClick(775, 180, 926, 228, "物品栏", "D8D7D7-272828", 1, 1, -1) >= 0)
//	{
//		pDM->KeyPress(66);
//		Sleep(500 + Random(1, 500));
//	}
//
//}


void 清理背包() {
	VARIANT x{};
	VARIANT y{};
	CString str;
	//太阳城卖垃圾,野外丢装备  729 279  x+35  
	pDM->KeyPress(66);
	Sleep(800);

	//TabPage1.LogOut("使用新手礼包怀旧版");
	//if (pDM->FindMultiColor(710, 262, 979, 531, "f5aa49", "14|1|ec8400,-1|10|b800db,18|10|a04a00,9|16|ffef5e", 0.95, 0, &x, &y) >= 0) {
	//	pDM->MoveTo(x.intVal, y.intVal);
	//	Sleep(200);
	//	pDM->RightClick();
	//	Sleep(1500);
	//}

	int bagBasePX = 0;
	int bagBasePY = 0;
	if (pDM->FindMultiColor(931, 196, 973, 245, "17ABFF", "-3|5|29B5FF,2|5|124B7C,1|4|0A3150,-7|1|459CC9,-9|-3|888580,6|9|605D56,6|9|605D56", 0.96, 0, &x, &y) < 0) {
		TabPage1.LogOut("包裹未打开");
		return;
	}

	bagBasePX = x.intVal;
	bagBasePY = y.intVal;

	TabPage1.LogOut("整理");
	moveTo(bagBasePX + 8, bagBasePY + 395, 0);//整理
	Sleep(500);

	TabPage1.LogOut("扔装备");
	//扔装备  五等装备不扔
	//装备  fcfcfc-404040
	int baseBagX = bagBasePX - 220;// 726;
	int baseBagY = bagBasePY + 72;// 277;
	int dist = 39;//两个包裹之间的宽高距离都是39

	int  index = 21;
	for (int i = 0; i < 21; i++) {
		int clickX = baseBagX + (i % 7) * 39;
		int clickY = baseBagY + (i / 7) * 39;
		pDM->MoveTo(clickX, clickY);
		Sleep(500 + Random(100, 500));

		//if (pDM->FindStrFast(608, 231, 993, 626, "劳尔晶体", "b49c5c-303030", 0.95, &x, &y) >= 0)
		if (pDM->FindStrFast(bagBasePX - 338, bagBasePY + 26, bagBasePX + 47, bagBasePY + 421, "劳尔晶体", "b49c5c-303030", 0.95, &x, &y) >= 0)
		{
			str.Format(L"第 %d 格是晶体,开始合成晶体", i + 1);
			TabPage1.LogOut(str);
			index = i;
			break;
		}
		//五等装备不扔
		//if (pDM->FindStrFast(495, 311, 785, 661, "装备", "fcfcfc-404040", 0.95, &x, &y) >= 0)
		if (pDM->FindStrFast(bagBasePX - 451, bagBasePY + 106, bagBasePX - 161, bagBasePY + 456, "装备", "fcfcfc-404040", 0.95, &x, &y) >= 0)
		{
			str.Format(L"第 %d 格是装备,删除", i + 1);
			TabPage1.LogOut(str);
			moveTo(clickX, clickY, 0);
			Sleep(200);
			moveTo(696 + Random(1, 113), 110 + Random(1, 71), 0);//移出
			Sleep(400 + Random(50, 150));
			str.Format(L"确认删除");
			pDM->KeyPress(13);//确认删除
			Sleep(500);
		}
	}

	TabPage1.LogOut("检查第2个包裹是否有数据");
	//检查第2个包裹是否有数据
	//moveTo(683, 334, 0);//弹出第二包裹项目
	moveTo(bagBasePX - 263, bagBasePY + 129, 0);//弹出第二包裹项目
	Sleep(200);
	if (pDM->FindMultiColor(bagBasePX - 233, bagBasePY + 59, bagBasePX - 203, bagBasePY + 90, "362e2e", "5|2|0b0a0a,9|8|322b2b,0|9|372f2f", 0.98, 0, &x, &y) > 0) {
		//if (pDM->FindMultiColor(713, 264, 743, 295, "362e2e", "5|2|0b0a0a,9|8|322b2b,0|9|372f2f", 0.98, 0, &x, &y) > 0) {

		TabPage1.LogOut("第2个包裹无数据");
		//moveTo(684, 279, 0); //点击第一个包裹项目:-262,74
		moveTo(bagBasePX - 262, bagBasePY + 74, 0); //点击第一个包裹项目:
		Sleep(500 + Random(1, 500));
	}
	//幸运宝箱,目的是将包裹已满提示给顶到上面去
	TabPage1.LogOut("查找幸运宝箱");
	do {
		//if (pDM->FindMultiColor(710, 262, 979, 531, "c65ed1", "-10|0|afffff,-8|14|9a7d47,11|19|ffb52e,9|2|6b5bb7,6|4|959e99", 0.98, 0, &x, &y) > 0)
		if (pDM->FindMultiColor(bagBasePX - 236, bagBasePY + 57, bagBasePX + 33, bagBasePY + 326, "c65ed1", "-10|0|afffff,-8|14|9a7d47,11|19|ffb52e,9|2|6b5bb7,6|4|959e99", 0.98, 0, &x, &y) > 0)
		{
			str.Format(L"使用坐标: %d,%d 的幸运宝箱", x.intVal, y.intVal);
			TabPage1.LogOut(str);
			pDM->MoveTo(x.intVal, y.intVal);
			Sleep(200);
			pDM->RightClick();
			Sleep(1500);
		}
		else
			break;
	} while (true);

	//合石头
	int len = 49 - index;


	bool ishecheng = false;
	for (int j = 0; j < laoerarr.size(); j++) {
		str.Format(L"开始合成 %d 级的晶体", j + 2);
		TabPage1.LogOut(str);

		//if (GetObjAndClick(362, 141, 497, 211, "合成", "D8D7D7-272828", 1, 1, -1) < 0)
		if (pDM->FindStrFast(bagBasePX - 582, bagBasePY - 44, bagBasePX - 415, bagBasePY + 42, "合成", "D8D7D7-272828", 0.9, &x, &y) < 0)
			//if (GetObjAndClick(bagBasePX-582, bagBasePY -44, bagBasePX -415, bagBasePY+42, "合成", "D8D7D7-272828", 1, 1, -1) < 0)
		{
			pDM->KeyPress(72);
			Sleep(800 + Random(1, 500));
		}
		Sleep(800);
		pDM->FindStrFast(bagBasePX - 582, bagBasePY - 44, bagBasePX - 415, bagBasePY + 42, "合成", "D8D7D7-272828", 0.9, &x, &y);
		int hechenngBasePX = x.intVal;
		int hechenngBasePY = y.intVal;


		string laoercolor = laoerarr[j];//从2级劳尔晶体遍历		946,205
		auto vect = stringSplit(laoercolor, '_');
		do
		{
			bool ismatch = false;
			//if (pDM->FindMultiColor(clickX - 18, clickY - 18, clickX + 18, clickY + 18, vect[0].c_str(), vect[1].c_str(), 0.98, 0, &x, &y) > 0) {
			int x1 = 0;
			int y1 = 0;

			//if (GetObjAndClick(362, 141, 497, 211, "合成", "D8D7D7-272828", x1, y1, -1) < 0)
			//{
			//	pDM->KeyPress(72);
			//	Sleep(500 + Random(1, 500));
			//}
			//查找包裹的晶体
			if (pDM->FindMultiColor(bagBasePX - 237, bagBasePY + 53, bagBasePX + 38, bagBasePY + 332, vect[0].c_str(), vect[1].c_str(), 0.98, 0, &x, &y) > 0) {
				//if (pDM->FindMultiColor(709, 258, 984, 537, vect[0].c_str(), vect[1].c_str(), 0.98, 0, &x, &y) > 0) {
				ismatch = true;
				x1 = x.intVal;
				y1 = y.intVal;
			}
			if (ismatch) {
				//匹配当前查找的等级晶体,则右击92,138,134,184
				//if (pDM->FindMultiColor(500, 311, 542, 357, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) <= 0) {
				if (pDM->FindMultiColor(hechenngBasePX + 92, hechenngBasePY + 138, hechenngBasePX + 134, hechenngBasePY + 184, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) <= 0) {
					//第4个有晶体合成
					TabPage1.LogOut("4个晶体合成");
					moveTo(hechenngBasePX - 41 + Random(1, 8), hechenngBasePY + 410 + Random(1, 8), 0);

					Sleep(500 + Random(300, 500));
				}
				else {
					str.Format(L"添加坐标: %d,%d 的晶体", x1, y1);
					TabPage1.LogOut(str);
					//添加到合成面板中
					pDM->MoveTo(x1 + Random(1, 10), y1 + Random(1, 10));
					Sleep(200);
					pDM->RightClick();
					Sleep(500 + Random(1, 500));
				}
			}
			else {
				//if (pDM->FindMultiColor(432, 309, 478, 357, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) > 0) {
				if (pDM->FindMultiColor(hechenngBasePX + 24, hechenngBasePY + 136, hechenngBasePX + 70, hechenngBasePY + 184, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) > 0) {
					//<=2个晶体关闭合成窗口,让晶体归位
					//if (pDM->FindMultiColor(306, 315, 346, 354, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) <= 0) {
					if (pDM->FindMultiColor(hechenngBasePX - 102, hechenngBasePY + 142, hechenngBasePX - 62, hechenngBasePY + 181, "22201F", "6|4|292827,-3|6|3B3A38,-8|14|32312F,-1|11|41403F,6|16|2E2C2A", 0.98, 0, &x, &y) <= 0) {
						//如果第一个格子没有添加晶体						
						TabPage1.LogOut("低于3个晶体合成,放弃");
						pDM->KeyPress(72);
						Sleep(800);
					}
					break;
				}
				else {
					TabPage1.LogOut("高于3个晶体合成");
					//第三个格子有晶体,且无第4个了需合成:-41,410
					moveTo(hechenngBasePX - 41 + Random(1, 8), hechenngBasePY + 410 + Random(1, 8), 0);
					Sleep(500 + Random(1, 500));
					pDM->KeyPress(72);
					Sleep(800);
					break;
				}
			}
		} while (true);

		Sleep(800);
	}
	TabPage1.LogOut("晶体合成完毕");

	Sleep(1500 + Random(1, 500));

	if (GetObjAndClick(bagBasePX - 582, bagBasePY - 44, bagBasePX - 415, bagBasePY + 42, "合成", "D8D7D7-272828", 1, 1, -1) >= 0)
	{
		pDM->KeyPress(72);
		Sleep(500 + Random(1, 500));
	}
	if (pDM->FindMultiColor(931, 196, 973, 245, "17ABFF", "-3|5|29B5FF,2|5|124B7C,1|4|0A3150,-7|1|459CC9,-9|-3|888580,6|9|605D56,6|9|605D56", 0.96, 0, &x, &y) >= 0) {
		pDM->KeyPress(66);
		Sleep(500 + Random(1, 500));
	}

}

//读取中文数据文件
CString readGBKFile(string path, int iline)
{
	string bbb;
	string aaa[30000]; //采用 string 类型，存100行的文本，不要用数组
	int i = 0;
	const char* chr;
	ifstream infile;

	infile.open(path, ios::in);

	while (!infile.eof())            // 若未到文件结束一直循环 
	{
		getline(infile, aaa[i], '\n');//读取一行，以换行符结束，存入 a[] 中
		i++;                    //下一行
	}

	//获取账号密码
	bbb = aaa[iline - 1].c_str();
	chr = bbb.c_str();

	CString Acc(chr);
	return Acc;
}

void 创建角色()
{
	//判断你是不是在选择角色界面  17,175,284,598,宽高(267,423) "创建" "B9BDC5-3A3832"   找到双击

	/*if (pDM->FindStrFast(608, 231, 993, 626, "劳尔晶体", "b49c5c-303030", 0.95, &x, &y) >= 0)
	{
		str.Format(L"第 %d 格是晶体,开始合成晶体", i + 1);
		TabPage1.LogOut(str);
		index = i;
		
	}*/


	//判断是不是在创建界面 433,641,610,695,宽高(177,54) "确认创建" "d8d7d7-272828"  找到开始创建角色   否则下面不执行

	//57,233,204,253,宽高(147,20)  点击枪械师       //63,397,194,415,宽高(131,18) 猎    2枪3猎

	//412,611,598,631,宽高(186,20)  点击名字编辑框

	//476,656,571,677,宽高(95,21)   //点击创建角色

	//判断是否存在  366,295,655,406,宽高(289,111)   "已存在"  "d8d7d7-272828"     识别确定  "首饰任务确定" "af8950-1D2716"

	


}


void 换角色(int Index)
{
	TabPage1.LogOut(L"开始切换角色");
	while (GetObjAndClick(391, 203, 640, 521, "系统设置", "AF8950-1D2716", -17, 5, -1) < 0)
	{
		pDM->KeyPress(27);
		Sleep(500 + Random(1, 500));
	}
	GetObjAndClick(391, 203, 640, 521, "系统设置", "AF8950-1D2716", Random(3, 61) , 62+ Random(3, 8));
	Sleep(1000 + Random(1, 500));
	GetObjAndClick(354, 225, 645, 533, "首饰任务确定", "af8950-1D2716", Random(3, 10),Random(1, 8));
	Sleep(7000 + Random(1, 500));
	//80,126,91,226,宽高(11,100)    (142,20)

	moveTo(80+ Random(0, 142), 126 + Index * 100, 1);

	for (size_t i = 0; i < 15; i++)
	{
		CString str = pDM->Ocr(2, 3, 40, 46, "ffffff-606050", 0.95);
		if (_ttoi(str) > 0)
		{
			break;
		}
		Sleep(1000);
		TabPage1.LogOut(L"等待进入游戏界面");
	} 
	
	TabPage1.LogOut(L"切换角色完成");
	Sleep(1000 + Random(1, 500));
}


int 检测日常是否全部完成()
{
	int x1 = Random(1, 15);
	int y1 = Random(3, 8);
	if (GetObjAndClick(83, 108, 595, 473, "联邦任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) < 0)
		pDM->KeyPress(82);
	Sleep(1000 + Random(1, 500));

	if (GetObjAndClick(83, 108, 595, 473, "R虫族任务", "07D707-082807", x1, y1, -1)>=0)
	{
		if (GetObjAndClick(83, 108, 595, 473, "联邦任务", "07D707-082807", x1, y1, -1) >= 0)
		{
			if (GetObjAndClick(83, 108, 595, 473, "联邦任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0)
				pDM->KeyPress(82);
			return 0;
		}

		return 1;

	}
	if (GetObjAndClick(83, 108, 595, 473, "联邦任务", "D5D65A-2A290C|07D707-082807", x1, y1, -1) >= 0)
		pDM->KeyPress(82);
	return 2;
}


void run() {
	switch_on = 0;
	VARIANT x{};
	VARIANT y{};
	虫族已完成 = false;

	int Index = 1;
	提升战力();
	return;
	
	CString str2 = pDM->Ocr(2, 3, 40, 46, "ffffff-606050", 0.95);
	//TabPage1.LogOut(str);
	
	int a = _ttoi(str2);
	if (a > 0&& a<2)
	{
		自动加点设置();
		挂机设置();
		领取福利();
	}
	
	
	TabPage1.LogOut("运行");
	bool Over = true;
	while (Over)
	{

		while (Stop)
		{
			Sleep(1000);
		}

		


		if (switch_on != 3 && (pDM->FindStrFast(784, 577, 990, 645, "背包", "D75851-282B25", 0.90, &x, &y) >= 0))
		{
			TabPage1.LogOut(L"背包已满");
			switch_on = 3;

		}
		else if (switch_on != 3 && (pDM->FindStrFast(325, 102, 732, 149, "背包不足", "D3D300-2C2C01", 0.90, &x, &y) >= 0))
		{
			TabPage1.LogOut(L"背包已满2");
			switch_on = 3;

		}
		else if (switch_on != 3 && (pDM->FindStrFast(325, 102, 732, 149, "无足够空间", "D3D300-2C2C01", 0.90, &x, &y) >= 0))
		{
			TabPage1.LogOut(L"背包已满3");
			switch_on = 3;

		}
		else if (pDM->FindStrFast(803, 255, 976, 505, "联邦任务|无下划线联邦", "ffff00-101010", 0.98, &x, &y) >= 0)
		{
			
			switch_on = 1;
			
			Sleep(1000 + Random(1, 500));
			
		}
		else if (pDM->FindStrFast(803, 255, 976, 505, "虫族任务", "ffff00-101010", 0.98, &x, &y) >= 0)
		{

			if (虫族已完成==false)
			{
				switch_on = 2;
				
				Sleep(1000 + Random(1, 500));
			}
			else
			{
				switch_on = 1;
				TabPage1.LogOut(L"虫族已完成执行联邦任务");
				Sleep(1000 + Random(1, 500));
			}
			
		}
		else if (pDM->FindStrFast(803, 255, 976, 505, "提升", "FF3333-101010", 0.9, &x, &y) >= 0)
		{
			if (pDM->FindStrFast(803, 255, 976, 505, "级", "FF3333-101010", 0.9, &x, &y) >= 0)
			{
				switch_on = 检测日常是否全部完成();
				if (switch_on==0)
				{
					CString str = pDM->Ocr(2, 3, 40, 46, "ffffff-606050", 0.95);
					TabPage1.LogOut(str);
					if (str.Find(L"42") >= 0)
					{
						换角色(Index);
						Index++;
						自动加点设置();
						挂机设置();
						领取福利();
						continue;
					}
				}

				CString str2;
				str2.Format(L"提升等级执行%d", switch_on);
				TabPage1.LogOut(str2);
				
			}
			
		}
		else 
		{
		
			switch_on = 0;
		}


		switch (switch_on)
		{
		case 0:
			主线();
			break;
		case 1:
			TabPage1.LogOut(L"执行联邦任务");
			联邦任务();
			break;
		case 2:
			TabPage1.LogOut(L"执行虫族任务");
			虫族任务();
			break;
		case 3:
			//开始挂机
			while (pDM->FindStrFast(414, 94, 619, 223, "开启", "AFAF01-505001", 0.9, &x, &y) < 0)
			{
				pDM->KeyDown(18);
				Sleep(200);
				pDM->KeyPress(65);
				Sleep(200);
				pDM->KeyUp(18);
				Sleep(1000);
			}
			
			提升战力();
			while (pDM->FindStrFast(414, 94, 619, 223, "开启", "AFAF01-505001", 0.9, &x, &y) < 0)
			{
				pDM->KeyDown(18);
				Sleep(200);
				pDM->KeyPress(65);
				Sleep(200);
				pDM->KeyUp(18);
				Sleep(1000);
			}
			//战斗技巧();
			清理背包();
			break;

		}
	}


}



void CGreenLegendDlg::OnBnClickedBtnEnd()
{
	Stop = false;
	

	

	


	


	return;
	// TODO: 在此添加控件通知处理程序代码

	pDM->UnBindWindow();
	StartScript.SetCheck(BST_UNCHECKED);

	if (hRunScript != NULL)
	{
		TerminateThread(hRunScript,1);
		CloseHandle(hRunScript);
		hRunScript = NULL;
		bAutoFlag =  false;
		//EndFlag = false;
	}

	if (hTime != NULL)
	{
		TerminateThread(hTime, 1);
		CloseHandle(hTime);
		hTime = NULL;
	}

	if (hShowInfor!=NULL)
	{
		SuspendThread(hShowInfor);
	}

	if (hFitUser != NULL)
	{
		TerminateThread(hFitUser, 1);
		hFitUser = NULL;
		bFitFlag = false;
	}

	if (hCheckProtectThread != NULL)
	{
		TerminateThread(hCheckProtectThread, 1);
		CloseHandle(hCheckProtectThread);
		hCheckProtectThread = NULL;
		CheckHp = false;
	}
	
}


void CGreenLegendDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	pDM->UnBindWindow();

	SP_Cloud_Offline(0);
	CDialogEx::OnClose();

}


void CGreenLegendDlg::OnCbnSelchangeCmbConfig()
{
	// TODO: 在此添加控件通知处理程序代码


	
	ReadGameConfig();

}


void CGreenLegendDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
