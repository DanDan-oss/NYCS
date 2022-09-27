// TabPAGE5.cpp: 实现文件
//

#include "pch.h"
#include "GreenLegend.h"
#include "TabPAGE5.h"
#include "afxdialogex.h"


// TabPAGE5 对话框

IMPLEMENT_DYNAMIC(TabPAGE5, CDialogEx)

TabPAGE5::TabPAGE5(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TabPAGE5, pParent)
{

}

TabPAGE5::~TabPAGE5()
{
}

void TabPAGE5::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_Green, m_cGreenStart);
	DDX_Control(pDX, IDC_CHECK_173, m_c173Start);
	DDX_Control(pDX, IDC_EDIT_GamePath, m_eGamePath);
	DDX_Control(pDX, IDC_EDIT_Account, m_eAccount);
	DDX_Control(pDX, IDC_EDIT_PassWord, m_ePassWord);
	DDX_Control(pDX, IDC_CKB_AutoLogn, m_cAutoLogin);
	DDX_Control(pDX, IDC_CKB_Connect, m_cAutoContinue);
	DDX_Control(pDX, IDC_CMB_Area, m_cGreen);
	DDX_Control(pDX, IDC_CKB_CloseTeam, m_CloseTeam);
	DDX_Control(pDX, IDC_EDIT_WaitTime, m_eWaitTimes);
}


BEGIN_MESSAGE_MAP(TabPAGE5, CDialogEx)
	
	
	ON_BN_CLICKED(IDC_CHECK_Green, &TabPAGE5::OnBnClickedCheckGreen)
	ON_BN_CLICKED(IDC_CHECK_173, &TabPAGE5::OnBnClickedCheck173)
	ON_BN_CLICKED(IDC_CKB_MoreOpen, &TabPAGE5::OnBnClickedCkbMoreopen)
END_MESSAGE_MAP()


// TabPAGE5 消息处理程序




void TabPAGE5::OnBnClickedCheckGreen()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_cGreenStart.GetCheck() == BST_CHECKED)
	{
		m_c173Start.SetCheck(BST_UNCHECKED);

		m_cGreen.ResetContent();
		CString Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
		Map = L"开天(5区)";
		m_cGreen.AddString(Map);
	}
}


void TabPAGE5::OnBnClickedCheck173()
{
	// TODO: 在此添加控件通知处理程序代码
}


void TabPAGE5::OnBnClickedCkbMoreopen()
{
	// TODO: 在此添加控件通知处理程序代码


}
