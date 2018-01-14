
/*!
 @file		SqlDefine.h
 @author    kongxilong
 @param		Email: hzkongxilong@163.org
 @param		Copyright (c) 2016 
 @date		2016-6-10
 @brief	    sql语句的定义
 */

#pragma once

//数组的维度
#define DIMENSION(X)  (sizeof(X)/sizeof(X[0]))


/**************以下为sql语句定义*******************/

const CString sSqlCreateCompanyTable = _T("CREATE TABLE IF NOT EXISTS company(comid INTEGER PRIMARY KEY, name TEXT);");
const CString sSqlCreateDepartmentTable = _T("CREATE TABLE IF NOT EXISTS department(departid INTEGER PRIMARY KEY, name TEXT, comid INTEGER);");
const CString sSqlCreateEmployTable = _T("CREATE TABLE IF NOT EXISTS employ(employid INTEGER PRIMARY KEY, name TEXT, departid INTEGER,comid INTEGER);");
const CString sSqlSelectCompany = _T("SELECT * FROM company WHERE name = %s;");
const CString sSqlSelectAllCompany = _T("SELECT * FROM company;");
const CString sSqlInsertNewCompany = _T("REPLACE INTO company(comid, name) VALUES (NULL, ?);");
const CString sSqlDeleteCompany = _T("DELETE FROM employ WHERE comid = ?;DELETE FROM department WHERE comid = ?;DELETE FROM company WHERE comid = ?;");
const CString sSqlSelectDepartFromTabelDepartment = _T("SELECT * FROM department WHERE name = %s,comid = %d;");
const CString sSqlInsertNewDepartment = _T("REPLACE INTO department(departid,name,comid) VALUES (NULL, ?,?);");
const CString sSqlDeleteDepartmentFromDepartment = _T("DELETE FROM employ WHERE departid = ?;DELETE FROM department WHERE name = ? AND comid = ?;");
const CString sSqlSelectAllEmploye = _T("SELECT * FROM employ;");
const CString sSqlSelectAllDepart = _T("SELECT * FROM department;");
const CString sSqlInsertNewEmploy = _T("REPLACE INTO employ(employid,name,departid,comid) VALUES (NULL, ?,?,?);");
const CString sSqlDeleteEmployFromEmploy = _T("DELETE FROM employ WHERE name = ? AND departid=? AND comid = ?;");

/**************以上为sql语句定义*******************/



/**************以下为sql语句ID定义*******************/
enum 
{
	ENUM_CREATE_COMPANY_TABLE = 0,
    ENUM_CREATE_DEPARTMENT_TABLE = 1,
	ENUM_CREATE_EMPLOY_TABLE = 2,
	ENUM_SELECT_COMPANY = 3,
	ENUM_SELECT_ALL_COMPANY = 4,
	ENUM_INSERT_NEW_COMPANY = 5,
	ENUM_DELETE_COMPANY = 6,
	ENUM_SELECT_DEPARTMENT_FROM_TABEL_DEPARTMENT = 7,
	ENUM_INSERT_NEW_DEPARTMENT = 8,
	ENUM_DELETE_DEPARTMENT_FROM_TABEL_DEPARTMENT = 9,
	ENUM_SELECT_ALL_EMPLOE = 10,
	ENUM_SELECT_ALL_DEPART = 11,
	ENUM_INSERT_NEW_EMPLOY = 12,
	ENUM_DELETE_EMPLOY_FROM_TABEL_EMPLOY = 13,
};

/**************以上为sql语句ID定义*******************/


typedef struct _SqlStruct
{
	int id;
	CString  sql;
}SqlStruct;

SqlStruct SqlMap[]=
{
	{ ENUM_CREATE_COMPANY_TABLE,sSqlCreateCompanyTable},
	{ ENUM_CREATE_DEPARTMENT_TABLE,sSqlCreateDepartmentTable},
	{ ENUM_CREATE_EMPLOY_TABLE,sSqlCreateEmployTable},
	{ ENUM_SELECT_COMPANY,sSqlSelectCompany},
	{ ENUM_SELECT_ALL_COMPANY,sSqlSelectAllCompany},
	{ ENUM_INSERT_NEW_COMPANY,sSqlInsertNewCompany},
	{ ENUM_DELETE_COMPANY,sSqlDeleteCompany},
	{ ENUM_SELECT_DEPARTMENT_FROM_TABEL_DEPARTMENT,sSqlSelectDepartFromTabelDepartment},
	{ ENUM_INSERT_NEW_DEPARTMENT,sSqlInsertNewDepartment},
	{ ENUM_DELETE_DEPARTMENT_FROM_TABEL_DEPARTMENT,sSqlDeleteDepartmentFromDepartment},
	{ ENUM_SELECT_ALL_EMPLOE,sSqlSelectAllEmploye},
	{ ENUM_SELECT_ALL_DEPART,sSqlSelectAllDepart},
	{ ENUM_INSERT_NEW_EMPLOY,sSqlInsertNewEmploy},
	{ ENUM_DELETE_EMPLOY_FROM_TABEL_EMPLOY ,sSqlDeleteEmployFromEmploy},
};


