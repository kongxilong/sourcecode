
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "sqlitedemo.h"
#include "ChildView.h"
#include "service/ISqliteService.h"
#include "core\SingletonManagerCenter.h"
#include "core\SingletonIDDefine.h"
#include "DBOperatDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_BTN_NEW_DB,&CChildView::OnBtnNewDBClicked)
	ON_BN_CLICKED(ID_BTN_OPERATION,&CChildView::OnBtnOperatDBClicked)
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
    
	CreanteControls();
	
}

void  CChildView::CreanteControls()
{
	CreateButtons();
	InitTreeDB();
}

void  CChildView::InitTreeDB()
{
	DWORD style = TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT;
	m_treeDB.Create(style,CRect(160,80,500,600),this,ID_TREE_DB);
	m_treeDB.InsertItem(_T("数据库树"),TVI_ROOT,TVI_LAST);
    m_treeDB.ShowWindow(SW_SHOWNORMAL);
	m_curTreeItem = m_treeDB.GetRootItem();
}

void  CChildView::CreateButtons()
{
	//m_BtnCreateDB.Create(_T("新建数据库"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_ICON,
	//	CRect(40,80,120,120), this, ID_BTN_NEW_DB);
	////m_MyButton.SetIcon(AfxGetApp()->LoadIcon(ICON_SAVE)); //给按钮加载一个图标
	//m_BtnCreateDB.ShowWindow(SW_SHOWNORMAL);
    
	m_BtnOpeartDB.Create(_T("操作数据库"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_ICON,
		CRect(40,80,120,120), this, ID_BTN_OPERATION);
	//m_MyButton.SetIcon(AfxGetApp()->LoadIcon(ICON_SAVE)); //给按钮加载一个图标
	m_BtnOpeartDB.ShowWindow(SW_SHOWNORMAL);

}

void  CChildView::OnBtnOperatDBClicked()
{
	DB_Operation org = m_dbOperation;
    CDBOperatDlg  dlg(this);
	dlg.DoModal();
	if(org == m_dbOperation)
	{
		return;
	}
	HandleDBOperation();
}

void  CChildView::OnBtnNewDBClicked()
{
	ISqliteService* pSS = (ISqliteService*)SingletonManagerCenter::GetInstance()->GetSingletonByID(STID_SQLITESV);
	if(pSS)
	{
		if(pSS->CreanteDB())
		{
			MessageBox(_T("新建数据库成功"));
		}else
		{
			MessageBox(_T("新建一个数据库失败"));
		}
	}
	
}

void CChildView::HandleDBOperation()
{
    DB_Operation op = m_dbOperation;
	switch(m_dbOperation.commandID)
	{
	case ENUM_COMMAND_NEW_COMPANY:
		{
			HandleNewCompany();
			break;
		}
	case ENUM_COMMAND_DELETE_COMPANY:
		{
			HandleDeleteCompany();
			break;
		}
	case ENUM_COMMAND_NEW_DEPARTMENT:
		{
			HandleNewDepartment();
			break;
		}
	case ENUM_COMMAND_DELETE_DEPARTMENT:
		{
			HandleDeleteDepartment();
			break;
		}
	case ENUM_COMMAND_NEW_EMPLOY:
		{
			HandleNewEmploy();
			break;
		}
	case ENUM_COMMAND_DELETE_EMPLOY:
		{
			HandleDeleteEmploy();
			break;
		}
	default:
		break;
	}
}
