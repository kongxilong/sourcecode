#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CServReg 对话框

class CServReg : public CDialogEx
{
	DECLARE_DYNAMIC(CServReg)

public:
	CServReg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServReg();

// 对话框数据
	enum { IDD = IDD_DLG_SERVICE_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
//	int m_servPhone;
	void							InitTipCtrl();
	void							InitAllCombobox();
	virtual         BOOL			OnInitDialog();
	afx_msg			HBRUSH			OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg			void			OnTimer(UINT_PTR nIDEvent);
	afx_msg			void			OnClose();
	void							ShowTip();
	afx_msg			void			OnBnClickedBtnCancel();
	afx_msg			void			OnBnClickedBtnSave();
protected:
	afx_msg			LRESULT			OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg			LRESULT			OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	CComboBox			m_ctmSex;
	CComboBox			m_ctmType;
	CComboBox			m_prog;
	CDateTimeCtrl		m_dateDelay;
	CDateTimeCtrl		m_dateReq;
	CString				m_addr;
	double				m_charge;
	CString				m_clientNM;
	CString				m_ctmNM;
	CString				m_ps;
	CString				m_servContent;
	CString				m_servNM;
	CString				m_phone;
	CString				m_servPhone;
	CStatic				m_tip;
	CString				m_servNO;
	int                 m_time_take;
	CString				m_servType;
	ServInfo			m_servInfo_struct;
	
};
