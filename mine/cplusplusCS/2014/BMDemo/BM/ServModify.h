#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CServModify 对话框

class CServModify : public CDialogEx
{
	DECLARE_DYNAMIC(CServModify)

public:
	CServModify(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServModify();

// 对话框数据
	enum { IDD = IDD_DLG_SERVICE_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg		void		OnBnClickedBtnCancel();
	afx_msg		void		OnBnClickedBtnSave();
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	afx_msg		void		OnClose();
	void					ShowTip();
	virtual     BOOL		OnInitDialog();
	void					InitTipCtrl();
	void					LoadDate(CString _strTime,CDateTimeCtrl &date);
	void					InitComb();
protected:
	afx_msg		LRESULT		OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT		OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	CString			m_servType;
	CStatic			m_tip;
	CComboBox		m_ctmSex;
	CString         m_strCtmSex;
	CString         m_strCtmType;
	CString         m_strProg;
	CComboBox		m_ctmType;
	CComboBox		m_prog;
	CDateTimeCtrl	m_dateDelay;
	CString         m_strDateReq;
	CString         m_strDateDelay;
	CDateTimeCtrl	m_dateReq;
	CString			m_addr;
	double			m_charge;
	CString			m_clientNM;
	CString			m_ctmNM;
	CString			m_phone;
	CString			m_ps;
	CString			m_servContent;
	CString			m_servNM;
	CString		    m_servNO;
	CString 		m_servPhone;
	int             m_time_take;
	ServInfo		m_servInfo_struct;
};
