// CrashReporter application.
// Copyright (c) 2009-2010 by Elijah Zarezky,
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

// GetFileSizeAsString.cpp  Version 1.0
//
// Author:                  Hans Dietrich
//                          hdietrich2@hotmail.com
//
// Modified by:             Elijah Zarezky
//                          elijah.zarezky@gmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like, except
// that you may not sell this source code.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.

#include "stdafx.h"
#include <io.h>
#include "GetFileSizeAsString.h"
#include "FormatNumberForLocale.h"

BOOL GetFileSizeAsString(LPCTSTR lpszFile, CString& rSize)
{
	BOOL bRet = TRUE;

	rSize = _T("");

	// skip file if it doesn't exist in our directory
	if (_taccess(lpszFile, 00) == -1)
		return FALSE;

	// open file to get its size
	HANDLE hFile = CreateFile(lpszFile,
							  GENERIC_READ,
							  0,
							  NULL,
							  OPEN_EXISTING,
							  FILE_ATTRIBUTE_NORMAL,
							  NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE(_T("ERROR - failed to open %s\n"), lpszFile);
		return FALSE;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);

	if (dwFileSize == INVALID_FILE_SIZE || dwFileSize == 0)
	{
		TRACE(_T("ERROR - GetFileSize failed or returned 0\n"));
		bRet = FALSE;
	}
	else
	{
		// convert bytes to KB
		dwFileSize += 512;
		dwFileSize = dwFileSize / 1024;
		if (dwFileSize == 0)
			dwFileSize = 1;

		rSize.Format(_T("%u"), dwFileSize);

		CString str = CFormatNumberForLocale::FormatNumber(rSize);
		rSize = str;
		rSize += _T(" KB ");
	}

	CloseHandle(hFile);

	return bRet;
}
