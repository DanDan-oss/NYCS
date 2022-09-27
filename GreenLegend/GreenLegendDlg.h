
// GreenLegendDlg.h: 头文件
//

#pragma once


// CGreenLegendDlg 对话框



class CGreenLegendDlg : public CDialogEx
{
// 构造
public:
	CGreenLegendDlg(CWnd* pParent = nullptr);	// 标准构造函数


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GREENLEGEND_DIALOG};
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 玩家信息
	CTabCtrl PlayerInfor;
	// 游戏脚本功能
	CTabCtrl FuncTab;
	CButton EndScript;							//中止辅助
	CButton StartScript;						//开始辅助
	CDialog* pDialog[5];						//功能Tab控件窗口
	CDialog* pPlayDialog[2];					//玩家Tab控件窗口
	int m_CurSelTab;							//功能Tab控件索引
	int m_PlayCurSelTab;						//功能Tab控件索引
	afx_msg void OnTcnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);


protected:
	CRect m_rect;
	void ChangeSize(UINT nID, int x, int y);
	void ReSize(void);
	void ReSize(int  nID, int  cx, int  cy);
	POINT old;

public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedBtnSaveconfig();
	afx_msg void OnBnClickedBtnOpengame();
	afx_msg void OnBnClickedBtnEnd();
	
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCmbConfig();
	// 配置下拉菜单
	CComboBox m_cbCongfig;
	afx_msg void OnEnChangeEdit1();
};

DWORD WINAPI RunScript(_In_ LPVOID lpParameter);


DWORD WINAPI MyThreadFunction(LPVOID lpParam);

int Random(int minValue, int maxValue);
void 联邦任务();
void 添加保护();
void run();
void 提升战力(bool open = false);
void 挂机设置(bool x = false);