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

// FtpManagerDialog.cpp - implementation of the CFtpManagerDialog class

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "Resource.h"
#include "../Languages/English_USA.1252/Source/Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "FtpManagerDialog.h"

#include "UpdateItApp.h"
#include "../Common/Registry.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CFtpManagerDialog, CCustomDialog)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CFtpManagerDialog, CCustomDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CFtpManagerDialog::CFtpManagerDialog(CWnd* pParentWnd):
CCustomDialog(IDD_FTP_MANAGER, pParentWnd)
{
}

CFtpManagerDialog::~CFtpManagerDialog(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CFtpManagerDialog::OnInitDialog(void)
{
	__super::OnInitDialog();

	return (TRUE);
}

void CFtpManagerDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_FTP_SITES, m_listSites);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

INT_PTR CFtpManagerDialog::RegQuerySites(void)
{
	m_arrData.RemoveAll();

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	ATL::CRegKey regKeyFtp;
	regKeyFtp.Attach(pApp->GetSectionKey(SZ_REGK_FTP));

	int nError = ERROR_SUCCESS;

	if (static_cast<HKEY>(regKeyFtp) != NULL)
	{
		ATL::CRegKey regKeySites;
		nError = regKeySites.Create(regKeyFtp, SZ_REGK_SITES);

		if (nError == ERROR_SUCCESS)
		{
			DWORD cNumSubKeys = 0;
			::RegQueryInfoKey(regKeySites, 0, 0, 0, &cNumSubKeys, 0, 0, 0, 0, 0, 0, 0);

			for (DWORD i = 0; i < cNumSubKeys; ++i)
			{
				SITE_DATA siteData = { 0 };
				ULONG uMaxChars = 0;

				DWORD cchNameLen = _countof(siteData.szName);
				nError = regKeySites.EnumKey(i, siteData.szName, &cchNameLen);

				if (nError == ERROR_SUCCESS)
				{
					ATL::CRegKey regKeySite;
					nError = regKeySite.Open(regKeySites, siteData.szName);

					if (nError == ERROR_SUCCESS)
					{
						uMaxChars = _countof(siteData.szComment);
						regKeySite.QueryStringValue(SZ_REGV_FTP_COMMENT, siteData.szComment, &uMaxChars);
						uMaxChars = _countof(siteData.szServer);
						regKeySite.QueryStringValue(SZ_REGV_FTP_SERVER, siteData.szServer, &uMaxChars);

						m_arrData.Add(siteData);
					}
				}
			}
		}
	}

	return (m_arrData.GetCount());
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CFtpManagerDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
	ASSERT_VALID(&m_listSites);
}

void CFtpManagerDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_arrData = " << m_arrData;
		dumpCtx << "\nm_listSites = " << m_listSites;
	}
	catch (CFileException* pErr)
	{
		pErr->ReportError();
		pErr->Delete();
	}
}

#endif   // _DEBUG

// end of file
