#ifndef FACTORYEDIT_H_H_H_H
#define FACTORYEDIT_H_H_H_H
#pragma once
 
#include "FctSet.h"
#include "afxwin.h"

// CFactoryEdit 对话框

class CFactoryEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CFactoryEdit)

public:
	CFactoryEdit(CWnd* pParent = NULL);   // 标准构造函数
	CFactoryEdit(CFctSet* pFactorySet,CWnd* pParent = NULL);
	virtual ~CFactoryEdit();

// 对话框数据
	enum { IDD = IDD_DLG_FACTORY_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFctSet*	m_pFactorySet;
	
	int         m_time_take;
	CString		m_main_product_fctEdit;
	CString		m_name_fctEdit;
	CString		m_phone_fctEdit;
	CString		m_ps_fctEdit;
	CString		m_addr_fctEdit;
	CString		m_contact_person_fctEdit;
	CStatic		m_tip;
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	afx_msg		void		OnClose();
	afx_msg		void		OnBnClickedBtnQuit();
	void					ShowTip();
	afx_msg		void		OnBnClickedBtnSave();
	virtual			BOOL        OnInitDialog();
};

#endif