
#include "stdafx.h"
#include "..\..\Inc\IFeatureObject.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "..\..\Inc\ICommonFun.h"


IFeatureObject::IFeatureObject(IUiFeatureKernel *pUiKernel)
{
	::CoInitialize(NULL);

	m_pUiKernel = pUiKernel;

	m_bIsLockName = false;
	m_bIsActiveProp = false;
	m_pOwnerObj = NULL;

	m_dwObjId = ERROR_OBJECT_ID;
	if (m_pUiKernel != NULL)
		m_dwObjId = m_pUiKernel->KernelGetId();

	m_pszObjType = NULL;
	SetObjectType(L"NO_OBJECT_TYPE_NAME");

	m_pszObjName = NULL;
}

IFeatureObject::~IFeatureObject()
{
	if (m_pUiKernel != NULL)
	{
		m_pUiKernel->KernelFree((BYTE**)&m_pszObjType);
		m_pUiKernel->KernelFree((BYTE**)&m_pszObjName);
	}
}

void IFeatureObject::SetUiKernel(IUiFeatureKernel* pUiKernel)
{
	m_pUiKernel = pUiKernel;
}

IFeatureObject* IFeatureObject::GetOwnerObject()
{
	return m_pOwnerObj;
}

void IFeatureObject::SetOwnerObject(IFeatureObject* pOwnerObj)
{
	m_pOwnerObj = pOwnerObj;
}

DWORD IFeatureObject::GetObjectId()
{
	return m_dwObjId;
}

void IFeatureObject::SetObjectId(DWORD dwObjId)
{
	m_dwObjId = dwObjId;
}

const WCHAR* IFeatureObject::GetObjectType()
{
	return (const WCHAR*)m_pszObjType;
}

void IFeatureObject::SetObjectType(const WCHAR* pszObjType)
{
	if (m_pUiKernel == NULL)
		return;

	m_pUiKernel->KernelFree((BYTE**)&m_pszObjType);

	if (pszObjType == NULL || wcslen(pszObjType) <= 0)
		return;

	int nLen = (wcslen(pszObjType) + 1) * sizeof(WCHAR);

	m_pszObjType = (WCHAR*)m_pUiKernel->KernelMalloc(nLen);
	if (m_pszObjType == NULL)
		return;

	memset(m_pszObjType, 0, nLen);
	swprintf_s(m_pszObjType, wcslen(pszObjType) + 1, L"%s", pszObjType);
}

const WCHAR* IFeatureObject::GetObjectName()
{
	return (const WCHAR*)m_pszObjName;
}

void IFeatureObject::SetObjectName(const WCHAR* pszObjName)
{
	if (m_pUiKernel == NULL)
		return;

	m_pUiKernel->KernelFree((BYTE**)&m_pszObjName);

	if (pszObjName == NULL || wcslen(pszObjName) <= 0)
		return;

	int nLen = (wcslen(pszObjName) + 1) * sizeof(WCHAR);

	m_pszObjName = (WCHAR*)m_pUiKernel->KernelMalloc(nLen);
	if (m_pszObjName == NULL)
		return;

	memset(m_pszObjName, 0, nLen);
	swprintf_s(m_pszObjName, wcslen(pszObjName) + 1, L"%s", pszObjName);
}

OBJECT_TYPE_ID IFeatureObject::GetObjectTypeId()
{
	return PropStringToType(m_pszObjType);
}

void IFeatureObject::SetActivePropetry(bool bIsActive)
{
	m_bIsActiveProp = bIsActive;
}

bool IFeatureObject::GetActivePropetry()
{
	return m_bIsActiveProp;
}

// 锁定ObjectName，不允许修改ObjectName
void IFeatureObject::LockName(bool bLock)
{
	m_bIsLockName = bLock;
}

bool IFeatureObject::IsLockName()
{
	return m_bIsLockName;
}

IUiFeatureKernel* IFeatureObject::GetUiKernel()
{
	return m_pUiKernel;
}
