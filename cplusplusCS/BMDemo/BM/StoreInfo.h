#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CStoreInfo 对话框

class CStoreInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CStoreInfo)

public:
	CStoreInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStoreInfo();

// 对话框数据
	enum { IDD = IDD_DLG_STOREINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl				m_list;
public:
	virtual		BOOL		OnInitDialog();
	void					InitList();

	//afx_msg void OnButtonStockInfo();
	afx_msg		void		OnBnClickedBtnStoreAdd();
	afx_msg		void		OnBnClickedBtnStoreOut();
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	afx_msg		void		OnClose();
	void		ShowTip();
	
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	CStatic					m_tip;
	int						m_time_take;
	afx_msg		void		OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnAlertSet();
};
