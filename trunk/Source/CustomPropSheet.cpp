// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.
// Portions copyright (c) 1996-1998 by Microsoft (KB Q142170).

// CustomPropSheet.cpp - implementation of the CCustomPropSheet class

#include "stdafx.h"
#include "CustomPropSheet.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

#define _BUGFIX_

// object model
IMPLEMENT_DYNAMIC(CCustomPropSheet, CPropertySheet)

// message map
BEGIN_MESSAGE_MAP(CCustomPropSheet, CPropertySheet)
	ON_MESSAGE(PSM_RESIZE_PAGE, OnResizePage)
END_MESSAGE_MAP()

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
	if (m_fontPage.m_hObject != NULL) {
		VERIFY(m_fontPage.DeleteObject());
	}
}

BOOL CCustomPropSheet::OnInitDialog(void)
{
	CPropertySheet::OnInitDialog();

	// get the font for the first active page
	CPropertyPage* pCurPage = GetActivePage();
	ASSERT(pCurPage != NULL);

	// change the font for the sheet
	ChangeDialogFont(this, &m_fontPage, CDF_CENTER);

	// change the font for each page
	for (int i = 0, cPages = GetPageCount(); i < cPages; ++i) {
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

	if ((m_psh.dwFlags & PSH_WIZARD) != 0) {
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
	if (reinterpret_cast<NMHDR*>(lParam)->code == TCN_SELCHANGE) {
		PostMessage(PSM_RESIZE_PAGE);
	}
	return (CPropertySheet::OnNotify(wParam, lParam, pnResult));
}

BOOL CCustomPropSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// the sheet resizes the page whenever the Apply button is clicked
	if (wParam == ID_APPLY_NOW || wParam == ID_WIZBACK || wParam == ID_WIZNEXT) {
		PostMessage(PSM_RESIZE_PAGE);
	}
	return (CPropertySheet::OnCommand(wParam, lParam));
}

LRESULT CCustomPropSheet::OnResizePage(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CPropertyPage* pCurPage = GetActivePage();
	ASSERT(pCurPage != NULL);
	pCurPage->MoveWindow(&m_rcPage);
	return (0);
}

void CCustomPropSheet::ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag)
{
	TEXTMETRIC tmOld, tmNew;
	CRect rectWindow;
	CString strClassName;

	// grab old and new text metrics
	CDC* pDC = pWnd->GetDC();
	CFont* pPrevFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pPrevFont);
	pWnd->ReleaseDC(pDC);

	long nOldHeight = tmOld.tmHeight + tmOld.tmExternalLeading;
	long nNewHeight = tmNew.tmHeight + tmNew.tmExternalLeading;

	if (nFlag != CDF_NONE) {
		// calculate new dialog window rectangle
		CRect rectClient, rectNewClient, rectNewWindow;

		pWnd->GetWindowRect(rectWindow);
		pWnd->GetClientRect(rectClient);
		long dxWidth = rectWindow.Width() - rectClient.Width();
		long dyHeight = rectWindow.Height() - rectClient.Height();

		rectNewClient.left = rectNewClient.top = 0;
		rectNewClient.right = rectClient.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		rectNewClient.bottom = rectClient.bottom * nNewHeight / nOldHeight;

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
		if (pWnd->IsKindOf(RUNTIME_CLASS(CPropertyPage))) {
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
			static BOOL fFirst = TRUE;
			if (fFirst) {
				::CopyRect(&m_rcPage, rectNewWindow);
				fFirst = FALSE;
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
	while (pChildWnd != NULL) {
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(rectWindow);

		::GetClassName(*pChildWnd, strClassName.GetBufferSetLength(32), 31);
		strClassName.MakeUpper();
		if (strClassName == _T("COMBOBOX")) {
			CRect rectDropped;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)&rectDropped);
			rectWindow.right = rectDropped.right;
			rectWindow.bottom = rectDropped.bottom;
		}

		pWnd->ScreenToClient(rectWindow);
		rectWindow.left = rectWindow.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		rectWindow.top = rectWindow.top * nNewHeight / nOldHeight;
		rectWindow.right = rectWindow.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		rectWindow.bottom = rectWindow.bottom * nNewHeight / nOldHeight;
		pChildWnd->MoveWindow(rectWindow);

		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CCustomPropSheet::BuildPropPageArray(void)
{
	CDialogTemplate dlgTemp;
	LOGFONT lf;

	CPropertySheet::BuildPropPageArray();

	// get first page
	CPropertyPage* pPage = GetPage(0);
	ASSERT(pPage != NULL);

	// load the dialog template
	VERIFY(dlgTemp.Load(pPage->m_psp.pszTemplate));

	// get the font information
	CWinApp* pApp = AfxGetApp();
	CString strFaceName = pApp->GetProfileString(_T("Font"), _T("FaceName"));
	WORD wFontSize = LOWORD(pApp->GetProfileInt(_T("Font"), _T("Size"), 0));
	if (strFaceName.IsEmpty() || wFontSize == 0) {
		VERIFY(dlgTemp.GetFont(strFaceName, wFontSize));
	}
	if (m_fontPage.m_hObject != NULL) {
		VERIFY(m_fontPage.DeleteObject());
	}

	// create a font using the info from first page
	HDC hdcScreen = ::GetDC(NULL);
	ASSERT(hdcScreen != NULL);
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = ::MulDiv(-wFontSize, ::GetDeviceCaps(hdcScreen, LOGPIXELSY), 72);
	lf.lfWeight = FW_REGULAR;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	::lstrcpy(lf.lfFaceName, strFaceName);
	VERIFY(m_fontPage.CreateFontIndirect(&lf));
	::ReleaseDC(NULL, hdcScreen);
}

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CCustomPropSheet::AssertValid(void) const
{
	// first perform inherited validity check...
	CPropertySheet::AssertValid();
	// ...and then verify own state as well
	ASSERT_VALID(&m_fontPage);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CCustomPropSheet::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CPropertySheet::Dump(dumpCtx);
		// ...and then dump own unique members
		dumpCtx << "m_rcPage = " << m_rcPage;
		dumpCtx << "\nm_fontPage = " << m_fontPage;
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
