#ifndef ADOEXCEL_H_H_H_H
#define ADOEXCEL_H_H_H_H

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF","adoEOF")
#pragma once
class CADOExcel
{
public:
	CADOExcel(void);
	~CADOExcel(void);
public:
	_ConnectionPtr	m_pConnection;
	_RecordsetPtr	m_pRecordset;
	CString		m_path;   //excel 路径
	CString		m_fileType;   //文件的类型 Excel 2003 或者Excel 2007
public:
	void SetExcelFilePath(CString pathOfExcel);
	void SetFileType(CString fileType);
	BOOL OnInitConnect();
	_RecordsetPtr& GetRecordSet(CString str);
	BOOL		ExecuteSQL(CString str);
	void		ExitConnect();
};

#endif