#pragma once
#include "afxcmn.h"


// CGoodsBook 对话框

class CGoodsBook : public CPropertyPage
{
	DECLARE_DYNAMIC(CGoodsBook)

public:
	CGoodsBook();
	virtual ~CGoodsBook();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_GOODS_BOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
public:
	virtual BOOL OnInitDialog();
	void InitList();
//	afx_msg void OnLvnItemchangedListMtInGoodsBook(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnMtAdd();
};
