// ThreadOperate.cpp: implementation of the ThreadOperate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadServer.h"
#include "ThreadOperate.h"
#include "ADOConn.h"
#include "ThreadServerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CString IP_Address_Require;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ThreadOperate::ThreadOperate()
{

}

ThreadOperate::~ThreadOperate()
{

}



//搜索的处理线程
UINT SearchHandleThread(LPVOID clientSocket)
{
	AfxSocketInit(NULL);
	SOCKET client = (SOCKET)clientSocket;
	CSocket SearchHandle;
	SearchHandle.Attach(client);//转化为Csocket

	char readSearchFile[150]={0};//
	SearchHandle.Receive(readSearchFile,sizeof(readSearchFile),0);
	//AfxMessageBox(readSearchFile);
	ADOConn adoconn;
	adoconn.OnInitConnect();
	CString sql;
	sql.Format(_T("select * from fileinfo where filename like '%%%s%%'"),readSearchFile);//查询比较是否重复共享
	_bstr_t vSQL;
	vSQL=(_bstr_t)sql;
	_RecordsetPtr m_pRecordSet;
	m_pRecordSet=adoconn.GetRecordSet(vSQL);
	
	FILEINFO FileInfoSearchSend;
	
	CString FileType,FileName,FilePathName;

	int n=0;//获取数据库中所含文件的个数
	while(!m_pRecordSet->adoEOF)
	{
		n++;
		m_pRecordSet->MoveNext();
	}

	m_pRecordSet->Close();
	//adoconn.ExitConnect();
	SearchHandle.Send(&n,sizeof(n),0);
	CString sql1;
	//AfxMessageBox(readSearchFile);
	sql1.Format(_T("select * from fileinfo where filename like '%%%s%%'"),readSearchFile);//查询比较是否重复共享
	_bstr_t vSQL1;
	vSQL1=(_bstr_t)sql1;
	_RecordsetPtr m_pRecordSet1;
	m_pRecordSet1=adoconn.GetRecordSet(vSQL1);
	int nCount =0; //在哪一行插入
	while(!m_pRecordSet1->adoEOF)  //循环发送数据库文件信息
	{
		memset(&FileInfoSearchSend,0,sizeof(FileInfoSearchSend));
		FileType = m_pRecordSet1->GetCollect("filetype");

		FileInfoSearchSend.fileSize = m_pRecordSet1->GetCollect("filesize");
		FileName= m_pRecordSet1->GetCollect("filename");
		FilePathName=m_pRecordSet1->GetCollect("filepathname");
		strcpy_s(FileInfoSearchSend.fileType,FileType);
		strcpy_s(FileInfoSearchSend.fileName,FileName);
		strcpy_s(FileInfoSearchSend.filepath,FilePathName);
		SearchHandle.Send(&FileInfoSearchSend,sizeof(FileInfoSearchSend),0);
		nCount++;
		m_pRecordSet1->MoveNext();
	}
	m_pRecordSet1->Close();
	//adoconn.ExitConnect();
	SearchHandle.Detach();
	SearchHandle.Close();//关闭套接字
	return 0;
}

//注册处理线程
UINT RegisterHandleThread(LPVOID clientSocket)
{
	AfxSocketInit(NULL);
	SOCKET client = (SOCKET)clientSocket;
	CSocket RegisterSocket;
	RegisterSocket.Attach(client);//转化为Csocket

	USERINFO userinfo;
	memset(&userinfo,0,sizeof(userinfo));
	RegisterSocket.Receive(&userinfo,sizeof(userinfo),0);
	try
	{
		ADOConn adoconn;//连接对象
		adoconn.OnInitConnect();
		CString sqlstr;
		sqlstr.Format(_T("select * from userinfo where username='%s'"),userinfo.username);//schooljoy
		_bstr_t vSQL;
		vSQL=(_bstr_t)sqlstr;	
		_RecordsetPtr m_recordset;//记录集对象
		m_recordset=adoconn.GetRecordSet(vSQL);//执行查询
		char flags[2]={0};
		if(m_recordset->adoEOF)
		{
			m_recordset->AddNew();
			m_recordset->PutCollect("username",_variant_t(userinfo.username));
			m_recordset->PutCollect("password",_variant_t(userinfo.password));
			m_recordset->Update();
			//MessageBox(_T("用户添加成功，请记住用户名和密码"),_T("恭喜"),MB_OK);
			adoconn.ExitConnect();
			flags[0]='#';
			flags[1]='1';
			RegisterSocket.Send(flags,sizeof(flags),0);
			RegisterSocket.Close();
		}
		else
		{
			//MessageBox(_T("用户名已存在！请重新注册另外一个"),_T("警告"),MB_ICONWARNING|MB_OK);
			flags[0]='#';
			flags[1]='0';
			RegisterSocket.Send(flags,sizeof(flags),0);
			RegisterSocket.Close();
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("注册失败"));
		return -1 ;
	}
	return 0;
}
//上传线程
UINT UploadHandleThread(LPVOID clientSocket)
{
	AfxSocketInit(NULL);
	SOCKET client = (SOCKET)clientSocket;
	CSocket UploadSocket;
	UploadSocket.Attach(client);//转化为Csocket

	FILEINFO fileinfo;
	memset(&fileinfo,0,sizeof(fileinfo));
	UploadSocket.Receive(&fileinfo,sizeof(fileinfo),0);
	try
	{
		char flags[2]={0};
		ADOConn adoconn;//连接对象
		adoconn.OnInitConnect();
		CString sqlstr;
		//filesize.Format("%f",fileinfo.fileSize);
		sqlstr.Format(_T("select * from fileinfo where filename='%s' and filesize='%f' and userlist='%s'"),fileinfo.fileName,fileinfo.fileSize,fileinfo.username);//schooljoy,filesize
		_bstr_t vSQL;
		vSQL=(_bstr_t)sqlstr;	
		_RecordsetPtr m_recordset;//记录集对象
		m_recordset=adoconn.GetRecordSet(vSQL);//执行查询
		if(m_recordset->adoEOF)
		{
			m_recordset->AddNew();
			m_recordset->PutCollect("filename",(const _variant_t)(fileinfo.fileName));//fileinfo.fileName
			m_recordset->PutCollect("userlist",(const _variant_t)(fileinfo.username));//fileinfo.username
			m_recordset->PutCollect("filepathname",(const _variant_t)(fileinfo.filepath));//fileinfo.filepath
			m_recordset->PutCollect("filetype",(const _variant_t)(fileinfo.fileType));//fileinfo.fileType
			m_recordset->PutCollect("userIP",(const _variant_t)(fileinfo.fileownerIP));//fileinfo.fileownerIP
			m_recordset->PutCollect("filesize",(const _variant_t)(fileinfo.fileSize));//fileinfo.fileSize
			//m_recordset->PutCollect("totalsize",(const _variant_t)(filesize));
			m_recordset->Update();			
			//adoconn.ExitConnect();
			flags[0]='#';
			flags[1]='1';
			UploadSocket.Send(flags,sizeof(flags),0);
			UploadSocket.Close();

		}
		else
		{
			flags[0]='#';
			flags[1]='0';
			UploadSocket.Send(flags,sizeof(flags),0);
			UploadSocket.Close();

		}
		
	}
	catch(...)
	{
		AfxMessageBox(_T("上传文件信息失败"));
		return -1 ;
	}

	return 0;

}

//用户退出登录线程
UINT QuitHandleThread(LPVOID clientSocket)
{
	AfxSocketInit(NULL);
	SOCKET client = (SOCKET)clientSocket;
	CSocket QuitSocket;
	QuitSocket.Attach(client);//转化为Csocket
	USERINFO userinfo;
	memset(&userinfo,0,sizeof(userinfo));
	QuitSocket.Receive(&userinfo,sizeof(userinfo),0);

	CString sqlstr;
	try
	{
		ADOConn adoconn;//连接对象
		adoconn.OnInitConnect();
		sqlstr.Format(_T("select * from userinfo where username='%s'and password='%s'"),userinfo.username,userinfo.password);//StudentTable
		_bstr_t vSQL;
		vSQL=(_bstr_t)sqlstr;	
		_RecordsetPtr m_recordset;//记录集对象
		m_recordset=adoconn.GetRecordSet(vSQL);//执行查询
		char flags[2]={0};
		if(!m_recordset->adoEOF)
		{
			m_recordset->Close();
			CString _str1;
			_str1.Format(_T("select * from userinfo where username='%s'and password='%s'and onlineflag='%s'"),userinfo.username,userinfo.password,_T("1"));
			_bstr_t vSQL1;
			vSQL1=(_bstr_t)_str1;	
			_RecordsetPtr m_recordset1;//记录集对象
			m_recordset1=adoconn.GetRecordSet(vSQL1);//执行查询
			if(!m_recordset1->adoEOF)
			{
				m_recordset1->Close();
				flags[0]='#';
				flags[1]='1';
				QuitSocket.Send(flags,sizeof(flags),0);
				CString sql2;
				sql2.Format(_T("update userinfo set username='%s',password='%s',IP='%s',onlineflag='%s' where username='%s'"),userinfo.username,userinfo.password,_T("0"),_T("0"),userinfo.username);
				//m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
				_bstr_t vSQL2;
				vSQL2=(_bstr_t)sql2;	
				adoconn.ExecuteSQL(vSQL2);
				CString _str;
				_str.Format("用户'%s'退出成功！",userinfo.username);
				AfxMessageBox(_str);
				//adoconn.ExitConnect();
				QuitSocket.Close();
				return 0;
			}
			else
			{
				m_recordset1->Close();
				flags[0]='#';
				flags[1]='0';
				QuitSocket.Send(flags,sizeof(flags),0);
				CString _str;
				_str.Format("用户'%s'没有登录，退出失败！",userinfo.username);
				AfxMessageBox(_str);
				//adoconn.ExitConnect();
				QuitSocket.Close();
				return 0;
			}
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("退出失败"));
		return 0 ;
	}
	QuitSocket.Close();

	return 0;

}

//用户登录的处理线程
UINT LoginHandleThread(LPVOID clientSocket)
{
	AfxSocketInit(NULL);
	SOCKET client = (SOCKET)clientSocket;
	CSocket LoginHandle;
	LoginHandle.Attach(client);//转化为Csocket

	USERINFO userinfo;
	memset(&userinfo,0,sizeof(userinfo));
	LoginHandle.Receive(&userinfo,sizeof(userinfo),0);
	CString sqlstr;
	try
	{
		ADOConn adoconn;//连接对象
		adoconn.OnInitConnect();
		sqlstr.Format(_T("select * from userinfo where username='%s'and password='%s'"),userinfo.username,userinfo.password);//StudentTable
		_bstr_t vSQL;
		vSQL=(_bstr_t)sqlstr;	
		_RecordsetPtr m_recordset;//记录集对象
		m_recordset=adoconn.GetRecordSet(vSQL);//执行查询
		char flags[2]={0};
		if(!m_recordset->adoEOF)
		{
			m_recordset->Close();
			CString _str1;
			_str1.Format(_T("select * from userinfo where username='%s'and password='%s'and onlineflag='%s'"),userinfo.username,userinfo.password,_T("1"));
			_bstr_t vSQL1;
			vSQL1=(_bstr_t)_str1;	
			_RecordsetPtr m_recordset1;//记录集对象
			m_recordset1=adoconn.GetRecordSet(vSQL1);//执行查询
			if(!m_recordset1->adoEOF)
			{
				flags[0]='#';
				flags[1]='2';
				LoginHandle.Send(flags,sizeof(flags),0);
				CString _str;
				_str.Format("'%s'已登录,无需再次登录！",userinfo.username);
				AfxMessageBox(_str);
				adoconn.ExitConnect();
				LoginHandle.Close();
				return 0;
			}
			else
			{
				flags[0]='#';
				flags[1]='1';
				LoginHandle.Send(flags,sizeof(flags),0);
				CString sql2;
				//AfxMessageBox(userinfo.userIP);
				sql2.Format(_T("update userinfo set username='%s',password='%s',IP='%s',onlineflag='%s' where username='%s'"),userinfo.username,userinfo.password,userinfo.userIP,_T("1"),userinfo.username);
				//m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
				_bstr_t vSQL2;
				vSQL2=(_bstr_t)sql2;	
				adoconn.ExecuteSQL(vSQL2);
				CString _str;
				_str.Format("'%s'登录成功！",userinfo.username);
				AfxMessageBox(_str);
				adoconn.ExitConnect();
				LoginHandle.Close();
				return 0;
			}
			m_recordset1->Close();
		}
		else
		{
			CString _str3;
			_str3.Format(_T("select * from userinfo where username='%s'"),userinfo.username);
			_bstr_t vSQL3;
			vSQL3=(_bstr_t)_str3;	
			_RecordsetPtr m_recordset3;//记录集对象
			m_recordset3=adoconn.GetRecordSet(vSQL3);//执行查询
			if (m_recordset3->adoEOF)
			{
				flags[0]='#';
				flags[1]='0';
				LoginHandle.Send(flags,sizeof(flags),0);
				CString _str;
				_str.Format("'%s'尝试登录失败，用户名不存在！",userinfo.username);
				AfxMessageBox(_str);
				adoconn.ExitConnect();
				LoginHandle.Close();
				return 0;
			}
			else
			{
				flags[0]='#';
				flags[1]='3';
				LoginHandle.Send(flags,sizeof(flags),0);
				CString _str;
				_str.Format("'%s'尝试登录失败，用户密码不正确！",userinfo.username);
				AfxMessageBox(_str);
				adoconn.ExitConnect();
				LoginHandle.Close();
				return 0;
			}
			
		}
		

	}
	catch(...)
	{
		AfxMessageBox(_T("登陆失败"));
		return 0 ;
	}
	LoginHandle.Close();
	return 0;
}



//请求文件信息的处理线程
UINT RequireHandleThread(LPVOID clientSocket)
{
	AfxSocketInit(NULL);
	SOCKET client = (SOCKET)clientSocket;
	CSocket RequireHandle;
	RequireHandle.Attach(client);//转化为Csocket

	FILEINFO FileInfo1;
	memset(&FileInfo1,0,sizeof(FileInfo1));
	RequireHandle.Receive(&FileInfo1,sizeof(FileInfo1));
	CString FileName=FileInfo1.fileName;
	ADOConn adoconn;//到数据库中查找信息
	adoconn.OnInitConnect();
	CString sql;
	sql.Format(_T("select * from userinfo,fileinfo where userinfo.username=fileinfo.userlist and fileinfo.filename='%s' and userinfo.onlineflag='%s'"),FileName,_T("1"));//数据库查找文件是否匹配
	_bstr_t vSQL;
	vSQL=(_bstr_t)sql;
	_RecordsetPtr m_pRecordSet;
	m_pRecordSet=adoconn.GetRecordSet(vSQL);
	CString FileOwnerIP;//文件拥有者IP
	if(!m_pRecordSet->adoEOF)
	{    
		FileOwnerIP = m_pRecordSet->GetCollect("IP");//得到资源拥有方的IP
	}	
	else
	{
		AfxMessageBox(_T("查找出错！"));

	}
	m_pRecordSet->Close();//关闭记录集
	FILEINFO FileInfo2;
	memset(&FileInfo2,0,sizeof(FileInfo2));
	strcpy_s(FileInfo2.fileownerIP,FileOwnerIP);//将IP写进结构体
	RequireHandle.Send(&FileInfo2,sizeof(FileInfo2));	
	RequireHandle.Detach();
	RequireHandle.Close();//关闭套接字
	return 0;
}

////////////////////////////////////////////////////
UINT StartshowResourcesThread(LPVOID pParam)
{
	CThreadServerDlg *dialog = (CThreadServerDlg *)pParam;
	dialog->m_Resourceslist.DeleteAllItems();//删除ListView中的所有数据
	CString _str="请稍等，正在努力更新资源信息中。。。";
	dialog->Update(_str);
	try
	{
		ADOConn  adoconn;
		adoconn.OnInitConnect();
		CString sqlstr;
		_RecordsetPtr m_recordset;//记录集对象
		_ConnectionPtr m_pConnection;
		sqlstr.Format(_T("select * from fileinfo "));
		_bstr_t vSQL;
		vSQL=(_bstr_t)sqlstr;		
		m_recordset=adoconn.GetRecordSet(vSQL);//执行查询
		int nCount =0; //在哪一行插入
		while(!m_recordset->adoEOF)
		{
			dialog->m_Resourceslist.InsertItem(nCount,_T(""));//插入新的一行
			dialog->m_Resourceslist.SetItemText(nCount,0,(LPCTSTR)(_bstr_t)m_recordset->GetCollect("filename"));
			dialog->m_Resourceslist.SetItemText(nCount,1,(LPCTSTR)(_bstr_t)m_recordset->GetCollect("userlist"));
			dialog->m_Resourceslist.SetItemText(nCount,2,(LPCTSTR)(_bstr_t)m_recordset->GetCollect("filetype"));
			double FileSize=m_recordset->GetCollect("filesize");
			dialog->m_Resourceslist.SetItemText(nCount,3,(LPCTSTR)(_bstr_t)dialog->FileSizeTransfer(FileSize));
			nCount++;
			m_recordset->MoveNext();
		}
		CString _str="资源信息更新成功";
		dialog->Update(_str);
		adoconn.ExitConnect();
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
	return 0;
}

//显示用户列表的线程
UINT StartshowUserThread(LPVOID pParam)
{
	CThreadServerDlg *dialog = (CThreadServerDlg *)pParam;
	dialog->m_Userlist.DeleteAllItems();//删除ListView中的所有数据
	CString _str="请稍等，正在努力更新用户信息中。。。";
	dialog->Update(_str);
	try
	{
		ADOConn  adoconn;
		adoconn.OnInitConnect();
		CString sqlstr;
		_RecordsetPtr m_recordset;//记录集对象
		//_ConnectionPtr m_pConnection;
		sqlstr.Format(_T("select * from userinfo where onlineflag='%s'"),"1");
		_bstr_t vSQL;
		vSQL=(_bstr_t)sqlstr;		
		m_recordset=adoconn.GetRecordSet(vSQL);//执行查询
		int nCount =0; //在哪一行插入
		while(!m_recordset->adoEOF)
		{
			dialog->m_Userlist.InsertItem(nCount,_T(""));//插入新的一行
			dialog->m_Userlist.SetItemText(nCount,0,(LPCTSTR)(_bstr_t)m_recordset->GetCollect("username"));
			dialog->m_Userlist.SetItemText(nCount,1,(LPCTSTR)(_bstr_t)m_recordset->GetCollect("IP"));
			nCount++;
			m_recordset->MoveNext();
		}
		CString _str="用户信息更新成功";
		dialog->Update(_str);
		adoconn.ExitConnect();
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
	return 0;

}

//开始主线程
UINT StartServerThread(LPVOID pParam)
{
	CThreadServerDlg *dialog = (CThreadServerDlg *)pParam;
	AfxSocketInit(NULL);
    CSocket ServerListenSocket;//服务器监听套接字
	if(!ServerListenSocket.Create(dialog->m_serverPORT))
	{
	  CString _str=_T("创建套接字异常，请检查本地端口是否占用\\r\n");
	  dialog->Update(_str);
	  return 0;
	}
	if(!ServerListenSocket.Listen(500))
	{
		CString _str=_T("启动监听过程失败,\r\n");
		dialog->Update(_str);
		return 0;
	}
	CString _str=_T("监听程序已启动，正在等待连接。。。\r\n");
	dialog->Update(_str);

    CSocket ClientSocketIn;
	int stateCode=0;
	char buffer[sizeof(stateCode)]="0";
	while(true)
	{
		ServerListenSocket.Accept(ClientSocketIn);
		int bytesRec = ClientSocketIn.Receive(buffer,sizeof(stateCode),0);//接收类型码
		stateCode = atoi(buffer);//转变为整型
		switch(stateCode)
		{
		case 1://1号情况，用户登录
			{
				sockaddr_in clientAddress;
				CString ipAddress;
				CString nowPort;
				int addressLen = sizeof(clientAddress);
				::getpeername(ClientSocketIn,(sockaddr*)&clientAddress,&addressLen);
				ipAddress = inet_ntoa(clientAddress.sin_addr);
				int port = ntohs(clientAddress.sin_port);
				nowPort.Format("%d",port);
				CString _str;
				_str.Format("正在处理来自'%s'的登录请求，通信端口号为'%s'",ipAddress,nowPort);
				dialog->Update(_str);
				SOCKET LoginHandleSocket;
				LoginHandleSocket = ClientSocketIn.Detach();//从套接字分析转而使用另外一个套接字
				AfxBeginThread(LoginHandleThread,(LPVOID)LoginHandleSocket);//开启工作者线
				break;
			}

		case 2://2号情况，创建一个子线程,处理客户端索要文件请求
			{
				sockaddr_in clientAddress;
				int addressLen = sizeof(clientAddress);
				::getpeername(ClientSocketIn,(sockaddr*)&clientAddress,&addressLen);
				IP_Address_Require= inet_ntoa(clientAddress.sin_addr);
				CString _str;
				_str.Format("正在处理来自'%s'的远程文件请求",IP_Address_Require);
				dialog->Update(_str);
				SOCKET RequireHandleSocket;
				RequireHandleSocket = ClientSocketIn.Detach();//从套接字分析转而使用另外一个套接字
				AfxBeginThread(RequireHandleThread,(LPVOID)RequireHandleSocket);//开启工作者线程参数handeConnectThread
				break;
			}
		case 3://3号情况，创建一个子线程,处理客户端搜索文件请求
			{
				sockaddr_in clientAddress;
				int addressLen = sizeof(clientAddress);
				::getpeername(ClientSocketIn,(sockaddr*)&clientAddress,&addressLen);
				IP_Address_Require= inet_ntoa(clientAddress.sin_addr);
				CString _str;
				_str.Format("正在处理来自'%s'的搜索文件请求",IP_Address_Require);
				dialog->Update(_str);
				SOCKET SearchHandleSocket;
				SearchHandleSocket = ClientSocketIn.Detach();//从套接字分析转而使用另外一个套接字
				AfxBeginThread(SearchHandleThread,(LPVOID)SearchHandleSocket);//开启工作者线程参数handeConnectThread
				break;
			}
		case 4://4号情况，用户发过来一个注册申请，创建子线程，处理客户端申请
			{
				sockaddr_in clientAddress;
				int addressLen = sizeof(clientAddress);
				::getpeername(ClientSocketIn,(sockaddr*)&clientAddress,&addressLen);
				IP_Address_Require= inet_ntoa(clientAddress.sin_addr);
				CString _str;
				_str.Format("正在处理来自'%s'的用户注册请求",IP_Address_Require);
				dialog->Update(_str);
				SOCKET RegisterHandleSocket;
				RegisterHandleSocket = ClientSocketIn.Detach();//从套接字分析转而使用另外一个套接字
				AfxBeginThread(RegisterHandleThread,(LPVOID)RegisterHandleSocket);//开启工作者线程参数handeConnectThread
				break;

			}
			case 5://5号情况，用户发过来一个上传文件申请，创建子线程，处理客户端上传申请
			{
				sockaddr_in clientAddress;
				int addressLen = sizeof(clientAddress);
				::getpeername(ClientSocketIn,(sockaddr*)&clientAddress,&addressLen);
				IP_Address_Require= inet_ntoa(clientAddress.sin_addr);
				CString _str;
				_str.Format("正在处理来自'%s'的用户上传文件请求",IP_Address_Require);
				dialog->Update(_str);
				SOCKET UploadHandleSocket;
				UploadHandleSocket = ClientSocketIn.Detach();//从套接字分析转而使用另外一个套接字
				AfxBeginThread(UploadHandleThread,(LPVOID)UploadHandleSocket);//开启工作者线程参数handeConnectThread
				break;
			}
			case 6://6号情况，用户退出登录请求
			{
				sockaddr_in clientAddress;
				int addressLen = sizeof(clientAddress);
				::getpeername(ClientSocketIn,(sockaddr*)&clientAddress,&addressLen);
				IP_Address_Require= inet_ntoa(clientAddress.sin_addr);
				CString _str;
				_str.Format("正在处理来自'%s'的用户退出登录请求",IP_Address_Require);
				dialog->Update(_str);
				SOCKET QuitHandleSocket;
				QuitHandleSocket = ClientSocketIn.Detach();//从套接字分析转而使用另外一个套接字
				AfxBeginThread(QuitHandleThread,(LPVOID)QuitHandleSocket); //开启工作者线程参数
				break;
			}
		}
		ClientSocketIn.Close();
	}
	return 0;
}



void ThreadOperate::startMainThread(void* cw)
{
	this->dialog =(CDialog*)cw;
	AfxBeginThread(StartServerThread,dialog);
}
void ThreadOperate::startshowUserThread( void* cw)
{
	this->dialog =(CDialog*)cw;
	AfxBeginThread(StartshowUserThread,dialog);
}

void ThreadOperate::startshowResourcesThread( void* cw)
{
	this->dialog =(CDialog*)cw;
	AfxBeginThread(StartshowResourcesThread,dialog);
}