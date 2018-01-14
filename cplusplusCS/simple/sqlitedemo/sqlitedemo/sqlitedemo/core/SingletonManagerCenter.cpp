/*!
 @file		SingletonManagerCenter.cpp
 @author    kongxilong
 @param		Email: hzkongxilong@163.org
 @param		Copyright (c) 2016 
 @date		2016-6-5
 @brief		单例类管理中心实现
 */


#include "..\stdafx.h"
#include "SingletonManagerCenter.h"

static  SingletonManagerCenter* g_instance = NULL;
static  CLocker                 g_locker;

SingletonManagerCenter* SingletonManagerCenter::GetInstance()
{
    if(g_instance == NULL)
	{
        g_locker.Lock();
		if (g_instance == NULL)
		{
			g_instance = new SingletonManagerCenter();
		}
		g_locker.Unlock();
	}
	return g_instance;
}

void  SingletonManagerCenter::SafeRelease()
{
    RemoveAllSingletons();
	SAFE_RELEASE(g_instance);
}
