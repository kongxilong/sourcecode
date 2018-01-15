#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CCtmEdit 对话框
class CCtmInfo;
class CCtmEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CCtmEdit)

public:
	CCtmEdit(CWnd* pParent = NULL);   // 标准构造函数
	CCtmEdit(CCtmInfo* pDlg,CWnd* pParent = NULL);   
	virtual ~CCtmEdit();

// 对话框数据
	enum { IDD = IDD_DLG_CTM_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CCtmInfo*			m_pParentDlg;
	CComboBox			m_combSex;
	CString				m_ctmCall;
	CString				m_ctmNM;
	int					m_ctmNum;
	CString				m_ps;
	double				m_price;
	CString				m_phone;
	CStatic				m_tip;
	int					m_times;
	int					m_time_take;
	CString				m_timeFirst;
	CString				m_timeLate;
	CtmInfo             m_ctmEditStruct;
	CString             m_sexflag;
protected:
	afx_msg		LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void	OnBnClickedBtnSave();
	afx_msg		void	OnBnClickedBtnCancle();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
	afx_msg		void	OnClose();
	void				ShowTip();
	void                LoadDateDeal(CString _strTime,CDateTimeCtrl &date);
	virtual		BOOL    OnInitDialog();
	void                LoadCombOfSex();
	BOOL				IsNum(CString str,BOOL bCheckDot = FALSE);
	BOOL				ContainsCharsRemain(CString str);     //检查字符串是否包含子字符串
	BOOL				IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max,int data_min_len,BOOL bCheckDot = FALSE);
	CDateTimeCtrl m_first_deal;
	CDateTimeCtrl m_late_deal;
	
};
