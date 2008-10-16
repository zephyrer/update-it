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

// AboutPage.h - interface of the CAboutPage class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__AboutPage_h)
#define __AboutPage_h

class CCustomResourceBase
{

// construction/destruction

protected:

	CCustomResourceBase(HINSTANCE hInstRes, LPCTSTR pszType, LPCTSTR pszName):
	m_hInstRes(hInstRes),
	m_strType(pszType),
	m_pszName(IS_INTRESOURCE(pszName) ? pszName : _tcsdup(pszName)),
	m_hrsrcInfo(NULL),
	m_hMemBlock(NULL),
	m_pvDataPtr(NULL)
	{
		ASSERT(m_pszName != NULL);
		
		m_hrsrcInfo = ::FindResource(m_hInstRes, m_pszName, m_strType);
		m_hMemBlock = ::LoadResource(m_hInstRes, m_hrsrcInfo);
		m_pvDataPtr = ::LockResource(m_hMemBlock);
	}

	virtual ~CCustomResourceBase(void)
	{
		if (m_hMemBlock != NULL)
		{
			UnlockResource(m_hMemBlock);
			::FreeResource(m_hMemBlock);
		}

		if (!IS_INTRESOURCE(m_pszName))
		{
			free(const_cast<LPTSTR>(m_pszName));
		}
	}

// operations

public:

	void* GetData(void) const
	{
		return (m_pvDataPtr);
	}

	DWORD GetSize(void) const
	{
		return (::SizeofResource(m_hInstRes, m_hrsrcInfo));
	}

// attributes

protected:

	HINSTANCE m_hInstRes;
	CString m_strType;
	LPCTSTR m_pszName;
	HRSRC m_hrsrcInfo;
	HGLOBAL m_hMemBlock;
	void* m_pvDataPtr;

};

class CCustomResource: public CCustomResourceBase
{

// construction

public:

	CCustomResource(HINSTANCE hInstRes, LPCTSTR pszType, UINT uID):
	CCustomResourceBase(hInstRes, pszType, MAKEINTRESOURCE(uID))
	{
	}

	CCustomResource(HINSTANCE hInstRes, LPCTSTR pszType, LPCTSTR pszName):
	CCustomResourceBase(hInstRes, pszType, pszName)
	{
	}

};

//! Encapsulates the "Weclome!" step of the UpdateIt! wizard.
class CAboutPage: public CBetterPropPage
{
	DECLARE_DYNAMIC(CAboutPage)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CAboutPage(void);
	virtual ~CAboutPage(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
	virtual BOOL OnSetActive(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// message map functions
protected:
	afx_msg void OnPaint(void);

// attributes
public:
	CStatic m_textVersion;
	CHyperLink m_linkZarezky;
	CHyperLink m_linkZLib;
	CHyperLink m_linkArtpol;
	CHyperLink m_linkRSA;
	CHyperLink m_linkNaughter;
	CHyperLink m_linkOpenSSL;
	CCustomResource m_resSoftpediaAward;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif   // __AboutPage_h

// end of file
