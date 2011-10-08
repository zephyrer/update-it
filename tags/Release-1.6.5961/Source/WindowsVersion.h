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

// WindowsVersion.h - interface and implementation of the CWindowsVersion class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__WindowsVersion_h)
#define __WindowsVersion_h

//////////////////////////////////////////////////////////////////////////////////////////////
// standard includes

#include <ntverp.h>

//-----------------------------------------------------------------------
// Windows SDK version     |  2003  |  v5.0  |  v6.0A |  v7.0  |  v7.1  |
//-------------------------+--------+--------+--------+--------+--------+
// VER_PRODUCTBUILD        |   3668 |   3790 |   6000 |   7600 |   7600 |
// VER_PRODUCTBUILD_QFE    |      0 |   2075 |  16384 |  16385 |  16385 |
// VER_PRODUCTMAJORVERSION |      5 |      5 |      6 |      6 |      6 |
// VER_PRODUCTMINORVERSION |      2 |      2 |      0 |      1 |      1 |
// VER_PRODUCTVERSION_W    | 0x0502 | 0x0502 | 0x0600 | 0x0601 | 0x0601 |
//-----------------------------------------------------------------------

#if !defined(VER_PRODUCTBUILD) || (VER_PRODUCTBUILD < 7600)
#error Windows SDK version 7.0 or greater required to compile this code
#endif   // VER_PRODUCTBUILD

#include <sdkddkver.h>

// Why are the generic version numbers called NTDDI?
//
// In my earlier discussion on the variety of symbols that describe the target Windows version, I pointed 
// out that the NTDDI symbols attempt to cut through the mess and consolidate everything into a single symbol. 
// But why the name NTDDI?
// One of my colleagues contacted me privately with the story. When setting out to change the operating system
// version number, my colleague was shocked to find so many different version number mechanisms were scattered
// throughout the various Windows header files. It so happened that the DDK people were already in the process
// of cleaning up the version number mess and were using NTDDI as their version number system. Seeing no reason
// to invent a new different system for user mode, my colleague proposed using the DDK system in the SDK and 
// asked if anybody had any better ideas.
// Nobody came up with any better ideas, no compelling reason why we should have two different version number
// systems, so the NTDDI name stuck. And it stands for NT Device Driver Interface.
//
// http://blogs.msdn.com/b/oldnewthing/archive/2008/12/05/9177689.aspx

//////////////////////////////////////////////////////////////////////////////////////////////
// CWindowsVersion

class CWindowsVersion
{
// construction
public:
	__declspec(noinline) CWindowsVersion(void):
	m_wWinNT(0),
		m_dwNTDDI(0)
	{
		OSVERSIONINFO osVerInfo = { 0 };
		osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
		::GetVersionEx(&osVerInfo);
		m_wWinNT = MAKEWORD(LOBYTE(LOWORD(osVerInfo.dwMinorVersion)), LOBYTE(LOWORD(osVerInfo.dwMajorVersion)));
		m_dwNTDDI = MAKELONG(0, m_wWinNT);

		ATL::CRegKey regKeyWindows;
		LONG nError = regKeyWindows.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Windows"));
		if (nError == ERROR_SUCCESS)
		{
			DWORD dwCSDVersion = 0;
			nError = regKeyWindows.QueryDWORDValue(_T("CSDVersion"), dwCSDVersion);
			if (nError == ERROR_SUCCESS)
			{
				m_dwNTDDI |= LOWORD(dwCSDVersion);
			}
			regKeyWindows.Close();
		}
	}

// operations
public:
	WORD WinNT(void) const
	{
		// compare it with _WIN32_WINNT_* version constants
		return (m_wWinNT);
	}

	DWORD NTDDI(void) const
	{
		// compare it with NTDDI_* version constants
		return (m_dwNTDDI);
	}

	__declspec(noinline) DWORD TrueWinNT(void) const
	{
		OSVERSIONINFOEX osVerInfo = { 0 };
		osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
		::GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osVerInfo));

		DWORD fdwProductType = PRODUCT_UNDEFINED;
		BOOL (WINAPI* pfnGetProductInfo)(DWORD, DWORD, DWORD, DWORD, DWORD*) = NULL;
		HMODULE hKernel32 = ::GetModuleHandle(_T("kernel32.dll"));
		(FARPROC&)pfnGetProductInfo = ::GetProcAddress(hKernel32, "GetProductInfo");
		if (pfnGetProductInfo != NULL)
		{
			pfnGetProductInfo(osVerInfo.dwMajorVersion, osVerInfo.dwMinorVersion, 0, 0, &fdwProductType);
		}

		DWORD fdwTrueVer = MAKELONG(0, m_wWinNT);

		if (osVerInfo.wProductType != VER_NT_WORKSTATION)
		{
			fdwTrueVer |= EDITION_SERVER;
		}
		else if (fdwProductType == PRODUCT_STARTER)
		{
			fdwTrueVer |= EDITION_STARTER;
		}
		else if ((osVerInfo.wSuiteMask & VER_SUITE_PERSONAL) != 0)
		{
			fdwTrueVer |= EDITION_HOME;
		}
		else
		{
			fdwTrueVer |= EDITION_WORKSTATION;
		}

		// just in case of Windows Server 2003 R2
		if (m_wWinNT == _WIN32_WINNT_WS03 && ::GetSystemMetrics(SM_SERVERR2) > 0)
		{
			fdwTrueVer |= 0x0100;
		}

		fdwTrueVer |= (LOBYTE(osVerInfo.wServicePackMajor));

		return (fdwTrueVer);
	}

// attributes
public:
	enum
	{
		MASK_VERMAJOR = 0xFF000000,
		MASK_VERMINOR = 0x00FF0000,
		MASK_WIN32_WINNT = MASK_VERMAJOR | MASK_VERMINOR,
		MASK_EDITION = 0x0000FF00,
		MASK_OSVER = MASK_WIN32_WINNT | MASK_EDITION,
		MASK_SPVER = 0x000000FF,

		EDITION_STARTER = 0x1000,
		EDITION_HOME = 0x2000,
		EDITION_WORKSTATION = 0x4000,
		EDITION_SERVER = 0x8000,

		// compare TrueWinNT() result with these constants
		NT4_WORKSTATION = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_NT4),
		NT4_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_NT4),
		WIN2K_PROFESSIONAL = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_WIN2K),
		WIN2K_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_WIN2K),
		WINXP_HOME = MAKELONG(EDITION_HOME, _WIN32_WINNT_WINXP),
		WINXP_PROFESSIONAL = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_WINXP),
		WS03_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_WS03),
		WS03R2_SERVER = MAKELONG(EDITION_SERVER | 0x0100, _WIN32_WINNT_WS03),
		VISTA_STARTER = MAKELONG(EDITION_STARTER, _WIN32_WINNT_VISTA),
		VISTA_HOME = MAKELONG(EDITION_HOME, _WIN32_WINNT_VISTA),
		VISTA_BUSINESS = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_VISTA),
		WS08_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_VISTA),
		WIN7_STARTER = MAKELONG(EDITION_STARTER, _WIN32_WINNT_WIN7),
		WIN7_HOME = MAKELONG(EDITION_HOME, _WIN32_WINNT_WIN7),
		WIN7_PROFESSIONAL = MAKELONG(EDITION_WORKSTATION, _WIN32_WINNT_WIN7),
		WS08R2_SERVER = MAKELONG(EDITION_SERVER, _WIN32_WINNT_WIN7)
	};

private:
	WORD m_wWinNT;
	DWORD m_dwNTDDI;
};

#endif   // __WindowsVersion_h

// end of file
