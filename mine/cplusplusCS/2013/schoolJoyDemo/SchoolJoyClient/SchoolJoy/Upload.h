#pragma once
#include "afxcmn.h"



// CUpload 对话框

class CUpload : public CDialogEx
{
	DECLARE_DYNAMIC(CUpload)

public:
	CUpload(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpload();

// 对话框数据
	enum { IDD = IDD_DIALOG_Upload };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_Upload;
	void InitUpload(void);
	BOOL OnInitDialog(void);
	afx_msg void OnBnClickedButtonUploadbrowse();
	CString CalculateFileSize(ULONGLONG ulong);
	int m_uploadNum;
	CString intToStr(int n);
	CString strFilePath;
	CString strFileName;
	CString strFileUserList;
	CString strFileTotalSize;
	int onUploadflag;
	float fFileTotalSize;
	CString strFileType;
};
