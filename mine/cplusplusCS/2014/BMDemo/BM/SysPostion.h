#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSysPostion 对话框

class CSysPostion : public CDialogEx
{
	DECLARE_DYNAMIC(CSysPostion)

public:
	CSysPostion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysPostion();

// 对话框数据
	enum { IDD = IDD_DLG_SYSPOSTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl			m_list;
	CSplitButton		m_split;
	CStatic				m_tip;
	int					m_time_take;
	CString             m_pos_batch;
	CString				m_content_to_find;    //将要查找的内容
	CString             m_users_to_delete;     //将要被删除的用户字符串
public:
	void				TransferStrToArr(CString source,CStringArray &des);    //将字符串解开  变成array形式的数据
	void				FillDataToList();
	void				ShowTip();
	void				InitTipCtrl();
	void				InitSplit();
	virtual		BOOL	OnInitDialog();
	void				InitList();
	void				FormatDataByAddComma(CStringArray &_data_source_arr,CString &_result);
	void				PackageData(CStringArray &_data_source_arr,CString &_result);
	void				DelUsersFromList();
	CString				GetItemFromArray(CString source,int index);
protected:
	afx_msg		LRESULT OnEndTask(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnStartTask(WPARAM wParam, LPARAM lParam);
	
public:
	afx_msg		LRESULT OnFillList(WPARAM wParam, LPARAM lParam);
	afx_msg		LRESULT OnDelUsers(WPARAM wParam, LPARAM lParam);
	afx_msg		void	OnBnClickedBtnDelete();
	afx_msg		void	OnClose();
	afx_msg		void	OnBnClickedBtnFindAll();
	afx_msg		void	OnChooseAll();
	afx_msg		void	OnChooseNone();
	afx_msg		void	OnReverseChoose();
	afx_msg		void	OnDelAlll();
	afx_msg		void	OnDelFromList();
	afx_msg		void	OnTimer(UINT_PTR nIDEvent);
	afx_msg		void	OnBnClickedBtnAdd();
	afx_msg		void	OnBnClickedBtnEdit();
	afx_msg		HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
