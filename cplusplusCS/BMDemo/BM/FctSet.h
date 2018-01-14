#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CFctSet 对话框

class CFctSet : public CPropertyPage
{
	DECLARE_DYNAMIC(CFctSet)

public:
	CFctSet();
	virtual ~CFctSet();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_FCTSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl	m_list;
	CStatic		m_tip;
	int			m_time_take;
	CString     m_strfctToDel;   //将要被删除的厂家名字
public:
	virtual		BOOL	OnInitDialog();
	void				ShowTip();
	void				InitList();
	void				EnableParentDlgShutDownBtn(BOOL nEnable);
	afx_msg		void	OnBnClickedBtnFctNew();
	afx_msg		void	OnBnClickedBtnFctEdit();
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void	OnBnClickedBtnSearchAll();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg		LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		void	OnBnClickedBtnFctdelete();
protected:
	afx_msg LRESULT OnDelFct(WPARAM wParam, LPARAM lParam);
};
