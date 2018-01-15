#pragma once


// CEmpPerformanceSearch 对话框

class CEmpPerformanceSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CEmpPerformanceSearch)

public:
	CEmpPerformanceSearch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmpPerformanceSearch();

// 对话框数据
	enum { IDD = IDD_DLG_EMP_PERFORMANCE_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnSeach();
};
