// TabPAGE4.cpp: 实现文件
//

#include "pch.h"
#include "GreenLegend.h"
#include "TabPAGE4.h"
#include "afxdialogex.h"


// TabPAGE4 对话框

IMPLEMENT_DYNAMIC(TabPAGE4, CDialogEx)

TabPAGE4::TabPAGE4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TabPAGE4, pParent)
{
	ShowNumb = 0;
	ListNumb = 0;
}

TabPAGE4::~TabPAGE4()
{
}

void TabPAGE4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_HookMap, m_Cob_InHook_Map1);
	DDX_Control(pDX, IDC_COMBO_MapOrder, m_Cob_InHook_Map2);
	DDX_Control(pDX, IDC_COMBO_EverydayTask, m_Cob_EveryDayTask);
	DDX_Control(pDX, IDC_EDIT_Map, m_Map);
	DDX_Control(pDX, IDC_EDIT_Times, m_ETime);
	DDX_Control(pDX, IDC_LIST_FlowPath, m_LFlowPath);
	DDX_Control(pDX, IDC_CKB_Psychedelic, m_cDifficulty1);
	DDX_Control(pDX, IDC_CKB_Poliphili, m_cDifficulty2);
	DDX_Control(pDX, IDC_CKB_Nightmare, m_cDifficulty3);
	DDX_Control(pDX, IDC_CKB_Order, m_cOrder);
	DDX_Control(pDX, IDC_CKC_Rand, m_cRand);
	DDX_Control(pDX, IDC_EDIT_Map2, m_eStandbyMap);
	DDX_Control(pDX, IDC_CKB_BindMoney, m_cBindMoney);
	DDX_Control(pDX, IDC_EDIT_BindMoney, m_eBindMoney);
}


BEGIN_MESSAGE_MAP(TabPAGE4, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_HookMap, &TabPAGE4::OnCbnSelchangeComboHookmap)
	ON_CBN_SELCHANGE(IDC_COMBO_MapOrder, &TabPAGE4::OnCbnSelchangeComboMaporder)
	ON_BN_CLICKED(IDC_BTN_AddFlowPath, &TabPAGE4::OnBnClickedBtnAddflowpath)
	ON_BN_CLICKED(IDC_BTN_AddTask, &TabPAGE4::OnBnClickedBtnAddtask)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FlowPath, &TabPAGE4::OnNMDblclkListFlowpath)
	ON_BN_CLICKED(IDC_CKB_Order, &TabPAGE4::OnBnClickedCkbOrder)
	ON_BN_CLICKED(IDC_CKC_Rand, &TabPAGE4::OnBnClickedCkcRand)
END_MESSAGE_MAP()


// TabPAGE4 消息处理程序


void TabPAGE4::OnCbnSelchangeComboHookmap()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_Cob_InHook_Map1.GetCurSel();
	CString strCBText;
	m_Cob_InHook_Map1.GetLBText(nIndex, strCBText);

	if (strCBText.Find(L"烟花")>=0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"如雾似梦(战场一)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"如雾似梦(战场二)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"一心一意(战场一)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"一心一意(战场二)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"心心相印(战场一)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"心心相印(战场二)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"飞火流星(战场一)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"飞火流星(战场二)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"绮梦幻想(战场一)";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"绮梦幻想(战场二)";
		m_Cob_InHook_Map2.AddString(Map);

	}
	if (strCBText.Find(L"沃玛暗殿") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"沃玛暗殿东";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"沃玛暗殿西";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"沃玛暗殿南";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"沃玛暗殿北";
		m_Cob_InHook_Map2.AddString(Map);

	}

	if (strCBText.Find(L"祖玛暗殿") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"祖玛暗殿上";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"祖玛暗殿中";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"祖玛暗殿下";
		m_Cob_InHook_Map2.AddString(Map);
	}

	if (strCBText.Find(L"赤月暗殿") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"赤月暗殿上";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"赤月暗殿下";
		m_Cob_InHook_Map2.AddString(Map);
	}
	if (strCBText.Find(L"封魔暗殿") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"封魔暗殿元宝";
		m_Cob_InHook_Map2.AddString(Map);
	}

	if (strCBText.Find(L"幻境") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"幻境一层";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"幻境二层";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"幻境三层";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"幻境四层";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"幻境五层";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"幻境六层";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"幻境七层";
		m_Cob_InHook_Map2.AddString(Map);
	}

	if (strCBText.Find(L"秘之魔境") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"神秘洞穴一层绑金";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"神秘洞穴一层";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"神秘洞穴二层";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"神秘洞穴三层";
		m_Cob_InHook_Map2.AddString(Map);
	}
	
	if (strCBText.Find(L"其他") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"地牢";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"祖玛";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"宫殿长廊";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"月卡地宫";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"跨服龙城";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"未知暗殿";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"金刚试炼";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"龙门三劫";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"屠魔";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"卧龙山庄";
		m_Cob_InHook_Map2.AddString(Map);
		
	}

	if (strCBText.Find(L"节日活动地图") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"节日活动地图1";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"节日活动地图2";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"节日活动地图3";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"节日活动地图4";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"节日活动地图5";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"节日活动地图6";
		m_Cob_InHook_Map2.AddString(Map);
	
	}
	if (strCBText.Find(L"合服活动地图") >= 0)
	{
		m_Cob_InHook_Map2.ResetContent();
		CString Map = L"合服活动地图1";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"合服活动地图2";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"合服活动地图3";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"合服活动地图4";
		m_Cob_InHook_Map2.AddString(Map);
		Map = L"合服活动地图5";
		m_Cob_InHook_Map2.AddString(Map);
		
	}


}


void TabPAGE4::OnCbnSelchangeComboMaporder()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_Cob_InHook_Map2.GetCurSel();
	CString strCBText;
	CString strETText;
	m_Cob_InHook_Map2.GetLBText(nIndex, strCBText);
	m_Map.GetWindowTextW(strETText);
	
	
	if (ShowNumb>1)
	{
		strETText += L"|\r\n";
		ShowNumb = 0;
	}
	if (ShowNumb != 0)
	{
		strETText += L"|";
	}
	strETText += strCBText;
	ShowNumb++;
	
	m_Map.SetWindowTextW(strETText);
}


void TabPAGE4::OnBnClickedBtnAddflowpath()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strETText;
	m_Map.GetWindowTextW(strETText);
	//strETText.Replace(L"\r\n", L"");
	strETText.Remove(L'\r');
	strETText.Remove(L'\n');
	CString strETTimeText;
	m_ETime.GetWindowTextW(strETTimeText);
	/*if (strETTimeText.IsEmpty() == NULL)
	{
		strETTimeText += L' ';
	}
	strETTimeText += strETText;*/
	m_LFlowPath.InsertItem(ListNumb, L"挂机");
	m_LFlowPath.SetItemText(ListNumb,1,strETTimeText);
	m_LFlowPath.SetItemText(ListNumb, 2, strETText);
	ListNumb++;
	ShowNumb = 0;
	m_Map.SetWindowTextW(L"");
}


void TabPAGE4::OnBnClickedBtnAddtask()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_Cob_EveryDayTask.GetCurSel();
	CString strCBText;
	m_Cob_EveryDayTask.GetLBText(nIndex, strCBText);
	
	CString strETTimeText;
	m_ETime.GetWindowTextW(strETTimeText);

	m_LFlowPath.InsertItem(ListNumb, L"日常");
	m_LFlowPath.SetItemText(ListNumb, 1, strETTimeText);
	m_LFlowPath.SetItemText(ListNumb, 2, strCBText);
	ListNumb++;
	ShowNumb = 0;
	m_Map.SetWindowTextW(L"");
}


void TabPAGE4::OnNMDblclkListFlowpath(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION  sSelPos = NULL;
	
	while (sSelPos = m_LFlowPath.GetFirstSelectedItemPosition())
	{
		int nSelItem = -1;
		nSelItem = m_LFlowPath.GetNextSelectedItem(sSelPos);

		if (nSelItem >= 0 && nSelItem < m_LFlowPath.GetItemCount())
		{
			
			m_LFlowPath.DeleteItem(nSelItem);

		}

	}

	
	ListNumb--;
	
	*pResult = 0;
}


void TabPAGE4::OnBnClickedCkbOrder()
{
	// TODO: 在此添加控件通知处理程序代码
	int state = m_cRand.GetCheck();
	int state2 = m_cOrder.GetCheck();
	if (state==BST_CHECKED&& state2 == BST_CHECKED)
	{
		m_cRand.SetCheck(BST_UNCHECKED);
	}

}


void TabPAGE4::OnBnClickedCkcRand()
{
	// TODO: 在此添加控件通知处理程序代码
	int state = m_cRand.GetCheck();
	int state2 = m_cOrder.GetCheck();
	if (state == BST_CHECKED && state2 == BST_CHECKED)
	{
		m_cOrder.SetCheck(BST_UNCHECKED);
	}

}
