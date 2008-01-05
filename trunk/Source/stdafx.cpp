// UpdateIt! application.
// Copyright (c) 2002-2008 by Elijah Zarezky,
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

#endif	// _MSC_VER

// ripped from %VCINSTALLDIR%\Vc7\atlmfc\src\mfc\dlgdata.cpp
void AFX_CDECL DDX_TextWithFormat(CDataExchange* pDX, int nIDC, LPCTSTR pszFormat, UINT idsPrompt, ...)
{
	va_list argPtr;
	va_start(argPtr, idsPrompt);

	HWND hEditWnd = pDX->PrepareEditCtrl(nIDC);
   ASSERT(hEditWnd != NULL);

	TCHAR szText[64] = { 0 };

	if (pDX->m_bSaveAndValidate)
	{
		void* pResult = va_arg(argPtr, void*);
		::GetWindowText(hEditWnd, szText, _countof(szText));
		if (_stscanf(szText, pszFormat, pResult) != 1)
		{
			AfxMessageBox(idsPrompt);
			pDX->Fail();
		}
	}
	else {
		_vstprintf(szText, pszFormat, argPtr);
		AfxSetWindowText(hEditWnd, szText);
	}

	va_end(argPtr);
}

void DDX_Text(CDataExchange* pDX, int nIDC, WORD& value)
{
	HWND hEditWnd = pDX->PrepareEditCtrl(nIDC);
   ASSERT(hEditWnd != NULL);

	TCHAR szText[64] = { 0 };

	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hEditWnd, szText, _countof(szText));
		if (_stscanf(szText, _T("%hu"), &value) != 1)
		{
			AfxMessageBox(AFX_IDP_PARSE_UINT);
			pDX->Fail();
		}
	}
	else {
		_stprintf(szText, _T("%hu"), value);
		AfxSetWindowText(hEditWnd, szText);
	}
}

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
	else {
		DDV_MaxChars(pDX, strValue, cMaxChars);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// import libraries

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

#if defined(_DEBUG)
#pragma comment(lib, "ssleay32MDd.lib")
#pragma comment(lib, "libeay32MDd.lib")
#else
#pragma comment(lib, "ssleay32MD.lib")
#pragma comment(lib, "libeay32MD.lib")
#endif	// _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// linker options

#if (_MSC_VER < 1300)
#pragma comment(linker, "/osversion:4.1")
#pragma comment(linker, "/subsystem:windows,4.10")
// all references to DLL discarded by /opt:ref
#pragma comment(linker, "/ignore:4089")
#endif	// _MSC_VER

// end of file
