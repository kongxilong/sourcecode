// MainFormView.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "MainFormView.h"
#include "NotePublic.h"

// CMainFormView

IMPLEMENT_DYNCREATE(CMainFormView, CFormView)

CMainFormView::CMainFormView()
	: CFormView(CMainFormView::IDD)
{
	m_time_take = 0;
	m_nNoteToLook = 0;
}

CMainFormView::~CMainFormView()
{
}

void CMainFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NOTIFY, m_list);
	//  DDX_Control(pDX, IDC_RICHEDIT2_NOTIFY, m_richEdit);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
}

BEGIN_MESSAGE_MAP(CMainFormView, CFormView)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
ON_BN_CLICKED(IDC_BTN_PUBLIC, &CMainFormView::OnBnClickedBtnPublic)
ON_WM_TIMER()
ON_MESSAGE(WM_SHOWNOTEINEDIT_MainView, &CMainFormView::OnShowNoteInEdit)
ON_MESSAGE(WM_REQINITNOTEEND_MainView, &CMainFormView::OnReqInitNoteEnd)

ON_BN_CLICKED(IDC_BTN_DEL, &CMainFormView::OnBnClickedBtnDel)
ON_BN_CLICKED(IDC_BTN_REFRESH, &CMainFormView::OnBnClickedBtnRefresh)
ON_MESSAGE(WM_REQSTARTINITNOTE_MainView, &CMainFormView::OnReqStartInitNote)
ON_BN_CLICKED(IDC_BTN_FIND_MORE, &CMainFormView::OnBnClickedBtnFindMore)
ON_MESSAGE(WM_DELSELNOTE_MainView, &CMainFormView::OnDelSelNote)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_NOTIFY, &CMainFormView::OnNMDblclkListNotify)
END_MESSAGE_MAP()


// CMainFormView 诊断

#ifdef _DEBUG
void CMainFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


HBRUSH CMainFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(RGB(185,211,255));	
		return b_static;	
	}
	if(nCtlColor == CTLCOLOR_DLG)   //更改对话框背景色 
	{ 
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(RGB(185,211,255)); 
		HBRUSH b=CreateSolidBrush(RGB(185,211,255)); 
		return b; 
	} 
	return hbr;
}


void CMainFormView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	OnPrepareDC(&dc);
	OnDraw(&dc);
}

void CMainFormView::OnDraw(CDC* pDc)
{
	CPen pen(PS_SOLID,2,RGB(238,138,0));
	pDc->SelectObject(&pen);
	CFont font;
	font.CreatePointFont(450,_T("华文行楷"),NULL);
	CFont *pOldFont = pDc->SelectObject(&font); 
	pDc->SetBkMode(TRANSPARENT); //设置背景透明
	pDc->TextOut(m_point.x,m_point.y, _T("公告栏"));
	pDc->SelectObject(pOldFont);
	CFormView::OnDraw(pDc);
}


void CMainFormView::OnInitialUpdate() {
	CFormView::OnInitialUpdate();
	//AfxInitRichEdit();
	InitRichEdit();
	InitList();
	WriteFont();

	CFont font;
	LOGFONT m_tempfont={25,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_status.SetFont(&font);
	((CStatic*)GetDlgItem(IDC_STATIC_TIP))->SetFont(&font);
	font.Detach();

	SetTimer(1,TIMEHEARTPACKAGE,NULL);
	//SetTimer(2,20*1100,NULL);  //设置20秒超时时间
	//SetTimer(2,1000,NULL);   //设置1秒刷新时间
	SetTimer(2,500,NULL);   //用于当连接上服务器后就开始刷新通知
}
// CMainFormView 消息处理程序

void CMainFormView::InitRichEdit()
{
	m_richEdit.SubclassDlgItem(IDC_RICHEDIT2_NOTIFY,this);
	m_richEdit.SetBackgroundColor(0,RGB(223, 223, 223));
	//m_richEdit.SetReadOnly();
	/*
	//设置行间距
	PARAFORMAT2 pf2;
	//ZeroMemory(&pf2,sizeof(PARAFORMAT2)); 
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_LINESPACING | PFM_SPACEAFTER;
	pf2.dyLineSpacing = 200;
	pf2.bLineSpacingRule = 0.8;
	m_richEdit.SetParaFormat(pf2);
	*/
	//另一种设置字体的方式

	m_editFont.CreatePointFont(200,_T("宋体"));
	m_richEdit.SetFont(&m_editFont);
	
	/*
	//设置字体
	CHARFORMAT2 cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD|CFM_COLOR|CFM_FACE|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE;
	cf.dwEffects = 0;
	cf.yHeight = 18*18;   //文字[wen zi]高度
	cf.crTextColor = RGB(200,100,255);//文字[wen zi]颜色
	
	USES_CONVERSION;
	//char* p = "华文行楷";
	char *p = "华文行楷";
	//CLSID clsid;
	long len = MultiByteToWideChar(CP_ACP,0,p,-1,cf.szFaceName,32);
	cf.szFaceName[len] = 0;    //设置[she zhi]字体[zi ti]
	m_richEdit.SetSel(0, -1);      //设置[she zhi]处理区域[qu yu]
	m_richEdit.SetSelectionCharFormat(cf);
	//CString _str = _T("华文行楷华文行楷华文行楷华文行楷");
	//m_richEdit.SetWindowText(_str);
	*/
}



void CMainFormView::InitList()
{
	m_list.SetBkColor(RGB(223, 223, 223));
	m_list.SetExtendedStyle(m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	CRect rect;
	m_list.GetClientRect(rect);

	m_list.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 75);    
    m_list.InsertColumn(1, _T("最近公告"), LVCFMT_LEFT, 200);
	m_list.InsertColumn(2, _T("发布时间"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(3, _T("发布人"), LVCFMT_LEFT, rect.Width()-375);   
}

void CMainFormView::WriteFont()
{
	CClientDC dc(this);
	CPen pen(PS_SOLID,2,RGB(238,138,0));
	dc.SelectObject(&pen);
	CFont font;
	font.CreatePointFont(400,_T("华文行楷"),NULL);
	CFont *pOldFont = dc.SelectObject(&font); 
	
	//透明画刷	
	CRect clientRect;	
	this->GetClientRect(clientRect);

	dc.SetBkMode(TRANSPARENT); //设置背景透明
	int x = clientRect.Width()/2 - 40;
	m_point.x = x;
	m_point.y = 4; 
	dc.TextOut(m_point.x,m_point.y, _T("公告栏"));
	OnPrepareDC(&dc);
	dc.DPtoLP(&m_point);
	dc.SelectObject(pOldFont);
}
void CMainFormView::OnBnClickedBtnPublic()
{
	// TODO: 在此添加控件通知处理程序代码	
	CNotePublic noteDlg(this);
	//noteDlg.SetBackgroundColor(RGB(185,211,255)); 
	//CString str = _T("hehe");
	//CString str2 = _T("哈哈");
	//CString str3 = NULL;
	//str3.Format(_T("第一个包含%d个字符,第二个是%d个字符"),str.GetLength(),str2.GetLength());
	//MessageBox(str3);
	if(!CSock::m_nameOnline.IsEmpty())
	{
		noteDlg.m_personPublicName = CSock::m_nameOnline;
	}
	noteDlg.DoModal();
}


void CMainFormView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			CSock::StartSendHeartPackage();
			break;
		}
	case 2:
		{
			if(CSock::m_bIsConnecting && !(CSock::m_nameOnline.IsEmpty())){  //如果连接上服务器后就开始请求
				m_nNoteToLook = 9;   //查找最新的9条信息
				LoadNote();
				KillTimer(2);
			}
			break;
		}
	case 3:
		{
			ShowTimeTakeStatus();
			break;
		}
	default:
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CMainFormView::DeleteSelNote()
{
	CString _strToDelete = m_noteToDelete;
	CString _strToFind = _T(",");    //找到这样的标志位 进行字符段的截取
	while(TRUE){
		int _strEnd = _strToDelete.Find(_strToFind);
		if(-1 == _strEnd)   //没有找到
		{
			break;
		}else{
			CString temp = _strToDelete.Mid(0,_strEnd);
			int num =m_list.GetItemCount();
			for(int i = 0;i< num;i++){
				if(temp == m_list.GetItemText(i,2))
				{
					m_list.DeleteItem(i);
				}
			}
			_strToDelete = _strToDelete.Mid(_strEnd+1);
		}
	}
	int count = m_list.GetItemCount();
	for(int j=0;j<count;j++)
	{
		CString index;
		index.Format(_T("%d"),(j+1));
		m_list.SetItemText(j,0,index);
	}
}

void CMainFormView::ShowTimeTakeStatus()
{
	m_time_take++;
	CString time;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_status.SetWindowText(time);
	if(m_time_take == 60)  //60秒后请求就超时了
	{
		m_time_take = 0;
		m_status.SetWindowText(_T("请求超时"));
		KillTimer(3);
		CSock::m_pMainFormView = NULL;  //清除指针
	}
}

afx_msg LRESULT CMainFormView::OnShowNoteInEdit(WPARAM wParam, LPARAM lParam)
{
	WT;
	CString strTemp = NULL;
	if(m_title_note.GetLength()<4){
		for(int i=0;i<19;i++){
			strTemp+=" ";
		}
	}else if(m_title_note.GetLength()<8){
		for(int i=0;i<17;i++){
			strTemp+=" ";
		}
	}else if(m_title_note.GetLength()<12){
		for(int i=0;i<15;i++){
			strTemp+=" ";
		}
	}else if(m_title_note.GetLength()<16){
		for(int i=0;i<13;i++){
			strTemp+=" ";
		}
	}else if(m_title_note.GetLength()<20){
		for(int i=0;i<10;i++){
			strTemp+=" ";
		}
	}else if(m_title_note.GetLength()<24){
		for(int i=0;i<6;i++){
			strTemp+=" ";
		}
	}else if(m_title_note.GetLength()<28){
		for(int i=0;i<4;i++){
			strTemp+=" ";
		}
	}else if(m_title_note.GetLength()<32){
		for(int i=0;i<2;i++){
			strTemp+=" ";
		}
	}else{
	}

	strTemp = strTemp + m_title_note + _T("\n");
	for(int i=0;i<17;i++){
		strTemp += " ";
	}
	strTemp = strTemp + _T("发布者：")+ m_persion_public_note + _T("\n");
	for(int i=0;i<15;i++){
		strTemp += " ";
	}
	strTemp = strTemp + _T("发布时间：")+ m_time_note_public + _T("\n");
	strTemp += "    ";
	strTemp += m_content_note;
	this->m_richEdit.SetWindowText(strTemp);
	return 0;
}


void CMainFormView::LoadNote()
{
	void* dlg = this;
	CSock::StartReqLoadNote(dlg);
}


void CMainFormView::StartInitNote()
{	
	SetTimer(3,1000,NULL);
	m_status.SetWindowText(_T(""));
	m_time_take = 0;
}

void CMainFormView::ReqInitNoteEnd()
{
	KillTimer(3);
	m_status.SetWindowText(_T(""));
	m_time_take = 0;
}


void CMainFormView::OnBnClickedBtnDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = 0;
	CString num;
	CString  lineToDelete = _T("将要被删除的行编号是：");
	m_noteToDelete.Empty();
	int total = m_list.GetItemCount();
	for(int i=0; i< total; i++)
	{
		if(m_list.GetCheck(i))
		{	
			count++;
			lineToDelete += m_list.GetItemText(i,0);
			lineToDelete += ",";
			m_noteToDelete += m_list.GetItemText(i,2);
			m_noteToDelete += ",";   //两个时间段的分隔符
		}
	}
	if(count == 0)
	{
		MessageBox(_T("请先至少勾选一项通知，然后再删除"));
		return;
	}else{   //有选中项
		lineToDelete += _T("确认删除吗？");
		if(IDYES == MessageBox(lineToDelete,_T("警告"),MB_YESNO))
		{
			CSock::StartDelNoteMainView(this);
		}else{
			return;
		}
	}
}


void CMainFormView::OnBnClickedBtnRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nNoteToLook = 9;   //查找最新的9条信息
	LoadNote();
}


afx_msg LRESULT CMainFormView::OnReqStartInitNote(WPARAM wParam, LPARAM lParam)
{
	StartInitNote();
	return 0;
}
afx_msg LRESULT CMainFormView::OnReqInitNoteEnd(WPARAM wParam, LPARAM lParam)
{
	ReqInitNoteEnd();
	return 0;
}

void CMainFormView::OnBnClickedBtnFindMore()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nNoteToLook = -1;   //代表查看所有的通知
	LoadNote();
}


afx_msg LRESULT CMainFormView::OnDelSelNote(WPARAM wParam, LPARAM lParam)
{
	DeleteSelNote();
	return 0;
}


void CMainFormView::OnNMDblclkListNotify(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIndex = m_list.GetNextItem( -1, LVNI_SELECTED );
	if(nIndex == -1)
	{
		MessageBox(_T("当前没有选中行"));
		return;
	}
	CString time = m_list.GetItemText(nIndex,2);
	CString person = m_list.GetItemText(nIndex,3);
	if(time.IsEmpty())
	{
		MessageBox(_T("当前选中行没有通知"));
		return;
	}
	m_note_to_find = time + _T(",")+person;
	CSock::StartLookNoteDetail(this);
}
