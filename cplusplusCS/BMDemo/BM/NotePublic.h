#ifndef NOTEPUBLIC_H_H_H_H
#define NOTEPUBLIC_H_H_H_H
#pragma once

#include "afxwin.h"

// CNotePublic 对话框
class CMainFormView;
class CNotePublic : public CDialogEx
{
	DECLARE_DYNAMIC(CNotePublic)

public:
	CNotePublic(CWnd* pParent = NULL);   // 标准构造函数
	CNotePublic(CMainFormView* pView, CWnd* pParent = NULL);
	virtual ~CNotePublic();

// 对话框数据
	enum { IDD = IDD_DLG_NOTE_PUBLIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMainFormView* m_pMainView;
	CString		m_title;
	CString		m_personPublicName;
	CString		m_content;
	CString		m_cur_time;
public:
	virtual     BOOL    OnInitDialog();
	afx_msg		void	OnBnClickedBtnPublic();
	afx_msg		void	OnBnClickedBtnCancel();
	//void   ShowNote();   //调整信息格式  显示公告的
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void		EnableSysShutDownBtn(BOOL bEnable); 
	void		GetCurTime(CString &time);
public:
	afx_msg LRESULT OnReqNotePublic(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndNotePublic(WPARAM wParam, LPARAM lParam);
public:
	CStatic		m_status;
	CStatic		m_time_take;
	int			timeTake;      //时间消耗
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	void		ShowTimeTakeStatus();
	void        NotePublicSuccess();
protected:
	afx_msg LRESULT OnNotePublicSuccessMainView(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnCloseWindowNotePublic(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
};
#endif 