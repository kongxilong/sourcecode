
#include "StdAfx.h"
#include "Function.h"
#include "MainFrm.h"

SKIN_FILE_ITEM* GetSkinFileItem()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (pMain == NULL)
		return NULL;

	SKIN_FILE_ITEM* pSkinItem = pMain->GetSkinFileItem();

	return pSkinItem;
}
