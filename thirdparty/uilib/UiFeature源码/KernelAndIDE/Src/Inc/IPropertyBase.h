//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once

#define PUGIXML_WCHAR_MODE
#include "PugiXml\pugixml.hpp"
using namespace pugi;

#include "IFeatureObject.h"
#include "IPropertyBuilderCtrl.h"
#include "CTList.h"
#include <atlstr.h>
using namespace ATL;

class IPropertyBase;
class IUiFeatureKernel;


// 一种属性的队列，如：font、string、image等，KEY：ObjectId
// 一种控件的属性队列，如：Button、CheckBox等，KEY：ObjectId
// 一种窗口的属性队列，KEY：ObjectId
typedef map<DWORD, IPropertyBase*>						IPropertyBaseMap;

// 所有种类的属性的队列，KEY：属性类型名称，如：font
// 所有控件种类的属性的队列，KEY：控件类型名称，如：Button
// KEY:ObjectId，所有窗口的管理器map
typedef map<CStringW, IPropertyBaseMap*>					IPropertyBaseMapMap;

class IUiFeatureKernel;
class IPropertyBase : public IFeatureObject, public ListObj
{
public:
	IPropertyBase(IUiFeatureKernel *pUiKernel);
	virtual ~IPropertyBase();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	// 关联属性名称
	virtual const WCHAR* GetRelevancyPropName();
	virtual void SetRelevancyPropName(WCHAR *pszPropName);

	const char* GetObjectInfo();
	void SetObjectInfo(const char* pszObjInfo);

	// 关联属性
	virtual IPropertyBase* GetRelevancyProp();
	virtual void SetRelevancyProp(IPropertyBase *pszProp);
	
	// 在Builder中禁用
	virtual void EnableInBuilder(bool bEnable);
	virtual bool IsEnableInBuilder();
	virtual void SetPropBuilderCtrl(IPropertyBuilderCtrl* pBuilderCtrl);

	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase) = 0;

private:
	char *m_pszObjInfo;
	// 关联属性名称：长度不允许超过30字节
	WCHAR *m_pszRelevancyPropName;
	// 关联属性
	IPropertyBase *m_pRelevancyProp;
	IPropertyBuilderCtrl* m_pBuilderCtrl;
	bool m_bIsEnableInBuilder;
};
