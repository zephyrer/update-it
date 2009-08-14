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

// GetWinVer.cpp  Version 1.1
//
// Copyright (C) 2001-2003 Hans Dietrich
//
// Modified by:  Elijah Zarezky
//               elijah.zarezky@gmail.com
//
// This software is released into the public domain.  
// You are free to use it in any way you like, except
// that you may not sell this source code.
//
// This software is provided "as is" with no expressed 
// or implied warranty.  I accept no liability for any 
// damage or loss of business that this software may cause. 

#include "stdafx.h"

#include "GetWinVer.h"


// from winbase.h
#ifndef VER_PLATFORM_WIN32s
#define VER_PLATFORM_WIN32s             0
#endif
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS      1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT           2
#endif
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE           3
#endif


/*
    This table has been assembled from Usenet postings, personal
    observations, and reading other people's code.  Please feel
    free to add to it or correct it.


         dwPlatFormID  dwMajorVersion  dwMinorVersion  dwBuildNumber
95             1              4               0             950
95 SP1         1              4               0        >950 && <=1080
95 OSR2        1              4             <10           >1080
98             1              4              10            1998
98 SP1         1              4              10       >1998 && <2183
98 SE          1              4              10          >=2183
ME             1              4              90            3000

NT 3.51        2              3              51
NT 4           2              4               0            1381
2000           2              5               0            2195
XP             2              5               1            2600
2003 Server    2              5               2            3790
Vista          2              6               0            6000
2008 Server    2              6               1            6001
Vista SP1      2              6               1            6001

CE             3

*/

///////////////////////////////////////////////////////////////////////////////
// GetWinVer
BOOL GetWinVer(LPTSTR pszVersion, int *nVersion, LPTSTR pszMajorMinorBuild)
{
	if (!pszVersion || !nVersion || !pszMajorMinorBuild)
		return FALSE;
	_tcscpy(pszVersion, WUNKNOWNSTR);
	*nVersion = WUNKNOWN;

	OSVERSIONINFOEX osinfo = { 0 };
	osinfo.dwOSVersionInfoSize = sizeof(osinfo);

	if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osinfo)))
		return FALSE;

	DWORD dwPlatformId   = osinfo.dwPlatformId;
	DWORD dwMinorVersion = osinfo.dwMinorVersion;
	DWORD dwMajorVersion = osinfo.dwMajorVersion;
	DWORD dwBuildNumber  = osinfo.dwBuildNumber & 0xFFFF;	// Win 95 needs this

	wsprintf(pszMajorMinorBuild, _T("%u.%u.%u"), dwMajorVersion, dwMinorVersion, dwBuildNumber);

	if ((dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4))
	{
		if ((dwMinorVersion < 10) && (dwBuildNumber == 950))
		{
			_tcscpy(pszVersion, W95STR);
			*nVersion = W95;
		}
		else if ((dwMinorVersion < 10) && 
				((dwBuildNumber > 950) && (dwBuildNumber <= 1080)))
		{
			_tcscpy(pszVersion, W95SP1STR);
			*nVersion = W95SP1;
		}
		else if ((dwMinorVersion < 10) && (dwBuildNumber > 1080))
		{
			_tcscpy(pszVersion, W95OSR2STR);
			*nVersion = W95OSR2;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber == 1998))
		{
			_tcscpy(pszVersion, W98STR);
			*nVersion = W98;
		}
		else if ((dwMinorVersion == 10) && 
				((dwBuildNumber > 1998) && (dwBuildNumber < 2183)))
		{
			_tcscpy(pszVersion, W98SP1STR);
			*nVersion = W98SP1;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber >= 2183))
		{
			_tcscpy(pszVersion, W98SESTR);
			*nVersion = W98SE;
		}
		else if (dwMinorVersion == 90)
		{
			_tcscpy(pszVersion, WMESTR);
			*nVersion = WME;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if ((dwMajorVersion == 3) && (dwMinorVersion == 51))
		{
			_tcscpy(pszVersion, WNT351STR);
			*nVersion = WNT351;
		}
		else if ((dwMajorVersion == 4) && (dwMinorVersion == 0))
		{
			_tcscpy(pszVersion, WNT4STR);
			*nVersion = WNT4;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 0))
		{
			_tcscpy(pszVersion, W2KSTR);
			*nVersion = W2K;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
		{
			_tcscpy(pszVersion, WXPSTR);
			*nVersion = WXP;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 2))
		{
			_tcscpy(pszVersion, W2003SERVERSTR);
			*nVersion = W2003SERVER;
		}
		else if ((dwMajorVersion == 6) && (dwMinorVersion == 0))
		{
			if (dwBuildNumber == 6000)
			{
				_tcscpy(pszVersion, WVISTASTR);
				*nVersion = WVISTA;
			}
			else if (dwBuildNumber == 6001)
			{
				if (osinfo.wProductType == VER_NT_SERVER)
				{
					_tcscpy(pszVersion, W2008SERVERSTR);
					*nVersion = W2008SERVER;
				}
				else if (osinfo.wProductType == VER_NT_WORKSTATION)
				{
					_tcscpy(pszVersion, WVISTASP1STR);
					*nVersion = WVISTASP1;
				}
			}
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_CE)
	{
		_tcscpy(pszVersion, WCESTR);
		*nVersion = WCE;
	}
	return TRUE;
}
