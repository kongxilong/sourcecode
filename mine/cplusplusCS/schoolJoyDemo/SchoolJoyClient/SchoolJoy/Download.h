#pragma once
#include "afxcmn.h"
#include "download_downloading.h"
#include "download_downloaded.h"
#include "download_alldownloaded.h"


// CDownload 对话框

class CDownload : public CDialogEx
{
	DECLARE_DYNAMIC(CDownload)

public:
	CDownload(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownload();

// 对话框数据
	enum { IDD = IDD_DIALOG_Download };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CListCtrl m_List_Download;
	BOOL OnInitDialog(void);
	void InitDownload(void);
	CTabCtrl m_tab_download;
	CDownload_Downloading m_download_downloading;
	CDownload_Downloaded m_download_downloaded;
	CDownload_AllDownloaded m_download_alldownloaded;
	afx_msg void OnSelchangeTabDownload(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStartdownload();
};
