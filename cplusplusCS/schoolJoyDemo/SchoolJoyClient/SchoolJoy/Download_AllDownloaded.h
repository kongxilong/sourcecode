#pragma once
#include "afxcmn.h"


// CDownload_AllDownloaded 对话框

class CDownload_AllDownloaded : public CDialogEx
{
	DECLARE_DYNAMIC(CDownload_AllDownloaded)

public:
	CDownload_AllDownloaded(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownload_AllDownloaded();

// 对话框数据
	enum { IDD = IDD_DIALOG_Download_AllDownloaded };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_Download_AllDownloaded;
	void InitDownload_AllDownloaded(void);
	BOOL OnInitDialog(void);
};
