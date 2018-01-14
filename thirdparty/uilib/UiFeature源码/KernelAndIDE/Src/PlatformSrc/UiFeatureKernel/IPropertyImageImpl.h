//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyImage.h"

class IPropertyImageImpl : public IPropertyImage
{
public:
	IPropertyImageImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyImageImpl();

	virtual void SetActivePropetry(bool bIsActive);
	virtual bool GetActivePropetry();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

//////////////////////////////////////////////////////////////////////////
	// 生成缩略图，缩略图如果外部要释放，调用 DeleteThumbnailImage 或 IPropertySkinManager::DeleteThumbnailImage() 即可，也可不释放，内核会在最后一起释放
	virtual IPropertyImageBase* CreateThumbnailImage(SIZE sizeThumbnail);
	// 删除缩略图
	virtual bool DeleteThumbnailImage(IPropertyImageBase* pImgBase);

	virtual IPropertyImageBase* GetImageBaseProp();
	virtual void SetImageBaseProp(IPropertyImageBase* pNewImgBase);
	virtual const WCHAR * GetImageBaseName();
	virtual SIZE GetImageSize();
	virtual void SetImageProp(IMAGE_BASE_PROP* pImgProp);
	virtual IMAGE_BASE_PROP* GetImageProp();
	// 得到图片的内存DC
	virtual CDrawingImage* GetDrawingImage();
	// 创建图片的内存DC
	virtual CDrawingImage* CreateDrawingImage();
	virtual void SetNeedRedraw();
	// 初始化Image，将Image加载到内存，程序默认是没有加载图片数据的
	virtual void InitImage();
	// 轻量级动画定时器绘制动画
	virtual bool OnDrawAnimation();

	// 绘制图片，如果alpha不是255，则根据这个alpha值来绘制
	virtual bool DrawImage(CDrawingBoard &DstDc, RECT DstRct, int nAlpha = 255);
	virtual bool DrawImage(HDC hDc, RECT DstRct, int nAlpha = 255);
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);
};
