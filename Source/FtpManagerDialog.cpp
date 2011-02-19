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
	ON_BN_CLICKED(IDC_BUTTON_FTP_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_FTP_REMOVE, OnButtonRemove)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CFtpManagerDialog::CFtpManagerDialog(CWnd* pParentWnd):
CCustomDialog(IDD_FTP_MANAGER, pParentWnd),
m_iFtpRootImg(-1)
{
	RegQueryData();
}

CFtpManagerDialog::~CFtpManagerDialog(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CFtpManagerDialog::OnInitDialog(void)
{
	CRect rectList;
	CString strHeading;

	__super::OnInitDialog();

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	int cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	int cySmIcon = ::GetSystemMetrics(SM_CYSMICON);

	HDC hdcScreen = ::GetDC(NULL);
	int nBitsPixel = ::GetDeviceCaps(hdcScreen, BITSPIXEL);
	::ReleaseDC(NULL, hdcScreen);

	m_imageList.Create(cxSmIcon, cySmIcon, nBitsPixel | ILC_MASK, 8, 8);

	HICON hItemIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(m_idrFtpRoot));
	m_iFtpRootImg = m_imageList.Add(hItemIcon);

	m_listSites.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listSites.SetImageList(&m_imageList, LVSIL_SMALL);

	int cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);
	m_listSites.GetClientRect(rectList);
	int cxWidth = ((rectList.Width() - cxVScroll) / NUM_COLUMNS);

	strHeading.LoadString(IDS_FTP_NAME);
	m_listSites.InsertColumn(I_NAME, strHeading, LVCFMT_LEFT, cxWidth, I_NAME);
	strHeading.LoadString(IDS_FTP_COMMENT);
	m_listSites.InsertColumn(I_COMMENT, strHeading, LVCFMT_LEFT, cxWidth, I_COMMENT);

	if (PutDataToList() > 0)
	{
		m_listSites.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, 0xFFFFFFFF);
	}
	UpdateControls();

	return (TRUE);
}

void CFtpManagerDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_FTP_SITES, m_listSites);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CFtpManagerDialog::OnButtonEdit(void)
{
	POSITION pos = m_listSites.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int iItem = m_listSites.GetNextSelectedItem(pos);
		ASSERT(iItem >= 0);
		SITE_DATA& siteData = m_arrData[iItem];

		CFtpPropertiesDialog dlgFtpProperties(IDS_EDIT_FTP_SITE);
		dlgFtpProperties.m_strName = siteData.szName;
		dlgFtpProperties.m_strComment = siteData.szComment;
		dlgFtpProperties.m_strServer = siteData.szServer;
		dlgFtpProperties.m_nPort = siteData.nPort;
		dlgFtpProperties.m_strLogin = siteData.szLogin;
		dlgFtpProperties.m_strPassword = siteData.szPassword;
		dlgFtpProperties.m_strRoot = siteData.szRoot;
		dlgFtpProperties.m_fPassive = siteData.fPassive;

		if (dlgFtpProperties.DoModal() == IDOK)
		{
		}
	}
}

void CFtpManagerDialog::OnButtonRemove(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

INT_PTR CFtpManagerDialog::RegQueryData(void)
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
						// comment
						uMaxChars = _countof(siteData.szComment);
						regKeySite.QueryStringValue(SZ_REGV_FTP_COMMENT, siteData.szComment, &uMaxChars);
						// server
						uMaxChars = _countof(siteData.szServer);
						regKeySite.QueryStringValue(SZ_REGV_FTP_SERVER, siteData.szServer, &uMaxChars);
						// port
						DWORD dwTemp = 0;
						regKeySite.QueryDWORDValue(SZ_REGV_FTP_PORT, dwTemp);
						siteData.nPort = LOWORD(dwTemp);
						// login
						uMaxChars = _countof(siteData.szLogin);
						regKeySite.QueryStringValue(SZ_REGV_FTP_LOGIN, siteData.szLogin, &uMaxChars);
						// password
						CString strTemp = pApp->GetProfilePassword(_T(""), SZ_REGV_FTP_PASSWORD, NULL, regKeySite);
						_tcsncpy(siteData.szPassword, strTemp, _countof(siteData.szPassword) - 1);
						// root
						uMaxChars = _countof(siteData.szRoot);
						regKeySite.QueryStringValue(SZ_REGV_FTP_ROOT, siteData.szRoot, &uMaxChars);
						// passive
						regKeySite.QueryDWORDValue(SZ_REGV_FTP_PASSIVE, *reinterpret_cast<DWORD*>(&siteData.fPassive));

						m_arrData.Add(siteData);
					}
				}
			}
		}
	}

	return (m_arrData.GetCount());
}

int CFtpManagerDialog::PutDataToList(void)
{
	// precondition
	ASSERT(::IsWindow(m_listSites));

	m_listSites.DeleteAllItems();

	for (int i = 0, cCount = m_arrData.GetCount(); i < cCount; ++i)
	{
		int iItem = m_listSites.InsertItem(i, m_arrData[i].szName, m_iFtpRootImg);
		if (iItem >= 0)
		{
			ASSERT(iItem == i);
			m_listSites.SetItemText(iItem, I_COMMENT, m_arrData[i].szComment);
		}
	}

	int cNumItems = m_listSites.GetItemCount();

	m_listSites.SetColumnWidth(I_NAME, cNumItems > 0 ? LVSCW_AUTOSIZE : LVSCW_AUTOSIZE_USEHEADER);

	CRect rectList;
	m_listSites.GetClientRect(rectList);
	int cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);
	int cxName = m_listSites.GetColumnWidth(I_NAME);
	int cxComment = rectList.Width() - cxVScroll - cxName;
	m_listSites.SetColumnWidth(I_COMMENT, cxComment);

	return (cNumItems);
}

void CFtpManagerDialog::UpdateControls(void)
{
	int cNumItems = m_listSites.GetItemCount();
	POSITION pos = m_listSites.GetFirstSelectedItemPosition();

	GetDlgItem(IDC_LIST_FTP_SITES)->EnableWindow(cNumItems > 0);
	GetDlgItem(IDC_BUTTON_FTP_EDIT)->EnableWindow(cNumItems > 0 && pos != NULL);
	GetDlgItem(IDC_BUTTON_FTP_REMOVE)->EnableWindow(cNumItems > 0 && pos != NULL);
	GetDlgItem(IDOK)->EnableWindow(cNumItems > 0 && pos != NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CFtpManagerDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
	ASSERT_VALID(&m_imageList);
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
		dumpCtx << "\nm_imageList = " << m_imageList;
		dumpCtx << "\nm_iFtpRootImg = " << m_iFtpRootImg;
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
