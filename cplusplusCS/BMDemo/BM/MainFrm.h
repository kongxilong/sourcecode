// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.h : CMainFrame 类的接口
//

#pragma once

//class CMtAndFctSheet;
class CMainFormView;
class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	//CMFCRibbonStatusBar  m_wndStatusBar;
	CMFCCaptionBar    m_wndCaptionBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	DECLARE_MESSAGE_MAP()

	BOOL CreateCaptionBar();
public:
	void SwitchToView(int nForm);
	afx_msg void OnBtnDataImport();
	afx_msg void OnButtonEmployeeinfo();
	afx_msg void OnButtonHome();
	afx_msg void OnButtonDataExport();
	afx_msg void OnButtonSysytempostion();
private:
	bool m_bForwardEnable;
	bool m_bBackEnable;
	int m_index;
public:
	//CMtAndFctSheet* m_pMtAndFctSheet;
	//BOOL  m_bMtAndFctCreated;   //材料树窗口是否已经创建
	CMFCRibbonStatusBar		m_wndStatusBar;
	//CStatusBar m_wndStatusBar;
	CStringArray*			m_pStrArr;
	CMainFormView*			m_pMainView;  
public:
	int InitStatusBar();
	void SetForwardAndBackState();
	
	afx_msg void OnBTNBackoff();
	void UndoToItem(int& nItem);
	//afx_msg void OnBtnTest();
	afx_msg void OnUpdateButtonGoforward(CCmdUI *pCmdUI);

	afx_msg void OnUpdateButtonBackoff(CCmdUI *pCmdUI);
	afx_msg void OnButtonGoforward();
	BOOL AddToTail(int nItem);
	afx_msg void OnButtonMaterialandfactory();
	afx_msg void OnBtnSwitchToCustominfo();
	afx_msg void OnButtonStockInfo();
	afx_msg void OnButtonHistory();
	afx_msg void OnBtnServMg();
	afx_msg void OnBtnServReg();
	afx_msg void OnButtonDealList();
	afx_msg void OnBtnDealNew();
	afx_msg void OnButtonDealtrace();
	afx_msg void OnBtnPerformenceMg();
	afx_msg void OnButtonRelogin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void    UpdateBaseStatus();
public:
	afx_msg LRESULT OnUpdateOperationStatus(WPARAM wParam, LPARAM lParam);
	void UpdateOperationStatus(CString _operationStatus);
protected:
	//afx_msg LRESULT OnAfterLoginMainView(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnReqStartInitNoteMainView(WPARAM wParam, LPARAM lParam);
};


