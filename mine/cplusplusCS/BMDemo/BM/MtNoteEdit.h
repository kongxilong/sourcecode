#ifndef MTNOTEEDIT_H_H_H_H
#define MTNOTEEDIT_H_H_H_H
#pragma once
#include "MtSetProp.h"

// CMtNoteEdit 对话框

class CMtNoteEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CMtNoteEdit)

public:
	CMtNoteEdit(CWnd* pParent = NULL);   // 标准构造函数
	CMtNoteEdit(CMtSetProp* pMtSetProp,CWnd* pParent = NULL);
	virtual ~CMtNoteEdit();

// 对话框数据
	enum { IDD = IDD_DLG_MT_NOTE_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMtSetProp* m_pMtSetProp;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_nodeOrgName;
	CString m_nodeNameNew;
	afx_msg void OnBnClickedBtnNoteSave();
	afx_msg void OnBnClickedBtnQuit();
};

#endif