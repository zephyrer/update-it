// UpdateIt! application.
// Copyright (c) 2002-2005 by Elijah Zarezky,
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

// BetterPropPage.cpp - implementation of the CBetterPropPage class

#include "stdafx.h"
#include "BetterPropPage.h"

#if defined(__INTEL_COMPILER)
// remark #171: invalid type conversion
#pragma warning(disable: 171)
// remark #279: controlling expression is constant
#pragma warning(disable: 279)
#endif	// __INTEL_COMPILER

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CBetterPropPage, CPropertyPage)

// message map
BEGIN_MESSAGE_MAP(CBetterPropPage, CPropertyPage)
	ON_NOTIFY_RANGE(TTN_GETDISPINFO, 0, 0xFFFF, OnGetTipText)
END_MESSAGE_MAP()

CBetterPropPage::CBetterPropPage(UINT idTemplate, UINT idCaption):
CPropertyPage(idTemplate, idCaption)
{
}

CBetterPropPage::CBetterPropPage(LPCTSTR pszTemplate, UINT idCaption):
CPropertyPage(pszTemplate, idCaption)
{
}

BOOL CBetterPropPage::OnInitDialog(void)
{
	BOOL fResult = CPropertyPage::OnInitDialog();
	m_tipWnd.Create(this);
	return (fResult);
}

BOOL CBetterPropPage::OnSetActive(void)
{
	BOOL fSuccess = CPropertyPage::OnSetActive();
	if (fSuccess)
	{
		AfxBeginThread(ActivationWatcher, m_hWnd);
	}
	return (fSuccess);
}

void CBetterPropPage::OnBecameActive(void)
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		::DispatchMessage(&msg);
	}
}

LRESULT CBetterPropPage::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case BPP_BECAME_ACTIVE:
		OnBecameActive();
		return (0);
	default:
		return (CPropertyPage::WindowProc(uMsg, wParam, lParam));
	}
}

BOOL CBetterPropPage::PreTranslateMessage(MSG* pMsg)
{
	if (WM_MOUSEFIRST <= pMsg->message && pMsg->message <= WM_MOUSELAST)
	{
		m_tipWnd.RelayEvent(pMsg);
	}
	return (CPropertyPage::PreTranslateMessage(pMsg));
}

#if (_MFC_VER < 0x0700)

void CBetterPropPage::OnGetTipText(UINT /*uID*/, NMTTDISPINFO* pInfo, LRESULT* pnResult)
{
	if ((pInfo->uFlags & TTF_IDISHWND) != 0)
	{
		CWnd* pWnd = CWnd::FromHandle(reinterpret_cast<HWND>(pInfo->hdr.idFrom));
		if (pWnd != NULL)
		{
			UINT idsTip = pWnd->GetDlgCtrlID();
			CString strTemp;
			if (strTemp.LoadString(idsTip))
			{
				::lstrcpyn(m_szTipText, strTemp, 256);
				pInfo->lpszText = m_szTipText;
			}
		}
	}
	*pnResult = 0;
}

#else

void CBetterPropPage::OnGetTipText(UINT /*uID*/, NMHDR* pHdr, LRESULT* pnResult)
{
	if ((reinterpret_cast<NMTTDISPINFO*>(pHdr)->uFlags & TTF_IDISHWND) != 0)
	{
		CWnd* pWnd = CWnd::FromHandle(reinterpret_cast<HWND>(reinterpret_cast<NMTTDISPINFO*>(pHdr)->hdr.idFrom));
		if (pWnd != NULL)
		{
			UINT idsTip = pWnd->GetDlgCtrlID();
			CString strTemp;
			if (strTemp.LoadString(idsTip))
			{
				::lstrcpyn(m_szTipText, strTemp, 256);
				reinterpret_cast<NMTTDISPINFO*>(pHdr)->lpszText = m_szTipText;
			}
		}
	}
	*pnResult = 0;
}

#endif	// _MFC_VER

CToolTipCtrl& CBetterPropPage::GetToolTipCtrl(void)
{
	return (m_tipWnd);
}

UINT AFX_CDECL CBetterPropPage::ActivationWatcher(void* pvParam)
{
	HWND hPageWnd = reinterpret_cast<HWND>(pvParam);
	ASSERT(::IsWindow(hPageWnd));
	while (!::IsWindowVisible(hPageWnd));
	::PostMessage(hPageWnd, BPP_BECAME_ACTIVE, 0, 0);
	AfxEndThread(0);
	return (0);
}

void CBetterPropPage::PumpWaitingMessages(void)
{
	MSG msg = { 0 };

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
#if (_MFC_VER < 0x0700)
		AfxGetThread()->PumpMessage();
#else
		AfxPumpMessage();
#endif	// _MFC_VER
	}
}

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CBetterPropPage::AssertValid(void) const
{
	// first perform inherited validity check...
	CPropertyPage::AssertValid();

	// ...and then verify own state as well
	ASSERT_VALID(&m_tipWnd);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CBetterPropPage::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		CPropertyPage::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_tipWnd = " << m_tipWnd;
		dumpCtx << "\nm_szTipText = " << m_szTipText;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
