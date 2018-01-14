#ifndef DBCONF_H_H_H_H
#define DBCONF_H_H_H_H

#pragma once
#include "afxwin.h"


// CDBConf 对话框

class CDBConf : public CDialogEx
{
	DECLARE_DYNAMIC(CDBConf)

public:
	CDBConf(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDBConf();

// 对话框数据
	enum { IDD = IDD_DLG_DBCONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDbnew();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnLoginContinue();
	virtual BOOL OnInitDialog();
	void WriteToText(CString& strToWtrite,CString& writeToPath);		//将str 写到path路径的文件中 覆盖原有的
	BOOL ReadFromTxt(CString& path,CString& result);
	void WriteCombToFile(void* comb, CString path);				//将端口写到MyPort.text文件中
	BOOL LoadComb(void* ptr,CString& path);       //从文本文件中加载combox 的内容
public:
	
	CComboBox m_DBName;
	CComboBox m_ip;
	CComboBox m_port;
	//int m_portDefault;        /////当没有文件存在时就加载一个默认的端口
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


#endif