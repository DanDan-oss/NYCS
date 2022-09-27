#pragma once


// CTabPage1 对话框

class CTabPage1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage1)

public:
	CTabPage1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabPage1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TabPAGE1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 日志消息
	CEdit Log_Edit;
	CStringW m_InfoStr;
	void LogOut(CStringW infoStr);
	void LogOut(CStringA infoStr);
	BOOL Common_Savelog(CString strInfo);
};
