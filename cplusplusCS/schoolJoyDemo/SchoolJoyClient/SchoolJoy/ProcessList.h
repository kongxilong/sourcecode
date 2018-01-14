#pragma once


// CProcessList

class CProcessList : public CListCtrl
{
	DECLARE_DYNAMIC(CProcessList)

public:
		
	void DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect);
	CProcessList();
	virtual ~CProcessList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


