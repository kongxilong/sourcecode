#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"


// CPerformanceMgView 窗体视图
#include "EmpPerformanceSearch.h"
#include "DeductSet.h"
#include "afxwin.h"
class CPerformanceMgView : public CFormView
{
	DECLARE_DYNCREATE(CPerformanceMgView)

public:
	CPerformanceMgView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPerformanceMgView();

public:
	enum { IDD = IDD_FORMVIEW_PERFORMANCE_MG };
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
	CExListCtrl			m_list;
	CDeductSet			m_deductSetDlg;
	CSplitButton		m_split;
	CStatic				m_tip;
	CString				m_keyword;
	int					m_time_take;
	CString             m_str_emp_performance_batch;
public:
	BOOL					IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max);    //true  有效 否则无效  
	void					TransferStrToArr(CString source,CStringArray &des);    //将字符串解开  变成array形式的数据
	CString                 GetItemFromArray(CString source,int index);     //从字符串中取得子项数据
	virtual		void		OnInitialUpdate();
	void					InitList();
	void					ShowTip();
	void					InitSplit();
	void					InitTipCtrl();
	void					FillDataToList();
	afx_msg		void		OnBnClickedBtnSearch();
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnFillDatatoList(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDeductSet();
	afx_msg void OnAddAllDeduct();
	afx_msg void OnGetDeduct();
	afx_msg void OnChooseAll();
	afx_msg void OnChooseNone();
	afx_msg void OnReverseChoose();
	afx_msg void OnClearCurList();
	afx_msg void OnDelFromList();
};


