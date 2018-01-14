/*!
 * @file   DBModel.h
 * @author kongxilong
 * @param  Email: hzkongxilong@corp.netease.com
 * @param  Copyright(C) 2016 
 * @date   2016/06/11
 * @brief  数据库模型
 */

#pragma once 

struct Company
{
	int64_t comid;
	CString name;
	Company()
	{
		comid = -1;
		name = _T("");
	}
	Company(int64_t comid,CString name)
	{
		this->comid = comid;
		this->name = name;
	}

	BOOL operator==(const Department& other)
	{
		if(this->name == other.name
			&& this->comid == other.comid)
		{
			return TRUE;
		}
		return FALSE;
	}
};

struct Department
{
	int64_t departid;
	CString name;
	int64_t comid;
    Department()
	{
        departid = -1;
		name = _T("");
		comid = -1;
	}
	Department(	int64_t departid,CString name,int64_t comid)
	{
		this->departid = departid;
		this->name = name;
		this->comid = comid;
	}

	BOOL operator==(const Department& other)
	{
		if(this->departid == other.departid
			&& this->name == other.name
			&& this->comid == other.comid)
		{
			return TRUE;
		}
		return FALSE;
	}
};

struct Employe
{
	int64_t employid;
	int64_t departid;
	CString name;
	int64_t comid;
	Employe(int64_t employid,CString name,int64_t departid,int64_t comid)
	{
		this->employid = employid;
		this->departid = departid;
		this->name = name;
		this->comid = comid;
	}
	Employe()
	{
		this->employid = -1;
		this->departid = -1;
		this->name = _T("");
		this->comid = -1;
	}
	BOOL operator==(const Employe& other)
	{
		if(this->employid == other.employid
			&& this->departid == other.departid
			&& this->name == other.name
			&& this->comid == other.comid)
		{
			return TRUE;
		}
		return FALSE;
	}
};



