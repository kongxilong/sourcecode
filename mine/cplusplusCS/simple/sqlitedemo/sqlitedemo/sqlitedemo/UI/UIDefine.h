/*!
 @file		UIDefine.h
 @author    kongxilong
 @param		Email: hzkongxilong@163.org
 @param		Copyright (c) 2016 
 @date		2016-6-10
 @brief	    一些UI的控件ID设定
 */


#pragma once

#define ID_BTN_NEW_DB        100
#define ID_TREE_DB           101
#define ID_BTN_OPERATION     102


enum
{
	 ENUM_COMMAND_NONE = 0,
     ENUM_COMMAND_NEW_COMPANY ,
	 ENUM_COMMAND_DELETE_COMPANY,
	 ENUM_COMMAND_NEW_DEPARTMENT,
	 ENUM_COMMAND_DELETE_DEPARTMENT,
	 ENUM_COMMAND_NEW_EMPLOY,
	 ENUM_COMMAND_DELETE_EMPLOY,
};

struct DB_Operation
{
	int commandID;
	CString sComanyName;
	CString sDepartmentName;
	CString sEmployName;
    
	DB_Operation()
	{
		commandID = ENUM_COMMAND_NONE;
		sComanyName = _T("");
		sDepartmentName = _T("");
		sEmployName = _T("");
	}

	DB_Operation(int commandID,CString companyName,CString departmentName,CString employName)
	{
		this->commandID = commandID;
		this->sComanyName = companyName;
		this->sDepartmentName = departmentName;
		this->sEmployName = employName;
	}

	BOOL operator==(const DB_Operation& other)
	{
		if(this->commandID == other.commandID
			&& this->sComanyName == other.sComanyName
			&& this->sDepartmentName == other.sDepartmentName
			&& this->sEmployName == other.sEmployName)
		{
			return TRUE;
		}
		return FALSE;
	}

};