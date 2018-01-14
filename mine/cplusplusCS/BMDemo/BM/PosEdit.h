#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPosEdit 对话框

class CPosEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CPosEdit)

public:
	CPosEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPosEdit();

// 对话框数据
	enum { IDD = IDD_DLG_POS_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic			m_tip;
	CString			m_emp_num;
	CString			m_password_confirm;
	CString			m_emp_ps;
	CString			m_emp_name;
	CString			m_emp_password;
	CComboBox		m_pos_name;
	int				m_time_take;
	CString         m_str_pos_name;
	CString         m_str_pos_authorities;     //用户权限
	CString			m_str_pos_edit;
public:
	virtual		BOOL	OnInitDialog();
	void				InitTipCtrl();
	void				AddItemToString(CString &_source,CString item_new,int flag);
	void				InitUserAuthority();
	CString				GetItemFromArray(CString source,int index);     //从字符串中取得子项数据
	void				InitComb();
	void				ShowTip();
	BOOL				IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max);    //true  有效 否则无效  
	void				LoadUser();   //加载用户权限
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void	OnBnClickedBtnSavePos();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
	afx_msg		void	OnClose();
	afx_msg		void	OnBnClickedBtnCancel();
protected:
	afx_msg		LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnInitUserAuthority(WPARAM wParam, LPARAM lParam);
};
