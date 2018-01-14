#ifndef MTNOTENEW_H_H_H_H
#define MTNOTENEW_H_H_H_H
#pragma once


// CMtNoteNew 对话框
class CMtSetProp;
class CMtNoteNew : public CDialogEx
{
	DECLARE_DYNAMIC(CMtNoteNew)

public:
	CMtNoteNew(CWnd* pParent = NULL);   // 标准构造函数
	CMtNoteNew(CMtSetProp* pMtSetProp,CWnd* pParent = NULL); 
	virtual ~CMtNoteNew();

// 对话框数据
	enum { IDD = IDD_DLG_MT_NOTE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMtSetProp* m_pMtSetProp;
	afx_msg void OnBnClickedBtnNoteNew();
	
	afx_msg void OnBnClickedBtnQuit();
	CString m_parentNode;
	CString m_nodeNew;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

#endif