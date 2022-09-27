#pragma once


// CTabPage3 对话框

class CTabPage3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage3)

public:
	CTabPage3(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabPage3();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TabPAGE3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 血少回城
	CButton m_cLittleRedBack;
	// 血少随机
	CButton m_cLittleRedRand;
	// 血少下线
	CButton m_cLittleRedDown;
	// 血少截图
	CButton m_cLittleRedScreenshot;
	// 回城等待
	CButton m_cBackWait;
	// 进图随机
	CButton m_cJoinRand;
	// 低于血量回城
	CEdit m_eLittleRedBack;
	// 低于血量随机
	CEdit m_eLittleRedRand;
	// 低于下线
	CEdit m_eLittleRedDown;
	// 低于截图
	CEdit m_eLittleRedScreenshot;
	// 回城等待时长编辑框
	CEdit m_eBackWait;
	// 时间无怪随机
	CButton m_cTimeNoMasterRand;
	// 周围无怪随机
	CButton m_cRoundNoMasterRand;
	// 背包剩余
	CButton m_cKnapsackResidue;
	// 金刚石装备回城
	CButton m_cAdamasEquipment;
	// 多少时间随机
	CEdit m_eTimeNoMasterRand;
	// 多少范围随机
	CEdit m_eRoundNoMasterRand;
	// 背包剩余多少格回收
	CEdit m_eKnapsackResidue;
	// 多少金刚石装备回城
	CEdit m_eAdamasEquipment;
	// 黑名单编辑框
	CEdit m_eBlackList;
	// 遇黑名单随机
	CButton m_cBlackListRand;
	// 遇黑名单回城
	CButton m_cBlackListBack;
	// 上线攻击模式
	CComboBox m_cPattern;
	//攻击模式下标
	DWORD iPattern;
	afx_msg void OnBnClickedCkbBlackrand();
	afx_msg void OnBnClickedCkbBlackback();
	afx_msg void OnEnChangeEditBlack();

	afx_msg void OnBnClickedCkbNomastertime();
	
	afx_msg void OnCbnSelchangeCmbPattern();
	afx_msg void OnCbnDropdownCmbPattern();
	// 挂机坐标点
	CButton m_cFitXY;
	// 挂机坐标点X编辑框
	CEdit m_eFitXY;
	// 坐标点归位时间
	CEdit m_eXYTime;
	// 挂机y坐标
	CEdit m_EDIT_Y;
	// 黑名单打人
	CButton m_cBlackFit;
	// 打人延迟
	CEdit m_eFit;
};
