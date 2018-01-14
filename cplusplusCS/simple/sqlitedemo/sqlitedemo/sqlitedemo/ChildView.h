
// ChildView.h : CChildView 类的接口
//
#pragma once

#include "UI/UIDefine.h"

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 属性
public:
    void CreateButtons();
	void CreanteControls();
	void InitTreeDB();

	//button处理
	void OnBtnNewDBClicked();
	void OnBtnOperatDBClicked();
public:
	//数据库处理
	void HandleDBOperation();
	void HandleNewCompany();
	void HandleDeleteCompany();
    void HandleNewDepartment();
	void HandleDeleteDepartment();
	void HandleNewEmploy();
	void HandleDeleteEmploy();

	//树处理
	void RefreshTree();

// 操作
public:
   
// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
// 实现
public:
	virtual ~CChildView();
    
	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

public:
	CButton m_BtnCreateDB;
	CButton m_BtnOpeartDB;
	CTreeCtrl  m_treeDB;
	HTREEITEM  m_curTreeItem;
	DB_Operation  m_dbOperation;
};

