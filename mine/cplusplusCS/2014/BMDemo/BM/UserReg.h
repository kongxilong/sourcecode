#pragma once


// CUserReg 对话框

class CUserReg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserReg)

public:
	CUserReg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserReg();

// 对话框数据
	enum { IDD = IDD_DIALOG_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
