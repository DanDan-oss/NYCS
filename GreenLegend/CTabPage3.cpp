// CTabPage3.cpp: 实现文件
//

#include "pch.h"
#include "GreenLegend.h"
#include "CTabPage3.h"
#include "afxdialogex.h"


// CTabPage3 对话框

IMPLEMENT_DYNAMIC(CTabPage3, CDialogEx)

CTabPage3::CTabPage3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TabPAGE3, pParent)
{
	iPattern =0;
}

CTabPage3::~CTabPage3()
{
}

void CTabPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CKB_RedFewReturn, m_cLittleRedBack);
	DDX_Control(pDX, IDC_CKB_RedFewRand, m_cLittleRedRand);
	DDX_Control(pDX, IDC_CKB_RedFewOffline, m_cLittleRedDown);
	DDX_Control(pDX, IDC_CKB_RedFewScreenshot, m_cLittleRedScreenshot);
	DDX_Control(pDX, IDC_CKB_ReturnWait, m_cBackWait);
	DDX_Control(pDX, IDC_CKB_EnterMapRand, m_cJoinRand);
	DDX_Control(pDX, IDC_EDIT_Return, m_eLittleRedBack);
	DDX_Control(pDX, IDC_EDIT_Rand, m_eLittleRedRand);
	DDX_Control(pDX, IDC_EDIT_Offline, m_eLittleRedDown);
	DDX_Control(pDX, IDC_EDIT_Screenshot, m_eLittleRedScreenshot);
	DDX_Control(pDX, IDC_EDIT_ReturnWait, m_eBackWait);
	DDX_Control(pDX, IDC_CKB_NoMasterTime, m_cTimeNoMasterRand);
	DDX_Control(pDX, IDC_CKB_NoMasterRound, m_cRoundNoMasterRand);
	DDX_Control(pDX, IDC_CKB_BackpackNumb, m_cKnapsackResidue);
	DDX_Control(pDX, IDC_CKB_EquipmentBack, m_cAdamasEquipment);
	DDX_Control(pDX, IDC_EDIT_NoMasterTime, m_eTimeNoMasterRand);
	DDX_Control(pDX, IDC_EDIT_NoMasterRound, m_eRoundNoMasterRand);
	DDX_Control(pDX, IDC_EDIT_BackpackNumb, m_eKnapsackResidue);
	DDX_Control(pDX, IDC_EDIT_EquipmentBack, m_eAdamasEquipment);
	DDX_Control(pDX, IDC_EDIT_Black, m_eBlackList);
	DDX_Control(pDX, IDC_CKB_BlackRand, m_cBlackListRand);
	DDX_Control(pDX, IDC_CKB_BlackBack, m_cBlackListBack);
	DDX_Control(pDX, IDC_CMB_Pattern, m_cPattern);
	DDX_Control(pDX, IDC_CKB_Fitxy, m_cFitXY);
	DDX_Control(pDX, IDC_EDIT_X_Y, m_eFitXY);
	DDX_Control(pDX, IDC_EDIT_Time, m_eXYTime);
	DDX_Control(pDX, IDC_EDIT_X_Y2, m_EDIT_Y);
	DDX_Control(pDX, IDC_CKB_Fit, m_cBlackFit);
	DDX_Control(pDX, IDC_EDIT_eFit, m_eFit);
}


BEGIN_MESSAGE_MAP(CTabPage3, CDialogEx)
	ON_BN_CLICKED(IDC_CKB_BlackRand, &CTabPage3::OnBnClickedCkbBlackrand)
	ON_BN_CLICKED(IDC_CKB_BlackBack, &CTabPage3::OnBnClickedCkbBlackback)
	ON_EN_CHANGE(IDC_EDIT_Black, &CTabPage3::OnEnChangeEditBlack)
	ON_BN_CLICKED(IDC_CKB_NoMasterTime, &CTabPage3::OnBnClickedCkbNomastertime)
	ON_CBN_SELCHANGE(IDC_CMB_Pattern, &CTabPage3::OnCbnSelchangeCmbPattern)
	ON_CBN_DROPDOWN(IDC_CMB_Pattern, &CTabPage3::OnCbnDropdownCmbPattern)
END_MESSAGE_MAP()


// CTabPage3 消息处理程序


void CTabPage3::OnBnClickedCkbBlackrand()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cBlackListBack.GetState() == BST_CHECKED&& m_cBlackListRand.GetState() == BST_CHECKED)
	{
		MessageBoxA(NULL, "请选择其中一个", "提示", NULL);
		return;
	}

}


void CTabPage3::OnBnClickedCkbBlackback()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cBlackListBack.GetState() == BST_CHECKED && m_cBlackListRand.GetState() == BST_CHECKED)
	{
		MessageBoxA(NULL, "请选择其中一个", "提示", NULL);
		return;
	}
}


void CTabPage3::OnEnChangeEditBlack()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码


}





void CTabPage3::OnBnClickedCkbNomastertime()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (m_cTimeNoMasterRand.GetCheck() == BST_CHECKED&& m_cRoundNoMasterRand.GetCheck() == BST_CHECKED)
	{
		MessageBoxA(NULL, "时长,周围请选择其中一个", "提示", NULL);
		return;
	}*/
}


void CTabPage3::OnCbnSelchangeCmbPattern()
{
	// TODO: 在此添加控件通知处理程序代码
	iPattern =m_cPattern.GetCurSel()+1;
}


void CTabPage3::OnCbnDropdownCmbPattern()
{
	// TODO: 在此添加控件通知处理程序代码
	//cInitShow = L"全体模式";
	m_cPattern.ResetContent();
	m_cPattern.AddString(L"全体模式");
	
	m_cPattern.AddString(L"和平模式");

	m_cPattern.AddString(L"编组模式");
	
	m_cPattern.AddString(L"行会模式");
	
	m_cPattern.AddString(L"善恶模式");
	
	m_cPattern.AddString(L"夫妻模式");
	
	m_cPattern.AddString(L"师徒模式");
}
