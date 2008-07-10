// UpdateIt! application.
// Copyright (c) 2002-2007 by Elijah Zarezky,
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

// CustomDialog.cpp - implementation of the CCustomDialog class

#include "stdafx.h"

#include "CustomDialog.h"
#include "Registry.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CCustomDialog, CDialog)

// message map
BEGIN_MESSAGE_MAP(CCustomDialog, CDialog)
END_MESSAGE_MAP()

// construction/destruction

CCustomDialog::CCustomDialog(UINT uResID, CWnd* pParentWnd):
CDialog(uResID, pParentWnd)
{
}

CCustomDialog::CCustomDialog(LPCTSTR pszResName, CWnd* pParentWnd):
CDialog(pszResName, pParentWnd)
{
}

CCustomDialog::~CCustomDialog(void)
{
}

// overridables

INT_PTR CCustomDialog::DoModal(void)
{
	// get the custom font information
	CWinApp* pApp = AfxGetApp();
	CString strFaceName = pApp->GetProfileString(SZ_REGK_FONT, SZ_REGV_FONT_FACENAME);
	WORD wFontSize = LOWORD(pApp->GetProfileInt(SZ_REGK_FONT, SZ_REGV_FONT_SIZE, 0));

	if (!strFaceName.IsEmpty() && wFontSize > 0)
	{
		// try to load dialog template
		CDialogTemplate dlgtRes;
		if (!dlgtRes.Load(m_lpszTemplateName))
		{
			return (-1);
		}

		// set specified custom font
		dlgtRes.SetFont(strFaceName, wFontSize);

		// get pointer to the modified dialog template
		DLGTEMPLATE* pTemplate = reinterpret_cast<DLGTEMPLATE*>(::GlobalLock(dlgtRes.m_hTemplate));

		// try to load coresponding DLGINIT resource (if any)
		HGLOBAL hResData = NULL;
		void* pvDlgInit = NULL;
		HINSTANCE hResInst = AfxGetResourceHandle();
		HRSRC hResInfo = ::FindResource(hResInst, m_lpszTemplateName, RT_DLGINIT);
		if (hResInfo != NULL)
		{
			hResData = ::LoadResource(hResInst, hResInfo);
			ASSERT(hResData != NULL);
			pvDlgInit = ::LockResource(hResData);
			ASSERT(pvDlgInit != NULL);
		}

		// let MFC know that we are using our own template
		LPCTSTR pszTemplateName = m_lpszTemplateName;
		m_lpszTemplateName = NULL;
		InitModalIndirect(pTemplate, NULL, pvDlgInit);

		// display dialog box
		INT_PTR nResult = __super::DoModal();

		if (hResData != NULL)
		{
			::UnlockResource(hResData);
			::FreeResource(hResData);
		}

		// unlock memory object
		::GlobalUnlock(dlgtRes.m_hTemplate);

		m_lpszTemplateName = pszTemplateName;
		m_lpDialogTemplate = NULL;

		return (nResult);
	}
	else {
		// no custom font is specified
		return (__super::DoModal());
	}
}

// diagnostic services

#if defined(_DEBUG)

void CCustomDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
}

void CCustomDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file