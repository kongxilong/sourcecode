
#include "stdafx.h"
#include "UiFeatureBuilder.h"
#include "MainFrm.h"
#include "UiFeatureDefs.h"
#include "CreateNewSkinProject.h"
#include "..\..\Inc\ICommonFun.h"
#include "CA2W_W2A.h"
#include "UiFeatureBuilderDoc.h"
#include "UiFeatureBuilderView.h"
#include "PasswordVerDlg.h"
#include "PasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CMainFrame::InitUiFeatureKernel()
{
	CStringW strKernelPath = PathHelperW(_T(NAME_KERNEL_DLL));
	m_hKernelDll = ::LoadLibraryW(strKernelPath);
	if (m_hKernelDll == NULL)
	{
		AfxMessageBox(_T("找不到UiFeature内核DLL！"), MB_OK | MB_ICONERROR);
		return;
	}

	GETUIKERNELINTERFACE GetKernel = (GETUIKERNELINTERFACE)::GetProcAddress(m_hKernelDll, "GetUiKernelInterface");
	if (GetKernel == NULL)
	{
		AfxMessageBox(_T("找不到UiFeature内核DLL的【GetUiKernelInterface】函数！"), MB_OK | MB_ICONERROR);
		return;
	}

	m_pUiKernel = GetKernel();
	if (m_pUiKernel == NULL)
	{
		AfxMessageBox(_T("找不到UiFeature内核DLL的【IUiFeatureKernel】接口！"), MB_OK | MB_ICONERROR);
		return;
	}
	m_pUiKernel->BD_SetDesignMode(true);
	m_pSkinMgr = m_pUiKernel->GetSkinManager();

	m_wndWindowView.Init(m_pUiKernel, m_wndProperties.GetPropetryCtrl());
	m_wndProperties.Init(m_pUiKernel, m_wndWindowView.GetViewTreeCtrl());

	// 加载控件显示数据
	m_pRegControlMap = m_pUiKernel->BD_GetRegisterControl();
	// 显示控件
	m_wndControls.SetControlList(m_pRegControlMap);

	m_bInitOk = true;
	SetProjectInitState(false);
}

void CMainFrame::SetViewCursor(int nCursor)
{
	::ShowCursor(FALSE);
	m_nViewCursor = nCursor;
	::ShowCursor(TRUE);
	::PostMessage(this->m_hWnd, WM_SETCURSOR, NULL, NULL);
}

void CMainFrame::OnFileNew()
{
	m_strUserPassword = _T("");
	// 创建新工程
	CCreateNewSkinProject NewSkinDlg;
	if (NewSkinDlg.DoModal() != IDOK)
		return;

	// 取得新工程路径信息
	NewSkinDlg.GetNewProjectPath(m_strNewSkinDir, m_strNewSkinName);
	if (m_strNewSkinDir.GetLength() <= 0 || m_strNewSkinName.GetLength() <= 0)
		return;

	if (m_strNewSkinDir.GetAt(m_strNewSkinDir.GetLength() - 1) != '\\')
		m_strNewSkinDir += "\\";

//	CString strInfo(_T(""));
//	strInfo.Format(_T("创建皮肤工程文件【%s%s】成功！"), m_strNewSkinName, _T(NAME_SKIN_FILE_EX_NAME));

	m_strNewUfpPath.Format(_T("%s%s%s"), m_strNewSkinDir, m_strNewSkinName, _T(NAME_SKIN_PROJ_EX_NAME));

	// 保存老工程
	if (!SaveSkinProject(m_strCurSkinDir, m_strCurSkinName, true))
		return;

	// 关闭老工程
	if (!CloseSkinProject())
		return;

	// 设置皮肤包密码
	CPasswordDlg PassDlg;
	PassDlg.Init(m_strUserPassword);
	if (PassDlg.DoModal() == IDOK)
	{
		PassDlg.GetPassword(m_strUserPassword);
	}

	if (!OpenSkinProject(true, m_strNewSkinDir, m_strNewSkinName))
		return;

//	AfxMessageBox(strInfo, MB_OK | MB_ICONINFORMATION);
}

void CMainFrame::OnFileOpen()
{
	m_strUserPassword = _T("");
	if (m_pUiKernel == NULL || m_pUiKernel->GetSkinManager() == NULL)
		return;

	// 打开工程文件
	CFileDialog UfpFileSelDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING,
		_T("ufp Files (*.ufp)|*.ufp;)||"), NULL);
	UfpFileSelDlg.DoModal();

	m_strNewUfpPath = UfpFileSelDlg.GetPathName();
	m_strNewSkinName = UfpFileSelDlg.GetFileTitle();
	m_strNewSkinDir = m_strNewUfpPath.Left(m_strNewUfpPath.ReverseFind('\\') + 1);

	if (m_strNewUfpPath.GetLength() <= 0 || m_strNewSkinName.GetLength() <= 0 || m_strNewSkinDir.GetLength() <= 0)
		return;

	if (m_strNewSkinDir.CompareNoCase(m_strCurSkinDir) == 0 && m_strNewSkinName.CompareNoCase(m_strCurSkinName) == 0)
	{
		AfxMessageBox(_T("此工程已经被打开！"), MB_OK | MB_ICONERROR);
		return;
	}

	// 保存老工程
	if (!SaveSkinProject(m_strCurSkinDir, m_strCurSkinName, true))
		return;

	// 关闭老工程
	if (!CloseSkinProject())
		return;

	// 判断皮肤包的版本
	CString strUfd = _T("");
	strUfd.Format(_T("%s%s%s"), m_strNewSkinDir, m_strNewSkinName, _T(NAME_SKIN_FILE_EX_NAME));
	int nPasswordLen = 0;
	SKIN_VERSION skinVer = m_pUiKernel->GetSkinManager()->GetSkinFileVersion(strUfd, nPasswordLen);
	if (skinVer == SV_ERROR)
	{
		AfxMessageBox(_T("抱歉，居然无法识别这个皮肤包，打不开了！"), MB_OK | MB_ICONERROR);
		return;
	}
	else if (skinVer == SV_0)
	{
		AfxMessageBox(_T("抱歉，特殊版本的皮肤包是受版权保护的，不能打开！"), MB_OK | MB_ICONWARNING);
		return;
	}
	else if (skinVer == SV_1)
	{
		AfxMessageBox(_T("抱歉，请使用2.0.0.1030以前的版本打开！"), MB_OK | MB_ICONWARNING);
		return;
	}
	else if (skinVer == SV_2)
	{
		if (nPasswordLen > 0)
		{
			// 验证密码
			CPasswordVerDlg VerDlg;
			VerDlg.Init(m_pUiKernel->GetSkinManager(), strUfd);
			if (VerDlg.DoModal() != IDOK)
				return;

			VerDlg.GetPassword(m_strUserPassword);
		}
	}

	// 打开工程
	if (!OpenSkinProject(false, m_strNewSkinDir, m_strNewSkinName))
		return;

	CStringW strTitle(_T(""));
	strTitle.Format(_T("当前工程：%s"), m_strCurSkinName);
	// 改标题
	this->SetWindowText(strTitle);
	this->SendMessage(WM_NCPAINT, NULL, NULL);


//	CString strInfo(_T(""));
//	strInfo.Format(_T("打开皮肤工程文件【%s%s】成功！"), m_strCurSkinName, _T(NAME_SKIN_FILE_EX_NAME));
//	AfxMessageBox(strInfo, MB_OK | MB_ICONINFORMATION);
}

void CMainFrame::SetProjectInitState(bool bInitOk)
{
	m_bProjectInitState = bInitOk;
	m_wndWindowView.SetProjectInitState(bInitOk);
	m_wndProperties.SetProjectInitState(bInitOk);
	m_wndControls.SetProjectInitState(bInitOk);
	if (m_pView != NULL)
		m_pView->SetProjectInitState(bInitOk);
}

void CMainFrame::OnSetPassword()
{
	if (!m_bProjectInitState)
	{
		AfxMessageBox(_T("还没有皮肤工程呢！"), MB_OK | MB_ICONERROR);
		return;
	}
	// 设置皮肤包密码
	CPasswordDlg PassDlg;
	PassDlg.Init(m_strUserPassword);
	if (PassDlg.DoModal() == IDOK)
	{
		PassDlg.GetPassword(m_strUserPassword);
		SetPropetryChange();
	}
}

void CMainFrame::OnRunTest()
{
	USES_CONVERSION_F;
	if (m_pView == NULL)
		return;

	IWindowBase* pCurWnd = m_pView->GetCurrentWindow();
	if (pCurWnd == NULL)
		return;

	string strSkinPath = W2A_F(m_strCurSkinDir.LockBuffer());
	m_strCurSkinDir.UnlockBuffer();

	strSkinPath += W2A_F(m_strCurSkinName.LockBuffer());
	m_strCurSkinName.UnlockBuffer();

	strSkinPath += NAME_SKIN_FILE_EX_NAME;

	string strWndName = W2A_F((WCHAR*)(pCurWnd->GetObjectName()));

	string strCmd = "\"";
	strCmd += PathHelper(NAME_FEATURE_RUN_TEST);
	strCmd += "\" \"";
	strCmd += strSkinPath;
	strCmd += "\" \"";
	strCmd += strWndName;
	strCmd += "\"";

	::WinExec(strCmd.c_str(), SW_SHOW);
}

void CMainFrame::OnAppExit()
{
	// 保存老工程
	if (!SaveSkinProject(m_strCurSkinDir, m_strCurSkinName, false))
		return;

	// 关闭老工程
	if (!CloseSkinProject())
		return;
}

void CMainFrame::OnFileClose()
{
	// 保存老工程
	if (!SaveSkinProject(m_strCurSkinDir, m_strCurSkinName, true))
		return;

	// 关闭老工程
	if (!CloseSkinProject())
		return;

	// 改标题
	this->SetWindowText(_T("UiFeatureBuilder"));
	this->SendMessage(WM_NCPAINT, NULL, NULL);
}

void CMainFrame::OnFileSave()
{
	// 保存老工程
	if (!SaveSkinProject(m_strCurSkinDir, m_strCurSkinName, true))
		return;
}

// 创建/打开一个新的皮肤工程
bool CMainFrame::BD_OpenProject(LPCWSTR pszSkinDir, LPCWSTR pszSkinName)
{
	if (pszSkinDir == NULL || pszSkinName == NULL || m_pSkinMgr == NULL)
		return false;

	m_pSkinFileItem = NULL;
	m_pSkinMgr->ReleaseSkinManagerPropetry();

	// 导入zip文件
	CStringW strZipPath = pszSkinDir;
	if (pszSkinDir[wcslen(pszSkinDir) - 1] != '\\')
		strZipPath += L"\\";
	strZipPath += pszSkinName;
	strZipPath += _T(NAME_SKIN_FILE_EX_NAME);

	m_pSkinFileItem = m_pSkinMgr->GetSkinFileItem(strZipPath, true);
	return (m_pSkinFileItem != NULL);
}

// 创建一个新的皮肤工程
bool CMainFrame::BD_CreateEmptyProject(LPCWSTR pszSkinDir, LPCWSTR pszSkinName)
{
	if (pszSkinDir == NULL || pszSkinName == NULL || m_pSkinMgr == NULL)
		return false;

	m_pSkinFileItem = NULL;
	m_pSkinMgr->ReleaseSkinManagerPropetry();

	// 导入zip文件
	CStringW strZipPath = pszSkinDir;
	if (pszSkinDir[wcslen(pszSkinDir) - 1] != '\\')
		strZipPath += L"\\";
	strZipPath += pszSkinName;
	strZipPath += _T(NAME_SKIN_FILE_EX_NAME);

	m_pSkinFileItem = m_pSkinMgr->CreateEmptySkinFileItem(strZipPath);
	return (m_pSkinFileItem != NULL);
}

// 打开一个工程
bool CMainFrame::OpenSkinProject(bool bIsNew, CString strSkinDir, CString strSkinName)
{
	SetProjectInitState(true);
	if (m_pUiKernel == NULL || m_pSkinMgr == NULL)
		return false;

	if (!bIsNew)
	{
		if (!BD_OpenProject(m_strNewSkinDir, m_strNewSkinName))
		{

			CString strInfo(_T(""));
			strInfo.Format(_T("初始化皮肤文件【%s%s】错误！"), m_strNewSkinName, _T(NAME_SKIN_FILE_EX_NAME));
			AfxMessageBox(strInfo, MB_OK | MB_ICONERROR);
			return false;
		}

		SetProjectInitState(true);
		m_pUiKernel->BD_SetBuilderReadingSkin(true);
		m_wndWindowView.InitShowNewProject();
		m_pUiKernel->BD_SetBuilderReadingSkin(false);
	}
	else
	{
		if (!BD_CreateEmptyProject(m_strNewSkinDir, m_strNewSkinName))
		{
			CStringW strInfo(_T(""));
			strInfo.Format(_T("创建空皮肤文件【%s%s】错误！"), m_strNewSkinName, _T(NAME_SKIN_FILE_EX_NAME));
			AfxMessageBox(strInfo, MB_OK | MB_ICONERROR);
			return false;
		}

		SetProjectInitState(true);
	}
	m_pSkinMgr->InitCursorBasePropetry(this->GetSkinFileItem());

	m_strCurUfpPath = m_strNewUfpPath;
	m_strCurSkinName = m_strNewSkinName;
	m_strCurSkinDir = m_strNewSkinDir;

	m_strNewUfpPath = _T("");
	m_strNewSkinName = _T("");
	m_strNewSkinDir = _T("");

	return true;
}

// 保存
bool CMainFrame::SaveSkinProject(CString strSkinDir, CString strSkinName, bool bNeedErroInfo)
{
	if (strSkinDir.GetLength() <= 0 || strSkinName.GetLength() <= 0)
		return true;

	if (m_pSkinMgr == NULL || m_wndWindowView.GetViewTreeCtrl() == NULL)
		return false;

	// 设置可以保存的有效属性
	m_wndWindowView.GetViewTreeCtrl()->SetSaveWindowActivePropetry();

	USES_CONVERSION_F;
	bool bOk = m_pSkinMgr->BD_SaveSkinFile(m_pSkinFileItem, strSkinDir, strSkinName, W2A_F(m_strUserPassword.LockBuffer()));
	m_strUserPassword.UnlockBuffer();

	if (bNeedErroInfo && !bOk)
		AfxMessageBox(_T("保存皮肤工程失败！"), MB_OK | MB_ICONERROR);

	m_bNeedSave = !bOk;
	return bOk;
}

// 保存
bool CMainFrame::CloseSkinProject()
{
	m_strUserPassword = _T("");
	SetProjectInitState(false);

	if (m_pUiKernel == NULL)
		return false;

	m_pUiKernel->BD_CloseProject();

	m_strCurUfpPath = _T("");
	m_strCurSkinName = _T("");
	m_strCurSkinDir = _T("");
	return true;
}

SKIN_FILE_ITEM* CMainFrame::GetSkinFileItem()
{
	return m_pSkinFileItem;
}
