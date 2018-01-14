#pragma once
#include "afxwin.h"


// CStoreAdd 对话框

class CStoreAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CStoreAdd)

public:
	CStoreAdd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStoreAdd();

// 对话框数据
	enum { IDD = IDD_DLG_STOREADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual         BOOL            OnInitDialog();
	afx_msg			HBRUSH			OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg			void			OnBnClickedBtnCancle();
	afx_msg			void			OnBnClickedBtnStoreadd();
	afx_msg			void			OnTimer(UINT_PTR nIDEvent);
	afx_msg			void			OnClose();
	void							ShowTip();
protected:
	afx_msg			LRESULT			OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg			LRESULT			OnEndTask(WPARAM wParam, LPARAM lParam);
public:
	CString			m_name_store;
	CString			m_num_add;
	CString			m_num_alert;
	CStatic			m_tip;
	int				m_time_take;
	StoreInfo       m_storeAddStruct;
};
