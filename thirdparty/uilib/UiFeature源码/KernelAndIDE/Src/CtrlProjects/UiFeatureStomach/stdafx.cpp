// stdafx.cpp : source file that includes just the standard includes
// FdemPrime.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include <atlbase.h>
#include "..\..\3rd\WTL80\atlapp.h"
using namespace ATL;

CAppModule _Module;

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			_Module.Init(NULL, hModule);
			break;
		}

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			_Module.Term();
		}
		break;
	}
	return TRUE;
}
