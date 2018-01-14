//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"
#include "IPropertyImageBase.h"
#include "CDrawingBoard.h"


class _declspec(novtable) IPropertyImage : public IPropertyBase
{
public:
	IPropertyImage(IUiFeatureKernel* pUiKernel);
	virtual void SetActivePropetry(bool bIsActive) = 0;
	virtual bool GetActivePropetry() = 0;

	// 是否为合法数据
	virtual bool IsRightData() = 0;
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode) = 0;

//////////////////////////////////////////////////////////////////////////
	// 生成缩略图，缩略图如果外部要释放，调用 DeleteThumbnailImage 或 IPropertySkinManager::DeleteThumbnailImage() 即可，也可不释放，内核会在最后一起释放
	virtual IPropertyImageBase* CreateThumbnailImage(SIZE sizeThumbnail) = 0;
	// 删除缩略图
	virtual bool DeleteThumbnailImage(IPropertyImageBase* pImgBase) = 0;
	virtual IPropertyImageBase* GetImageBaseProp() = 0;
	virtual void SetImageBaseProp(IPropertyImageBase* pNewImgBase) = 0;
	virtual const WCHAR * GetImageBaseName() = 0;
	virtual SIZE GetImageSize() = 0;
	virtual void SetImageProp(IMAGE_BASE_PROP* pImgProp) = 0;
	virtual IMAGE_BASE_PROP* GetImageProp() = 0;
	// 得到图片的内存DC
	virtual CDrawingImage* GetDrawingImage() = 0;
	// 创建图片的内存DC
	virtual CDrawingImage* CreateDrawingImage() = 0;
	virtual void SetNeedRedraw() = 0;
	// 初始化Image，将Image加载到内存，程序默认是没有加载图片数据的
	virtual void InitImage() = 0;
	// 轻量级动画定时器绘制动画
	virtual bool OnDrawAnimation() = 0;

	// 绘制图片，如果alpha不是255，则根据这个alpha值来绘制
	virtual bool DrawImage(CDrawingBoard &DstDc, RECT DstRct, int nAlpha = 255) = 0;
	virtual bool DrawImage(HDC hDc, RECT DstRct, int nAlpha = 255) = 0;
};
