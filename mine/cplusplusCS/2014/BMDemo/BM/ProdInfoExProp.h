#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CProdInfoExProp 对话框

class CProdInfoExProp : public CPropertyPage
{
	DECLARE_DYNAMIC(CProdInfoExProp)

public:
	CProdInfoExProp();
	virtual ~CProdInfoExProp();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_PRODUCTINFO_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl			m_list;
	CSplitButton		m_split;
	int					m_time_take;
	CStatic				m_tip;
	CString             m_prod_batch;    //批量产品数据
public:
	void					InitSplit();
	void					InitList();
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	void					InitTipCtrl();
	virtual		BOOL		OnInitDialog();
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString					GetItemFromArray(CString source,int index);     //从字符数组中取得子项数据
	void					TransferStrToArr(CString source,CStringArray &des);    //将字符串解开  变成array形式的数据 主要用于批量传输
	void					ShowTip();
	void					FillDataToList();
	void					EnableParentDlgShutDownBtn(BOOL Enable);
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnChooseAll();
	afx_msg void OnChooseNone();
	afx_msg void OnReverseChoose();
	afx_msg void OnDelFromList();
	afx_msg void OnClearList();
	afx_msg void OnBnClickedBtnDataExport();

	afx_msg void OnBnClickedBtnProdsearch();
protected:
	afx_msg LRESULT OnHandleBatch(WPARAM wParam, LPARAM lParam);
};
