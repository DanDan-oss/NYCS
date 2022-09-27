// CPlayTabPage2.cpp: 实现文件
//

#include "pch.h"
#include "GreenLegend.h"
#include "CPlayTabPage2.h"
#include "afxdialogex.h"


// CPlayTabPage2 对话框

IMPLEMENT_DYNAMIC(CPlayTabPage2, CDialogEx)

CPlayTabPage2::CPlayTabPage2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PlayTabPage2, pParent)
{

}

CPlayTabPage2::~CPlayTabPage2()
{
}

void CPlayTabPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlayTabPage2, CDialogEx)
END_MESSAGE_MAP()


// CPlayTabPage2 消息处理程序
