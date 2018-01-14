#pragma once
#include "afxcmn.h"
//#include "ConnectSet.h"

// CSearch 对话框
#define WM_ONOKSearch     (100)

//class CConnectSet;
class CSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CSearch)

public:
	CSearch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearch();

// 对话框数据
	enum { IDD = IDD_DIALOG_Search };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_Search;
//	CString m_SelectedFileName;
	//CString m_SelectedFileType;
//	CString m_SelectedFilePath;
	void InitSearch(void);
	BOOL OnInitDialog(void);
//	afx_msg void OnClickListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_SearchFile;
	afx_msg void OnBnClickedButtonSearchtofind();
	afx_msg void OnBnClickedButtonAddtodownload();
	afx_msg void OnDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString FileSizeTransfer(double &NUM);
	//CConnectSet connectset;
protected:
	afx_msg LRESULT OnOnOKSearch(WPARAM wParam, LPARAM lParam);

};
