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

// MiniVersion.h  Version 1.1
//
// Author:       Hans Dietrich
//               hdietrich2@hotmail.com
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

#ifndef __MINIVERSION_H
#define __MINIVERSION_H

class CMiniVersion
{
// constructors
public:
	CMiniVersion(LPCTSTR lpszPath = NULL);
	BOOL Init();
	void Release();

// operations
public:

// attributes
public:
	// fixed info
	BOOL GetFileVersion(WORD *pwVersion);
	BOOL GetProductVersion(WORD* pwVersion);
	BOOL GetFileFlags(DWORD& rdwFlags);
	BOOL GetFileOS(DWORD& rdwOS);
	BOOL GetFileType(DWORD& rdwType);
	BOOL GetFileSubtype(DWORD& rdwType);	

	// string info
	BOOL GetCompanyName(LPTSTR lpszCompanyName, int nSize);
	BOOL GetFileDescription(LPTSTR lpszFileDescription, int nSize);
	BOOL GetProductName(LPTSTR lpszProductName, int nSize);

// implementation
protected:
	BOOL GetFixedInfo(VS_FIXEDFILEINFO& rFixedInfo);
	BOOL GetStringInfo(LPCTSTR lpszKey, LPTSTR lpszValue);

	BYTE*		m_pData;
	DWORD		m_dwHandle;
	WORD		m_wFileVersion[4];
	WORD		m_wProductVersion[4];
	DWORD		m_dwFileFlags;
	DWORD		m_dwFileOS;
	DWORD		m_dwFileType;
	DWORD		m_dwFileSubtype;

	TCHAR		m_szPath[MAX_PATH*2];
	TCHAR		m_szCompanyName[MAX_PATH*2];
	TCHAR		m_szProductName[MAX_PATH*2];
	TCHAR		m_szFileDescription[MAX_PATH*2];
};

#endif   // __MINIVERSION_H
