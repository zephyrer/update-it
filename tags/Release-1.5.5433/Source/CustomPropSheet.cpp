// UpdateIt! application.
// Copyright (c) 2002-2010 by Elijah Zarezky,
// All rights reserved.
// Portions copyright (c) 1996-1998 by Microsoft (KB Q142170).

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

// CustomPropSheet.cpp - implementation of the CCustomPropSheet class

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "CustomPropSheet.h"
#include "../Common/Registry.h"
#include "UpdateItApp.h"
#include "Arguments.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// avoid unwanted ICL warnings

#if defined(__INTEL_COMPILER)
// remark #171: invalid type conversion
#pragma warning(disable: 171)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
// remark #383: value copied to temporary, reference to temporary used
#pragma warning(disable: 383)
// remark #981: operands are evaluated in unspecified order
#pragma warning(disable: 981)
#endif	// __INTEL_COMPILER

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

#define _BUGFIX_

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CCustomPropSheet, CPropertySheet)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CCustomPropSheet, CPropertySheet)
	ON_MESSAGE(PSM_RESIZE_PAGE, OnResizePage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CCustomPropSheet::CCustomPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage):
CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	::SetRectEmpty(&m_rcPage);
}

CCustomPropSheet::CCustomPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage):
CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	::SetRectEmpty(&m_rcPage);
}

CCustomPropSheet::~CCustomPropSheet(void)
{
	if (m_fontPage.m_hObject != NULL)
	{
		VERIFY(m_fontPage.DeleteObject());
	}
	m_fIsFirstChangeFont = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CCustomPropSheet::OnInitDialog(void)
{
	__super::OnInitDialog();

	// get the font for the first active page
	CPropertyPage* pCurPage = GetActivePage();
	ASSERT(pCurPage != NULL);

	// change the font for the sheet
	ChangeDialogFont(this, &m_fontPage, CDF_CENTER);

	// change the font for each page
	for (int i = 0, cPages = GetPageCount(); i < cPages; ++i)
	{
		CPropertyPage* pPage = GetPage(i);
		ASSERT(pPage != NULL);
#if !defined(_BUGFIX_)
		ChangeDialogFont(pPage, &m_fontPage, CDF_CENTER);
#else
		ChangeDialogFont(pPage, &m_fontPage, CDF_TOPLEFT);
#endif	// _BUGFIX_
	}

	// set and save the size of the page
	CTabCtrl* pTab = GetTabControl();
	ASSERT(pTab != NULL);

	if ((m_psh.dwFlags & PSH_WIZARD) != 0)
	{
		pTab->ShowWindow(SW_HIDE);
#if !defined(_BUGFIX_)
		GetClientRect(&m_rcPage);
		CWnd* pButton = GetDlgItem(ID_WIZBACK);
		ASSERT(pButton != NULL);
		CRect rectButton;
		pButton->GetWindowRect(rectButton);
		ScreenToClient(rectButton);
		m_rcPage.bottom = rectButton.top - 2;
#else
		CRect rectDivider;
		CWnd* pDivider = GetDlgItem(0x3026);
		ASSERT(pDivider != NULL);
		pDivider->GetWindowRect(rectDivider);
		ScreenToClient(rectDivider);
		m_rcPage.bottom = rectDivider.top - 2;
#endif	// _BUGFIX_
	}
	else {
		pTab->GetWindowRect(&m_rcPage);
		ScreenToClient(&m_rcPage);
		pTab->AdjustRect(FALSE, &m_rcPage);
	}

	// resize the page
	pCurPage->MoveWindow(&m_rcPage);

	return (TRUE);
}

BOOL CCustomPropSheet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pnResult)
{
	// the sheet resizes the page whenever it is activated
	if (reinterpret_cast<NMHDR*>(lParam)->code == TCN_SELCHANGE)
	{
		PostMessage(PSM_RESIZE_PAGE);
	}
	return (__super::OnNotify(wParam, lParam, pnResult));
}

BOOL CCustomPropSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// the sheet resizes the page whenever the Apply button is clicked
	if (wParam == ID_APPLY_NOW || wParam == ID_WIZBACK || wParam == ID_WIZNEXT)
	{
		PostMessage(PSM_RESIZE_PAGE);
	}
	return (__super::OnCommand(wParam, lParam));
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

LRESULT CCustomPropSheet::OnResizePage(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CPropertyPage* pCurPage = GetActivePage();
	ASSERT(pCurPage != NULL);
	pCurPage->MoveWindow(&m_rcPage);
	return (0);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// attributes

bool CCustomPropSheet::m_fIsFirstChangeFont = true;

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CCustomPropSheet::ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag)
{
	TEXTMETRIC tmCurrent = { 0 }, tmNew = { 0 };
	CRect rectWindow;
	CString strClassName;

	// grab current and new text metrics
	CDC* pDC = pWnd->GetDC();
	CFont* pPrevFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmCurrent);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pPrevFont);
	pWnd->ReleaseDC(pDC);

#if defined(_DEBUG)
	LOGFONT lfCurrent = { 0 }, lfNew = { 0 };
	pWnd->GetFont()->GetLogFont(&lfCurrent);
	TRACE(_T("Current font: %s, %i\n"), lfCurrent.lfFaceName, lfCurrent.lfHeight);
	pFont->GetLogFont(&lfNew);
	TRACE(_T("New font: %s, %i\n"), lfNew.lfFaceName, lfNew.lfHeight);
#endif   // _DEBUG

	long nCurHeight = tmCurrent.tmHeight + tmCurrent.tmExternalLeading;
	long nNewHeight = tmNew.tmHeight + tmNew.tmExternalLeading;

	if (nFlag != CDF_NONE)
	{
		// calculate new dialog window rectangle
		CRect rectClient, rectNewClient, rectNewWindow;

		pWnd->GetWindowRect(rectWindow);
		pWnd->GetClientRect(rectClient);
		long dxWidth = rectWindow.Width() - rectClient.Width();
		long dyHeight = rectWindow.Height() - rectClient.Height();

		rectNewClient.left = rectNewClient.top = 0;
		rectNewClient.right = rectClient.right * tmNew.tmAveCharWidth / tmCurrent.tmAveCharWidth;
		rectNewClient.bottom = rectClient.bottom * nNewHeight / nCurHeight;

		switch (nFlag)
		{
		case CDF_TOPLEFT:
			// resize with origin at top/left of window
			rectNewWindow.left = rectWindow.left;
			rectNewWindow.top = rectWindow.top;
			rectNewWindow.right = rectWindow.left + rectNewClient.right + dxWidth;
			rectNewWindow.bottom = rectWindow.top + rectNewClient.bottom + dyHeight;
			break;
		case CDF_CENTER:
			// resize with origin at center of window
			rectNewWindow.left = rectWindow.left - (rectNewClient.right - rectClient.right) / 2;
			rectNewWindow.top = rectWindow.top - (rectNewClient.bottom - rectClient.bottom) / 2;
			rectNewWindow.right = rectNewWindow.left + rectNewClient.right + dxWidth;
			rectNewWindow.bottom = rectNewWindow.top + rectNewClient.bottom + dyHeight;
			break;
		}
#if !defined(_BUGFIX_)
		pWnd->MoveWindow(rectNewWindow);
#else
		if (pWnd->IsKindOf(RUNTIME_CLASS(CPropertyPage)))
		{
			CRect rectParent;
			CPropertySheet* pSheet = DYNAMIC_DOWNCAST(CPropertySheet, pWnd->GetParent());
			ASSERT(pSheet != NULL);
			pSheet->GetClientRect(rectParent);
			pSheet->ScreenToClient(rectNewWindow);
			rectNewWindow.left = min(rectParent.right, max(0L, rectNewWindow.left));
			rectNewWindow.top = min(rectParent.bottom, max(0L, rectNewWindow.top));
			rectNewWindow.right = min(rectNewWindow.right, rectParent.right - rectNewWindow.left);
			rectNewWindow.bottom = min(rectNewWindow.bottom, rectParent.bottom - rectNewWindow.top);
			pWnd->MoveWindow(rectNewWindow);
			if (m_fIsFirstChangeFont)
			{
				::CopyRect(&m_rcPage, rectNewWindow);
				m_fIsFirstChangeFont = false;
			}
		}
		else {
			pWnd->MoveWindow(rectNewWindow);
		}
#endif	// _BUGFIX_
	}

	pWnd->SetFont(pFont);

	// iterate through and move all child windows and change their font
	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);
	while (pChildWnd != NULL)
	{
		if (!pChildWnd->IsKindOf(RUNTIME_CLASS(CPropertyPage)))
		{
			bool fNoResize = false;

			pChildWnd->SetFont(pFont);
			pChildWnd->GetWindowRect(rectWindow);

			// window class specific cases
			::GetClassName(*pChildWnd, strClassName.GetBuffer(_MAX_PATH), _MAX_PATH);
			strClassName.ReleaseBuffer();
			strClassName.MakeUpper();
			if (strClassName == _T("COMBOBOX"))
			{
				CRect rectDropped;
				pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)&rectDropped);
				rectWindow.right = rectDropped.right;
				rectWindow.bottom = rectDropped.bottom;
			}
			else if (strClassName == _T("STATIC"))
			{
				WORD wStaticType = LOWORD(pChildWnd->GetStyle() & SS_TYPEMASK);
				fNoResize = wStaticType == SS_ICON || wStaticType == SS_BITMAP;
			}

			pWnd->ScreenToClient(rectWindow);
			SIZE sizWindow = { rectWindow.Width(), rectWindow.Height() };
			rectWindow.left = rectWindow.left * tmNew.tmAveCharWidth / tmCurrent.tmAveCharWidth;
			rectWindow.top = rectWindow.top * nNewHeight / nCurHeight;
			if (fNoResize)
			{
				// just move
				rectWindow.right = rectWindow.left + sizWindow.cx;
				rectWindow.bottom = rectWindow.top + sizWindow.cy;
			}
			else
			{
				// resize
				rectWindow.right = rectWindow.right * tmNew.tmAveCharWidth / tmCurrent.tmAveCharWidth;
				rectWindow.bottom = rectWindow.bottom * nNewHeight / nCurHeight;
			}
			pChildWnd->MoveWindow(rectWindow);
		}

		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CCustomPropSheet::BuildPropPageArray(void)
{
	CDialogTemplate dlgtPageRes;

	__super::BuildPropPageArray();

	// get first page
	CPropertyPage* pPage = GetPage(0);
	ASSERT(pPage != NULL);

	// load the dialog template
	VERIFY(dlgtPageRes.Load(pPage->m_psp.pszTemplate));

	// get the font information

	// defaults
	CString strFaceName(_T("MS Shell Dlg 2"));
	WORD wPointSize = 8;

	if (!GetFontMetrics(strFaceName, wPointSize))
	{
		// obtain from resource template
		VERIFY(dlgtPageRes.GetFont(strFaceName, wPointSize));
	}

	if (m_fontPage.m_hObject != NULL)
	{
		VERIFY(m_fontPage.DeleteObject());
	}

	// create a font using the info from first page
	LOGFONT lf = { 0 };
	HDC hdcScreen = ::GetDC(NULL);
	ASSERT(hdcScreen != NULL);
	lf.lfHeight = ::MulDiv(-wPointSize, ::GetDeviceCaps(hdcScreen, LOGPIXELSY), 72);
	lf.lfWeight = FW_REGULAR;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcsncpy(lf.lfFaceName, strFaceName, LF_FACESIZE - 1);
	VERIFY(m_fontPage.CreateFontIndirect(&lf));
	::ReleaseDC(NULL, hdcScreen);
}

bool CCustomPropSheet::GetFontSubstitute(LPCTSTR pszRegvName, CString& strDest)
{
	ATL::CRegKey regKeyFontSubst;
	regKeyFontSubst.Create(HKEY_LOCAL_MACHINE, SZ_REGK_FONT_SUBSTITUTES);
	int nError = ERROR_SUCCESS;
	TCHAR szMsShellDlg[LF_FACESIZE] = { 0 };
	ULONG cchMaxLen = _countof(szMsShellDlg);
	nError = regKeyFontSubst.QueryStringValue(pszRegvName, szMsShellDlg, &cchMaxLen);
	if (nError == ERROR_SUCCESS)
	{
		strDest = szMsShellDlg;
		return (true);
	}
	else {
		return (false);
	}
}

bool CCustomPropSheet::GetFontMetrics(CString& strFaceName, WORD& wPointSize)
{
	bool fHasFont = false;

	CUpdateItApp* pApp = DYNAMIC_DOWNCAST(CUpdateItApp, AfxGetApp());
	ASSERT_VALID(pApp);

	// examine command line
	if (pApp->m_argsParser.HasKey(SZ_ARG_FONT_FACENAME))
	{
		strFaceName = pApp->m_argsParser.GetStringValue(SZ_ARG_FONT_FACENAME);
		if (pApp->m_argsParser.HasKey(SZ_ARG_FONT_FACENAME))
		{
			UINT uTemp = 0;
			fHasFont = pApp->m_argsParser.GetUIntValue(SZ_ARG_FONT_POINTSIZE, uTemp, 10);
			wPointSize = LOWORD(uTemp);
		}
	}

	// examine our registry key
	if (!fHasFont)
	{
		strFaceName = pApp->GetProfileString(SZ_REGK_FONT, SZ_REGV_FONT_FACENAME);
		wPointSize = LOWORD(pApp->GetProfileInt(SZ_REGK_FONT, SZ_REGV_FONT_SIZE, 0));
		fHasFont = !strFaceName.IsEmpty() && wPointSize != 0;
	}

	// examine system registry keys
	if (!fHasFont)
	{
		fHasFont = GetFontSubstitute(SZ_REGV_MS_SHELL_DLG_2, strFaceName);
		if (!fHasFont)
		{
			fHasFont = GetFontSubstitute(SZ_REGV_MS_SHELL_DLG, strFaceName);
		}
		if (fHasFont)
		{
			// gotcha - use default point size
			wPointSize = 8;
		}
	}

	return (fHasFont);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CCustomPropSheet::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify own state as well
	ASSERT_VALID(&m_fontPage);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CCustomPropSheet::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_rcPage = " << m_rcPage;
		dumpCtx << "\nm_fontPage = " << m_fontPage;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
