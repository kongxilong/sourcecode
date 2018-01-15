#ifndef MAINFORMVIEW_H_H_H_H
#define MAINFORMVIEW_H_H_H_H
#pragma once
#include "afxcmn.h"
#include "afxwin.h"




// CMainFormView 窗体视图

class CMainFormView : public CFormView
{
	DECLARE_DYNCREATE(CMainFormView)

public:
	CMainFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMainFormView();

public:
	enum { IDD = IDD_DLG_MAINVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:

	CListCtrl		m_list;
	CPoint			m_point;			//记录写字的位置
	CRichEditCtrl	m_richEdit;
	CFont			m_editFont;
	CString			m_title_note;      //通知的标题
	CString         m_persion_public_note;   //通知发布者
	CString			m_content_note;			//通知的内容
	CString         m_time_note_public;     //通知发布的时间
	int				m_nNoteToLook;          //查找多少条通知
	CString         m_noteToDelete;         //想要删除的通知 按时间标准删除
	CString         m_note_to_find;         //想要查看的通知
public:
	void				DeleteSelNote();   //删除选中的通知
	void				ShowTimeTakeStatus();
	void				LoadNote();
	virtual		void	OnInitialUpdate(); // called first time after construct
	void				InitList();
	void				InitRichEdit();
	void				WriteFont();
	virtual		void	OnDraw(CDC* pDc);
	void				StartInitNote();
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg		void	OnPaint();

	afx_msg		void	OnBnClickedBtnPublic();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg		LRESULT OnShowNoteInEdit(WPARAM wParam, LPARAM lParam);
public:
	CStatic		m_status;
	int			m_time_take;
	//BOOL		m_bShowStatus;
public:
	void				ReqInitNoteEnd();
	//afx_msg LRESULT OnAfterLongIn(WPARAM wParam, LPARAM lParam);
	afx_msg		void	OnBnClickedBtnDel();
	afx_msg		void	OnBnClickedBtnRefresh();
protected:
	afx_msg		LRESULT OnReqStartInitNote(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnReqInitNoteEnd(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		void	OnBnClickedBtnFindMore();
protected:
	afx_msg		LRESULT OnDelSelNote(WPARAM wParam, LPARAM lParam);
public:
	afx_msg		void OnNMDblclkListNotify(NMHDR *pNMHDR, LRESULT *pResult);
};


#endif