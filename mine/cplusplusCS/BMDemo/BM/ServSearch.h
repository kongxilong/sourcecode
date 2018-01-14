#ifndef SERVSEARCH_H_H_H_H
#define SERVSEARCH_H_H_H_H
#pragma once

// CServSearch 对话框
class CServiceSearch;
class CServSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CServSearch)

public:
	CServSearch(CWnd* pParent = NULL);   // 标准构造函数
	CServSearch(CServiceSearch* pServiceSearch,CWnd* pParent = NULL); 
	virtual ~CServSearch();

// 对话框数据
	enum { IDD = IDD_DLG_SERV_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CServiceSearch*				m_pServiceSearch;
public:
	afx_msg			HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg			void		OnBnClickedBtnFindall();
	afx_msg			void		OnBnClickedBtnSearch();
	void						LoadDefaultChioceFromIni();
	virtual			BOOL		OnInitDialog();
	afx_msg			void		OnBnClickedRadioServDate();
	afx_msg			void		OnBnClickedRadioCtmName();
	CString						m_keyword;
};

#endif