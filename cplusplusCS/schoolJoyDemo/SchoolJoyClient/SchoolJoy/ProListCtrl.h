#pragma once


// CProListCtrl

class CProListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CProListCtrl)

public:
		
	void DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect);
	CProListCtrl();
	virtual ~CProListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};
