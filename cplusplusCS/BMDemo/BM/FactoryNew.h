#ifndef FACTORYNEW_H_H_H_H
#define FACTORYNEW_H_H_H_H
#pragma once
#include "FctSet.h"
#include "afxwin.h"

// CFactoryNew 对话框

class CFactoryNew : public CDialogEx
{
	DECLARE_DYNAMIC(CFactoryNew)

public:
	CFactoryNew(CWnd* pParent = NULL);   // 标准构造函数
	CFactoryNew(CFctSet* pfactorySet,CWnd* pParent = NULL); 
	virtual ~CFactoryNew();

// 对话框数据
	enum { IDD = IDD_DLG_FACTORY_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_addr_fctNew;
	CString m_contacts_fctNew;
	CString m_mainProduct_fctNew;
	CString m_name_fctNew;
	CString m_phone_fctNew;
	CString m_ps_fctNew;
	CStatic m_tip;
	int m_time_take;
	CFctSet* m_pfactorySet;
protected:
	afx_msg LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		void	OnBnClickedBtnQuit();
	afx_msg		void	OnBnClickedBtnNew();
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
	virtual		BOOL	OnInitDialog();
	void		ShowTip();
	afx_msg		void	OnClose();
};

#endif