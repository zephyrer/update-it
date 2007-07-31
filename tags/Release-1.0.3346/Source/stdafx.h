// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
// All rights reserved.

// stdafx.h - include file for standard system include files

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

// use Win2000 features
#define _WIN32_WINNT 0x0500

// MFC headers
#include <afxwin.h>			// MFC core and standard components
#include <afxdlgs.h>			// MFC standard dialogs
#include <afxdtctl.h>		// MFC date/time control classes
#include <afxcmn.h>			// MFC common control classes
#include <afxpriv.h>			// MFC private classes
#include <afxsock.h>			// MFC support for Windows Sockets
#include <afxinet.h>			// MFC support for WinInet

// PSDK headers
#include <shlwapi.h>			// light-weight utility APIs
#include <wincrypt.h>		// cryptographic API prototypes and definitions
#include <windns.h>			// DNS definitions and DNS API

// STL headers
#pragma warning(push, 3)
#include <string>
#include <algorithm>
#pragma warning(pop)

#if !defined(_STRING_)
#define _STRING_			// for compatibility with Dinkumware STL
#endif

#if !defined(min)
using std::min;
#endif	// min

#if !defined(max)
using std::max;
#endif	// max

// CRT headers
#include <sys/utime.h>
#include <locale.h>

// additional headers
#include "../../Repository/CodeProject/Source/HyperLink.h"
#include "../../Repository/CodeProject/Source/FolderDlg.h"
#pragma warning(push)
#include "../../Repository/ZipArchive/Source/ZipArchive.h"
#pragma warning(pop)
#include "../../Repository/AfxGadgets/Source/SortingListCtrl.h"
#include "../../Repository/AfxGadgets/Source/WinCrypto.h"
#include "../../Repository/AfxGadgets/Source/Win32Error.h"
#include "../../Repository/AfxGadgets/Source/StringConv.h"
#include "../../Repository/AfxGadgets/Source/MemMapFile.h"
#include "../../Repository/AfxGadgets/Source/ProcessPrivileges.h"
#include "../../Repository/Naughter/Source/Smtp.h"

#if (_MFC_VER < 0x0700)
#include "../../Repository/atlmfc71/Source/FileDialogEx.h"
#endif	// _MFC_VER

#pragma hdrstop

#if defined(CSMTPConnection)
#undef CSMTPConnection
#endif	// CSMTPConnection
typedef CPJNSMTPConnection CSmtpConnection;
typedef CPJNSMTPMessage CSmtpMessage;
typedef CPJNSMTPBodyPart CSmtpBodyPart;
typedef CPJNSMTPAddress CSmtpAddress;
typedef CPJNSMTPException CSmtpException;

// file data
struct FILE_DATA {
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szFolder[_MAX_DIR];
	CTime timeWrite;
#if (_MFC_VER < 0x0700)
	DWORD cbLength;
#else
	ULONGLONG cbLength;
#endif	// _MFC_VER
};

#if defined(ZIPSTRING_DOT_H) && defined(for)
#undef for
#endif	// ZIPSTRING_DOT_H && for

// force ISO/IEC 14882 conformance in for loop scope
#if (_MSC_VER < 1300)
#define for if (false); else for
#else
#pragma conform(forScope, on)
#endif	// _MSC_VER

#endif	// __stdafx_h

// end of file