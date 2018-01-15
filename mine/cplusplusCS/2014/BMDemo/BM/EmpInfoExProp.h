#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CEmpInfoExProp 对话框

class CEmpInfoExProp : public CPropertyPage
{
	DECLARE_DYNAMIC(CEmpInfoExProp)

public:
	CEmpInfoExProp();
	virtual ~CEmpInfoExProp();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_EMPINFO_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
//	CSplitButton m_split;
	CSplitButton m_split;
	CStatic		m_tip;
	int         m_time_take;
public:
	virtual		BOOL		OnInitDialog();
	void					InitList();
	void					InitSplit();
	
	void					InitTipCtrl();
	afx_msg		void		OnBnClickedBtnEmpexport();
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void		OnBnClickedBtnEmpsearch();
	void					EnableParentDlgShutDownBtn(BOOL Enable);
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	void					ShowTip();
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		void		OnChooseAll();
	afx_msg		void		OnReverseChoose();
	afx_msg		void		OnCancelChoose();
	afx_msg		void		OnDelEmp();
	afx_msg		void		OnClearAll();
};
