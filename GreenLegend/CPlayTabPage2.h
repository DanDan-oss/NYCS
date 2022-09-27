#pragma once


// CPlayTabPage2 对话框

class CPlayTabPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(CPlayTabPage2)

public:
	CPlayTabPage2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPlayTabPage2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PlayTabPage2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
