#pragma once


// TabPAGE5 对话框

class TabPAGE5 : public CDialogEx
{
	DECLARE_DYNAMIC(TabPAGE5)

public:
	TabPAGE5(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TabPAGE5();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TabPAGE5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 绿色平台启动游戏
	CButton m_cGreenStart;
	// 173平台启动游戏
	CButton m_c173Start;
	// 游戏路径编辑框
	CEdit m_eGamePath;
	// 游戏账号编辑框
	CEdit m_eAccount;
	// 游戏密码编辑框
	CEdit m_ePassWord;
	// 自动登录
	CButton m_cAutoLogin;
	// 掉线重连
	CButton m_cAutoContinue;
	
	
	// 游戏大区
	CComboBox m_cGreen;
	afx_msg void OnBnClickedCheckGreen();
	afx_msg void OnBnClickedCheck173();
	// 关闭组队
	CButton m_CloseTeam;
	// 掉线等待时长
	CEdit m_eWaitTimes;
	afx_msg void OnBnClickedCkbMoreopen();
};
