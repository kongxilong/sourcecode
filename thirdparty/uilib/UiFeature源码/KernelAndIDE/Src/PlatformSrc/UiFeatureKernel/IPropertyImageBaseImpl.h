//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyImageBase.h"

class CGifImage;
class IPropertyImageBaseImpl : public IPropertyImageBase
{
public:
	IPropertyImageBaseImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyImageBaseImpl();

	virtual bool GetActivePropetry();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

//////////////////////////////////////////////////////////////////////////
	// Builder使用函数，即：编辑状态下使用函数

//////////////////////////////////////////////////////////////////////////
	// 生成缩略图，缩略图如果外部要释放，调用 IPropertySkinManager::DeleteImageBase() 即可，也可不释放，内核会在最后一起释放
	virtual IPropertyImageBaseImpl* CreateThumbnailImage(SIZE sizeThumbnail);
	// 删除缩略图
	virtual bool DeleteThumbnailImage(IPropertyImageBase* pImgBase);

	// 运行时态使用函数
	virtual void SetImageProp(IMAGE_BASE_PROP* pImgProp);
	virtual IMAGE_BASE_PROP* GetImageProp();
	virtual ZIP_FILE * GetZipFile();
	virtual bool SetZipFile(ZIP_FILE *pZipFile, bool bCreateMemDc);
	virtual SIZE GetImageSize();
	// 得到图片的内存DC
	virtual CDrawingImage* GetDrawingImage();
	// 创建图片的内存DC
	virtual CDrawingImage* CreateDrawingImage();
	// 初始化Image，将Image加载到内存，程序默认是没有加载图片数据的
	virtual void InitImage();
	// 轻量级动画定时器绘制动画
	virtual bool OnDrawAnimation();
	// 设置使用计数
	virtual void AddUseCounts();
	// 减掉技术
	virtual void SubtractUseCounts();
	// 取得使用技术
	virtual int GetUseCounts();

	// 绘制图片，如果alpha不是255，则根据这个alpha值来绘制
	virtual bool DrawImage(CDrawingBoard &DstDc, RECT DstRct, int nAlpha = 255);
	virtual bool DrawImage(HDC hDc, RECT DstRct, int nAlpha = 255);
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	bool DrawImage_AllLaShen(CDrawingBoard &DstDc, RECT DstRct, int nAlpha);
	bool DrawImage_PingPu(CDrawingBoard &DstDc, RECT DstRct, int nAlpha);
	bool DrawImage_JggLaShen(CDrawingBoard &DstDc, RECT DstRct, int nAlpha);

	bool DrawImage_AllLaShen(HDC hDstDc, RECT DstRct, int nAlpha);
	bool DrawImage_PingPu(HDC hDstDc, RECT DstRct, int nAlpha);
	bool DrawImage_JggLaShen(HDC hDstDc, RECT DstRct, int nAlpha);

	bool SetXuLieDrawInTimer();
	bool InitDrawXuLieRect();

	bool SetGifDrawInTimer();
	void InitGifImage();

	bool InitStaticImage();
	void SafeDeleteDrawImage();

private:
	bool m_bIsDrawImgCreate;
	IMAGE_BASE_PROP m_ImageProp;
	CDrawingImage *m_pDrawImg;
	// 被使用的计数器
	int m_nUseCtns;
	// 写入zip文件的信息
	ZIP_FILE *m_pZipFile;

//////////////////////////////////////////////////////////////////////////
	// 序列使用参数
	RECT m_rctXuLieDraw;

//////////////////////////////////////////////////////////////////////////
	CGifImage *m_pGifImg;
	int m_nGifCurFrameTime;
	int m_nGifTimeCtns;

	bool m_bIsTimerDrawEnd;
};
