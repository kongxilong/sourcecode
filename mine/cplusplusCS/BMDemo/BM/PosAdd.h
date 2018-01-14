#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPosAdd 对话框

class CPosAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CPosAdd)

public:
	CPosAdd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPosAdd();

// 对话框数据
	enum { IDD = IDD_DLG_POS_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic		m_tip;
	CString		m_emp_num;
	CString		m_password_confirm;
	CString		m_emp_ps;
	CString		m_emp_name;
	CString		m_emp_password;
	int			m_time_take;
	CComboBox	m_pos_name;
	CString     m_str_pos_new;    //用于将新建角色的信息装到这个字符串中
public:
	void				ShowTip();
	void				InitTipCtrl();
	BOOL				IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max);    //true  有效 否则无效  
	BOOL				ContainsCharsRemain(CString str);     //检查字符串是否包含子字符串
	void				InitPosCombox();
	BOOL                IsNum(CString str);    //传入一个字符串 ，检查是否为数字
	void				AddItemToString(CString &_source,CString item_new,int flag);
	virtual		BOOL	OnInitDialog();
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg		void	OnBnClickedBtnSavePos();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg		LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
public:

	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnCancel();
};
