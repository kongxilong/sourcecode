#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"



// CServiceSearch 窗体视图

class CServiceSearch : public CFormView
{
	DECLARE_DYNCREATE(CServiceSearch)

public:
	CServiceSearch();           // 动态创建所使用的受保护的构造函数
	virtual ~CServiceSearch();

public:
	enum { IDD = IDD_FORMVIEW_SERVICE_SEARCH };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox			m_dateType;
	CDateTimeCtrl		m_dateEnd;
	CDateTimeCtrl		m_dateStart;
	CListCtrl			m_list;
	int					m_time_take;
	CStatic				m_tip;
	CString             m_strDelServ;
	CString             m_searchByKeyWord;
public:
	void					InitList();
	void					InitCombDate();
	void					InitTipCtrl();
	virtual		void		OnInitialUpdate();
	afx_msg		void		OnBnClickedBtnEdit();
	afx_msg		void		OnBnClickedBtnSearch();
	void					FindAllServ();
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void					ShowTip();
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	afx_msg		void		OnClose();
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnDelete();
protected:
	afx_msg LRESULT OnDelServ(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnSearchComm();
};


