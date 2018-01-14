// XMLDescriptionFile.h: interface for the XMLDescriptionFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLDESCRIPTIONFILE_H__908DC360_1831_4B73_8E16_6CFC7F7142FC__INCLUDED_)
#define AFX_XMLDESCRIPTIONFILE_H__908DC360_1831_4B73_8E16_6CFC7F7142FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MetaFileInfo.h"
#include "VariableConstant.h"
#include "FragmentBean.h"
#include "FragmentStruct.h"
#import <msxml4.dll>
#include <list>
using std::list;
using namespace MSXML2;
class XMLDescriptionFile  
{
public:
	XMLDescriptionFile();
	virtual ~XMLDescriptionFile();
    bool fileIsExist(MetaFileInfo metaInfo);
	CFile getmeteFragmentDescription(MetaFileInfo metaInfo);
	void writeXMLFragmentToFile(MetaFileInfo metaInfo,CString fileInfo);
	CString getXMLFileName(MetaFileInfo fileInfo);
	CString getXmlFilePath(CString fileName);
	void writeDetailFragmentToFile(list<FragmentBean> fragmentList,FragmentStruct fragmentBean);
	CFile readXMLFile(FragmentStruct fragment);
	list<FragmentBean> getFragmentList(CString filePath);//根据文件的路径，获取所有的信息
};
#endif // !defined(AFX_XMLDESCRIPTIONFILE_H__908DC360_1831_4B73_8E16_6CFC7F7142FC__INCLUDED_)
