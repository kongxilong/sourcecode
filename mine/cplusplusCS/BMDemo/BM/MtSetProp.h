#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMtSetProp 对话框

class CMtSetProp : public CPropertyPage
{
	DECLARE_DYNAMIC(CMtSetProp)

public:
	CMtSetProp();
	virtual ~CMtSetProp();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_MTSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl     m_treeMaterial;
	HTREEITEM     m_hItemCur;
	HTREEITEM     m_hItemChecked; 
	CString		  m_nodeName;
	CString       m_strMtInfo;
	CString       m_parentNodeName;   //新建节点的父节点名称
	CString       m_nodeNameNew;     //新建节点名称

	CString       m_nodeNameNew_Edit;  //修改名称的节点
	CString       m_nodeNameOrg_Edit;   //接收从编辑窗口传过来的值
public:
	virtual		BOOL	OnInitDialog();
	void				InitTree();
	//void WriteStatus();
	afx_msg		void   OnBnClickedBtnNew();
	afx_msg		void   OnBnClickedBtnMtEdit();
	afx_msg		void   OnBnClickedBtnRefresh();
public:
	CStringArray	m_strArr;
	void			TransferSecondNode();
	void			PaintTree(HTREEITEM hItem);		//填充树视
	afx_msg			LRESULT		OnPaitMtTree(WPARAM wParam, LPARAM lParam);
	afx_msg			void		OnItemExpandingTreeMt(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg			HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CStatic		m_status;
	CStatic		m_timeTakeStatus;
	int			timeTake;   
protected:
	afx_msg LRESULT OnUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReqNoteNew(WPARAM wParam, LPARAM lParam);
public:
	
	//afx_msg void OnClose();
	afx_msg void OnClose();
	void   EnableParentDlgShutDownBtn(BOOL nEnable);   //禁用属性页关闭窗口按钮
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void    ShowTimeTakeStatus();
	
protected:
	afx_msg LRESULT OnStratReqMt(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndReqMt(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearMtTree(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMDblclkTreeMaterial(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnReqNodeEdit(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnMtDelete();
};
