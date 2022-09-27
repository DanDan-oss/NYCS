#pragma once


// CPlayTabPage1 对话框

class CPlayTabPage1 : public CDialogEx
{
	DECLARE_DYNAMIC(CPlayTabPage1)

public:
	CPlayTabPage1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPlayTabPage1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PlayTabPage1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 玩家信息控件
	CListCtrl PlayInfoList;
};
