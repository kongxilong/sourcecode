
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "WindowsView.h"
#include "PropertiesWnd.h"
#include "UiFeatureBuilderDoc.h"
#include "UiFeatureBuilderView.h"
#include "ControlsWnd.h"
#include "IUiFeatureKernel.h"
#include "LocalImageList.h"
#include "..\..\Inc\IZipFile.h"

// 双击打开消息
#define OPEN_SKIN_BY_DB_CLICK				(WM_APP + 0x0100)

class CMainFrame : public CFrameWndEx
{
// 实现
public:
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	// 设置属性发生变化，需要保存
	void SetPropetryChange();
	SKIN_FILE_ITEM* GetSkinFileItem();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	LRESULT OnOpenSkinByDbClick(WPARAM wParam, LPARAM lParam);

	// 自动关联并可以打开皮肤包文件
	void AutoOpenSkin();

private:
	HICON m_hClassViewIcon;
	HICON m_hPropertiesBarIcon;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	void SetViewCursor(int nCursor);
	void SetView(CUiFeatureBuilderView *pView);
	CUiFeatureBuilderView* GetView() { return m_pView; }
	IPropertySkinManager* GetPropertySkinMgr() { return m_pSkinMgr; }

	
protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCToolBar       m_wndTestRunToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
//	CFileView         m_wndFileView;
	CWindowsView	  m_wndWindowView;
	CPropertiesWnd    m_wndProperties;
	CControlsWnd      m_wndControls;
	CUiFeatureBuilderView *m_pView;

	bool m_bNeedSave;

private:
//////////////////////////////////////////////////////////////////////////
	// Kernel 相关
	HMODULE m_hKernelDll;
	IUiFeatureKernel* m_pUiKernel;
	IPropertySkinManager* m_pSkinMgr;
	CONTROL_REG_TLIST* m_pRegControlMap;

//////////////////////////////////////////////////////////////////////////
	// 控件相关
	HMODULE m_hControlDll;

//////////////////////////////////////////////////////////////////////////
	bool m_bInitOk;
	// 光标
	int m_nViewCursor;

	// 当前项目工程文件
	CString m_strCurUfpPath;
	CString m_strCurSkinName;
	CString m_strCurSkinDir;

	CString m_strNewUfpPath;
	CString m_strNewSkinName;
	CString m_strNewSkinDir;
	CString m_strUserPassword;
	bool m_bProjectInitState;

	// 一个皮肤包
	SKIN_FILE_ITEM* m_pSkinFileItem;

private:
	void InitUiFeatureKernel();
	// 设置所有显示的控件的初始化状态
	void SetProjectInitState(bool bInitOk);

	// 打开一个工程
	bool OpenSkinProject(bool bIsNew, CString strSkinDir, CString strSkinName);
	// 保存
	bool SaveSkinProject(CString strSkinDir, CString strSkinName, bool bNeedErroInfo);
	// 保存
	bool CloseSkinProject();

	// 打开一个新的皮肤工程
	bool BD_OpenProject(LPCWSTR pszSkinDir, LPCWSTR pszSkinName);
	// 创建/打开一个新的皮肤工程
	bool BD_CreateEmptyProject(LPCWSTR pszSkinDir, LPCWSTR pszSkinName);

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileClose();
	afx_msg void OnAppExit();
	afx_msg void OnRunTest();
	afx_msg void OnSetPassword();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	afx_msg void OnUpdateRunTest(CCmdUI *pCmdUI);
};
