/*!
 * @file   SqliteService.cpp
 * @author kongxilong
 * @param  Email: hzkongxilong@corp.netease.com
 * @param  Copyright(C) 2016 
 * @date   2016/06/05
 * @brief  数据库服务实现文件
 */

#include "..\stdafx.h"
#include "SqliteService.h"
#include "..\core\util\Common.h"
#include "..\core\SingletonIDDefine.h"
#include "..\sqlite\SqlDefine.h"

static  SqliteService* g_instance = NULL;
static  CLocker        g_locker;



SqliteService* SqliteService::GetInstance()
{
	if(g_instance == NULL)
	{
		g_locker.Lock();
		if (g_instance == NULL)
		{
			g_instance = new SqliteService();
		}
		g_locker.Unlock();
	}
	return g_instance;
}


void SqliteService::OnQuit()
{
	 m_db.Close();
}

void SqliteService::OnStart()
{
    CreanteDB();
}

void SqliteService::SafeRelease()
{
	SAFE_RELEASE(g_instance);
}

BOOL SqliteService::CreanteDB()
{
	CString directory = Common::GetAppPath();
	CString dbfile = directory + _T("/employ.db");

	BOOL result = FALSE;
	result = m_db.Open(Common::CString2String(dbfile).c_str(), SQLiteEx::modeReadWrite | SQLiteEx::modeCreate | SQLiteEx::modeSerialized);
    CString sSql;
	sSql.Format(_T("%s%s%s"),FindSqlByID(ENUM_CREATE_COMPANY_TABLE),
		FindSqlByID(ENUM_CREATE_DEPARTMENT_TABLE),
		FindSqlByID(ENUM_CREATE_EMPLOY_TABLE));
	int dbresult = m_db.Query(Common::CString2String(sSql).c_str());

	if(result && dbresult == SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		if(dbresult != SQLITE_OK)
		{
			Common::OutputDebugInfo(_T("Create employ table failed,errcode = %d"),dbresult);
		}

		return FALSE;
	}

	return FALSE;

}

int SqliteService::GetSingletonID()
{
	return STID_SQLITESV;
}

CString  SqliteService::FindSqlByID(const int& id)
{
    CString dest(_T(""));
	int size = DIMENSION(SqlMap);
	if(id >= 0 && id <= size-1)
	{
		dest = SqlMap[id].sql;
	}
	return dest;
}

BOOL SqliteService::IsComanyExist(const CString& company) 
{
	BOOL ret = FALSE;
	m_locker.Lock();
	SQLiteStatement stmt;
	CString query_sql;
	CString sSql = FindSqlByID(ENUM_SELECT_COMPANY);
	query_sql.Format(sSql,company);
	m_db.Query(stmt, Common::CString2String(query_sql).c_str());

	uint32_t db_resutl = stmt.NextRow();
	if(db_resutl == SQLITE_OK || db_resutl == SQLITE_ROW)
	{
		ret = true;
	}
	m_locker.Unlock();
	return ret;
}

BOOL SqliteService::NewComany(const CString& company) 
{
	BOOL result = TRUE;
	CString sSql = FindSqlByID(ENUM_INSERT_NEW_COMPANY);
	m_locker.Lock();
	m_db.TransactionBegin(); //立即执行
	SQLiteStatement stmt;
	m_db.Query(stmt, Common::CString2String(sSql).c_str());
	m_db.TransactionReset(stmt);
	stmt.BindText(1, Common::CString2String(company).c_str(), company.GetLength());
	m_db.TransactionStep(stmt);
	stmt.Finalize();
	m_db.TransactionCommit();
	
	m_locker.Unlock();
	return result;
}

BOOL SqliteService::RemoveComany(const CString& company) 
{
    int64_t id;
	FindComidByComName(company,id);
	BOOL result = TRUE;
	CString sql = FindSqlByID(ENUM_DELETE_COMPANY);
    m_locker.Lock();
	m_db.TransactionBegin(); //立即执行
	SQLiteStatement stmt;
	m_db.TransactionQuery(stmt, Common::CString2String(sql).c_str(), -1);
	m_db.TransactionReset(stmt);
	stmt.BindInt64(1,id);
	stmt.BindInt64(2,id);
	stmt.BindInt64(3,id);
	m_db.TransactionStep(stmt);
	stmt.Finalize();
	m_db.TransactionCommit();
	m_locker.Unlock();
	return result;
}

BOOL SqliteService::IsDepartmentExist(const int64_t& companyid,const CString& depart) 
{
	BOOL ret = FALSE;
	m_locker.Lock();
	SQLiteStatement stmt;
	CString query_sql;
	CString sSql = FindSqlByID(ENUM_SELECT_DEPARTMENT_FROM_TABEL_DEPARTMENT);
	query_sql.Format(sSql,depart,companyid);
	m_db.Query(stmt, Common::CString2String(query_sql).c_str());
	uint32_t db_resutl = stmt.NextRow();
	if(db_resutl == SQLITE_OK || db_resutl == SQLITE_ROW)
	{
		ret = true;
	}
	m_locker.Unlock();
	return ret;
}

BOOL SqliteService::NewDepartment(const int& companyid,const CString& depart) 
{
	BOOL result = TRUE;
	CString sSql = FindSqlByID(ENUM_INSERT_NEW_DEPARTMENT);
	m_locker.Lock();
	m_db.TransactionBegin(); //立即执行
	SQLiteStatement stmt;
	m_db.Query(stmt, Common::CString2String(sSql).c_str());
	m_db.TransactionReset(stmt);
	stmt.BindText(1, Common::CString2String(depart).c_str(), depart.GetLength());
	stmt.BindInt64(2, companyid);
	m_db.TransactionStep(stmt);
	stmt.Finalize();
	m_db.TransactionCommit();

	m_locker.Unlock();
	return result;
}

BOOL SqliteService::RemoveDepartment(const int64_t& companyid,const CString& depart)
{
	int64_t id;
	FindDepartmentIDByDepartmentName(companyid,depart,id);
	BOOL result = TRUE;
	CString sql = FindSqlByID(ENUM_DELETE_DEPARTMENT_FROM_TABEL_DEPARTMENT);
	m_locker.Lock();
	m_db.TransactionBegin(); //立即执行
	SQLiteStatement stmt;
	m_db.TransactionQuery(stmt, Common::CString2String(sql).c_str(), -1);
	m_db.TransactionReset(stmt);
	stmt.BindInt64(1, id);
	stmt.BindText(2, Common::CString2String(depart).c_str(), depart.GetLength());
	stmt.BindInt64(3, companyid);
	m_db.TransactionStep(stmt);
	stmt.Finalize();
	m_db.TransactionCommit();
	m_locker.Unlock();
	return result;
}

BOOL SqliteService::IsEmployExist(const int64_t& companyid,const int64_t& departid,const CString& employ)
{
	std::list<Employe> employs;
	QueryAllEmploye(employs);
	std::list<Employe>::iterator it = employs.begin();
	for(; it != employs.end(); ++it)
	{
		if(it->comid == companyid
			&& it->departid == departid
			&& it->name == employ)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL SqliteService::NewEmploy(const int64_t& companyid,const int64_t& departid,const CString& employ)
{
	BOOL result = TRUE;
	CString sSql = FindSqlByID(ENUM_INSERT_NEW_EMPLOY);
	m_locker.Lock();
	m_db.TransactionBegin(); //立即执行
	SQLiteStatement stmt;
	m_db.Query(stmt, Common::CString2String(sSql).c_str());
	m_db.TransactionReset(stmt);
	stmt.BindText(1, Common::CString2String(employ).c_str(), employ.GetLength());
	stmt.BindInt64(2, departid);
	stmt.BindInt64(3, companyid);
	m_db.TransactionStep(stmt);
	stmt.Finalize();
	m_db.TransactionCommit();

	m_locker.Unlock();
	return result;
}

BOOL SqliteService::RemoveEmploy(const int64_t& companyid,const int64_t& departid,,const CString& employ)
{
	BOOL result = TRUE;
	CString sql = FindSqlByID(ENUM_DELETE_EMPLOY_FROM_TABEL_EMPLOY);
	m_locker.Lock();
	m_db.TransactionBegin(); //立即执行
	SQLiteStatement stmt;
	m_db.TransactionQuery(stmt, Common::CString2String(sql).c_str(), -1);
	m_db.TransactionReset(stmt);
	stmt.BindText(1, Common::CString2String(employ).c_str(), employ.GetLength());
	stmt.BindInt64(2, departid);
	stmt.BindInt64(3, companyid);
	m_db.TransactionStep(stmt);
	stmt.Finalize();
	m_db.TransactionCommit();
	m_locker.Unlock();
	return result;
}

BOOL SqliteService::QueryAllCompany(std::list<Company>& companys)
{
    companys.clear();
	CString sSql = FindSqlByID(ENUM_SELECT_ALL_COMPANY);

	m_locker.Lock();
	SQLiteStatement stmt;
	m_db.Query(stmt, Common::CString2String(sSql).c_str());
	uint32_t db_result = stmt.NextRow();
	while (db_result == SQLITE_ROW) {
		int64_t id = stmt.GetInt64Field(0);
		CString s = Common::String2CString(stmt.GetTextField(1));
        if(s.GetLength()>0)
		{
			companys.push_back(Company(id,s));
		}
		db_result = stmt.NextRow();
	}
	m_locker.Unlock();
	return TRUE;
}

BOOL SqliteService::QueryAllEmploye(std::list<Employe>& employes)
{
	employes.clear();
	CString sSql = FindSqlByID(ENUM_SELECT_ALL_EMPLOE);

	m_locker.Lock();
	SQLiteStatement stmt;
	m_db.Query(stmt, Common::CString2String(sSql).c_str());
	uint32_t db_result = stmt.NextRow();
	while (db_result == SQLITE_ROW) {
		int64_t id = stmt.GetInt64Field(0);
		CString name = Common::String2CString(stmt.GetTextField(1));
		int64_t departid = stmt.GetInt64Field(2);
		int64_t comid = stmt.GetInt64Field(3);
		employes.push_back(Employe(id,name,departid,comid));
		db_result = stmt.NextRow();
	}
	m_locker.Unlock();
	return TRUE;
}

BOOL SqliteService::QueryAllDepartment(std::list<Department>& departs)
{
	departs.clear();
	CString sSql = FindSqlByID(ENUM_SELECT_ALL_DEPART);

	m_locker.Lock();
	SQLiteStatement stmt;
	m_db.Query(stmt, Common::CString2String(sSql).c_str());
	uint32_t db_result = stmt.NextRow();
	while (db_result == SQLITE_ROW) {
		int64_t id = stmt.GetInt64Field(0);
		CString name = Common::String2CString(stmt.GetTextField(1));
		int64_t comid = stmt.GetInt64Field(2);
		departs.push_back(Department(id,name,comid));
		db_result = stmt.NextRow();
	}
	m_locker.Unlock();
	return TRUE;
}

BOOL SqliteService::FindComidByComName(const CString& name, int64_t& id)
{
	std::list<Company> coms;
	QueryAllCompany(coms);
	id = -1;
	std::list<Company>::iterator it = coms.begin();
	for(; it != coms.end(); ++it)
	{
		if(it->name == name)
		{
			id = it->comid;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL SqliteService::FindDepartmentIDByDepartmentName(const int64_t& comid,const CString& name, int64_t& departid)
{
	std::list<Department> departs;
	QueryAllDepartment(departs);
	departid = -1;
	std::list<Department>::iterator it = departs.begin();
	for(; it != departs.end(); ++it)
	{
		if(it->name == name && it->comid == comid)
		{
			departid = it->departid;
			return TRUE;
		}
	}
	return FALSE;
}