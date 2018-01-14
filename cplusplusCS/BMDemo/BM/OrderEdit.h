#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// COrderEdit 对话框

class COrderEdit : public CDialogEx
{
	DECLARE_DYNAMIC(COrderEdit)

public:
	COrderEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrderEdit();

// 对话框数据
	enum { IDD = IDD_DLG_ORDER_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	OrderInfo	m_orderInfo_edit;
	int			m_time_take;
public:
	void                    InitEditCtrl();
	void                    LoadDate(CString _strTime,CDateTimeCtrl &date);
	void					InitTipCtrl();
	void					InitCtmFromComb();
	void					InitBusinessProgComb();
	void					InitPayDealChooseComb();
	void					InitOrderDiscussTypeComb();
	void					InitEarlyStageTypeComb();
	void                    InitAllCtrls();
	void					ShowTip();
	void					CheckCtmFrom();
	virtual		BOOL		OnInitDialog();
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	afx_msg		void		OnClose();
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	double			m_money_already_take;
	CComboBox		m_prog_business;
	CComboBox		m_ctm_from;
	CComboBox		m_early_stage_type;
	CComboBox		m_order_discuss_type;
	CComboBox		m_pay_deal_choose;
	CDateTimeCtrl	m_date_book;
	CDateTimeCtrl	m_date_final_scheme;
	CDateTimeCtrl	m_date_first_test;
	CDateTimeCtrl	m_date_install_book;
	CDateTimeCtrl	m_date_order_accept;
	CDateTimeCtrl	m_date_order_finished;
	CString			m_ps_base_info;
	CDateTimeCtrl	m_date_out_order_book;
	CString			m_person_charge;
	CString			m_NO_contract;
//	CString m_contract_total_price;
	double			m_contract_total_price;
	CString			m_ctm_addr;
	CString			m_ctm_call;
	CString			m_ctm_name;
	CString			m_ctm_phone;
	CString			m_ctm_ps;
	CString			m_person1_discuss_deal;
	CString			m_person2_discuss_deal;
	CString			m_person_early_stage_modify;
//	CString			m_emp_name;
	CString			m_person_exact_measure;
	CString			m_fct_choose;
	CString			m_install_person;
	CString			m_problems_install;
	CEdit			m_intr_name;
	CEdit			m_intr_phone;
	CString			m_NO_Order;
	CString			m_person_budget;
	CString			m_person_first_design;
	CString			m_person_first_test;
	CString			m_person_out_order;
	CEdit			m_person_serv;
	CEdit			m_emp_done;
	CStatic			m_emp_done_static;
	CStatic			m_intr_name_static;
	CStatic			m_intr_phone_static;
	CStatic			m_serv_person_static;
	CStatic			m_tip;
public:
	afx_msg		void		OnBnClickedBtnGiveup();
	afx_msg		void		OnCbnSelchangeComboCtmFrom();
	BOOL					IsNum(CString str,BOOL bCheckDot = FALSE);
	BOOL					ContainsCharsRemain(CString str);     //检查字符串是否包含子字符串
	BOOL					IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max,int data_min_len,BOOL bCheckDot = FALSE);
	afx_msg		void		OnBnClickedBtnSave();
};
