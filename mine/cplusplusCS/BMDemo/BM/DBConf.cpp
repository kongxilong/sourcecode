// DBConf.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "DBConf.h"
#include "afxdialogex.h"
#include "DBNew.h"
#include "Login.h"

// CDBConf 对话框

IMPLEMENT_DYNAMIC(CDBConf, CDialogEx)

CDBConf::CDBConf(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDBConf::IDD, pParent)
{
	//m_portDefault = 6686;
}

CDBConf::~CDBConf()
{
}

void CDBConf::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DB_NAME, m_DBName);
	DDX_Control(pDX, IDC_COMBO_IP, m_ip);
	DDX_Control(pDX, IDC_COMBO_PORT, m_port);
}


BEGIN_MESSAGE_MAP(CDBConf, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DBNEW, &CDBConf::OnBnClickedButtonDbnew)
	ON_BN_CLICKED(IDCANCEL, &CDBConf::OnBnClickedCancel)
//	ON_BN_CLICKED(IDOK, &CDBConf::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BTN_LOGIN_CONTINUE, &CDBConf::OnBnClickedBtnLoginContinue)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDBConf 消息处理程序


void CDBConf::OnBnClickedButtonDbnew()
{
	// TODO: 在此添加控件通知处理程序代码
	WT;
	UpdateData(TRUE);
	CString ip ;
	CString port;
	m_ip.GetWindowText(ip);
	m_port.GetWindowText(port);
	if(ip.IsEmpty())
	{
		MessageBox(_T("请选择或填写服务器ip地址"));
		return;
	}
	if(port.IsEmpty())
	{
		MessageBox(_T("请选择或填写服务器端口"));
		return;
	}else{
		int len = port.GetLength();
		char *p = T2A(port);
		for(int i=0;i<len;i++)
		{
			if(p[i] >'9'||p[i]<'0')
			{
				MessageBox(_T("服务器端口必须为数字"));
				return;
			}
		}
	}

	CDBNew		dlg;
	dlg.m_ip = ip;
	dlg.m_port = port;
	dlg.DoModal();
}


void CDBConf::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

BOOL CDBConf::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	//HICON hIcon= ::LoadIcon(NULL,(LPCWSTR)IDI_ICON_BMMAIN);
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON_BMMAIN);
	
	this->SetIcon(hIcon,TRUE);
	this->SetIcon(hIcon,FALSE);

	void* ptr = (void *)&m_port;
	CString path = _T(".//MyPort.txt");
	if(!LoadComb(ptr,path)){   //如果读取失败
		CString port;
		port.Format(_T("%d"),SERVPORT);
		m_port.AddString(port);
		m_port.SetCurSel(0);
	}
	
	void* pIP = (void*)&m_ip;
	CString pathOfIp = _T(".//MyServerIP.txt");
	if(!LoadComb(pIP,pathOfIp)){   //如果读取失败
		m_ip.AddString(_T("127.0.0.1"));
		m_ip.SetCurSel(0);
	}

	void* pDB = (void*)&m_DBName;
	CString pathOfDB = _T(".//MyDB.txt");
	if(!LoadComb(pDB,pathOfDB)){   //如果读取失败
		m_DBName.AddString(_T("TEST"));
		m_DBName.SetCurSel(0);
	}

	return TRUE;
}



void CDBConf::OnBnClickedBtnLoginContinue()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取端口号 传给全局变量  并写进txt 中
	CString strPort;
	m_port.GetWindowText(strPort);
	if(strPort.IsEmpty())
	{
		AfxMessageBox(_T("请选择或输入一个有效服务器端口"));
		return;
	}
	CString pathOfPort = _T(".//MyPort.txt");
	void * p = (void*)&m_port;
	WriteCombToFile(p,pathOfPort);

	USES_CONVERSION;
	char *portTemp = T2A(strPort);
	int port = atoi(portTemp);
	CSock::m_port = port;

	//获取服务器IP 传给全局变量  并写进txt 中
	CString strIP = NULL;
	m_ip.GetWindowText(strIP);
	if(strIP.IsEmpty()){
		AfxMessageBox(_T("请选择或输入一个有效的IP地址"));		
		return;
	}
	void* pIP = (void*)&m_ip;
	CString pathOfIp = _T(".//MyServerIP.txt");
	WriteCombToFile(pIP,pathOfIp);

	CSock::m_IP = strIP;
	
	
	//获取数据库名字 传给服务器  并写进本地txt 中
	CString strDB = NULL;
	m_DBName.GetWindowText(strDB);
	if(strDB.IsEmpty()){
		AfxMessageBox(_T("请选择或输入一个有效的数据库名称！"));		
		return;
	}
	CSock::m_DB = strDB;
	void* pDB = (void*)&m_DBName;
	CString pathOfDB = _T(".//MyDB.txt");
	WriteCombToFile(pDB,pathOfDB);

	CDialog::OnOK();
	CLogin lgDlg;
	lgDlg.DoModal();	
}

void CDBConf::WriteCombToFile(void* comb,CString path)             //将combox 中的内容 写到path文件中
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

void CDBConf:: WriteToText(CString& strToWtrite,CString& writeToPath)     //将str 写到path路径的文件中 覆盖原有的
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

BOOL CDBConf::ReadFromTxt(CString& path,CString& result){
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

BOOL CDBConf::LoadComb(void* ptr,CString& path)		//从文本文件中加载combox 的内容
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

HBRUSH CDBConf::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		/*
		if(pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS)
		{
			pDC->SetTextColor(RGB(21,21,21));
		}*/
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(RGB(185,211,255));
		
		return b_static;
		
	}
	if(nCtlColor == CTLCOLOR_DLG){
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(RGB(185,211,255)); 
		HBRUSH b=CreateSolidBrush(RGB(185,211,255)); 
		return b;
	}
	return hbr;
}
