#pragma once
//------------------------------------------------------------------------���

extern DWORD GameBase;													//��Ϸ��ַ
#define GameUseBaseOffset 0x9cc1D0										//*����׵�ַ				33C58945F05657508D45??64A3000000008B0D????????85C90F84????????+0x14
//#define GameNpcStateOffset 0xAA4B50									//��ϷNpc�Ի����״̬ƫ��  1�� 0�ر�	
#define GameKnapsackBaseOffset 0x98Ab9c									//*�����׵�ַ				558BEC57FF75??8BF9E8???????? 837D??0075??833D???????? +0x16
#define GameUserMaxHpBaseOffset 0x9cba80								//*����Լ����Ѫ			8BCFFF35????????FF35????????50680E030000E8????????FF4424??FF4C24??+0x4
#define GameUserMoneyOffset 0x9cbaad									//*��ҽ��
#define GameUserBindingMoneyOffset GameUserMoneyOffset+0x58				//��Ұ󶨽��
#define GameUserExpOffset 0x9cba95										//*��Ҿ���ƫ��					8B15????????8B3D????????8B1D????????660FD64424+0x2
#define UseOn_lineFlagOffset 0x98b158									//*��������״̬  1���� 0����    558BEC5333DB391D????????0F84????????578B7D??85FF+0x9
#define AutoTraceOffset 0x9cb37c										//*�Զ�׷�б�ʶƫ��				833D????????0075??33C0EB??B80100000083FE24  +0x2
#define HideOtherUseOffset 0x98abc0										//*�������ƫ��					85C074??8B068BCEFF50??833D????????0074??83F824 +0xe
//#define GameObj *(int*)(*((int*)(*((int*)(GameBase+0xAE2FF8))+0x1E0))+0)//��������
#define GameObjFlagOffset 0xD											//�����־
#define GameObjInformationOffset 0x4									//������ϸ��Ϣ�͹���
#define GameObjFlag 0x0													//��0���Ƕ���  1������β  һ���ֽ�
#define GameObjOffset 0x14												//0x14�Ƕ����ַ
#define GameUserNameLenOffset 0x2c										//���ݴ��ڵ���8+0x1c�Ǹ����ֵ�ַ
#define GameUserMapNameLenOffset 0x28									//���ݴ��ڵ���8+0x14�Ǹ����ֵ�ַ
#define GameUserNameOffset 0x1c											//�������
#define GameUserXOffset 0x4c											//���x����
#define GameUserYOffset 0x50											//���y����
#define GameUserInfoOffset 0x4											//��Ҹ���״̬״̬
#define GameUserInMapOffset 0x14										//���ڵ�ͼ�ַ���   char*
#define GameOBJIDFlagOffset 0x8											//����ר��id
#define GameUserFlagOffset 0xC											//0x24 ���   0xC npc   0x14����
#define GameRetinueFlagOffset 0x30										//0x30 0xF���    7���NPC����
#define GameMeFlagOffset 0x88											//����Լ���ʶƫ��
#define GameMeFlag 0x7FFFFFFF											//0x7FFFFFFF����Լ���ʶ
#define GameUserHpOffSet 0x94											//����Լ���ǰѪ��ƫ��
#define GameUserMaxHpOffSet 0x98										//����Լ����Ѫ��ƫ��
#define GameUserOccupationFlagOffset 0x90								//0x301 �е�ʿ  0x302 Ů��ʿ  0x201�з�ʦ   0x202Ů��ʦ  0x101��սʿ   0x102Ůսʿ
//#define GameUserEquipFlagOffset 0xb0									// װ����ʶƫ��

#define HideTaskbarOffset AutoTraceOffset+0xD0							//��ʾ������ƫ��											
#define ChoseGoodsDisintegrateEcxOffset 0xFDC							//ѡ��ֽ�callEcx
#define ChoseGoodsSaveEcxOffset 0xE70									//ѡ���ֿ�callEcx
#define GameOn_lineTirps AutoTraceOffset+0xD4							//�һ���ʾ
#define GameAutoFixOffset AutoTraceOffset+0xF0							//�Զ�����
#define GameAutoPickOffset AutoTraceOffset+0xF4							//�Զ�ʰȡ
#define UseOn_lineFlagOffset2 0x50										//��������״̬2
#define On_LineStateOffset 0x240										//����һ�״̬ ƫ�� 1�һ� 0���ڹһ�
#define GameMouseXOffset 0x134											//��Ϸ�ڴ����xƫ��
#define GameMouseYOffset 0x138											//��Ϸ�ڴ����yƫ��
#define GameMasterObj 0x188												//ѡ�й������
#define GameSkillMasterObj 0x194										//����ѡ�й������

#define UseFitStateOffset 0x1010										//���﹥��ģʽ
#define SafetyAreaOffset 0x2D8											//���ﰲȫ�� ƫ�� 0����������  6��ȫ��
#define UseUseNowStateOffset 0xEC										//�����˶�״̬  6ֹͣ 1ս��
#define GameMasterDieFlagOffset 0x100									//��������ʬ���־

//------------------------------------------------------------------------����
#define GameGoodsMaxNumber 45										//�����������
#define GameGoodsNameOffset 0x1										// ��Ʒ����
#define GameGoodsNumbOffset 0x3d									// ��Ʒ����
#define GameGoodsIDOffset 0x25										// ��Ʒid 
#define GameGoodsChoseIDOffset 0x21									// ��Ʒѡ��id 
#define GameUserEquipFlagOffset 0x48								// ���ʯ װ��ƫ��
#define GameUserGoodsFlagOffset 0x4c								// ��Ʒ ����
#define GameGoodsRockFlag 0x300										// ��ʯ��Ʒ ����
#define GameUserEquipFlag 0x28										// 28���ʯ ��װװ����ʶ
#define GameUserEquipFlag2 0x38										// 38���ʯ ����װװ����ʶ
//#define GameGoodsFlagOffset 0x28									// ��Ʒαid  ѩ˪��������������һ��

//����ѩ˪  127       �޸���ˮ 138   ѩ˪�� 128   ����ѩ˪�� 22241   ����ҩ 123    ����ҩ�� 125   �������˰�22240
//��֮�� 5160    ʷʫ������� 1400    ħ��ָ����2006    �߼���ҳ 245     ��ʯս22011     ��ʯ��22031  ��ʯ��22021  
//ǿЧ��ҩ 104   ǿЧħ��ҩ114    ������ҩ(��)109   ������ҩ 105    ����ħ��ҩ(��)119    ����ħ��ҩ115   ѩ˪������ 353
//���˴����� 354   ��ҳ 242   ���ʯ 2063
#define GameGoodsSnowID 127											// 127����ѩ˪ id
#define GameGoodsSnowBagID 128										// B900ѩ˪�� id
#define GameGoodsCureID 123											// 123����ҩ id
#define GameGoodsCureBagID 125										// 125���˰� id

//�ֿ�
#define GameGoodsBigSnowBagID 22241									// C300����ѩ˪�� id
#define GameGoodsBigCureBagID 22240									// 22240�������˰� id
#define GameGoodsPrayID 5160										// 5160��֮�� id
#define GameGoodsTaskScrollID 1400									// 1400ʷʫ������� id
#define GameGoodsInfernoID 2006										// 2006ħ��ָ���� id
#define GameGoodsGoldInfernoID 22200								// 22200�ƽ�ħ��ָ���� id
#define GameGoodsHighPageID 245										// 245�߼���ҳ id
#define GameGoodsSapphireWarriorID 22011							// 22011����ʯ(ս) id
#define GameGoodsSapphireTaoistID 22031								// 22031����ʯ(��) id
#define GameGoodsSapphireMagicID 22021								// 22021����ʯ(��) id
#define GameGoodsPageID 242											// 242��ҳ id
#define GameGoodsAdamasID 2063										// 2063���ʯ id

//����ҩʦ
#define GameGoodsfgrdID 104											// 104ǿЧ��ҩ id
#define GameGoodsPresentfgrdID 108									// 108ǿЧ��ҩ(��) id
#define GameGoodsSuperfgrdID 105									// 105������ҩ id
#define GameGoodsPresentSuperfgrdID 109								// ������ҩ(��)109 id

#define GameGoodsMagicDrugID 114									// 114ǿЧħ��ҩ id
#define GameGoodsPresentMagicDrugID 118								// 118ǿЧħ��ҩ(��) id
#define GameGoodsSuperMagicID 115									// 115����ħ��ҩ id
#define GameGoodsPresentSuperMagicID 119							// 119����ħ��ҩ(��) id

#define GameGoodsmiddleMagicID 112									// 112ħ��ҩ����
#define GameGoodsmiddlefgrdID 102									// 102��ҩ����
#define GameGoodsSunWaterID 120										// 120̫��ˮ����
#define GameGoodsSuperSunWaterID 121								// 121ǿЧ̫��ˮ


//���˺�
#define GameGoodsRedPoisonID 306									// 306��ɫҩ�۴���id(�춾)
#define GameGoodsGreenPoisonID 307									// 307��ɫҩ�۴���id(�̶�)
#define GameGoodsSuperAmuletID 310									// 310���������id
#define GameGoodsBackRollID 433										// 433�سǾ�id
#define GameGoodsDungeonPackID 440									// 440�������Ѱ�id
#define GameGoodsDungeonID 439										// 439�������Ѿ�id
//�̳�
#define GameGoodsPresentBackRockID 338								// 338�س�ʯ(��)id
#define GameGoodsBackRockID 337										// 337�س�ʯid
#define GameGoodsRandRockID 340										// 340���ʯ
#define GameGoodsFixWaterID 138										// 138�޸���ˮ id
#define GameGoodsSnowRopeID 353										// 355ѩ˪������ id
#define GameGoodsCureRopeID 354										// 354���˴����� id

//��������
#define GameGoodsTreasureID 33220										// 33220������������ id
#define GameGoodsTreasure2ID 33221										// 33221������������ id
#define GameGoodsGoldID 33222											// 33222���ʯ id
#define GameGoodsTokenID 33119											// 33119�������� id

//--------------------------------------------------------------------------------------call��ַ

#define PunchUseGoodsCallAddress GameBase+0xBBA70									//�ϻ���ʹ����Ʒcall��ַ			8B71??8B41??2BC6C1F8??3BD07D??8B049685C074??6A0150+0x25
#define PunchHeroUseGoodsCallAddress GameBase+0xB4450								//�ϻ���Ӣ��ʹ����Ʒcall��ַ		2BC6C1F8??3BD07D??8B049685C074??50+0x13
#define UseGoodsCallAddress GameBase+0xBBce0										//ʹ����Ʒcall��ַ				8D4D??51538BC8C745??00000000C745??00000000E8????????85C0+0x17
#define ChoseGoodsDisintegrateCallAddress GameBase+0x10A680							//ѡ����Ʒ�ֽ�call��ַ			8B8E5803000085C974??3B0D???????? (+3D)
#define ChoseGoodsSaveCallAddress GameBase+0x72bb0									//ѡ����Ʒ��ֿ�call��ַ			E8????????8B4C242C5E33CCB8 01000000E8????????8BE55D+0x1 
#define ChoseHeroGoodsSaveCallAddress GameBase+0x72bb0								//ѡ����Ʒ��Ӣ�۱���call��ַ		B8010000008BE55DC3A1????????8945??6A048D45+0x2
#define SkillCallAddress GameBase+0x95010											//����call��ַ		


//------------------------------------------------------------------------------------------------------------------------
#define NpcMoveSleep Sleep(2000);


struct OtherUser
{
	wchar_t Name[34];																	//�������
	int UseFlag;																		//��ұ�ʶ
	int ObjID;																			//����ר����ʶ
	int RetinueFlag;																	//��ӱ�ʶ
	int ObjAddress;																		//��ҽṹ���ַ
	int x;																				//x����
	int y;																				//y����
};


struct User
{
	wchar_t Name[34];																	//����
	wchar_t InMap[34];																	//���ڵ�ͼ
	int Occupation;																		//ְҵ
	int Hp;																				//Ѫ
	int MaxHp;																			//���Ѫ
	//int Blue;
	//int MaxBlue;
	long long Exp;																		//��ǰ����
	int Money;																			//��ǰ���
	int BindingMoney;																	//��ǰ�󶨽��
	int x;																				//x����
	int y;																				//y����
	int On_hookState;																	//�һ�״̬
	int Infor;																			//[����+4]
	int On_line;																		//�һ�״̬
	int State;																			//���ﶯ��״̬
	int ObjAddress;																		//�ṹ���ַ
};


struct Knapsack
{
	char Name[34];																		//����
	int Numb;																			//����
	short GoodsId;																		//��Ʒid
	char EquipFlag;																		//װ����ʶ
	int ChoseId;																		//��Ʒ����ѡ��id 4�ֽ�
	int Address;																		//�����ṹ���ַ
	int GoodType;																		//��Ʒ����
};



DWORD WINAPI GetGameBase(_In_ LPVOID lpParameter);																					//��ȡ��Ϸģ���ַ �ӳ�ʼ��
int InitUserInfor(DWORD GameBase, int flag=0);																								//��ʼ��������Ϣ
DWORD InitKnapsackInfor(DWORD GameBase);																							//��ʼ��������Ϣ
DWORD WayfindingCall(int x, int y);																									//Ѱ·call
DWORD UseGoodsCall(int GoodsId);																									//ʹ����Ʒcall
DWORD ChoseGoodsDisintegrateCall();																									//ѡ����Ʒ�ֽ�call
DWORD SaveGoodsCall();																												//������Ʒcall
DWORD GetUserInMap();																												//��ǰ�������ڵ�ͼ
int GetObjAndClick(long x1, long y1, long x2, long y2, _bstr_t str, _bstr_t color, int offset_x=3, int offset_y=3,int Click = 0);	//��ȡ���󲢵��
//����call
DWORD KeyCall(int key);																												//��Ϸ����call
//�ƶ������
DWORD moveTo(int x, int y,int Click = 0);
//��ȡ�ֿ�ѩ˪������
CString GetSnowWarehouseNum();
//��ȡ�ֿ�����ҩ������
CString GetCurWarehouseNum();
//�ر��Զ�׷��
DWORD CloseAutoTrace(bool Switch);
//������Һ͹��￪��
DWORD HideUseAndMaster(bool Switch);
//����׷�ٿ���
DWORD TaskbarTraceSwitch(bool Switch);
//��Ѫ��������
DWORD WINAPI LowRedFunc(DWORD NowHp, DWORD MaxHp);
//��ȡ���Ѫ�����Ѫ
DWORD GetUserHp_MaxHp();
//�ж��Ƿ��ڰ�ȫ��
DWORD IsSafetyArea();
//�����̱��ȡ�һ���ͼ
DWORD GetOn_lineMap();
//�����ܲ���
int SupplyGoods();
//Ѱ·����
int FindWay(int x, int y);
//���˺�����
int TrampNpcFunc(bool Ryongson = false);
//ȥ�һ���ͼ
int Go_On_lineMap(CString Map,bool Jmp=false);
//�һ���;���
DWORD WINAPI AutoOn_lineCheck(_In_ LPVOID lpParameter);
//���ղ���
int RecycleNpcFunc();
//�ֽ�ʦ����
int ResolveNpcFun();
//��ȡ����Ҫ�һ��ĵ�ͼ
int GetNowGotoMap(int iCount);
//�����
int RandFunc(int count, bool Switch);
//��һسǹ���
int UseBack();
//�ֿ����
int StoreManagement();
//����ҩʦ����
int DrugsManagement(int BuyRedNumb, int BuyBlueNumb, int iRedDurgNumb, int iBlueDurgNumb);
//�̳ǹ���
int Shopping(bool Open=false);
//��ȡ��Ҿ���
long long  GetUserExp();
//��ȡ��ҹһ�״̬ 0��ֹͣ 1�ǹһ�
int GetOn_lineState();
//��ȡ��ҽ�� 0�ǰ��  1�ǽ��
long long  GetUserMoney(DWORD sign);
//�������سǹ���
int BlackFunc(bool Fit = false);
//�һ���ʱ�ܵ�ָ�������
int On_lineGoX_Y();
//��ȡ���xy����
DWORD GetUserX_YCoord();
//���ùһ�״̬
int SetOn_lineState(bool State);
//�Զ���¼
int AutoLogin();
//���������ļ�
int SaveGameConfig();
//��ʾ������Ϣ
int ShowGameConfig();
//��ȡ������Ϣ
int ReadGameConfig();
//�л�����״̬
int ChoseUseState();
//Ѱ·
int CheckWay(int Npc);
//ʹ�ü���call
int UseSkillCall(int SkillId);
//��¼;
int Login();
//����
int Start();
//��������
DWORD WINAPI Reconnection(LPVOID lpParam);
//�����Զ�����
int SetAutoFix(bool State);
//�����һ�
int OpenAuto();
//д����а�
int WriteClipboard(char* str);
//�����߳�
DWORD WINAPI FitUser(_In_ LPVOID lpParameter);
//��ʿ����
int PoisonMonster(int Scope);
//��������Ѱ·
int FindNearWay(int Coord = 0);
//���������ʾ��
DWORD WINAPI ClickError(_In_ LPVOID lpParameter);
//�Զ�ʰȡ����
int SetAutoPick(bool State);