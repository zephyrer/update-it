// Russian_Russia.1251 MFC extension DLL.
// Copyright (c) 2007-2008 by Elijah Zarezky,
// All rights reserved.

// stdafx.h - include file for standard system include files

// initially generated by AfxScratch v1.0.3839 on 04.08.2007 at 11:12:50
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

#if !defined(__stdafx_h)
#define __stdafx_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

// unreferenced inline/local function has been removed
#pragma warning(disable: 4514)
// function not inlined
#pragma warning(disable: 4710)
// identifier was truncated in the debug information
#pragma warning(disable: 4786)

#define WINVER 0x0400
#define _WIN32_WINDOWS 0x0410
#define _WIN32_IE 0x0400
#define _WIN32_WINNT 0x0400

#if (_MSC_VER >= 1300)
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#endif	// _MSC_VER

// MFC headers
#include <afxwin.h>			// MFC core and standard components

// force ISO/IEC 14882 conformance in for loop scope
#if (_MSC_VER < 1300)
#define for if (false); else for
#else
#pragma conform(forScope, on)
#endif	// _MSC_VER

#endif	// __stdafx_h

// end of file
