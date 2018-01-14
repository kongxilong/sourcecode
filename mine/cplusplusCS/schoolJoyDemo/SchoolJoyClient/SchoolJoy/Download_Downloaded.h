#pragma once
#include "afxcmn.h"


// CDownload_Downloaded 对话框

class CDownload_Downloaded : public CDialogEx
{
	DECLARE_DYNAMIC(CDownload_Downloaded)

public:
	CDownload_Downloaded(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownload_Downloaded();

// 对话框数据
	enum { IDD = IDD_DIALOG_Download_Downloaded };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Download_Downloaded;
	void InitDownload_Downloaded(void);
	BOOL OnInitDialog(void);
};
