// DataSearchDao.cpp: implementation of the DataSearchDao class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadServer.h"
#include "DataSearchDao.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataSearchDao::DataSearchDao()
{

}

DataSearchDao::~DataSearchDao()
{

}
/************************************************************************/
/* 查询某个视频文件的元信息（即有几个分段，每个分段的起始位置是什么）  */
/************************************************************************/
list<FragmentBean> DataSearchDao::getMetaFragments(MetaFileInfo fileInfo){
	CString fileName = fileInfo.fileName;
	list<FragmentBean> metaFragmentList =  list<FragmentBean>();
	DBConnection dataSource;
	m_pConnection = dataSource.getConnection();
    m_pCommand.CreateInstance(__uuidof(Command));
	m_pCommand->ActiveConnection = m_pConnection;
	m_pRecordSet.CreateInstance(_uuidof(Recordset));
	fileName.Format("%s",fileInfo.fileName);
    m_pCommand->CommandText ="select *from FragmentTable where fileName='"+(_bstr_t)fileName+"'";
	m_pRecordSet = m_pCommand->Execute(NULL,NULL,adCmdText);
	while(!m_pRecordSet->adoEOF){
		FragmentBean fragBean;
		fragBean.orderId = m_pRecordSet->GetCollect("fragmentId").intVal;
		fragBean.startIndex=m_pRecordSet->GetCollect("startIndex").lVal;
		fragBean.endIndex=m_pRecordSet->GetCollect("endIndex").lVal;
        fragBean.totalFragment=m_pRecordSet->GetCollect("totalFragment").intVal;
		fragBean.fileName = fileName;
		metaFragmentList.push_back(fragBean);
		m_pRecordSet->MoveNext();
	}
	m_pConnection->Close();
   return metaFragmentList;
}
CString DataSearchDao::getFilePathByName(CString fileName){
	CString filePath;
	DBConnection dataSource;
	m_pConnection = dataSource.getConnection();
	try{
		m_pRecordSet.CreateInstance(_uuidof(Recordset));
		CString sql;
		sql.Format("select *from FragmentTable where fileName='%s'",fileName);
		m_pRecordSet = m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
		while(!m_pRecordSet->adoEOF){
			filePath = (LPCTSTR)(_bstr_t)m_pRecordSet->GetCollect("filePath");
			break;
		}
	}catch(_com_error ex){
		CString errorMessage;
		errorMessage.Format("%s",ex.ErrorMessage());
		AfxMessageBox(errorMessage);
	}
	m_pConnection->Close();
	return filePath;
}
/************************************************************************/
/* 获取第i个分片在客户端的哪个位置                                      */
/************************************************************************/
list<FragmentBean> DataSearchDao::getClientFragmentDetail(FragmentStruct fragment){
	list<FragmentBean> clientFragmentList =  list<FragmentBean>();
	DBConnection dataSource;
	m_pConnection = dataSource.getConnection();
	try{
	m_pRecordSet.CreateInstance(_uuidof(Recordset));
	CString sql;
	sql.Format("select *from ClientFragmentTable where fileName='%s' and startIndex=%ld and endIndex=%ld and fragmentId=%d and status=1",fragment.fileName,fragment.startIndex,fragment.endIndex,fragment.orderId);
	m_pRecordSet = m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
	int i=0;
	while(!m_pRecordSet->adoEOF){
		FragmentBean fragmentBean;
		fragmentBean.fileName = fragment.fileName;
		fragmentBean.totalFragment = fragment.totalFragment;
		fragmentBean.orderId = fragment.orderId;
		fragmentBean.startIndex = fragment.startIndex;
		fragmentBean.endIndex = fragment.endIndex;
		fragmentBean.clientAddress =(LPCTSTR)(_bstr_t)m_pRecordSet->GetCollect("clientAddress");
		fragmentBean.clientPort = m_pRecordSet->GetCollect("clientPort").intVal;
		fragmentBean.natAddress = (LPCTSTR)(_bstr_t)m_pRecordSet->GetCollect("natAddress");
		fragmentBean.natPort = m_pRecordSet->GetCollect("natPort").intVal;
		clientFragmentList.push_back(fragmentBean);
		m_pRecordSet->MoveNext();
		i++;
		if(i>10)
			break;
	}
	}catch(_com_error ex){
		CString errorMessage;
		errorMessage.Format("%s",ex.ErrorMessage());
		AfxMessageBox(errorMessage);
	}
	m_pConnection->Close();
	return clientFragmentList;
}
/************************************************************************/
/* 给视频文件分段，并将分段信息写入到数据库中              */
/************************************************************************/
list<FragmentBean> DataSearchDao::generateMetaFragment(MetaFileInfo fileInfo){
	CFile localFile;
	long fileLength;
	CFileException ex;
	BOOL fileOpen =false;
	CString fileName = fileInfo.fileName;
	CString filePath = fileInfo.filePath;
	list<FragmentBean> fragmentList = list<FragmentBean>();
	if(!(fileOpen=localFile.Open((filePath+fileName),CFile::shareDenyNone|CFile::typeBinary,&ex))){		
		AfxMessageBox(filePath+fileName);
		return NULL;
	}
	fileLength = localFile.GetLength();
	long startIndex,endIndex;
	int totalFragment = fileLength/VariableConstant.fragment_longbytes;

	DBConnection dataSource;
	for(int i=1;i<=totalFragment;i++){
		FragmentBean fragmentBean;
		startIndex = (i-1)*VariableConstant.fragment_longbytes;
		endIndex = startIndex+VariableConstant.fragment_longbytes;
		if(i==totalFragment)
			endIndex+=fileLength%VariableConstant.fragment_longbytes;
		fragmentBean.startIndex = startIndex;
		fragmentBean.endIndex = endIndex;
		fragmentBean.fileName =fileName;
		fragmentBean.totalFragment=totalFragment;
		fragmentBean.orderId=i;
		fragmentBean.clientAddress = fileInfo.filePath;
		fragmentList.push_back(fragmentBean);
	
		try{
		  m_pConnection = dataSource.getConnection();
		  CString sql;
		  sql.Format("insert into FragmentTable (fileName,totalFragment,fragmentId,startIndex,endIndex,filePath)values('%s',%d,%d,%ld,%ld,'%s')",
			  fragmentBean.fileName,fragmentBean.totalFragment,fragmentBean.orderId,fragmentBean.startIndex,fragmentBean.endIndex,fragmentBean.clientAddress);
		  m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
		}catch(_com_error ex){
			CString errorMessage;
			errorMessage.Format("%s",ex.ErrorMessage());
			AfxMessageBox(errorMessage);
		}
    m_pConnection->Close();
	
	}
return fragmentList;
}
/************************************************************************/
/* 将客户端上传的分段信息传入到服务器中，请写入到数据库中               */
/************************************************************************/
void DataSearchDao::writeClientFragmentToBase(CString fileName){
	XMLDescriptionFile xmlDesFile;
	DBConnection dataSource;
	list<FragmentBean> fragmentList = xmlDesFile.getFragmentList(fileName);
	std::list<FragmentBean>::iterator it;
	m_pConnection = dataSource.getConnection();
	for(it = fragmentList.begin();it!=fragmentList.end();it++){
		FragmentBean fragmentBean = *it;
		try{
		  CString sql;
		  sql.Format("insert into ClientFragmentTable (fileName,totalFragment,fragmentId,startIndex,endIndex,clientAddress,clientPort,natAddress,natPort,status)values('%s',%d,%d,%ld,%ld,'%s',%d,'%s',%d,%d)",
			  fragmentBean.fileName,fragmentBean.totalFragment,fragmentBean.orderId,fragmentBean.startIndex,fragmentBean.endIndex,fragmentBean.clientAddress,fragmentBean.clientPort,fragmentBean.natAddress,fragmentBean.natPort,1);	 
		  m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
		}catch(_com_error ex){
			CString errorMessage;
			errorMessage.Format("%s",ex.ErrorMessage());
			AfxMessageBox(errorMessage);
		}
	}
	m_pConnection->Close();
}

/************************************************************************/
/* 当客户端退出时，从数据库中删除在线状态信息                */
/************************************************************************/
void DataSearchDao::deleteOnLineFragmentInfo(UserBean userBean){
	DBConnection dataSource;
	m_pConnection = dataSource.getConnection();
		try{
		CString sql;
		sql.Format("delete from ClientFragmentTable where natAddress='%s' and natPort=%d",userBean.getClientAddress(),userBean.getPort());
	     m_pConnection->Execute((_bstr_t)sql,NULL,adCmdText);
	}catch(_com_error ex){
		CString errorMessage;
		errorMessage.Format("%s",ex.ErrorMessage());
		AfxMessageBox(errorMessage);
	}
	m_pConnection->Close();
}
