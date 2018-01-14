#pragma once
#include "afxwin.h"


// CEmpEdit 对话框
class CEmpInfoView;
class CEmpEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CEmpEdit)

public:
	CEmpEdit(CWnd* pParent = NULL);   // 标准构造函数
	CEmpEdit(CEmpInfoView* p_dlg,CWnd* pParent = NULL);   //自定义构造函数
	virtual ~CEmpEdit();

// 对话框数据
	enum { IDD = IDD_DLG_EMP_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL		 IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max,int data_min_len,BOOL bCheckDot = FALSE);
	virtual BOOL OnInitDialog();
	BOOL		 ContainsCharsRemain(CString str);
	BOOL		 IsNum(CString str,BOOL bCheckDot = FALSE);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void   ShowTip();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CEmpInfoView* m_pEmpInfoView;
	CString m_addr_emp;
	CString m_name_emp;
	CString m_num_emp;
	CString m_phone_emp;
	CString m_ps_emp;
	CString m_pos_emp;
	CStatic m_tip;
	int     m_time_take;
protected:
	afx_msg LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnClose();
};
