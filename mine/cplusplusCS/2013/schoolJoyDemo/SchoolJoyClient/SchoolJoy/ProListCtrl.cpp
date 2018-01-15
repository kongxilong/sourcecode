// ProListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "ProListCtrl.h"


// CProListCtrl
IMPLEMENT_DYNAMIC(CProListCtrl, CListCtrl)

CProListCtrl::CProListCtrl()
{

}

CProListCtrl::~CProListCtrl()
{
}


BEGIN_MESSAGE_MAP(CProListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CProListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()



// CProListCtrl ��Ϣ��������




void CProListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pLVCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
		*pResult = CDRF_DODEFAULT;
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
		int nSubItem = pLVCD->iSubItem;
		if(nSubItem != 4)     //�ػ������,������������;
			return;
		COLORREF crText  = ::GetSysColor(COLOR_WINDOWFRAME);
		COLORREF crBkgnd = ::GetSysColor(COLOR_WINDOW);
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
		if (GetItemState(nItem, LVIS_SELECTED))
			DrawText(nItem, nSubItem, pDC, ::GetSysColor(COLOR_HIGHLIGHT), 
			::GetSysColor(COLOR_HIGHLIGHT), rect);
		else
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect);
		*pResult = CDRF_SKIPDEFAULT; // We've painted everything.
	}
}


//�����аѽ��ȴ���ItemData��;
void CProListCtrl::DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd,CRect &rect)
{
	ASSERT(pDC);
	pDC->FillSolidRect(&rect, crBkgnd);
	int nProcess = GetItemData(nItem);
	rect.left+=5;
	rect.bottom-=2;
	CRect procRect = rect;
	pDC->Rectangle(procRect);
	procRect.left += 1;
	procRect.bottom -= 1;
	procRect.top += 1;
	procRect.right = procRect.left + rect.Width() * nProcess / 100;
	CBrush brush(RGB(00,236,00));     //��������ɫ;
	pDC->FillRect(&procRect, &brush);
	CString str;
	str.Format(_T("%d%%"), nProcess);
	if (!str.IsEmpty())
	{
		UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_CENTER;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		pDC->DrawText(str, &rect, nFormat);
	}
}



// CProListCtrl ��Ϣ��������

