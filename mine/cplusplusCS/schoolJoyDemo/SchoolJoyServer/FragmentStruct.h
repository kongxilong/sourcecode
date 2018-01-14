// FragmentStruct.h: interface for the FragmentStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAGMENTSTRUCT_H__1BC73C33_B165_4170_B640_5A8D910CEC32__INCLUDED_)
#define AFX_FRAGMENTSTRUCT_H__1BC73C33_B165_4170_B640_5A8D910CEC32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct  
{
	char fileName [100];
	int orderId;
	long startIndex;
	int order_offet_set;
	long endIndex;
	int totalFragment;
} FragmentStruct;

#endif // !defined(AFX_FRAGMENTSTRUCT_H__1BC73C33_B165_4170_B640_5A8D910CEC32__INCLUDED_)
