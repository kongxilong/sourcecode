#pragma once
#include "afxcmn.h"
#include "afxwin.h"


#define WM_CURSORBACKNORMAL     (WM_USER+1000)
// CEmpInfoView 窗体视图

class CEmpInfoView : public CFormView
{
	DECLARE_DYNCREATE(CEmpInfoView)

public:
	CEmpInfoView();           // 动态创建所使用的受保护的构造函数
	virtual ~CEmpInfoView();

public:
	enum { IDD = IDD_DLG_EMPLOYEEINFOVIEW };
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
	virtual			void		OnInitialUpdate();
	void						ShowTip();
	void						InitList();
	afx_msg			LRESULT		OnCursorToNormal(WPARAM wParam, LPARAM lParam);
	afx_msg			HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg			void		OnBnClickedBtnEmpNew();
	afx_msg			void		OnBnClickedBtnEmpEdit();
	afx_msg			void		OnBnClickedBtnSearch();
public:
	afx_msg			void		OnTimer(UINT_PTR nIDEvent);
	afx_msg			void		OnBnClickedBtnEmpDel();
protected:
	afx_msg			LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg			LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
	
public:
	CString         m_searchByKeyWord;   //依据关键字搜索
	CStatic			m_tip;
	CListCtrl		m_list;	
	int				m_time_take;
	CString			m_empToDelete;   //将要被删除的员工
protected:
	afx_msg			LRESULT		OnDelSelEmpInfo(WPARAM wParam, LPARAM lParam);
};


