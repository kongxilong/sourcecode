// ExListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BM.h"
#include "ExListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListEdit

CListEdit::CListEdit()
{
}

CListEdit::~CListEdit()
{
}


BEGIN_MESSAGE_MAP(CListEdit, CEdit)
	//{{AFX_MSG_MAP(CListEdit)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEdit message handlers

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl

CExListCtrl::CExListCtrl()
{
	m_bItemEdit = FALSE;
	m_nEditRow = -1;
	m_nEditCol = -2;
	m_nCurRow = -1;
	m_nCurCol = -1;
	m_strEditCol.Empty();
}

CExListCtrl::~CExListCtrl()
{
}

void CListEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	CExListCtrl *pParentListCtrl; 

	pParentListCtrl = (CExListCtrl*)GetParent();

	if (NULL == pParentListCtrl || !IsWindow(pParentListCtrl->m_hWnd))
	{
		ASSERT(FALSE);
		return;
	}

	pParentListCtrl->DisposeEdit();
}

BOOL CListEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		SendMessage(WM_KILLFOCUS);
		return TRUE;
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CExListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl message handlers

void CExListCtrl::DisposeEdit()
{
	if (IsWindow(m_edit.m_hWnd) && m_edit.IsWindowVisible())
	{
		CString strLabel;

		m_edit.GetWindowText(strLabel);
		m_edit.ShowWindow(SW_HIDE);

		if (strLabel != GetItemText(m_nCurRow, m_nCurCol))
		{
			SetItemText(m_nCurRow, m_nCurCol, strLabel);
		}
	}

	return ;
}

void CExListCtrl::CreateLableEdit()
{
	UINT uEditStyle = WS_CLIPSIBLINGS | WS_CHILDWINDOW | WS_EX_TOOLWINDOW;

	HWND hEdit = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
		_T("EDIT"), NULL, uEditStyle, 0, 0, 0, 0, m_hWnd, 0, NULL, 0);

	m_edit.SubclassWindow(hEdit);
	m_edit.SetFont(GetFont());  // 与父窗口使用相同字体
}

void CExListCtrl::ShowLableEdit(void)
{
	// 判断此单元格是否使能编辑
	if ((m_nEditRow == -1 || m_nEditRow == m_nCurRow)
		&& IsEditCol(m_nCurCol))    //可以在字符串中找到编辑列
	{
		CRect rtItem;
		GetSubItemRect(m_nCurRow, m_nCurCol, LVIR_LABEL, rtItem);    //得到该单元格矩形的位置大小信息
		rtItem.top -= 2;
		rtItem.bottom += 2;
		
		if (!IsWindow(m_edit.m_hWnd))
		{
			CreateLableEdit();
		}
		
		CString strEdit;
		strEdit = GetItemText(m_nCurRow, m_nCurCol);  //获取该单元格已存在的文本内容
		m_edit.MoveWindow(rtItem);                    //把编辑框移动到该单元格矩形上
		m_edit.SetWindowText(strEdit);                //把单元格原本的内容显示到编辑框上
		m_edit.ShowWindow(SW_SHOW);                   //显示编辑框
		m_edit.SetSel(0, -1);                         //全选编辑框的内容
		m_edit.SetFocus();                            //设置输入焦点在编辑框上
		
		UpdateWindow();
	}else{
		AfxMessageBox(_T("抱歉，该单元格不允许被修改"));
	}
}

BOOL CExListCtrl::IsEditCol(int col)
{
	BOOL flag = FALSE;
	if(m_nEditCol == -1 || m_nEditCol == m_nCurCol)
	{
		flag = TRUE;
	}else{
		CString _str = NULL;
		_str.Format(_T("%d"),col);
		CString edit_col = m_strEditCol;
		if(!edit_col.IsEmpty())
		{
			while(TRUE)
			{
				int index = edit_col.Find('#');
				if(index == -1)
				{
					break;
				}else{
					CString temp = edit_col.Mid(0,index);
					if(temp != _str)
					{
						edit_col = edit_col.Mid(index+1);
					}else{
						flag = TRUE;
						break;
					}
				}
			}
		}
	}
	return flag;
}

void CExListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnLButtonDblClk(nFlags, point);

	LVHITTESTINFO info;
	info.pt = point;
	info.flags = LVHT_ONITEMLABEL;

	if (m_bItemEdit && (SubItemHitTest(&info) >= 0))
	{
		m_nCurRow = info.iItem;     //行索引
		m_nCurCol = info.iSubItem;  //列索引

		ShowLableEdit();
	}
	
	CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CExListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	LVHITTESTINFO info;
	info.pt = point;
	info.flags = LVHT_ONITEMLABEL;

	if (m_bItemEdit && (SubItemHitTest(&info) >= 0))
	{
		m_nCurRow = info.iItem;     //行索引
		m_nCurCol = info.iSubItem;  //列索引
	}
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

HWND CExListCtrl::GetEditHwnd()
{
	return m_edit.m_hWnd;
}

void CExListCtrl::SetItemEdit(BOOL bEdit, CString strCol, int nRow)
{
	m_strEditCol = strCol;
	m_nEditRow = nRow;

	if (m_bItemEdit != bEdit)
	{
		m_bItemEdit = bEdit;

		if (m_bItemEdit)
		{
			ModifyStyle(LVS_EDITLABELS, 0);

			if (!IsWindow(m_edit.m_hWnd))
			{
				CreateLableEdit();
			}
		}
		else
		{
			if (IsWindow(m_edit.m_hWnd))
			{
				m_edit.ShowWindow(SW_HIDE);
			}
		}
	}
}

void CExListCtrl::SetItemEdit(BOOL bEdit, int nRow, int nCol)
{
	m_nEditRow = nRow;
	m_nEditCol = nCol;	

	if (m_bItemEdit != bEdit)
	{
		m_bItemEdit = bEdit;

		if (m_bItemEdit)
		{
			ModifyStyle(LVS_EDITLABELS, 0);

			if (!IsWindow(m_edit.m_hWnd))
			{
				CreateLableEdit();
			}
		}
		else
		{
			if (IsWindow(m_edit.m_hWnd))
			{
				m_edit.ShowWindow(SW_HIDE);
			}
		}
	}
}

void CExListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD uStyle = GetStyle();

	if (uStyle & LVS_EDITLABELS)
	{
		ModifyStyle(LVS_EDITLABELS, 0);

		m_bItemEdit = TRUE;
		m_nEditRow = -1;
		m_nEditCol = 0;
	}
	
	CListCtrl::PreSubclassWindow();
}

LRESULT CExListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_VSCROLL
		|| message == WM_HSCROLL
		|| message == WM_SIZE
		|| message == WM_MOUSEWHEEL
		|| message == WM_LBUTTONDOWN)
	{
		DisposeEdit();
	}
	
	return CListCtrl::WindowProc(message, wParam, lParam);
}

void CExListCtrl::OnCustomdrawMyList(NMHDR** pNMHDR, LRESULT** pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(*pNMHDR);
    
    // Take the default processing unless we set this to something else below.
    **pResult = CDRF_DODEFAULT;
    
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
    {
        **pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
    {
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
        
		if ( m_nCurRow == nItem && m_nCurCol == pLVCD->iSubItem)
		{
			//pLVCD->clrText = RGB(255,0,0);   //Set the text to red
            //pLVCD->clrTextBk = RGB(0,0,0);   //Set the bkgrnd color to blue
		}
        
        **pResult = CDRF_NOTIFYSUBITEMDRAW;
    }
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
    {
        int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
        
		if ( m_nCurRow == nItem && m_nCurCol == pLVCD->iSubItem)
		{
			//pLVCD->clrText = RGB(255,0,0);   //Set the text to red
            //pLVCD->clrTextBk = RGB(0,0,0);   //Set the bkgrnd color to blue
		}

        // Tell Windows to paint the control itself.
        **pResult = CDRF_DODEFAULT;
    }
}

void CExListCtrl::AppendItem(LPCTSTR lpszItem)
{
	m_nCurRow = GetItemCount();
	m_nCurCol = 0;

	InsertItem(GetItemCount(), lpszItem);
	ShowLableEdit();
}