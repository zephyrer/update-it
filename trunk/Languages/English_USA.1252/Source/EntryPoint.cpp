// English_USA.1252 MFC extension DLL.
// Copyright (c) 2007-2011 by Elijah Zarezky,
// All rights reserved.

// EntryPoint.cpp - library entry point

// initially generated by AfxScratch v1.0.3839 on 04.08.2007 at 11:11:56
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include <afxdllx.h>			// MFC extension DLL helpers

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// state of the extension DLL module

AFX_EXTENSION_MODULE g_afxModule = { FALSE, NULL };

//////////////////////////////////////////////////////////////////////////////////////////////
// entry point

extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, void* /*pvReserved*/)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		TRACE0("English_USA.1252.DLL Initializing!\n");
		// perform one-time initialization
		VERIFY(AfxInitExtensionModule(g_afxModule, hInst));
		// insert this DLL into the resource chain
		new CDynLinkLibrary(g_afxModule);
		// successfully attached
		return (TRUE);
	case DLL_PROCESS_DETACH:
		TRACE0("English_USA.1252.DLL Terminating!\n");
		// perform per-process termination
		AfxTermExtensionModule(g_afxModule);
		// successfully detached
		return (TRUE);
	default:
		return (TRUE);
	}
}

// end of file
