#pragma once
#include "afxcmn.h"


// CStoreAlert 对话框

class CStoreAlert : public CDialogEx
{
	DECLARE_DYNAMIC(CStoreAlert)

public:
	CStoreAlert(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStoreAlert();

// 对话框数据
	enum { IDD = IDD_DLG_STORE_ALERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
