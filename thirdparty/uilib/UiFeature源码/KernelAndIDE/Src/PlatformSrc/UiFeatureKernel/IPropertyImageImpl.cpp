
#include "stdafx.h"
#include "IPropertyImageImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IControlBase.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyImageImpl::IPropertyImageImpl(IUiFeatureKernel* pUiKernel) : IPropertyImage(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_IMAGE_NAME));
}

IPropertyImageImpl::~IPropertyImageImpl()
{

}

bool IPropertyImageImpl::OnDrawAnimation()
{
	if (GetImageBaseProp() == NULL)
		return false;

	return GetImageBaseProp()->OnDrawAnimation();
}

const WCHAR * IPropertyImageImpl::GetImageBaseName()
{
	return GetRelevancyPropName();
}

bool IPropertyImageImpl::IsRightData()
{
	return (GetRelevancyProp() != NULL);
}

IPropertyImageBase* IPropertyImageImpl::GetImageBaseProp()
{
	return dynamic_cast<IPropertyImageBase*>(GetRelevancyProp());
}

void IPropertyImageImpl::SetNeedRedraw()
{
	if (GetOwnerObject() == NULL)
		return;

	IControlBase *pCtrlBase = dynamic_cast<IControlBase*>(GetOwnerObject());
	if (pCtrlBase == NULL)
		return;

	pCtrlBase->RedrawControl();
}

void IPropertyImageImpl::SetImageBaseProp(IPropertyImageBase* pNewImgBase)
{
	if (pNewImgBase == NULL)
	{
		SetRelevancyProp(NULL);
		SetNeedRedraw();
		return;
	}

	IPropertyBase* pPropBase = dynamic_cast<IPropertyBase*>(pNewImgBase);
	SetRelevancyProp(pPropBase);
	SetNeedRedraw();
}

// 从XML节点读取属性值，并放入属性队列
bool IPropertyImageImpl::ReadPropertyFromXmlNode(xml_node* pXmlNode)
{
	if (pXmlNode == NULL)
		return false;

	const WCHAR * psz_id = XmlGetAttrValue(pXmlNode, _T(SKIN_OBJECT_ID));
	const WCHAR * psz_name = XmlGetAttrValue(pXmlNode, L"name");
	const WCHAR * psz_imagebasename = XmlGetAttrValue(pXmlNode, L"imagebasename");
	if (psz_id == NULL || psz_name == NULL || psz_imagebasename == NULL)
		return false;

	SetObjectId(_wtoi(psz_id));
	SetObjectName(psz_name);
	SetRelevancyPropName((WCHAR*)psz_imagebasename);
	return true;
}

// 写入xml
bool IPropertyImageImpl::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
{
	// 如果是无效属性，不写入XML
	if (!GetActivePropetry())
		return true;

	if (pParentXmlNode == NULL)
		return false;

	// 如果没有数据，就不存入xml
	if (GetRelevancyPropName() == NULL || wcslen(GetRelevancyPropName()) <= 0)
		return true;

	xml_node PropNode = pParentXmlNode->append_child(L"item");
	if (!PropNode)
		return false;

	AddIntAttrToNode(&PropNode, _T(SKIN_OBJECT_ID), (int)GetObjectId());
	AddStringAttrToNode(&PropNode, L"name", GetObjectName());
	AddStringAttrToNode(&PropNode, L"imagebasename", GetRelevancyPropName());
	return true;
}

void IPropertyImageImpl::SetActivePropetry(bool bIsActive)
{
	m_bIsActiveProp = bIsActive;

	if (GetRelevancyProp() != NULL && !bIsActive)
		GetRelevancyProp()->SetActivePropetry(false);
}

bool IPropertyImageImpl::GetActivePropetry()
{
	return m_bIsActiveProp;
}

SIZE IPropertyImageImpl::GetImageSize()
{
	SIZE sizeImg;
	sizeImg.cx = sizeImg.cy = 0;

	if (GetImageBaseProp() != NULL)
		sizeImg = GetImageBaseProp()->GetImageSize();

	return sizeImg;
}

// 初始化Image，将Image加载到内存，程序默认是没有加载图片数据的
void IPropertyImageImpl::InitImage()
{
	if (GetImageBaseProp() == NULL)
		return;

	GetImageBaseProp()->InitImage();
}

bool IPropertyImageImpl::DrawImage(CDrawingBoard &DstDc, RECT DstRct, int nAlpha)
{
	IPropertyImageBase *pImgBase = (IPropertyImageBase*)GetRelevancyProp();
	if (pImgBase == NULL)
		return false;

	return pImgBase->DrawImage(DstDc, DstRct, nAlpha);
}

bool IPropertyImageImpl::DrawImage(HDC hDc, RECT DstRct, int nAlpha)
{
	IPropertyImageBase *pImgBase = (IPropertyImageBase*)GetRelevancyProp();
	if (pImgBase == NULL)
		return false;

	return pImgBase->DrawImage(hDc, DstRct, nAlpha);
}

void IPropertyImageImpl::SetImageProp(IMAGE_BASE_PROP* pImgProp)
{
	if (GetImageBaseProp() == NULL)
		return;

	GetImageBaseProp()->SetImageProp(pImgProp);
}

IMAGE_BASE_PROP* IPropertyImageImpl::GetImageProp()
{
	if (GetImageBaseProp() == NULL)
		return NULL;

	return GetImageBaseProp()->GetImageProp();
}

// 得到图片的内存DC
CDrawingImage* IPropertyImageImpl::GetDrawingImage()
{
	if (GetImageBaseProp() == NULL)
		return NULL;

	return GetImageBaseProp()->GetDrawingImage();
}

// 创建图片的内存DC
CDrawingImage* IPropertyImageImpl::CreateDrawingImage()
{
	if (GetImageBaseProp() == NULL)
		return NULL;

	return GetImageBaseProp()->CreateDrawingImage();
}

// 生成缩略图
IPropertyImageBase* IPropertyImageImpl::CreateThumbnailImage(SIZE sizeThumbnail)
{
	if (GetImageBaseProp() == NULL)
		return NULL;

	return GetImageBaseProp()->CreateThumbnailImage(sizeThumbnail);
}

// 删除缩略图
bool IPropertyImageImpl::DeleteThumbnailImage(IPropertyImageBase* pImgBase)
{
	if (GetImageBaseProp() == NULL)
		return NULL;

	return GetImageBaseProp()->DeleteThumbnailImage(pImgBase);
}

// 从另一个属性克隆
bool IPropertyImageImpl::Clone(IPropertyBase* pFromPropBase)
{
	if (!IsSameTypePropetry(dynamic_cast<IPropertyBase*>(this), pFromPropBase))
		return false;

	IPropertyImageImpl* pFromProp = dynamic_cast<IPropertyImageImpl*>(pFromPropBase);
	if (pFromProp == NULL)
		return false;

//	SetRelevancyProp(NULL);
//	SetRelevancyPropName((WCHAR*)pFromProp->GetRelevancyPropName());

	SetRelevancyProp(pFromProp->GetRelevancyProp());
	return true;
}
