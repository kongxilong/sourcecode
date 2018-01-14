
#include "stdafx.h"
#include "IPropertyImageBaseImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "..\..\Inc\ICommonFun.h"
#include "CGifImage.h"

IPropertyImageBaseImpl::IPropertyImageBaseImpl(IUiFeatureKernel* pUiKernel) : IPropertyImageBase(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_IMAGE_BASE_NAME));

	m_pZipFile = NULL;
	m_pDrawImg = NULL;

	m_nUseCtns = 0;
	AddUseCounts();

	InitPropImageBase(&m_ImageProp);

	INIT_RECT(m_rctXuLieDraw);
	m_bIsTimerDrawEnd = true;

	m_pGifImg = new CGifImage;
	m_nGifCurFrameTime = -1;
	m_nGifTimeCtns = 0;
	m_bIsDrawImgCreate = false;
}

IPropertyImageBaseImpl::~IPropertyImageBaseImpl()
{
	SAFE_DELETE(m_pGifImg);
	SafeDeleteDrawImage();
}

// 设置使用计数
void IPropertyImageBaseImpl::AddUseCounts()
{
	m_nUseCtns++;
}

// 减掉技术
void IPropertyImageBaseImpl::SubtractUseCounts()
{
	m_nUseCtns--;
}

// 取得使用技术
int IPropertyImageBaseImpl::GetUseCounts()
{
	return m_nUseCtns;
}

void IPropertyImageBaseImpl::SafeDeleteDrawImage()
{
	if (m_bIsDrawImgCreate)
	{
		SAFE_DELETE(m_pDrawImg);
	}
	m_pDrawImg = NULL;
	m_bIsDrawImgCreate = false;
}

ZIP_FILE* IPropertyImageBaseImpl::GetZipFile()
{
	return m_pZipFile;
}

bool IPropertyImageBaseImpl::SetZipFile(ZIP_FILE *pZipFile, bool bCreateMemDc)
{
	if (GetUiKernel() == NULL || (m_pDrawImg != NULL && m_pZipFile != NULL && m_pZipFile == pZipFile))
		return false;

	SafeDeleteDrawImage();
	m_pZipFile = pZipFile;
	if (m_pZipFile == NULL)
	{
		InitPropImageBase(&m_ImageProp);
		return true;
	}

	m_pDrawImg = &m_pZipFile->DrawImg;
	if (m_pDrawImg == NULL)
	{
		m_pZipFile = NULL;
		InitPropImageBase(&m_ImageProp);
		return false;
	}

	m_ImageProp.bIsZipFile = true;
	wcscpy_s(m_ImageProp.strFileName, MAX_PATH, pZipFile->strFileName);

	if (bCreateMemDc && m_pZipFile != NULL)
	{
		INIT_RECT(m_ImageProp.RectInImage);
		if (m_ImageProp.ImgPlayType == IPT_GIF)
		{
			if (m_pGifImg != NULL)
			{
				if (m_pGifImg->CreateByMem(m_pZipFile->pFileData, m_pZipFile->dwSrcFileLen))
				{
					m_ImageProp.RectInImage.right = m_pGifImg->GetImageSize().cx;
					m_ImageProp.RectInImage.bottom = m_pGifImg->GetImageSize().cy;

					m_nGifTimeCtns = 0;
					if (m_ImageProp.ImgBoFangType == IBFT_ZHENGXIANG)
					{
						// 正向播放
						m_nGifCurFrameTime = m_pGifImg->SetCurrentPlayFrame(0);
					}
					else
					{
						// 反向播放
						m_nGifCurFrameTime = m_pGifImg->SetCurrentPlayFrame(m_pGifImg->GetFrameCounts() - 1);
					}
				}
			}
		}
		else
		{
			if (IS_INVALID_HANDLE(m_pDrawImg->GetSafeHdc()))
			{
				m_pDrawImg->CreateByMem(m_pZipFile->pFileData, m_pZipFile->dwSrcFileLen);
				if (IS_SAFE_HANDLE(m_pDrawImg->GetSafeHdc()) && !GetUiKernel()->IsDesignMode())
				{
					SAFE_DELETE_LIST(m_pZipFile->pFileData);
				}
			}

			if (IS_SAFE_HANDLE(m_pDrawImg->GetSafeHdc()))
			{
				m_ImageProp.RectInImage.right = m_pDrawImg->GetDcSize().cx;
				m_ImageProp.RectInImage.bottom = m_pDrawImg->GetDcSize().cy;
			}
		}
	}

	return true;
}

bool IPropertyImageBaseImpl::IsRightData()
{
	return (wcslen(m_ImageProp.strFileName) > 0 && RECT_WIDTH(m_ImageProp.RectInImage) > 0 && RECT_HEIGHT(m_ImageProp.RectInImage) > 0);
}

// 初始化Image，将Image加载到内存，程序默认是没有加载图片数据的
void IPropertyImageBaseImpl::InitImage()
{
	if (m_ImageProp.ImgPlayType == IPT_GIF)
		InitGifImage();
	else
		InitStaticImage();
}

SIZE IPropertyImageBaseImpl::GetImageSize()
{
	InitImage();

	SIZE sizImg;
	sizImg.cx = sizImg.cy = 0;
	if (m_ImageProp.ImgPlayType == IPT_GIF)
	{
		if (m_pGifImg != NULL)
		{
			sizImg = m_pGifImg->GetImageSize();
		}
	}
	else
	{
		if (m_pDrawImg != NULL)
			sizImg = m_pDrawImg->GetDcSize();
	}
	return sizImg;
}

IMAGE_BASE_PROP* IPropertyImageBaseImpl::GetImageProp()
{
	return &m_ImageProp;
}

void IPropertyImageBaseImpl::SetImageProp(IMAGE_BASE_PROP* pImgProp)
{
	if (pImgProp == NULL)
		return;

	m_ImageProp = *pImgProp;
}

// 从XML节点读取属性值，并放入属性队列
bool IPropertyImageBaseImpl::ReadPropertyFromXmlNode(xml_node* pXmlNode)
{
	if (pXmlNode == NULL)
		return false;

	const WCHAR * psz_id = XmlGetAttrValue(pXmlNode, _T(SKIN_OBJECT_ID));
	const WCHAR * psz_name = XmlGetAttrValue(pXmlNode, L"name");
	const WCHAR * psz_imageinzip = XmlGetAttrValue(pXmlNode, L"imageinzip");
	const WCHAR * psz_showtype = XmlGetAttrValue(pXmlNode, L"showtype");
	if (psz_id == NULL || psz_name == NULL || psz_imageinzip == NULL || psz_showtype == NULL)
		return false;

	xml_node RectInImage = pXmlNode->child(L"rectinimage");
	xml_node Jgg = pXmlNode->child(L"jgg");
	xml_node Animation = pXmlNode->child(L"animation");
	if (!RectInImage || !Jgg)
		return false;

	// rectinimage
	const WCHAR * psz_left = XmlGetAttrValue(&RectInImage, L"left");
	const WCHAR * psz_top = XmlGetAttrValue(&RectInImage, L"top");
	const WCHAR * psz_right = XmlGetAttrValue(&RectInImage, L"right");
	const WCHAR * psz_bottom = XmlGetAttrValue(&RectInImage, L"bottom");

	// jgg
	const WCHAR * psz_jgg_left = XmlGetAttrValue(&Jgg, L"left");
	const WCHAR * psz_jgg_top = XmlGetAttrValue(&Jgg, L"top");
	const WCHAR * psz_jgg_right = XmlGetAttrValue(&Jgg, L"right");
	const WCHAR * psz_jgg_bottom = XmlGetAttrValue(&Jgg, L"bottom");
	const WCHAR * psz_drawmid = XmlGetAttrValue(&Jgg, L"drawmid");

	if (psz_left == NULL || psz_top == NULL || psz_right == NULL || psz_bottom == NULL || 
		psz_jgg_left == NULL || psz_jgg_top == NULL || psz_jgg_right == NULL || psz_jgg_bottom == NULL)
		return false;

	SetObjectId(_wtoi(psz_id));
	SetObjectName(psz_name);

	m_ImageProp.bIsZipFile = true;
	wcscpy_s(m_ImageProp.strFileName, MAX_PATH, psz_imageinzip);
	m_ImageProp.ImgShowType = (IMAGE_SHOW_TYPE)_wtoi(psz_showtype);

	m_ImageProp.RectInImage.left = _wtoi(psz_left);
	m_ImageProp.RectInImage.top = _wtoi(psz_top);
	m_ImageProp.RectInImage.right = _wtoi(psz_right);
	m_ImageProp.RectInImage.bottom = _wtoi(psz_bottom);

	m_ImageProp.jggInfo.left = _wtoi(psz_jgg_left);
	m_ImageProp.jggInfo.top = _wtoi(psz_jgg_top);
	m_ImageProp.jggInfo.right = _wtoi(psz_jgg_right);
	m_ImageProp.jggInfo.bottom = _wtoi(psz_jgg_bottom);

	// 为了兼容以前测试皮肤
	if (psz_drawmid == NULL)
		m_ImageProp.bIsDrawJggMid = false;
	else
		m_ImageProp.bIsDrawJggMid = (psz_drawmid[0] == '1');

	if (Animation)
	{
		const WCHAR * psz_playtype = XmlGetAttrValue(&Animation, L"playtype");
		const WCHAR * psz_looptype = XmlGetAttrValue(&Animation, L"looptype");
		const WCHAR * psz_bofangtype = XmlGetAttrValue(&Animation, L"bofangtype");
		if (psz_playtype == NULL || psz_looptype == NULL || psz_bofangtype == NULL)
			return false;

		m_ImageProp.ImgPlayType = (IMAGE_PLAY_TYPE)_wtoi(psz_playtype);
		m_ImageProp.ImgLoopType = (IMAGE_LOOP_TYPE)_wtoi(psz_looptype);
		m_ImageProp.ImgBoFangType = (IMAGE_BOFANG_TYPE)_wtoi(psz_bofangtype);
	}

	return true;
}

bool IPropertyImageBaseImpl::GetActivePropetry()
{
	return true;
}

// 写入xml
bool IPropertyImageBaseImpl::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
{
	// 如果是无效属性，不写入XML
	if (!GetActivePropetry())
		return true;

	if (pParentXmlNode == NULL)
		return false;

	xml_node PropNode = pParentXmlNode->append_child(L"item");
	if (!PropNode)
		return false;

	AddIntAttrToNode(&PropNode, _T(SKIN_OBJECT_ID), (int)GetObjectId());
	AddStringAttrToNode(&PropNode, L"name", GetObjectName());

	// 在压缩文件中的文件名
	AddStringAttrToNode(&PropNode, L"imageinzip", m_ImageProp.strFileName);
	AddIntAttrToNode(&PropNode, L"showtype", m_ImageProp.ImgShowType);

	xml_node Node_rectinimage = PropNode.append_child(L"rectinimage");
	if (!Node_rectinimage)
		return false;

	AddIntAttrToNode(&Node_rectinimage, L"left", m_ImageProp.RectInImage.left);
	AddIntAttrToNode(&Node_rectinimage, L"top", m_ImageProp.RectInImage.top);
	AddIntAttrToNode(&Node_rectinimage, L"right", m_ImageProp.RectInImage.right);
	AddIntAttrToNode(&Node_rectinimage, L"bottom", m_ImageProp.RectInImage.bottom);

	xml_node Node_jgg = PropNode.append_child(L"jgg");
	if (!Node_jgg)
		return false;

	AddIntAttrToNode(&Node_jgg, L"left", m_ImageProp.jggInfo.left);
	AddIntAttrToNode(&Node_jgg, L"top", m_ImageProp.jggInfo.top);
	AddIntAttrToNode(&Node_jgg, L"right", m_ImageProp.jggInfo.right);
	AddIntAttrToNode(&Node_jgg, L"bottom", m_ImageProp.jggInfo.bottom);
	AddStringAttrToNode(&Node_jgg, L"drawmid", (m_ImageProp.bIsDrawJggMid ? L"1" : L"0"));

	xml_node Node_animation = PropNode.append_child(L"animation");
	if (!Node_animation)
		return false;

	AddIntAttrToNode(&Node_animation, L"playtype", m_ImageProp.ImgPlayType);
	AddIntAttrToNode(&Node_animation, L"looptype", m_ImageProp.ImgLoopType);
	AddIntAttrToNode(&Node_animation, L"bofangtype", m_ImageProp.ImgBoFangType);
	return true;
}

// 创建图片的内存DC
CDrawingImage* IPropertyImageBaseImpl::CreateDrawingImage()
{
	SafeDeleteDrawImage();

	m_bIsDrawImgCreate = true;
	m_pDrawImg = new CDrawingImage;
	if (m_pDrawImg == NULL)
		m_bIsDrawImgCreate = false;

	return m_pDrawImg;
}

bool IPropertyImageBaseImpl::InitStaticImage()
{
	if (GetUiKernel() == NULL || GetUiKernel()->GetSkinManager() == NULL)
		return false;

	if (m_pDrawImg == NULL || IS_INVALID_HANDLE(m_pDrawImg->GetSafeHdc()))
	{
		SafeDeleteDrawImage();
		// 普通图片
		if (m_pZipFile == NULL)
		{
			if (m_ImageProp.bIsZipFile)
			{
				BYTE *pBuffer = NULL;
				int nLen = 0;
				ZIP_FILE *pFindZipFile = NULL;
				if (!GetUiKernel()->GetSkinManager()->FindUnZipFile(m_ImageProp.pszSkinFilePath, m_ImageProp.strFileName, &pBuffer, &nLen, &pFindZipFile) || pFindZipFile == NULL)
					return false;

				m_pDrawImg = &pFindZipFile->DrawImg;
				if (m_pDrawImg != NULL && IS_INVALID_HANDLE(m_pDrawImg->GetSafeHdc()))
				{
					m_pDrawImg->CreateByMem(pBuffer, nLen);
					// 内存优化，运行时态删除文件内存
					if (m_pZipFile != NULL && IS_SAFE_HANDLE(m_pDrawImg->GetSafeHdc()) && !GetUiKernel()->IsDesignMode())
					{
						SAFE_DELETE_LIST(m_pZipFile->pFileData);
					}
				}
			}
			else
			{
				m_bIsDrawImgCreate = true;
				m_pDrawImg = new CDrawingImage;
				if (m_pDrawImg == NULL)
				{
					m_bIsDrawImgCreate = false;
					return false;
				}

				m_pDrawImg->CreateByFile(m_ImageProp.strFileName);
			}
		}
		else
		{
			m_pDrawImg = &(m_pZipFile->DrawImg);
			if (m_pDrawImg != NULL && IS_INVALID_HANDLE(m_pDrawImg->GetSafeHdc()))
			{
				// 内存优化，只有在使用的时候从磁盘读取
				if (m_pZipFile->pFileData == NULL)
					GetUiKernel()->GetSkinManager()->ReadSkinFileItem(m_pZipFile);

				m_pDrawImg->CreateByMem(m_pZipFile->pFileData, m_pZipFile->dwSrcFileLen);
				if (IS_SAFE_HANDLE(m_pDrawImg->GetSafeHdc()) && !GetUiKernel()->IsDesignMode())
				{
					SAFE_DELETE_LIST(m_pZipFile->pFileData);
				}
			}
		}
	}

	return (m_pDrawImg != NULL && IS_SAFE_HANDLE(m_pDrawImg->GetSafeHdc()));
}

bool IPropertyImageBaseImpl::DrawImage(CDrawingBoard &DstDc, RECT DstRct, int nAlpha)
{
	return DrawImage(DstDc.GetSafeHdc(), DstRct, nAlpha);
}

bool IPropertyImageBaseImpl::DrawImage(HDC hDc, RECT DstRct, int nAlpha)
{
	if (GetUiKernel() == NULL || GetUiKernel()->GetUiEngine() == NULL)
		return false;

	InitImage();

	if (m_ImageProp.ImgPlayType == IPT_GIF)
	{
		if (m_pGifImg == NULL || m_pGifImg->GetImage() == NULL)
			return false;
	}
	else
	{
		if (m_pDrawImg == NULL || IS_INVALID_HANDLE(m_pDrawImg->GetSafeHdc()))
			return false;
	}

	m_bIsTimerDrawEnd = true;
	if (m_ImageProp.ImgPlayType == IPT_STATIC_IMG)
	{
		// 静态图片
		if (IST_ALL_LASHEN == m_ImageProp.ImgShowType)
		{
			return DrawImage_AllLaShen(hDc, DstRct, nAlpha);
		}
		else if (IST_PINGPU == m_ImageProp.ImgShowType)
		{
			return DrawImage_PingPu(hDc, DstRct, nAlpha);
		}
		else if (IST_JGG_LASHEN == m_ImageProp.ImgShowType)
		{
			return DrawImage_JggLaShen(hDc, DstRct, nAlpha);
		}
	}
	else if (m_ImageProp.ImgPlayType == IPT_GIF)
	{
		Graphics DoGrap(hDc);
		Rect DstRect1 = Rect(DstRct.left, DstRct.top, RECT_WIDTH(DstRct), RECT_HEIGHT(DstRct));
		DoGrap.DrawImage(m_pGifImg->GetImage(), DstRect1, 0, 0, m_pGifImg->GetImageSize().cx, m_pGifImg->GetImageSize().cy, UnitPixel);
	}
	else if (m_ImageProp.ImgPlayType == IPT_IMAGE_XULIE)
	{
		// 序列图片
		InitDrawXuLieRect();

		int nDstWidth = RECT_WIDTH(DstRct);
		int nDstHeight = RECT_HEIGHT(DstRct);
		int nSrcWidth = RECT_WIDTH(m_rctXuLieDraw);
		int nSrcHeight = RECT_HEIGHT(m_rctXuLieDraw);
		if (nDstHeight <= 0 || nDstWidth <= 0 || nSrcWidth <= 0 || nSrcHeight <= 0)
			return true;

		return GetUiKernel()->GetUiEngine()->AlphaBlend(hDc, DstRct.left, DstRct.top, nDstWidth, nDstHeight,
			*m_pDrawImg, m_rctXuLieDraw.left, m_rctXuLieDraw.top, nSrcWidth, nSrcHeight, nAlpha);
	}

	return false;
}

bool IPropertyImageBaseImpl::DrawImage_AllLaShen(CDrawingBoard &DstDc, RECT DstRct, int nAlpha)
{
	return DrawImage_AllLaShen(DstDc.GetSafeHdc(), DstRct, nAlpha);
}

bool IPropertyImageBaseImpl::DrawImage_AllLaShen(HDC hDstDc, RECT DstRct, int nAlpha)
{
	if (hDstDc == NULL || GetUiKernel() == NULL || GetUiKernel()->GetUiEngine() == NULL || m_pDrawImg == NULL)
		return false;

	int nDstWidth = RECT_WIDTH(DstRct);
	int nDstHeight = RECT_HEIGHT(DstRct);
	int nSrcWidth = RECT_WIDTH(m_ImageProp.RectInImage);
	int nSrcHeight = RECT_HEIGHT(m_ImageProp.RectInImage);
	if (nDstHeight <= 0 || nDstWidth <= 0 || nSrcWidth <= 0 || nSrcHeight <= 0)
		return true;

	return GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.left, DstRct.top, nDstWidth, nDstHeight,
		*m_pDrawImg, m_ImageProp.RectInImage.left, m_ImageProp.RectInImage.top, nSrcWidth, nSrcHeight, nAlpha);
}

bool IPropertyImageBaseImpl::DrawImage_PingPu(CDrawingBoard &DstDc, RECT DstRct, int nAlpha)
{
	return DrawImage_PingPu(DstDc.GetSafeHdc(), DstRct, nAlpha);
}

bool IPropertyImageBaseImpl::DrawImage_PingPu(HDC hDstDc, RECT DstRct, int nAlpha)
{
	if (hDstDc == NULL || GetUiKernel() == NULL || GetUiKernel()->GetUiEngine() == NULL || m_pDrawImg == NULL)
		return false;

	int nDstWidth = RECT_WIDTH(DstRct);
	int nDstHeight = RECT_HEIGHT(DstRct);
	int nSrcWidth = RECT_WIDTH(m_ImageProp.RectInImage);
	int nSrcHeight = RECT_HEIGHT(m_ImageProp.RectInImage);
	if (nDstHeight <= 0 || nDstWidth <= 0 || nSrcWidth <= 0 || nSrcHeight <= 0)
		return true;

	for (int nZong = 0; nZong < nDstHeight; nZong += nSrcHeight)
	{
		for (int nHeng = 0; nHeng < nDstWidth; nHeng += nSrcWidth)
		{
			int nHengOffset = DstRct.left + nHeng;
			int nZongOffset = DstRct.top + nZong;
			int nSrcWidthOffset = nSrcWidth;
			int nSrcHeightOffset = nSrcHeight;

			if (nSrcHeightOffset > nDstHeight - nZong)
				nSrcHeightOffset = nDstHeight - nZong;

			if (nSrcWidthOffset > nDstWidth - nHeng)
				nSrcWidthOffset = nDstWidth - nHeng;

			// 横向绘制
			if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, nHengOffset, nZongOffset, nSrcWidthOffset, nSrcHeightOffset,
				*m_pDrawImg, m_ImageProp.RectInImage.left, m_ImageProp.RectInImage.top, nSrcWidthOffset, nSrcHeightOffset, nAlpha))
				return false;
		}
	}

	return true;
}

bool IPropertyImageBaseImpl::DrawImage_JggLaShen(CDrawingBoard &DstDc, RECT DstRct, int nAlpha)
{
	return DrawImage_JggLaShen(DstDc.GetSafeHdc(), DstRct, nAlpha);
}

bool IPropertyImageBaseImpl::DrawImage_JggLaShen(HDC hDstDc, RECT DstRct, int nAlpha)
{
	if (hDstDc == NULL || GetUiKernel() == NULL || GetUiKernel()->GetUiEngine() == NULL || m_pDrawImg == NULL)
		return false;

	int nDstWidth = RECT_WIDTH(DstRct);
	int nDstHeight = RECT_HEIGHT(DstRct);
	int nSrcWidth = RECT_WIDTH(m_ImageProp.RectInImage);
	int nSrcHeight = RECT_HEIGHT(m_ImageProp.RectInImage);
	if (nDstHeight <= 0 || nDstWidth <= 0 || nSrcWidth <= 0 || nSrcHeight <= 0)
		return true;

	if (m_ImageProp.jggInfo.left > 0)
	{
		// 左上角
		if (m_ImageProp.jggInfo.top > 0)
		{
			if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.left, DstRct.top,
				m_ImageProp.jggInfo.left, m_ImageProp.jggInfo.top,
				*m_pDrawImg, m_ImageProp.RectInImage.left, m_ImageProp.RectInImage.top,
				m_ImageProp.jggInfo.left, m_ImageProp.jggInfo.top, nAlpha))
				return false;
		}

		// 左下角
		if (m_ImageProp.jggInfo.bottom > 0)
		{
			if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.left, DstRct.bottom - m_ImageProp.jggInfo.bottom,
				m_ImageProp.jggInfo.left, m_ImageProp.jggInfo.bottom,
				*m_pDrawImg, m_ImageProp.RectInImage.left, m_ImageProp.RectInImage.bottom - m_ImageProp.jggInfo.bottom,
				m_ImageProp.jggInfo.left, m_ImageProp.jggInfo.bottom, nAlpha))
				return false;
		}

		// 左侧
		if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.left, DstRct.top + m_ImageProp.jggInfo.top,
			m_ImageProp.jggInfo.left, RECT_HEIGHT(DstRct) - m_ImageProp.jggInfo.top - m_ImageProp.jggInfo.bottom,
			*m_pDrawImg, m_ImageProp.RectInImage.left, m_ImageProp.RectInImage.top + m_ImageProp.jggInfo.top,
			m_ImageProp.jggInfo.left, RECT_HEIGHT(m_ImageProp.RectInImage) - m_ImageProp.jggInfo.top - m_ImageProp.jggInfo.bottom, nAlpha))
			return false;
	}

	if (m_ImageProp.jggInfo.right > 0)
	{
		// 右上角
		if (m_ImageProp.jggInfo.top > 0)
		{
			if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.right - m_ImageProp.jggInfo.right, DstRct.top,
				m_ImageProp.jggInfo.right, m_ImageProp.jggInfo.top,
				*m_pDrawImg, m_ImageProp.RectInImage.right - m_ImageProp.jggInfo.right, m_ImageProp.RectInImage.top,
				m_ImageProp.jggInfo.right, m_ImageProp.jggInfo.top, nAlpha))
				return false;
		}

		// 右下角
		if (m_ImageProp.jggInfo.bottom > 0)
		{
			if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.right - m_ImageProp.jggInfo.right, DstRct.bottom - m_ImageProp.jggInfo.bottom,
				m_ImageProp.jggInfo.right, m_ImageProp.jggInfo.bottom,
				*m_pDrawImg, m_ImageProp.RectInImage.right - m_ImageProp.jggInfo.right, m_ImageProp.RectInImage.bottom - m_ImageProp.jggInfo.bottom,
				m_ImageProp.jggInfo.right, m_ImageProp.jggInfo.bottom, nAlpha))
				return false;
		}

		// 右侧
		if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.right - m_ImageProp.jggInfo.right, DstRct.top + m_ImageProp.jggInfo.top,
			m_ImageProp.jggInfo.right, RECT_HEIGHT(DstRct) - m_ImageProp.jggInfo.top - m_ImageProp.jggInfo.bottom,
			*m_pDrawImg, m_ImageProp.RectInImage.right - m_ImageProp.jggInfo.right, m_ImageProp.RectInImage.top + m_ImageProp.jggInfo.top,
			m_ImageProp.jggInfo.right, RECT_HEIGHT(m_ImageProp.RectInImage) - m_ImageProp.jggInfo.top - m_ImageProp.jggInfo.bottom, nAlpha))
			return false;
	}

	// 上方
	if (m_ImageProp.jggInfo.top > 0)
	{
		if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.left + m_ImageProp.jggInfo.left, DstRct.top,
			RECT_WIDTH(DstRct) - m_ImageProp.jggInfo.left - m_ImageProp.jggInfo.right, m_ImageProp.jggInfo.top,
			*m_pDrawImg, m_ImageProp.RectInImage.left + m_ImageProp.jggInfo.left, m_ImageProp.RectInImage.top,
			RECT_WIDTH(m_ImageProp.RectInImage) - m_ImageProp.jggInfo.left - m_ImageProp.jggInfo.right, m_ImageProp.jggInfo.top, nAlpha))
			return false;
	}

	// 下方
	if (m_ImageProp.jggInfo.bottom > 0)
	{
		if (!GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.left + m_ImageProp.jggInfo.left, DstRct.bottom - m_ImageProp.jggInfo.bottom,
			RECT_WIDTH(DstRct) - m_ImageProp.jggInfo.left - m_ImageProp.jggInfo.right, m_ImageProp.jggInfo.bottom,
			*m_pDrawImg, m_ImageProp.RectInImage.left + m_ImageProp.jggInfo.left, m_ImageProp.RectInImage.bottom - m_ImageProp.jggInfo.bottom,
			RECT_WIDTH(m_ImageProp.RectInImage) - m_ImageProp.jggInfo.left - m_ImageProp.jggInfo.right, m_ImageProp.jggInfo.bottom, nAlpha))
			return false;
	}

	if (m_ImageProp.bIsDrawJggMid)
	{
		// 中间
		return GetUiKernel()->GetUiEngine()->AlphaBlend(hDstDc, DstRct.left + m_ImageProp.jggInfo.left, DstRct.top + m_ImageProp.jggInfo.top,
			RECT_WIDTH(DstRct) - m_ImageProp.jggInfo.left - m_ImageProp.jggInfo.right,
			RECT_HEIGHT(DstRct) - m_ImageProp.jggInfo.top - m_ImageProp.jggInfo.bottom,
			*m_pDrawImg, m_ImageProp.RectInImage.left + m_ImageProp.jggInfo.left, m_ImageProp.RectInImage.top + m_ImageProp.jggInfo.top,
			RECT_WIDTH(m_ImageProp.RectInImage) - m_ImageProp.jggInfo.left - m_ImageProp.jggInfo.right,
			RECT_HEIGHT(m_ImageProp.RectInImage) - m_ImageProp.jggInfo.top - m_ImageProp.jggInfo.bottom, nAlpha);
	}

	return true;
}

bool IPropertyImageBaseImpl::SetXuLieDrawInTimer()
{
	if (InitDrawXuLieRect())
		return true;

	if (m_pDrawImg == NULL)
		return false;

	if (m_ImageProp.ImgBoFangType == IBFT_ZHENGXIANG)
	{
		// 正向播放
		if (m_rctXuLieDraw.right >= m_pDrawImg->GetDcSize().cx)
		{
			if (m_ImageProp.ImgLoopType == ILT_LOOP_1)
				return false;

			INIT_RECT(m_rctXuLieDraw);
			InitDrawXuLieRect();
		}
		else
		{
			m_rctXuLieDraw.left += RECT_WIDTH(m_ImageProp.RectInImage);
			m_rctXuLieDraw.right = m_rctXuLieDraw.left + RECT_WIDTH(m_ImageProp.RectInImage);
		}
	}
	else
	{
		// 反向播放
		if (m_rctXuLieDraw.left <= 0)
		{
			if (m_ImageProp.ImgLoopType == ILT_LOOP_1)
				return false;

			INIT_RECT(m_rctXuLieDraw);
			InitDrawXuLieRect();
		}
		else
		{
			m_rctXuLieDraw.right = m_rctXuLieDraw.left;
			m_rctXuLieDraw.left = m_rctXuLieDraw.right - RECT_WIDTH(m_ImageProp.RectInImage);
		}
	}

	if (m_rctXuLieDraw.left < 0)
		m_rctXuLieDraw.left = 0;

	if (m_rctXuLieDraw.right < 0)
		m_rctXuLieDraw.right = 0;

	if (m_rctXuLieDraw.left > m_pDrawImg->GetDcSize().cx)
		m_rctXuLieDraw.left = m_pDrawImg->GetDcSize().cx;

	if (m_rctXuLieDraw.right > m_pDrawImg->GetDcSize().cx)
		m_rctXuLieDraw.right = m_pDrawImg->GetDcSize().cx;

	return true;
}

bool IPropertyImageBaseImpl::InitDrawXuLieRect()
{
	if (m_pDrawImg == NULL)
		return false;

	if (RECT_WIDTH(m_rctXuLieDraw) <= 0 || RECT_HEIGHT(m_rctXuLieDraw) <= 0)
	{
		if (m_ImageProp.ImgBoFangType == IBFT_ZHENGXIANG)
		{
			m_rctXuLieDraw.left = m_rctXuLieDraw.top = 0;
			m_rctXuLieDraw.right = RECT_WIDTH(m_ImageProp.RectInImage);
			m_rctXuLieDraw.bottom = RECT_HEIGHT(m_ImageProp.RectInImage);
		}
		else
		{
			m_rctXuLieDraw.right = m_pDrawImg->GetDcSize().cx;
			m_rctXuLieDraw.left = m_rctXuLieDraw.right - RECT_WIDTH(m_ImageProp.RectInImage);
			m_rctXuLieDraw.top = 0;
			m_rctXuLieDraw.bottom = RECT_HEIGHT(m_ImageProp.RectInImage);
		}
		return true;
	}
	return false;
}

bool IPropertyImageBaseImpl::OnDrawAnimation()
{
	if (m_ImageProp.ImgPlayType == IPT_STATIC_IMG)
		return false;

	// 已经计算完毕，等待刷新界面
	if (!m_bIsTimerDrawEnd)
		return true;

	if (m_ImageProp.ImgPlayType == IPT_GIF)
	{
		if (!SetGifDrawInTimer())
			return false;
	}
	else if (m_ImageProp.ImgPlayType == IPT_IMAGE_XULIE)
	{
		if (!SetXuLieDrawInTimer())
			return false;
	}
	else
	{
		return false;
	}

	m_bIsTimerDrawEnd = false;
	return true;
}

bool IPropertyImageBaseImpl::SetGifDrawInTimer()
{
	if (m_pGifImg == NULL || m_nGifCurFrameTime == -1)
		return false;

	m_nGifTimeCtns += UM_DFT_ANIMATION_TIMER_100S;
	if (m_ImageProp.ImgBoFangType == IBFT_ZHENGXIANG)
	{
		// 正向播放
		if (m_nGifTimeCtns >= m_nGifCurFrameTime)
		{
			m_nGifTimeCtns = 0;
			int nCur = m_pGifImg->GetCurrentPlayFrame() + 1;
			if (m_ImageProp.ImgLoopType == ILT_LOOP_1 && nCur >= m_pGifImg->GetFrameCounts())
				return false;

			m_nGifCurFrameTime = m_pGifImg->SetCurrentPlayFrame(nCur);
		}
	}
	else
	{
		// 反向播放
		if (m_nGifTimeCtns >= m_nGifCurFrameTime)
		{
			m_nGifTimeCtns = 0;
			int nCur = m_pGifImg->GetCurrentPlayFrame() - 1;
			if (m_ImageProp.ImgLoopType == ILT_LOOP_1 && nCur < 0)
				return false;

			if (nCur < 0)
				nCur = m_pGifImg->GetFrameCounts() - 1;

			m_nGifCurFrameTime = m_pGifImg->SetCurrentPlayFrame(nCur);
		}
	}

	return true;
}

void IPropertyImageBaseImpl::InitGifImage()
{
	if (m_pGifImg == NULL || m_pGifImg->GetImage() != NULL || GetUiKernel() == NULL || GetUiKernel()->GetSkinManager() == NULL)
		return;

	// 普通图片
	if (m_pZipFile == NULL)
	{
		if (m_ImageProp.bIsZipFile)
		{
			BYTE *pBuffer = NULL;
			int nLen = 0;
			if (!GetUiKernel()->GetSkinManager()->FindUnZipFile(m_ImageProp.pszSkinFilePath, m_ImageProp.strFileName, &pBuffer, &nLen, NULL))
				return;

			m_pGifImg->CreateByMem(pBuffer, nLen);
		}
		else
		{
			m_pGifImg->CreateByFile(m_ImageProp.strFileName);
		}
	}
	else
	{
		// 内存优化，只有在使用的时候从磁盘读取
		if (m_pZipFile->pFileData == NULL)
			GetUiKernel()->GetSkinManager()->ReadSkinFileItem(m_pZipFile);

		m_pGifImg->CreateByMem(m_pZipFile->pFileData, m_pZipFile->dwSrcFileLen);
	}

	m_nGifTimeCtns = 0;
	if (m_ImageProp.ImgBoFangType == IBFT_ZHENGXIANG)
	{
		// 正向播放
		m_nGifCurFrameTime = m_pGifImg->SetCurrentPlayFrame(0);
	}
	else
	{
		// 反向播放
		m_nGifCurFrameTime = m_pGifImg->SetCurrentPlayFrame(m_pGifImg->GetFrameCounts() - 1);
	}
}

// 得到图片的内存DC
CDrawingImage* IPropertyImageBaseImpl::GetDrawingImage()
{
	return m_pDrawImg;
}

// 生成缩略图
IPropertyImageBaseImpl* IPropertyImageBaseImpl::CreateThumbnailImage(SIZE sizeThumbnail)
{
	if (m_pUiKernel == NULL || m_pUiKernel->GetSkinManager() == NULL || sizeThumbnail.cx <= 0 || sizeThumbnail.cy <= 0)
		return NULL;

	this->InitImage();
	if (m_pDrawImg == NULL || m_pDrawImg->GetSafeHdc() == NULL)
		return NULL;

	IPropertyImageBaseImpl* pNewImgBase = (IPropertyImageBaseImpl*)m_pUiKernel->GetSkinManager()->CreateEmptyBaseProp(m_pUiKernel->GetSkinManager()->GetLocalImageSkinFileItem(), OTID_IMAGE_BASE);
	if (pNewImgBase == NULL)
		return NULL;

	WCHAR szObjId[MAX_PATH] = {};
	swprintf_s(szObjId, MAX_PATH, L"%d", GetObjectId());
	pNewImgBase->SetObjectName(szObjId);

	CDrawingImage* pDrawingImg = pNewImgBase->CreateDrawingImage();
	if (pDrawingImg == NULL)
	{
		m_pUiKernel->GetSkinManager()->DeleteImageBase(m_pUiKernel->GetSkinManager()->GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	pDrawingImg->Create(sizeThumbnail.cx, sizeThumbnail.cy);
	if (pDrawingImg->GetSafeHdc() == NULL)
	{
		m_pUiKernel->GetSkinManager()->DeleteImageBase(m_pUiKernel->GetSkinManager()->GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	RECT fromRct = {0, 0, m_pDrawImg->GetDcSize().cx, m_pDrawImg->GetDcSize().cy};
	RECT toRct = {0, 0, sizeThumbnail.cx, sizeThumbnail.cy};
	if (!m_pDrawImg->AlphaBlendTo(*pDrawingImg, toRct, fromRct, m_pUiKernel))
	{
		m_pUiKernel->GetSkinManager()->DeleteImageBase(m_pUiKernel->GetSkinManager()->GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	IMAGE_BASE_PROP LocalImgProp;
	InitPropImageBase(&LocalImgProp);
	LocalImgProp.bIsZipFile = false;
	memset(LocalImgProp.strFileName, 0, MAX_PATH + 1);
	LocalImgProp.ImgShowType = IST_ALL_LASHEN;
	LocalImgProp.RectInImage.left = LocalImgProp.RectInImage.top = 0;
	LocalImgProp.RectInImage.right = pDrawingImg->GetDcSize().cx;
	LocalImgProp.RectInImage.bottom = pDrawingImg->GetDcSize().cy;
	pNewImgBase->SetImageProp(&LocalImgProp);

	return pNewImgBase;
}

// 删除缩略图
bool IPropertyImageBaseImpl::DeleteThumbnailImage(IPropertyImageBase* pImgBase)
{
	if (pImgBase == NULL || m_pUiKernel == NULL || m_pUiKernel->GetSkinManager() == NULL)
		return false;

	return m_pUiKernel->GetSkinManager()->DeleteImageBase(m_pUiKernel->GetSkinManager()->GetLocalImageSkinFileItem(), pImgBase);
}

// 从另一个属性克隆
bool IPropertyImageBaseImpl::Clone(IPropertyBase* pFromPropBase)
{
	if (!IsSameTypePropetry(dynamic_cast<IPropertyBase*>(this), pFromPropBase))
		return false;

	IPropertyImageBaseImpl* pFromProp = dynamic_cast<IPropertyImageBaseImpl*>(pFromPropBase);
	if (pFromProp == NULL)
		return false;

	memcpy(&m_ImageProp, &(pFromProp->m_ImageProp), sizeof(IMAGE_BASE_PROP));
	return true;
}
