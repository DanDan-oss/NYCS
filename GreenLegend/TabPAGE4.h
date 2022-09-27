#pragma once


// TabPAGE4 对话框

class TabPAGE4 : public CDialogEx
{
	DECLARE_DYNAMIC(TabPAGE4)

public:
	TabPAGE4(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TabPAGE4();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TabPAGE4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//显示地图数量
	int ShowNumb;

	DECLARE_MESSAGE_MAP()
public:
	// 挂机主地图
	CComboBox m_Cob_InHook_Map1;
	// 挂机第几张图
	CComboBox m_Cob_InHook_Map2;
	// 每日任务
	CComboBox m_Cob_EveryDayTask;
	afx_msg void OnCbnSelchangeComboHookmap();
	afx_msg void OnCbnSelchangeComboMaporder();
	// 地图显示编辑框
	CEdit m_Map;
	
	afx_msg void OnBnClickedBtnAddflowpath();
	// 时间编辑框
	CEdit m_ETime;
	// 流程表
	CListCtrl m_LFlowPath;
	//list子项数量
	int ListNumb;
	afx_msg void OnBnClickedBtnAddtask();
	afx_msg void OnNMDblclkListFlowpath(NMHDR* pNMHDR, LRESULT* pResult);
	// 幻境难度迷幻
	CButton m_cDifficulty1;
	// 幻境绮梦难度
	CButton m_cDifficulty2;
	// 幻境噩梦难度
	CButton m_cDifficulty3;
	// 顺序进图
	CButton m_cOrder;
	// 随机进图
	CButton m_cRand;
	// 备用地图编辑框
	CEdit m_eStandbyMap;
	afx_msg void OnBnClickedCkbOrder();
	afx_msg void OnBnClickedCkcRand();
	// 低绑不进
	CButton m_cBindMoney;
	// 设定绑金数量
	CEdit m_eBindMoney;
};
