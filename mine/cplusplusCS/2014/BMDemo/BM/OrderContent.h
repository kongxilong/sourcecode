#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"


// COrderContent 对话框

class COrderContent : public CDialogEx
{
	DECLARE_DYNAMIC(COrderContent)

public:
	COrderContent(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrderContent();

// 对话框数据
	enum { IDD = IDD_DLG_ORDER_CONTENT};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox		m_ctmFrom;      //客户来源
	CStatic			m_staticIntrName;    //介绍人姓名
	CStatic			m_staticIntrPhone;   //介绍人电话
	CStatic			m_staticServPersonNM;		//接待人员姓名
	
public:
	CEdit				m_intrName;			//介绍人姓名
	CEdit				m_intrPhone;		//介绍人电话
	CEdit				m_servPerson;		//接待人员
	CStatic				m_static_emp_done;		//业务员跑单
	CEdit				m_empName;			//业务员姓名
	CComboBox			m_businessProg;		//业务进度
	CComboBox			m_payDealChoose;	//付款选择
	CComboBox			m_orderDiscussType;		//谈单类型
	CComboBox			m_earlyStageType;		//前期修改类型
	CDateTimeCtrl		m_date_book;		//放样预约时间
	CDateTimeCtrl		m_date_final_scheme;	//方案定稿日期
	CDateTimeCtrl		m_date_firsttest;		//初测预约时间
	CDateTimeCtrl		m_date_installBOOK;		//安装预约时间
	CDateTimeCtrl		m_date_orderaccept;		//接单日期
	CDateTimeCtrl		m_order_orderfinished;		//完成时间
	CDateTimeCtrl		m_date_out_order_book;		//发单预约时间
	CString				m_accessory_filepath;		//附件文件路径
	CString				m_baseinfo_ps;		//基本信息备注
	CString				m_charge_name;		//负责人姓名
	CString				m_contract_NO;		//合同单号
	double				m_contract_total_price;		//订单总价
	CString				m_ctm_addr;			//顾客地址
	CString				m_ctm_call;			//客户名称
	CString				m_ctm_name;			//顾客姓名
	CString				m_ctm_phone;		//顾客电话
	CString				m_ctm_ps;			//客户备注
	CString				m_persion1_discuss;		//谈单人员1
	CString				m_person2_discuss;		//谈单人员2
	CString				m_person_earlyStageModify;		//前期修改人员
	CString				m_person_exact_measure;		///放样精量人员
	CString				m_factory_choose;			//单位选择
	CString				m_person_install;		//安装人员
	CString				m_problems_install;		//安装问题
	double				m_money_already_take;		//已收金额
	CString				m_order_NO;		//订单编号
	CString				m_person_budget;		//预算人员	
	CString				m_person_out_order;		//发单人员
//	CString				m_upload_status;
	//上传状态
	CProgressCtrl		m_progressCtrl;			//进度条	
	CString				m_persion_first_test;    //初测人员
	CString				m_persion_first_design;   //初次设计人员
	CStatic				m_tip;	
	CStatic				m_static_upload_status;
public:	
	BOOL					IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max,int data_min_len,BOOL bCheckDot = FALSE);
	BOOL					ContainsCharsRemain(CString str);     //检查字符串是否包含子字符串
	BOOL					IsNum(CString str,BOOL bCheckDot = FALSE);
	afx_msg		void		OnBnClickedBtnAccessoryCheck();
	void					ResetCurFileUpload();
	void					ShowUploadStatus();     //显示上传状态
	void					ShowTip();
	afx_msg		void		OnCbnSelchangeComboCtmFrom();
	virtual		BOOL		OnInitDialog();
	void					InitTipCtrl();
	void					InitCtmFromComb();
	void					InitBusinessProgComb();
	void					InitPayDealChooseComb();
	void					InitOrderDiscussTypeComb();
	void					InitEarlyStageTypeComb();
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void		OnBnClickedBtnCancle();
	afx_msg		void		OnBnClickedBtnNew();
	void					CheckCtmFrom();
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	afx_msg		void		OnClose();
	afx_msg		void		OnBnClickedBtnAccessoryChoose();
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	FileInfo                m_fileULInfo;
	CString                 m_filename;        //附件名
	OrderInfo               m_orderInfo;
	int						m_time_take;
	u_long					m_ulCurTotal;    //已经上传文件的数量
	u_long					m_ulFileSize;    //文件总大小
protected:
	u_long					GetFileSize();
	afx_msg		LRESULT		OnStartUploadFile(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnConfirmUploadFile(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnResetUpload(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnFileUpload();
	afx_msg void OnDtnDatetimechangeDatetimeFinalScheme(NMHDR *pNMHDR, LRESULT *pResult);
};
