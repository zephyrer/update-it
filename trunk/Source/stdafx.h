// UpdateIt! application.
// Copyright (c) 2002-2009 by Elijah Zarezky,
// All rights reserved.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// stdafx.h - include file for standard system include files

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__stdafx_h)
#define __stdafx_h

//////////////////////////////////////////////////////////////////////////////////////////////
// unwanted warnings

// unreferenced inline/local function has been removed
#pragma warning(disable: 4514)
// function not inlined
#pragma warning(disable: 4710)
// identifier was truncated in the debug information
#pragma warning(disable: 4786)

// C4996: function or variable may be unsafe
#define _CRT_SECURE_NO_WARNINGS

//////////////////////////////////////////////////////////////////////////////////////////////
// use WinXP/IE6 features

#define WINVER 0x0501
#define _WIN32_WINDOWS 0x0500
#define _WIN32_IE 0x0600
#define _WIN32_WINNT 0x0501

//////////////////////////////////////////////////////////////////////////////////////////////
// remove automatic CRT/MFC dependencies generation

// see http://www.codeproject.com/KB/cpp/PrivateAssemblyProjects.aspx and
// http://blog.m-ri.de/index.php/2008/05/06/hotfix-fuer-usemsprivateassembliesh-und-vc-2008/
// by by Martin Richter for more information

#define _STL_NOFORCE_MANIFEST
#define _CRT_NOFORCE_MANIFEST
#define _AFX_NOFORCE_MANIFEST
#define _ATL_NOFORCE_MANIFEST

#ifdef __cplusplus
extern "C"
{
#endif

__declspec(selectany) int _forceCRTManifest;
__declspec(selectany) int _forceMFCManifest;
__declspec(selectany) int _forceAtlDllManifest;

// the next symbols are used by the several versions of VC++ 9.0
__declspec(selectany) int _forceCRTManifestRTM;
__declspec(selectany) int _forceMFCManifestRTM;
__declspec(selectany) int _forceMFCManifestCUR;

#ifdef __cplusplus
}   // extern "C"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// MFC headers

#if (_MSC_VER >= 1300)
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#endif	// _MSC_VER

#include <afxwin.h>        // MFC core and standard components
#include <afxdlgs.h>       // MFC standard dialogs
#include <afxdtctl.h>      // MFC date/time control classes
#include <afxcmn.h>        // MFC common control classes
#include <afxpriv.h>       // MFC private classes
#include <afxsock.h>       // MFC support for Windows Sockets
#include <afxinet.h>       // MFC support for WinInet
#include <afxdisp.h>       // MFC IDispatch & ClassFactory support

#include <../src/mfc/afximpl.h>

//////////////////////////////////////////////////////////////////////////////////////////////
// PSDK headers

#include <shlwapi.h>       // light-weight utility APIs
#include <wincrypt.h>      // cryptographic API prototypes and definitions
#include <windns.h>        // DNS definitions and DNS API

#include <dbghelp.h>       // debugging support routines that are redistributable

//////////////////////////////////////////////////////////////////////////////////////////////
// ATL headers

#include <atlbase.h>       // basic ATL classes
#include <atlfile.h>       // CAtlFile implementation

//////////////////////////////////////////////////////////////////////////////////////////////
// STL headers

#pragma warning(push, 3)

#include <string>
#include <algorithm>

#pragma warning(pop)

#if !defined(_STRING_)
#define _STRING_           // for compatibility with Dinkumware STL
#endif

#if !defined(min)
using std::min;
#endif	// min

#if !defined(max)
using std::max;
#endif	// max

//////////////////////////////////////////////////////////////////////////////////////////////
// CRT headers

#include <sys/utime.h>
#include <locale.h>

//////////////////////////////////////////////////////////////////////////////////////////////
// additional headers

#include "../../Repository/CodeProject/Source/HyperLink.h"
#include "../../Repository/CodeProject/Source/FolderDlg.h"
#include "../../Repository/CodeProject/Source/XColorStatic.h"

#pragma warning(push)
#include "../../Repository/ZipArchive/Source/ZipArchive.h"
#pragma warning(pop)

#include "../../Repository/AfxGadgets/Source/ddxv.h"
#include "../../Repository/AfxGadgets/Source/SortingListCtrl.h"
#include "../../Repository/AfxGadgets/Source/WinCrypto.h"
#include "../../Repository/AfxGadgets/Source/Win32Error.h"
#include "../../Repository/AfxGadgets/Source/StringConv.h"
#include "../../Repository/AfxGadgets/Source/MemMapFile.h"
#include "../../Repository/AfxGadgets/Source/ProcessPrivileges.h"
#include "../../Repository/AfxGadgets/Source/ArgsParser.h"
#include "../../Repository/AfxGadgets/Source/CustomResource.h"

#include "../../Repository/Naughter/Source/AppSettings.h"

// Microsoft PSDK version 6.0A conflicts with OpenSSL version 0.9.8h:
// <wincrypt.h> contains "#define OCSP_RESPONSE ((LPCSTR) 67)" and
// <ossl_typ.h> contains "typedef struct ocsp_response_st OCSP_RESPONSE;"
// hence we use the following trick to avoid compilation errors

#if defined(OCSP_RESPONSE)
#pragma push_macro("OCSP_RESPONSE")
#undef OCSP_RESPONSE
#define POP_MACRO_OCSP_RESPONSE
#endif   // OCSP_RESPONSE
#include "../../Repository/Naughter/Source/PJNSMTP.h"
#if defined(POP_MACRO_OCSP_RESPONSE)
#pragma pop_macro("OCSP_RESPONSE")
#endif   // POP_MACRO_OCSP_RESPONSE

#if (_MFC_VER < 0x0700)
#include "../../Repository/atlmfc71/Source/FileDialogEx.h"
#endif   // _MFC_VER

#include <FreeImagePlus.h>

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH generation finish

#pragma hdrstop

//////////////////////////////////////////////////////////////////////////////////////////////
// aliases

#if defined(CSMTPConnection)
#undef CSMTPConnection
#endif   // CSMTPConnection

typedef CPJNSMTPConnection CSmtpConnection;
typedef CPJNSMTPMessage CSmtpMessage;
typedef CPJNSMTPBodyPart CSmtpBodyPart;
typedef CPJNSMTPAddress CSmtpAddress;
typedef CPJNSMTPException CSmtpException;

//////////////////////////////////////////////////////////////////////////////////////////////
// file data

struct FILE_DATA
{
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szFolder[_MAX_DIR];
	CTime timeWrite;
#if (_MFC_VER < 0x0700)
	DWORD cbLength;
#else
	ULONGLONG cbLength;
#endif   // _MFC_VER
};

//////////////////////////////////////////////////////////////////////////////////////////////
// shared globals

extern bool g_fRestartInterface;

//////////////////////////////////////////////////////////////////////////////////////////////
// global helpers

void DDV_MinMaxChars(CDataExchange* pDX, CString const& strValue, int cMinChars, int cMaxChars);

//////////////////////////////////////////////////////////////////////////////////////////////
// some tricks

#if !defined(_countof)
#define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif   // _countof

#if defined(ZIPSTRING_DOT_H) && defined(for)
#undef for
#endif   // ZIPSTRING_DOT_H && for

// force ISO/IEC 14882 conformance in for loop scope
#if (_MSC_VER < 1300)
#define for if (false); else for
#else
#pragma conform(forScope, on)
#endif   // _MSC_VER

// helper macro STRINGIZE
// converts the parameter 'text' to a string after
// macro replacement on 'text' has been performed
#define STRINGIZE(text) STRINGIZE_IMPL(text)
#define STRINGIZE_IMPL(text) #text

#endif   // __stdafx_h

// end of file
