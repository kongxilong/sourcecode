/*!
 @file		SingletonManagerCenter.h
 @author    kongxilong
 @param		Email: hzkongxilong@163.org
 @param		Copyright (c) 2016 
 @date		2016-6-5
 @brief		单例类管理中心
 */

#pragma once

#include "ISingleton.h"

class SingletonManagerCenter : public SingletonManagerBase
{

	
public:
	//声明为单例类
    DECLARE_SINGLETON_CLASS(SingletonManagerCenter)
};