// XMLDescriptionFile.cpp: implementation of the XMLDescriptionFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadServer.h"
#include "XMLDescriptionFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XMLDescriptionFile::XMLDescriptionFile()
{

}

XMLDescriptionFile::~XMLDescriptionFile()
{

}
/************************************************************************/
/* 判断文件分片描述的XML元数据是否存在                                                                     */
/************************************************************************/
bool XMLDescriptionFile::fileIsExist(MetaFileInfo metaInfo){
	CString fileInfo = getXMLFileName(metaInfo);
	CFile tempFile;
	CFileException ex;
	if(!tempFile.Open(fileInfo,CFile::modeRead|CFile::typeBinary,&ex)){
		writeXMLFragmentToFile(metaInfo,fileInfo);
		return false;
	}
	else
		tempFile.Close();
		return true;
}

/************************************************************************/
/* 若上述文件分片描述的XML元数据不存在，则将其写入到文件中                                                                     */
/************************************************************************/
void XMLDescriptionFile::writeXMLFragmentToFile(MetaFileInfo metaFile,CString fileInfo){
	CString fileName = metaFile.filePath;
	fileName+= metaFile.fileName;
	CFile localFile;
	long fileLength;
	CFileException ex;
	BOOL fileOpen =false;
	if(!(fileOpen=localFile.Open((fileName),CFile::modeRead|CFile::typeBinary,&ex))){		
		AfxMessageBox(fileInfo);
		return;
	}
	//声明XML文件的相关信息
	IXMLDOMDocumentPtr pDoc = NULL; // xml文档
    IXMLDOMProcessingInstructionPtr pProInstruction = NULL; // xml声明
    IXMLDOMElementPtr pRootElement = NULL, pElement = NULL,pChildElement=NULL; // 根节点(元素)
    IXMLDOMNodePtr pNode = NULL;
	IXMLDOMAttributePtr pAttrNode = NULL; // 属性
	CoInitialize(NULL);
	HRESULT hr;
	hr = pDoc.CreateInstance(__uuidof(DOMDocument));
	if(FAILED(hr)){
		return;
	}
	pProInstruction = pDoc->createProcessingInstruction((_bstr_t)(char*)"xml", (_bstr_t)(char*)"version=\"1.0\" encoding=\"utf-8\"");
    pDoc->appendChild((IXMLDOMNode*)pProInstruction);
	pRootElement = pDoc->createElement("FragmentList");
	if(pRootElement!=NULL){
		pDoc->appendChild(pRootElement);
	}
	fileLength = localFile.GetLength();
	long startIndex,endIndex;
	int totalFragment = fileLength/VariableConstant.fragment_longbytes;
	for(int i=1;i<=totalFragment;i++){
		startIndex = (i-1)*VariableConstant.fragment_longbytes;
		endIndex = startIndex+VariableConstant.fragment_longbytes;
		if(i==totalFragment)
			endIndex+=fileLength%VariableConstant.fragment_longbytes;

		pElement = pDoc->createElement("fragment");
		pAttrNode =pDoc->createAttribute("fileName");
		if(pAttrNode!=NULL){
			pAttrNode->Puttext((_bstr_t)metaFile.fileName);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		
		pAttrNode =pDoc->createAttribute("fragmentId");
		if(pAttrNode!=NULL){
			CString temp;
			temp.Format("%d",i);
			pAttrNode->Puttext((_bstr_t)temp);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}

		pAttrNode =pDoc->createAttribute("startIndex");
		if(pAttrNode!=NULL){
			pAttrNode->Puttext((_bstr_t)startIndex);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}

		pAttrNode =pDoc->createAttribute("totalFragment");
		if(pAttrNode!=NULL){
			CString temp;
			temp.Format("%d",totalFragment);
			pAttrNode->Puttext((_bstr_t)temp);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		pAttrNode = pDoc->createAttribute("endIndex");
		if(pAttrNode!=NULL){
			pAttrNode->Puttext((_bstr_t)endIndex);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		pRootElement->appendChild(pElement);
		pElement.Release();
	}
	//end of for loop;
	hr= pDoc->save((_variant_t)fileInfo);
	if(!FAILED(hr))
		pDoc.Release();
	if(fileOpen){
		localFile.Close();
	}
	
}
/************************************************************************/
/* 将请求的某个分片的详细信息写入到本地                                 */
/************************************************************************/
void XMLDescriptionFile::writeDetailFragmentToFile(list<FragmentBean> fragmentList,FragmentStruct fragmentBean){
	CString filePath = getXmlFilePath(fragmentBean.fileName);

	//声明XML文件的相关信息
	IXMLDOMDocumentPtr pDoc = NULL; // xml文档
    IXMLDOMProcessingInstructionPtr pProInstruction = NULL; // xml声明
    IXMLDOMElementPtr pRootElement = NULL, pElement = NULL,pChildElement=NULL; // 根节点(元素)
    IXMLDOMNodePtr pNode = NULL;
	IXMLDOMAttributePtr pAttrNode = NULL; // 属性
	CoInitialize(NULL);
	HRESULT hr;
	hr = pDoc.CreateInstance(__uuidof(DOMDocument));
	if(FAILED(hr)){
		return;
	}
	pProInstruction = pDoc->createProcessingInstruction((_bstr_t)(char*)"xml", (_bstr_t)(char*)"version=\"1.0\" encoding=\"utf-8\"");
    pDoc->appendChild((IXMLDOMNode*)pProInstruction);
	pRootElement = pDoc->createElement("FragmentList");
	if(pRootElement!=NULL)
		pDoc->appendChild(pRootElement);
	std::list<FragmentBean>::iterator it;
	for(it = fragmentList.begin();it!=fragmentList.end();it++){
		FragmentBean fragmentBean = *it;
		pElement = pDoc->createElement("fragment");

		pAttrNode =pDoc->createAttribute("fileName");
		if(pAttrNode!=NULL){
			pAttrNode->Puttext((_bstr_t)fragmentBean.fileName);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		
		pAttrNode =pDoc->createAttribute("fragmentId");
		if(pAttrNode!=NULL){
			CString temp;
			temp.Format("%d",fragmentBean.orderId);
			pAttrNode->Puttext((_bstr_t)temp);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		pAttrNode =pDoc->createAttribute("totalFragment");
		if(pAttrNode!=NULL){
			CString temp;
			temp.Format("%d",fragmentBean.totalFragment);
			pAttrNode->Puttext((_bstr_t)temp);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}

		pAttrNode =pDoc->createAttribute("startIndex");
		if(pAttrNode!=NULL){
			CString temp;
			temp.Format("%d",fragmentBean.startIndex);
			pAttrNode->Puttext((_bstr_t)temp);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		
		pAttrNode = pDoc->createAttribute("endIndex");
		if(pAttrNode!=NULL){
			CString temp;
			temp.Format("%d",fragmentBean.endIndex);
			pAttrNode->Puttext((_bstr_t)temp);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		pAttrNode = pDoc->createAttribute("clientAddress");
		if(pAttrNode!=NULL){
			pAttrNode->Puttext((_bstr_t)fragmentBean.clientAddress);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		pAttrNode = pDoc->createAttribute("clientPort");
		if(pAttrNode!=NULL){
			CString temp;
			temp.Format("%d",fragmentBean.clientPort);
			pAttrNode->Puttext((_bstr_t)temp);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
			pAttrNode = pDoc->createAttribute("natAddress");
		if(pAttrNode!=NULL){
			pAttrNode->Puttext((_bstr_t)fragmentBean.natAddress);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}
		pAttrNode = pDoc->createAttribute("natPort");
		if(pAttrNode!=NULL){
			CString temp;
			temp.Format("%d",fragmentBean.natPort);
			pAttrNode->Puttext((_bstr_t)temp);
			pElement->setAttributeNode(pAttrNode);
			pAttrNode.Release();
		}

		pRootElement->appendChild(pElement);
		pElement.Release();
	}
	hr= pDoc->save((_variant_t)filePath);
	if(!FAILED(hr))
		pDoc.Release();
}
CString XMLDescriptionFile::getXMLFileName(MetaFileInfo metaInfo){
	CString fileName = metaInfo.fileName;
	CString filePath = metaInfo.filePath;
	CString tempInfo = filePath+fileName;
	int index=tempInfo.Find(".");
	return tempInfo.Left(index)+".xml";
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString XMLDescriptionFile::getXmlFilePath(CString fileName){
	int index = fileName.Find(".");
	CString filePath = VariableConstant.xml_file_path+fileName.Left(index)+".xml";
	return filePath;
}
CFile XMLDescriptionFile::getmeteFragmentDescription(MetaFileInfo metaInfo){
  CFile xmlFile;
  CString filePath = getXMLFileName(metaInfo);
  CFileException ex;
  if(xmlFile.Open(filePath,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary,&ex)){
	  return NULL;
  }
  return xmlFile;
}
CFile XMLDescriptionFile::readXMLFile(FragmentStruct fragment){
	CFile xmlFile;
	CString filePath = fragment.fileName;
	CFileException ex;
	if(xmlFile.Open(filePath,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary,&ex)){
		return NULL;
	}
	return xmlFile;
}

/************************************************************************/
/* 获取从客户端上传的所有更新分段信息                                 */
/************************************************************************/
list<FragmentBean> XMLDescriptionFile::getFragmentList(CString filePath){
	list<FragmentBean> fragmentList = list<FragmentBean>();
	CoInitialize(NULL);
	HRESULT hr;
	IXMLDOMDocumentPtr pDoc = NULL; // xml文档
    IXMLDOMProcessingInstructionPtr pProInstruction = NULL; // xml声明
    IXMLDOMElementPtr pRootElement = NULL, pElement = NULL,pChildElement=NULL; // 根节点(元素)
    IXMLDOMNodePtr pNode = NULL,childNode=NULL,serverNameNode = NULL, portNode = NULL; // 节点	
    IXMLDOMAttributePtr pAttrNode = NULL; // 属性
	IXMLDOMNodeListPtr pNodeList = NULL,childList=NULL; // 节点链表
	IXMLDOMNamedNodeMapPtr pAttrList = NULL; // 属性链表
	hr = pDoc.CreateInstance(__uuidof(DOMDocument));
	if(FAILED(hr)){
		AfxMessageBox("加载文件失败");
		return fragmentList;
	}
	VARIANT_BOOL loadFile = pDoc->load((_variant_t)filePath);
	if(!loadFile){
		AfxMessageBox("加载从客户端上传的分段信息文件失败\r\n");
		return fragmentList;
	}
	pRootElement = pDoc->GetdocumentElement();
	pNodeList = pRootElement->GetchildNodes();
	int childNumbers = pNodeList->Getlength();
	for(int i=0;i<childNumbers;i++){
		FragmentBean fragmentBean;
		pNode = pNodeList->Getitem(i);
		pAttrList = pNode->Getattributes();
		for(int j=0;j<pAttrList->Getlength();j++){
			childNode= pAttrList->Getitem(j);
			CString type = (LPCTSTR)childNode->GetnodeName();
			
			if(type=="fileName"){
				fragmentBean.fileName= (LPCTSTR)(_bstr_t)childNode->GetnodeValue();
			}
			if(type =="fragmentId"){
				CString orderId= (LPCTSTR)(_bstr_t)childNode->GetnodeValue();
				fragmentBean.orderId=_ttoi(orderId);
			}
			if(type=="startIndex"){
				fragmentBean.startIndex=(childNode->GetnodeValue());
			}
			if(type=="totalFragment"){
				fragmentBean.totalFragment=childNode->GetnodeValue();
			}
			if(type=="endIndex"){
				fragmentBean.endIndex=(childNode->GetnodeValue());
			}
			if(type=="clientAddress"){
				fragmentBean.clientAddress=(LPCTSTR)(_bstr_t)(childNode->GetnodeValue());
			}
			
			if(type=="clientPort"){
				CString port= (LPCTSTR)(_bstr_t)childNode->GetnodeValue();
				fragmentBean.clientPort=_ttoi(port);
			}
				if(type=="natAddress"){
				fragmentBean.natAddress=(LPCTSTR)(_bstr_t)(childNode->GetnodeValue());
			}
			if(type=="natPort"){
				CString port= (LPCTSTR)(_bstr_t)childNode->GetnodeValue();
				fragmentBean.natPort=_ttoi(port);
			}
		}
		fragmentList.push_back(fragmentBean);
	}
	if(!FAILED(hr))
		pDoc.Release();
	return fragmentList;
}
