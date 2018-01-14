// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "Login.h"
#include "afxdialogex.h"
#include "PwdEdit.h"
#include "DBConf.h"
#include "MainFrm.h"
#include "MainFormView.h"
//#include "UserReg.h"

// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)



CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
	, m_password(_T(""))
{
	//m_bLoginSuccess = CSock::m_bOnline;
	m_bWorking = FALSE;
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SPLIT_ADDR_ADD, m_split);
	DDX_Control(pDX, IDC_COMBO_LOGIN_ADDR, m_addr);
	DDX_Control(pDX, IDC_COMBO_LOGIN_POS, m_pos);
	DDX_Control(pDX, IDC_COMBO_LOGIN_USERNAME, m_username);
	DDX_Control(pDX, IDC_DATETIME_DEAL_NEW, m_dateDealNew);
	DDX_Text(pDX, IDC_EDIT_LOGIN_PSW, m_password);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
	DDX_Control(pDX, IDC_CHECK_REM_PSW, m_remPsw);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PWDEDIT, &CLogin::OnBnClickedButtonPwdedit)
	ON_MESSAGE(WM_CURSORTONORMAL,&CLogin::OnCursorToNormal)
	ON_MESSAGE(WM_LOGINFAIL,&CLogin::OnLoginFail)
	//ON_BN_CLICKED(IDC_SPLIT_ADDR_ADD, &CLogin::OnBnClickedSplitAddrAdd)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CLogin::OnBnClickedBtnLogin)
	ON_MESSAGE(WM_LOGINSUCCESS, &CLogin::OnLoginSuccess)
	ON_BN_CLICKED(IDC_BTN_GO_BACK, &CLogin::OnBnClickedBtnGoBack)
	ON_CBN_SELCHANGE(IDC_COMBO_LOGIN_USERNAME, &CLogin::OnCbnSelchangeComboLoginUsername)
	ON_CBN_EDITCHANGE(IDC_COMBO_LOGIN_USERNAME, &CLogin::OnCbnEditchangeComboLoginUsername)
	ON_WM_CTLCOLOR()
//	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLogin 消息处理程序



void CLogin::OnBnClickedButtonPwdedit()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!(CSock::m_bIsConnecting && !CSock::m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("请先登录后再修改密码！"));
		return ;
	}
	CDialogEx::OnOK();
	CPwdEdit pwdEditDlg;
	pwdEditDlg.DoModal();

}
/*
void CLogin::OnBnClickedSplitAddrAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}*/
BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON_BMMAIN);
	
	this->SetIcon(hIcon,TRUE);
	this->SetIcon(hIcon,FALSE);

	CFont font;
	LOGFONT m_tempfont={20,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_status.SetFont(&font);
	font.Detach();
	m_pos.AddString(_T("系统管理员"));
	m_pos.AddString(_T("总经理"));
	m_pos.AddString(_T("客户经理"));
	m_pos.AddString(_T("设计师"));
	m_pos.AddString(_T("普通员工"));
	m_pos.SetCurSel(0);
	LoadUser();
	if(CSock::m_bOnline != FALSE)   //如果在线
	{
		m_status.SetWindowText(_T("在线"));
		(CButton *)GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
		m_username.SetWindowText(CSock::m_nameOnline);
	}
	//InitSplit();

	m_addr.AddString(_T("江阴延陵路店"));
	m_addr.AddString(_T("江阴长寿镇厂"));
	m_addr.SetCurSel(0);
	return TRUE;
}
/*
void CLogin::InitSplit()
{
	m_split.SetDropDownMenu(IDR_MENU_LOGIN_ADDR_OPERATION,0);
}
*/
void CLogin::OnBnClickedBtnLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strName;
	m_username.GetWindowText(strName);
	if(strName.IsEmpty()){
		AfxMessageBox(_T("用户名不能为空！"));
		return;
	}else if(strName.GetLength()>15)
	{
		AfxMessageBox(_T("用户名不能超过15个字符！"));
		return;
	}
	if(m_password.IsEmpty()){
		AfxMessageBox(_T("密码不能为空！"));
		return;
	}else if(m_password.GetLength()<3)
	{
		AfxMessageBox(_T("密码不能少于3个字符！"));
		return;
	}else if(m_password.GetLength()>13)
	{
		AfxMessageBox(_T("密码不能大于13个字符！"));
		return;
	}
	if( !((-1 == strName.Find('#'))&&(-1 == strName.Find(' '))&&(-1 == strName.Find('$'))))
	{
		AfxMessageBox(_T("用户名中不能包含#、$以及空格等特殊字符！"));
		return;
	}
	if( !((-1 == m_password.Find('#')) && (-1 == m_password.Find(' '))&&(-1 == m_password.Find(','))))
	{
		AfxMessageBox(_T("密码中不能包含'#'、'$'、','已经空格等特殊字符！"));
		return;
	}
	if(BST_CHECKED == m_remPsw.GetCheck()){     //将记住密码的checkbox框的状态写入配置文件
		WritePrivateProfileString(_T("remember password"),_T("value"),_T("TRUE"),_T(".//BMClient.ini"));
	}else{
		WritePrivateProfileString(_T("remember password"),_T("value"),_T("FALSE"),_T(".//BMClient.ini"));
	}

	WriteUserToIni();  // 将用户名和密码写进配置文件中
	USES_CONVERSION;
	CString temp;
	m_username.GetWindowText(temp);

	strcpy_s(m_loginInfo_Stru.username,T2A(temp));
	
	strcpy_s(m_loginInfo_Stru.password,T2A(m_password));
	strcpy_s(m_loginInfo_Stru.DBName,T2A(CSock::m_DB));

	CString loginAddr;
	m_addr.GetWindowText(loginAddr);
	strcpy_s(m_loginInfo_Stru.loginAddr,T2A(loginAddr));

	CString loginPos;
	m_pos.GetWindowText(loginPos);
	strcpy_s(m_loginInfo_Stru.loginPos,T2A(loginPos));


	m_status.SetWindowText(_T("正在开始登录"));  //修改登录状态
	
	SetCloseButton(TRUE);
	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_WAIT));  //圆圈 表示等待
	(CButton *)GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
	SetTimer(1,20*1000,NULL);  //设置登录超时时间
	VOID* dlg = this;
	CSock::StartLogin(dlg);
}

void CLogin::GoToMainApp()			//登录成功后 进入主程序界面
{
	WT;
	KillTimer(1);
	SetCloseButton(FALSE);
	CSock::m_bOnline = TRUE;
	//CString username;
	//m_username.GetWindowText(username); //保存在线的用户名
	CSock::m_nameOnline = A2T(m_loginInfo_Stru.username); //保存在线的用户名
	CSock::m_user_pos = A2T(m_loginInfo_Stru.loginPos);   //用户登录信息
	(CButton *)GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
	CDialogEx::OnOK();
	CSock::m_pLogin = NULL;
	CBMApp *app = (CBMApp *)AfxGetApp();
	if(INVALID_SOCKET == app->m_sHost)  //如果是运行中间与服务器断开连接后重新登录的
	{
		app->m_sHost = CSock::m_sockWhole;		
	}
}

afx_msg	LRESULT CLogin::OnLoginFail(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	SetCloseButton(FALSE);
	CSock::m_bOnline = FALSE;
	(CButton *)GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);  //重新登录用
	return 0;
}

afx_msg LRESULT CLogin::OnLoginSuccess(WPARAM wParam, LPARAM lParam)
{
	GoToMainApp();
	return 0;
}

afx_msg LRESULT CLogin::OnCursorToNormal(WPARAM wParam, LPARAM lParam)
{
	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_ARROW));
	return 0;
}



void CLogin::OnBnClickedBtnGoBack()
{	
	
	CDialogEx::OnOK();
	CDBConf dbDlg;
	dbDlg.DoModal();	
}


void CLogin:: WriteToText(CString& strToWtrite,CString& writeToPath)     //将str 写到path路径的文件中 覆盖原有的
{
	CStdioFile mFile;
	CFileException mExcept;
	if(mFile.Open(writeToPath, CFile::typeText|CFile::modeCreate|CFile::modeWrite, &mExcept))
	{
		mFile.WriteString(strToWtrite);
	}else{
		TRACE("Can't open file %s,error=%u\n",writeToPath,mExcept.m_cause);
	}
	mFile.Close();
	return ;
}


BOOL CLogin::ReadFromTxt(CString& path,CString& result){
	CStdioFile mFile;
	CFileException mExcept;
	if(mFile.Open(path, CFile::typeText|CFile::modeRead, &mExcept))
	{
		mFile.ReadString(result);
	}else{
		TRACE("Can't open file %s,error=%u\n",path,mExcept.m_cause);
		return FALSE;
	}
	mFile.Close();
	return TRUE;
}

void CLogin::WriteCombToFile(void* comb,CString path)             //将combox 中的内容 写到path文件中
{
	CString temp = NULL;
	CString readyToWrite = NULL;
	CComboBox * pComb = (CComboBox *)comb;
	pComb->GetWindowText(temp);
	int index = pComb->FindStringExact(0,temp);
	if(CB_ERR == index)     //没有找到现有项  表明新增了
	{
		readyToWrite = _T("#");  //以#隔开要写的端口 #中间的字符为要写进去的内容
		readyToWrite = readyToWrite + temp;
		int count = pComb->GetCount();
		if(count < 1){   // 如果m_port中原来没有内容 
			WriteToText(readyToWrite,path);
			return;
		}else{
			for(int i = 0;i < count;i++){
				CString str = NULL;
				pComb->GetLBText(i,str);
				readyToWrite =  readyToWrite + _T("#");
				readyToWrite = readyToWrite + str;
			}
			WriteToText(readyToWrite,path);
			return;
		}
	}else
	{
		readyToWrite = _T("#");
		readyToWrite = readyToWrite + temp;
		int count = pComb->GetCount();
		for(int i = 0;i < count;i++)
		{
			if(index == i)
			{
				continue;
			}
			CString str = NULL;
			pComb->GetLBText(i,str);
			readyToWrite = readyToWrite + _T("#");
			readyToWrite = readyToWrite + str;
		}
			WriteToText(readyToWrite,path);
			return;
	}
}


BOOL CLogin::LoadComb(void* ptr,CString& path)		//从文本文件中加载combox 的内容
{
	CComboBox * pComb = (CComboBox *)ptr;
	//CString path = _T("MyPort.txt");
	CString data = NULL;
	BOOL flag = ReadFromTxt(path,data);
	if(FALSE == flag){
		return FALSE;
	}else
	{
		int len = data.GetLength();
		if(len < 1)
		{
			return FALSE;    //文件为空
		}
		int readed = 0;
		USES_CONVERSION;
		char *p = T2A(data);
		CString temp = NULL;
		while(readed < len)
		{
			if('#' == *p)
			{
				if(!temp.IsEmpty())//表明不是第一次遇到#  将temp加入到combox 中
				{
					pComb->AddString(temp);
				}
				temp.Empty();
				p++;
				readed++;        //已经读一个字符了
				continue;
			}else
			{
				temp = temp + (*p);
				p++;
				readed++;        //已经读一个字符了
			}
			if(readed == len)    //已经读到文件的末尾了
			{
				pComb->AddString(temp);
				break;
			}
		}
		pComb->SetCurSel(0);         //结束
	}
	return TRUE;
}


BOOL CLogin::WriteUserToIni()      //将用户信息写进配置文件
{
	CString strUser = NULL;
	m_username.GetWindowText(strUser);
	CString strUserPos = NULL;
	m_pos.GetWindowText(strUserPos);
	WritePrivateProfileString(_T("登录角色"),_T("默认角色"),strUserPos,_T(".//BMClient.ini")); 
	CString total ;
	GetPrivateProfileString(_T("USERINFO"),_T("Total"),_T("#"),total.GetBuffer(100),100,_T(".//BMClient.ini"));
	//首先判断是否有保存的文件
	if(_T("#") == total)    //代表没有文件写入
	{
		WritePrivateProfileString(_T("USERINFO"),_T("Total"),_T("1"),_T(".//BMClient.ini")); 
		if(BST_CHECKED == m_remPsw.GetCheck())   //如果记住密码被选中
		{
			WritePrivateProfileString(_T("USERINFO"),_T("user0"),strUser,_T(".//BMClient.ini")); 
			CString result;      //将已经加密过的密码取出 写进ini文件中
			EncryptCode(result);

			WritePrivateProfileString(_T("USERINFO"),_T("psw0"),result,_T(".//BMClient.ini")); 
			//将默认用户名写进配置文件中
			WritePrivateProfileString(_T("USERINFO"),_T("defaultUser"),strUser,_T(".//BMClient.ini"));
		}else{
			WritePrivateProfileString(_T("USERINFO"),_T("user0"),strUser,_T(".//BMClient.ini")); 
			//将默认用户名写进配置文件中
			WritePrivateProfileString(_T("USERINFO"),_T("defaultUser"),strUser,_T(".//BMClient.ini"));
		}
	}else{

		USES_CONVERSION;
		char *p = T2A(total);
		int count = atoi(p);   //得到用户总数 为做循环用
		if(BST_CHECKED == m_remPsw.GetCheck())   //如果记住密码
		{
			CString result;
			EncryptCode(result);
			int i;
			for(i = 0;i < count; i++)
			{
				CString strTemp ;
				CString strNO;
				strNO.Format(_T("%d"),i);
				GetPrivateProfileString(_T("USERINFO"),_T("user")+strNO,_T(""),strTemp.GetBuffer(100),100,_T(".//BMClient.ini")); 
				if(strUser == strTemp){
					WritePrivateProfileString(_T("USERINFO"),_T("psw")+strNO,result,_T(".//BMClient.ini"));
					//将默认用户名写进配置文件中
					WritePrivateProfileString(_T("USERINFO"),_T("defaultUser"),strUser,_T(".//BMClient.ini"));
					strTemp.ReleaseBuffer();
					break;
				}else{
					strTemp.ReleaseBuffer();
					continue;
				}
			}
			if(i == count)  //如果用户名在ini文件中没有的话  就写进ini文件中
			{
				count ++;    //刷新用户个数  写进配置文件中  在配置文件的最后写进该新增的用户
				CString str = NULL;
				str.Format(_T("%d"),count);
				CString strNO;
				strNO.Format(_T("%d"),i);
				WritePrivateProfileString(_T("USERINFO"),_T("Total"),str,_T(".//BMClient.ini"));
				WritePrivateProfileString(_T("USERINFO"),_T("user")+strNO,strUser,_T(".//BMClient.ini"));
				WritePrivateProfileString(_T("USERINFO"),_T("psw")+strNO,result,_T(".//BMClient.ini"));
				//将默认用户名写进配置文件中
				WritePrivateProfileString(_T("USERINFO"),_T("defaultUser"),strUser,_T(".//BMClient.ini"));
			}
		}else{  //不记住密码
			
			int i;
			for(i = 0;i < count; i++)
			{
				CString strTemp ;
				CString strNO;
				strNO.Format(_T("%d"),i);
				GetPrivateProfileString(_T("USERINFO"),_T("user")+strNO,_T(""),strTemp.GetBuffer(100),100,_T(".//BMClient.ini")); 
				if(strUser == strTemp){
					WritePrivateProfileString(_T("USERINFO"),_T("psw")+strNO,NULL,_T(".//BMClient.ini"));
					//将默认用户名写进配置文件中
					WritePrivateProfileString(_T("USERINFO"),_T("defaultUser"),strUser,_T(".//BMClient.ini"));
					strTemp.ReleaseBuffer();
					break;
				}else{
					strTemp.ReleaseBuffer();
					continue;
				}
			}
			if(i == count)  //如果用户名在ini文件中没有的话  就写进ini文件中
			{
				count ++;    //刷新用户个数  写进配置文件中  在配置文件的最后写进该新增的用户
				CString str = NULL;
				str.Format(_T("%d"),count);
				CString strNO;
				strNO.Format(_T("%d"),i);
				WritePrivateProfileString(_T("USERINFO"),_T("Total"),str,_T(".//BMClient.ini"));
				WritePrivateProfileString(_T("USERINFO"),_T("user")+strNO,strUser,_T(".//BMClient.ini"));
				WritePrivateProfileString(_T("USERINFO"),_T("psw")+strNO,NULL,_T(".//BMClient.ini"));
				//将默认用户名写进配置文件中
				WritePrivateProfileString(_T("USERINFO"),_T("defaultUser"),strUser,_T(".//BMClient.ini"));
			}
		}
	}
	return TRUE;
}

void CLogin::EncryptCode(CString& result){  //对密码加密进行存储
	result.Empty();
	UpdateData(TRUE);
	CString code = m_password;
	int len = code.GetLength();
	USES_CONVERSION;
	char *p = T2A(code);
	char pCode[50];
	memset(pCode,0,50);
	strncpy_s(pCode,p,len);
	int readed = 0;
	while(readed < len)
	{
		if(pCode[readed]> 'A'&& pCode[readed] < 'Z')
		{
			pCode[readed] = 'A'- pCode[readed] + 'Z';
			int temp = pCode[readed]*421;
			CString str  = NULL;
			str.Format(_T("%d"),temp);
			result = result + _T("#");
			result = result + str;
			readed++;
		}
		else if(pCode[readed]> 'a'&& pCode[readed] < 'z')
		{
			pCode[readed] = 'a'-pCode[readed] + 'z';
			int temp = pCode[readed]*421;
			CString str  = NULL;
			str.Format(_T("%d"),temp);
			result = result + _T("#");
			result = result + str;
			
			readed++;
		}
		else if(pCode[readed]> '0' && pCode[readed] < '9')
		{
			pCode[readed] = '0'- pCode[readed] + '9';
			int temp = pCode[readed] * 421;
			CString str  = NULL;
			str.Format(_T("%d"),temp);
			result = result + _T("#");
			result = result + str;
			
			readed++;
		}else{
		   int temp = pCode[readed] *  421;
		   CString str  = NULL;
		   str.Format(_T("%d"),temp);
		   result = result + _T("#");
		   result = result + str;	   
		   readed++;
		}
	}
}


void CLogin::DecryptCode(CString source,CString& result)    //将密码还原
{
	
	USES_CONVERSION;
	
	char *pCode = T2A(source);

	CString str = A2T(pCode);
	int len = str.GetLength();  //不知道为啥 需要转换一次 才能求得字符串的长度
	
	int readed = 0;
	CString midstr = NULL;
	while(readed < len)
	{
		if('#' == *pCode)
		{
			if(!midstr.IsEmpty())
			{
				char temp = NULL;
				RestoreCode(midstr,temp);
				result = result + temp;
			}
			midstr.Empty();
			readed++;
			pCode++;
			continue;
		}else{
			midstr = midstr + *pCode;
			readed++;
			pCode++;
		}

		if(readed == len)   //已经读到字符串的末尾
		{
			char temp = NULL;
			RestoreCode(midstr,temp);
			result = result + temp;
			break;
		}
	}
}

void CLogin::RestoreCode(CString& midValue,char& strReturn)
{
	USES_CONVERSION;
	char * p = T2A(midValue);
	int value = atoi(p);
	strReturn = value/421;
	if(strReturn > 'A' && strReturn < 'Z')
	{
		strReturn = 'A'- strReturn + 'Z';
	}
	if(strReturn > 'a' && strReturn < 'z')
	{
		strReturn = 'a'- strReturn + 'z';
	}
	if(strReturn > '0' && strReturn < '9')
	{
		strReturn = '0'- strReturn + '9';
	}
}

void CLogin::LoadUser()    // 加载用户名和密码
{
	////****读取关于checkbox控件的状态****/////////	

	CString stateOfCheckbox;	
	CFileFind finder;
	BOOL isFind = finder.FindFile(_T(".//BMClient.ini"));
	if(isFind)
	{	
		CString _strPos = NULL;
		GetPrivateProfileString(_T("登录角色"),_T("默认角色"),_T("FALSE"),_strPos.GetBuffer(30),30,_T(".//BMClient.ini"));
		if(_T("FALSE") == _strPos)
		{
			_strPos.ReleaseBuffer();
		}else
		{
			//m_pos.SetWindowText(pos);
			int index = m_pos.FindStringExact(0,_strPos);
			if(CB_ERR != index)
			{
				m_pos.SetCurSel(index);
			}
			_strPos.ReleaseBuffer();
		}
		GetPrivateProfileString(_T("remember password"),_T("value"),_T("FALSE"),stateOfCheckbox.GetBuffer(10),10,_T(".//BMClient.ini"));
		if(_T("TRUE") == stateOfCheckbox)
		{
			m_remPsw.SetCheck(TRUE);    //勾选了记住密码按钮
			CString total = NULL ;
			GetPrivateProfileString(_T("USERINFO"),_T("Total"),NULL,total.GetBuffer(100),100,_T(".//BMClient.ini"));
			if(total)  //有记录
			{
				USES_CONVERSION;
				int count = atoi(T2A(total));
				CString defaultstr = NULL;
				GetPrivateProfileString(_T("USERINFO"),_T("defaultUser"),NULL,defaultstr.GetBuffer(100),100,_T(".//BMClient.ini"));
				for(int i = 0; i < count; i++)
				{
					CString str = NULL;
					str.Format(_T("%d"),i);
					CString result = NULL;
					GetPrivateProfileString(_T("USERINFO"),_T("user")+str,NULL,result.GetBuffer(100),100,_T(".//BMClient.ini"));
					m_username.AddString(result);
					if(defaultstr == result)
					{
						CString psw = NULL;
						GetPrivateProfileString(_T("USERINFO"),_T("psw")+str,NULL,psw.GetBuffer(100),100,_T(".//BMClient.ini"));						
						CString code = NULL;	
						if(psw){
							DecryptCode(psw,code);
							m_password = code;
						}else{
							m_password = _T("");
						}
						UpdateData(FALSE);
						psw.ReleaseBuffer();
						m_username.SetWindowText(defaultstr);
					}
					result.ReleaseBuffer();
				}
				total.ReleaseBuffer();
			}
		}
		if(_T("FALSE") == stateOfCheckbox){
			m_remPsw.SetCheck(FALSE);
			CString total = NULL ;
			GetPrivateProfileString(_T("USERINFO"),_T("total"),NULL,total.GetBuffer(100),100,_T(".//BMClient.ini"));
			if(total)  //有记录
			{
				USES_CONVERSION;
				int count = atoi(T2A(total));
				CString defaultstr = NULL;
				GetPrivateProfileString(_T("USERINFO"),_T("defaultUser"),NULL,defaultstr.GetBuffer(100),100,_T(".//BMClient.ini"));
				for(int i = 0; i < count; i++)
				{
					CString str = NULL;
					str.Format(_T("%d"),i);
					CString result = NULL;
					GetPrivateProfileString(_T("USERINFO"),_T("user")+str,NULL,result.GetBuffer(100),100,_T(".//BMClient.ini"));
					m_username.AddString(result);
					if(defaultstr == result)
					{
						m_username.SetWindowText(defaultstr);
					}
					result.ReleaseBuffer();
				}
				total.ReleaseBuffer();
			}
		}
	}else{
		AfxMessageBox(_T("系统配置文件丢失！"));
		m_remPsw.SetCheck(FALSE);
	}

	stateOfCheckbox.ReleaseBuffer();
	////****读取关于checkbox控件的状态*****/////////
}

void CLogin::OnCbnSelchangeComboLoginUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	CString curUser;
	//m_username.GetWindowText(curUser);
	int index  = m_username.GetCurSel();
	m_username.GetLBText(index,curUser);
	CString total = NULL ;
	GetPrivateProfileString(_T("USERINFO"),_T("total"),NULL,total.GetBuffer(100),100,_T(".//BMClient.ini"));
	USES_CONVERSION;
	int count = atoi(T2A(total));
	for(int i = 0; i < count; i++)
	{
		CString str = NULL;
		str.Format(_T("%d"),i);
		CString result = NULL;
		GetPrivateProfileString(_T("USERINFO"),_T("user")+str,NULL,result.GetBuffer(100),100,_T(".//BMClient.ini"));
		if( curUser == result)
		{
			CString psw = NULL;
			GetPrivateProfileString(_T("USERINFO"),_T("psw")+str,NULL,psw.GetBuffer(100),100,_T(".//BMClient.ini"));						
			CString code = NULL;	
			if(psw){   //密码非空
				DecryptCode(psw,code);
				m_password = code;
			}else{
				m_password = _T("");
			}
			UpdateData(FALSE);
			psw.ReleaseBuffer();
			//m_username.SetWindowText(defaultstr);
		}
	}
	total.ReleaseBuffer();
}


void CLogin::OnCbnEditchangeComboLoginUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	CString curUser;
	m_username.GetWindowText(curUser);
	int flag = m_username.FindStringExact(0,curUser);
	if(CB_ERR != flag)  //找到了
	{
		CString total = NULL ;
		GetPrivateProfileString(_T("USERINFO"),_T("total"),NULL,total.GetBuffer(100),100,_T(".//BMClient.ini"));
		USES_CONVERSION;
		int count = atoi(T2A(total));
		for(int i = 0; i < count; i++)
		{
			CString str = NULL;
			str.Format(_T("%d"),i);
			CString result = NULL;
			GetPrivateProfileString(_T("USERINFO"),_T("user")+str,NULL,result.GetBuffer(100),100,_T(".//BMClient.ini"));
			if( curUser == result)
			{
				CString psw = NULL;
				GetPrivateProfileString(_T("USERINFO"),_T("psw")+str,NULL,psw.GetBuffer(100),100,_T(".//BMClient.ini"));						
				CString code = NULL;	
				if(psw){   //密码非空
					DecryptCode(psw,code);
					m_password = code;
				}else{
					m_password = _T("");
					
				}
				UpdateData(FALSE);
				psw.ReleaseBuffer();
				//m_username.SetWindowText(defaultstr);
			}
		}
		total.ReleaseBuffer();
	}else{
		m_password = _T("");
		UpdateData(FALSE);
	}
}




HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		if(pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS)
		{
			pDC->SetTextColor(RGB(21,21,21));
		}
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(RGB(185,211,255));
		
		return b_static;
		
	}
	
	if(nCtlColor == CTLCOLOR_DLG){
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(RGB(185,211,255)); 
		HBRUSH b=CreateSolidBrush(BACKGROUNDCOLOR); 
		return b;
	}
	return hbr;
}


//void CLogin::OnDestroy()
//{
//	if(IDYES == MessageBox(_T("确实要退出吗？"),_T("提示"),MB_YESNO))
//	{
//		CSock::m_bCurTaskBreak = TRUE;
//		Sleep(1000);
//		//CBMApp* app = (CBMApp*)AfxGetApp();
//		//app->ExitInstance();
//		CDialogEx::OnDestroy();
//	}else{
//		CDialogEx::OnCancel();
//	}
//	// TODO: 在此处添加消息处理程序代码
//}


void CLogin::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pLogin = NULL;
	CDialogEx::OnClose();
}


void CLogin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			m_status.SetWindowText(_T("登录超时，请重新登录！"));
			SetCloseButton(FALSE);
			KillTimer(1);
		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CLogin::SetCloseButton(BOOL b)
{
	CMenu *pSysMenu = (CMenu *)GetSystemMenu(FALSE);
    ASSERT(pSysMenu != NULL);
	if(b){
		pSysMenu->EnableMenuItem(SC_CLOSE,MF_DISABLED);
	}else{
		pSysMenu->EnableMenuItem(SC_CLOSE,MF_ENABLED);
	}
}