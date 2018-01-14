/*======================================================================
  使用说明：

  1) 当 CListCtrl 具有 LVS_EDITLABELS 样式时，
  在 PreSubclassWindow() 函数中会去掉 LVS_EDITLABELS 样式，
  并将第一列设为可编辑列。

  2) 可以通过 SetItemEdit(BOOL bEdit, int nRow, int nCol) 
  来设置第几行或第几列为可编辑的，默认参数-1表示所有行、列均可编辑。

  3) 本类去掉了系统的 LVS_EDITLABELS 样式，改用一个文本框(CListEdit)来实现编辑功能。
  可以通过 GetEditHwnd() 来获取文本框的窗口句柄。

  4) 当需要对某一单元格进行编辑时，双击这个单元格即可。

  ***你可以自由使用本代码，只希望你发现bug时能通知我，谢谢。***

  最新版本下载地址: http://hplight.download.csdn.net/

  作者: hplight@163.com QQ-397108819
  2011-02-20
======================================================================*/
#if !defined(AFX_EXLISTCTRL_H__A082919B_E034_4908_B9C6_0F85FEA688C7__INCLUDED_)
#define AFX_EXLISTCTRL_H__A082919B_E034_4908_B9C6_0F85FEA688C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl window

class CListEdit : public CEdit
{
// Construction
public:
	CListEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	
};


class CExListCtrl : public CListCtrl
{
// Construction
public:
	CExListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExListCtrl)
	protected:
	virtual		void		PreSubclassWindow();
	virtual		LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void                    SetItemEdit(BOOL bEdit, CString strCol, int nRow = -1);
	void					SetItemEdit(BOOL bEdit, int nRow = -1, int nCol = -1);
	void					ShowLableEdit(void);    // 显示文本框
	void					DisposeEdit(void);      // 当 m_edit 失去焦点时被调用
	void					AppendItem(LPCTSTR lpszItem);       // 从末尾添加一项并编辑
	HWND					GetEditHwnd();          // 获取 m_edit 句柄
	BOOL					IsEditCol(int col);    //查看列是否可以编辑
	void					OnCustomdrawMyList(NMHDR** pNMHDR, LRESULT** pResult);

	virtual ~CExListCtrl();

	// Generated message map functions
protected:
	BOOL				m_bItemEdit;  // 是否可编辑
	int					m_nEditRow;    // 第 m_nEditRow 行可以被编辑，-1表示所以行可以被编辑
	int					m_nEditCol;    // 第 m_nEditCol 列可以被编辑，-1表示所以列可以被编辑
	int					m_nCurRow;     // 当前被编辑单元格的行
	int					m_nCurCol;     // 当前被编辑单元格的列
	CString             m_strEditCol;      //当前允许被编辑的列
	//{{AFX_MSG(CExListCtrl)
	afx_msg		void	OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg		void	OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void			CreateLableEdit();
	
	CListEdit		m_edit;   // 单元格上方显示的编辑框
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXLISTCTRL_H__A082919B_E034_4908_B9C6_0F85FEA688C7__INCLUDED_)


