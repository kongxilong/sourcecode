#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// COrderIncompletedProgView 窗体视图

class COrderIncompletedProgView : public CFormView
{
	DECLARE_DYNCREATE(COrderIncompletedProgView)

public:
	COrderIncompletedProgView();           // 动态创建所使用的受保护的构造函数
	virtual ~COrderIncompletedProgView();

public:
	enum { IDD = IDD_FORMVIEW_DEAL_INCOMPLETED };
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
	CExListCtrl		m_list;
	CStatic			m_tip;
	int				m_time_take;
	CString         m_list_IncompleteOrder;     //用来保存下载或上传的list内容
	CStringArray    m_OrderToUpdateArr;       //将要更新到服务器的订单列表
public:
	virtual		void		OnInitialUpdate();
	void					InitList();
	void					ShowTip();
	void                    InitSplit();
	void					FillDataToList();
	void					TransferStrToArr(CString source,CStringArray &des);    //将字符串解开  变成array形式的数据
	void					InitTipCtrl();
	CString					GetItemFromArray(CString source,int index);    //从字符串中取得子项数据
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
protected:
	
	BOOL					IsOrderProgVality(CString source); 
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		void		OnBnClickedBtnSave();
	afx_msg		void		OnBnClickedBtnFindDealIncomplete();
protected:
	afx_msg		LRESULT		OnFillList(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnChooseAll();
	CSplitButton m_split;
	afx_msg void OnReverseChoose();
	afx_msg void OnChooseNone();
	afx_msg void OnDelFromList();
	afx_msg void OnClearList();
};


