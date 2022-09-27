// CPlayTabPage1.cpp: 实现文件
//

#include "pch.h"
#include "GreenLegend.h"
#include "CPlayTabPage1.h"
#include "afxdialogex.h"


// CPlayTabPage1 对话框

IMPLEMENT_DYNAMIC(CPlayTabPage1, CDialogEx)

CPlayTabPage1::CPlayTabPage1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PlayTabPage1, pParent)
{

}

CPlayTabPage1::~CPlayTabPage1()
{
}

void CPlayTabPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PlayerInfoLIST, PlayInfoList);
}


BEGIN_MESSAGE_MAP(CPlayTabPage1, CDialogEx)
END_MESSAGE_MAP()


// CPlayTabPage1 消息处理程序
