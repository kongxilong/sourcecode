// DataSearchDao.h: interface for the DataSearchDao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASEARCHDAO_H__A1B83024_299D_4D89_B145_5E293AFD3FCF__INCLUDED_)
#define AFX_DATASEARCHDAO_H__A1B83024_299D_4D89_B145_5E293AFD3FCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FragmentBean.h"
#include<list>
#include "MetaFileInfo.h"
#include "DBConnection.h"
#include "FragmentBean.h"
#include "VariableConstant.h"
#include "FragmentStruct.h"
#include "XMLDescriptionFile.h"
#include "UserBean.h"
using std::list;
class DataSearchDao  
{
public:
	DataSearchDao();
	virtual ~DataSearchDao();
    list<FragmentBean> getMetaFragments(MetaFileInfo fileInfo);
	list<FragmentBean> generateMetaFragment(MetaFileInfo fileInfo);
	list<FragmentBean> getClientFragmentDetail(FragmentStruct fragment);//获取第i个分片的详细位置
	CString getFilePathByName(CString fileName);
	void writeClientFragmentToBase(CString fileName);
	void deleteOnLineFragmentInfo(UserBean userBean);
private:
		_RecordsetPtr m_pRecordSet;
		_CommandPtr  m_pCommand;
		_ConnectionPtr m_pConnection;
};

#endif // !defined(AFX_DATASEARCHDAO_H__A1B83024_299D_4D89_B145_5E293AFD3FCF__INCLUDED_)
