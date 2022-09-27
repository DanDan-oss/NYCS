#pragma once
//------------------------------------------------------------------------玩家

extern DWORD GameBase;													//游戏基址
#define GameUseBaseOffset 0x9cc1D0										//*玩家首地址				33C58945F05657508D45??64A3000000008B0D????????85C90F84????????+0x14
//#define GameNpcStateOffset 0xAA4B50									//游戏Npc对话框打开状态偏移  1打开 0关闭	
#define GameKnapsackBaseOffset 0x98Ab9c									//*背包首地址				558BEC57FF75??8BF9E8???????? 837D??0075??833D???????? +0x16
#define GameUserMaxHpBaseOffset 0x9cba80								//*玩家自己最大血			8BCFFF35????????FF35????????50680E030000E8????????FF4424??FF4C24??+0x4
#define GameUserMoneyOffset 0x9cbaad									//*玩家金币
#define GameUserBindingMoneyOffset GameUserMoneyOffset+0x58				//玩家绑定金币
#define GameUserExpOffset 0x9cba95										//*玩家经验偏移					8B15????????8B3D????????8B1D????????660FD64424+0x2
#define UseOn_lineFlagOffset 0x98b158									//*人物在线状态  1在线 0掉线    558BEC5333DB391D????????0F84????????578B7D??85FF+0x9
#define AutoTraceOffset 0x9cb37c										//*自动追敌标识偏移				833D????????0075??33C0EB??B80100000083FE24  +0x2
#define HideOtherUseOffset 0x98abc0										//*隐藏玩家偏移					85C074??8B068BCEFF50??833D????????0074??83F824 +0xe
//#define GameObj *(int*)(*((int*)(*((int*)(GameBase+0xAE2FF8))+0x1E0))+0)//对象链表
#define GameObjFlagOffset 0xD											//对象标志
#define GameObjInformationOffset 0x4									//对象详细信息和功能
#define GameObjFlag 0x0													//是0就是对象  1是链表尾  一个字节
#define GameObjOffset 0x14												//0x14是对象地址
#define GameUserNameLenOffset 0x2c										//内容大于等于8+0x1c是个名字地址
#define GameUserMapNameLenOffset 0x28									//内容大于等于8+0x14是个名字地址
#define GameUserNameOffset 0x1c											//玩家名称
#define GameUserXOffset 0x4c											//玩家x坐标
#define GameUserYOffset 0x50											//玩家y坐标
#define GameUserInfoOffset 0x4											//玩家个人状态状态
#define GameUserInMapOffset 0x14										//所在地图字符串   char*
#define GameOBJIDFlagOffset 0x8											//对象专属id
#define GameUserFlagOffset 0xC											//0x24 玩家   0xC npc   0x14怪物
#define GameRetinueFlagOffset 0x30										//0x30 0xF随从    7随从NPC怪物
#define GameMeFlagOffset 0x88											//玩家自己标识偏移
#define GameMeFlag 0x7FFFFFFF											//0x7FFFFFFF玩家自己标识
#define GameUserHpOffSet 0x94											//玩家自己当前血量偏移
#define GameUserMaxHpOffSet 0x98										//玩家自己最大血量偏移
#define GameUserOccupationFlagOffset 0x90								//0x301 男道士  0x302 女道士  0x201男法师   0x202女法师  0x101男战士   0x102女战士
//#define GameUserEquipFlagOffset 0xb0									// 装备标识偏移

#define HideTaskbarOffset AutoTraceOffset+0xD0							//显示任务栏偏移											
#define ChoseGoodsDisintegrateEcxOffset 0xFDC							//选择分解callEcx
#define ChoseGoodsSaveEcxOffset 0xE70									//选择存仓库callEcx
#define GameOn_lineTirps AutoTraceOffset+0xD4							//挂机提示
#define GameAutoFixOffset AutoTraceOffset+0xF0							//自动修理
#define GameAutoPickOffset AutoTraceOffset+0xF4							//自动拾取
#define UseOn_lineFlagOffset2 0x50										//人物在线状态2
#define On_LineStateOffset 0x240										//人物挂机状态 偏移 1挂机 0不在挂机
#define GameMouseXOffset 0x134											//游戏内存鼠标x偏移
#define GameMouseYOffset 0x138											//游戏内存鼠标y偏移
#define GameMasterObj 0x188												//选中怪物对象
#define GameSkillMasterObj 0x194										//技能选中怪物对象

#define UseFitStateOffset 0x1010										//人物攻击模式
#define SafetyAreaOffset 0x2D8											//人物安全区 偏移 0不在其他在  6安全区
#define UseUseNowStateOffset 0xEC										//人物运动状态  6停止 1战斗
#define GameMasterDieFlagOffset 0x100									//怪物死亡尸体标志

//------------------------------------------------------------------------背包
#define GameGoodsMaxNumber 45										//背包最大数量
#define GameGoodsNameOffset 0x1										// 物品名称
#define GameGoodsNumbOffset 0x3d									// 物品数量
#define GameGoodsIDOffset 0x25										// 物品id 
#define GameGoodsChoseIDOffset 0x21									// 物品选择id 
#define GameUserEquipFlagOffset 0x48								// 金刚石 装备偏移
#define GameUserGoodsFlagOffset 0x4c								// 物品 种类
#define GameGoodsRockFlag 0x300										// 宝石物品 种类
#define GameUserEquipFlag 0x28										// 28金刚石 套装装备标识
#define GameUserEquipFlag2 0x38										// 38金刚石 非套装装备标识
//#define GameGoodsFlagOffset 0x28									// 物品伪id  雪霜捆绳和疗伤捆绳一样

//万年雪霜  127       修复神水 138   雪霜包 128   大捆雪霜包 22241   疗伤药 123    疗伤药包 125   大捆疗伤包22240
//祈祷之心 5160    史诗任务卷轴 1400    魔族指令书2006    高级书页 245     蓝石战22011     蓝石道22031  蓝石法22021  
//强效金创药 104   强效魔法药114    超级金创药(赠)109   超级金创药 105    超级魔法药(赠)119    超级魔法药115   雪霜打捆绳 353
//疗伤打捆绳 354   书页 242   金刚石 2063
#define GameGoodsSnowID 127											// 127万年雪霜 id
#define GameGoodsSnowBagID 128										// B900雪霜包 id
#define GameGoodsCureID 123											// 123疗伤药 id
#define GameGoodsCureBagID 125										// 125疗伤包 id

//仓库
#define GameGoodsBigSnowBagID 22241									// C300大捆雪霜包 id
#define GameGoodsBigCureBagID 22240									// 22240大捆疗伤包 id
#define GameGoodsPrayID 5160										// 5160祈祷之心 id
#define GameGoodsTaskScrollID 1400									// 1400史诗任务卷轴 id
#define GameGoodsInfernoID 2006										// 2006魔族指令书 id
#define GameGoodsGoldInfernoID 22200								// 22200黄金魔族指令书 id
#define GameGoodsHighPageID 245										// 245高级书页 id
#define GameGoodsSapphireWarriorID 22011							// 22011蓝宝石(战) id
#define GameGoodsSapphireTaoistID 22031								// 22031蓝宝石(道) id
#define GameGoodsSapphireMagicID 22021								// 22021蓝宝石(法) id
#define GameGoodsPageID 242											// 242书页 id
#define GameGoodsAdamasID 2063										// 2063金刚石 id

//苍月药师
#define GameGoodsfgrdID 104											// 104强效金疮药 id
#define GameGoodsPresentfgrdID 108									// 108强效金疮药(赠) id
#define GameGoodsSuperfgrdID 105									// 105超级金疮药 id
#define GameGoodsPresentSuperfgrdID 109								// 超级金创药(赠)109 id

#define GameGoodsMagicDrugID 114									// 114强效魔法药 id
#define GameGoodsPresentMagicDrugID 118								// 118强效魔法药(赠) id
#define GameGoodsSuperMagicID 115									// 115超级魔法药 id
#define GameGoodsPresentSuperMagicID 119							// 119超级魔法药(赠) id

#define GameGoodsmiddleMagicID 112									// 112魔法药中量
#define GameGoodsmiddlefgrdID 102									// 102金疮药中量
#define GameGoodsSunWaterID 120										// 120太阳水中量
#define GameGoodsSuperSunWaterID 121								// 121强效太阳水


//流浪汉
#define GameGoodsRedPoisonID 306									// 306黄色药粉大量id(红毒)
#define GameGoodsGreenPoisonID 307									// 307灰色药粉大量id(绿毒)
#define GameGoodsSuperAmuletID 310									// 310超级护身符id
#define GameGoodsBackRollID 433										// 433回城卷id
#define GameGoodsDungeonPackID 440									// 440地牢逃脱包id
#define GameGoodsDungeonID 439										// 439地牢逃脱卷id
//商城
#define GameGoodsPresentBackRockID 338								// 338回城石(赠)id
#define GameGoodsBackRockID 337										// 337回城石id
#define GameGoodsRandRockID 340										// 340随机石
#define GameGoodsFixWaterID 138										// 138修复神水 id
#define GameGoodsSnowRopeID 353										// 355雪霜打捆绳 id
#define GameGoodsCureRopeID 354										// 354疗伤打捆绳 id

//龙门三劫
#define GameGoodsTreasureID 33220										// 33220无名馈赠宝箱 id
#define GameGoodsTreasure2ID 33221										// 33221无名馈赠宝箱 id
#define GameGoodsGoldID 33222											// 33222点金石 id
#define GameGoodsTokenID 33119											// 33119龙门令牌 id

//--------------------------------------------------------------------------------------call地址

#define PunchUseGoodsCallAddress GameBase+0xBBA70									//合击服使用物品call地址			8B71??8B41??2BC6C1F8??3BD07D??8B049685C074??6A0150+0x25
#define PunchHeroUseGoodsCallAddress GameBase+0xB4450								//合击服英雄使用物品call地址		2BC6C1F8??3BD07D??8B049685C074??50+0x13
#define UseGoodsCallAddress GameBase+0xBBce0										//使用物品call地址				8D4D??51538BC8C745??00000000C745??00000000E8????????85C0+0x17
#define ChoseGoodsDisintegrateCallAddress GameBase+0x10A680							//选择物品分解call地址			8B8E5803000085C974??3B0D???????? (+3D)
#define ChoseGoodsSaveCallAddress GameBase+0x72bb0									//选择物品存仓库call地址			E8????????8B4C242C5E33CCB8 01000000E8????????8BE55D+0x1 
#define ChoseHeroGoodsSaveCallAddress GameBase+0x72bb0								//选择物品存英雄背包call地址		B8010000008BE55DC3A1????????8945??6A048D45+0x2
#define SkillCallAddress GameBase+0x95010											//技能call地址		


//------------------------------------------------------------------------------------------------------------------------
#define NpcMoveSleep Sleep(2000);


struct OtherUser
{
	wchar_t Name[34];																	//玩家姓名
	int UseFlag;																		//玩家标识
	int ObjID;																			//对象专属标识
	int RetinueFlag;																	//随从标识
	int ObjAddress;																		//玩家结构体地址
	int x;																				//x坐标
	int y;																				//y坐标
};


struct User
{
	wchar_t Name[34];																	//名字
	wchar_t InMap[34];																	//所在地图
	int Occupation;																		//职业
	int Hp;																				//血
	int MaxHp;																			//最大血
	//int Blue;
	//int MaxBlue;
	long long Exp;																		//当前经验
	int Money;																			//当前金币
	int BindingMoney;																	//当前绑定金币
	int x;																				//x坐标
	int y;																				//y坐标
	int On_hookState;																	//挂机状态
	int Infor;																			//[对象+4]
	int On_line;																		//挂机状态
	int State;																			//人物动作状态
	int ObjAddress;																		//结构体地址
};


struct Knapsack
{
	char Name[34];																		//名字
	int Numb;																			//数量
	short GoodsId;																		//物品id
	char EquipFlag;																		//装备标识
	int ChoseId;																		//物品独立选择id 4字节
	int Address;																		//背包结构体地址
	int GoodType;																		//物品种类
};



DWORD WINAPI GetGameBase(_In_ LPVOID lpParameter);																					//获取游戏模块基址 加初始化
int InitUserInfor(DWORD GameBase, int flag=0);																								//初始化对象信息
DWORD InitKnapsackInfor(DWORD GameBase);																							//初始化背包信息
DWORD WayfindingCall(int x, int y);																									//寻路call
DWORD UseGoodsCall(int GoodsId);																									//使用物品call
DWORD ChoseGoodsDisintegrateCall();																									//选着物品分解call
DWORD SaveGoodsCall();																												//保存物品call
DWORD GetUserInMap();																												//当前任务所在地图
int GetObjAndClick(long x1, long y1, long x2, long y2, _bstr_t str, _bstr_t color, int offset_x=3, int offset_y=3,int Click = 0);	//获取对象并点击
//键盘call
DWORD KeyCall(int key);																												//游戏按键call
//移动鼠标点击
DWORD moveTo(int x, int y,int Click = 0);
//获取仓库雪霜包数量
CString GetSnowWarehouseNum();
//获取仓库疗伤药包数量
CString GetCurWarehouseNum();
//关闭自动追踪
DWORD CloseAutoTrace(bool Switch);
//隐藏玩家和怪物开关
DWORD HideUseAndMaster(bool Switch);
//任务追踪开关
DWORD TaskbarTraceSwitch(bool Switch);
//残血保护功能
DWORD WINAPI LowRedFunc(DWORD NowHp, DWORD MaxHp);
//获取玩家血和最大血
DWORD GetUserHp_MaxHp();
//判断是否在安全区
DWORD IsSafetyArea();
//从流程表获取挂机地图
DWORD GetOn_lineMap();
//补给总操作
int SupplyGoods();
//寻路功能
int FindWay(int x, int y);
//流浪汉购买
int TrampNpcFunc(bool Ryongson = false);
//去挂机地图
int Go_On_lineMap(CString Map,bool Jmp=false);
//挂机中途检测
DWORD WINAPI AutoOn_lineCheck(_In_ LPVOID lpParameter);
//回收操作
int RecycleNpcFunc();
//分解师操作
int ResolveNpcFun();
//获取现在要挂机的地图
int GetNowGotoMap(int iCount);
//随机数
int RandFunc(int count, bool Switch);
//玩家回城功能
int UseBack();
//仓库管理
int StoreManagement();
//苍月药师补给
int DrugsManagement(int BuyRedNumb, int BuyBlueNumb, int iRedDurgNumb, int iBlueDurgNumb);
//商城购买
int Shopping(bool Open=false);
//获取玩家经验
long long  GetUserExp();
//获取玩家挂机状态 0是停止 1是挂机
int GetOn_lineState();
//获取玩家金币 0是绑金  1是金币
long long  GetUserMoney(DWORD sign);
//黑名单回城功能
int BlackFunc(bool Fit = false);
//挂机定时跑到指定坐标点
int On_lineGoX_Y();
//获取玩家xy坐标
DWORD GetUserX_YCoord();
//设置挂机状态
int SetOn_lineState(bool State);
//自动登录
int AutoLogin();
//保存配置文件
int SaveGameConfig();
//显示配置信息
int ShowGameConfig();
//读取配置信息
int ReadGameConfig();
//切换攻击状态
int ChoseUseState();
//寻路
int CheckWay(int Npc);
//使用技能call
int UseSkillCall(int SkillId);
//登录;
int Login();
//启动
int Start();
//掉线重连
DWORD WINAPI Reconnection(LPVOID lpParam);
//设置自动修理
int SetAutoFix(bool State);
//启动挂机
int OpenAuto();
//写入剪切板
int WriteClipboard(char* str);
//打人线程
DWORD WINAPI FitUser(_In_ LPVOID lpParameter);
//道士毒怪
int PoisonMonster(int Scope);
//毒怪坐标寻路
int FindNearWay(int Coord = 0);
//点击错误提示框
DWORD WINAPI ClickError(_In_ LPVOID lpParameter);
//自动拾取设置
int SetAutoPick(bool State);