#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CCustomInfoExProp 对话框

class CCustomInfoExProp : public CPropertyPage
{
	DECLARE_DYNAMIC(CCustomInfoExProp)

public:
	CCustomInfoExProp();
	virtual ~CCustomInfoExProp();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CUSTOMINFO_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl			m_list;
	CSplitButton		m_split;
	CStatic				m_tip;
	int					m_time_take;
	CString             m_ctm_batch_from_db;     //接收从数据库读到的所有客户资料
public:
	virtual		BOOL	OnInitDialog();
	void				InitList();
	void				InitTipCtrl();
	void				InitSplit();
	void				ShowTip();
	CString				GetItemFromArray(CString source,int index);     //从字符数组中取得子项数据
	void				TransferStrToArr(CString source,CStringArray &des);    //将字符串解开  变成array形式的数据 主要用于批量传输
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);

protected:
	afx_msg		LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	void				EnableParentDlgShutDownBtn(BOOL Enable);
	afx_msg		void	OnChooseAlll();
	afx_msg		void	OnChooseNone();
	afx_msg		void	OnReverseChoose();
	afx_msg		void	OnDelFromList();
	afx_msg		void	OnClearList();
	afx_msg		void	OnBnClickedBtnFindAllctm();
	void				FillDataToList();
protected:
	afx_msg		LRESULT OnHandleBatchCtm(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		void	OnBnClickedBtnDataExport();
};
