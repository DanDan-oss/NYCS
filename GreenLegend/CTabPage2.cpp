// CTabPage2.cpp: 实现文件
//

#include "pch.h"
#include "GreenLegend.h"
#include "CTabPage2.h"
#include "afxdialogex.h"


// CTabPage2 对话框

IMPLEMENT_DYNAMIC(CTabPage2, CDialogEx)

CTabPage2::CTabPage2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TabPAGE2, pParent)
{

}

CTabPage2::~CTabPage2()
{
}

void CTabPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RedDrug, m_eRedNumb);
	DDX_Control(pDX, IDC_EDIT_BlueDrug, m_eBlueNumb);
	DDX_Control(pDX, IDC_EDIT_SnowRope, m_eSnowNumb);
	DDX_Control(pDX, IDC_EDIT_CureRope, m_eCureNumb);
	DDX_Control(pDX, IDC_EDIT_BackStone, m_eBackNumb);
	DDX_Control(pDX, IDC_EDIT_RandStone, m_eRandNumb);
	DDX_Control(pDX, IDC_EDIT_BackReel, m_eBackRollNumb);
	DDX_Control(pDX, IDC_EDIT_RepairWater, m_eFixWaterNumb);
	DDX_Control(pDX, IDC_CKB_RepairWater, m_cFixWaterStart);
	DDX_Control(pDX, IDC_CKB_BackReel, m_cBackRollStart);
	DDX_Control(pDX, IDC_CKB_RandStone, m_cRandStart);
	DDX_Control(pDX, IDC_CKB_BackStone, m_cBackRockStart);
	DDX_Control(pDX, IDC_CKB_CureRope, m_cCureStart);
	DDX_Control(pDX, IDC_CKB_SnowRope, m_cSnowStart);
	DDX_Control(pDX, IDC_CKB_NoBlueBlack, m_cNoBlueStart);
	DDX_Control(pDX, IDC_CKB_NoRedBlack, m_cNoRedStart);
	DDX_Control(pDX, IDC_CKB_BackResolve, m_cBackResolveStart);
	DDX_Control(pDX, IDC_CKB_BackRecycle, m_cBackRecycleStart);
	DDX_Control(pDX, IDC_CKB_SaveGoods, m_cSaveGoods);
	DDX_Control(pDX, IDC_CKB_SaveDrug, m_cSaveDrug);
	DDX_Control(pDX, IDC_CKB_RedPoison, m_cRedPoison);
	DDX_Control(pDX, IDC_CKB_GreenPoison, m_cGreenPoison);
	DDX_Control(pDX, IDC_CKB_Amulet, m_cAmulet);
	DDX_Control(pDX, IDC_EDIT_Amulet, m_eAmulet);
	DDX_Control(pDX, IDC_CKB_AutoDefend, m_cAutoDefend);
	DDX_Control(pDX, IDC_EDIT_RedPoison, m_eRedPoison);
	DDX_Control(pDX, IDC_EDIT_GreenPoison, m_eGreenPoison);
	DDX_Control(pDX, IDC_EDIT_Dungeon, m_eDungeon);
	DDX_Control(pDX, IDC_CKB_Dungeon, m_cDungeon);
	DDX_Control(pDX, IDC_CHECK1, m_cPackSurplus);
	DDX_Control(pDX, IDC_EDIT1, m_ePackSurplus);
	DDX_Control(pDX, IDC_CKB_Hide, m_cHideMaster);
	DDX_Control(pDX, IDC_EDIT_Times, m_eTimes);
	DDX_Control(pDX, IDC_CKB_Fire, m_cFire);
	DDX_Control(pDX, IDC_EDIT_Fire, m_eFire);
	DDX_Control(pDX, IDC_CKB_PoisonMonster, m_cPoisonMonster);
	DDX_Control(pDX, IDC_CKB_Resurgence, m_cResurgence);
}


BEGIN_MESSAGE_MAP(CTabPage2, CDialogEx)

	ON_BN_CLICKED(IDC_CKB_Hide, &CTabPage2::OnBnClickedCkbHide)
	ON_BN_CLICKED(IDC_CKB_AutoDefend, &CTabPage2::OnBnClickedCkbAutodefend)
END_MESSAGE_MAP()


// CTabPage2 消息处理程序




void CTabPage2::OnBnClickedCkbHide()
{
	// TODO: 在此添加控件通知处理程序代码
	int state=m_cHideMaster.GetCheck();
	if (state==BST_CHECKED)
	{
		HideUseAndMaster(1);
	}
	else
	{
		HideUseAndMaster(0);
	}
}


void CTabPage2::OnBnClickedCkbAutodefend()
{
	// TODO: 在此添加控件通知处理程序代码
}
