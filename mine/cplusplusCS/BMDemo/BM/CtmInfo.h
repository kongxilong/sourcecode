#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CCtmInfo 对话框

class CCtmInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CCtmInfo)

public:
	CCtmInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCtmInfo();

// 对话框数据
	enum { IDD = IDD_DLG_CTMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl	m_list;
	int			m_time_take;
	CStatic		m_tip;
	CString     m_strToDelCtm;
public:
	void		ShowTip();
	virtual		BOOL	OnInitDialog();
	void		InitList();
	afx_msg		void	OnBnClickedBtnNew();
	afx_msg		void	OnBnClickedBtnEdit();
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void	OnClose();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnSearchall();
	afx_msg void OnBnClickedBtnDelete();
protected:
	afx_msg LRESULT OnDelCtm(WPARAM wParam, LPARAM lParam);
};
