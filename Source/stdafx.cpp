// UpdateIt! application.
// Copyright (c) 2002-2011 by Elijah Zarezky,
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

// stdafx.cpp - source file that includes just the standard includes

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// shared globals

bool g_fRestartInterface = false;

//////////////////////////////////////////////////////////////////////////////////////////////
// global helpers

#if (_MSC_VER >= 1300) && defined(_DLL)

extern "C" __int64 __cdecl _abs64(__int64 num)
{
	return (num < 0 ? -num : num);
}

#endif   // _MSC_VER

void DDV_MinMaxChars(CDataExchange* pDX, CString const& strValue, int cMinChars, int cMaxChars)
{
	if (pDX->m_bSaveAndValidate && strValue.GetLength() < cMinChars)
	{
		TCHAR szMinCHarsStr[32] = { 0 };
		wsprintf(szMinCHarsStr, _T("%d"), cMinChars);
		CString strPrompt;
		AfxFormatString1(strPrompt, IDS_PARSE_MIN_STRING_SIZE, szMinCHarsStr);
		AfxMessageBox(strPrompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		strPrompt.Empty();
		pDX->Fail();
	}
	else
	{
		DDV_MaxChars(pDX, strValue, cMaxChars);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// import libraries

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")

#pragma comment(lib, "dbghelp.lib")

//////////////////////////////////////////////////////////////////////////////////////////////
// Detours libraries

#if defined(UPDATEIT_DETOURED)
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "detoured.lib")
#endif   // UPDATEIT_DETOURED

//////////////////////////////////////////////////////////////////////////////////////////////
// linker options

#if (_MSC_VER < 1300)
#pragma comment(linker, "/osversion:4.1")
#pragma comment(linker, "/subsystem:windows,4.10")
// all references to DLL discarded by /opt:ref
#pragma comment(linker, "/ignore:4089")
#endif   // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////////
// entry-point symbol for the Unicode builds

#if defined(UNICODE) || defined(_UNICODE)
#pragma comment(linker, "/entry:wWinMainCRTStartup")
#endif   // UNICODE

//////////////////////////////////////////////////////////////////////////////////////////////
// manifest dependencies

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

// VC-2008 RTM                       9.0.21022.8
// VC-2008 Feature Pack              9.0.30411.0
// VC-2008 SP1                       9.0.30729.1
// VC-2008 SP1 ATL Security Update   9.0.30729.4148
// VC-2008 SP1 MFC Security Update   9.0.30729.5570
// VC-2008 SP1 CRT Security Update   9.0.30729.6161

#if defined(_DEBUG)
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.VC90.DebugCRT' version='9.0.30729.6161' processorArchitecture='x86' publicKeyToken='1fc8b3b9a1e18e3b'\"")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.VC90.DebugMFC' version='9.0.30729.6161' processorArchitecture='x86' publicKeyToken='1fc8b3b9a1e18e3b'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.VC90.PrivateCRT' version='9.0.30729.6161' processorArchitecture='x86' publicKeyToken='1fc8b3b9a1e18e3b'\"")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.VC90.PrivateMFC' version='9.0.30729.6161' processorArchitecture='x86' publicKeyToken='1fc8b3b9a1e18e3b'\"")
#endif   // _DEBUG

// end of file
