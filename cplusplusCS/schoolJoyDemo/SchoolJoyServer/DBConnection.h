// DBConnection.h: interface for the DBConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCONNECTION_H__1321D432_A8EF_4B4E_9506_5D703CD5B29A__INCLUDED_)
#define AFX_DBCONNECTION_H__1321D432_A8EF_4B4E_9506_5D703CD5B29A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class DBConnection  
{
public:
	DBConnection();
	virtual ~DBConnection();
	_ConnectionPtr getConnection();
private:
	_ConnectionPtr m_pConnection;

};

#endif // !defined(AFX_DBCONNECTION_H__1321D432_A8EF_4B4E_9506_5D703CD5B29A__INCLUDED_)
