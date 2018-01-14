/*!
 @file		ISingleton.h
 @author    kongxilong
 @param		Email: hzkongxilong@163.org
 @param		Copyright (c) 2016 
 @date		2016-6-5
 @brief		单例接口
	   
 */

#pragma once

#include <list>
#include <map>

/************************************************************

一种不完美的单例模式的实现 比较完美的实现方式是使用模板 
但是模板中夹带静态变量 导出dll是非常难实现的 先使用宏实现吧

使用单例模式的方法：
1、在类声明的最后部分使用宏 DECLARE_SINGLETON_CLASS(CLASSNAME)
2、在实现文件中初始化静态全局实例变量及锁 实现GetInstance函数和SafeRelease函数
*************************************************************/

#define SAFE_RELEASE(p)   {if(!(p)) delete (p); (p) = NULL;}


class  CLocker
{
public: 
	inline CLocker() 
	{ 
		m_hMutex = CreateMutex(NULL,FALSE,NULL); 
	} 
	inline ~CLocker() 
	{ 
		if (IsLock())
		{
			Unlock();
		}
		CloseHandle(m_hMutex); 
	} 
	inline void Lock() 
	{ 
		m_bLocked = TRUE; 
		WaitForSingleObject(m_hMutex, INFINITE); 	
	} 

	inline void Unlock() 
	{ 
		if(IsLock())
		{
			m_bLocked = FALSE; 
			ReleaseMutex(m_hMutex);
		}
	} 
	inline BOOL IsLock()
	{ 
		return m_bLocked == TRUE;
	}
private: 
	HANDLE m_hMutex;
	BOOL   m_bLocked;
};
/*!
 @brief ISingletonBase  所有单例必须继承的接口
*/
struct  ISingletonBase
{
	virtual void OnStart() = 0;    //用于初始化单例模式时初始化
	virtual void OnQuit() = 0;     //用于退出程序时清理单例中的数据
	virtual void SafeRelease() = 0;
	virtual int  GetSingletonID() = 0;
};

//单例的管理类基类  可在此基础上 实现单例类形式的管理类  
class  SingletonManagerBase
{
public:
	virtual void  RegisterSingleton(ISingletonBase* pISingletonInterface)
	{
		ASSERT(pISingletonInterface != NULL);
		//CSingleLock lock(&m_listMutex);
		m_listLocker.Lock();
		int id = pISingletonInterface->GetSingletonID();
		m_mapSingletons[id] = pISingletonInterface;
		m_listLocker.Unlock();
		//注册的时候自动调用初始化函数 
		pISingletonInterface->OnStart();
	}
	virtual void  UnRegisterSingleton(ISingletonBase* pISingletonInterface)
	{
		ASSERT(pISingletonInterface != NULL);
		m_listLocker.Lock();
		std::map<int,ISingletonBase*>::iterator it = m_mapSingletons.begin();
		for(; it != m_mapSingletons.end(); ++it)
		{
			if(it->second == pISingletonInterface)
			{
				m_mapSingletons.erase(it);
				break;
			}
		}
		m_listLocker.Unlock();
		//取消注册的时候自动调用清理函数 并删除单例资源
		pISingletonInterface->OnQuit();
		pISingletonInterface->SafeRelease();
	}
	virtual void  RemoveAllSingletons()
	{
		m_listLocker.Lock();
		std::map<int,ISingletonBase* >::iterator it = m_mapSingletons.begin();
		for(; it != m_mapSingletons.end(); ++it)
		{
			ISingletonBase* pSingletonClass = it->second;
			pSingletonClass->OnQuit();
			pSingletonClass->SafeRelease();
		}
        m_mapSingletons.clear();
		m_listLocker.Unlock();
	}
	virtual ISingletonBase*  GetSingletonByID(const int& id)
	{
		m_listLocker.Lock();
		std::map<int,ISingletonBase* >::iterator it = m_mapSingletons.begin();
		for(; it != m_mapSingletons.end(); ++it)
		{
			ISingletonBase* pSingletonClass = it->second;
			if(it->first == id)
			{
				return pSingletonClass;
			}
		}
		m_listLocker.Unlock();
		return NULL;
	}
private:
	std::map<int,ISingletonBase*>  m_mapSingletons;
	CLocker  m_listLocker;
};




#define DECLARE_SINGLETON_CLASS(CLASSNAME) \
public:                              \
	static CLASSNAME* GetInstance(); \
    virtual void SafeRelease();      \
private:                             \
	CLASSNAME(void){}                \
	CLASSNAME(const CLASSNAME&){}    \
	CLASSNAME& operator= (const CLASSNAME &){ return *this;} \
public:                              




