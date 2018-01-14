/*!
 * @file   SqliteService.h
 * @author kongxilong
 * @param  Email: hzkongxilong@corp.netease.com
 * @param  Copyright(C) 2016 
 * @date   2016/06/05
 * @brief  数据库服务实现文件
 */

#pragma once

#include "ISqliteService.h"
#include "..\sqlite\sqlite3Ex.h"

class SqliteService : public ISqliteService
{

public:
	//单例类的接口
    void OnStart();
	void OnQuit();
    int  GetSingletonID();
    
	BOOL CreanteDB();
    
    CString  FindSqlByID(const int& id);

public:
	virtual BOOL QueryAllCompany(std::list<Company>& companys);
	virtual BOOL IsComanyExist(const CString& company) ;
	virtual BOOL NewComany(const CString& company) ;
	virtual BOOL RemoveComany(const CString& company) ;
	virtual BOOL FindComidByComName(const CString& name, int64_t& id);
	virtual BOOL FindDepartmentIDByDepartmentName(const int64_t& comid,const CString& name, int64_t& departid);
	virtual BOOL QueryAllDepartment(std::list<Department>& departs);
	virtual BOOL IsDepartmentExist(const int64_t& companyid,const CString& depart) ;
	virtual BOOL NewDepartment(const int64_t& companyid,const CString& depart) ;
	virtual BOOL RemoveDepartment(const int64_t& companyid,const CString& depart) ;
	virtual BOOL QueryAllEmploye(std::list<Employe>& employes);
	virtual BOOL IsEmployExist(const int64_t& companyid,const int64_t& departid,const CString& employ) ;
	virtual BOOL NewEmploy(const int64_t& companyid,const int64_t& departid,const CString& employ);
	virtual BOOL RemoveEmploy(const int64_t& companyid,const int64_t& departid,,const CString& employ);
    
	DECLARE_SINGLETON_CLASS(SqliteService)

private:
	SQLiteEx                   m_db;            //数据库对象
	CLocker                    m_locker;        //锁
};