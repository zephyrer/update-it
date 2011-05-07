// CrashReporter application.
// Copyright (c) 2009-2011 by Elijah Zarezky,
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

// BackupFile.cpp  Version 1.0
//
// Author:         Hans Dietrich
//                 hdietrich2@hotmail.com
//
// Modified by:    Elijah Zarezky
//                 elijah.zarezky@gmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like, except
// that you may not sell this source code.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.

#include "stdafx.h"
#include "BackupFile.h"
#include <io.h>

BOOL BackupFile(LPCTSTR lpszFile)
{
	ASSERT(lpszFile);

	// if file doesn't exist, nothing to do
	if (_taccess(lpszFile, 00) == -1)
		return TRUE;

	// file exists, so make a backup copy

	CString strFileRoot = lpszFile;

	CString strPath = _T("");
	CString strExt = _T("");
	BOOL bSuccess = FALSE;
	for (int i = 1; i <= 9999; i++)
	{
		strExt.Format(_T(".%04d"), i);
		strPath = strFileRoot;
		strPath += strExt;
		if (_taccess(strPath, 00) == -1)
		{
			// file doesn't exist, so we can use it
			bSuccess = CopyFile(lpszFile, strPath, TRUE);
			break;
		}
	}

	return bSuccess;
}
