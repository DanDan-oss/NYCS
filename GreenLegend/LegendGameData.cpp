#include "pch.h"
#include "GreenLegendDlg.h"
typedef IPHLPAPI_DLL_LINKAGE
ULONG
WINAPI
pGetAdaptersInfo(
	_Out_writes_bytes_opt_(*SizePointer) PIP_ADAPTER_INFO AdapterInfo,
	_Inout_                         PULONG           SizePointer
);
pGetAdaptersInfo* pMyGetAdaptersInfo = NULL;																//多开
char shell[3]{ 194,8,0 };																					//多开



extern CTabPage1 TabPage1;																					//日志功能
extern CTabPage2 TabPage2;																					//基本功能
extern CTabPage3 TabPage3;																					//战斗功能
extern TabPAGE4 TabPage4;																					//进程功能
extern TabPAGE5 TabPage5;																					//账号功能
extern CPlayTabPage1 PlayTabPage1;																			//玩家信息
extern CPlayTabPage2 PlayTabPage2;																			//背包信息

extern CGreenLegendDlg* Main;																				//主窗口

extern HWND hGameWindow ;																				//游戏窗口句柄
HWND hGameWindow2;																						//游戏窗口多开句柄
extern HANDLE hMainProcess;																				//游戏进程句柄
extern HWND hClientWindow;																				//客户端窗口句柄
extern HANDLE hShowInfor;																			//显示个人信息线程句柄
extern char WindowTitlebuffer[256];																		//游戏窗口标题缓冲区
extern char WindowClassbuffer[256];																		//游戏窗口类名缓冲区
extern User GameUser;																					//玩家自己结构体
extern OtherUser GameOtherObj[0x200];																	//周围对象结构体
OtherUser GameOtherMaster[0x200];																	//周围对象结构体
extern Knapsack MyKnapsack[GameGoodsMaxNumber];															//自己背包结构体
extern Idmsoft* pDM;																					//大漠类

extern bool Rand;																						//随机石开关
extern bool Screenshot ;																				//截图开关
extern int NoEquipCount;																				//回收错误次数
extern bool WaitBackFlag ;																				//回城等待开关

bool BlackRandFlag;																						//黑名单随机开关

extern wchar_t wMap[0x20][0x100] ;

wchar_t wTime[0x20][0x20]{};																			//定时时间

extern wchar_t wOn_lineMap[0x20][0x20];																	//没到时间要挂机的地图

//extern wchar_t wLifeMap[0x20][0x20];																	//日常任务地图

extern int index[0x40];																					//随机地图顺序索引
extern bool isNow;																						//有到时间的任务标志
extern bool Initflag ;																				//初始化标志

extern HANDLE hRunScript;																			//脚本运行线程
extern HANDLE hTime;																				//计时线程句柄
extern HANDLE hCheckProtectThread ;																	//保护线程句柄

extern bool BlackCd;																				//黑名单CD

extern bool EndFlag ;																				//游戏全局线程标志
extern DWORD dMinute;																				//分钟计时
extern bool CheckHp;																				//检测血开关
extern bool bAutoFlag ;																				//挂机线程开关

int UserNumb = 0;
int MasterNumb = 0;
bool NoExpSwatch = true;																			//多少分钟没经验检测开关
DWORD NoExpTime = 0;																				//多少分钟计时
long long ExpFlag = 0;																				//经验标志  用来比较经验是否变化
long long ExpFlag2 = 0;																				//经验标志  用来比较经验是否变化

extern void* WayfindingAdress;																		//走路call内存地址
extern void* UseGoodsAdreress;																		//使用物品call内存地址
extern void* ChoseGoodsDisintegrateAdreress;														//选择装备分解call内存地址
extern void* ChoseGoodsSaveAdreress;																//选择物品存仓库call内存地址
extern void* KeyAdreress;																			//按键call内存地址
extern void* SkillAdreress;																			//技能call内存地址


wchar_t BlackListName[0x200][0x20]{};


																		//大漠远程call字符串转换缓冲区
//wchar_t InMap[40]{};

bool ChangeMap = true;																				//是否更改挂机地图标志
bool WordLibSet = true;																				//字库设置标志
DWORD SaveGoodsId[GameGoodsMaxNumber]{};															//保存需要存处的物品id

int ObjList = 0;																					//对象链表
int KnapsackList = 0;																				//背包链表
int UseGoodsEcx = 0;																				//使用物品的ecx
//DWORD Objbase = 0;																				//对象基址

DWORD ScreenNumb = 0;																				//截图数量

SYSTEMTIME sysTime{};
bool MapBug = false;																				//心心相印地图bug

extern bool EliminateDemonOver;																		//除魔全部完成标志
bool EliminateDemonFlag = false;																	//除魔任务完成标志
bool NowEliminateDemonFlag = false;																	//除魔任务正在进行标志
extern bool EliminateDemonContinueFlag ;															//除魔任务没有完成继续标志

extern bool StartDartsOver;																			//押镖完成标志
extern bool StartDartsContinueFlag;																	//押镖任务没有完成继续标志
DWORD ChoseGoodsDisintegrateEcx = 0;																//分解NPCecx


extern int PrivateMapOver;																			//个人副本完成标志
DWORD ObjID[100]{};																					//存放已经读取过的怪物ID
DWORD IdIndex = 0;																					//怪物ID数组索引

extern bool bFitFlag;

//仓库ecx
DWORD SaveGoodsoffset1 = 0;
DWORD SaveGoodsoffset2 = 0;



DWORD flag = 0;																						//对象链表标志
SIZE_T Size = 0;																					//函数实际读取大小
DWORD Obj = 0;																						//对象地址
DWORD ObjLeft = 0;																					//对象链表左
DWORD ObjRight = 0;																					//对象链表右

DWORD Myflag = 0;																						//对象链表标志
DWORD MyObj = 0;																						//对象地址
DWORD MyObjLeft = 0;																					//对象链表左
DWORD MyObjRight = 0;																					//对象链表右



//DWORD Count = 0;																					

extern CRITICAL_SECTION cs;
extern CRITICAL_SECTION cs2;
extern CRITICAL_SECTION cs3;

int RandFunc(int count,bool Switch)
{
	srand((unsigned int)time(NULL));
	memset(index, 0, sizeof(index));
	for (int i = 0; i < count; i++) 
	{
		index[i] = i ;
	}
	int T = 1000;
	int i = 0, j = 0, tmp = 0;
	if (Switch)
	{
		while (T--)
		{
			i = rand() % count;
			j = rand() % count;
			tmp = index[i];
			index[i] = index[j];
			index[j] = tmp;
		}
	}
	
	return 0;
}

int findCount(wchar_t* str1, wchar_t const str2)
{

	int count = 0;
	wchar_t* ptmp = str1;

	while (*ptmp)
	{
		if (*ptmp == str2)
		{
			count++;
		}
		ptmp ++;
	}

	return count;

}

DWORD GetOn_lineMap()
{
	//DWORD nNumb = TabPage4.m_LFlowPath.GetItemCount();
	SIZE_T i = 0;
	wchar_t TmpTime[0x20]{};
	//wchar_t TmpType[0x10]{};
	wchar_t TmpMap[0x80]{};
	memset(wMap, 0, sizeof(wMap));
	memset(wTime, 0, sizeof(wTime));

	for ( ; i < 16; i++)
	{

		memset(TmpMap, 0, sizeof(TmpMap));
		memset(TmpTime, 0, sizeof(TmpTime));
		LV_ITEM lvitem = { 0 };



		//地图
		lvitem.iItem = i;
		lvitem.iSubItem = 2;
		lvitem.mask = LVIF_TEXT;
		lvitem.pszText = TmpMap;
		lvitem.cchTextMax = sizeof(TmpMap);
		TabPage4.m_LFlowPath.GetItem(&lvitem);
		if (TmpMap[0] == 0)
		{
			break;
		}

		StrCatW(wMap[i], TmpMap);

		


		//时间
		lvitem.iItem = i;
		lvitem.iSubItem = 1;
		lvitem.mask = LVIF_TEXT;
		lvitem.pszText = TmpTime;
		lvitem.cchTextMax = sizeof(TmpTime);
		TabPage4.m_LFlowPath.GetItem(&lvitem);
		if (TmpTime[0] == 0)
		{
			break;
		}
		StrCatW(wTime[i], TmpTime);

		
	}

	return i;

}

struct MapTime
{
	int time;
	int Index;
};

void BubbleSort(MapTime a[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			MapTime temp{};
			if (a[j].time > a[j + 1].time)
			{
				temp.time = a[j].time;
				a[j].time = a[j + 1].time;
				a[j + 1].time = temp.time;

				temp.Index = a[j].Index;
				a[j].Index = a[j + 1].Index;
				a[j + 1].Index = temp.Index;

			}
		}
	}
}

int GetNowGotoMap(int iCount)
{
	SYSTEMTIME sTime{};
	MapTime Maps[0x20]{};
	GetLocalTime(&sTime);
	int iTime = 0;
	int itmpTime{};
	int itmpTime2{};
	wchar_t buffer[0x8]{};
	CString Map;
	int iNowlen = 0;
	size_t Nowx = 0;
	
	int ilen = 0;
	size_t x = 0;
	int t = -1;
	//memset(wNowOn_lineMap, 0, sizeof(wNowOn_lineMap));
	memset(wOn_lineMap, 0, sizeof(wOn_lineMap));
	//memset(wLifeMap, 0, sizeof(wLifeMap));
	isNow = false;
	for (size_t i = 0; i < iCount; i++)
	{
		wcsncpy(buffer, wTime[i],2);
		iTime = _ttoi(buffer);

		if (sTime.wHour == iTime)
		{
			wcsncpy(buffer, (&wTime[i][0])+3,2);
			iTime = _ttoi(buffer);
			if (sTime.wMinute >= iTime)
			{
				Map = wMap[i];

				if (Map.Find(L"除魔") >= 0)
				{
					if (EliminateDemonOver == false)
					{
						continue;
					}
					
				}

				if (Map.Find(L"押镖") >= 0)
				{
					if (StartDartsOver == false)
					{
						continue;
					}
				}

				if (Map.Find(L"个人副本") >= 0)
				{
					if (PrivateMapOver == 0)
					{
						continue;
					}
				}
				int length = findCount(wMap[i], '|' ) + 1;
				
				for (size_t y = 0; y < length; y++)
				{
					if (length==1)
					{
						wcscpy(wOn_lineMap[Nowx +y], Map.GetBuffer());
					}
					else
					{
						if (Map.Find(L"\r\n")>=0)
						{
							//*(Map.GetBuffer() + Map.Find(L"\r\n")) = L'|';
							wcscpy(Map.GetBuffer() + Map.Find(L"\r\n"), (Map.GetBuffer() + Map.Find(L"\n")) + 1);
						}
						
						if (Map.Find(L"|")>=0)
						{
							wcsncpy(wOn_lineMap[Nowx +y], Map.GetBuffer() + iNowlen, Map.Find(L"|") - iNowlen);
							iNowlen = Map.Find(L"|");
						}
						else
						{
							wcsncpy(wOn_lineMap[Nowx +y], Map.GetBuffer() + iNowlen, wcslen(Map.GetBuffer()) + 1 - iNowlen);
						}

						if (Map.Find(L"|") >= 0)
						{
							*(Map.GetBuffer() + Map.Find(L"|")) = L',';
							iNowlen++;
						}
					}
				
				}
				iNowlen = 0;
				Nowx += length;
				
				isNow=true;
			}
			
		}
		
	}
	if (isNow == false)
	{
		for (size_t i = 0; i < iCount; i++)
		{
			wcsncpy(buffer, wTime[i], 2);
			Maps[i].time = _ttoi(buffer);
			Maps[i].Index = i;
			
			if (Maps[i].time >= itmpTime && Maps[i].time <sTime.wHour)
			{
				wcsncpy(buffer, (&wTime[i][0]) + 3, 2);
				iTime = _ttoi(buffer);
				if (iTime>=itmpTime2)
				{
					Map = wMap[i];
					if (Map.Find(L"除魔") >= 0)
					{
						continue;
					}

					if (Map.Find(L"押镖") >= 0)
					{
						continue;
					}

					if (Map.Find(L"个人副本") >= 0)
					{
						continue;
					}
					itmpTime = Maps[i].time;
					itmpTime2 = iTime;
					t = Maps[i].Index;
				}
				
			}
		}

		if (t<0)
		{
			for (size_t i = 0; i < iCount; i++)
			{
				wcsncpy(buffer, wTime[i], 2);
				Maps[i].time = _ttoi(buffer);
				Maps[i].Index = i;

				if (Maps[i].time >= itmpTime )
				{
					wcsncpy(buffer, (&wTime[i][0]) + 3, 2);
					iTime = _ttoi(buffer);
					if (iTime >= itmpTime2)
					{
						Map = wMap[i];
						if (Map.Find(L"除魔") >= 0)
						{
							continue;
						}

						if (Map.Find(L"押镖") >= 0)
						{
							continue;
						}

						if (Map.Find(L"个人副本") >= 0)
						{
							continue;
						}
						itmpTime = Maps[i].time;
						itmpTime2 = iTime;
						t = Maps[i].Index;
					}

				}
			}
		}

		if (t < 0)
		{
			TabPage1.LogOut(L"流程表出错,检测流程表");
			return 0;
		}

		int length = findCount(wMap[t], '|') + 1;
		Map = wMap[t];
		for (size_t y = 0; y < length; y++)
		{
			if (length == 1)
			{
				wcscpy(wOn_lineMap[y], Map.GetBuffer());
			}
			else
			{
				if (Map.Find(L"\r\n") >= 0)
				{
					//*(Map.GetBuffer() + Map.Find(L"\r\n")) = L'|';
					wcscpy(Map.GetBuffer() + Map.Find(L"\r\n"), (Map.GetBuffer() + Map.Find(L"\n")) + 1);
				}

				if (Map.Find(L"|") >= 0)
				{
					wcsncpy(wOn_lineMap[y], Map.GetBuffer() + ilen, Map.Find(L"|") - ilen);
					ilen = Map.Find(L"|");
				}
				else
				{
					wcsncpy(wOn_lineMap[y], Map.GetBuffer() + ilen, wcslen(Map.GetBuffer()) + 1 - ilen);
				}

				if (Map.Find(L"|") >= 0)
				{
					*(Map.GetBuffer() + Map.Find(L"|")) = L',';
					ilen++;
				}
			}

		}
		x = length;
	}
		
	
	return isNow? Nowx:x;
}

DWORD WINAPI AutoLoginGame(_In_ LPVOID lpParameter);

DWORD TraversalMyUser(DWORD root, HANDLE hProcess)
{
	if (root==0|| hProcess==NULL)
	{
		TabPage1.LogOut(L"TraversalMyUser参数出错");
		return 0;
	}
	int ret = 0;
	CStringW str;
	DWORD MeFlag2 = 0;
	DWORD MeFlag3 = 0;
	DWORD NameLen = 0;																					//名字长度
	DWORD* NameAddress = NULL;																			//名字长度超过8的地址
	DWORD MeFlag = 0;																					//自己的标识符
	Myflag = 0;
	if (ReadProcessMemory(hProcess, PDWORD(root + GameObjFlagOffset), &Myflag, 1, &Size) != 0)
	{

		if (Myflag == GameObjFlag)
		{
			MyObj = 0;
			if (ReadProcessMemory(hProcess, PDWORD(root + GameObjOffset), &MyObj, 4, &Size) != 0)
			{
				
				if (ReadProcessMemory(hProcess, PDWORD(MyObj + GameUserNameLenOffset), &NameLen, 4, &Size) != 0)
				{

					if (NameLen >= 8)
					{
						NameAddress = 0;
						if (ReadProcessMemory(hProcess, PDWORD(MyObj + GameUserNameOffset), &NameAddress, 4, &Size) != 0)
						{

							if (ReadProcessMemory(hProcess, PDWORD(MyObj + GameMeFlagOffset), &MeFlag, 4, &Size) != 0)
							{
								MeFlag2 = 0;
								if (ReadProcessMemory(hProcess, PDWORD(GameBase + GameUserMaxHpBaseOffset), &MeFlag2, 4, &Size) != 0)
								{
									MeFlag3 = 0;
									if (ReadProcessMemory(hProcess, PDWORD(MyObj + GameUserMaxHpOffSet), &MeFlag3, 4, &Size) != 0)
									{
										if (MeFlag2 == MeFlag3 && MeFlag == GameMeFlag )
										{

											if (ReadProcessMemory(hProcess, (PVOID)(MyObj + GameObjInformationOffset), &GameUser.Infor, 4, &Size) != 0)
											{
												if (ReadProcessMemory(hProcess, (PVOID)(MyObj + GameUserOccupationFlagOffset), &GameUser.Occupation, 4, &Size) != 0)
												{
													if (ReadProcessMemory(hProcess, NameAddress, GameUser.Name, sizeof(GameUser.Name), &Size) != 0)
													{
														//str.Format(L"自己名字:%s", GameUser.Name);
														//TabPage1.LogOut(str);

														GameUser.ObjAddress = MyObj;

														ret = 1;
														return 1;

													}
												}
											}

										}
										
									}


								}


							}






						}
					}
					else
					{

						MeFlag = 0;
						if (ReadProcessMemory(hProcess, PDWORD(MyObj + GameMeFlagOffset), &MeFlag, 4, &Size) != 0)
						{
							MeFlag2 = 0;
							if (ReadProcessMemory(hProcess, PDWORD(GameBase + GameUserMaxHpBaseOffset), &MeFlag2, 4, &Size) != 0)
							{
								MeFlag3 = 0;
								if (ReadProcessMemory(hProcess, PDWORD(MyObj + GameUserMaxHpOffSet), &MeFlag3, 4, &Size) != 0)
								{
									if (MeFlag2 == MeFlag3 && MeFlag == GameMeFlag)
									{

										if (ReadProcessMemory(hProcess, (PVOID)(MyObj + GameObjInformationOffset), &GameUser.Infor, 4, &Size) != 0)
										{
											if (ReadProcessMemory(hProcess, (PVOID)(MyObj + GameUserOccupationFlagOffset), &GameUser.Occupation, 4, &Size) != 0)
											{
												if (ReadProcessMemory(hProcess, PDWORD(MyObj + GameUserNameOffset), GameUser.Name, sizeof(GameUser.Name), &Size) != 0)
												{
													//str.Format(L"自己名字:%s", GameUser.Name);
													//TabPage1.LogOut(str);

													GameUser.ObjAddress = MyObj;

													ret = 1;
													return 1;
												}
											}
										}


									}
									
								}

							}
						}

					}

				}


			}

			MyObjLeft = 0;
			if (ReadProcessMemory(hProcess, PDWORD(root), &MyObjLeft, 4, &Size) != 0)
			{
				if (MyObjLeft !=NULL)
				{
					TraversalMyUser(MyObjLeft, hProcess);
				}
				
			}
			MyObjRight = 0;

			if (ReadProcessMemory(hProcess, PDWORD(root + 0x8), &MyObjRight, 4, &Size) != 0)
			{
				if (MyObjRight!=NULL)
				{
					TraversalMyUser(MyObjRight, hProcess);
				}
				
			}
		}

	}

	return ret;
}



DWORD TraversalUser(DWORD root, HANDLE hProcess, OtherUser* GameOtherObj , int* UserNumb,bool sign=false)
{
	//Sleep(200);
	if (root == 0 || hProcess == NULL)
	{
		TabPage1.LogOut(L"TraversalUser参数出错");
		return 0;
	}
	int ret = 0;
	CStringW str;
	DWORD MeFlag2 = 0;
	DWORD MeFlag3 = 0;
	DWORD NameLen = 0;																					//名字长度
	flag = 0;
	DWORD* NameAddress = NULL;																			//名字长度超过8的地址
	DWORD MeFlag = 0;																					//自己的标识符
	if (ReadProcessMemory(hProcess, PDWORD(root + GameObjFlagOffset), &flag, 1, &Size) != 0)
	{

		if (flag == GameObjFlag)
		{
			Obj = 0;
			if (ReadProcessMemory(hProcess, PDWORD(root + GameObjOffset), &Obj, 4, &Size) != 0)
			{
				
				if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserNameLenOffset), &NameLen, 4, &Size) != 0)
				{

					if (NameLen >= 8)
					{
						NameAddress = 0;
						if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserNameOffset), &NameAddress, 4, &Size) != 0)
						{

							MeFlag = 0;
							if (ReadProcessMemory(hProcess, PDWORD(Obj + GameMeFlagOffset), &MeFlag, 4, &Size) != 0)
							{
								MeFlag2 = 0;
								if (ReadProcessMemory(hProcess, PDWORD(GameBase + GameUserMaxHpBaseOffset), &MeFlag2, 4, &Size) != 0)
								{
									MeFlag3 = 0;
									if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserMaxHpOffSet), &MeFlag3, 4, &Size) != 0)
									{
										if (MeFlag2 == MeFlag3 && MeFlag == GameMeFlag&& sign==false)
										{


										}
										else
										{
											if (GameOtherObj == NULL)
											{

											}
											else
											{
												if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserFlagOffset), &GameOtherObj[*UserNumb].UseFlag, 4, &Size) != 0)
												{
													if (ReadProcessMemory(hProcess, PDWORD(Obj + GameRetinueFlagOffset), &GameOtherObj[*UserNumb].RetinueFlag, 4, &Size) != 0)
													{
														if (ReadProcessMemory(hProcess, PDWORD(Obj + GameOBJIDFlagOffset), &GameOtherObj[*UserNumb].ObjID, 4, &Size) != 0)
														{
															if (ReadProcessMemory(hProcess, NameAddress, &GameOtherObj[*UserNumb].Name, sizeof(GameOtherObj[*UserNumb].Name), &Size) != 0)
															{
																if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserXOffset), &GameOtherObj[*UserNumb].x, sizeof(GameOtherObj[*UserNumb].x), &Size) != 0)
																{
																	if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserYOffset), &GameOtherObj[*UserNumb].y, sizeof(GameOtherObj[*UserNumb].y), &Size) != 0)
																	{
																		GameOtherObj[*UserNumb].ObjAddress = Obj;
																		//str.Format(L"地址%08X其他名字%s", Obj + GameUserNameOffset,GameOtherObj[UserNumb].Name);

																		//TabPage1.LogOut(str);
																		
																		(*UserNumb)++;
																		

																	}
																}
															}
														}
													}
												}
											}


										}
									}


								}


							}






						}
					}
					else
					{

						if (ReadProcessMemory(hProcess, PDWORD(Obj + GameMeFlagOffset), &MeFlag, 4, &Size) != 0)
						{
							MeFlag2 = 0;
							if (ReadProcessMemory(hProcess, PDWORD(GameBase + GameUserMaxHpBaseOffset), &MeFlag2, 4, &Size) != 0)
							{
								MeFlag3 = 0;
								if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserMaxHpOffSet), &MeFlag3, 4, &Size) != 0)
								{
									if (MeFlag2 == MeFlag3 && MeFlag == GameMeFlag && sign == false)
									{


									}
									else
									{
										if (GameOtherObj == NULL)
										{

										}
										else
										{
											if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserFlagOffset), &GameOtherObj[*UserNumb].UseFlag, 4, &Size) != 0)
											{
												if (ReadProcessMemory(hProcess, PDWORD(Obj + GameRetinueFlagOffset), &GameOtherObj[*UserNumb].RetinueFlag, 4, &Size) != 0)
												{
													if (ReadProcessMemory(hProcess, PDWORD(Obj + GameOBJIDFlagOffset), &GameOtherObj[*UserNumb].ObjID, 4, &Size) != 0)
													{
														if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserNameOffset), &GameOtherObj[*UserNumb].Name, sizeof(GameOtherObj[*UserNumb].Name), &Size) != 0)
														{
															if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserXOffset), &GameOtherObj[*UserNumb].x, sizeof(GameOtherObj[*UserNumb].x), &Size) != 0)
															{
																if (ReadProcessMemory(hProcess, PDWORD(Obj + GameUserYOffset), &GameOtherObj[*UserNumb].y, sizeof(GameOtherObj[*UserNumb].y), &Size) != 0)
																{
																	GameOtherObj[*UserNumb].ObjAddress = Obj;
																	//str.Format(L"地址%08X其他名字%s", Obj + GameUserNameOffset,GameOtherObj[UserNumb].Name);

																	//TabPage1.LogOut(str);
																	
																	(*UserNumb)++;

																}
															}
														}
													}
												}
											}
										}

									}
								}

							}
						}

					}

				}


			}
			
			ObjLeft = 0;
			if (ReadProcessMemory(hProcess, PDWORD(root), &ObjLeft, 4, &Size) != 0)
			{
				if (ObjLeft!=0)
				{
					TraversalUser(ObjLeft, hProcess, GameOtherObj, UserNumb, sign);
				}
				
			}
			ObjRight = 0;

			if (ReadProcessMemory(hProcess, PDWORD(root + 0x8), &ObjRight, 4, &Size) != 0)
			{

				if (ObjRight!=0)
				{
					TraversalUser(ObjRight, hProcess, GameOtherObj, UserNumb, sign);
				}
				
			}
		}
		
	}
							
	return (*UserNumb);
}

DWORD TraversalKnapsack(DWORD root, HANDLE hProcess, Knapsack* pKnapsack)
{
	
	
	DWORD ret = 0;

	if (root == 0 || hProcess == NULL || pKnapsack == NULL)
	{
		TabPage1.LogOut(L"TraversalKnapsack参数出错");
		return 0;
	}
	EnterCriticalSection(&cs2);
	memset(MyKnapsack, 0, sizeof(MyKnapsack));
	for (SIZE_T i = 0; i < GameGoodsMaxNumber; i++)
	{

		if (ReadProcessMemory(hProcess, PDWORD(root + 4 * i), &pKnapsack[i].Address, sizeof(pKnapsack[i].Address), &Size) != 0)
		{
			if (pKnapsack[i].Address != 0)
			{
				if (ReadProcessMemory(hProcess, PDWORD(pKnapsack[i].Address + GameGoodsNameOffset), &pKnapsack[i].Name, sizeof(pKnapsack[i].Name), &Size) == 0)
				{
					TabPage1.LogOut(L"读取物品名称失败");
					
					LeaveCriticalSection(&cs2);
					return ret;
				}
				if (ReadProcessMemory(hProcess, PDWORD(pKnapsack[i].Address + GameGoodsNumbOffset), &pKnapsack[i].Numb, sizeof(pKnapsack[i].Numb), &Size) == 0)
				{
					TabPage1.LogOut(L"读取物品数量失败");
					LeaveCriticalSection(&cs2);
					return ret;
				}
				if (ReadProcessMemory(hProcess, PDWORD(pKnapsack[i].Address + GameGoodsChoseIDOffset), &pKnapsack[i].ChoseId, sizeof(pKnapsack[i].ChoseId), &Size) == 0)
				{
					TabPage1.LogOut(L"读取物品选择id失败");
					LeaveCriticalSection(&cs2);
					return ret;
				}

				if (ReadProcessMemory(hProcess, PDWORD(pKnapsack[i].Address + GameGoodsIDOffset), &pKnapsack[i].GoodsId, sizeof(pKnapsack[i].GoodsId), &Size) == 0)
				{
					TabPage1.LogOut(L"读取物品ID失败");
					LeaveCriticalSection(&cs2);
					return ret;
				}
				if (ReadProcessMemory(hProcess, PDWORD(pKnapsack[i].Address + GameUserEquipFlagOffset), &pKnapsack[i].EquipFlag, sizeof(pKnapsack[i].EquipFlag), &Size) == 0)
				{
					TabPage1.LogOut(L"读取物品装备标识失败");
					LeaveCriticalSection(&cs2);
					return ret;
				}
				if (ReadProcessMemory(hProcess, PDWORD(pKnapsack[i].Address + GameUserGoodsFlagOffset), &pKnapsack[i].GoodType, sizeof(pKnapsack[i].GoodType), &Size) == 0)
				{
					TabPage1.LogOut(L"读取物品种类标识失败");
					LeaveCriticalSection(&cs2);
					return ret;
				}
				//else
				//{
					//CStringA string;
					//string.Format("物品:%s 数量:%d ID:%d 装备标识%X", pKnapsack[i].Name, pKnapsack[i].Numb, pKnapsack[i].GoodsId, pKnapsack[i].EquipFlag);
					//TabPage1.LogOut(string);
				//}
				ret++;
			}
		}
	}
	LeaveCriticalSection(&cs2);
	return ret;
}

DWORD TraverseProcessModules(DWORD dwPid) {
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	// 创建一个快照句柄
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return FALSE;
	// 通过模块快照句柄获取第一个模块
	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return FALSE;
	}


	//Lvitem.cchTextMax = 0x10;
	// 循环获取模块信息
	do {
		if (wcscmp(L"Client.bin", me32.szModule) == 0)
		{
			return (DWORD)me32.modBaseAddr;
		};
	} while (Module32Next(hModuleSnap, &me32));
	// 关闭句柄并且退出函数
	CloseHandle(hModuleSnap);
	return 0;

}


BOOL CALLBACK EnumWindowsProc(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{

	GetWindowTextA(hwnd, WindowTitlebuffer, 256);

	GetClassNameA(hwnd, WindowClassbuffer, 256);
	if (strcmp(WindowClassbuffer, "Window.Frame") == 0)
	{
		
		GetWindowTextA(hwnd, WindowTitlebuffer, 256);
		if (*WindowTitlebuffer == NULL)
		{
			return true;

		}
		if (strstr(WindowTitlebuffer, "绿色辅助") == NULL)
		{
			//strcat(WindowTitlebuffer, "绿色辅助");		
			//SetWindowTextA(hwnd, WindowTitlebuffer);

			hGameWindow = hwnd;
			return false;
		}
	}
	

	return true;

}


BOOL CALLBACK EnumWindowsProc2(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{

	GetWindowTextA(hwnd, WindowTitlebuffer, 256);

	GetClassNameA(hwnd, WindowClassbuffer, 256);
	if (strcmp(WindowClassbuffer, "Window.Frame") == 0)
	{

		GetWindowTextA(hwnd, WindowTitlebuffer, 256);
		if (*WindowTitlebuffer == NULL)
		{
			return true;
		}
		if (strstr(WindowTitlebuffer, "绿色辅助") == NULL)
		{
			//strcat(WindowTitlebuffer, "绿色辅助");		
			//SetWindowTextA(hwnd, WindowTitlebuffer);

			hGameWindow2 = hwnd;
			return false;
		}
	}


	return true;

}



DWORD WINAPI GetGameBase(_In_ LPVOID lpParameter)
{

	DWORD pid = 0;
	DWORD GameBase = 0;
	DWORD Size = 0;
	DWORD Count = 0;
	hGameWindow = 0;
	RECT rect{};
	pDM->UnBindWindow();
	
	while (hGameWindow == NULL)
	{
		EnumWindows(EnumWindowsProc, NULL);
		Sleep(3000);
		
		if (Count > 7)
		{
			TabPage1.LogOut(L"多开失败,关闭辅助,重启游戏");
			return 0;
		}

		Count++;
	}
	
	pMyGetAdaptersInfo = (pGetAdaptersInfo*)GetProcAddress(LoadLibraryA("iphlpapi.dll"), "GetAdaptersInfo");
	if (pMyGetAdaptersInfo != NULL)
	{

		if (hGameWindow != NULL)
		{
			GetWindowRect(hGameWindow ,&rect);
			
			if (MoveWindow(hGameWindow, rect.left, rect.top, 0x00000462, 0x0000030c, false) != 0)
			//if (MoveWindow(hGameWindow, 0x0000015e- 0x0000012e, 0x00000076-0x40, 0x00000462, 0x0000030c, false)!=0)
			{
				GetWindowThreadProcessId(hGameWindow, &pid);
				if (pid != NULL)
				{
					GameBase = TraverseProcessModules(pid);
					if (GameBase == 0)
					{
						return 0;
					}
					hMainProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
					if (hMainProcess != NULL)
					{

						if (WriteProcessMemory(hMainProcess, pMyGetAdaptersInfo, shell, 3, &Size) != 0)
						{
							//pDM->SetSimMode(1);
							
								if (WordLibSet)
								{
									if (pDM->SetPath(_T(".\\")))
									{
										pDM->SetDictPwd("pqc");
										pDM->SetDict(0, _T("libstr.txt"));
										pDM->UseDict(0);
										WordLibSet = false;
										TabPage1.LogOut(L"多开功能开启");
									}
									
								}										
						}
														
					}
				}
			}
			
		}
	}
	
	return GameBase;
}

int InitUserInfor(DWORD GameBase,int flag)
{
	DWORD offset1 = 0;
	DWORD offset2 = 0;
	int Ret = 0;
	int Numb = 0;
	if (hMainProcess != NULL)
	{
		//DWORD Obj = *(int*)(*((int*)(*((int*)(GameBase + 0xAE5100)) + 0x1E0)) + 8);

		if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameUseBaseOffset), &offset1, 4, &Size) != 0)
		{
			if (ReadProcessMemory(hMainProcess, PDWORD(offset1 + 0x1E0), &offset2, 4, &Size) != 0)
			{
				if (ReadProcessMemory(hMainProcess, PDWORD(offset2+4), &ObjList, 4, &Size) != 0)
				{
					//if (ReadProcessMemory(hMainProcess, PDWORD(ObjList +4), &Objbase, 4, &Size) != 0)
					//{

						if (ObjList != 0)
						{
							if (flag==0)
							{
								UserNumb = 0;
								EnterCriticalSection(&cs);
								Ret = TraversalUser(ObjList, hMainProcess, GameOtherObj, &UserNumb);
								LeaveCriticalSection(&cs);
								
							}								
							else if (flag == 1)
							{
								MasterNumb = 0;
								EnterCriticalSection(&cs3);
								Ret = TraversalUser(ObjList, hMainProcess, GameOtherMaster, &MasterNumb,true);
								LeaveCriticalSection(&cs3);
								
									
							}
							else
							{
								Ret = TraversalMyUser(ObjList, hMainProcess);
								
							}
							
						//}
						
					}
					
				}
					
			}

		}

	}
	return Ret;
}


DWORD InitKnapsackInfor(DWORD GameBase)
{
	DWORD offset1 = 0;
	DWORD offset2 = 0;
	DWORD offset3 = 0;
	DWORD offset4 = 0;
	//DWORD offset5 = 0;
	DWORD offset6 = 0;

	if (hMainProcess != NULL)
	{
		//[[[[[[[GameBase+0xAA3B50]+0x14]+0]+0x8]+0x2d4]0x14]+0*4]

		if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameKnapsackBaseOffset), &offset1, 4, &Size) != 0)
		{
			if (ReadProcessMemory(hMainProcess, PDWORD(offset1 + 0x14), &offset2, 4, &Size) != 0)
			{
				if (ReadProcessMemory(hMainProcess, PDWORD(offset2 + 0x0), &offset3, 4, &Size) != 0)
				{
					if (ReadProcessMemory(hMainProcess, PDWORD(offset3 + 0x8), &offset4, 4, &Size) != 0)
					{
						if (ReadProcessMemory(hMainProcess, PDWORD(offset4 + 0x2d4), &UseGoodsEcx, 4, &Size) != 0)
						{
							if (ReadProcessMemory(hMainProcess, PDWORD(UseGoodsEcx + 0x14), &KnapsackList, 4, &Size) != 0)
							{
								if (KnapsackList != 0)
								{
									int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
									if (Ret)
									{
										//TabPage1.LogOut(L"背包信息初始化成功");
										return Ret;
									}
									else
									{
										TabPage1.LogOut(L"背包信息初始化失败");
									}

								}
							}
						}
					}
				
				}
			}

		}

	}
	return 0;
}


int FitDied(OtherUser* GameOtherUser)
{

	DWORD Falg1 = 0;
	DWORD Falg2 = 0;
	DWORD i = 0;
	CString time;
	int itime = 0;
	if (hMainProcess == NULL)
	{
		return 0;
	}

	//判断是不是已经死了的怪
	if (ReadProcessMemory(hMainProcess, PDWORD(GameOtherUser->ObjAddress + 0xF4), &Falg1, 4, &Size) != 0)
	{
		if (Falg1 == 0x60118)
		{
			return 1;
		}
	}
	if (GameUser.Infor!=0)
	{
		if (WriteProcessMemory(hMainProcess, PDWORD(GameUser.Infor + GameSkillMasterObj), &GameOtherUser->ObjID, 4, &Size) != 0)
		{
			
			TabPage3.m_eFit.GetWindowTextW(time);
			itime = _wtoi(time);
			if (itime!=0)
			{
				Sleep(itime);
				pDM->KeyPress(0x31);
			}
			
		}
	}

	return 0;
}

int BlackFunc(bool Fit)
{
	CString User;
	CString User2;
	CString str;
	int Ret=0;
	//DWORD Count = 0;
	DWORD Namelen = 0;
	TabPage3.m_eBlackList.GetWindowTextW(User);
	int length = findCount(User.GetBuffer(), '|') + 1;
	memset(BlackListName, 0, sizeof(BlackListName));
	int state = TabPage3.m_cBlackListBack.GetCheck();
	int state2 = TabPage3.m_cBlackListRand.GetCheck();



	for (size_t y = 0; y < length; y++)
	{
		if (length == 1)
		{
			wcscpy(BlackListName[y], User.GetBuffer());
		}
		else
		{
			if (User.Find(L"\r\n") >= 0)
			{
				//*(Map.GetBuffer() + Map.Find(L"\r\n")) = L'|';
				wcscpy(User.GetBuffer() + User.Find(L"\r\n"), (User.GetBuffer() + User.Find(L"\n")) + 1);
			}

			if (User.Find(L"|") >= 0)
			{
				wcsncpy(BlackListName[y], User.GetBuffer() + Namelen, User.Find(L"|") - Namelen);
				Namelen = User.Find(L"|");
			}
			else
			{
				wcsncpy(BlackListName[y], User.GetBuffer() + Namelen, wcslen(User.GetBuffer()) + 1 - Namelen);
			}

			if (User.Find(L"|") >= 0)
			{
				*(User.GetBuffer() + User.Find(L"|")) = L',';
				Namelen++;
			}
		}

	}
	

	int ret = 0;

	if (ObjList != 0)
	{
		UserNumb = 0;
		memset(GameOtherObj, 0, sizeof(GameOtherObj));
		if (GameBase != 0)
		{
			Ret=InitUserInfor(GameBase);
		}
		
		if (Ret >0)
		{
			for (size_t i = 0; i < UserNumb; i++)
			{
				if (GameOtherObj[i].UseFlag==0x24|| GameOtherObj[i].UseFlag == 0x14)
				{
					for (size_t x = 0; x < length; x++)
					{
						if (*BlackListName[x] != 0)
						{
							if (wcscmp(GameOtherObj[i].Name, BlackListName[x]) == NULL)
							{
								if (Fit)
								{
									FitDied(&GameOtherObj[i]);
								}
								else
								{
									if (state == BST_CHECKED)
									{
										
										if (WaitBackFlag == false)
										{
											WaitBackFlag = true;
										}
										

										ret = UseBack();
										str.Format(L"遇见黑名单回城:%s", BlackListName[x]);
										TabPage1.LogOut(str);


									}
									else if (state2 == BST_CHECKED)
									{

										ret = UseGoodsCall(GameGoodsRandRockID);
										int iState12 = TabPage2.m_cPoisonMonster.GetCheck();
										if (iState12 == BST_CHECKED)
										{
											Sleep(300);
											SetOn_lineState(0);
										}
										

										str.Format(L"遇见黑名单随机:%s", BlackListName[x]);
										TabPage1.LogOut(str);

									}

									BlackCd = false;

								}
								return ret;
							}
						}

					}
				
				}
				
			}

		}
		else
		{
			//TabPage1.LogOut(L"周围玩家信息刷新失败");
		}


	}
	
	return ret;
}


DWORD NoMasterAround(int iScope)
{
	DWORD Ret = 0;
	GetUserX_YCoord();
	
	memset(GameOtherMaster, 0, sizeof(GameOtherMaster));
	if (GameBase != 0)
	{
		Ret = InitUserInfor(GameBase,1);
	}

	if (Ret > 0)
	{
		for (size_t i = 0; i < Ret; i++)
		{
			if (GameOtherMaster[i].UseFlag == 0x14 && GameOtherMaster[i].RetinueFlag!=0xF)
			{
				
				if (((GameOtherMaster[i].x > (GameUser.x - iScope)) && (GameOtherMaster[i].x < (GameUser.x + iScope))) && ((GameOtherMaster[i].y > (GameUser.y - iScope)) && (GameOtherMaster[i].y < (GameUser.y + iScope))))
				{
					return 1;
				}

			}

		}
	}
	else
	{
		//TabPage1.LogOut(L"周围怪物信息刷新失败");
		return 0;
	}

	return 0;
}

DWORD MasterDied()
{

	DWORD Ret = 0;
	DWORD Falg = 0;
	DWORD Falg2 = 0;
	DWORD Address = 0;
	DWORD i = 0;
	if (hMainProcess==NULL)
	{
		return 0;
	}
	
	
	memset(GameOtherMaster, 0, sizeof(GameOtherMaster));
	if (GameBase != 0)
	{
		Ret = InitUserInfor(GameBase,1);
	}

	if (Ret > 0)
	{
		for (; i < Ret; )
		{
			if (GameOtherMaster[i].UseFlag == 0x14 && GameOtherMaster[i].RetinueFlag != 0xF)
			{
	
				if (GameOtherMaster[i].ObjAddress !=0)
				{
					//判断是否已经遍历过的怪
					if (GameOtherMaster[i].ObjID != 0)
					{
						for (size_t y = 0; y < 0x10; y++)
						{
							if (ObjID[y] == GameOtherMaster[i].ObjID)
							{
								goto Next1;
							}
						}
						if (IdIndex >= 0x10)
						{
							IdIndex = 0;
						}
						ObjID[IdIndex++] = GameOtherMaster[i].ObjID;
					}
					else
					{
						i++;
						continue;
					}



					Address = GameOtherMaster[i].ObjAddress;
					//判断是不是已经死了的怪
					if (ReadProcessMemory(hMainProcess, PDWORD(Address + 0xF4), &Falg2, 4, &Size) != 0)
					{
						if (Falg2 == 0x60118)
						{
							i++;
							continue;
						}
					}
					
					while (true)
					{
						//如果没死一直判断怪是否死了
						if (ReadProcessMemory(hMainProcess, PDWORD(Address + GameMasterDieFlagOffset), &Falg2, 4, &Size) != 0)
						{
							if (Falg2 == 0x60118)
							{
								return 1;
							}
						}
						Sleep(300);
					} 

				}
				else
				{
					i++;
					continue;
				}

			}
		Next1:
			i++;
		}


	}
	else
	{
		//TabPage1.LogOut(L"周围怪物信息刷新失败");
		return 2;
	}

	if (i!=0)
	{
		return 1;
	}
	return 0;
}

int NoExpMinute(int time)
{
	if (NoExpSwatch)
	{
		NoExpTime = dMinute + time;
		NoExpSwatch = false;

		ExpFlag = GetUserExp();
		if (ExpFlag == 0)
		{
			TabPage1.LogOut(L"经验获取失败");
		}
	}
	if (NoExpTime <= dMinute&& NoExpSwatch==false)
	{
		ExpFlag2= GetUserExp();
		if (ExpFlag != ExpFlag2)
		{
			NoExpSwatch = true;
			return 1;
		}
		else
		{
			NoExpSwatch = true;
			return 0;
		}
	}
	
	return -1;
}

DWORD GetUserInMap()
{
	if (GameUser.Infor ==0)
	{
		return 0;
	}
	DWORD Flag = 0;
	DWORD offset = 0;
	memset(GameUser.InMap, 0, sizeof(GameUser.InMap));
	if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.Infor + GameUserMapNameLenOffset), &Flag, sizeof(Flag), &Size) != 0)
	{
		if (Flag == 0xF)
		{
			if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.Infor + GameUserInMapOffset), &offset, sizeof(offset), &Size) != 0)
			{
				if (ReadProcessMemory(hMainProcess, PDWORD(offset), GameUser.InMap, sizeof(GameUser.InMap), &Size) != 0)
				{
					if (*GameUser.InMap != 0)
					{
						return 1;
					}
				}
			}
		}
		else 
		{
			if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.Infor + GameUserInMapOffset), GameUser.InMap, sizeof(GameUser.InMap), &Size) != 0)
			{
				if (*GameUser.InMap != 0)
				{
					return 1;
				}
			}
			
		}
	}
	
	return 0;
}

DWORD GetUserX_YCoord()
{
	if (GameUser.ObjAddress == 0)
	{
		return 0;
	}
	if (GameBase!=0)
	{
		InitUserInfor(GameBase);
	}
	GameUser.x=0;
	GameUser.y = 0;
	if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.ObjAddress + GameUserXOffset), &GameUser.x, sizeof(GameUser.x), &Size) != 0)
	{
		if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.ObjAddress + GameUserYOffset), &GameUser.y, sizeof(GameUser.y), &Size) != 0)
		{
			if (GameUser.x != 0&& GameUser.y != 0)
			{
				return 1;
			}
		}
		
	}
	return 0;
}

long long GetUserExp()
{
	
	if (GameBase == 0)
	{
		return 0;
	}
	long long Exp = 0;
	
	
	if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameUserExpOffset),&Exp, sizeof(DWORD), &Size) != 0)
	{
		
		if (Exp != 0 )
		{
			return Exp;
		}

	}
	return 0;
}

long long GetUserMoney(DWORD sign)
{
	//金币 Client.bin + AE4A0D
	//绑金 Client.bin+AE4A65
	
	if (GameBase==0)
	{
		return -1;
	}
	GameUser.Money = 0;
	GameUser.BindingMoney = 0;
	if (sign==1)
	{
		if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameUserMoneyOffset), &GameUser.Money, sizeof(GameUser.Money), &Size) != 0)
		{
			if (GameUser.Money!=0)
			{
				return GameUser.Money;
			}
			
		}
	}
	if (sign == 0)
	{
		if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameUserBindingMoneyOffset), &GameUser.BindingMoney, sizeof(GameUser.BindingMoney), &Size) != 0)
		{
			if (GameUser.BindingMoney != 0)
			{
				return GameUser.BindingMoney;
			}
			
		}
	}
	return -1;
}

DWORD GetUserHp_MaxHp()
{
	if (GameUser.ObjAddress == 0)
	{
		return 0;
	}
	GameUser.Hp = 0;
	GameUser.MaxHp = 0;
	if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameUserMaxHpBaseOffset-4), &GameUser.Hp, sizeof(GameUser.Hp), &Size) != 0)
	{
		if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameUserMaxHpBaseOffset), &GameUser.MaxHp, sizeof(GameUser.MaxHp), &Size) != 0)
		{
			if (GameUser.Hp > 0 && GameUser.MaxHp > 0)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}

	}
	return 0;
}

DWORD IsSafetyArea()			//安全区返回2   不是返回1  失败返回0
{
	InitUserInfor(GameBase, 3);
	if (GameUser.ObjAddress == 0)
	{
		return 0;
	}
	char SafetyArea = 0;
	if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.ObjAddress + SafetyAreaOffset), &SafetyArea, sizeof(SafetyArea), &Size) != 0)
	{
		if (SafetyArea == 0)
		{
			return 1;
		}
		else
		{
			return 2;

		}
	}

	
	return 0;
}

int ChoseUseState()
{
	if (TabPage3.iPattern==0)
	{
		return 0;
	}
	int state = -1;
	for (size_t i = 0; i < 8; i++)
	{
		if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.Infor + UseFitStateOffset), &state, 4, &Size) != 0)
		{
			if (state == TabPage3.iPattern-1)
			{
				return 1;
			}
			else
			{
				pDM->KeyDown(17);
				Sleep(100);
				pDM->KeyPress(72);
				Sleep(100);
				pDM->KeyUp(17);
			}

		}
	}
	return 0;
	
}

DWORD WayfindingCall(int x, int y)
{
	wchar_t string2[0x20]{};
	_bstr_t string;																						//大漠远程call字符串转换
	if (hMainProcess==NULL|| hGameWindow==NULL)
	{
		return 0;
	}
	if (GameBase == 0)
	{
		return 0;
	}
	InitUserInfor(GameBase);

	if (GameUser.ObjAddress==0)
	{
		return 0;
	}
	
	
	if (WayfindingAdress!=0)
	{
		
		if (pDM->AsmClear() != 1)
		{
			return 0;
		}
		if (pDM->AsmAdd("push 1FBD0") != 1)
		{
			return 0;
		}

		if (pDM->AsmAdd("push 0") != 1)
		{
			return 0;
		}
		
		memset(string2, 0, sizeof(string2));
		
		wsprintf(string2, L"push %x", y);
		string = _bstr_t(string2);
		if (((char*)string)==NULL)
		{
			return 0;
		}
		if (pDM->AsmAdd(string))
		{
			SysFreeString(string);
			memset(string2, 0, sizeof(string2));
			wsprintf(string2, L"push %x", x);
			string = _bstr_t(string2);
			if(GetLastError())
			{
				return 0;
			}
			if (pDM->AsmAdd(string))
			{
				SysFreeString(string);
				memset(string2, 0, sizeof(string2));
				wsprintf(string2, L"mov eax,%x", GameUser.ObjAddress);
				string = _bstr_t(string2);
				if(GetLastError())
				{
					return 0;
				}
				if (pDM->AsmAdd(string))
				{
					if (pDM->AsmAdd("mov ecx,dword ptr ds:[eax+4]"))
					{
						if (pDM->AsmAdd("mov edx,dword ptr ds:[ecx]"))
						{
							if (pDM->AsmAdd("mov eax,dword ptr ds:[edx+c8]"))
							{
								if (pDM->AsmAdd("call eax"))
								{
									SysFreeString(string);
									memset(string2, 0, sizeof(string2));
									wsprintf(string2, L"%x", WayfindingAdress);
									string = _bstr_t(string2);
									if(GetLastError())
									{
										return 0;
									}
									pDM->AsmCallEx((long)hGameWindow, 3, string);
									if (pDM->AsmClear() != 1)
									{
										return 0;
									}

								}

							}
						}
					}

				}
			}
		}
		
	
	}
	pDM->AsmClear();
	//Sleep(100);
	//memset(WayfindingAdress, 0, 1024);
	return 0;
}

DWORD UseGoodsCall(int GoodsId)
{
	wchar_t string2[0x50]{};
	_bstr_t string;																						//大漠远程call字符串转换
	if (hMainProcess == NULL || hGameWindow == NULL)
	{
		return 0;
	}
	if (UseGoodsEcx == 0)
	{
		return 0;
	}
	if (GameBase == 0)
	{
		return 0;
	}
	

	if (UseGoodsAdreress != 0)
	{
		if (pDM->AsmClear() != 1)
		{
			
			return 0;
		}
		if (pDM->AsmAdd("sub esp,20") != 1)
		{
			
			return 0;
		}

		if (pDM->AsmAdd("mov dword ptr ss:[esp+14],1") != 1)
		{
			
			return 0;
		}

		
		if (pDM->AsmAdd("lea ecx,dword ptr ss:[esp+14]"))
		{
			
			if (pDM->AsmAdd("push ecx"))
			{
				
				memset(string2, 0, sizeof(string2));
				wsprintf(string2, L"push %x", GoodsId);
				string = _bstr_t(string2);
				if(GetLastError())
				{
					
					return 0;
				}
				if (pDM->AsmAdd(string))
				{
					SysFreeString(string);
					memset(string2, 0, sizeof(string2));
					wsprintf(string2, L"mov ecx,%x", UseGoodsEcx);
					string = _bstr_t(string2);
					if(GetLastError())
					{
						
						return 0;
					}
					if (pDM->AsmAdd(string))
					{
						SysFreeString(string);
						memset(string2, 0, sizeof(string2));
						wsprintf(string2, L"mov eax,%x", UseGoodsCallAddress);
						string = _bstr_t(string2);
						if(GetLastError())
						{
							
							return 0;
						}
						if (pDM->AsmAdd(string))
						{
							if (pDM->AsmAdd("call eax"))
							{
								if (pDM->AsmAdd("add esp,20"))
								{
									SysFreeString(string);
									memset(string2, 0, sizeof(string2));
									wsprintf(string2, L"%x", UseGoodsAdreress);
									string = _bstr_t(string2);
									if(GetLastError())
									{
										return 0;
									}
									pDM->AsmCallEx((long)hGameWindow, 3, string);
									if (pDM->AsmClear() != 1)
									{
										
										return 0;
									}
									Sleep(1300);
									return 1;
								}
								
							}
							
						}
					}

				}
			}
		}

	}
	
	return 0;
}

Knapsack* ChoseGoods(char* GoodsName)
{
	if (KnapsackList==0 || hMainProcess==0)
	{
		return 0;
	}
	
	int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
	if (Ret)
	{
		for (SIZE_T i = 0; i < GameGoodsMaxNumber; i++)
		{
			if (strcmp(GoodsName, MyKnapsack[i].Name) == 0)
			{
				return &MyKnapsack[i];
				
			}

		}
		return NULL;
	}
	else
	{
		TabPage1.LogOut(L"ChoseGoods背包信息初始化失败");
	}
	return NULL;
}

DWORD GetGoodsNumber(short GoodsId)
{
	if (KnapsackList == 0 || hMainProcess == 0)
	{
		return 0;
	}
	DWORD Numb = 0;
	int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
	if (Ret)
	{
		for (SIZE_T i = 0; i < GameGoodsMaxNumber; i++)
		{
			if (GoodsId == MyKnapsack[i].GoodsId)
			{
				Numb += MyKnapsack[i].Numb;
			}
		}
		return Numb;
	}
	else
	{
		TabPage1.LogOut(L"GetGoodsNumber背包信息初始化失败");
	}
	return Numb;
}

DWORD ChoseAdamasEquip(DWORD* Numb=NULL)
{
	if (KnapsackList == 0 || hMainProcess == 0)
	{
		return 0;
	}
	DWORD address = 0;
	bool sign = true;
	if (Numb != NULL)
	{
		*Numb = 0;
	}
	
	
	int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
	if (Ret)
	{
		for (SIZE_T i = 0; i < GameGoodsMaxNumber; i++)
		{
			if ((MyKnapsack[i].EquipFlag == GameUserEquipFlag|| MyKnapsack[i].EquipFlag == GameUserEquipFlag2)&& MyKnapsack[i].GoodType != GameGoodsRockFlag)
			{
				if (sign == true)
				{
					
					address = MyKnapsack[i].Address;
					sign = false;

				}
				if (Numb != NULL)
				{
					(*Numb)++;
				}
			}

		}
		return address;
	}
	else
	{
		TabPage1.LogOut(L"ChoseAdamasEquip背包信息初始化失败");
	}
	return 0;
}

DWORD ChoseGoodsDisintegrateCall()
{
	DWORD GoodsAddress=0;
	DWORD offset = 0;
	DWORD offset2 = 0;
	wchar_t string2[0x50]{};
	_bstr_t string;																						//大漠远程call字符串转换
	if (hMainProcess == NULL || hGameWindow == NULL)
	{
		return 0;
	}
	
	if (GameBase == 0)
	{
		return 0;
	}
	if (GameUser.ObjAddress==0|| ChoseGoodsDisintegrateAdreress==0)
	{
		return 0;
	}
	GoodsAddress= ChoseAdamasEquip();
	if (GoodsAddress == 0)
	{
		return 2;
	}
	//[[对象+4]+0xFC8]

	if (ChoseGoodsDisintegrateEcx == 0)
	{
		if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.Infor + ChoseGoodsDisintegrateEcxOffset), &ChoseGoodsDisintegrateEcx, sizeof(DWORD), &Size) != 0)
		{
			if (ChoseGoodsDisintegrateEcx == 0)
			{
				return 0;
			}
		}
	}



	if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.Infor + ChoseGoodsDisintegrateEcxOffset), &offset2, sizeof(DWORD), &Size) != 0)
	{
		if (offset2!= 0)
		{
			ChoseGoodsDisintegrateEcx = offset2;
		}
	}
	if (ChoseGoodsDisintegrateEcx==0)
	{
		return 0;
	}
	
		

	

	if (ChoseGoodsDisintegrateCallAddress != 0)
	{
		if (pDM->AsmClear() != 1)
		{
			return 0;
		}
		
		memset(string2, 0, sizeof(string2));
		wsprintf(string2, L"mov edi,%x", GoodsAddress);
		string = _bstr_t(string2);
		if(GetLastError())
		{
			return 0;
		}
		if (pDM->AsmAdd(string))
		{

			if (pDM->AsmAdd("push dword ptr ds:[edi+21]"))
			{
				SysFreeString(string);
				memset(string2, 0, sizeof(string2));
				wsprintf(string2, L"mov ecx,%x", ChoseGoodsDisintegrateEcx);
				string = _bstr_t(string2);
				if(GetLastError())
				{
					return 0;
				}
				if (pDM->AsmAdd(string))
				{
					SysFreeString(string);
					memset(string2, 0, sizeof(string2));
					wsprintf(string2, L"mov eax,%x", ChoseGoodsDisintegrateCallAddress);
					string = _bstr_t(string2);
					if(GetLastError())
					{
						return 0;
					}
					if (pDM->AsmAdd(string))
					{
							
						if (pDM->AsmAdd("call eax"))
						{
							SysFreeString(string);
							memset(string2, 0, sizeof(string2));
							wsprintf(string2, L"%x", ChoseGoodsDisintegrateAdreress);
							string = _bstr_t(string2);
							if(GetLastError())
							{
								return 0;
							}
							pDM->AsmCallEx((long)hGameWindow, 3, string);
							if (pDM->AsmClear() != 1)
							{
								return 0;
							}
							return 1;
						}
					}

				}
			}

		}
		


	}
	return 0;
}

DWORD ChoseGoodsSave(DWORD* SaveGoodsAddress)
{
	if (KnapsackList == 0 || hMainProcess == 0)
	{
		return 0;
	}
	int istate1 = TabPage2.m_cSaveGoods.GetCheck();
	
	int index = 0;
	int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);

	if (Ret)
	{
		for (SIZE_T i = 0; i < GameGoodsMaxNumber; i++)
		{
			if (istate1 == BST_CHECKED)
			{
				if (MyKnapsack[i].GoodsId == GameGoodsPrayID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsTaskScrollID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsInfernoID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsHighPageID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsSapphireWarriorID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsSapphireTaoistID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsSapphireMagicID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsPageID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsAdamasID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
		
			}
			
				if (MyKnapsack[i].GoodsId == GameGoodsBigCureBagID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
				if (MyKnapsack[i].GoodsId == GameGoodsBigSnowBagID)
				{
					SaveGoodsAddress[index] = MyKnapsack[i].ChoseId;
					index++;
					continue;
				}
			
			
			
		}
		
	}
	else
	{
		TabPage1.LogOut(L"ChoseGoodsSave背包信息初始化失败");
	}
	return index;
}

DWORD SaveGoodsCall()
{
	wchar_t string2[0x50]{};
	_bstr_t string;																						//大漠远程call字符串转换
	if (hMainProcess == NULL || hGameWindow == NULL)
	{
		return -1;
	}
	if (UseGoodsEcx == 0)
	{
		return -1;
	}

	if (GameBase == 0|| GameUser.ObjAddress==0|| ChoseGoodsSaveAdreress==0)
	{
		return -1;
	}

	if (SaveGoodsoffset2 == 0)
	{
		if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.ObjAddress + GameUserInfoOffset), &SaveGoodsoffset1, sizeof(DWORD), &Size) != 0)
		{
			if (ReadProcessMemory(hMainProcess, PDWORD(SaveGoodsoffset1 + ChoseGoodsSaveEcxOffset), &SaveGoodsoffset2, sizeof(DWORD), &Size) != 0)
			{
			}

		}
	}

	if (SaveGoodsoffset2 == 0)
	{
		return -1;
	}


	memset(SaveGoodsId, 0, sizeof(SaveGoodsId));
	DWORD Count=ChoseGoodsSave(SaveGoodsId);
	if (Count==0)
	{
		return -1;
	}
	SIZE_T i = 0;
	for (; i < Count; i++)
	{
		if (pDM->AsmClear() != 1)
		{
			return 0;
		}
		if (pDM->AsmAdd("sub esp,20") != 1)
		{
			return 0;
		}
		
		memset(string2, 0, sizeof(string2));
		wsprintf(string2, L"mov dword ptr ss:[esp+8],%x", SaveGoodsId[i]);
		string = _bstr_t(string2);
		if(GetLastError())
		{
			return 0;
		}
		if (pDM->AsmAdd(string))
		{

			if (pDM->AsmAdd("push 4"))
			{


				if (pDM->AsmAdd("lea eax,dword ptr ss:[esp+C]"))
				{
					if (pDM->AsmAdd("push eax"))
					{

						if (pDM->AsmAdd("push 45C"))
						{
							SysFreeString(string);
							memset(string2, 0, sizeof(string2));
							wsprintf(string2, L"mov ecx,%x", SaveGoodsoffset2);
							string = _bstr_t(string2);
							if(GetLastError())
							{
								return 0;
							}
							if (pDM->AsmAdd(string))
							{
								SysFreeString(string);
								memset(string2, 0, sizeof(string2));
								wsprintf(string2, L"mov eax,%x", ChoseGoodsSaveCallAddress);
								string = _bstr_t(string2);
								if(GetLastError())
								{
									return 0;
								}
								if (pDM->AsmAdd(string))
								{
									if (pDM->AsmAdd("call eax"))
									{
										if (pDM->AsmAdd("add esp,20"))
										{
											SysFreeString(string);
											memset(string2, 0, sizeof(string2));
											wsprintf(string2, L"%x", ChoseGoodsSaveAdreress);
											string = _bstr_t(string2);
											if(GetLastError())
											{
												return 0;
											}
											pDM->AsmCallEx((long)hGameWindow, 3, string);
											if (pDM->AsmClear() != 1)
											{
												return 0;
											}
										}

									}
								}
							}
						}
					}
				}

			}
		}
		Sleep(1000);
	}
		
	return i;
}

DWORD KeyCall(int key)
{
	
	DWORD Edi = 0;
	wchar_t string2[0x50]{};
	_bstr_t string;																						//大漠远程call字符串转换
	if (hMainProcess == NULL || hGameWindow == NULL)
	{
		return 0;
	}

	if (GameBase == 0)
	{
		return 0;
	}
	if (KeyAdreress == 0)
	{
		return 0;
	}
	

	//edi获取


	if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameKnapsackBaseOffset+0x1c), &Edi, sizeof(DWORD), &Size) != 0)
	{

		if (Edi == 0)
		{
			return 0;
		}

	}


		if (pDM->AsmClear() != 1)
		{
			return 0;
		}
		
		memset(string2, 0, sizeof(string2));
		wsprintf(string2, L"mov edi,%x", Edi);
		string = _bstr_t(string2);
		
		if(GetLastError())
		{
			return 0;
		}
		if (pDM->AsmAdd(string))
		{
			SysFreeString(string);
			memset(string2, 0, sizeof(string2));
			wsprintf(string2, L"push %x", key);
			string = _bstr_t(string2);
			if(GetLastError())
			{
				return 0;
			}
			if (pDM->AsmAdd(string))
			{

				if (pDM->AsmAdd("mov eax,dword ptr ds:[edi]"))
				{

					if (pDM->AsmAdd("push 0"))
					{

						if (pDM->AsmAdd("mov ecx,edi"))
						{

							if (pDM->AsmAdd("call dword ptr ds:[eax+3C]"))
							{
								SysFreeString(string);
								memset(string2, 0, sizeof(string2));
								wsprintf(string2, L"%x", KeyAdreress);
								string = _bstr_t(string2);
								if(GetLastError())
								{
									return 0;
								}
								pDM->AsmCallEx((long)hGameWindow, 3, string);
								if (pDM->AsmClear() != 1)
								{
									return 0;
								}
								return 1;
							}
						}

					}
				}

			}

		}
	return 0;
}

int UseSkillCall(int SkillId)
{
	wchar_t string2[0x50]{};
	_bstr_t string;																						//大漠远程call字符串转换
	if (hMainProcess == NULL || hGameWindow == NULL)
	{
		return -1;
	}

	if (GameBase == 0 || GameUser.Infor == 0 )
	{
		return -1;
	}

	if (pDM->AsmClear() != 1)
	{
		return 0;
	}
	
	memset(string2, 0, sizeof(string2));
	wsprintf(string2, L"push %x", SkillId);
	string = _bstr_t(string2);
	if(GetLastError())
	{
		return 0;
	}
	if (pDM->AsmAdd(string))
	{
		SysFreeString(string);
		memset(string2, 0, sizeof(string2));
		wsprintf(string2, L"mov ecx,%x", GameUser.Infor+ 0x180);
		string = _bstr_t(string2);
		if(GetLastError())
		{
			return 0;
		}
		if (pDM->AsmAdd(string))
		{
			SysFreeString(string);
			memset(string2, 0, sizeof(string2));
			wsprintf(string2, L"mov eax,%x", SkillCallAddress);
			string = _bstr_t(string2);
			if(GetLastError())
			{
				return 0;
			}
			if (pDM->AsmAdd(string))
			{

				if (pDM->AsmAdd("call eax"))
				{
					SysFreeString(string);
					memset(string2, 0, sizeof(string2));
					wsprintf(string2, L"%x", SkillAdreress);
					string = _bstr_t(string2);
					if(GetLastError())
					{
						return 0;
					}
					pDM->AsmCallEx((long)hGameWindow, 3, string);
					if (pDM->AsmClear() != 1)
					{
						return 0;
					}
					//Sleep(100);
					//memset(SkillAdreress, 0, 1024);
					return 1;
				}
					
			}

		}

	}
	pDM->AsmClear();
	return 0;
}

int GetObjAndClick(long x1, long y1, long x2, long y2, _bstr_t str, _bstr_t color,int offset_x, int offset_y,int Click)
{	
	VARIANT x{};
	VARIANT y{};
	int a=pDM->FindStrFast(x1, y1, x2, y2, str, color, 0.9, &x, &y);
	
	if (Click!=-1)
	{
		CString str1;
		str1.Format(L"%x-%x", x.intVal, y.intVal);
		TabPage1.LogOut(str1);
	}
	
	if (a>=0 && Click!=-1)
	{
		Sleep(500);
		moveTo(x.intVal + offset_x, y.intVal + offset_y, Click);

	}
	return a;
}

//血少保护
DWORD WINAPI LowRedFunc(DWORD NowHp,DWORD MaxHp)
{
	wchar_t string2[0x50]{};
	_bstr_t string;																						//大漠远程call字符串转换
	DWORD state = TabPage3.m_cLittleRedBack.GetCheck();
	DWORD state2 = TabPage3.m_cLittleRedRand.GetCheck();
	DWORD state3 = TabPage3.m_cLittleRedDown.GetCheck();
	DWORD state4 = TabPage3.m_cLittleRedScreenshot.GetCheck();
	int state5 = TabPage2.m_cBackRollStart.GetCheck();
	//自动复活
	int iState13 = TabPage2.m_cResurgence.GetCheck();
	
	CString sHp;
	float tmphp = 0;
	if (MaxHp==0)
	{
		return 0;
	}

	if (state4 == BST_CHECKED && Screenshot)
	{
		TabPage3.m_eLittleRedScreenshot.GetWindowTextW(sHp);
		tmphp = _wtoi(sHp);

		if ((MaxHp * (tmphp / 100)) > NowHp)
		{
			//截图
			memset(string2, 0, sizeof(string2));
			wsprintf(string2, L"screen%d.bmp", ScreenNumb);
			string = _bstr_t(string2);
			pDM->Capture(0, 0, 0x0000045c, 0x000002fe, string);
			ScreenNumb++;

			Screenshot = false;
		}

	}

	if (NowHp == 0)
	{
		if (iState13 == BST_CHECKED && GetUserHp_MaxHp() == 2)
		{
			Sleep(1000);
			GetObjAndClick(10, 10, 1102, 729, "回城复活", "dedede-020202");
		}
	}

	if (state == BST_CHECKED)
	{
		TabPage3.m_eLittleRedBack.GetWindowTextW(sHp);
		tmphp = _wtoi(sHp);

		if ((MaxHp * (tmphp / 100)) > NowHp)
		{
			if (state == BST_CHECKED)
			{
				if (WaitBackFlag == false)
				{
					WaitBackFlag = true;
				}
			}

			if (UseBack() > 0)
			{
				return 2;
			}
			else
			{
				return 0;
			}
			

		}
	}
	if (state2 == BST_CHECKED&& Rand)
	{
		TabPage3.m_eLittleRedRand.GetWindowTextW(sHp);
		tmphp = _wtoi(sHp);

		if ((MaxHp * (tmphp / 100)) > NowHp)
		{

			GetUserInMap();
			if (StrCmpW(L"跨服龙城", GameUser.InMap )==0)
			{
				//使用地牢逃脱卷
				//UseGoodsCall(GameGoodsDungeonID);
			}
			else 
			{
				//使用随机石
				UseGoodsCall(GameGoodsRandRockID);
				
			}
			Rand = false;
			
			return 3;
		}
		
	}
	if (state3 == BST_CHECKED)
	{
		TabPage3.m_eLittleRedDown.GetWindowTextW(sHp);
		tmphp = _wtoi(sHp);

		if ((MaxHp * (tmphp / 100)) > NowHp)
		{
			
			
			TabPage1.LogOut(L"血少下线");
			// 结束游戏
			::TerminateProcess(hMainProcess, 0);
			SaveGoodsoffset2 = 0;			//仓库ecx
			CheckHp = false;				//检测hp标志
			EndFlag = false;				//挂机线程标志
			KnapsackList = 0;				//背包链表
			ChoseGoodsDisintegrateEcx = 0;	//分解NPCecx

			Main->StartScript.SetCheck(BST_UNCHECKED);

			if (hCheckProtectThread != NULL)
			{
				TerminateThread(hCheckProtectThread, 1);
				CloseHandle(hCheckProtectThread);
				hCheckProtectThread = NULL;		//玩家保护线程句柄
			}


			if (hRunScript != NULL)
			{
				TerminateThread(hRunScript, 1);
				CloseHandle(hRunScript);
				hRunScript = NULL;			//运行脚本线程句柄
				bAutoFlag = false;
				//EndFlag = false;
			}

			if (hTime != NULL)
			{
				TerminateThread(hTime, 1);
				CloseHandle(hTime);
				hTime = NULL;				//运行时间线程句柄
			}	
			if (hShowInfor != NULL)
			{
				SuspendThread(hShowInfor);
			}
			
			
			Initflag = true;
			hGameWindow = NULL;				//游戏窗口句柄
			hMainProcess = NULL;			//游戏进程句柄

			return 4;
		}
	}
	if (Screenshot==false)
	{
		return 1;
	}
	return 0;
}

DWORD CALLBACK CheakNoMasterFunc()
{
	if (TabPage3.m_cTimeNoMasterRand.GetCheck() == BST_CHECKED)
	{
		//没怪随机
		UseGoodsCall(340);
		return 1;
	}
	if (TabPage3.m_cRoundNoMasterRand.GetCheck() == BST_CHECKED)
	{
		//没怪随机
		UseGoodsCall(340);
		return 2;
	}

	return 0;
}

DWORD CALLBACK CheakPackFunc()
{
	if (KnapsackList==0|| hMainProcess==0)
	{
		return 0;
	}
	int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
	DWORD EquipNumb = 0;
	CString Numb;
	if (Ret)
	{
		
		if (TabPage3.m_cKnapsackResidue.GetCheck() == BST_CHECKED)
		{
			DWORD ret = KeyCall(0x42);
			if (ret)
			{
				
				TabPage3.m_eKnapsackResidue.GetWindowText(Numb);
				int num = _ttoi(Numb);
				if (Ret >= (GameGoodsMaxNumber - num))
				{
					//回收
					return 1;
				}
				
			}
		}
		if (TabPage3.m_cAdamasEquipment.GetCheck() == BST_CHECKED)
		{
			for (int i = 0; i < GameGoodsMaxNumber; i++)
			{
				if (MyKnapsack->EquipFlag== GameUserEquipFlag|| MyKnapsack->EquipFlag == GameUserEquipFlag2)
				{
					EquipNumb++;
				}
				
			}
			
			TabPage3.m_eAdamasEquipment.GetWindowText(Numb);
			int num = _ttoi(Numb);
			if (EquipNumb >=num)
			{
				//回城分解金刚石
				UseGoodsCall(337);
				return 2;
			}
			
		}
	}

	return 0;
	
}

DWORD moveTo(int x, int y,int Click)
{
	Sleep(300);
	if (x==0&& y==0)
	{
		
		//pDM->SetMouseDelay("windows", 50);
		pDM->MoveTo(x, y);
		return 1;
	}
	
	//pDM->SetMouseDelay("windows", 50);
	pDM->MoveTo(x, y);
	Sleep(1500 + Random(1, 500));
	if (Click ==1)
	{
		pDM->LeftDoubleClick();
	}
	else if(Click==0)
	{
		pDM->LeftDown();
		Sleep(Random(20, 40)); 
		pDM->LeftUp();
	}
	else if (Click == 3)
	{
		pDM->RightClick();
	}

	
	//pDM->MoveTo(601 + Random(1, 200), 59+ Random(1, 92) );
	Sleep(Random(1, 300));
	return 1;
}

CString GetSnowWarehouseNum()
{
	CString str = pDM->Ocr(58, 87, 104, 98, "FFFFFF-000000", 0.90);
	//TabPage1.LogOut(str);
	return str;
}

CString GetCurWarehouseNum()
{
	CString str2 = pDM->Ocr(69, 67, 111, 84, "FFFFFF-000000", 0.90);
	//TabPage1.LogOut(str2);
	return str2;
}

DWORD CloseAutoTrace(bool Switch)
{
	//0是关闭  1是打开
	if (GameBase == 0)
	{
		return 0;
	}
	DWORD AutoTrace = Switch;

	if (WriteProcessMemory(hMainProcess, PDWORD(GameBase + AutoTraceOffset), &AutoTrace, sizeof(AutoTrace), &Size) != 0)
	{
		return 1;
	}

	return 0;
}

DWORD HideUseAndMaster(bool Switch)
{
	//1隐藏  0显示
	if (GameBase == 0)
	{
		return 0;
	}
	DWORD HideUser = Switch;

	if (WriteProcessMemory(hMainProcess, PDWORD(GameBase + HideOtherUseOffset), &HideUser, sizeof(HideUser), &Size) != 0)
	{
		return 1;
	}

	return 0;
}

DWORD TaskbarTraceSwitch(bool Switch)
{
	//1隐藏  0显示
	if (GameBase == 0)
	{
		return 0;
	}
	DWORD TaskbarTrace = Switch;
	DWORD offset1 = 0;
	DWORD offset2 = 0;
	DWORD offset3 = 0;
	DWORD offset4 = 0;
	DWORD offset5 = 0;
	DWORD offset6 = 0;
	DWORD offset7 = 0;
	DWORD offset8 = 0;
	DWORD offset9 = 0;
	DWORD offset10 = 0;
	DWORD offset11 = 0;
	DWORD offset12 = 0;
	DWORD offset13 = 0;
	

	if (Switch==0)
	{
		if (WriteProcessMemory(hMainProcess, PDWORD(GameBase + HideTaskbarOffset), &TaskbarTrace, sizeof(TaskbarTrace), &Size) != 0)
		{
			return 1;
		}
	}
	else
	{
		if (WriteProcessMemory(hMainProcess, PDWORD(GameBase + HideTaskbarOffset), &TaskbarTrace, sizeof(TaskbarTrace), &Size) != 0)
		{
			if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameKnapsackBaseOffset), &offset1, sizeof(DWORD), &Size) != 0)
			{
				if (ReadProcessMemory(hMainProcess, PDWORD(offset1 + 0x14), &offset2, sizeof(DWORD), &Size) != 0)
				{
					if (ReadProcessMemory(hMainProcess, PDWORD(offset2), &offset3, sizeof(DWORD), &Size) != 0)
					{
						if (ReadProcessMemory(hMainProcess, PDWORD(offset3), &offset4, sizeof(DWORD), &Size) != 0)
						{
							if (ReadProcessMemory(hMainProcess, PDWORD(offset4), &offset5, sizeof(DWORD), &Size) != 0)
							{
								if (ReadProcessMemory(hMainProcess, PDWORD(offset5), &offset6, sizeof(DWORD), &Size) != 0)
								{
									if (ReadProcessMemory(hMainProcess, PDWORD(offset6), &offset7, sizeof(DWORD), &Size) != 0)
									{
										if (ReadProcessMemory(hMainProcess, PDWORD(offset7), &offset8, sizeof(DWORD), &Size) != 0)
										{
											if (ReadProcessMemory(hMainProcess, PDWORD(offset8), &offset9, sizeof(DWORD), &Size) != 0)
											{
												if (ReadProcessMemory(hMainProcess, PDWORD(offset9), &offset10, sizeof(DWORD), &Size) != 0)
												{
													if (ReadProcessMemory(hMainProcess, PDWORD(offset10 + 8), &offset11, sizeof(DWORD), &Size) != 0)
													{
														if (ReadProcessMemory(hMainProcess, PDWORD(offset11 + 0x30c), &offset12, sizeof(DWORD), &Size) != 0)
														{
															if (ReadProcessMemory(hMainProcess, PDWORD(offset12 + 4), &offset13, sizeof(DWORD), &Size) != 0)
															{
																if (WriteProcessMemory(hMainProcess, PDWORD(offset13 + 0xfc), &TaskbarTrace, sizeof(DWORD), &Size) != 0)
																{
																	return 1;
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

		

	return 0;
	
}

int AutoDefense()
{

	SetOn_lineState(0);
	Sleep(2000);
	pDM->MoveTo(544, 306);
	Sleep(50);
	UseSkillCall(0x136);

	Sleep(2000);
	pDM->MoveTo(544, 306);
	Sleep(50);
	UseSkillCall(0x137);


	return 1;
}

int StoreManagement()
{
	DWORD count = 0;
	DWORD count2 = 0;
	int iSnowNumb = 0;
	int iCurNumb = 0;
	DWORD Count = 0;
	//清理窗口
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(1000);
	//隐藏人物
	HideUseAndMaster(1);



	//打开仓库npc
	Sleep(1000);
	FindWay(132, 332);
	NpcMoveSleep
	count = 0;
	while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000",3,3,-1) < 0)
	{
		GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
		Sleep(1000);
		FindWay(132, 332);

		count++;
		if (count > 10)
		{
			return -1;
		}
	}
	Count = ChoseGoodsSave(SaveGoodsId);

	int istate1 = TabPage2.m_cSaveGoods.GetCheck();

	if (SaveGoodsoffset2 == 0)
	{
		//点击保管
		count = 0;
		while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000") < 0)
		{

			Sleep(1000);
			count++;
			if (count > 10)
			{
				return -1;
			}
		}
		//储存物品
		if (Count > 0)
		{
			Sleep(2000);
			if (SaveGoodsCall() < 0)
			{
				TabPage1.LogOut(L"储存仓库出错");
			}
		}
		//储存药品
		Sleep(500);
		count = 0;
		while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
		{
			Sleep(1000);
			count++;
			if (count > 10)
			{
				return -2;
			}
		}
		moveTo(0, 0);
	}
	else
	{
		if (istate1 == BST_CHECKED)
		{

			if (Count > 0)
			{
				//点击保管
				count = 0;
				while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					count++;
					if (count > 10)
					{
						return -1;
					}
				}
				//储存物品
				Sleep(2000);
				if (SaveGoodsCall() < 0)
				{
					TabPage1.LogOut(L"储存仓库出错");
				}

				//储存药品
				Sleep(500);
				count = 0;
				while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
				{
					Sleep(1000);
					count++;
					if (count > 10)
					{
						return -2;
					}
				}
				moveTo(0, 0);
			}
		}
	}


	Sleep(1000);
	count = 0;

	int istate2 = TabPage2.m_cSaveDrug.GetCheck();
	if (istate2 == BST_CHECKED)
	{


		while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
		{

			Sleep(1000);
			count++;
			if (count > 10)
			{
				return -3;
			}
		}
		moveTo(0, 0);
		Sleep(1000);
		count = 0;
		while (GetObjAndClick(0, 0, 402, 186, "存储药品", "FFFF00-000000") < 0)
		{

			Sleep(1000);
			count++;
			if (count > 10)
			{
				return -4;
			}
		}
		moveTo(0, 0);
		Sleep(1000);
		//雪霜包如果当前背包里的加上仓库的超过50   存储到50不储存了准备打大捆
		CString Numb{};
		count = 0;
		while (*Numb.GetBuffer() == NULL)
		{
			Numb = GetSnowWarehouseNum();
			TabPage1.LogOut(Numb);
			count++;
			if (count > 10)
			{
				TabPage1.LogOut(L"识别雪霜包出错");
				return -5;
			}
		}


		iSnowNumb = _wtoi(Numb);

		count2 = GetGoodsNumber(128);
		if (count2 > 0)
		{
			if (count2 + iSnowNumb <= 50)
			{
				for (size_t i = 0; i < count2; i++)
				{
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "存储雪霜包", "FFFF00-000000") < 0)
					{
						if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000") >= 0)
						{
							pDM->KeyPress(27);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
							{
								GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
								Sleep(1000);
								FindWay(132, 332);

								count++;
								if (count > 10)
								{
									return -1;
								}
							}
							Sleep(1000);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
							{

								while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
								{
									GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
									Sleep(1000);
									FindWay(132, 332);

									count++;
									if (count > 10)
									{
										return -1;
									}
								}
								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -5;
								}
							}
							moveTo(0, 0);
							count = 0;
							while (GetObjAndClick(0, 0, 402, 186, "存储药品", "FFFF00-000000") < 0)
							{
								count++;
								if (count > 10)
								{
									return -6;
								}
								Sleep(1000);
							}
							break;
							moveTo(0, 0);
						}
						Sleep(1000);

						count++;
						if (count > 10)
						{
							return -7;
						}
					}
					moveTo(0, 0);

				}
			}
			else
			{
				count2 = 50 - iSnowNumb;
				for (size_t i = 0; i < count2; i++)
				{
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "存储雪霜包", "FFFF00-000000") < 0)
					{
						if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000") >= 0)
						{
							pDM->KeyPress(27);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
							{
								GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
								Sleep(1000);
								FindWay(132, 332);

								count++;
								if (count > 10)
								{
									return -1;
								}
							}
							Sleep(1000);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
							{

								while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
								{
									GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
									Sleep(1000);
									FindWay(132, 332);

									count++;
									if (count > 10)
									{
										return -1;
									}
								}
								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -8;
								}
							}
							moveTo(0, 0);
							count = 0;
							while (GetObjAndClick(0, 0, 402, 186, "存储药品", "FFFF00-000000") < 0)
							{

								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -9;
								}
							}
							break;
							moveTo(0, 0);
						}
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -10;
						}
					}
					moveTo(0, 0);

				}

			}
		}
		//疗伤药包如果当前背包里的加上仓库的超过50  存储到50不储存了准备打大捆


		count = 0;
		Numb.Empty();

		while (*Numb.GetBuffer() == NULL)
		{
			Numb = GetCurWarehouseNum();
			TabPage1.LogOut(Numb);
			count++;
			if (count > 10)
			{
				TabPage1.LogOut(L"识别疗伤包出错");
				return -11;
			}
		}

		iCurNumb = _wtoi(Numb);
		count2 = GetGoodsNumber(125);
		if (count2 > 0)
		{
			if (count2 + iCurNumb <= 50)
			{

				for (size_t i = 0; i < count2; i++)
				{
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "存储疗伤药包", "FFFF00-000000") < 0)
					{
						if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000") >= 0)
						{
							pDM->KeyPress(27);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
							{
								GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
								Sleep(1000);
								FindWay(132, 332);

								count++;
								if (count > 10)
								{
									return -1;
								}
							}
							Sleep(1000);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
							{

								while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
								{
									GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
									Sleep(1000);
									FindWay(132, 332);

									count++;
									if (count > 10)
									{
										return -1;
									}
								}
								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -12;
								}
							}
							moveTo(0, 0);
							count = 0;
							while (GetObjAndClick(0, 0, 402, 186, "存储药品", "FFFF00-000000") < 0)
							{

								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -13;
								}
							}
							moveTo(0, 0);
							break;
						}
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -14;
						}
					}
					moveTo(0, 0);

				}
			}
			else
			{
				count2 = 50 - iCurNumb;
				for (size_t i = 0; i < count2; i++)
				{
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "存储疗伤药包", "FFFF00-000000") < 0)
					{
						if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000") >= 0)
						{
							pDM->KeyPress(27);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
							{
								GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
								Sleep(1000);
								FindWay(132, 332);

								count++;
								if (count > 10)
								{
									return -1;
								}
							}
							Sleep(1000);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
							{

								while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
								{
									GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
									Sleep(1000);
									FindWay(132, 332);

									count++;
									if (count > 10)
									{
										return -1;
									}
								}
								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -15;
								}
							}
							moveTo(0, 0);
							count = 0;
							while (GetObjAndClick(0, 0, 402, 186, "存储药品", "FFFF00-000000") < 0)
							{

								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -16;
								}
							}
							moveTo(0, 0);
							break;
						}
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -17;
						}
					}
					moveTo(0, 0);

				}

			}
		}

	}
	//如果启动打大捆  雪霜包
	if (TabPage2.m_cSnowStart.GetCheck() == BST_CHECKED&& iSnowNumb>5)
	{
		// 如果仓库雪霜药 数量大于40 准备打成大捆
		CString SnowNumb ;
		count = 0;
		while (*SnowNumb.GetBuffer() == NULL)
		{
			SnowNumb = GetSnowWarehouseNum();
			TabPage1.LogOut(SnowNumb);
			count++;
			if (count > 10)
			{
				TabPage1.LogOut(L"识别雪霜包出错");
				return -43;
			}
		}

		iSnowNumb = 0;
		iSnowNumb = _wtoi(SnowNumb);
		if (iSnowNumb > 40)
		{
			Sleep(500);
			count = 0;
			while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				count++;
				if (count > 10)
				{
					return -18;
				}
			}
			moveTo(0, 0);
			//循环4次
			for (size_t x = 0; x < 4; x++)
			{

				//如果背包剩余格数加上背包已经有的雪霜包数量 大于或者等于10个
				int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
				DWORD PackResidue = GameGoodsMaxNumber - Ret;
				count2 = GetGoodsNumber(128);
				size_t i = 0;
				if (PackResidue + count2 >= 10)
				{


				agen:
					Sleep(1000);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "取出雪霜包", "FFFF00-000000", 3, 3, -1) < 0)
					{

						while (GetObjAndClick(10, 68, 402, 186, "取出药品", "FFFF00-000000") < 0)
						{
							Sleep(1000);
							count++;
							if (count > 10)
							{
								return -32;
							}
						}
					}
					moveTo(0, 0);
					Sleep(1000);
					count2 = GetGoodsNumber(128);
					if (count2 < 10)
					{

						if (count2 = 0)
						{
							for (int t = 0; t < 2; t++)
							{
								count = 0;
								while (GetObjAndClick(10, 68, 402, 186, "取出5雪霜包", "FFFF00-000000") < 0)
								{
									if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000", 3, 3, -1) >= 0)
									{
										break;
									}
									Sleep(1000);
									count++;
									if (count > 10)
									{
										return -19;
									}
								}
								moveTo(0, 0);
								if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000") >= 0)
								{
									pDM->KeyPress(27);
									Sleep(2000);
									count = 0;
									while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
									{
										GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
										Sleep(1000);
										FindWay(132, 332);

										count++;
										if (count > 10)
										{
											return -1;
										}
									}
									Sleep(1000);
									count = 0;
									while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
									{
										
										while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
										{
											GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
											Sleep(1000);
											FindWay(132, 332);

											count++;
											if (count > 10)
											{
												return -1;
											}
										}
										Sleep(1000);
										count++;
										if (count > 10)
										{
											return -20;
										}
									}
									goto agen;
									break;
								}
								moveTo(0, 0);
							}
						}
						else
						{
							for (; i < 10 - count2; i++)
							{
								count = 0;
								while (GetObjAndClick(10, 68, 402, 186, "取出雪霜包", "FFFF00-000000") < 0)
								{
									if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000", 3, 3, -1) >= 0)
									{
										break;
									}
									Sleep(1000);
									count++;
									if (count > 10)
									{
										return -19;
									}
								}
								moveTo(0, 0);
								if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000") >= 0)
								{
									pDM->KeyPress(27);
									Sleep(2000);
									count = 0;
									while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
									{
										GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
										Sleep(1000);
										FindWay(132, 332);

										count++;
										if (count > 10)
										{
											return -1;
										}
									}
									Sleep(1000);
									count = 0;
									while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
									{
										
										while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
										{
											GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
											Sleep(1000);
											FindWay(132, 332);

											count++;
											if (count > 10)
											{
												return -1;
											}
										}
										Sleep(1000);
										count++;
										if (count > 10)
										{
											return -20;
										}
									}
									goto agen;
									break;
								}
								moveTo(0, 0);
						}
						
						}
					}
					Sleep(500);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -21;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -22;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "捆绑", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -23;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "捆绑", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -24;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(8, 112, 402, 186, "药水", "FFFFFF-000000", -15) < 0)
					{
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -25;
						}
					}
					Sleep(500);
					count = 0;
					while (GetObjAndClick(0, 0, 402, 186, "大捆雪霜包", "FFFFFF-000000", -15) < 0)
					{
						if (GetObjAndClick(0, 0, 402, 186, "缺少", "ffffff-000000", -15) >= 0)
						{
							pDM->KeyPress(27);
							Sleep(2000);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
							{
								GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
								Sleep(1000);
								FindWay(132, 332);

								count++;
								if (count > 10)
								{
									return -1;
								}
							}
							Sleep(1000);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
							{
								
								while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
								{
									GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
									Sleep(1000);
									FindWay(132, 332);

									count++;
									if (count > 10)
									{
										return -1;
									}
								}
								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -26;
								}
							}
							moveTo(0, 0);
							break;
						}
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -27;
						}
						moveTo(0, 0);
					}
					Sleep(1000);
					pDM->KeyPress(27);
					Sleep(1000);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
					{
						GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
						Sleep(1000);
						FindWay(132, 332);

						count++;
						if (count > 10)
						{
							return -1;
						}
					}
					Sleep(1000);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
					{
						while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
						{
							GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
							Sleep(1000);
							FindWay(132, 332);

							count++;
							if (count > 10)
							{
								return -1;
							}
						}
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -28;
						}
					}
					moveTo(0, 0);

				}
			}
		}
	}

	
	//如果启动打大捆 疗伤包
	if (TabPage2.m_cCureStart.GetCheck() == BST_CHECKED&& iCurNumb>5)
	{
		pDM->KeyPress(27);
		Sleep(1000);
		pDM->KeyPress(27);
		Sleep(1000);
		pDM->KeyPress(27);
		Sleep(1000);
		//隐藏人物
		HideUseAndMaster(1);
		count = 0;
		FindWay(132, 332);
		while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
		{
			GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
			Sleep(1000);
			FindWay(132, 332);

			count++;
			if (count > 10)
			{
				return -1;
			}
		}
		Sleep(1000);
		count = 0;
		while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
		{
			while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
				Sleep(1000);
				FindWay(132, 332);

				count++;
				if (count > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
			count++;
			if (count > 10)
			{
				return -29;
			}
		}
		moveTo(0, 0);
		count = 0;
		while (GetObjAndClick(0, 0, 402, 186, "取出药品", "FFFF00-000000") < 0)
		{

			Sleep(1000);
			count++;
			if (count > 10)
			{
				return -30;
			}
		}
		moveTo(0, 0);
		// 如果仓库疗伤药 数量大于40 准备打成大捆
		CString CurNumb ;
		count = 0;
		while (*CurNumb.GetBuffer() == NULL)
		{
			CurNumb = GetCurWarehouseNum();
			TabPage1.LogOut(CurNumb);
			count++;
			if (count > 10)
			{
				TabPage1.LogOut(L"识别疗伤包出错");
				return -43;
			}
		}
		iCurNumb = 0;
		iCurNumb = _wtoi(CurNumb);
		if (iCurNumb > 40)
		{
			/*Sleep(500);
			count = 0;
			while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				count++;
				if (count > 10)
				{
					return -31;
				}
			}
			moveTo(0, 0);*/
			//循环4次
			for (size_t x = 0; x < 4; x++)
			{

				//如果背包剩余格数加上背包已经有的雪霜包数量 大于或者等于10个
				int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
				DWORD PackResidue = GameGoodsMaxNumber - Ret;
				count2 = GetGoodsNumber(125);
				size_t i = 0;
				if (PackResidue + count2 >= 10)
				{


				agen2:
					Sleep(1000);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "取出疗伤药包", "FFFF00-000000", 3, 3, -1) < 0)
					{
						

						while(GetObjAndClick(10, 68, 402, 186, "取出药品", "FFFF00-000000")<0)
						{
							Sleep(1000);
							count++;
							if (count > 10)
							{
								return -32;
							}
						}
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -32;
						}
					}
					moveTo(0, 0);
					Sleep(1000);
					count2 = GetGoodsNumber(125);
					if (count2 < 10)
					{
						if (count2==0)
						{
							for (int t = 0; t < 2; t++)
							{
								count = 0;
								while (GetObjAndClick(10, 68, 402, 186, "取出5疗伤药包", "FFFF00-000000") < 0)
								{
									if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000", 3, 3, -1) >= 0)
									{
										break;
									}
									Sleep(1000);
									count++;
									if (count > 10)
									{
										return -33;
									}
								}
								moveTo(0, 0);
								if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000") >= 0)
								{
									pDM->KeyPress(27);
									Sleep(2000);
									count = 0;
									while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
									{
										GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
										Sleep(1000);
										FindWay(132, 332);

										count++;
										if (count > 10)
										{
											return -1;
										}
									}
									Sleep(1000);
									count = 0;
									while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
									{
										
										while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
										{
											GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
											Sleep(1000);
											FindWay(132, 332);

											count++;
											if (count > 10)
											{
												return -1;
											}
										}
										
										Sleep(1000);
										count++;
										if (count > 10)
										{
											return -34;
										}
									}
									moveTo(0, 0);
									goto agen2;
									break;
								}
								moveTo(0, 0);
							}
						}
						else
						{
							for (; i < 10 - count2; i++)
							{
								count = 0;
								while (GetObjAndClick(10, 68, 402, 186, "取出疗伤药包", "FFFF00-000000") < 0)
								{
									if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000",3,3,-1) >= 0)
									{
										break;
									}
									Sleep(1000);
									count++;
									if (count > 10)
									{
										return -33;
									}
								}
								moveTo(0, 0);
								if (GetObjAndClick(0, 0, 402, 186, "失败", "FFFF00-000000") >= 0)
								{
									pDM->KeyPress(27);
									Sleep(2000);
									count = 0;
									while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
									{
										GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
										Sleep(1000);
										FindWay(132, 332);

										count++;
										if (count > 10)
										{
											return -1;
										}
									}
									Sleep(1000);
									count = 0;
									while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
									{
										
										while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
										{
											GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
											Sleep(1000);
											FindWay(132, 332);

											count++;
											if (count > 10)
											{
												return -1;
											}
										}
										Sleep(1000);
										count++;
										if (count > 10)
										{
											return -34;
										}
									}
									moveTo(0, 0);
									goto agen2;
									break;
								}
								moveTo(0, 0);
						}

						

						}
					}
					Sleep(500);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -35;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -36;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "捆绑", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -37;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "捆绑", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -38;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(8, 112, 402, 186, "药水", "FFFFFF-000000", -15) < 0)
					{
						Sleep(1000);
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -39;
						}
					}
					moveTo(0, 0);
					Sleep(500);
					count = 0;
					while (GetObjAndClick(0, 0, 402, 186, "大捆疗伤药包", "FFFFFF-000000", -15) < 0)
					{
						if (GetObjAndClick(0, 0, 402, 186, "缺少", "ffffff-000000", -15) >= 0)
						{
							pDM->KeyPress(27);
							Sleep(2000);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
							{
								GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
								Sleep(1000);
								FindWay(132, 332);

								count++;
								if (count > 10)
								{
									return -1;
								}
							}
							Sleep(1000);
							count = 0;
							while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
							{
								
								while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
								{
									GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
									Sleep(1000);
									FindWay(132, 332);

									count++;
									if (count > 10)
									{
										return -1;
									}
								}
								Sleep(1000);
								count++;
								if (count > 10)
								{
									return -40;
								}
							}

							moveTo(0, 0);
							break;
						}
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -41;
						}
					}
					moveTo(0, 0);

					Sleep(1000);
					pDM->KeyPress(27);
					Sleep(1000);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "保管", "FFFF00-000000", 3, 3, -1) < 0)
					{
						GetObjAndClick(29, 258, 285, 403, "仓库保管员", "00FF00-000000", 29, -34);
						Sleep(1000);
						FindWay(132, 332);

						count++;
						if (count > 10)
						{
							return -1;
						}
					}
					Sleep(1000);
					count = 0;
					while (GetObjAndClick(10, 68, 402, 186, "药品仓库", "FFFF00-000000") < 0)
					{
						
						Sleep(1000);
						count++;
						if (count > 10)
						{
							return -42;
						}
					}
					moveTo(0, 0);
				}
			}
		}
	}
	pDM->KeyPress(27);
	Sleep(1000);
	pDM->KeyPress(27);
	Sleep(1000);
	pDM->KeyPress(27);
	Sleep(1000);

	//打完大捆 再次存下物品
	
	if (SaveGoodsCall() < 0)
	{
		TabPage1.LogOut(L"储存仓库出错");
	}
	
	TabPage1.LogOut(L"储存仓库完成");
	return 1;

}

int DrugsManagement(int BuyRedNumb, int BuyBlueNumb)
{
	DWORD Overtime = 0;
	//清理窗口
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	//隐藏人物
	HideUseAndMaster(1);
	FindWay(133, 344);
	NpcMoveSleep
		Overtime = 0;
	while (GetObjAndClick(10, 68, 402, 186, "批量购买药品", "FFFF00-000000", 3, 3, -1) < 0)
	{
		GetObjAndClick(862, 589, 1110, 747, "苍月药", "00FF00-000000", 24, -37);
		FindWay(133, 344);
		Sleep(2000);
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}
	}

	while (GetObjAndClick(10, 68, 402, 186, "批量购买药品", "FFFF00-000000") < 0)
	{
		Overtime++;
		Sleep(1000);
		if (Overtime > 10)
		{
			return -1;
		}
	}
	DWORD Times = BuyRedNumb % 10;
	DWORD Times2 = BuyBlueNumb % 10;

	if (Times > 0 && BuyRedNumb > 0)
	{


		for (size_t i = 0; i < (BuyRedNumb / 10) + 1; i++)
		{
			Overtime = 0;
			while (GetObjAndClick(2, 28, 402, 55, "购买", "ffff00-00000", 5, 3) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}



			}

			Sleep(1000);

		}





	}
	else if (BuyRedNumb > 0)
	{


		for (size_t i = 0; i < (BuyRedNumb / 10); i++)
		{
			Overtime = 0;
			while (GetObjAndClick(2, 28, 402, 55, "购买", "ffff00-00000", 5, 3) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}



			}


			Sleep(1000);


		}
	}


	if (Times2 > 0 && BuyBlueNumb > 0)
	{
		for (size_t i = 0; i < (BuyBlueNumb / 10) + 1; i++)
		{
			Overtime = 0;
			while (GetObjAndClick(4, 67, 403, 89, "购买", "ffff00-00000", 5, 3) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
		}


	}
	else if (BuyBlueNumb > 0)
	{
		for (size_t i = 0; i < (BuyBlueNumb / 10); i++)
		{
			Overtime = 0;
			while (GetObjAndClick(4, 67, 403, 89, "购买", "ffff00-00000", 5, 3) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
		}


	}

	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	//隐藏人物
	HideUseAndMaster(1);
	

	return 1;
}

int TrampNpcFunc(bool Ryongson)
{
	//用来防止卡死
	DWORD Overtime = 0;
	//获取勾选状态
	int state  = TabPage2.m_cBackRollStart.GetCheck();
	int state2 = TabPage2.m_cRedPoison.GetCheck();
	int state3 = TabPage2.m_cGreenPoison.GetCheck();
	int state4 = TabPage2.m_cAmulet.GetCheck();
	int state5 = TabPage2.m_cDungeon.GetCheck();

	CString BackRollNumb;
	int iBackRollNumb = 0;
	CString RedPoisonNumb;
	int iRedPoisonNumb=0;
	CString GreenPoisonNumb;
	int iGreenPoisonNumb = 0;
	CString AmuletPoisonNumb;
	int iAmuletNumb = 0;
	CString sDungeonNumb;
	int iDungeonNumb = 0;
	



	//检测是否勾了回城卷
	if (state== BST_CHECKED)
	{
		TabPage2.m_eBackRollNumb.GetWindowTextW(BackRollNumb);
		iBackRollNumb = _wtoi(BackRollNumb);
		
	}
	//检测是否勾选地牢逃脱包
	if (state5 == BST_CHECKED)
	{
		TabPage2.m_eDungeon.GetWindowTextW(sDungeonNumb);
		iDungeonNumb = _wtoi(sDungeonNumb);

	}

	//检测是否道士  是就获取数量
	if (GameUser.Occupation>0x300)
	{
		if (state2==BST_CHECKED)
		{
			TabPage2.m_eRedPoison.GetWindowTextW(RedPoisonNumb);
			iRedPoisonNumb = _wtoi(RedPoisonNumb);
		}
		if (state3 == BST_CHECKED)
		{
			TabPage2.m_eGreenPoison.GetWindowTextW(GreenPoisonNumb);
			iGreenPoisonNumb = _wtoi(GreenPoisonNumb);
		}
		if (state4 == BST_CHECKED)
		{
			TabPage2.m_eAmulet.GetWindowTextW(AmuletPoisonNumb);
			iAmuletNumb = _wtoi(AmuletPoisonNumb);
		}

	}
	
	//获得背包当前有的数量
	int RedNumb = GetGoodsNumber(GameGoodsRedPoisonID);
	int GreenNumb = GetGoodsNumber(GameGoodsGreenPoisonID);
	int AmuletNumb = GetGoodsNumber(GameGoodsSuperAmuletID);
	int BackNumb = GetGoodsNumber(GameGoodsBackRollID);
	int DungeonNumb = GetGoodsNumber(GameGoodsDungeonPackID);
	
	//继续回来购买
	BuyAgen:

	//算出 所需 数量
	int NeedRedPoison = iRedPoisonNumb - RedNumb;
	int NeedGreenPoison = iGreenPoisonNumb - GreenNumb;
	int NeedAmulet = iAmuletNumb - AmuletNumb;
	int NeedBackRoll = iBackRollNumb - BackNumb;
	int NeedDungeon = iDungeonNumb - DungeonNumb;

	//如果有一样要买就继续否则返回
	if (NeedRedPoison <=0 && NeedGreenPoison <= 0&& NeedAmulet <= 0&& NeedBackRoll <=0)
	{
		return 2;
	}

	//清理窗口
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(1000);
	//隐藏人物
	HideUseAndMaster(1);
	Sleep(500);
	//关闭任务自动追踪
	TaskbarTraceSwitch(false);
	//过去购买
	if (FindWay(338, 335) == 1)
	{
		NpcMoveSleep
		Overtime = 0;
		while (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000", 3, 3, -1) < 0)
		{
			GetObjAndClick(929, 198, 1102, 348, "流浪汉", "00FF00-000000", 18, -28);
			FindWay(338, 335);
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}

		Overtime = 0;
		Sleep(1000);
		while (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000") < 0)
		{
		
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}
	}
	else
	{
		return -3;
	}

	//如果需要就买红毒
	if (NeedRedPoison>0)
	{
		for (size_t i = 0; i < NeedRedPoison; i++)
		{
			NpcMoveSleep
			Overtime = 0;
			while (GetObjAndClick(118, 360, 396, 431, "黄色药粉大量", "DEDEDE-020202", 5, 3, 1) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
				if (Overtime>2)
				{
					Overtime = 0;
					while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 11)
						{
							return -2;
						}
					}

					Overtime = 0;
					Sleep(1000);
					while (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000") < 0)
					{

						Sleep(1000);
						Overtime++;
						if (Overtime > 12)
						{
							return -1;
						}
					}

				}
			}

			moveTo(386, 421);
			Sleep(1000);
		}
	}

	//如果需要就买绿毒
	if (NeedGreenPoison > 0)
	{
		for (size_t i = 0; i < NeedGreenPoison; i++)
		{
			NpcMoveSleep
			Overtime = 0;
			while (GetObjAndClick(118, 360, 396, 431, "灰色药粉大量", "DEDEDE-020202", 5, 3, 1) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
				if (Overtime > 2)
				{
					Overtime = 0;
					while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 11)
						{
							return -2;
						}
					}

					Overtime = 0;
					Sleep(1000);
					while (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000") < 0)
					{

						Sleep(1000);
						Overtime++;
						if (Overtime > 12)
						{
							return -1;
						}
					}

				}
			}

			moveTo(386, 421);
			Sleep(1000);
		}
	}

	//下面的需要翻页

	if (NeedAmulet > 0|| NeedBackRoll > 0|| NeedDungeon > 0)
	{
		NpcMoveSleep
		Overtime = 0;
		while (GetObjAndClick(85, 425, 406, 479, "下一页", "DEDEDE-020202") < 0)
		{

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}
	}
	else
	{
		//清理窗口
		pDM->KeyPress(27);
		Sleep(1000);
		pDM->KeyPress(27);
		Sleep(1000);
		pDM->KeyPress(27);
		return 3;
	}
	


	//如果需要就买护身符
	if (NeedAmulet > 0)
	{
		for (size_t i = 0; i < NeedAmulet; i++)
		{
			NpcMoveSleep
			Overtime = 0;
			while (GetObjAndClick(86, 245, 396, 431, "超级护身符", "DEDEDE-020202", 5, 3, 1) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}

				if (Overtime > 2)
				{
					Overtime = 0;
					while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -2;
						}
					}

					Overtime = 0;
					Sleep(1000);
					while (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000") < 0)
					{

						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}
					}
					Overtime = 0;
					while (GetObjAndClick(85, 425, 406, 479, "下一页", "DEDEDE-020202") < 0)
					{

						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}
					}
					Sleep(1000);
				}
			}

			moveTo(386, 421);
		}
	}
	//判断背包格子是否足够  不然会卡死
	/*int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
	DWORD PackResidue = GameGoodsMaxNumber - Ret;*/
	/*if (PackResidue < NeedDungeon)
	{
		NeedBackRoll = PackResidue;
		iBackRollNumb = NeedBackRoll;
	}*/

	//如果需要就买回城卷
	if (NeedBackRoll > 0)
	{
		for (size_t i = 0; i < NeedBackRoll; i++)
		{
			
			while (GetGoodsNumber(GameGoodsBackRollID) < iBackRollNumb)
			{
				while (GetObjAndClick(86, 245, 396, 431, "回城卷", "DEDEDE-020202", 5, 3, 1) < 0)
				{

					Overtime = 0;
					while (GetObjAndClick(10, 68, 402, 186, "返回", "FFFF00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 11)
						{
							return -2;
						}
					}


					Sleep(1000);
					while (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000") < 0)
					{

						Sleep(1000);
						Overtime++;
						if (Overtime > 12)
						{
							return -1;
						}
					}

					while (GetObjAndClick(85, 425, 406, 479, "下一页", "DEDEDE-020202") < 0)
					{

						Sleep(1000);
						Overtime++;
						if (Overtime > 13)
						{
							return -1;
						}
					}
					
				}
				Sleep(1500);

				moveTo(386, 421);
			}

			
		}
	}

	//判断是不是在跨服龙城
	//if (Ryongson)
	//{
	//	//判断背包格子是否足够  不然会卡死
	//	int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
	//	int PackResidue = GameGoodsMaxNumber - Ret;
	//	if (PackResidue < NeedDungeon)
	//	{
	//		NeedDungeon = PackResidue;
	//		iDungeonNumb = DungeonNumb + NeedDungeon;
	//	}

	//	//如果需要就买地牢逃脱包
	//	if (NeedDungeon > 0)
	//	{
	//		for (size_t i = 0; i < NeedDungeon; i++)
	//		{
	//			
	//			while (GetGoodsNumber(GameGoodsDungeonPackID) < DungeonNumb+ NeedDungeon)
	//			{
	//				GetObjAndClick(86, 245, 396, 431, "地牢逃脱卷包", "DEDEDE-020202", 5, 3, 1);
	//				NpcMoveSleep
	//				moveTo(386, 421);
	//			}

	//			
	//		}
	//	}
	//}
	

	//清理窗口
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(1000);
	//隐藏人物
	HideUseAndMaster(1);
	//获取已经有的
	RedNumb = GetGoodsNumber(GameGoodsRedPoisonID);
	GreenNumb = GetGoodsNumber(GameGoodsGreenPoisonID);
	AmuletNumb = GetGoodsNumber(GameGoodsSuperAmuletID);
	BackNumb = GetGoodsNumber(GameGoodsBackRollID);
	DungeonNumb = GetGoodsNumber(GameGoodsDungeonPackID);

	if (Ryongson)
	{
		//有一个数量不够回去再买
		if (RedNumb < iRedPoisonNumb || GreenNumb < iGreenPoisonNumb || AmuletNumb < iAmuletNumb || BackNumb < iBackRollNumb || DungeonNumb < iDungeonNumb)
		{
			goto BuyAgen;
		}
	}
	else
	{
		//有一个数量不够回去再买
		if (RedNumb < iRedPoisonNumb || GreenNumb < iGreenPoisonNumb || AmuletNumb < iAmuletNumb || BackNumb < iBackRollNumb )
		{
			goto BuyAgen;
		}
	}
	

	return 1;

}

int RecycleNpcFunc()
{

	DWORD Overtime = 0;
	DWORD state=TabPage3.m_cKnapsackResidue.GetCheck();
	int istate = TabPage2.m_cBackRecycleStart.GetCheck();

	if (istate == BST_UNCHECKED && state == BST_UNCHECKED)
	{
		return 0;
	}

	if (state == BST_UNCHECKED)
	{
		//走到Npc附近坐标
		if (FindWay(334, 322) < 0)
		{
			return -1;
		}
	}

	Overtime = 0;
	while (GetObjAndClick(10, 10, 1102, 729, "回收", "DEDEDE-020202", 3, 3, -1) < 0)
	{
		//打开背包
		KeyCall(0x78);
		Sleep(1000);
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}

	}

	GetUserInMap();
	if (wcsstr( GameUser.InMap, L"地牢") != NULL && state == BST_CHECKED)
	{
		//吃散红
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsfgrdID) > 1)
		{
			pDM->KeyDown(49);
		}
		pDM->KeyUp(49);
		//吃散蓝
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsMagicDrugID) > 1)
		{
			pDM->KeyDown(50);
			
		}
		pDM->KeyUp(50);

		//吃中量红
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsmiddlefgrdID) > 0)
		{
			pDM->KeyDown(51);
			
		}
		pDM->KeyUp(51);
		//吃中量蓝
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsmiddleMagicID) > 0)
		{
			pDM->KeyDown(52);
			
		}
		pDM->KeyUp(52);
		//吃中量太阳水
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsSunWaterID) > 0)
		{
			pDM->KeyDown(53);
			
		}
		pDM->KeyUp(53);
	}
	else
	{
		////吃散红
		//Overtime = 0;
		//while (GetGoodsNumber(GameGoodsfgrdID) > 1)
		//{
		//	Sleep(1300);
		//	UseGoodsCall(GameGoodsfgrdID);
		//	//Sleep(1300);
		//	Overtime++;
		//	if (Overtime > 100)
		//	{
		//		return -4;
		//	}
		//}
		////吃散蓝
		//Overtime = 0;
		//while (GetGoodsNumber(GameGoodsMagicDrugID) > 1)
		//{
		//	
		//	UseGoodsCall(GameGoodsMagicDrugID);
		//	//Sleep(1300);
		//	Overtime++;
		//	if (Overtime > 100)
		//	{
		//		return -4;
		//	}
		//}
		

		//吃中量红
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsmiddlefgrdID) > 0)
		{
			
			UseGoodsCall(GameGoodsmiddlefgrdID);
			//Sleep(1300);
			Overtime++;
			if (Overtime > 100)
			{
				return -4;
			}
		}

		//吃中量蓝
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsmiddleMagicID) > 0)
		{
			
			UseGoodsCall(GameGoodsmiddleMagicID);
			//Sleep(1300);
			Overtime++;
			if (Overtime > 100)
			{
				return -4;
			}
		}

		//吃中量太阳水
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsSunWaterID) > 0)
		{
			
			UseGoodsCall(GameGoodsSunWaterID);
			//Sleep(1300);
			Overtime++;
			if (Overtime > 100)
			{
				return -4;
			}
		}
	}


	Overtime = 0;
	while (GetObjAndClick(10, 10, 1102, 729, "回收", "DEDEDE-020202") < 0)
	{
		//打开背包
		KeyCall(0x78);
		Sleep(1000);
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}

	}


	Overtime = 0;
	while (GetObjAndClick(10, 10, 402, 186, "全部回收", "FFFF00-000000") < 0)
	{
		Sleep(1000);

		if (GetObjAndClick(10, 10, 402, 186, "没可回收的装备", "FFFFFF-000000", 3, 3, -1) >= 0)
		{
			//清理窗口
			TabPage1.LogOut(L"没有物品可回收");
			pDM->KeyPress(27);
			Sleep(500);
			pDM->KeyPress(27);
			return -2;
		}


		if (GetObjAndClick(10, 10, 1102, 729, "开通特权", "DEDEDE-020202", 3, 3, -1) >= 0)
		{
			//清理窗口

			TabPage1.LogOut(L"没有开通特权");
			Sleep(1000);
			pDM->KeyPress(27);
			Sleep(1000);
			pDM->KeyPress(27);
			return -3;
		}

		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}
	}


	Overtime = 0;
	while (GetObjAndClick(10, 10, 1102, 729, "回收", "DEDEDE-020202",3,3,-1) > 0)
	{
		//关闭背包
		KeyCall(0x78);
		Sleep(1000);
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}

	}

	//清理窗口
	pDM->KeyPress(27);
	Sleep(200);
	pDM->KeyPress(27);


	return 1;
}

int ResolveNpcFun()
{
	DWORD Overtime = 0;
	int state=TabPage2.m_cBackResolveStart.GetCheck();
	int state2 = TabPage3.m_cAdamasEquipment.GetCheck();

	if (state==BST_UNCHECKED&& state2 == BST_UNCHECKED)
	{
		return 0;
	}
	//走到Npc附近坐标
	if (FindWay(338, 335) < 0)
	{
		return -1;
	}
	//隐藏人物
	HideUseAndMaster(1);
	int ret=ChoseAdamasEquip();
	if (ret==0)
	{
		return 0;
	}


	Sleep(1000);
	//清理窗口
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(1000);
	//隐藏人物
	HideUseAndMaster(1);
	//过去分解
	Overtime = 0;
	if (FindWay(338, 335) < 0)
	{
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}
	}
	Sleep(1000);
	Overtime = 0;
	while (GetObjAndClick(10, 10, 402, 186, "装备分解", "FFFF00-000000", 3, 3, -1) < 0)
	{
		GetObjAndClick(52, 374, 347, 593, "金石巧匠", "00FF00-000000", 24, -31);
		FindWay(338, 335);
		Sleep(2000);
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}
	}

	while (GetObjAndClick(10, 10, 402, 186, "装备分解", "FFFF00-000000") < 0)
	{
		//点击分解npc
		moveTo(69, 494);
		Sleep(1000);
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}
	}
	
	Overtime = 0;
	//判断背包格子是否足够  不然会卡死
	int Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
	DWORD PackResidue = GameGoodsMaxNumber - Ret;
	Ret = 0;
	DWORD Times = 0;
	if (PackResidue>=2)
	{
		while(true)
		{
			Ret = ChoseGoodsDisintegrateCall();
			if (Ret==1)
			{
				Sleep(1500);
				while (GetObjAndClick(10, 10, 1102, 729, "分解可获得", "DEDEDE-020202", 192, 88) < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}
			}
			else if (Ret==2)
			{
				break;
			}
			else if (Ret==0)
			{
				TabPage1.LogOut("分解出错无法分解");
				Times++;
			}
			if (Times>=3)
			{
				break;
			}
			
		}
	}
	else
	{
		TabPage1.LogOut("背包位置不足无法分解");
		return 1;
	}

	//清理窗口
	Sleep(1000);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	return 2;
	

}

int FindWay(int x, int y)
{
	WayfindingCall(x, y);

	DWORD tmp_x = 0, tmp_y = 0;
	GetUserX_YCoord();
	tmp_x = GameUser.x;
	tmp_y = GameUser.y;
	DWORD times = 0;
	while ((GameUser.x != x || GameUser.y != y))
	{
		if (WaitBackFlag)
		{
			moveTo(551, 324, 3);
			moveTo(551, 324, 3);
			return 0;
		}
		else
		{
			tmp_x = GameUser.x;
			tmp_y = GameUser.y;
			Sleep(1000);
		}

		GetUserX_YCoord();
		if (GameUser.x == tmp_x && GameUser.y == tmp_y)
		{
			if (WaitBackFlag)
			{
				moveTo(551, 324, 3);
				moveTo(551, 324, 3);
				return 0;
			}
			else
			{
				WayfindingCall(x, y);
			}
			Sleep(1000);
			times++;
			if (times > 4)
			{
				WayfindingCall(GameUser.x, GameUser.y);
				moveTo(551, 324, 3);
				moveTo(551, 324, 3);
				return -1;
			}
		}
		else
		{
			WayfindingCall(x, y);
		}

	}
	return 1;

}

int CheckWay(int Npc)
{

	//宫殿迷宫Npc
	if (Npc==1)
	{
		GetUserX_YCoord();

		while (GameUser.x >21)
		{
			if (WaitBackFlag)
			{
				break;
			}
			moveTo(484, 318);
			GetUserX_YCoord();
			if (GameUser.x <= 25)
			{
				break;
			}
			
		}

		while (GameUser.x < 21)
		{
			if (WaitBackFlag)
			{
				break;
			}
			moveTo(632, 327);
			GetUserX_YCoord();
			if (GameUser.x >= 19)
			{
				break;
			}
		}

		while (GameUser.y < 21)
		{
			if (WaitBackFlag)
			{
				break;
			}
			moveTo(552, 366);
			GetUserX_YCoord();
			if (GameUser.x >= 16)
			{
				break;
			}
		}

		while (GameUser.y > 21)
		{
			if (WaitBackFlag)
			{
				break;
			}
			moveTo(552, 215);
			GetUserX_YCoord();
			if (GameUser.y <= 23)
			{
				break;
			}
		}
		return Npc;

	}
	return -1;
}

int On_lineGoX_Y()
{
	int state = TabPage3.m_cFitXY.GetCheck();
	CString x_y;
	wchar_t x[0x4]{};
	wchar_t y[0x4]{};
	int iX = 0;
	int iY = 0;
	if (state==BST_CHECKED)
	{
		TabPage3.m_eFitXY.GetWindowTextW(x_y);
		//memcpy(x, x_y.GetBuffer(), x_y.Find(L":")*2);
		iX = _wtoi(x_y);
		TabPage3.m_EDIT_Y.GetWindowTextW(x_y);
		//memcpy(y, x_y.GetBuffer()+ ((x_y.Find(L":")+1)),(x_y.GetLength()*2)- (x_y.Find(L":") * 2));
		iY = _wtoi(x_y);
		pDM->KeyPress(27);
		Sleep(200);
		
		FindWay(iX, iY);
	}
	return 0;

}

int GetOn_lineState()
{
	if (GameUser.Infor==NULL)
	{
		return -1;
	}
	if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.Infor + On_LineStateOffset), &GameUser.On_line, sizeof(GameUser.On_line), &Size) != 0)
	{
		if (GameUser.On_line == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}

	}
	
}

int SetOn_lineState(bool State)
{
	DWORD Online = State;
	if (GameUser.Infor == NULL)
	{
		return -1;
	}
	if (GameBase == NULL)
	{
		return -1;
	}
	if (WriteProcessMemory(hMainProcess, PDWORD(GameUser.Infor + On_LineStateOffset), &Online, sizeof(Online), &Size) != 0)
	{
		if (WriteProcessMemory(hMainProcess, PDWORD(GameBase + GameOn_lineTirps), &Online, sizeof(Online), &Size) != 0)
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

int OpenAuto()
{
	int ret = -1;
	GetUserInMap();
	//如果是在安全区结束挂机检测
	if (StrCmpW(L"盟重省", GameUser.InMap) == 0 || StrCmpW(L"苍月岛", GameUser.InMap) == 0 || StrCmpW(L"盟重省(跨服)", GameUser.InMap) == 0)
	{
	}
	else
	{
		ret = GetOn_lineState();
		if (ret == 0)
		{
			pDM->KeyDown(17);
			Sleep(100);
			pDM->KeyPress(90);
			Sleep(100);
			pDM->KeyUp(17);
			return 1;
		}
		else if (ret==1)
		{
			return 1;
		}

	}
	
	return 0;
}

int SetAutoFix(bool State)
{
	DWORD Fix = State;
	if (GameBase == NULL)
	{
		return -1;
	}

	if (WriteProcessMemory(hMainProcess, PDWORD(GameBase + GameAutoFixOffset), &Fix, sizeof(Fix), &Size) != 0)
	{
		return 1;
	}
	
	return 0;
	
}

int SetAutoPick(bool State)
{
	DWORD ret = 0;
	DWORD Pick = State;
	if (GameBase == NULL)
	{
		return -1;
	}
	if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + GameAutoPickOffset), &ret, sizeof(ret), &Size) != 0)
	{
		if (WriteProcessMemory(hMainProcess, PDWORD(GameBase + GameAutoPickOffset), &Pick, sizeof(Pick), &Size) != 0)
		{
			return ret;
		}
	}
	

	return -1;

}

int UseBack()
{

	//DWORD state = TabPage3.m_cBackWait.GetCheck();

	GetUserInMap();
	if (wcscmp(L"跨服龙城", GameUser.InMap) == 0)
	{

		if (isNow==false)
		{
			if (GetGoodsNumber(GameGoodsBackRollID) > 0)
			{
				//使用回城卷
				UseGoodsCall(GameGoodsBackRollID);
				moveTo(551, 324, 3);
				moveTo(551, 324, 3);
				Sleep(700);
				if (IsSafetyArea() == 2)
				{
					return 1;
				}

			}
		}

		for (size_t i = 0; i < 32; i++)
		{

			if (wcsstr(wOn_lineMap[i], L"跨服龙城") != NULL)
			{
				if (GetGoodsNumber(GameGoodsBackRollID) > 0)
				{

					//使用回城卷

					UseGoodsCall(GameGoodsBackRollID);
					moveTo(551, 324, 3);
					moveTo(551, 324, 3);
					Sleep(700);
					if (IsSafetyArea() == 2)
					{
						return 1;
					}

				}
			}
		}
		
		
		if (GetGoodsNumber(GameGoodsPresentBackRockID) > 0)
		{
			//使用回城石(赠)
			UseGoodsCall(GameGoodsPresentBackRockID);
			moveTo(551, 324, 3);
			moveTo(551, 324, 3);
			Sleep(700);
			if (IsSafetyArea() == 2)
			{
				return 2;
			}

		}

		if (GetGoodsNumber(GameGoodsBackRockID) >0)
		{
			//使用回城石
			UseGoodsCall(GameGoodsBackRockID);
			Sleep(500);
			moveTo(551, 324, 3);
			
			moveTo(551, 324, 3);
			Sleep(700);
			if (IsSafetyArea() == 2)
			{
				return 2;
			}

		}

	}

	if (wcscmp(L"苍月岛", GameUser.InMap) != 0)
	{
		if (GetGoodsNumber(GameGoodsBackRollID) > 0)
		{

			//使用回城卷
			UseGoodsCall(GameGoodsBackRollID);
		
			if (WaitBackFlag)
			{
				Sleep(600);
				moveTo(551, 324, 3);
				moveTo(551, 324, 3);
			}
			Sleep(700);
			GetUserInMap();
			if (wcscmp(L"盟重省", GameUser.InMap) != 0)
			{
				TabPage1.LogOut("没有回城卷挂机回不去尝试别的回城道具");

			}
			else
			{
				return 2;
			}

		}
	}



	if (GetGoodsNumber(GameGoodsPresentBackRockID) > 0)
	{
		//使用回城石(赠)
		UseGoodsCall(GameGoodsPresentBackRockID);
		moveTo(551, 324, 3);
		moveTo(551, 324, 3);
		Sleep(700);
		GetUserInMap();
		if (wcscmp(L"盟重省", GameUser.InMap) == 0)
		{
			return 2;

		}

	}

	if (GetGoodsNumber(GameGoodsBackRockID) >0)
	{
		//使用回城石
		UseGoodsCall(GameGoodsBackRockID);
		moveTo(551, 324, 3);
		moveTo(551, 324, 3);
		Sleep(700);
		GetUserInMap();
		if (wcscmp(L"盟重省", GameUser.InMap) == 0)
		{
			return 2;

		}

	}

	Sleep(1000);
	GetUserInMap();
	if (wcscmp(L"盟重省", GameUser.InMap) != 0)
	{
		TabPage1.LogOut("没有回城石挂机回不去尝试别的回城道具");

	}
	else
	{
		return 2;
	}

	Shopping(1);

	if (GetGoodsNumber(GameGoodsBackRockID) > 0)
	{
		//使用回城石
		UseGoodsCall(GameGoodsBackRockID);
		moveTo(551, 324, 3);
		moveTo(551, 324, 3);
		Sleep(700);
		GetUserInMap();
		if (wcscmp(L"盟重省", GameUser.InMap) == 0)
		{
			return 2;

		}

	}
	Sleep(2000);
	GetUserInMap();
	if (wcscmp(L"盟重省", GameUser.InMap) != 0)
	{
		TabPage1.LogOut("没有回城道具");
		return -1;

	}
	else
	{
		TabPage1.LogOut("商城购买的回城道具回城");
		return 2;
	}
	
	
}

int DreamlandBuyDrug(int BuyRedNumb, int BuyBlueNumb, int iRedDurgNumb, int iBlueDurgNumb)
{
	DWORD Overtime = 0;
	
	Sleep(300);
	while (bAutoFlag && EndFlag && WaitBackFlag == false)
	{

		if (GetObjAndClick(10, 10, 1102, 729, "幻境药师", "D2D0C4-000000", 3, 3, -1) >= 0|| GetObjAndClick(10, 10, 1102, 729, "幻境药师", "00ff00-000000", 3, 3, -1) >= 0|| GetObjAndClick(10, 10, 1102, 729, "幻境小贩", "D2D0C4-000000", 3, 3, -1) >= 0|| GetObjAndClick(10, 10, 1102, 729, "幻境小贩", "00ff00-000000", 3, 3, -1) >= 0)
		{
			
		}
		else 
		{
			//打开地图
			KeyCall(77);
			Sleep(1000);
		}

	if (GetObjAndClick(10, 10, 1102, 729, "幻境小贩", "D2D0C4-000000", 3, 3, -1) >= 0)
	{
		while (GetObjAndClick(10, 10, 1102, 729, "幻境小贩", "D2D0C4-000000") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
	}
	else if(GetObjAndClick(10, 10, 1102, 729, "幻境小贩", "00ff00-000000", 3, 3, -1) >= 0)
	{
		while (GetObjAndClick(10, 10, 1102, 729, "幻境小贩", "00ff00-000000") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
	}
	else if (GetObjAndClick(10, 10, 1102, 729, "幻境药师", "00ff00-000000", 3, 3, -1) >= 0)
	{
		while (GetObjAndClick(10, 10, 1102, 729, "幻境药师", "00ff00-000000") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
	}
	else
	{
		while (GetObjAndClick(10, 10, 1102, 729, "幻境药师", "D2D0C4-000000") < 0)
		{

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
	}


	//Sleep(1000);
	//while (GetObjAndClick(10, 10, 1102, 729, "幻境药师", "D2D0C4-000000", 3, 3, -1) >= 0|| GetObjAndClick(10, 10, 1102, 729, "幻境小贩", "D2D0C4-000000", 3, 3, -1) >= 0|| GetObjAndClick(10, 10, 1102, 729, "幻境药师", "00ff00-000000", 3, 3, -1) >= 0|| GetObjAndClick(10, 10, 1102, 729, "幻境小贩", "00ff00-000000", 3, 3, -1) >= 0)
	//{
	//	//关闭地图
	//	KeyCall(77);
	//	Sleep(1000);
	//	Overtime++;
	//	if (Overtime > 10)
	//	{
	//		return -1;
	//	}

	//}


	

		if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.ObjAddress + UseUseNowStateOffset), &GameUser.State, sizeof(GameUser.State), &Size) != 0)
		{
			if (GameUser.State==6)
			{
				if (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000", 3, 3, -1) >= 0)
				{
					break;
				}
				Sleep(3000);
				if (ReadProcessMemory(hMainProcess, PDWORD(GameUser.ObjAddress + UseUseNowStateOffset), &GameUser.State, sizeof(GameUser.State), &Size) != 0)
				{
					if (GameUser.State == 6)
					{
						if (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000", 3, 3, -1) >= 0)
						{
							break;
						}
						//清理界面
						pDM->KeyPress(27);
						Sleep(500);
						pDM->KeyPress(27);
						Sleep(500);
						pDM->KeyPress(27);
						Sleep(500);
						pDM->KeyPress(27);
						Sleep(1000);
						//SetOn_lineState(1);
						
						
						return -6;
					}
					else
					{
						if (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000", 3, 3, -1) >= 0)
						{
							break;
						}
					}
				}
			}
		}
		Sleep(5000);
	}

	Sleep(500);
	Overtime = 0;
	while (GetObjAndClick(10, 68, 402, 186, "购买绑金", "FFFF00-000000") < 0)
	{

		Sleep(1000);
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}
	}

	if (BuyRedNumb > 0)
	{

		for (size_t i = 0; i < BuyRedNumb; i++)
		{
			Overtime = 0;
			while (GetObjAndClick(51, 174, 445, 512, "超级金创药", "DEDEDE-020202", 5, 3, 1) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			moveTo(367, 303);
			Sleep(300);
			moveTo(367, 303);

		}

		int PackRedDurgNumb = GetGoodsNumber(GameGoodsPresentSuperfgrdID);
		PackRedDurgNumb += GetGoodsNumber(105);
		if (iRedDurgNumb > PackRedDurgNumb)
		{
			for (size_t i = 0; i < iRedDurgNumb - PackRedDurgNumb; i++)
			{
				Overtime = 0;
				while (GetObjAndClick(51, 174, 445, 512, "超级金创药", "DEDEDE-020202", 5, 3, 1) < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}

				moveTo(367, 303);
				Sleep(300);
				moveTo(367, 303);
			}
		}



	}

	if (BuyBlueNumb > 0)
	{
		Overtime = 0;
		while (GetObjAndClick(85, 425, 406, 479, "下一页", "DEDEDE-020202") < 0)
		{

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}

		for (size_t i = 0; i < BuyBlueNumb; i++)
		{
			Overtime = 0;
			while (GetObjAndClick(51, 174, 445, 512, "超级魔法药", "DEDEDE-020202", 5, 3, 1) < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			moveTo(367, 303);
			Sleep(300);
			moveTo(367, 303);
		}

		int PackBlueDurgNumb = GetGoodsNumber(GameGoodsPresentSuperMagicID);
		PackBlueDurgNumb += GetGoodsNumber(115);

		if (iBlueDurgNumb > PackBlueDurgNumb)
		{
			for (size_t i = 0; i < iBlueDurgNumb - PackBlueDurgNumb; i++)
			{
				Overtime = 0;
				while (GetObjAndClick(51, 174, 445, 512, "超级魔法药", "DEDEDE-020202", 5, 3, 1) < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}

				moveTo(367, 303);
				Sleep(300);
				moveTo(367, 303);
			}
		}
	}

	


	//清理界面
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	
	

}

int MoveGameMouse(int x, int y)
{
	if (GameUser.Infor == 0)
	{
		return 0;
	}
	DWORD X = x;
	DWORD Y = y;
	
	if (WriteProcessMemory(hMainProcess, PDWORD(GameUser.Infor + GameMouseXOffset), &X, sizeof(X), &Size) != 0)
	{
		if (WriteProcessMemory(hMainProcess, PDWORD(GameUser.Infor + GameMouseYOffset), &Y, sizeof(Y), &Size) != 0)
		{
			return 1;
		}
	}

	return 0;
}

int IsObjInScope(int x,int y, int Scope)
{

	DWORD Ret = 0;
	GetUserX_YCoord();

	if (WaitBackFlag)
	{
		return 0;
	}
	if (((x > (GameUser.x - Scope)) && (x < (GameUser.x + Scope))) && ((y > (GameUser.y - Scope)) && (y < (GameUser.y + Scope))))
	{
		return 1;
	}

	return 0;

}

int Scope = -1;	//坐标
int PoisonMonsterFlag = true;

//挂机检测
DWORD WINAPI AutoOn_lineCheck(_In_ LPVOID lpParameter)
{
	DWORD Overtime = 0;
	DWORD StopNumb = 0;
	DWORD Count = 0;
	CString Tmp;
	int State=TabPage3.m_cAdamasEquipment.GetCheck();
	int State2 = TabPage3.m_cJoinRand.GetCheck();
	//进图随机
	if (State2 == BST_CHECKED)
	{
		//使用随机石
		UseGoodsCall(GameGoodsRandRockID);
	}
	//无药回城
	int iState = TabPage2.m_cNoRedStart.GetCheck();
	int iState2 = TabPage2.m_cNoBlueStart.GetCheck();
	//金刚石装备数量检测
	if (State == BST_CHECKED)
	{
		CStringW str;
		TabPage3.m_eAdamasEquipment.GetWindowTextW(str);
		StopNumb = _ttoi(str);
	}
	
	DWORD Numb = 0;
	//回收相关
	int EquipNumb = 0;
	int iState4 = TabPage3.m_cKnapsackResidue.GetCheck();
	if (iState4== BST_CHECKED)
	{
		TabPage3.m_eKnapsackResidue.GetWindowTextW(Tmp);
		EquipNumb = _ttoi(Tmp);
	}
	int GoodsNumb = 0;
	
	int ret = 0;
	//背包剩余回城

	int SurplusNumb = 0;
	int iState3 = TabPage2.m_cPackSurplus.GetCheck();
	if (iState3 == BST_CHECKED)
	{
		TabPage2.m_ePackSurplus.GetWindowTextW(Tmp);
		SurplusNumb = _ttoi(Tmp);
	}
	//长时间无怪随机
	int TimeNoMasterRandState = TabPage3.m_cTimeNoMasterRand.GetCheck();
	int TimeNoMasterRand = 0;
	
	//范围无怪随机
	int AroundNoMasterRandState = TabPage3.m_cRoundNoMasterRand.GetCheck();
	int  AroundNoMasterRand = 0;
	
	

	//雪霜和疗伤药打捆
	int iState6 = TabPage2.m_cCureStart.GetCheck();
	int iState5 = TabPage2.m_cSnowStart.GetCheck();


	//商城购买物品
	int iState7 = TabPage2.m_cFixWaterStart.GetCheck();
	int iState8 = TabPage2.m_cRandStart.GetCheck();
	int iState9 = TabPage2.m_cBackRockStart.GetCheck();
	int time = 0;
	int time2 = 0;
	// 定点挂机
	int iState10 = TabPage3.m_cFitXY.GetCheck();
	int time3 = 0;
	bool Open = true;
	TabPage3.m_eXYTime.GetWindowTextW(Tmp);
	time3 = _wtoi(Tmp);
	int tmpdMinute = 0;
	//自动打防
	int istate = 0;
	int time4 = 0;
	CString cTime;
	int iTime = 0;
	if (GameUser.Occupation > 0x300)
	{
		istate = TabPage2.m_cAutoDefend.GetCheck();
		
		TabPage2.m_eTimes.GetWindowTextW(cTime);
		iTime = _wtoi(cTime);
		iTime = iTime / 1.5;
		if (istate==BST_CHECKED)
		{
			AutoDefense();
		}
		
	}

	//幻境买药挂机
	//判断是否需要买药
	CStringW DurgNumb;
	int iLowRedDurgNumb = 0;
	int iNeedRedDurgNumb = 0;
	int PackRedDurgNumb = 0;

	int iLowBlueDurgNumb = 0;
	int iNeedBlueDurgNumb = 0;
	int PackBlueDurgNumb = 0;
	//被堵打怪时长
	DWORD Times5 = 0;
	bool Fitflag = false;
	//法师放火
	int iState11 = TabPage2.m_cFire.GetCheck();
	int time5 = 0;
	TabPage2.m_eFire.GetWindowTextW(Tmp);
	time5 = _wtoi(Tmp);
	int FireTime = time5;
	int FirX = 318;
	int FirY = 116;
	int TmpFirY = 116;
	//int TmpFirX = 318;
	//道士毒怪
	Scope = -1;
	int iState12 = BST_UNCHECKED;
	//iState12 = TabPage2.m_cPoisonMonster.GetCheck();
	if (iState12 == BST_CHECKED)
	{
		Sleep(2000);
		Scope = FindNearWay();
	}

	//进图先去定点
	if (iState10 == BST_CHECKED)
	{
		SetOn_lineState(0);
		On_lineGoX_Y();
	}

	if (iState11 == BST_UNCHECKED && iState12 == BST_UNCHECKED)
	{
		//if (SetOn_lineState(1) == 0)
		while (OpenAuto() == 0)
		{
			Sleep(500);
			Overtime++;
			if (Overtime > 10)
			{
				TabPage1.LogOut("开始挂机加检测失败");
				return 0;
			}
		};
		
		TabPage1.LogOut("开始挂机");
	}
	

	while (bAutoFlag && EndFlag)
	{
		if (WaitBackFlag)
		{
			
			SetOn_lineState(0);
			return 20;
		}

		GetUserInMap();
		//如果是在安全区结束挂机检测
		if (StrCmpW(L"盟重省", GameUser.InMap) == 0 || StrCmpW(L"苍月岛", GameUser.InMap) == 0 || StrCmpW(L"盟重省(跨服)", GameUser.InMap) == 0)
		{
			
			SetOn_lineState(0);

			return 7;

		}


		if (StrStrW(GameUser.InMap, L"幻境") != NULL&& Fitflag==false)
		{
			TabPage2.m_eRedNumb.GetWindowTextW(DurgNumb);
			//红药最低数量
			iLowRedDurgNumb = _wtoi(DurgNumb.Mid(0, DurgNumb.Find(L"-")));
			//红药购买数量
			iNeedRedDurgNumb = _wtoi(DurgNumb.Mid(DurgNumb.Find(L"-") + 1));
			//背包红药已经有的数量
			PackRedDurgNumb = GetGoodsNumber(GameGoodsPresentSuperfgrdID);
			PackRedDurgNumb += GetGoodsNumber(GameGoodsSuperfgrdID);
			DurgNumb.Empty();
			TabPage2.m_eBlueNumb.GetWindowTextW(DurgNumb);
			//蓝药最低数量
			iLowBlueDurgNumb = _wtoi(DurgNumb.Mid(0, DurgNumb.Find(L"-")));
			//蓝药购买数量
			iNeedBlueDurgNumb = _wtoi(DurgNumb.Mid(DurgNumb.Find(L"-") + 1));
			//背包蓝药已经有的数量
			PackBlueDurgNumb = GetGoodsNumber(GameGoodsPresentSuperMagicID);
			PackBlueDurgNumb += GetGoodsNumber(GameGoodsSuperMagicID);
			if (iLowBlueDurgNumb> PackBlueDurgNumb|| iLowRedDurgNumb> PackRedDurgNumb)
			{
				if (DreamlandBuyDrug(iNeedRedDurgNumb - PackRedDurgNumb, iNeedBlueDurgNumb - PackBlueDurgNumb, iNeedRedDurgNumb, iNeedBlueDurgNumb) == -6)
				{
					Fitflag = true;
					TabPage1.LogOut("幻境买药路上被堵住继续挂机");
				}
			}

		}
		




		//如果挂机停止了
		if (GetOn_lineState() == 0)
		{
			GetUserInMap();
			//如果是在安全区结束挂机检测
			if (StrCmpW(L"盟重省", GameUser.InMap) == 0 || StrCmpW(L"苍月岛", GameUser.InMap) == 0|| StrCmpW(L"盟重省(跨服)", GameUser.InMap) == 0)
			{
				SetOn_lineState(0);

				return 7;
			}
			else
			{
				//否则继续挂机
				if (iState11 == BST_UNCHECKED && iState12 == BST_UNCHECKED && WaitBackFlag == false)
				{
					while (OpenAuto() == 0)
					{
						Sleep(500);
						Overtime++;
						if (Overtime > 10)
						{
							TabPage1.LogOut("继续挂机失败");
							return 0;
						}
					};
				}
			}

		}




		//定点挂机检测
		if (iState10 == BST_CHECKED && iState12 == BST_UNCHECKED && WaitBackFlag == false)
		{
			if (dMinute % time3 == 0 && Open )
			{
				tmpdMinute = dMinute;
				SetOn_lineState(0);
				On_lineGoX_Y();
				if (iState11 == BST_UNCHECKED && iState12 == BST_UNCHECKED)
				{
					while (OpenAuto() == 0 && WaitBackFlag == false)
					{
						Sleep(500);
						Overtime++;
						if (Overtime > 10)
						{
							TabPage1.LogOut("定点挂机开始挂机失败");
							return 0;
						}
					};
				}
				Open = false;
			}
			else
			{
				if (dMinute % time3 != 0)
				{
					Open = true;
				}
				else if (time3==1&&dMinute>tmpdMinute)
				{
					Open = true;
				}
				
			}
		} 
		//道士毒怪 60,81  84,52   75,15   48,19     23,42   21,69   31,81
		if (Scope >= 0&&WaitBackFlag == false)
		{
			switch (Scope)
			{
			case 1:
			{
				if (IsObjInScope(60, 81, 2))
				{
					Scope = 2;
				}
				break;
			}
			case 2:
			{
				if (IsObjInScope(84, 52, 2))
				{
					Scope = 3;
				}
				break;
			}
			case 3:
			{
				if (IsObjInScope(75, 15, 2))
				{
					Scope = 4;
				}
				break;
			}
			case 4:
			{
				if (IsObjInScope(48, 19, 2))
				{
					Scope = 5;
				}
				break;
			}
			case 5:
			{
				if (IsObjInScope(23, 42, 2))
				{
					Scope = 6;
				}
				break;
			}
			case 6:
			{
				if (IsObjInScope(21, 69, 2))
				{
					Scope = 7;
				}
				break;
			}
			case 7:
			{
				if (IsObjInScope(31, 81, 2))
				{
					Scope = 1;
				}
				break;
			}
			default:
				break;
			}
			SetOn_lineState(0);
		
			//59,67    76,49     54,29    32,47
			if (PoisonMonster(Scope) == 3 && WaitBackFlag == false)
			{
				Scope = 0;
			}

			switch (Scope)
			{
			case 1:
			{
				if (IsObjInScope(60, 81, 2))
				{
					Scope = 2;
				}
				break;
			}
			case 2:
			{
				if (IsObjInScope(84, 52, 2))
				{
					Scope = 3;
				}
				break;
			}
			case 3:
			{
				if (IsObjInScope(75, 15, 2))
				{
					Scope = 4;
				}
				break;
			}
			case 4:
			{
				if (IsObjInScope(48, 19, 2))
				{
					Scope = 5;
				}
				break;
			}
			case 5:
			{
				if (IsObjInScope(23, 42, 2))
				{
					Scope = 6;
				}
				break;
			}
			case 6:
			{
				if (IsObjInScope(21, 69, 2))
				{
					Scope = 7;
				}
				break;
			}
			case 7:
			{
				if (IsObjInScope(31, 81, 2))
				{
					Scope = 1;
				}
				break;
			}
			default:
				break;
			}
			SetOn_lineState(0);
		}

		//法师放火
		if (iState11 == BST_CHECKED && WaitBackFlag == false)
		{
			FireTime = time5;
			if (FireTime!=0)
			{
				FirY = TmpFirY;
				for (size_t x = 0; x < 7; x++)
				{
					for (size_t i = 0; i < 5; i++)
					{
						if (x % 2 != 0)
						{
							MoveGameMouse(FirX + 57 + i * 114, FirY);
							Sleep(50);
							UseSkillCall(0xCB);
						}
						else
						{
							MoveGameMouse(FirX + i * 114, FirY);
							Sleep(50);
							UseSkillCall(0xCB);
						}

						if (FireTime<=0)
						{
							break;
						}
						Sleep(2250);
						FireTime -= 2;
						if (WaitBackFlag)
						{
							return 0;
						}
					}
					if (x % 2 != 0)
					{
						FirY += 43;
					}
					else
					{
						FirY += 86;
					}

					if (FireTime <= 0)
					{
						break;
					}
					if (WaitBackFlag)
					{
						return 0;
					}
					
				}

				Sleep(500);
			}
			
		}



		//雪霜打捆检测
		if (iState5 == BST_CHECKED && iState12 == BST_UNCHECKED)
		{
			if (GetGoodsNumber(GameGoodsSnowRopeID) >= 1)
			{
				if (GetGoodsNumber(GameGoodsSnowID) >= 6)
				{
					Sleep(800);
					UseGoodsCall(GameGoodsSnowRopeID);
					TabPage1.LogOut("使用了雪霜打捆");

				}
				
			}
			if (GetGoodsNumber(GameGoodsSnowRopeID) == 0)
			{

				Shopping();

			}

		}
		//疗伤药打捆检测
		if (iState6 == BST_CHECKED && iState12 == BST_UNCHECKED)
		{
			if (GetGoodsNumber(GameGoodsCureRopeID) >= 1)
			{
				if (GetGoodsNumber(GameGoodsCureID) >= 6)
				{
					Sleep(800);
					UseGoodsCall(GameGoodsCureRopeID);
					TabPage1.LogOut("使用了疗伤药打捆");
				}
				
			}

			if (GetGoodsNumber(GameGoodsCureRopeID) == 0)
			{
				Shopping();

			}
			

		}
		//修复神水检测
		if (iState7 == BST_CHECKED && WaitBackFlag == false)
		{

			//超过20分钟使用一次修复神水
			if (GetGoodsNumber(GameGoodsFixWaterID) > 0)
			{
				time = dMinute / 20;
				if (time > time2)
				{
					Sleep(800);
					UseGoodsCall(GameGoodsFixWaterID);
					TabPage1.LogOut("使用了修复神水");
					time2 = time;
				}
			}

			if (GetGoodsNumber(GameGoodsFixWaterID) == 0)
			{
				Shopping();
			}
			
			
		}
		//随机石检测
		if (iState8 == BST_CHECKED && WaitBackFlag == false)
		{

			if (GetGoodsNumber(GameGoodsRandRockID) == 0)
			{
				Shopping();
			}
			
		}
		//回城石数量检测
		if (iState9 == BST_CHECKED && WaitBackFlag == false)
		{

			if (GetGoodsNumber(GameGoodsBackRockID) == 0)
			{
				Shopping();
			}
			
		}
		//金刚石到达一定数量回城
		if (State == BST_CHECKED && iState12 == BST_UNCHECKED)
		{
			//获取金刚石装备数量
			ChoseAdamasEquip(&Numb);
			//如果大于设置的金刚石装备就停止挂机
			if (Numb >= StopNumb)
			{

				//使用回城
				UseBack();
				Sleep(1500);
				SetOn_lineState(0);

				return 3;

			}
		}
		//无红回城
		if (iState == BST_CHECKED)
		{
			GetUserInMap();
			if (StrStrW(GameUser.InMap, L"幻境") == NULL)
			{
				if (GetGoodsNumber(GameGoodsSuperfgrdID) == 0 && GetGoodsNumber(GameGoodsPresentSuperfgrdID) == 0)
				{

					//使用回城					
					UseBack();
					Sleep(1500);
					SetOn_lineState(0);
					return 4;
				}
			}
		}

		//无蓝回城
		if (iState2 == BST_CHECKED)
		{
			GetUserInMap();
			if (StrStrW(GameUser.InMap, L"幻境") == NULL)
			{
				if (GetGoodsNumber(GameGoodsSuperMagicID) == 0 && GetGoodsNumber(GameGoodsPresentSuperMagicID) == 0)
				{

					//使用回城					
					UseBack();
					Sleep(1500);
					SetOn_lineState(0);
					return 5;
				}
			}
			

		}
		

		//背包剩余格数回收
		if (iState4 == BST_CHECKED && WaitBackFlag == false)
		{
			if (NoEquipCount < 10)
			{
				//遍历背包格子是否小于背包数量
				GoodsNumb = InitKnapsackInfor(GameBase);
				if (GoodsNumb > 0)
				{
					if ((GameGoodsMaxNumber - GoodsNumb) <= EquipNumb)
					{

						ret = RecycleNpcFunc();
						if (ret == -2)
						{
							
						}
						else if (ret == 1)
						{
							NoEquipCount = 0;
						}
						else if (ret == -3)
						{
							TabPage3.m_cKnapsackResidue.SetCheck(BST_UNCHECKED);
							NoEquipCount = 10;
						}
						else
						{
							TabPage1.LogOut("回收失败");
						}
					}
				}
			}
		}


		//背包剩余格数回城
		if (iState3 == BST_CHECKED)
		{
			//遍历背包格子是否小于背包数量
			GoodsNumb = InitKnapsackInfor(GameBase);
			if (GoodsNumb > 0)
			{
				if ((GameGoodsMaxNumber - GoodsNumb) <= SurplusNumb)
				{

					//使用回城
					
					UseBack();
					Sleep(1000);
					SetOn_lineState(0);
					Sleep(500);
					return 6;
				}
			}


		}

		if (MapBug)
		{
			GetUserX_YCoord();
			if ((GameUser.x > 79 && GameUser.x < 86) && (GameUser.y > 80 && GameUser.y < 90))
			{
				moveTo(402, 186);
				moveTo(402, 186);
				moveTo(402, 186);
			}
		}

		//长时间没有怪物使用随机石
		if (TimeNoMasterRandState == BST_CHECKED && NowEliminateDemonFlag == false && iState12 == BST_UNCHECKED)
		{
			TabPage3.m_eTimeNoMasterRand.GetWindowTextW(Tmp);
			TimeNoMasterRand = _wtoi(Tmp);
			if (TimeNoMasterRand > 0)
			{
				if (NoExpMinute(TimeNoMasterRand) == 0)
				{
					//使用随机石
					
					UseGoodsCall(GameGoodsRandRockID);
					TabPage1.LogOut("时长无怪随机");
				}

			}
		}
		//周围没有怪物使用随机石

		if (AroundNoMasterRandState == BST_CHECKED && NowEliminateDemonFlag == false && iState12 == BST_UNCHECKED)
		{
			TabPage3.m_eRoundNoMasterRand.GetWindowTextW(Tmp);
			AroundNoMasterRand = _wtoi(Tmp);
			if (AroundNoMasterRand > 0)
			{
				if (NoMasterAround(AroundNoMasterRand) == 0)
				{
					//使用随机石
					
					UseGoodsCall(GameGoodsRandRockID);
					TabPage1.LogOut("周围无怪随机");
				}
			}
		}


		//检测是否到点  做任务去

		Count = GetNowGotoMap(GetOn_lineMap());
		if (Count > 0 && iState12 == BST_UNCHECKED && WaitBackFlag == false)
		{
			if (isNow && wOn_lineMap[0][0] != 0)
			{
				size_t i = 0;
				GetUserInMap();
				for (; i < Count; i++)
				{
					//在挂机地图的话
					if (wcsstr(GameUser.InMap, wOn_lineMap[i]) != NULL)
					{
						goto Next;
					}
					else
					{
						if (StrStrW(wOn_lineMap[i], L"活动地图") != NULL || StrStrW(wOn_lineMap[i], L"神秘洞穴"))
						{
							if (wcsstr(GameUser.InMap, L"地图") != NULL|| wcsstr(GameUser.InMap, L"豪华") != NULL || wcsstr(GameUser.InMap, L"合服") != NULL || wcsstr(GameUser.InMap, L"神秘洞穴") != NULL)
							{
								
								goto Next;
							}
						}
					}
				}
				if (NowEliminateDemonFlag)
				{
					goto Next;
				}
				if (WaitBackFlag)
				{
					return 0;
				}
				TabPage1.LogOut("有需要完成的任务");
				if (wcscmp(L"盟重省", GameUser.InMap) != 0)
				{
					//使用回城
					
					UseBack();
				}
				Sleep(1000);
				SetOn_lineState(0);

				return 9;
			}
		}

	Next:
		if (EliminateDemonFlag)
		{

			return 8;
		}
		if (istate==BST_CHECKED)
		{
			if (time4 <= iTime)
			{
				time4++;
			}
			else
			{
				AutoDefense();
				time4 = 0;
			}

		}
		if (Fitflag)
		{
			if(Times5 <= 20)
			Times5++;
			else
			{
				Fitflag = false;
				Times5 = 0;
			}
		}

		if (iState12 == BST_UNCHECKED)
		{
			Sleep(1500);
		}
		else
		{
			Sleep(1000);
		}
		

	}
	
	return 0;
}

int DragonCity(bool Jmp)
{
	DWORD Overtime = 0;
	DWORD time3 = 0;
	CString sTimes;
	DWORD state = TabPage3.m_cBackWait.GetCheck();
	DWORD check = 0;
	//DWORD tmp_x = 0, tmp_y = 0;
	DWORD Threadid = 0;
	HANDLE hThread = NULL;
	DWORD state6 = TabPage4.m_cBindMoney.GetCheck();
	CString cNumb;
	int iNumb = 0;
	long long iNumb2 = 0;
	TabPage4.m_eBindMoney.GetWindowTextW(cNumb);
	iNumb = _ttoi(cNumb);
	while (ChangeMap)
	{
	Next:
		if (state6 == BST_CHECKED)
		{
			if (iNumb > 0)
			{
				iNumb2 = GetUserMoney(0);
				if (iNumb > iNumb2)
				{
					TabPage1.LogOut(L"绑金不足 停止进图,重新启动");
					break;
				}
			}
		}

		GetLocalTime(&sysTime);
		if (sysTime.wHour <12 || sysTime.wDayOfWeek == 1 || sysTime.wDayOfWeek == 3 || sysTime.wDayOfWeek == 5 || sysTime.wDayOfWeek == 7)
		{
			TabPage1.LogOut(L"跨服龙城时间不对");
			break;
		}
		GetUserInMap();
		if (wcscmp(L"盟重省(跨服)", GameUser.InMap) == 0)
		{

			if (Jmp == false)
			{

				if (TrampNpcFunc() < 0)
				{
					TabPage1.LogOut(L"跨服主城流浪汉补给出错");
					return -1;
				}

				Sleep(1000);
				Overtime = 0;
				if (FindWay(336, 321) < 0)
				{
					Overtime++;
					if (Overtime > 4)
					{
						return -1;
					}
					
				}


				Sleep(1000);
				Overtime = 0;
				while (GetObjAndClick(9, 50, 230, 85, "前往龙城", "ffffff-000000", 119, 5, -1) < 0)
				{
					GetObjAndClick(153, 324, 242, 350, "中央城老兵", "00FF00-000000", 28, -26);
					FindWay(336, 321);
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}

				Overtime = 0;
				while (GetObjAndClick(10, 68, 402, 186, "前往龙城", "ffffff-000000", 119, 5) < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
				
			}
		}
		

		hThread = CreateThread(NULL, 0, AutoOn_lineCheck, NULL, NULL, &Threadid);
		/*while (GetGoodsNumber(GameGoodsDungeonID) != 0)
		{
			Sleep(2000);
			check++;
			if (check > 60)
			{
				tmp_x = GameUser.x;
				tmp_y = GameUser.y;
				check = 0;
			}

			GetUserX_YCoord();

			if (GameUser.x == tmp_x && GameUser.y == tmp_y)
			{
				UseGoodsCall(GameGoodsDungeonID);
				Overtime++;
			}

			if (Overtime > 10)
			{
				return -1;
			}
		}*/
		if (hThread!=NULL)
		{
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);
			Sleep(2000);
			if (WaitBackFlag)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3 ; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}
			}
			Jmp = false;
		}
		else
		{
			TabPage1.LogOut("龙城挂机出错");
		}

		if (isNow)
		{
			for (size_t i = 0; i < 32; i++)
			{

				if (wcsstr(wOn_lineMap[i], L"跨服龙城") != NULL)
				{
					goto Next;
				}
			}
			TabPage1.LogOut("龙城挂机中检测到有任务");
			break;
		}

	}
	
	return 1;
}

int Shopping(bool Open)
{
	DWORD Overtime;
	CString sNumb;
	//CString sTmpNumb;
	DWORD iBackRockNumb = 0;
	DWORD iRandNumb = 0;
	DWORD iCureNumb = 0;
	DWORD iSnowNumb = 0;
	DWORD iFixWaterNumb = 0;

	DWORD  Back = 0;
	DWORD  Rand = 0;
	DWORD  Cure = 0;
	DWORD  Snow = 0;
	DWORD  FixWater = 0;

	DWORD NeedBack = 0;
	DWORD NeedRand = 0;
	DWORD NeedCure = 0;
	DWORD NeedSnow = 0;
	DWORD NeedFixWater = 0;

	int iState = TabPage2.m_cBackRockStart.GetCheck();
	int iState2 = TabPage2.m_cRandStart.GetCheck();
	int iState3 = TabPage2.m_cCureStart.GetCheck();
	int iState4 = TabPage2.m_cSnowStart.GetCheck();
	int iState5 = TabPage2.m_cFixWaterStart.GetCheck();

	
	
	if (iState==BST_CHECKED|| Open)
	{
		sNumb.Empty();
		TabPage2.m_eBackNumb.GetWindowTextW(sNumb);
		iBackRockNumb=_wtoi(sNumb);

		Back = GetGoodsNumber(GameGoodsBackRockID);
		Back += GetGoodsNumber(GameGoodsPresentBackRockID);
		if (iBackRockNumb > Back)
		{
			NeedBack = iBackRockNumb - Back;
		}
		
	}

	if (iState2 == BST_CHECKED)
	{
		sNumb.Empty();
		TabPage2.m_eRandNumb.GetWindowTextW(sNumb);
		iRandNumb = _wtoi(sNumb);
		Rand = GetGoodsNumber(GameGoodsRandRockID);
		if (iRandNumb > Rand)
		{
			NeedRand = iRandNumb - Rand;
		}
		
	}

	if (iState3 == BST_CHECKED)
	{
		sNumb.Empty();
		TabPage2.m_eCureNumb.GetWindowTextW(sNumb);
		iCureNumb = _wtoi(sNumb);
		Cure = GetGoodsNumber(GameGoodsCureRopeID);
	
		if (iCureNumb > Cure)
		{
			NeedCure = iCureNumb - Cure;
		}
	}

	if (iState4 == BST_CHECKED)
	{
		sNumb.Empty();
		TabPage2.m_eSnowNumb.GetWindowTextW(sNumb);
		iSnowNumb = _wtoi(sNumb);
		Snow = GetGoodsNumber(GameGoodsSnowRopeID);
		if (iSnowNumb > Snow)
		{
			NeedSnow = iSnowNumb - Snow;
		}

		
	}

	if (iState5 == BST_CHECKED)
	{
		sNumb.Empty();
		TabPage2.m_eFixWaterNumb.GetWindowTextW(sNumb);
		iFixWaterNumb = _wtoi(sNumb);
		FixWater = GetGoodsNumber(GameGoodsFixWaterID);
		if (iFixWaterNumb > FixWater)
		{
			NeedFixWater = iFixWaterNumb - FixWater;
		}
		
	}	
	
	

	if (NeedBack<=0&& NeedRand<=0 && NeedCure <= 0 && NeedSnow <= 0 && NeedFixWater <= 0)
	{
		return 1;
	}

	age:

	Overtime = 0;
	while (GetObjAndClick(10, 10, 1102, 729, "强化", "b4a97f-000000") < 0)
	{
		//打开商城
		KeyCall(0x54);
		Sleep(1000);
		Overtime++;
		if (Overtime > 10)
		{
			return -1;
		}

	}

	for (size_t i = 0; i < NeedBack; i++)
	{

		
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "盟重传送石", "DEDEDE-020202", 5, 3, 1) < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}

			}
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "确定", "DEDEDE-020202") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}

			}
			
			while (GetObjAndClick(10, 10, 1102, 729, "强化", "b4a97f-000000", 6, 336) < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}

			}
		
	}

	for (size_t i = 0; i < NeedRand; i++)
	{
		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "随机传送石", "DEDEDE-020202", 5, 3, 1) < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "确定", "DEDEDE-020202") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
		while (GetObjAndClick(10, 10, 1102, 729, "强化", "b4a97f-000000", 6, 336) < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
	}

	for (size_t i = 0; i < NeedCure; i++)
	{
		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "疗伤药捆药绳", "DEDEDE-020202", 5, 3, 1) < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}

		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "确定", "DEDEDE-020202") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
		while (GetObjAndClick(10, 10, 1102, 729, "强化", "b4a97f-000000", 6, 336) < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
	}

	for (size_t i = 0; i < NeedSnow; i++)
	{
		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "雪霜捆药绳", "DEDEDE-020202", 5, 3, 1) < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}

		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "确定", "DEDEDE-020202") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
		while (GetObjAndClick(10, 10, 1102, 729, "强化", "b4a97f-000000", 6, 336) < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
	}

	for (size_t i = 0; i < NeedFixWater; i++)
	{
		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "修复神水", "DEDEDE-020202", 5, 3, 1) < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "确定", "DEDEDE-020202") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
		while (GetObjAndClick(10, 10, 1102, 729, "强化", "b4a97f-000000", 6, 336) < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}

	}



	Back = GetGoodsNumber(GameGoodsBackRockID);
	Rand = GetGoodsNumber(GameGoodsRandRockID);
	Cure = GetGoodsNumber(GameGoodsCureRopeID);
	Snow = GetGoodsNumber(GameGoodsSnowRopeID);
	FixWater = GetGoodsNumber(GameGoodsFixWaterID);


	if (iBackRockNumb >= Back)
	{
		NeedBack = iBackRockNumb - Back;
	}
	else
	{
		NeedBack = 0;
	}
	if (iRandNumb >= Rand)
	{
		NeedRand = iRandNumb - Rand;
	}
	else
	{
		NeedRand = 0;
	}
	if (iCureNumb >= Cure)
	{
		NeedCure = iCureNumb - Cure;
	}
	else
	{
		NeedCure = 0;
	}
	if (iSnowNumb >= Snow)
	{
		NeedSnow = iSnowNumb - Snow;
	}
	else
	{
		NeedSnow = 0;
	}
	if (iFixWaterNumb >= FixWater)
	{
		NeedFixWater = iFixWaterNumb - FixWater;
	}
	else
	{
		NeedFixWater = 0;
	}
	

	if (NeedBack <= 0 && NeedRand <= 0 && NeedCure <= 0 && NeedSnow <= 0 && NeedFixWater <= 0)
	{
		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "强化", "b4a97f-000000") >= 0)
		{
			//关闭商城
			KeyCall(0x54);
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}

		}
		return 1;
	}

	
	goto age;

}

int IsGotoX_Y(int Xleft ,int Xright, int Ytop, int Ybottom)
{
	int itimes = 0;
	//判断玩家是否传送到指定地方
	do
	{
		GetUserX_YCoord();
		if (WaitBackFlag|| itimes >180)
		{
			return -1;
		}
		Sleep(1000);
		itimes++;
	} while ((GameUser.x < Xleft || GameUser.x > Xright) || (GameUser.y < Ytop || GameUser.y > Ybottom));
	return 1;
}


int EliminateDemon(bool Jmp)
{
	DWORD Overtime = 0;
	DWORD ret = 0;
	DWORD Times = 6;

	srand((unsigned int)time(NULL));
	int count = 0;
	int count2 = 0;
	//清理窗口
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);

	//隐藏人物
	HideUseAndMaster(1);
	Sleep(500);
	//打开任务自动追踪
	TaskbarTraceSwitch(true);
	Sleep(1500);

	for (size_t x = 0; x < 10; x++)
	{
		if (GetObjAndClick(945, 210, 1105, 575, "楔蛾", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "楔蛾", "00ff00-000000", 28, -26, -1) >= 0)
		{
			ret = 1;
			Jmp = true;
			break;
		}

		if (GetObjAndClick(945, 210, 1105, 575, "大老鼠", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "大老鼠", "00ff00-000000", 28, -26, -1) >= 0)
		{
			ret = 2;
			Jmp = true;
			break;
		}

		if (GetObjAndClick(945, 210, 1105, 575, "蜈蚣", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "蜈蚣", "00ff00-000000", 28, -26, -1) >= 0)
		{
			ret = 3;
			Jmp = true;
			break;
		}

		if (GetObjAndClick(945, 210, 1105, 575, "跳跳蜂", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "跳跳蜂", "00ff00-000000", 28, -26, -1) >= 0)
		{
			ret = 4;
			Jmp = true;
			break;
		}

		if (GetObjAndClick(945, 210, 1105, 575, "巨型蠕虫", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "巨型蠕虫", "00ff00-000000", 28, -26, -1) >= 0)
		{
			ret = 5;
			Jmp = true;
			break;
		}

		if (GetObjAndClick(945, 210, 1105, 575, "野猪", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "野猪", "00ff00-000000", 28, -26, -1) >= 0)
		{
			ret = 6;
			Jmp = true;
			break;
		}

		if (GetObjAndClick(945, 210, 1105, 575, "黑色恶蛆", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "黑色恶蛆", "00ff00-000000", 28, -26, -1) >= 0)
		{
			ret = 7;
			Jmp = true;
			break;
		}
		Sleep(1000);
	}




	for (size_t i = 0; i < Times; i++)
	{
		//清理窗口
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(1000);

		//隐藏人物
		HideUseAndMaster(1);
		Sleep(500);
		//打开任务自动追踪
		TaskbarTraceSwitch(true);
		Sleep(1500);
		Overtime = 0;


		if (Jmp == false)
		{
			while (GetObjAndClick(945, 210, 1105, 575, "除魔任务", "D69D56-000000", 3, 3, -1) < 0)
			{
				Overtime++;
				if (Overtime > 4)
				{
					TabPage1.LogOut(L"除魔任务全部完成");
					EliminateDemonOver = false;
					NowEliminateDemonFlag = false;
					EliminateDemonFlag = false;
					return -13;
				}

			}

		Over:
			Overtime = 0;
			count = rand() % 6;
			count2 = rand() % 2;
			ret = FindWay(367 + count, 338 + count2);
			while (ret < 0)
			{
				ret = FindWay(367 + count, 338 + count2);
				Overtime++;
				if (Overtime > 10 || ret == 0)
				{
					return -5;
				}
			};
			if (ret == 0)
			{
				return -1;
			}


			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "除魔任务", "ffff00-000000", 119, 5, -1) < 0)
			{

				if (WaitBackFlag)
				{
					EliminateDemonFlag = true;
					return -33;
				}
				GetObjAndClick(10, 10, 1102, 729, "除魔使者", "00ff00-000000", 24, -33);
				Sleep(300);

				if (GetObjAndClick(10, 10, 1105, 740, "完成任务", "E0E0E0-000000", 3, 3, -1) >= 0)
				{
					break;
				}
				//358-339   372-338

				ret = FindWay(367 + count, 338 + count2);
				while (ret < 0)
				{
					ret = FindWay(367 + count, 338 + count2);
					Overtime++;
					if (Overtime > 10 || ret == 0)
					{
						return -5;
					}
				};
				if (ret == 0)
				{
					return -1;
				}

				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Sleep(500);
			if (GetObjAndClick(10, 10, 1105, 740, "完成任务", "E0E0E0-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(10, 10, 1105, 740, "完成任务", "E0E0E0-000000") < 0)
				{
					Sleep(300);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}

				continue;
			}


			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "除魔任务", "ffff00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "接受任务", "ffff00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
		}

		Sleep(2000);
		if (GetObjAndClick(10, 10, 1102, 729, "除魔使者", "ffff00-000000", 28, -26, -1) >= 0)
		{
			goto Over;
		}



		for (size_t x = 0; x < 10; x++)
		{
			if (GetObjAndClick(945, 210, 1105, 575, "楔蛾", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "楔蛾", "00ff00-000000", 28, -26, -1) >= 0)
			{
				ret = 1;

				break;
			}

			if (GetObjAndClick(945, 210, 1105, 575, "大老鼠", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "大老鼠", "00ff00-000000", 28, -26, -1) >= 0)
			{
				ret = 2;

				break;
			}

			if (GetObjAndClick(945, 210, 1105, 575, "蜈蚣", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "蜈蚣", "00ff00-000000", 28, -26, -1) >= 0)
			{
				ret = 3;

				break;
			}

			if (GetObjAndClick(945, 210, 1105, 575, "跳跳蜂", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "跳跳蜂", "00ff00-000000", 28, -26, -1) >= 0)
			{
				ret = 4;

				break;
			}

			if (GetObjAndClick(945, 210, 1105, 575, "巨型蠕虫", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "巨型蠕虫", "00ff00-000000", 28, -26, -1) >= 0)
			{
				ret = 5;

				break;
			}

			if (GetObjAndClick(945, 210, 1105, 575, "野猪", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "野猪", "00ff00-000000", 28, -26, -1) >= 0)
			{
				ret = 6;

				break;
			}

			if (GetObjAndClick(945, 210, 1105, 575, "黑色恶蛆", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "黑色恶蛆", "00ff00-000000", 28, -26, -1) >= 0)
			{
				ret = 7;

				break;
			}
			Sleep(1000);
		}



		if (ret == 0)
		{
			TabPage1.LogOut(L"没有识别到除魔任务");
			Jmp == false;
			Times++;
			continue;
		}
		//移动到公告牌旁边


		int Ret = FindWay(338, 335);;
		while (Ret < 0)
		{
			Ret = FindWay(338, 335);;
			Overtime++;
			if (Overtime > 10 || Ret == 0)
			{
				return -5;
			}
		};
		if (Ret == 0)
		{
			return -1;
		}


		//清理窗口
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(1000);

		//隐藏人物
		HideUseAndMaster(1);
		Sleep(500);

		Sleep(500);
		//点击公告牌
		NpcMoveSleep
			Overtime = 0;;
		while (GetObjAndClick(0, 0, 402, 186, "使用移动", "FFFF00-000000", 3, 3, -1) < 0)
		{
			GetObjAndClick(415, 84, 860, 426, "公告牌", "00FF00-000000", 5, -77);
			FindWay(338, 335);

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -5;
			}
		}
		NpcMoveSleep
			//使用移动
			Overtime = 0;;
		while (GetObjAndClick(0, 0, 402, 186, "使用移动", "FFFF00-000000") < 0)
		{

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -2;
			}
		}
		moveTo(0, 0);
		Sleep(1000);

		int state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}


		if (ret == 1)
		{
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "祖玛寺庙", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}

			while (GetObjAndClick(945, 210, 1105, 575, "楔蛾", "ffff00-000000") < 0 && GetObjAndClick(945, 210, 1105, 575, "楔蛾", "00FF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			//判断是否到了坐标点
			Ret = IsGotoX_Y(204, 226, 137, 169);
			if (Ret == -1)
			{
				return -1;
			}
			EliminateDemonFlag = false;
			HANDLE hThread = NULL;
			DWORD Threadid = 0;
			//开始挂机
			hThread = CreateThread(NULL, 0, AutoOn_lineCheck, NULL, NULL, &Threadid);
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "除魔使者", "ffff00-000000", 28, -26, -1) < 0)
			{
				if (WaitBackFlag || (Overtime > 900))
				{
					TabPage1.LogOut(L"除魔任务被打或者超时15分钟");
					EliminateDemonFlag = true;
					return -33;
				}
				Sleep(1000);
				Overtime++;
			}
			EliminateDemonFlag = true;
			//等待挂机结束
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);

		}


		if (ret == 2)
		{
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "祖玛寺庙", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}

			while (GetObjAndClick(945, 210, 1105, 575, "大老鼠", "ffff00-000000") < 0 && GetObjAndClick(945, 210, 1105, 575, "大老鼠", "00FF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			//判断是否到了坐标点
			Ret = IsGotoX_Y(225, 255, 149, 186);
			if (Ret == -1)
			{
				return -1;
			}
			EliminateDemonFlag = false;
			HANDLE hThread = NULL;
			DWORD Threadid = 0;
			//开始挂机
			hThread = CreateThread(NULL, 0, AutoOn_lineCheck, NULL, NULL, &Threadid);
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "除魔使者", "ffff00-000000", 28, -26, -1) < 0)
			{
				if (WaitBackFlag || (Overtime > 900))
				{
					TabPage1.LogOut(L"除魔任务被打或者超时15分钟");
					EliminateDemonFlag = true;
					return -33;
				}
				Sleep(1000);
				Overtime++;
			}
			EliminateDemonFlag = true;
			//等待挂机结束
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);

		}




		if (ret == 3)
		{
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "蜈蚣洞", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}

			NpcMoveSleep
				for (size_t i = 0; i < 10; i++)
				{
					GetUserInMap();
					if (wcscmp(L"盟重省", GameUser.InMap) == 0)
					{
						WayfindingCall(139, 85);
					}
					else
					{
						break;
					}
					Sleep(700);
				}

			EliminateDemonFlag = false;
			HANDLE hThread = NULL;
			DWORD Threadid = 0;
			//开始挂机
			hThread = CreateThread(NULL, 0, AutoOn_lineCheck, NULL, NULL, &Threadid);
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "除魔使者", "ffff00-000000", 28, -26, -1) < 0)
			{
				if (WaitBackFlag || (Overtime > 900))
				{
					TabPage1.LogOut(L"除魔任务被打或者超时15分钟");
					EliminateDemonFlag = true;
					return -33;
				}
				Sleep(1000);
				Overtime++;
			}
			EliminateDemonFlag = true;
			//等待挂机结束
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);

		}



		if (ret == 4)
		{
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "蜈蚣洞", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}

			NpcMoveSleep
				for (size_t i = 0; i < 10; i++)
				{
					GetUserInMap();
					if (wcscmp(L"盟重省", GameUser.InMap) == 0)
					{
						FindWay(139, 85);
					}
					else
					{
						break;
					}
					Sleep(700);
				}
			EliminateDemonFlag = false;
			HANDLE hThread = NULL;
			DWORD Threadid = 0;
			//开始挂机
			hThread = CreateThread(NULL, 0, AutoOn_lineCheck, NULL, NULL, &Threadid);
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "除魔使者", "ffff00-000000", 28, -26, -1) < 0)
			{
				if (WaitBackFlag || (Overtime > 900))
				{
					TabPage1.LogOut(L"除魔任务被打或者超时15分钟");
					EliminateDemonFlag = true;
					return -33;
				}
				Sleep(1000);
				Overtime++;
			}
			EliminateDemonFlag = true;
			//等待挂机结束
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);

		}

		if (ret == 5)
		{
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "蜈蚣洞", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			for (size_t i = 0; i < 10; i++)
			{
				GetUserInMap();
				if (wcscmp(L"盟重省", GameUser.InMap) == 0)
				{
					WayfindingCall(139, 85);
				}
				else
				{
					break;
				}
				Sleep(700);
			}

			while (GetObjAndClick(945, 210, 1105, 575, "巨型蠕虫", "ffff00-000000") < 0 && GetObjAndClick(945, 210, 1105, 575, "巨型蠕虫", "00FF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			//判断是否到了坐标点
			Ret = Ret = IsGotoX_Y(70, 87, 164, 202);
			if (Ret == -1)
			{
				return -1;
			}
			EliminateDemonFlag = false;
			HANDLE hThread = NULL;
			DWORD Threadid = 0;
			//开始挂机
			hThread = CreateThread(NULL, 0, AutoOn_lineCheck, NULL, NULL, &Threadid);
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "除魔使者", "ffff00-000000", 28, -26, -1) < 0)
			{
				if (WaitBackFlag || (Overtime > 900))
				{
					TabPage1.LogOut(L"除魔任务被打或者超时15分钟");
					EliminateDemonFlag = true;
					return -33;
				}
				Sleep(1000);
				Overtime++;
			}
			EliminateDemonFlag = true;
			//等待挂机结束
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);

		}


		if (ret == 6)
		{
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "石墓洞穴", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{

					return -3;
				}
			}
			//168,139
			NpcMoveSleep
			Overtime = 0;
			while (GetObjAndClick(945, 210, 1105, 575, "野猪", "ffff00-000000") < 0 && GetObjAndClick(945, 210, 1105, 575, "野猪", "00FF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			if(GetObjAndClick(945, 210, 1105, 575, "黑野猪", "ffff00-000000", 28, -26, -1) >= 0 || GetObjAndClick(945, 210, 1105, 575, "黑野猪", "00ff00-000000", 28, -26, -1) >= 0)
			{
				//245,232  274,249
				//判断是否到了坐标点
				Ret = Ret = IsGotoX_Y(245, 274, 232, 249);
				if (Ret == -1)
				{
					return -1;
				}
			}
			else
			{
				//142,132  168,153
				Ret = Ret = IsGotoX_Y(142, 168, 132, 153);
				if (Ret == -1)
				{
					return -1;
				}
			}
			


			EliminateDemonFlag = false;
			HANDLE hThread = NULL;
			DWORD Threadid = 0;
			//开始挂机
			hThread = CreateThread(NULL, 0, AutoOn_lineCheck, NULL, NULL, &Threadid);
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "除魔使者", "ffff00-000000", 28, -26, -1) < 0)
			{
				if (WaitBackFlag || (Overtime > 900))
				{
					TabPage1.LogOut(L"除魔任务被打或者超时15分钟");
					EliminateDemonFlag = true;
					return -33;
				}
				Sleep(1000);
				Overtime++;
			}
			EliminateDemonFlag = true;
			//等待挂机结束
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);

		}

		if (ret == 7)
		{
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "石墓洞穴", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}

			while (GetObjAndClick(945, 210, 1105, 575, "黑色恶蛆", "ffff00-000000") < 0 && GetObjAndClick(945, 210, 1105, 575, "黑色恶蛆", "00FF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			//判断是否到了坐标点
			Ret = IsGotoX_Y(280, 309, 85, 118);
			if (Ret == -1)
			{
				return -1;
			}
			EliminateDemonFlag = false;
			HANDLE hThread = NULL;
			DWORD Threadid = 0;
			//开始挂机
			hThread = CreateThread(NULL, 0, AutoOn_lineCheck, NULL, NULL, &Threadid);
			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "除魔使者", "ffff00-000000", 28, -26, -1) < 0)
			{
				if (WaitBackFlag || (Overtime > 900))
				{
					TabPage1.LogOut(L"除魔任务被打或者超时15分钟");
					EliminateDemonFlag = true;
					return -33;
				}
				Sleep(1000);
				Overtime++;
			}
			EliminateDemonFlag = true;
			//等待挂机结束
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);
		}



		EliminateDemonFlag = false;
		UseBack();

		EliminateDemonFlag = false;
		for (size_t i = 0; i < 4; i++)
		{
			if (TrampNpcFunc() > 0)
			{
				TabPage1.LogOut(L"玩家除魔流浪汉完成");
				break;
			}
			if (TrampNpcFunc(true) > 0)
			{
				TabPage1.LogOut(L"英雄除魔流浪汉完成");
				break;
			}

			if (i >= 4)
			{
				return -5;
			}
		}
		count = rand() % 6;
		count2 = rand() % 2;
		ret = FindWay(367 + count, 338 + count2);
		while (ret < 0)
		{
			ret = FindWay(367 + count, 338 + count2);
			Overtime++;
			if (Overtime > 10 || ret == 0)
			{
				return -5;
			}
		};
		if (ret == 0)
		{
			return -1;
		}
		//清理窗口
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(1000);

		//隐藏人物
		HideUseAndMaster(1);
		Sleep(500);

		Overtime = 0;
		while (GetObjAndClick(10, 10, 1105, 740, "完成任务", "E0E0E0-000000", 119, 5, -1) < 0)
		{
			if (WaitBackFlag)
			{
				EliminateDemonFlag = true;
				return -33;
			}

			ret = FindWay(367 + count, 338 + count2);
			while (ret < 0)
			{
				ret = FindWay(367 + count, 338 + count2);
				Overtime++;
				if (Overtime > 10 || ret == 0)
				{
					return -5;
				}
			};
			if (ret == 0)
			{
				return -1;
			}

			GetObjAndClick(10, 10, 1102, 729, "除魔使者", "00ff00-000000", 24, -33);
			Sleep(300);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}

		while (GetObjAndClick(10, 10, 1105, 740, "完成任务", "E0E0E0-000000") < 0)
		{
			if (WaitBackFlag)
			{
				EliminateDemonFlag = true;
				return -33;
			}
			Sleep(300);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}

		Jmp = false;
		EliminateDemonContinueFlag = false;
	}
}


int StartDarts(bool Jmp)
{
	DWORD Overtime = 0;
	int ret = 0;
	

	for (size_t i = 0; i < 2; i++)
	{
		//清理窗口
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(500);
		//隐藏人物
		HideUseAndMaster(1);
		if (Jmp == false)
		{
			ret = FindWay(334, 323);
			if (ret < 0)
			{
				return -1;
			}
			Sleep(1000);

			Overtime = 0;
			while (GetObjAndClick(10, 27, 405, 189, "我要押镖", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(612, 151, 999, 534, "玛法镖局", "00FF00-000000", 24, -36);
				FindWay(334, 323);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
				
			}

			Overtime = 0;
			while (GetObjAndClick(10, 27, 405, 189, "我要押镖", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			NpcMoveSleep
			if (GetObjAndClick(10, 27, 405, 189, "请明日再来", "ffffff-000000", 3, 3, -1) >= 0)
			{
				TabPage1.LogOut(L"押镖任务全部完成");
				StartDartsOver = false;
				return -13;
			}

			if (GetObjAndClick(10, 27, 405, 189, "开启", "ffffff-000000", 3, 3, -1) >= 0)
			{
				TabPage1.LogOut(L"押镖任务不在规定时间");
				StartDartsOver = false;
				return -1;
			}


			TabPage1.LogOut(L"等待镖车到达");
			for (size_t i = 0; i < 424; i++)
			{
				Sleep(1000);
			}
			TabPage1.LogOut(L"开始去往收镖师");
			
		}
		//隐藏人物
		HideUseAndMaster(1);
		ret = FindWay(338, 335);
		if (ret < 0)
		{
			return -1;
		}

		NpcMoveSleep
			Overtime = 0;
		while (GetObjAndClick(0, 0, 402, 186, "前往沙城区域", "FFFF00-000000", 3, 3, -1) < 0)
		{
			GetObjAndClick(584, 240, 645, 269, "公告牌", "00FF00-000000", 5, -77);
			FindWay(338, 335);

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -5;
			}
		}

		NpcMoveSleep
			//前往收镖师
			Overtime = 0;
		while (GetObjAndClick(0, 0, 402, 186, "前往沙城区域", "FFFF00-000000") < 0)
		{

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -2;
			}
		}
		Sleep(1000);
		//显示人物
		HideUseAndMaster(0);


		Overtime = 0;
		ret = FindWay(674, 338);
		while (ret < 0)
		{
			ret = FindWay(674, 338);
			Overtime++;
			if (Overtime > 10 || ret == 0)
			{
				return -5;
			}
		};
		if (ret == 0)
		{
			return -1;
		}

		//清理窗口
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(500);
		//隐藏人物
		HideUseAndMaster(1);
		Overtime = 0;
		while (GetObjAndClick(0, 0, 402, 186, "领取押镖奖励", "FFFF00-000000", 3, 3, -1) < 0)
		{
			//19, 30
			GetObjAndClick(10, 10, 1102, 729, "收镖师", "00FF00-000000", 19, -30);
			FindWay(674, 338);

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -5;
			}
		}

		while (GetObjAndClick(0, 0, 402, 186, "领取押镖奖励", "FFFF00-000000") < 0)
		{

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -5;
			}
		}
		Sleep(1000);
		UseBack();
		for (size_t i = 0; i < 4; i++)
		{
			if (TrampNpcFunc() > 0)
			{
				TabPage1.LogOut(L"押镖流浪汉完成");
				break;
			}

			if (i >= 4)
			{
				return -5;
			}
		}

		Jmp = false;
		StartDartsContinueFlag = false;
	}
	TabPage1.LogOut(L"押镖任务全部完成");
	return 1;

}


int Trial(bool Jmp)
{
	DWORD Overtime = 0;
	DWORD Goods = 0;
	int ret = 0;
	int Ret = 0;
	int time = 0, time2=0;
	while (true)
	{
		//清理窗口
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);

		Goods = GetGoodsNumber(GameGoodsAdamasID);

		if (Goods < 100)
		{
			Sleep(1000);
			SetOn_lineState(0);
			TabPage1.LogOut("金刚石不足100");
			return 1;
		}

		if (Jmp == false)
		{
			//移动到分解师旁边
			Overtime = 0;
			while (FindWay(330, 340) < 0)
			{
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			};

			//点击公告牌
			NpcMoveSleep
				Overtime = 0;

			while (GetObjAndClick(10, 10, 402, 186, "装备分解", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(366, 155, 890, 559, "金石巧匠", "00FF00-000000", 24, -31);
				FindWay(330, 340);
				Sleep(2000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			while (GetObjAndClick(10, 10, 402, 186, "装备分解", "FFFF00-000000") < 0)
			{
				//点击分解npc
				//moveTo(69, 494);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			NpcMoveSleep
				//前往金刚试炼
				Overtime = 0;
			while (GetObjAndClick(366, 155, 890, 559, "金刚密卷", "00FF00-000000", 28, -12) < 0)
			{
				FindWay(332, 340);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -2;
				}
			}

			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "金刚试炼", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "前往", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}

		}


		Sleep(2000);

		if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
		{

		}
		else
		{
			//打开地图
			KeyCall(77);
			Sleep(1000);
		}

		if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0)
		{
			while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}

			}
			
		}
		else if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
		{
			while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}

			}
			
		}
		while (true)
		{
			Goods = GetGoodsNumber(GameGoodsAdamasID);

			if (Goods < 100)
			{
				Sleep(1000);
				SetOn_lineState(0);
				TabPage1.LogOut("金刚石不足100");
				break;
			}


			//超过30分钟回城一次
			time = dMinute / 30;
			if (time > time2)
			{
				time2 = time;
				Sleep(1000);
				SetOn_lineState(0);
				break;
				
			}

			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "摇筛子", "ffff00-000000", 119, 5, -1) < 0)
			{
				if (GetObjAndClick(10, 68, 402, 186, "摇筛子", "00ff00-000000", 119, 5, -1) >= 0)
				{
					break;
				}

				if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
				{

				}
				else
				{
					//打开地图
					KeyCall(77);
					Sleep(1000);
				}

				if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}
					
				}
				else if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}
					
				}

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			if (GetObjAndClick(10, 10, 1102, 729, "NPC列表", "D2D0C4-000000", 3, 3, -1) >= 0)
			{
				//关闭地图
				KeyCall(77);
				Sleep(1000);
			}

			while (GetObjAndClick(10, 68, 402, 186, "摇筛子", "ffff00-000000") < 0)
			{
				if (GetObjAndClick(10, 68, 402, 186, "摇筛子", "00ff00-000000") >= 0)
				{
					break;
				}
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}

			}

			Sleep(4000);
			Overtime = 0;
			while (OpenAuto() == 0)
			{
				Sleep(500);
				Overtime++;
				if (Overtime > 10)
				{
					TabPage1.LogOut("金刚试炼挂机失败");
					return -1;
				}
			};
				
				
				
			while ( MasterDied() !=1)
			{
				
			}
			ret = ChoseAdamasEquip();
			if (ret > 0)
			{
				Overtime = 0;
				//判断背包格子是否足够  不然会卡死
				Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
				DWORD PackResidue = GameGoodsMaxNumber - Ret;
				Ret = 0;
				DWORD Times = 0;
				if (PackResidue >= 2)
				{
					while (true)
					{
						Ret = ChoseGoodsDisintegrateCall();
						if (Ret == 1)
						{
							Sleep(1500);
							while (GetObjAndClick(10, 10, 1102, 729, "分解可获得", "DEDEDE-020202", 192, 88) < 0)
							{
								Sleep(1000);
								Overtime++;
								if (Overtime > 10)
								{
									return -1;
								}

							}
						}
						else if (Ret == 2)
						{
							break;
						}
						else if (Ret == 0)
						{
							TabPage1.LogOut("金刚试炼分解出错无法分解");
							Times++;
						}
						if (Times >= 3)
						{
							break;
						}

					}
				}
				else
				{
					TabPage1.LogOut("金刚试炼背包位置不足无法分解");
					return 1;
				}

				Ret = 0;
			}
		}
		Jmp = false;

		Sleep(2000);
		GetUserInMap();
		if (wcscmp(L"金刚试炼", GameUser.InMap) == 0)
		{
			if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
			{

			}
			else
			{
				//打开地图
				KeyCall(77);
				Sleep(1000);
			}

			if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}

			}
			else if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}

			}

			Sleep(1000);

			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "退出副本", "ffff00-000000", 119, 5, -1) < 0)
			{
				if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
				{

				}
				else
				{
					//打开地图
					KeyCall(77);
					Sleep(1000);
				}

				if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}

				}
				else if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}

				}
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "退出副本", "ffff00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
		}
		Sleep(2000);

	}

	return 1;

}


int Threehookgantry(bool Jmp)
{
	DWORD Overtime = 0;
	DWORD Goods = 0;
	DWORD Goods2 = 0;
	int ret = 0;
	int Ret = 0;
	int time = 0, time2 = 0;
	char direction[2][8]{ "往左走","往右走" };
	
	//清理窗口
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);

	Goods = GetGoodsNumber(GameGoodsAdamasID);
	Goods2= GetGoodsNumber(GameGoodsTokenID);

	if (Goods< 1500 || Goods2< 9)
	{
		Sleep(1000);
		SetOn_lineState(0);
		TabPage1.LogOut("龙门三劫门票或者金刚石不足");
		return 1;
	}

	if (Jmp == false)
	{
		//移动到分解师旁边
		Overtime = 0;
		while (FindWay(330, 340)< 0)
		{
			Overtime++;
			if (Overtime > 10)
			{
				return -5;
			}
		};


		NpcMoveSleep
			//前往龙门三劫
			Overtime = 0;
		while (GetObjAndClick(286, 409, 690, 601, "金刚密卷", "00FF00-000000", 28, -12) < 0)
		{
			FindWay(330, 340);
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -2;
			}
		}

		Sleep(1000);
		Overtime = 0;;
		while (GetObjAndClick(0, 0, 402, 186, "龙门三劫", "FFFF00-000000") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -3;
			}
		}
		Sleep(1000);
		Overtime = 0;;
		while (GetObjAndClick(0, 0, 402, 186, "前往", "FFFF00-000000") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -3;
			}
		}

	}
	while (true)
	{
		RandFunc(2, true);
		Sleep(1000);
		FindWay(33, 36);

		while (OpenAuto() == 0)
		{
			Sleep(500);
			Overtime++;
			if (Overtime > 10)
			{
				TabPage1.LogOut("龙门三劫挂机失败");
				return -1;
			}
		};

		while (MasterDied() != 1)
		{

		}

		Sleep(15000);
		SetOn_lineState(0);

		FindWay(33, 36);
		Sleep(1000);
		Overtime = 0;
		while (GetObjAndClick(10, 68, 402, 186, &direction[index[0]][0], "ffff00-000000", 119, 5, -1) < 0)
		{
			if (GetObjAndClick(10, 68, 402, 186, "退出副本", "ffff00-000000", 119, 5, -1) >= 0)
			{
				if (GetObjAndClick(10, 68, 402, 186, &direction[index[0]][0], "00ff00-000000", 119, 5, -1) < 0)
				{
					return 1;
				}
			}

			if (GetObjAndClick(10, 68, 402, 186, &direction[index[0]][0], "00ff00-000000", 119, 5, -1) >= 0)
			{
				break;
			}

			if (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000", 3, 3, -1) >= 0)
			{

			}
			else
			{
				//打开地图
				KeyCall(77);
				Sleep(1000);
			}

			if (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}
				
			}
			else if (GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}
				
			}

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}

		if (GetObjAndClick(10, 10, 1102, 729, "NPC列表", "D2D0C4-000000", 3, 3, -1) >= 0)
		{
			//关闭地图
			KeyCall(77);
			Sleep(1000);
		}

		while (GetObjAndClick(10, 68, 402, 186, &direction[index[0]][0], "ffff00-000000") < 0)
		{
			if (GetObjAndClick(10, 68, 402, 186, &direction[index[0]][0], "00ff00-000000") >= 0)
			{
				break;
			}
			Sleep(1000);
			Overtime++;
			if (Overtime > 2)
			{
				return -1;
			}

		}

		Sleep(2000);
		GetUserInMap();
		if (wcscmp(L"龙门三劫", GameUser.InMap) != 0)
		{
			return 1;
		}

	}
	

	return 1;

}


int DevilSlayer(bool Jmp)
{
	DWORD Overtime = 0;
	DWORD Goods = 0;
	DWORD Goods2 = 0;
	
	int ret = 0;
	int Ret = 0;
	int time = 0, time2 = 0;

	char direction[2][15]{ "魔族指令书" ,"黄金魔族指令书"};
	char cstr[15]{};
	while (true)
	{
		//清理窗口
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);

		Goods = GetGoodsNumber(GameGoodsInfernoID);
		Goods2 = GetGoodsNumber(GameGoodsGoldInfernoID);

		if (Goods <1 && Goods2 < 1)
		{
			Sleep(1000);
			SetOn_lineState(0);
			TabPage1.LogOut("魔族指令书不足");
			return 1;
		}


		if (Jmp == false)
		{
			//移动到分解师旁边
			Overtime = 0;
			while (FindWay(330, 340) < 0)
			{
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			};

			//点击公告牌
			NpcMoveSleep
			Overtime = 0;

			while (GetObjAndClick(10, 10, 402, 186, "装备分解", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(366, 155, 890, 559, "金石巧匠", "00FF00-000000", 24, -31);
				FindWay(330, 340);
				Sleep(2000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			while (GetObjAndClick(10, 10, 402, 186, "装备分解", "FFFF00-000000") < 0)
			{
				//点击分解npc
				//moveTo(69, 494);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			NpcMoveSleep
			//前往屠魔房间

			if (FindWay(330, 334) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;

			while (GetObjAndClick(0, 0, 402, 186, "魔族指令", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(38, 279, 107, 307, "活动大使", "00FF00-000000", 25, -37);
				FindWay(330, 334);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "魔族指令", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			if (FindWay(11, 18) < 0)
			{
				return -1;
			}

			Overtime = 0;
			while (GetObjAndClick(10, 10, 1102, 729, "神秘人", "00ff00-000000", 17,-36) < 0)
			{
				FindWay(11, 18);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}


			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "前往", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

		}

		time2 = dMinute / 30;
	
		while (true)
		{
			//超过30分钟回城一次
			time = dMinute / 30;
			if (time > time2)
			{
				time2 = time;
				Sleep(8000);
				SetOn_lineState(0);
				break;

			}

			Goods = GetGoodsNumber(GameGoodsInfernoID);
			Goods2 = GetGoodsNumber(GameGoodsGoldInfernoID);

			if (Goods < 1 && Goods2 < 1)
			{
				Sleep(1000);
				SetOn_lineState(0);
				TabPage1.LogOut("魔族指令书不足");
				return 1;
			}

			

			if (Goods>0)
			{
				memcpy(cstr, direction[0], 15);
			}
			else
			{
				memcpy(cstr, direction[1], 15);
			}


			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, cstr, "ffff00-000000", 119, 5, -1) < 0)
			{
				if (GetObjAndClick(10, 68, 402, 186, cstr, "00ff00-000000", 119, 5, -1) >= 0)
				{
					break;
				}

				if (GetObjAndClick(761, 96, 943, 587, "魔族长老", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(761, 96, 943, 587, "魔族长老", "00ff00-000000", 3, 3, -1) >= 0)
				{

				}
				else
				{
					//打开地图
					KeyCall(77);
					Sleep(1000);
				}

				if (GetObjAndClick(761, 96, 943, 587, "魔族长老", "D2D0C4-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(761, 96, 943, 587, "魔族长老", "D2D0C4-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}

				}
				else if (GetObjAndClick(761, 96, 943, 587, "魔族长老", "00ff00-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(761, 96, 943, 587, "魔族长老", "00ff00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}

				}

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			if (GetObjAndClick(10, 10, 1102, 729, "NPC列表", "D2D0C4-000000", 3, 3, -1) >= 0)
			{
				//关闭地图
				KeyCall(77);
				Sleep(1000);
			}

			while (GetObjAndClick(10, 68, 402, 186, cstr, "ffff00-000000") < 0)
			{
				if (GetObjAndClick(10, 68, 402, 186, cstr, "00ff00-000000") >= 0)
				{
					break;
				}
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}

			}

			Sleep(4000);
			Overtime = 0;
			while (OpenAuto() == 0)
			{
				Sleep(500);
				Overtime++;
				if (Overtime > 10)
				{
					TabPage1.LogOut("屠魔挂机失败");
					return -1;
				}
			};



			while (MasterDied() != 1)
			{

			}

			//吃散红
			Overtime = 0;
			while (GetGoodsNumber(GameGoodsfgrdID) > 1)
			{
				Sleep(1300);
				UseGoodsCall(GameGoodsfgrdID);
				//Sleep(1300);
				Overtime++;
				if (Overtime > 100)
				{
					return -4;
				}
			}
			//吃散蓝
			Overtime = 0;
			while (GetGoodsNumber(GameGoodsMagicDrugID) > 1)
			{

				UseGoodsCall(GameGoodsMagicDrugID);
				//Sleep(1300);
				Overtime++;
				if (Overtime > 100)
				{
					return -4;
				}
			}


			//吃中量红
			Overtime = 0;
			while (GetGoodsNumber(GameGoodsmiddlefgrdID) > 0)
			{

				UseGoodsCall(GameGoodsmiddlefgrdID);
				//Sleep(1300);
				Overtime++;
				if (Overtime > 100)
				{
					return -4;
				}
			}

			//吃中量蓝
			Overtime = 0;
			while (GetGoodsNumber(GameGoodsmiddleMagicID) > 0)
			{

				UseGoodsCall(GameGoodsmiddleMagicID);
				//Sleep(1300);
				Overtime++;
				if (Overtime > 100)
				{
					return -4;
				}
			}

			//吃中量太阳水
			Overtime = 0;
			while (GetGoodsNumber(GameGoodsSunWaterID) > 0)
			{

				UseGoodsCall(GameGoodsSunWaterID);
				//Sleep(1300);
				Overtime++;
				if (Overtime > 100)
				{
					return -4;
				}
			}
			//吃强效太阳水
			Overtime = 0;
			while (GetGoodsNumber(GameGoodsSuperSunWaterID) > 0)
			{

				UseGoodsCall(GameGoodsSuperSunWaterID);
				//Sleep(1300);
				Overtime++;
				if (Overtime > 100)
				{
					return -4;
				}
			}
			

			ret = ChoseAdamasEquip();
			if (ret > 0)
			{
				Overtime = 0;
				//判断背包格子是否足够  不然会卡死
				Ret = TraversalKnapsack(KnapsackList, hMainProcess, MyKnapsack);
				DWORD PackResidue = GameGoodsMaxNumber - Ret;
				Ret = 0;
				DWORD Times = 0;
				if (PackResidue >= 2)
				{
					while (true)
					{
						Ret = ChoseGoodsDisintegrateCall();
						if (Ret == 1)
						{
							Sleep(1500);
							while (GetObjAndClick(10, 10, 1102, 729, "分解可获得", "DEDEDE-020202", 192, 88) < 0)
							{
								Sleep(1000);
								Overtime++;
								if (Overtime > 10)
								{
									return -1;
								}

							}
						}
						else if (Ret == 2)
						{
							break;
						}
						else if (Ret == 0)
						{
							TabPage1.LogOut("屠魔分解出错无法分解");
							Times++;
						}
						if (Times >= 3)
						{
							break;
						}

					}
				}
				else
				{
					TabPage1.LogOut("屠魔背包位置不足无法分解");
					return 1;
				}

				Ret = 0;
			}
		}
		Jmp = false;

		Sleep(2000);
		GetUserInMap();
		if (wcscmp(L"屠魔房间", GameUser.InMap) == 0)
		{
			UseBack();
		}

	}

	return 1;

}

int PrivateMap(int Jmp)
{

	DWORD Overtime = 0;
	//清理窗口
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	if (Jmp > 0)
	{
		//点击单人副本

		while(GetObjAndClick(471, 5, 950, 193, "单人副本", "F8F0A8-000000", 3, 3, -1) < 0)
		{
			moveTo(915, 24);
			Sleep(500);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}
		Overtime = 0;
		while (GetObjAndClick(471, 5, 950, 193, "单人副本", "F8F0A8-000000", 28, -25) < 0)
		{
			
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}

		Overtime = 0;
		if (Jmp==1)
		{
			while (GetObjAndClick(169, 70, 528, 595, "镇魔殿", "FFD966-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
		}
		else if (Jmp == 2)
		{
			while (GetObjAndClick(169, 70, 528, 595, "教主殿堂", "FFD966-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
		}
		else if (Jmp == 3)
		{
			while (GetObjAndClick(169, 70, 528, 595, "特权之家", "FFD966-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
		}

		Overtime = 0;
		while (GetObjAndClick(488, 402, 927, 610, "进入副本", "dedede-020202") < 0)
		{

			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}

		

	}
	Sleep(2000);
	GetUserInMap();
	if (wcscmp(L"盟重省", GameUser.InMap) == 0)
	{
		TabPage1.LogOut("个人副本次数或者材料不足");
	}
	
	if (Jmp == 2)
	{
		if (FindWay(20, 18) < 0)
		{
			return -1;
		}
		Overtime = 0;
		while (GetObjAndClick(10, 10, 1102, 729, "大祭司", "00ff00-000000", 20, -16) < 0)
		{
			FindWay(20, 18);
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}
		Overtime = 0;
		while (GetObjAndClick(10, 68, 402, 186, "摇筛子", "ffff00-000000") < 0)
		{
			Sleep(1000);
			Overtime++;
			if (Overtime > 10)
			{
				return -1;
			}
		}
		Sleep(5000);

	}

	while (OpenAuto() == 0)
	{
		Sleep(500);
		Overtime++;
		if (Overtime > 10)
		{
			TabPage1.LogOut("个人副本挂机失败");
			return -1;
		}
	};

	while (MasterDied() != 1)
	{

	}

	Sleep(15000);
	SetOn_lineState(0);

	UseBack();
	Sleep(1500);
	return 1;
}




int Go_On_lineMap(CString Map,bool Jmp)
{ 
	DWORD Overtime = 0;
	int ret = 0;
	DWORD time3 = 0;
	CString sTimes;
	CString cNumb;
	int iNumb = 0;
	long long iNumb2 = 0;
	DWORD state = TabPage3.m_cBackWait.GetCheck();
	DWORD state6 = TabPage4.m_cBindMoney.GetCheck();
	if (state6==BST_CHECKED)
	{
		TabPage4.m_eBindMoney.GetWindowTextW(cNumb);
		iNumb=_ttoi(cNumb);
		if (iNumb>0)
		{
			iNumb2=GetUserMoney(0);
			if (iNumb > iNumb2)
			{
				TabPage1.LogOut(L"绑金不足 停止进图,重新启动");
				return -99;
			}
		}
	}
	int state2 = 0;
	//清理窗口
	pDM->KeyPress(27);
	Sleep(500);
	pDM->KeyPress(27);
	Sleep(1000);

	//隐藏人物
	HideUseAndMaster(1);


	//挂机龙城
	if (Map.Find(L"跨服龙城")>=0)
	{
		while (Jmp == false)
		{
			ret = FindWay(334, 322);
			if (ret < 0)
			{
				return -1;
			}
			Sleep(1000);
			
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "前往中央服", "FFFF00-000000", 119, 5, -1) < 0)
			{
				GetObjAndClick(850, 153, 1107, 436, "跨服使者", "00FF00-000000", 24, -36);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "前往中央服", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		//切换攻击模式
		//ChoseUseState();
		//关闭自动追敌
		//CloseAutoTrace(0);
		//关闭自动修理
		SetAutoFix(false);

		DragonCity(Jmp);
		Sleep(2000);
		GetUserInMap();
		if (wcscmp(L"盟重省(跨服)", GameUser.InMap) == 0)
		{
			while (FindWay(334, 322) < 0)
			{

			}

			Sleep(1000);
			HideUseAndMaster(1);
			Overtime = 0;
			while (GetObjAndClick(5, 31, 402, 186, "返回原服", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(850, 153, 1107, 436, "跨服使者", "00FF00-000000", 24, -36);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Overtime = 0;
			while (GetObjAndClick(5, 31, 402, 186, "返回原服", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
		}
		return 1;
	}
	//挂机沃玛暗殿东
	if (Map.Find(L"沃玛暗殿东") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}

			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "沃玛暗殿东", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "沃玛暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "沃玛暗殿东", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL)>19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		
		return 2;
	}
	//沃玛暗殿西
	if (Map.Find(L"沃玛暗殿西") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}

			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "沃玛暗殿东", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "沃玛暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "沃玛暗殿西", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 3;
	}

	//沃玛暗殿南
	if (Map.Find(L"沃玛暗殿南") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}

			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "沃玛暗殿东", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "沃玛暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "沃玛暗殿南", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 4;
	}

	//沃玛暗殿北
	if (Map.Find(L"沃玛暗殿北") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}

			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "沃玛暗殿东", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "沃玛暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "沃玛暗殿北", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 5;
	}

	//祖玛暗殿上
	if (Map.Find(L"祖玛暗殿上") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "祖玛暗殿上", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "祖玛暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "祖玛暗殿上", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 6;
	}

	//祖玛暗殿中
	if (Map.Find(L"祖玛暗殿中") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "祖玛暗殿上", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "祖玛暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "祖玛暗殿中", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 7;
	}

	//祖玛暗殿下
	if (Map.Find(L"祖玛暗殿下") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "祖玛暗殿上", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "祖玛暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "祖玛暗殿下", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 8;
	}

	//赤月暗殿上
	if (Map.Find(L"赤月暗殿上") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "赤月暗殿上", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "赤月暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "赤月暗殿上", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		
		return 9;
	}

	//赤月暗殿下
	if (Map.Find(L"赤月暗殿下") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(334, 322) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "赤月暗殿上", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(277, 96, 951, 388, "赤月暗殿", "00FF00-000000", 25, -41);
				FindWay(334, 322);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "赤月暗殿下", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{

			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		
		return 10;
	}

	//如雾似梦
	if (Map.Find(L"如雾似梦(战场一)") >= 0|| Map.Find(L"如雾似梦(战场二)") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(330, 334) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;

			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(38, 279, 107, 307, "活动大使", "00FF00-000000", 25, -37);
				FindWay(330, 334);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "初级屠魔战场", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}



			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "如雾似梦", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			if (Map.Find(L"如雾似梦(战场一)") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场一", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			else
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场二", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{

			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		
		
		return 11;
	}

	//一心一意
	if (Map.Find(L"一心一意(战场一)") >= 0 || Map.Find(L"一心一意(战场二)") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(330, 334) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(38, 279, 107, 307, "活动大使", "00FF00-000000", 25, -37);
				FindWay(330, 334);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "高级屠魔战场", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "一心一意", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			if (Map.Find(L"一心一意(战场一)") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场一", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			else
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场二", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 12;
	}

	//心心相印
	if (Map.Find(L"心心相印(战场一)") >= 0 || Map.Find(L"心心相印(战场二)") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(330, 334) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(38, 279, 107, 307, "活动大使", "00FF00-000000", 25, -37);
				FindWay(330, 334);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "高级屠魔战场", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "心心相印", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			if (Map.Find(L"心心相印(战场一)") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场一", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			else
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场二", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			break;
		}
		Sleep(1000);
		MapBug = true;
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}
			}
		}
		MapBug = false;
		return 13;
	}

	//飞火流星
	if (Map.Find(L"飞火流星(战场一)") >= 0 || Map.Find(L"飞火流星(战场二)") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(330, 334) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(38, 279, 107, 307, "活动大使", "00FF00-000000", 25, -37);
				FindWay(330, 334);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "高级屠魔战场", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "飞火流星", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			if (Map.Find(L"飞火流星(战场一)") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场一", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			else
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场二", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 14;
	}

	//绮梦幻想
	if (Map.Find(L"绮梦幻想(战场一)") >= 0 || Map.Find(L"绮梦幻想(战场二)") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(330, 334) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(38, 279, 107, 307, "活动大使", "00FF00-000000", 25, -37);
				FindWay(330, 334);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "屠魔烟花", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "高级屠魔战场", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "绮梦幻想", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
			if (Map.Find(L"绮梦幻想(战场一)") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场一", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			else
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "战场二", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 15;
	}

	//挂机幻境
	if (Map.Find(L"幻境") >= 0)
	{
		//移动到幻境旁边
		Overtime = 0;
		while (Jmp == false)
		{
			srand((unsigned int)time(NULL));
			int count = 0;
			int count2 = 0;
			count = rand() % 14;
			count2 = rand() % 4;
			ret = FindWay(361 + count, 338 + count2);
			while (ret < 0)
			{
				ret = FindWay(361 + count, 338 + count2);
				Overtime++;
				if (Overtime > 10 || ret == 0)
				{
					return -5;
				}
			};
			if (ret == 0)
			{
				return -1;
			}
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "迷梦幻境", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(10, 10, 1102, 729, "幻境之门", "00FF00-000000", 25, -41);
				ret = FindWay(361 + count, 338 + count2);
				while (ret < 0)
				{
					ret = FindWay(361 + count, 338 + count2);
					Overtime++;
					if (Overtime > 10 || ret == 0)
					{
						return -5;
					}
				};
				if (ret == 0)
				{
					return -1;
				}
				Sleep(300);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}

			int iState = TabPage4.m_cDifficulty1.GetCheck();
			int iState2 = TabPage4.m_cDifficulty2.GetCheck();
			int iState3 = TabPage4.m_cDifficulty3.GetCheck();

			if (iState == BST_CHECKED)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "迷梦幻境", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (iState2 == BST_CHECKED)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "绮梦幻境", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (iState3 == BST_CHECKED)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "噩梦幻境", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else
			{
				TabPage1.LogOut(L"幻境没有选择难度");
				return -6;
			}
			if (Map.Find(L"幻境一层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "一层", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"幻境二层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "二层", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"幻境三层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "三层", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"幻境四层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "四层", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"幻境五层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "五层", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"幻境六层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "六层", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"幻境七层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "七层", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else
			{
				TabPage1.LogOut(L"幻境层数出错");
				return -6;
			}
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		AutoOn_lineCheck(NULL);
		return 16;
	
	}

	//挂机地牢
	if (Map.Find(L"地牢") >= 0)
	{

		while (Jmp == false)
		{
			//移动到公告牌旁边
			Overtime = 0;
			while (FindWay(338, 335) < 0)
			{
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			};


			//点击公告牌

			NpcMoveSleep
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "使用移动", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(584, 240, 645, 269, "公告牌", "00FF00-000000", 5, -77);
				FindWay(338, 335);
				
				Sleep(1000);
				Overtime++;
				if (Overtime > 10 )
				{
					return -5;
				}
			}

			NpcMoveSleep
				//前往蜈蚣洞
				Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "使用移动", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -2;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "蜈蚣洞", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			
			break;
		}
		Sleep(2000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 17;
	}

	//除魔日常
	if (Map.Find(L"除魔") >= 0&& EliminateDemonOver)
	{
		int RetState = -1;
		NowEliminateDemonFlag = true;

		RetState=SetAutoPick(false);
		if (RetState<0)
		{
			TabPage1.LogOut(L"除魔关闭拾取出错");
			return -18;
		}
		ret = EliminateDemon(EliminateDemonContinueFlag);

		SetAutoPick(RetState);

		NowEliminateDemonFlag = false;
		EliminateDemonFlag = false;

		if (ret < 0&& ret!=-13)
		{
			TabPage1.LogOut(L"除魔出错");
			return -18;
		}
	
		return 18;
	}

	//押镖日常
	if (Map.Find(L"押镖") >= 0 && StartDartsOver)
	{
		
		ret = StartDarts(StartDartsContinueFlag);
		
		if (ret < 0 && ret != -13)
		{
			TabPage1.LogOut(L"押镖出错");
			return -19;
		}

		return 19;
	}

	
	//挂机祖玛
	if (Map.Find(L"祖玛") >= 0)
	{

		while (Jmp == false)
		{
			//移动到公告牌旁边
			Overtime = 0;
			while (FindWay(338, 335) < 0)
			{
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			};


			//点击公告牌

			NpcMoveSleep
				Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "使用移动", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(584, 240, 645, 269, "公告牌", "00FF00-000000", 5, -77);
				FindWay(338, 335);

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			}

			NpcMoveSleep
				//前往祖玛寺庙
				Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "使用移动", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -2;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "祖玛寺庙", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "移动", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}

			break;
		}
		Sleep(2000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		GetUserInMap();

		if (wcscmp(L"祖玛寺庙大殿", GameUser.InMap) == 0)
		{
			WayfindingCall(20, 16);
		}

		Overtime = 0;
		ret = FindWay(213, 167);
		while (ret < 0)
		{
			ret = FindWay(213, 167);
			Overtime++;
			if (Overtime > 10 || ret == 0)
			{
				return -5;
			}
		};
		if (ret == 0)
		{
			return -1;
		}

		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 20;
	}

	//挂机激情地宫
	if (Map.Find(L"宫殿长廊") >= 0)
	{

		while (Jmp == false)
		{
			//移动到公告牌旁边
			Overtime = 0;
			while (FindWay(330, 335) < 0)
			{
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			};


			//点击公告牌

			NpcMoveSleep
				Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "激情地宫", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(2, 380, 210, 586, "镇魔守将", "00FF00-000000", 22, -34);
				FindWay(330, 335);

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			}

			NpcMoveSleep
				//前往激情地宫
				Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "激情地宫", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -2;
				}
			}
			Sleep(1000);
			Overtime = 0;;
			while (GetObjAndClick(0, 0, 402, 186, "准备好了", "FFFF00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -3;
				}
			}
			Sleep(2000);
			if (GetObjAndClick(0, 0, 402, 186, "缺少", "ffffff-000000", 3,3,-1) >= 0)
			{
				TabPage1.LogOut(L"激情地宫缺少材料");
				return -3;
			}
			
			if (CheckWay(1) < 0)
			{
				return -1;
			}
			
			// 前往宫殿长廊
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "宫殿长廊", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(0, 0, 1105, 741, "镇魔守将", "00FF00-000000", 22, -34);
				FindWay(20, 20);

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			}
			Sleep(1000);
			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "宫殿长廊", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -2;
				}
			}
			break;
		}
		
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}

		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 21;
	}

	//挂机月卡地宫
	if (Map.Find(L"月卡地宫") >= 0|| Map.Find(L"未知暗殿") >= 0)
	{

		while (Jmp == false)
		{
			//移动到公告牌旁边
			Overtime = 0;
			while (FindWay(338, 335) < 0)
			{
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			};


			//点击新手地宫

			NpcMoveSleep
				Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "新手地宫", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(833, 471, 1122, 673, "新手地宫", "00FF00-000000", 25, -41);
				FindWay(338, 335);

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -5;
				}
			}

			Sleep(1000);

			if (Map.Find(L"未知暗殿") >= 0)
			{
				//前往未知暗殿
				while (GetObjAndClick(0, 0, 402, 186, "未知暗殿", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -2;
					}
				}

				//前往未知暗殿
				while (GetObjAndClick(0, 0, 402, 186, "进入", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -2;
					}
				}
			}
			else
			{
				//前往新手地宫
				while (GetObjAndClick(0, 0, 402, 186, "新手地宫", "FFFF00-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -5;
					}
				}
				Sleep(1000);
				Overtime = 0;;
				while (GetObjAndClick(0, 0, 402, 186, "月卡地宫", "FFFF00-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -3;
					}
				}
				Sleep(2000);
				if (GetObjAndClick(0, 0, 402, 186, "开通", "ffffff-000000", 3, 3, -1) >= 0)
				{
					TabPage1.LogOut(L"新手地宫没有月卡");
					return -3;
				}

				
			}

			break;
			
		}
		Sleep(2000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}


		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}
			}
		}
		return 22;
	}

	
	//挂机节日活动地图
	if (Map.Find(L"节日活动地图") >= 0)
	{
	//移动到幻境旁边
	Overtime = 0;
	while (Jmp == false)
	{
		ret = FindWay(324, 331);

		Overtime = 0;
		while (GetObjAndClick(0, 0, 402, 186, "地图", "FFFF00-000000", 3, 3, -1) < 0)
		{
			GetObjAndClick(15, 176, 365, 580, "精彩活动", "00FF00-000000", 24, -17);
			FindWay(324, 331);

			Sleep(300);
			Overtime++;
			if (Overtime > 10 || WaitBackFlag)
			{
				return -5;
			}
		}

		Overtime = 0;
		while (GetObjAndClick(0, 0, 402, 186, "地图", "ffff00-000000") < 0)
		{

			Sleep(1000);
			Overtime++;
			if (Overtime > 10 || WaitBackFlag)
			{
				return -5;
			}
		}

		moveTo(392, 105, 0);

		if (Map.Find(L"1") >= 0)
		{
			Overtime = 0;
			while (GetObjAndClick(5, 36, 368, 80, "进入", "ffff00-000000") < 0)
			{

				Sleep(300);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}
		}
		else if (Map.Find(L"2") >= 0)
		{
			Overtime = 0;
			while (GetObjAndClick(3, 69, 387, 90, "进入", "ffff00-000000") < 0)
			{

				Sleep(300);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}
		}
		else if (Map.Find(L"3") >= 0)
		{
			Overtime = 0;
			while (GetObjAndClick(3, 84, 385, 109, "进入", "ffff00-000000") < 0)
			{

				Sleep(300);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}
		}
		else if (Map.Find(L"4") >= 0)
		{
			Overtime = 0;
			while (GetObjAndClick(4, 108, 384, 141, "进入", "ffff00-000000") < 0)
			{

				Sleep(300);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}
		}
		else if (Map.Find(L"5") >= 0)
		{
			Overtime = 0;
			while (GetObjAndClick(7, 140, 388, 175, "进入", "ffff00-000000") < 0)
			{

				Sleep(300);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}
		}
		/*else if (Map.Find(L"6") >= 0)
		{
			Overtime = 0;
			while (GetObjAndClick(2, 28, 411, 193, "鲜花1进入", "ffff00-000000") < 0)
			{

				Sleep(300);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}
		}*/
		else
		{
			TabPage1.LogOut(L"活动地图选择出错");
			return -6;
		}
		break;
	}
	Sleep(1000);
	state2 = TabPage2.m_cHideMaster.GetCheck();
	if (state2 == BST_CHECKED)
	{
		HideUseAndMaster(1);
	}
	else
	{
		HideUseAndMaster(0);
	}
	if (AutoOn_lineCheck(NULL) > 19)
	{
		if (state == BST_CHECKED && IsSafetyArea() == 2)
		{
			TabPage3.m_eBackWait.GetWindowTextW(sTimes);
			time3 = _ttoi(sTimes);
			if (time3 > 0)
			{
				for (size_t i = 0; i < time3; i++)
				{
					if (IsSafetyArea() != 2)
					{
						break;
					}
					Sleep(1000);
				}
			}

		}
	}
	return 23;
	}



	//挂机合服活动地图
	if (Map.Find(L"合服活动地图") >= 0)
	{
		//移动到幻境旁边
		Overtime = 0;
		while (Jmp == false)
		{
			ret = FindWay(323, 334);

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "合区地图", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(3, 3, 403, 608, "合服使者", "00FF00-000000", 24, -17);
				FindWay(323, 334);

				Sleep(300);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "合区地图", "ffff00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10 || WaitBackFlag)
				{
					return -5;
				}
			}

			moveTo(394, 104, 0);

			if (Map.Find(L"1") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(3, 45, 387, 83, "进入", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"2") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(4, 73, 387, 97, "进入", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"3") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(1, 87, 386, 114, "进入", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"4") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(8, 119, 385, 146, "进入", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else if (Map.Find(L"5") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(8, 148, 385, 186, "进入", "ffff00-000000") < 0)
				{

					Sleep(300);
					Overtime++;
					if (Overtime > 10 || WaitBackFlag)
					{
						return -5;
					}
				}
			}
			else
			{
				TabPage1.LogOut(L"活动地图选择出错");
				return -6;
			}
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{
			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}
		return 23;
	}


	//挂机金刚试炼
	if (Map.Find(L"金刚试炼") >= 0)
	{

		
		Sleep(2000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}

		Trial(Jmp);

		Sleep(2000);
		GetUserInMap();
		if (wcscmp(L"金刚试炼", GameUser.InMap) == 0)
		{
			if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
			{

			}
			else
			{
				//打开地图
				KeyCall(77);
				Sleep(1000);
			}

			if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}

			}
			else if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}

			}
			Sleep(1000);

			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "退出副本", "ffff00-000000", 119, 5, -1) < 0)
			{
				if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
				{

				}
				else
				{
					//打开地图
					KeyCall(77);
					Sleep(1000);
				}

				if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "D2D0C4-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}

				}
				else if (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(10, 10, 1102, 729, "欧冶子", "00ff00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}

				}
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "退出副本", "ffff00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
		}
		
		return 24;
	}
	
	//挂机龙门三劫
	if (Map.Find(L"龙门三劫") >= 0)
	{

		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}

		Threehookgantry(Jmp);

		Sleep(2000);
		GetUserInMap();
		if (wcscmp(L"龙门三劫", GameUser.InMap) == 0)
		{
			if (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000", 3, 3, -1) >= 0)
			{

			}
			else
			{
				//打开地图
				KeyCall(77);
				Sleep(1000);
			}

			if (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}

			}
			else if (GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000", 3, 3, -1) >= 0)
			{
				while (GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000") < 0)
				{
					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}

				}

			}
			Sleep(1000);

			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "退出副本", "ffff00-000000", 119, 5, -1) < 0)
			{
				if (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000", 3, 3, -1) >= 0 || GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000", 3, 3, -1) >= 0)
				{

				}
				else
				{
					//打开地图
					KeyCall(77);
					Sleep(1000);
				}

				if (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(761, 96, 943, 587, "无名", "D2D0C4-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}

				}
				else if (GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000", 3, 3, -1) >= 0)
				{
					while (GetObjAndClick(761, 96, 943, 587, "无名", "00ff00-000000") < 0)
					{
						Sleep(1000);
						Overtime++;
						if (Overtime > 10)
						{
							return -1;
						}

					}

				}
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(10, 68, 402, 186, "退出副本", "ffff00-000000") < 0)
			{
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}
		}
		
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsTreasureID) > 0)
		{
			UseGoodsCall(GameGoodsTreasureID);
			Sleep(200);
			Overtime++;
			if (Overtime > 30)
			{
				return -4;
			}
		}
		
		Overtime = 0;
		while (GetGoodsNumber(GameGoodsTreasure2ID) > 0)
		{
			UseGoodsCall(GameGoodsTreasure2ID);
			Sleep(200);
			Overtime++;
			if (Overtime > 30)
			{
				return -4;
			}
		}

		return 24;
	}

	// 神秘洞穴
	if (Map.Find(L"神秘洞穴一层") >= 0 || Map.Find(L"神秘洞穴二层") >= 0 || Map.Find(L"神秘洞穴三层") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(330, 334) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;

			while (GetObjAndClick(0, 0, 402, 186, "秘之魔境", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(38, 279, 107, 307, "活动大使", "00FF00-000000", 25, -37);
				FindWay(330, 334);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "秘之魔境", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "活动地图", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}



			if (Map.Find(L"神秘洞穴一层") >= 0)
			{
				Overtime = 0;
				
				while (GetObjAndClick(0, 0, 402, 186, "一层", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
				
				
			}
			else if(Map.Find(L"神秘洞穴二层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "二层", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			else if (Map.Find(L"神秘洞穴三层") >= 0)
			{
				Overtime = 0;
				while (GetObjAndClick(0, 0, 402, 186, "三层", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			Sleep(1000);
			Overtime = 0;
			if (Map.Find(L"绑金") >= 0)
			{

				while (GetObjAndClick(0, 0, 402, 186, "100进入", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			else
			{
				
				while (GetObjAndClick(0, 0, 402, 186, "神秘洞穴", "FFFF00-000000") < 0)
				{

					Sleep(1000);
					Overtime++;
					if (Overtime > 10)
					{
						return -1;
					}
				}
			}
			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{

			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}


		return 25;
	}

	//屠魔
	if (Map.Find(L"屠魔") >= 0 )
	{
		
		Sleep(2000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}

		ret=DevilSlayer(Jmp);
		if (ret<=0)
		{
			return -1;
		}
		Sleep(2000);
		GetUserInMap();
		if (wcscmp(L"屠魔房间", GameUser.InMap) == 0)
		{
			UseBack();
		}

		return 27;
	}

	//卧龙山庄]
	if (Map.Find(L"卧龙山庄") >= 0)
	{
		while (Jmp == false)
		{
			if (FindWay(330, 334) < 0)
			{
				return -1;
			}
			Sleep(1000);
			Overtime = 0;

			while (GetObjAndClick(0, 0, 402, 186, "卧龙山庄", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(38, 279, 107, 307, "活动大使", "00FF00-000000", 25, -37);
				FindWay(330, 334);
				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "卧龙山庄", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			Overtime = 0;
			while (GetObjAndClick(0, 0, 402, 186, "强化1000进入", "FFFF00-000000") < 0)
			{

				Sleep(1000);
				Overtime++;
				if (Overtime > 10)
				{
					return -1;
				}
			}

			break;
		}
		Sleep(1000);
		state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}
		if (AutoOn_lineCheck(NULL) > 19)
		{

			if (state == BST_CHECKED && IsSafetyArea() == 2)
			{
				TabPage3.m_eBackWait.GetWindowTextW(sTimes);
				time3 = _ttoi(sTimes);
				if (time3 > 0)
				{
					for (size_t i = 0; i < time3; i++)
					{
						if (IsSafetyArea() != 2)
						{
							break;
						}
						Sleep(1000);
					}
				}

			}
		}


		return 33;
	}


	//个人副本
	if (Map.Find(L"个人副本") >= 0 && PrivateMapOver)
	{

		Sleep(1000);
		/*state2 = TabPage2.m_cHideMaster.GetCheck();
		if (state2 == BST_CHECKED)
		{
			HideUseAndMaster(1);
		}
		else
		{
			HideUseAndMaster(0);
		}*/
		for (int i= PrivateMapOver; i < 4; i++)
		{
			ret = PrivateMap(i);
			if (ret <= 0)
			{
				continue;
			}
			PrivateMapOver++;
		}
		
		Sleep(2000);
		GetUserInMap();
		if (wcscmp(L"盟重省", GameUser.InMap) != 0)
		{
			UseBack();
		}

		PrivateMapOver = 0;

		return 27;
	}


	return 0;
}


int SupplyGoods()
{
	DWORD  times = 0;

	int istate1 = TabPage2.m_cSaveGoods.GetCheck();
	int istate2 = TabPage2.m_cSaveDrug.GetCheck();
	//int istate3 = TabPage2.m_cNoBlueStart.GetCheck();
	//int istate4 = TabPage2.m_cNoRedStart.GetCheck();


	//判断是否需要买药
	CStringW DurgNumb;
	int iLowRedDurgNumb = 0;
	int iNeedRedDurgNumb = 0;
	int PackRedDurgNumb = 0;
	//int iState = TabPage2.m_cNoRedStart.GetCheck();
	//if (iState==BST_CHECKED)
	//{
	TabPage2.m_eRedNumb.GetWindowTextW(DurgNumb);
	//红药最低数量
	iLowRedDurgNumb = _wtoi(DurgNumb.Mid(0, DurgNumb.Find(L"-")));
	//红药购买数量
	iNeedRedDurgNumb = _wtoi(DurgNumb.Mid(DurgNumb.Find(L"-") + 1));
	//背包红药已经有的数量
	PackRedDurgNumb = GetGoodsNumber(GameGoodsPresentSuperfgrdID);
	PackRedDurgNumb += GetGoodsNumber(105);
	//}

	int iLowBlueDurgNumb = 0;
	int iNeedBlueDurgNumb = 0;
	int PackBlueDurgNumb = 0;
	//int iState2 = TabPage2.m_cNoBlueStart.GetCheck();
	//if (iState2 == BST_CHECKED)
	//{
	DurgNumb.Empty();
	TabPage2.m_eBlueNumb.GetWindowTextW(DurgNumb);
	//蓝药最低数量
	iLowBlueDurgNumb = _wtoi(DurgNumb.Mid(0, DurgNumb.Find(L"-")));
	//蓝药购买数量
	iNeedBlueDurgNumb = _wtoi(DurgNumb.Mid(DurgNumb.Find(L"-") + 1));
	//背包蓝药已经有的数量
	PackBlueDurgNumb = GetGoodsNumber(GameGoodsPresentSuperMagicID);
	PackBlueDurgNumb += GetGoodsNumber(115);

	times = 0;
	while (GetGoodsNumber(GameGoodsSnowID) > 1)
	{
		UseGoodsCall(GameGoodsSnowID);
		Sleep(200);
		times++;
		if (times > 30)
		{
			return -4;
		}
	}
	times = 0;
	while (GetGoodsNumber(GameGoodsCureID) > 1)
	{
		UseGoodsCall(GameGoodsCureID);
		Sleep(200);
		times++;
		if (times > 30)
		{
			return -4;
		}
	}

	/*times = 0;
	while (GetGoodsNumber(GameGoodsfgrdID) > 1)
	{
		UseGoodsCall(GameGoodsfgrdID);
		Sleep(200);
		times++;
		if (times > 30)
		{
			return -4;
		}
	}
	while (GetGoodsNumber(GameGoodsPresentfgrdID) > 1)
	{
		UseGoodsCall(GameGoodsPresentfgrdID);
		Sleep(200);
		times++;
		if (times > 20)
		{
			return -4;
		}
	}
	times = 0;
	while (GetGoodsNumber(GameGoodsMagicDrugID) > 1)
	{
		UseGoodsCall(GameGoodsMagicDrugID);
		Sleep(200);
		times++;
		if (times > 30)
		{
			return -4;
		}
	}
	times = 0;
	while (GetGoodsNumber(GameGoodsPresentMagicDrugID) > 1)
	{
		UseGoodsCall(GameGoodsPresentMagicDrugID); 
		Sleep(200);
		times++;
		if (times > 30)
		{
			return -4;
		}
	}*/

	if (istate1 == BST_CHECKED || istate2 == BST_CHECKED || (iLowRedDurgNumb > PackRedDurgNumb) || (iLowBlueDurgNumb > PackBlueDurgNumb))
	{



		//移动到公告牌旁边
		times = 0;
		while (FindWay(338, 335) < 0)
		{
			times++;
			if (times > 10)
			{
				return -5;
			}
		};
		//清理窗口
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(1000);
		
		//隐藏人物
		HideUseAndMaster(1);
		Sleep(500);
		//点击公告牌
		NpcMoveSleep
		times = 0;;
		while (GetObjAndClick(0, 0, 402, 186, "使用移动", "FFFF00-000000", 3, 3, -1) < 0)
		{
			GetObjAndClick(584, 240, 645, 269, "公告牌", "00FF00-000000", 5, -77);
			FindWay(338, 335);

			Sleep(1000);
			times++;
			if (times > 10)
			{
				return -5;
			}
		}
		NpcMoveSleep
		//前往苍月岛
		times = 0;;
		while (GetObjAndClick(0, 0, 402, 186, "使用移动", "FFFF00-000000") < 0)
		{

			Sleep(1000);
			times++;
			if (times > 10)
			{
				return -2;
			}
		}
		moveTo(0, 0);
		Sleep(2000);
		times = 0;;
		while (GetObjAndClick(0, 0, 402, 186, "苍月岛", "FFFF00-000000") < 0)
		{
			Sleep(1000);
			times++;
			if (times > 10)
			{
				return -3;
			}
		}
		//到了开始寻路
		NpcMoveSleep
		//判断玩家是否传送到苍月岛
		do
		{
			GetUserX_YCoord();
			Sleep(1000);
		} while ((GameUser.x < 131 || GameUser.x > 151) || (GameUser.y < 324 || GameUser.y > 345));
		

	
		//检测背包雪霜和疗伤能不能打捆
		times = 0;
		if (GetGoodsNumber(GameGoodsSnowRopeID)>0)
		{
			while (GetGoodsNumber(GameGoodsSnowID) >= 6)
			{
				UseGoodsCall(GameGoodsSnowRopeID);
				Sleep(200);
				times++;
				if (times > 10)
				{
					break;
				}
			}
		}
		
		//检测背包雪霜和疗伤能不能打捆
		times = 0;
		if (GetGoodsNumber(GameGoodsCureRopeID) > 0)
		{
			while (GetGoodsNumber(GameGoodsCureID) >= 6)
			{
				UseGoodsCall(GameGoodsCureRopeID);
				Sleep(200);
				times++;
				if (times > 10)
				{
					break;
				}
			}
		}
		memset(SaveGoodsId, 0, sizeof(SaveGoodsId));
		DWORD Count = ChoseGoodsSave(SaveGoodsId);

	
		if ((istate1 == BST_CHECKED || istate2 == BST_CHECKED)|| Count!=0)
		{

			//寻路找仓库管理员
			times = 0;
			while (FindWay(132, 332) < 0)
			{
				times++;
				if (times > 10)
				{
					return -5;
				}
			};
			//去找仓库管理员操作
			for (size_t i = 0; i < 4; i++)
			{
				if (StoreManagement() == 1)
				{
					break;
				}

				if (i >= 4)
				{
					return -6;
				}
			}
		}

		
		
		//}
		

		//只有背包红药或者蓝药 低于要求数量就会去购买
		if ((iLowRedDurgNumb > PackRedDurgNumb) || (iLowBlueDurgNumb > PackBlueDurgNumb))
		{
			/*int iState = TabPage2.m_cNoRedStart.GetCheck();
			int iState2 = TabPage2.m_cNoBlueStart.GetCheck();
			if (iState == 1 || iState2 == 1)
			{*/
				//寻路去找药师
				times = 0;
				while (FindWay(133, 344) < 0)
				{
					times++;
					if (times > 10)
					{
						return -5;
					}
				};
				
				for (size_t i = 0; i < 4; i++)
				{
					if (DrugsManagement(iNeedRedDurgNumb - PackRedDurgNumb, iNeedBlueDurgNumb - PackBlueDurgNumb) == 1)
					{
						TabPage1.LogOut(L"药师完成");
						break;
					}

					if (i >= 4)
					{
						return -5;
					}
				}
			//}

		}
		//清理窗口
		pDM->KeyPress(27);
		Sleep(500);
		pDM->KeyPress(27);
		Sleep(500);
	

		//隐藏人物
		HideUseAndMaster(1);
		if (FindWay(137, 327) == 1)
		{
			Sleep(1500);
			times = 0;
			while (GetObjAndClick(10, 68, 402, 186, "前往盟重", "FFFF00-000000", 3, 3, -1) < 0)
			{
				GetObjAndClick(567, 173, 812, 337, "海盗老兵", "00FF00-000000", 26, -29);
				FindWay(137, 327);;

				Sleep(1000);
				times++;
				if (times > 10)
				{
					return -5;
				}
			}


			NpcMoveSleep
			times = 0;
			while (GetObjAndClick(10, 68, 402, 186, "前往盟重", "FFFF00-000000") < 0)
			{
				
				Sleep(1000);
				times++;
				if (times > 5)
				{
					return -1;
				}
			}
		}
		else
		{
			return -6;
		}
	}
	NpcMoveSleep
	for (size_t i = 0; i < 4; i++)
	{
		if (TrampNpcFunc() > 0)
		{
			TabPage1.LogOut(L"流浪汉完成");
			break;
		}

		if (i >= 4)
		{
			return -5;
		}
	}
	NpcMoveSleep
	for (size_t i = 0; i < 4; i++)
	{
		if (Shopping() > 0)
		{
			TabPage1.LogOut(L"商城完成");
			break;
		}

		if (i >= 4)
		{
			return -7;
		}
	}
	

	return 0;
}

BOOL CALLBACK EnumClientWindowsProc(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{

	GetWindowTextA(hwnd, WindowTitlebuffer, 256);

	GetClassNameA(hwnd, WindowClassbuffer, 256);
	if (strcmp(WindowClassbuffer, "Client Launcher") == 0)
	{

		GetWindowTextA(hwnd, WindowTitlebuffer, 256);
		if (*WindowTitlebuffer == NULL)
		{
			hClientWindow = hwnd;
			return false;
		}
		
	}


	return true;

}

int WriteClipboard(char* str)
{
	HWND hWnd = NULL;
	OpenClipboard(hWnd);//打开剪切板
	EmptyClipboard();//清空剪切板
	HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 0x21);//分配内存
	char* pData = (char*)GlobalLock(hHandle);//锁定内存，返回申请内存的首地址
	strcpy_s(pData, 0x21, str);//或strcpy(pData, "this is a ClipBoard Test.");
	SetClipboardData(CF_TEXT, hHandle);//设置剪切板数据
	GlobalUnlock(hHandle);//解除锁定
	CloseClipboard();//关闭剪切板


	pDM->KeyDown(17);
	Sleep(20);
	pDM->KeyPress(86);
	Sleep(20);
	pDM->KeyUp(17);
	return 0;
}

DWORD WINAPI AutoLoginGame(_In_ LPVOID lpParameter)
{
	HWND hAccount = NULL;
	HWND hError = NULL, hError2 = NULL;
	HWND hTmp = NULL;
	char cAccount[0x20]{};
	char cPassWord[0x20]{};
	

	int state=TabPage5.m_cGreenStart.GetCheck();
	int state2 = TabPage5.m_c173Start.GetCheck();

	GetWindowTextA(TabPage5.m_eAccount.m_hWnd, cAccount, 0x20);
	
	GetWindowTextA(TabPage5.m_ePassWord.m_hWnd,cPassWord, 0x20);
	hClientWindow = NULL;
	while (hClientWindow==NULL)
	{
		EnumWindows(EnumClientWindowsProc, NULL);
		Sleep(3000);
	}
	

	if (hClientWindow)
	{
		if (pDM->BindWindowEx((long)hClientWindow, "dx.graphic.3d", "dx.mouse.raw.input", "dx.keypad.state.api|dx.keypad.raw.input", "", 0) == 0)
		{
			TabPage1.LogOut(L"客户端绑定失败");
		}
	}
	

	Sleep(2000);
	moveTo(781, 471);
	Sleep(3000);
	if (state2 == BST_CHECKED)
	{
		for (size_t i = 0; i < 60; i++)
		{
			hTmp = FindWindowA("TfrmWebPage", "心游帐号登录");
			if (hTmp != NULL)
			{
				hTmp = FindWindowExA(hTmp, NULL, "Shell Embedding", "");
				if (hTmp != NULL)
				{
					hTmp = FindWindowExA(hTmp, NULL, "Shell DocObject View", "");
					if (hTmp != NULL)
					{
						hAccount = FindWindowExA(hTmp, NULL, "Internet Explorer_Server", "");
						if (hAccount != NULL)
						{
							pDM->UnBindWindow();
							break;
						}
					}
				}


			}
		}

		if (hAccount)
		{
			if (pDM->BindWindowEx((long)hAccount, "dx.graphic.3d", "dx.mouse.raw.input", "dx.keypad.state.api|dx.keypad.raw.input", "", 0) == 0)
			{
				TabPage1.LogOut(L"客户端绑定失败2");
			}
		}
		agin:
		Sleep(2000);
		moveTo(140, 69, 1);
		Sleep(1000);
		pDM->KeyPress(8);
		Sleep(1500);
		WriteClipboard(cAccount);
		Sleep(1000);
		moveTo(131, 106, 1);
		Sleep(1000);
		pDM->KeyPress(8);
		Sleep(1500);
		WriteClipboard(cPassWord);
		Sleep(1000);
		moveTo(158, 214);
		Sleep(1000);
		moveTo(158, 214);
		Sleep(2000);

		hError = FindWindowA("#32770", "来自网页的消息");
		if (hError != NULL)
		{
			hError2 = FindWindowExA(hError, NULL, "Button", "确定");
			if (hError2 != NULL)
			{
				SetActiveWindow(hError);
				SendMessageA(hError2, WM_LBUTTONDOWN, 0, 0);
				SendMessageA(hError2, WM_LBUTTONUP, 0, 0);
				Sleep(1000);
			}
			else
			{
				MessageBoxW(NULL, L"出错请查看", L"", NULL);
			}
		}
		hTmp = FindWindowA("TfrmWebPage", "心游帐号登录");
		if (hTmp != NULL)
		{
			goto agin;
		}
	}

	Sleep(15000);


	GameBase = GetGameBase(NULL);

	if (hGameWindow)
	{

		if (pDM->BindWindowEx((long)hGameWindow, "dx.graphic.3d", "dx.mouse.raw.input", "dx.keypad.state.api|dx.keypad.raw.input", "", 0) == 0)
		{
			TabPage1.LogOut(L"客户端绑定失败2");
		}

		if (GameBase != 0)
		{

			Sleep(2000);
			moveTo(548, 418);

			if (state == BST_CHECKED)
			{
				Sleep(3000);
				moveTo(545, 351, 1);
				Sleep(1000);
				pDM->KeyPress(8);
				Sleep(1000);
				WriteClipboard(cAccount);
				Sleep(1000);
				moveTo(538, 380, 1);
				Sleep(1000);
				pDM->KeyPress(8);
				Sleep(1000);
				WriteClipboard(cPassWord);
				Sleep(2000);
				moveTo(551, 447, 1);
			}

			Sleep(8000);
			moveTo(552, 607);
			pDM->UnBindWindow();

			Start();
		}
		
	}
	
	return 0;

}

int Start()
{
	SP_Begin(1);


	
	int state = TabPage2.m_cBackRollStart.GetCheck();
	int state2 = TabPage2.m_cBackRockStart.GetCheck();
	if (state == BST_UNCHECKED && state2 == BST_UNCHECKED)
	{
		MessageBoxW(NULL,L"请选择购买回城卷或者回城石", L"提示",NULL);
		return 0;
	}
	DWORD Threadid;
	//pDM->BindWindowEx((long)262350, "gdi2", "dx.mouse.raw.input", "normal", "", 0);

	CreateThread(NULL, 0, MyThreadFunction, NULL, NULL, &Threadid);

	Main->EndScript.SetCheck(BST_UNCHECKED);

	return 1;
	SP_End();
}


DWORD WINAPI MoreOPen(_In_ LPVOID lpParameter)
{
	DWORD Times = 30;
	DWORD pid = 0;
	DWORD Size = 0;
	while (Times>0)
	{

		hGameWindow2 = 0;
		pDM->UnBindWindow();

		while (hGameWindow2 == NULL)
		{
			EnumWindows(EnumWindowsProc2, NULL);
			Sleep(2000);
			
		}

		pMyGetAdaptersInfo = (pGetAdaptersInfo*)GetProcAddress(LoadLibraryA("iphlpapi.dll"), "GetAdaptersInfo");
		if (pMyGetAdaptersInfo != NULL)
		{

			if (hGameWindow2 != NULL)
			{
					GetWindowThreadProcessId(hGameWindow2, &pid);
					if (pid != NULL)
					{

						hMainProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
						if (hMainProcess != NULL)
						{

							if (WriteProcessMemory(hMainProcess, pMyGetAdaptersInfo, shell, 3, &Size) != 0)
							{
							
								if (WordLibSet)
								{
									
									TabPage1.LogOut(L"多开功能开启");

								}

							}


						}
					}
				


			}
		}
		Sleep(1000);
		Times--;
	}
	return 0;
}


int Login()
{
	
	CHAR lpFilename[200]{};
	char ExePath[200]{};
	STARTUPINFOA stStartUpInfo;
	PROCESS_INFORMATION ProcessInformation{};
	DWORD Threadid{};

	int state = TabPage5.m_cGreenStart.GetCheck();
	int state2 = TabPage5.m_c173Start.GetCheck();
	int state3 = TabPage5.m_cAutoLogin.GetCheck();
	if (state == BST_UNCHECKED && state2 == BST_UNCHECKED)
	{
		MessageBoxW(NULL,L"请选择启动平台", L"提示", NULL);
		return 0;
	}
	if (state == BST_CHECKED && state2 == BST_CHECKED)
	{
		MessageBoxW(NULL,L"请勿一次选择两个启动平台", L"提示", NULL);
		return 0;
	}

	GetWindowTextA(TabPage5.m_eGamePath.m_hWnd, ExePath, 125);

	if (*ExePath == NULL)
	{
		MessageBoxW(NULL, L"请填写游戏客户端路径", L"提示", NULL);
		return 0;
	}
	memcpy(lpFilename, ExePath, strlen(ExePath));

	int i;
	for (i = 200; lpFilename[i] != '\\'; i--);
	lpFilename[i] = 0;
	stStartUpInfo = { sizeof(stStartUpInfo) };
	bool start = CreateProcessA(ExePath, NULL, NULL, NULL, FALSE, NULL, NULL, lpFilename, &stStartUpInfo, &ProcessInformation);
	if (start == 0)
	{
		TabPage1.LogOut(L"游戏启动失败");
		return 0;
	}
	int a = 0;
	if (state3 == BST_CHECKED)
	{
		CreateThread(NULL, 0, AutoLoginGame, NULL, NULL, &Threadid);

	}
	else
	{
		CreateThread(NULL, 0, MoreOPen, NULL, NULL, &Threadid);
	}
	
	
	return 1;
}

DWORD WINAPI Reconnection(LPVOID lpParam)
{
	DWORD flag = 0;

	DWORD flag2 = 0;
	if (hMainProcess == NULL || hGameWindow == NULL)
	{
		return 0;
	}

	if (GameBase == 0)
	{
		return 0;
	}

	CString ctimes;
	int iTime = 0;
	int ret = 0;
	while (true)
	{
		//获取掉线标识
		if (ReadProcessMemory(hMainProcess, PDWORD(GameBase + UseOn_lineFlagOffset), &flag, sizeof(DWORD), &Size) != 0)
		{
			
			if (flag == 0)
			{

				if (hCheckProtectThread != NULL)
				{
					ret = TerminateThread(hCheckProtectThread, 1);
					CloseHandle(hCheckProtectThread);
					hCheckProtectThread = NULL;		//玩家保护线程句柄
				}


				Main->StartScript.SetCheck(BST_UNCHECKED);
				if (hRunScript != NULL)
				{
					ret = TerminateThread(hRunScript, 1);
					CloseHandle(hRunScript);
					hRunScript = NULL;			//运行脚本线程句柄
					bAutoFlag = false;
					//EndFlag = false;
				}
				if (hTime != NULL)
				{
					ret = TerminateThread(hTime, 1);
					CloseHandle(hTime);
					hTime = NULL;				//运行时间线程句柄
				}

				if (hShowInfor != NULL)
				{
					ret = TerminateThread(hShowInfor, 1);
					CloseHandle(hShowInfor);
					hShowInfor = NULL;				//运行时间线程句柄
				}
				TabPage1.LogOut(L"检测到游戏掉线");
				
				// 结束游戏
				::TerminateProcess(hMainProcess, 0);
				SaveGoodsoffset2 = 0;			//仓库ecx
				CheckHp = false;
				KnapsackList = 0;				//背包链表
				CheckHp = false;				//检测hp标志
				EndFlag = false;				//挂机线程标志
				ChoseGoodsDisintegrateEcx = 0;	//分解NPCecx


				hGameWindow = NULL;				//游戏窗口句柄
				hMainProcess = NULL;			//游戏进程句柄
				Initflag = true;
				TabPage5.m_eWaitTimes.GetWindowTextW(ctimes);
				iTime = _wtoi(ctimes);
				for (size_t i = 0; i < iTime; i++)
				{
					Sleep(1000);
				}

				Login();
				return 1;
			}
			
		}
		Sleep(5000);
	}
	

	return 0;
}

int SaveGameConfig()
{

	if (*GameUser.Name != NULL)
	{


		CString Name;
		CString Path;

		Name = GameUser.Name;
		Path = L".\\GameUserCongfig.ini";
		CString ret ;
		wchar_t wcName[0x10]{};
		size_t i = 0;
		for (; i < 20; i++)
		{
			ret.Format(L"%d", i);
			GetPrivateProfileStringW(L"User", ret,L"NULL", wcName,sizeof(wcName), Path);
			if (wcscmp(wcName,L"NULL")==0|| Name.Find(wcName)>=0)
			{
				break;
			}
		}
			
		
		
		
		//游戏角色
		WritePrivateProfileStringW(L"User", ret, Name, Path);
		int a=GetLastError();
		//--------------------------------基本设置-------------
		CString Key;
		int state = 0;
		//Key.Preallocate(0x100);
		//红药
		TabPage2.m_eRedNumb.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eRedNumb", Key, Path);
		state = TabPage2.m_cNoRedStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cNoRedStart", Key, Path);
		
		//蓝药
		TabPage2.m_eBlueNumb.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eBlueNumb", Key, Path);
		state = TabPage2.m_cNoBlueStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cNoBlueStart", Key, Path);
		
		//雪霜打捆
		TabPage2.m_eSnowNumb.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eSnowNumb", Key, Path);
		state = TabPage2.m_cSnowStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cSnowStart", Key, Path);
		
		//疗伤打捆
		TabPage2.m_eCureNumb.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eCureNumb", Key, Path);
		state = TabPage2.m_cCureStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cCureStart", Key, Path);
		
		//修复神水
		TabPage2.m_eFixWaterNumb.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eFixWaterNumb", Key, Path);
		state = TabPage2.m_cFixWaterStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cFixWaterStart", Key, Path);
		
		//随机石
		TabPage2.m_eRandNumb.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eRandNumb", Key, Path);
		state = TabPage2.m_cRandStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cRandStart", Key, Path);
		
		//回城石
		TabPage2.m_eBackNumb.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eBackNumb", Key, Path);
		state = TabPage2.m_cBackRockStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cBackRockStart", Key, Path);
		
		//回城卷
		TabPage2.m_eBackRollNumb.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eBackRollNumb", Key, Path);
		state = TabPage2.m_cBackRollStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cBackRollStart", Key, Path);
		//回城分解		
		state = TabPage2.m_cBackResolveStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cBackResolveStart", Key, Path);

		//回城回收	
		state = TabPage2.m_cBackRecycleStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cBackRecycleStart", Key, Path);
		
		// 背包剩余回城
		TabPage2.m_ePackSurplus.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_ePackSurplus", Key, Path);
		state = TabPage2.m_cPackSurplus.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cPackSurplus", Key, Path);
		
		//隐藏人物和怪物		
		state = TabPage2.m_cHideMaster.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cHideMaster", Key, Path);
		
		// 绿毒
		TabPage2.m_eGreenPoison.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eGreenPoison", Key, Path);
		state = TabPage2.m_cGreenPoison.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cGreenPoison", Key, Path);
		
		// 红毒
		TabPage2.m_eRedPoison.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eRedPoison", Key, Path);
		state = TabPage2.m_cRedPoison.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cRedPoison", Key, Path);
		
		// 防护符
		TabPage2.m_eAmulet.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eAmulet", Key, Path);
		state = TabPage2.m_cAmulet.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cAmulet", Key, Path);

		//道士毒怪	
		state = TabPage2.m_cPoisonMonster.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cPoisonMonster", Key, Path);
		

		// 自动打防
		state = TabPage2.m_cAutoDefend.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cAutoDefend", Key, Path);
		TabPage2.m_eTimes.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eTimes", Key, Path);

		// 法师放火
		state = TabPage2.m_cFire.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cFire", Key, Path);
		TabPage2.m_eFire.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eFire", Key, Path);

		
		
		// 保存药
		state = TabPage2.m_cSaveDrug.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cSaveDrug", Key, Path);

		// 保存物
		state = TabPage2.m_cSaveGoods.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cSaveGoods", Key, Path);

		//--------------------------------战斗设置-------------
		
		// 血少回城
		TabPage3.m_eLittleRedBack.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eLittleRedBack", Key, Path);
		state = TabPage3.m_cLittleRedBack.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cLittleRedBack", Key, Path);
		
		// 血少随机
		TabPage3.m_eLittleRedRand.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eLittleRedRand", Key, Path);
		state = TabPage3.m_cLittleRedRand.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cLittleRedRand", Key, Path);
		
		// 血少下线
		TabPage3.m_eLittleRedDown.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eLittleRedDown", Key, Path);
		state = TabPage3.m_cLittleRedDown.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cLittleRedDown", Key, Path);

		// 血少截图
		TabPage3.m_eLittleRedScreenshot.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eLittleRedScreenshot", Key, Path);
		state = TabPage3.m_cLittleRedScreenshot.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cLittleRedScreenshot", Key, Path);

		//攻击模式
		Key.Format(L"%d", TabPage3.iPattern);
		WritePrivateProfileStringW(Name, L"iPattern", Key, Path);
		

		//回城等待
		TabPage3.m_eBackWait.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eBackWait", Key, Path);
		state = TabPage3.m_cBackWait.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cBackWait", Key, Path);

		//进图随机
		state = TabPage3.m_cJoinRand.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cJoinRand", Key, Path);

		//时长无怪随机
		TabPage3.m_eTimeNoMasterRand.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eTimeNoMasterRand", Key, Path);
		state = TabPage3.m_cTimeNoMasterRand.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cTimeNoMasterRand", Key, Path);

		//周围无怪随机
		TabPage3.m_eRoundNoMasterRand.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eRoundNoMasterRand", Key, Path);
		state = TabPage3.m_cRoundNoMasterRand.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cRoundNoMasterRand", Key, Path);

		//金刚石装备回城		
		TabPage3.m_eAdamasEquipment.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eAdamasEquipment", Key, Path);
		state = TabPage3.m_cAdamasEquipment.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cAdamasEquipment", Key, Path);

		// 背包剩余多少格回收
		TabPage3.m_eKnapsackResidue.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eKnapsackResidue", Key, Path);
		state = TabPage3.m_cKnapsackResidue.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cKnapsackResidue", Key, Path);

		// 黑名单编辑框
		TabPage3.m_eBlackList.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eBlackList", Key, Path);

		// 黑名单回城
		state = TabPage3.m_cBlackListBack.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cBlackListBack", Key, Path);

		// 黑名单随机
		state = TabPage3.m_cBlackListRand.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cBlackListRand", Key, Path);

		// 上线攻击模式
		Key.Format(L"%d", TabPage3.iPattern);
		WritePrivateProfileStringW(Name, L"iPattern", Key, Path);


		// 挂机坐标点x编辑框
		TabPage3.m_eFitXY.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eFitXY", Key, Path);

		// 挂机坐标点
		state = TabPage3.m_cFitXY.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cFitXY", Key, Path);

		// 挂机y坐标
		TabPage3.m_EDIT_Y.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_EDIT_Y", Key, Path);

		// 坐标点归位时间
		TabPage3.m_eXYTime.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eXYTime", Key, Path);


		

		//--------------------------------进程设置-------------
		// 幻境难度迷幻
		state = TabPage4.m_cDifficulty1.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cDifficulty1", Key, Path);

		// 幻境绮梦难度
		state = TabPage4.m_cDifficulty2.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cDifficulty2", Key, Path);

		// 幻境噩梦难度
		state = TabPage4.m_cDifficulty3.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cDifficulty3", Key, Path);

		// 顺序进图
		state = TabPage4.m_cOrder.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cOrder", Key, Path);

		// 随机进图
		state = TabPage4.m_cRand.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cRand", Key, Path);

		// 流程表
		//int nNumb = TabPage4.m_LFlowPath.GetItemCount();


		CString tmp;
		wchar_t wcTmp[0x80]{};
		
		for (SIZE_T i = 0; i < 16; i++)
		{
			memset(wcTmp, 0, sizeof(wcTmp));
			LV_ITEM lvitem = { 0 };

			lvitem.iItem = i;
			lvitem.iSubItem = 0;
			lvitem.mask = LVIF_TEXT;
			lvitem.pszText = wcTmp;
			lvitem.cchTextMax = 0x80;
			TabPage4.m_LFlowPath.GetItem(&lvitem);
			tmp.Format(L"类型%d", i);
			WritePrivateProfileStringW(Name, tmp, wcTmp, Path);

			lvitem.iItem = i;
			lvitem.iSubItem = 1;
			lvitem.mask = LVIF_TEXT;
			lvitem.pszText = wcTmp;
			lvitem.cchTextMax = 0x80;
			TabPage4.m_LFlowPath.GetItem(&lvitem);
			tmp.Format(L"时间%d", i);
			WritePrivateProfileStringW(Name, tmp, wcTmp, Path);


			lvitem.iItem = i;
			lvitem.iSubItem = 2;
			lvitem.mask = LVIF_TEXT;
			lvitem.pszText = wcTmp;
			lvitem.cchTextMax = 0x80;
			TabPage4.m_LFlowPath.GetItem(&lvitem);
			tmp.Format(L"地图%d", i);
			WritePrivateProfileStringW(Name, tmp, wcTmp, Path);

		}

		//低绑不进
		state = TabPage4.m_cBindMoney.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cBindMoney", Key, Path);

		TabPage4.m_eBindMoney.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eBindMoney", Key, Path);


		//--------------------------------账号设置-------------

		// 绿色平台
		state = TabPage5.m_cGreenStart.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cGreenStart", Key, Path);

		// 173平台
		state = TabPage5.m_c173Start.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_c173Start", Key, Path);

		// 游戏路径
		TabPage5.m_eGamePath.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eGamePath", Key, Path);

		// 账号
		TabPage5.m_eAccount.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eAccount", Key, Path);

		// 密码
		TabPage5.m_ePassWord.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_ePassWord", Key, Path);

		// 自动登录
		state = TabPage5.m_cAutoLogin.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cAutoLogin", Key, Path);

		// 掉线重连
		state = TabPage5.m_cAutoContinue.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_cAutoContinue", Key, Path);

		//关闭组队
		state = TabPage5.m_CloseTeam.GetCheck();
		Key.Format(L"%d", state);
		WritePrivateProfileStringW(Name, L"m_CloseTeam", Key, Path);

		// 掉线等待时长
		TabPage5.m_eWaitTimes.GetWindowTextW(Key);
		WritePrivateProfileStringW(Name, L"m_eWaitTimes", Key, Path);

		//Key.ReleaseBuffer(0x100);
		TabPage1.LogOut("配置保存成功");
	}
	
	return 1;
}

int ShowGameConfig()
{
	CString Name;
	CString Path;

	
	Path = L".\\GameUserCongfig.ini";
	CString ret;
	wchar_t wcName[0x10]{};
	size_t i = 0;
	Main->m_cbCongfig.ResetContent();
	for (; i < 40; i++)
	{
		ret.Format(L"%d", i);
		GetPrivateProfileStringW(L"User", ret, L"NULL", wcName, sizeof(wcName), Path);
		if (wcscmp(wcName, L"NULL") == 0)
		{
			break;
		}
		Main->m_cbCongfig.AddString(wcName);

	}

	
	return 0;
}

int ReadGameConfig()
{
	CString Name;
	CString Path;
	int nIndex = Main->m_cbCongfig.GetCurSel();

	
	Main->m_cbCongfig.GetLBText(nIndex, Name);

	Path = L".\\GameUserCongfig.ini";
	

	//--------------------------------基本设置-------------
	CString Key;
	Key.Preallocate(0x100);
	//红药
	GetPrivateProfileStringW(Name, L"m_eRedNumb",L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eRedNumb.SetWindowTextW(Key);
	nIndex=GetPrivateProfileIntW(Name, L"m_cNoRedStart", 0, Path);
	TabPage2.m_cNoRedStart.SetCheck(nIndex);

	//蓝药
	GetPrivateProfileStringW(Name, L"m_eBlueNumb", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eBlueNumb.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cNoBlueStart", 0, Path);
	TabPage2.m_cNoBlueStart.SetCheck(nIndex);


	//雪霜打捆
	GetPrivateProfileStringW(Name, L"m_eSnowNumb", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eSnowNumb.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cSnowStart", 0, Path);
	TabPage2.m_cSnowStart.SetCheck(nIndex);


	//疗伤打捆
	GetPrivateProfileStringW(Name, L"m_eCureNumb", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eCureNumb.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cCureStart", 0, Path);
	TabPage2.m_cCureStart.SetCheck(nIndex);


	//修复神水
	GetPrivateProfileStringW(Name, L"m_eFixWaterNumb", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eFixWaterNumb.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cFixWaterStart", 0, Path);
	TabPage2.m_cFixWaterStart.SetCheck(nIndex);


	//随机石
	GetPrivateProfileStringW(Name, L"m_eRandNumb", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eRandNumb.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cRandStart", 0, Path);
	TabPage2.m_cRandStart.SetCheck(nIndex);


	//回城石
	GetPrivateProfileStringW(Name, L"m_eBackNumb", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eBackNumb.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cBackRockStart", 0, Path);
	TabPage2.m_cBackRockStart.SetCheck(nIndex);


	//回城卷
	GetPrivateProfileStringW(Name, L"m_eBackRollNumb", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eBackRollNumb.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cBackRollStart", 0, Path);
	TabPage2.m_cBackRollStart.SetCheck(nIndex);


	//回城分解		
	nIndex = GetPrivateProfileIntW(Name, L"m_cBackResolveStart", 0, Path);
	TabPage2.m_cBackResolveStart.SetCheck(nIndex);

	//回城回收	
	nIndex = GetPrivateProfileIntW(Name, L"m_cBackRecycleStart", 0, Path);
	TabPage2.m_cBackRecycleStart.SetCheck(nIndex);


	// 背包剩余回城
	GetPrivateProfileStringW(Name, L"m_ePackSurplus", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_ePackSurplus.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cPackSurplus", 0, Path);
	TabPage2.m_cPackSurplus.SetCheck(nIndex);

	//隐藏人物和怪物		
	nIndex = GetPrivateProfileIntW(Name, L"m_cHideMaster", 0, Path);
	TabPage2.m_cHideMaster.SetCheck(nIndex);

	// 绿毒
	GetPrivateProfileStringW(Name, L"m_eGreenPoison", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eGreenPoison.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cGreenPoison", 0, Path);
	TabPage2.m_cGreenPoison.SetCheck(nIndex);


	// 红毒
	GetPrivateProfileStringW(Name, L"m_eRedPoison", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eRedPoison.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cRedPoison", 0, Path);
	TabPage2.m_cRedPoison.SetCheck(nIndex);


	// 防护符
	GetPrivateProfileStringW(Name, L"m_eAmulet", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eAmulet.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cAmulet", 0, Path);
	TabPage2.m_cAmulet.SetCheck(nIndex);

	//道士毒怪
	nIndex = GetPrivateProfileIntW(Name, L"m_cPoisonMonster", 0, Path);
	TabPage2.m_cPoisonMonster.SetCheck(nIndex);


	// 自动打防
	nIndex = GetPrivateProfileIntW(Name, L"m_cAutoDefend", 0, Path);
	TabPage2.m_cAutoDefend.SetCheck(nIndex);

	GetPrivateProfileStringW(Name, L"m_eTimes", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eTimes.SetWindowTextW(Key);

	// 法师放火
	nIndex = GetPrivateProfileIntW(Name, L"m_cFire", 0, Path);
	TabPage2.m_cFire.SetCheck(nIndex);

	GetPrivateProfileStringW(Name, L"m_eFire", L"", Key.GetBuffer(), 0x100, Path);
	TabPage2.m_eFire.SetWindowTextW(Key);




	// 保存药
	nIndex = GetPrivateProfileIntW(Name, L"m_cSaveDrug", 0, Path);
	TabPage2.m_cSaveDrug.SetCheck(nIndex);


	// 保存物
	nIndex = GetPrivateProfileIntW(Name, L"m_cSaveGoods", 0, Path);
	TabPage2.m_cSaveGoods.SetCheck(nIndex);


	//--------------------------------战斗设置-------------

	// 血少回城
	GetPrivateProfileStringW(Name, L"m_eLittleRedBack", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eLittleRedBack.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cLittleRedBack", 0, Path);
	TabPage3.m_cLittleRedBack.SetCheck(nIndex);



	// 血少随机
	GetPrivateProfileStringW(Name, L"m_eLittleRedRand", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eLittleRedRand.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cLittleRedRand", 0, Path);
	TabPage3.m_cLittleRedRand.SetCheck(nIndex);



	// 血少下线
	GetPrivateProfileStringW(Name, L"m_eLittleRedDown", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eLittleRedDown.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cLittleRedDown", 0, Path);
	TabPage3.m_cLittleRedDown.SetCheck(nIndex);


	// 血少截图
	GetPrivateProfileStringW(Name, L"m_eLittleRedScreenshot", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eLittleRedScreenshot.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cLittleRedScreenshot", 0, Path);
	TabPage3.m_cLittleRedScreenshot.SetCheck(nIndex);


	//回城等待
	GetPrivateProfileStringW(Name, L"m_eBackWait", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eBackWait.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cBackWait", 0, Path);
	TabPage3.m_cBackWait.SetCheck(nIndex);


	//进图随机
	nIndex = GetPrivateProfileIntW(Name, L"m_cJoinRand", 0, Path);
	TabPage3.m_cJoinRand.SetCheck(nIndex);

	//攻击模式
	nIndex = GetPrivateProfileIntW(Name, L"iPattern", 0, Path);
	TabPage3.iPattern = nIndex;

	//时长无怪随机
	GetPrivateProfileStringW(Name, L"m_eTimeNoMasterRand", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eTimeNoMasterRand.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cTimeNoMasterRand", 0, Path);
	TabPage3.m_cTimeNoMasterRand.SetCheck(nIndex);

	//周围无怪随机
	GetPrivateProfileStringW(Name, L"m_eRoundNoMasterRand", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eRoundNoMasterRand.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cRoundNoMasterRand", 0, Path);
	TabPage3.m_cRoundNoMasterRand.SetCheck(nIndex);


	//金刚石装备回城		
	GetPrivateProfileStringW(Name, L"m_eAdamasEquipment", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eAdamasEquipment.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cAdamasEquipment", 0, Path);
	TabPage3.m_cAdamasEquipment.SetCheck(nIndex);


	// 背包剩余多少格回收
	GetPrivateProfileStringW(Name, L"m_eKnapsackResidue", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eKnapsackResidue.SetWindowTextW(Key);
	nIndex = GetPrivateProfileIntW(Name, L"m_cKnapsackResidue", 0, Path);
	TabPage3.m_cKnapsackResidue.SetCheck(nIndex);


	// 黑名单编辑框
	GetPrivateProfileStringW(Name, L"m_eBlackList", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eBlackList.SetWindowTextW(Key);

	// 黑名单回城
	nIndex = GetPrivateProfileIntW(Name, L"m_cBlackListBack", 0, Path);
	TabPage3.m_cBlackListBack.SetCheck(nIndex);


	// 黑名单随机
	nIndex = GetPrivateProfileIntW(Name, L"m_cBlackListRand", 0, Path);
	TabPage3.m_cBlackListRand.SetCheck(nIndex);

	// 上线攻击模式
	GetPrivateProfileStringW(Name, L"iPattern", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.iPattern = _wtoi(Key);



	// 挂机坐标点x编辑框
	GetPrivateProfileStringW(Name, L"m_eFitXY", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eFitXY.SetWindowTextW(Key);


	// 挂机坐标点
	nIndex = GetPrivateProfileIntW(Name, L"m_cFitXY", 0, Path);
	TabPage3.m_cFitXY.SetCheck(nIndex);


	// 挂机y坐标
	GetPrivateProfileStringW(Name, L"m_EDIT_Y", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_EDIT_Y.SetWindowTextW(Key);


	// 坐标点归位时间
	GetPrivateProfileStringW(Name, L"m_eXYTime", L"", Key.GetBuffer(), 0x100, Path);
	TabPage3.m_eXYTime.SetWindowTextW(Key);


	//--------------------------------进程设置-------------
	// 幻境难度迷幻
	nIndex = GetPrivateProfileIntW(Name, L"m_cDifficulty1", 0, Path);
	TabPage4.m_cDifficulty1.SetCheck(nIndex);

	// 幻境绮梦难度
	nIndex = GetPrivateProfileIntW(Name, L"m_cDifficulty2", 0, Path);
	TabPage4.m_cDifficulty2.SetCheck(nIndex);


	// 幻境噩梦难度
	nIndex = GetPrivateProfileIntW(Name, L"m_cDifficulty3", 0, Path);
	TabPage4.m_cDifficulty3.SetCheck(nIndex);


	// 顺序进图
	nIndex = GetPrivateProfileIntW(Name, L"m_cOrder", 0, Path);
	TabPage4.m_cOrder.SetCheck(nIndex);

	// 随机进图
	nIndex = GetPrivateProfileIntW(Name, L"m_cRand", 0, Path);
	TabPage4.m_cRand.SetCheck(nIndex);


	// 流程表
	DWORD nNumb = TabPage4.m_LFlowPath.GetItemCount();


	CString tmp;
	tmp.Preallocate(0x100);

	for (SIZE_T i = 0; i < 16; i++)
	{
		tmp.Format(L"类型%d", i);
		GetPrivateProfileStringW(Name, tmp, L"", Key.GetBuffer(), 0x100, Path);
		
		if (*(Key.GetBuffer()) == 0)
		{
			break;
		}

		TabPage4.m_LFlowPath.InsertItem(i, Key);
		
		tmp.Format(L"时间%d", i);
		GetPrivateProfileStringW(Name, tmp, L"", Key.GetBuffer(), 0x100, Path);
		if (*(Key.GetBuffer()) == 0)
		{
			break;
		}

		TabPage4.m_LFlowPath.SetItemText( i, 1, Key);

		
		tmp.Format(L"地图%d", i);
		GetPrivateProfileStringW(Name, tmp, L"", Key.GetBuffer(), 0x100, Path);
		if (*(Key.GetBuffer()) == 0)
		{
			break;
		}
		
		TabPage4.m_LFlowPath.SetItemText( i, 2, Key);

		TabPage4.ListNumb += 1;

	}

	//低绑不进
	
	nIndex = GetPrivateProfileIntW(Name, L"m_cBindMoney", 0, Path);
	TabPage4.m_cBindMoney.SetCheck(nIndex);

	GetPrivateProfileStringW(Name, L"m_eBindMoney", L"", Key.GetBuffer(), 0x100, Path);
	TabPage4.m_eBindMoney.SetWindowTextW(Key);

	//--------------------------------账号设置-------------

	// 绿色平台
	nIndex = GetPrivateProfileIntW(Name, L"m_cGreenStart", 0, Path);
	TabPage5.m_cGreenStart.SetCheck(nIndex);


	// 173平台
	nIndex = GetPrivateProfileIntW(Name, L"m_c173Start", 0, Path);
	TabPage5.m_c173Start.SetCheck(nIndex);


	// 游戏路径
	GetPrivateProfileStringW(Name, L"m_eGamePath", L"", Key.GetBuffer(), 0x100, Path);
	TabPage5.m_eGamePath.SetWindowTextW(Key);


	// 账号
	GetPrivateProfileStringW(Name, L"m_eAccount", L"", Key.GetBuffer(), 0x100, Path);
	TabPage5.m_eAccount.SetWindowTextW(Key);


	// 密码
	GetPrivateProfileStringW(Name, L"m_ePassWord", L"", Key.GetBuffer(), 0x100, Path);
	TabPage5.m_ePassWord.SetWindowTextW(Key);


	// 自动登录
	nIndex = GetPrivateProfileIntW(Name, L"m_cAutoLogin", 0, Path);
	TabPage5.m_cAutoLogin.SetCheck(nIndex);


	// 掉线重连
	nIndex = GetPrivateProfileIntW(Name, L"m_cAutoContinue", 0, Path);
	TabPage5.m_cAutoContinue.SetCheck(nIndex);

	//关闭组队
	nIndex = GetPrivateProfileIntW(Name, L"m_CloseTeam", 0, Path);
	TabPage5.m_CloseTeam.SetCheck(nIndex);

	// 掉线等待时长
	GetPrivateProfileStringW(Name, L"m_eWaitTimes", L"", Key.GetBuffer(), 0x100, Path);
	TabPage5.m_eWaitTimes.SetWindowTextW(Key);

	return 1;
}



DWORD WINAPI FitUser(_In_ LPVOID lpParameter)
{
	if (hGameWindow)
	{
		if (pDM->BindWindowEx((long)hGameWindow, "dx.graphic.3d", "dx.mouse.raw.input", "dx.keypad.state.api|dx.keypad.raw.input", "", 0) == 0)
		{
			TabPage1.LogOut(L"辅助绑定失败1");
		}

	}
	else
	{
		TabPage1.LogOut(L"辅助绑定失败2");
	}
	while (bFitFlag)
	{

		GetUserInMap();
		//如果是在安全区结束挂机检测
		if (StrCmpW(L"盟重省", GameUser.InMap) == 0 || StrCmpW(L"苍月岛", GameUser.InMap) == 0 || StrCmpW(L"盟重省(跨服)", GameUser.InMap) == 0)
		{
		}
		else
		{
			BlackFunc(true);
			Sleep(50);
		}
		
	}
	return 0;
}


int ChoseObjUseKill(int KillID,DWORD* ObjId)
{
	
	if (GameUser.Infor != 0)
	{
		
		if (PoisonMonsterFlag)
		{
			UseSkillCall(KillID);
		}
			
		if (WriteProcessMemory(hMainProcess, PDWORD(GameUser.Infor + GameSkillMasterObj), ObjId, 4, &Size) != 0)
		{
			
			FindNearWay(Scope);
			Sleep(1000);
			UseSkillCall(KillID);
			PoisonMonsterFlag = true;
			//pDM->KeyPress(49);

			return 1;

		}
		
	}
	return 0;
}


int FindNearWay(int Coord)
{
	int CoordX1 = 0;
	int lCoordX1 = 0;
	int CoordY1 = 0;
	int lCoordY1 = 0;
	

	int CoordX2 = 0;
	int lCoordX2 = 0;
	int CoordY2 = 0;
	int lCoordY2 = 0;
	

	int CoordX3 = 0;
	int lCoordX3 = 0;
	int CoordY3 = 0;
	int lCoordY3 = 0;
	

	int CoordX4 = 0;
	int lCoordX4 = 0;
	int CoordY4 = 0;
	int lCoordY4 = 0;

	int CoordX5 = 0;
	int lCoordX5 = 0;
	int CoordY5 = 0;
	int lCoordY5 = 0;

	int CoordX6 = 0;
	int lCoordX6 = 0;
	int CoordY6 = 0;
	int lCoordY6 = 0;

	int CoordX7 = 0;
	int lCoordX7 = 0;
	int CoordY7 = 0;
	int lCoordY7 = 0;

	int Min = 0;

	int X_YIndex{};
	int value[7]{};
	int ret = -1;
	if (Coord == 0)
	{

		if (GetUserX_YCoord() <= 0)
		{
			TabPage1.LogOut(L"毒怪获取坐标出错");
		}

		//判断玩家最近坐标点   60,81  84,52   75,15   48,19     23,42   21,69   31,81
		CoordX1 = GameUser.x - 60;
		lCoordX1 = labs(CoordX1);
		CoordY1 = GameUser.y - 81;
		lCoordY1 = labs(CoordY1);
		value[0] = lCoordX1 + lCoordY1;

		CoordX2 = GameUser.x - 84;
		lCoordX2 = labs(CoordX2);
		CoordY2 = GameUser.y - 52;
		lCoordY2 = labs(CoordY2);
		value[1] = lCoordX2 + lCoordY2;

		CoordX3 = GameUser.x - 75;
		lCoordX3 = labs(CoordX3);
		CoordY3 = GameUser.y - 15;
		lCoordY3 = labs(CoordY3);
		value[2] = lCoordX3 + lCoordY3;

		CoordX4 = GameUser.x - 48;
		lCoordX4 = labs(CoordX4);
		CoordY4 = GameUser.y - 19;
		lCoordY4 = labs(CoordY4);
		value[3] = lCoordX4 + lCoordY4;

		CoordX5 = GameUser.x - 23;
		lCoordX5 = labs(CoordX4);
		CoordY5 = GameUser.y - 42;
		lCoordY5 = labs(CoordY4);
		value[4] = lCoordX5 + lCoordY5;

		CoordX6 = GameUser.x - 21;
		lCoordX6 = labs(CoordX4);
		CoordY6 = GameUser.y - 69;
		lCoordY6 = labs(CoordY4);
		value[5] = lCoordX6 + lCoordY6;

		CoordX7 = GameUser.x - 31;
		lCoordX7 = labs(CoordX4);
		CoordY7 = GameUser.y - 81;
		lCoordY7 = labs(CoordY4);
		value[6] = lCoordX7 + lCoordY7;
		Min = value[0];
		for (size_t i = 0; i < 7; i++)
		{
			if (Min > value[i])
			{
				Min = value[i];
				X_YIndex = i;
			}
		}

		if (X_YIndex = 0)
		{
			WayfindingCall(60, 81);
			ret = 1;
		}
		else if (X_YIndex = 1)
		{
			WayfindingCall(84, 52);
			ret = 2;
		}
		else if (X_YIndex = 2)
		{
			WayfindingCall(75, 15);
			ret = 3;
		}
		else if (X_YIndex = 3)
		{
			WayfindingCall(48, 19);
			ret = 4;
		}
		else if (X_YIndex = 4)
		{
			WayfindingCall(23, 42);
			ret = 5;
		}
		else if (X_YIndex = 5)
		{
			WayfindingCall(21, 69);
			ret = 6;
		}
		else if (X_YIndex = 6)
		{
			WayfindingCall(31, 81);
			ret = 7;
		}
	}
	else if(Coord == 1)
	{
		WayfindingCall(60, 81);
	}
	else if (Coord == 2)
	{
		WayfindingCall(84, 52);
	}
	else if (Coord == 3)
	{
		WayfindingCall(75, 15);
	}
	else if (Coord == 4)
	{
		WayfindingCall(48, 19);
	}
	else if (Coord == 5)
	{
		WayfindingCall(23, 42);
	}
	else if (Coord == 6)
	{
		WayfindingCall(21, 69);
	}
	else if (Coord == 7)
	{
		WayfindingCall(31, 81);
	}


	return ret;
}

int IsObjInScope(OtherUser* sObj,int Scope)
{
	if (sObj == NULL)
	{
		return 0;
	}

	DWORD Ret = 0;
	GetUserX_YCoord();


	if (((sObj->x > (GameUser.x - Scope)) && (sObj->x < (GameUser.x + Scope))) && ((sObj->y > (GameUser.y - Scope)) && (sObj->y < (GameUser.y + Scope))))
	{
		return 1;
	}

	return 0;

}

int SelectNearObject()
{
	int ret = 0;
	DWORD Falg1 = 0;
	if (ObjList != 0)
	{
		
		memset(GameOtherMaster, 0, sizeof(GameOtherMaster));
		if (GameBase != 0)
		{
			ret = InitUserInfor(GameBase,1);
		}

		if (ret >0)
		{
			for (size_t i = 0; i < MasterNumb; )
			{
				if (GameOtherMaster[i].UseFlag == 0x14 && GameOtherMaster[i].RetinueFlag != 0xF && IsObjInScope(&GameOtherMaster[i],6))
				{
					//判断是不是已经死了的怪
					if (ReadProcessMemory(hMainProcess, PDWORD(GameOtherMaster[i].ObjAddress + 0xF4), &Falg1, 4, &Size) != 0)
					{
						if (Falg1 == 0x60118)
						{
							i++;
							continue;
						}
					}
					//判断是否已经遍历过的怪
					if (GameOtherMaster[i].ObjID != 0)
					{
						for (size_t y = 0; y < 90 ; y++)
						{
							if (ObjID[y] == GameOtherMaster[i].ObjID)
							{
								goto Next;
							}
						}
						if (IdIndex >= 90)
						{
							IdIndex = 0;
						}
						ObjID[IdIndex++] = GameOtherMaster[i].ObjID;
						ChoseObjUseKill(0x12F, &ObjID[IdIndex-1]);
						return 2;
					}
					else
					{
						i++;
						continue;
					}
				}
			Next:
				i++;
			}
			return 3;

		}
		else
		{
			TabPage1.LogOut(L"毒怪周围怪物信息刷新失败");
		}
	}

	return 1;
}

int ix = 0 , iy = 0;

int PoisonMonster(int Scope)
{
	//60,81  84,52   75,15   48,19     23,42   21,69   31,81
	if (SelectNearObject() > 0)
	{
		
		GetUserX_YCoord();
		if ((ix != GameUser.x)|| (iy != GameUser.y))
		{
			ix = GameUser.x;
			iy = GameUser.y;
			FindNearWay(Scope);
			return 1;
		}
		else
		{	
			Sleep(1000);
			GetUserX_YCoord();
			if ((ix != GameUser.x) || (iy != GameUser.y))
			{
				ix = GameUser.x;
				iy = GameUser.y;
				SelectNearObject();
				FindNearWay(Scope);
				return 1;
			}
			Sleep(1000);
			GetUserX_YCoord();
			if ((ix != GameUser.x) || (iy != GameUser.y))
			{
				ix = GameUser.x;
				iy = GameUser.y;
				SelectNearObject();
				FindNearWay(Scope);
				return 1;
			}
			while (true)
			{
				if (SelectNearObject() == 3)
				{
					break;
				}
				Sleep(1000);
			}
			//使用随机石
			TabPage1.LogOut("毒怪被堵使用随机石");
			UseGoodsCall(GameGoodsRandRockID);
			Sleep(200);
			SetOn_lineState(0);
			PoisonMonsterFlag = false;

			return 2;
			
		}	
	}
	
	return 0;
}

HWND hwnd3 = NULL;
BOOL CALLBACK EnumErrorProc(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{
	
	
	if (hwnd != NULL)
	{
		hwnd3 = GetDlgItem(hwnd, 2);
		if (hwnd3 != NULL)
		{
			SendMessageA(hwnd3, BM_CLICK, 0, 0);
			hwnd3 = NULL;
			return false;
		}
	}

	return true;

}


DWORD WINAPI ClickError(_In_ LPVOID lpParameter)
{
	HWND hwnd = NULL;
	HWND hwnd2 = NULL;
	
	
	
	while (true)
	{
		EnumWindows(EnumErrorProc, NULL);
		Sleep(1000);
	}
	return 0;
}