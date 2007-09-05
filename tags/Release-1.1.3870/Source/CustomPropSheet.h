// UpdateIt! application.
// Copyright (c) 2002-2007 by Elijah Zarezky,
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

// CustomPropSheet.h - interface of the CCustomPropSheet class

#if !defined(__CustomPropSheet_h)
#define __CustomPropSheet_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

#define PSM_RESIZE_PAGE (WM_APP + 2)

class CCustomPropSheet: public CPropertySheet
{
	DECLARE_DYNAMIC(CCustomPropSheet)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CCustomPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCustomPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CCustomPropSheet(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pnResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

// message map functions
protected:
	afx_msg LRESULT OnResizePage(WPARAM wParam, LPARAM lParam);

// attributes
protected:
	static bool m_fIsFirstChangeFont;
	RECT m_rcPage;
	CFont m_fontPage;

// implementation helpers
protected:
	enum { CDF_CENTER, CDF_TOPLEFT, CDF_NONE };
	void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag);
	virtual void BuildPropPageArray(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __CustomPropSheet_h

// end of file
