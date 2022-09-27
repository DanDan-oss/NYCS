#pragma once


// CTabPage2 对话框

class CTabPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage2)

public:
	CTabPage2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabPage2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TabPAGE2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 红药编辑框
	CEdit m_eRedNumb;
	// 蓝药数量
	CEdit m_eBlueNumb;
	// 雪霜困绳数量
	CEdit m_eSnowNumb;
	// 疗伤捆绳
	CEdit m_eCureNumb;
	// 回城石数量
	CEdit m_eBackNumb;
	// 随机石数量
	CEdit m_eRandNumb;
	// 回城卷数量
	CEdit m_eBackRollNumb;
	// 修复神水数量
	CEdit m_eFixWaterNumb;
	// 购买修复神水
	CButton m_cFixWaterStart;
	// 购买回城卷
	CButton m_cBackRollStart;
	// 购买随机石
	CButton m_cRandStart;
	// 购买回城石
	CButton m_cBackRockStart;
	// 购买疗伤捆绳
	CButton m_cCureStart;
	// 购买雪霜捆绳
	CButton m_cSnowStart;
	// 无蓝回城
	CButton m_cNoBlueStart;
	// 无红回城
	CButton m_cNoRedStart;
	// 回城分解
	CButton m_cBackResolveStart;
	// 回城回收
	CButton m_cBackRecycleStart;
	// 存物品
	CButton m_cSaveGoods;
	// 存药品
	CButton m_cSaveDrug;
	// 红毒
	CButton m_cRedPoison;
	// 绿毒
	CButton m_cGreenPoison;
	// 护身符
	CButton m_cAmulet;
	// 护身符编辑框
	CEdit m_eAmulet;
	// 自动打防
	CButton m_cAutoDefend;
	// 红毒编辑框
	CEdit m_eRedPoison;
	// 绿毒编辑框
	CEdit m_eGreenPoison;
	// 地牢逃脱包数量编辑框
	CEdit m_eDungeon;
	// 启动购买地牢逃脱包
	CButton m_cDungeon;
	// 背包剩余格数回城启动
	CButton m_cPackSurplus;
	// 背包剩余回城编辑框
	CEdit m_ePackSurplus;
	// 隐藏人物和怪物
	CButton m_cHideMaster;
	afx_msg void OnBnClickedCkbHide();
	afx_msg void OnBnClickedCkbAutodefend();
	// 自动打防秒数
	CEdit m_eTimes;
	// 法师放火
	CButton m_cFire;
	// 法师放火时间
	CEdit m_eFire;
	// 道士毒怪
	CButton m_cPoisonMonster;
	// 自动复活
	CButton m_cResurgence;
};
