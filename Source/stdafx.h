// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// stdafx.h - include file for standard system include files

#if !defined(__stdafx_h)
#define __stdafx_h

#pragma warning(disable: 4514)	// unreferenced inline/local function has been removed
#pragma warning(disable: 4710)	// function not inlined
#pragma warning(disable: 4786)	// identifier was truncated in the debug information

// use Win98/NT4 features
#define _WIN32_WINNT 0x0400

// MFC headers
#include <afxwin.h>
#include <afxdlgs.h>
#include <afxdtctl.h>
#include <afxcmn.h>
#include <afxpriv.h>
#include <afxsock.h>
#include <afxinet.h>

// PSDK headers
#include <shlwapi.h>
#include <wincrypt.h>

// STL headers
#pragma warning(push, 3)
#include <string>
#if !defined(_STRING_)
#define _STRING_			// for compatibility with Dinkumware STL
#endif
#include <algorithm>
using std::min;
using std::max;
#pragma warning(pop)

// CRT headers
#include <sys/utime.h>

// additional headers
#pragma warning(push)
#include "../../Repository/ZipArchive/Source/ZipArchive.h"
#pragma warning(pop)
#include "../../Repository/Naughter/Source/Smtp.h"
#include "../../Repository/CodeProject/Source/HyperLink.h"
#include "../../Repository/AfxGadgets/Source/WinCrypto.h"
#include "../../Repository/AfxGadgets/Source/Win32Error.h"
#include "../../Repository/AfxGadgets/Source/StringConv.h"

#pragma hdrstop

// file data
struct FILE_DATA {
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szFolder[_MAX_DIR];
	CTime timeWrite;
	DWORD cbLength;
};

#endif	// __stdafx_h

// end of file
