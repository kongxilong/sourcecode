
#pragma once

// 在 UiFeatureBuilder 中能够编辑的属性的类型
enum OBJECT_TYPE_ID 
{
	// 非法类型ID
	OTID_NONE	= 0,
	// bool属性
	OTID_BOOL	= 1,
	// 颜色属性
	OTID_COLOR,
	// 颜色属性的子节点，存储了具体的颜色信息
	OTID_COLOR_BASE,
	// 下拉框属性
	OTID_COMBOBOX,
	// 鼠标手势属性
	OTID_CURSOR,
	// 鼠标手势属性的子节点，存储了具体的鼠标手势信息
	OTID_CURSOR_BASE,
	// 字体属性
	OTID_FONT,
	// 字体属性的子节点，存储了具体的字体信息
	OTID_FONT_BASE,
	// 图片属性
	OTID_IMAGE,
	// 图片属性的子节点，存储了具体的图片信息
	OTID_IMAGE_BASE,
	// 整数属性
	OTID_INT,
	// 字符串属性
	OTID_STRING,
	// 属性组属性，其下可以存储一堆属性
	OTID_GROUP,
	// 控件的属性结合
	OTID_CONTROL,
	// 窗口的属性结合
	OTID_WINDOW,
	// 属性的最后一个枚举值，用于计数
	OTID_LAST
};

#define PROP_TYPE_BOOL_NAME						"bool"
#define PROP_TYPE_COLOR_NAME					"color"
#define PROP_TYPE_COLOR_BASE_NAME				"colorbase"
#define PROP_TYPE_COMBOBOX_NAME					"combobox"
#define PROP_TYPE_CURSOR_NAME					"cursor"
#define PROP_TYPE_CURSOR_BASE_NAME				"cursorbase"
#define PROP_TYPE_FONT_NAME						"font"
#define PROP_TYPE_FONT_BASE_NAME				"fontbase"
#define PROP_TYPE_IMAGE_NAME					"image"
#define PROP_TYPE_IMAGE_BASE_NAME				"imagebase"
#define PROP_TYPE_INT_NAME						"int"
#define PROP_TYPE_STRING_NAME					"string"
#define PROP_TYPE_GROUP_NAME					"group"
#define PROP_TYPE_CONTROL_NAME					"control"
#define PROP_TYPE_WINDOW_NAME					"window"

#define ERROR_OBJECT_ID							(0)

class IUiFeatureKernel;
class IFeatureObject
{
public:
	IFeatureObject(IUiFeatureKernel *pUiKernel);
	virtual ~IFeatureObject();

	DWORD GetObjectId();
	void SetObjectId(DWORD dwObjId);

	const WCHAR* GetObjectType();
	void SetObjectType(const WCHAR* pszObjType);
	OBJECT_TYPE_ID GetObjectTypeId();

	const WCHAR* GetObjectName();
	void SetObjectName(const WCHAR* pszObjName);

	virtual void SetActivePropetry(bool bIsActive);
	virtual bool GetActivePropetry();

	// 父控件/父窗口
	IFeatureObject* GetOwnerObject();
	void SetOwnerObject(IFeatureObject* pOwnerObj);

	// 锁定 ObjectName，不允许修改ObjectName
	void LockName(bool bLock);
	bool IsLockName();

	void SetUiKernel(IUiFeatureKernel* pUiKernel);
	IUiFeatureKernel *GetUiKernel();

protected:
	IUiFeatureKernel *m_pUiKernel;
	// 是否为有效属性，在Builder保存属性时判断使用。
	bool m_bIsActiveProp;

private:
	bool m_bIsLockName;
	// ID，唯一的识别id
	DWORD m_dwObjId;
	// 类型
	WCHAR *m_pszObjType;
	// 名称
	WCHAR *m_pszObjName;
	IFeatureObject *m_pOwnerObj;
};
