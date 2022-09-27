// CTabPage1.cpp: 实现文件
//

#include "pch.h"
#include "GreenLegend.h"
#include "CTabPage1.h"
#include "afxdialogex.h"


// CTabPage1 对话框
extern User GameUser;																					//玩家自己结构体
IMPLEMENT_DYNAMIC(CTabPage1, CDialogEx)

CTabPage1::CTabPage1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TabPAGE1, pParent)
{

}

CTabPage1::~CTabPage1()
{
}

void CTabPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Log, Log_Edit);
}


BEGIN_MESSAGE_MAP(CTabPage1, CDialogEx)
END_MESSAGE_MAP()


// CTabPage1 消息处理程序
BOOL CTabPage1::Common_Savelog(CString strInfo)
{
	wchar_t path[512];
	CString pszlogName;
	CStdioFile MyFile;
	
	CFileException fileException;
	GetModuleFileNameW(NULL, path, sizeof(path));
	
	*(wcsrchr(path, '\\')) = '\0';

	pszlogName.Format(_T("%s\\%s.txt"), path, GameUser.Name);
	//CFile MyFile;
	//Log_Edit.SetWindowTextW(pszlogName.GetBuffer());
	if (MyFile.Open(pszlogName, CFile::typeText | CFile::modeReadWrite, &fileException)==NULL)
	//if (MyFile.Open(pszlogName, CFile::modeReadWrite) == NULL)
	{
		//BYTE unicodeHeader[2] = {0xFE,0xFF};   //放开这两个屏蔽点可保存中文字符
		MyFile.Open(pszlogName, CFile::typeText | CFile::modeReadWrite |CFile::modeCreate, & fileException);
		//MyFile.Write(unicodeHeader,2);
	}
	
	if (MyFile.m_hFile == NULL)
	{
		
		return FALSE;
	}

	MyFile.Seek(0L, CFile::end);
	char* pOldLocale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");
	strInfo + L"\r\n";
	MyFile.WriteString(strInfo);
	//TCHAR* pBuff = strInfo.GetBuffer(strInfo.GetLength() * sizeof(TCHAR));
	//MyFile.Write(pBuff, strInfo.GetLength() * sizeof(TCHAR));

	//pBuff = L"\r\n";
	///MyFile.Write(pBuff, sizeof(pBuff));
	setlocale(LC_CTYPE, pOldLocale);
	free(pOldLocale);

	

	MyFile.Close();
	
	return TRUE;

}




void CTabPage1::LogOut(CStringW infoStr)
{
	int LineCnt = Log_Edit.GetLineCount();
	//if (LineCnt > 100)
	//{
	//	Log_Edit.SetWindowText(_T(""));
	//}

	CStringW tmpStr;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	tmpStr.Format(L">[%02d:%02d:%02d]", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	m_InfoStr += tmpStr;	// 添加log显示时间
	m_InfoStr += infoStr;	// 需要显示的信息
	m_InfoStr += L"\r\n";	// 换行
	// 将m_InfoStr的值显示到界面
	//UpdateData(FALSE);
	Log_Edit.SetWindowTextW(m_InfoStr);
	// 将滚轮调到最后
	Log_Edit.LineScroll(Log_Edit.GetLineCount());

	int size = m_InfoStr.GetLength();
	//if (size > 1024 * 1000)
	if (size > 40000)//测试用
	{
		Common_Savelog(m_InfoStr);
		m_InfoStr.Empty();
	}
		
}

void CTabPage1::LogOut(CStringA infoStr)
{
	int LineCnt = Log_Edit.GetLineCount();
	//if (LineCnt > 100)
	//{
	//	Log_Edit.SetWindowText(_T(""));
	//}

	CStringW tmpStr;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	tmpStr.Format(L">[%02d:%02d:%02d]", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	m_InfoStr += tmpStr;	// 添加log显示时间
	m_InfoStr += infoStr;	// 需要显示的信息
	m_InfoStr += L"\r\n";	// 换行
	// 将m_InfoStr的值显示到界面
	//UpdateData(FALSE);
	Log_Edit.SetWindowTextW(m_InfoStr);
	// 将滚轮调到最后
	Log_Edit.LineScroll(Log_Edit.GetLineCount());

	int size = m_InfoStr.GetLength();
	//if (size > 1024 * 1000)
	if (size > 40000)//测试用
	{
		Common_Savelog(m_InfoStr);
		m_InfoStr.Empty();
	}

}
