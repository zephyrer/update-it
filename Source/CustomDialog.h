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

// CustomDialog.h - interface of the CCustomDialog class

#if !defined(__CustomDialog_h)
#define __CustomDialog_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

class CCustomDialog: public CDialog
{
	DECLARE_DYNAMIC(CCustomDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	explicit CCustomDialog(UINT uResID, CWnd* pParentWnd = NULL);
	explicit CCustomDialog(LPCTSTR pszResName, CWnd* pParentWnd = NULL);
	virtual ~CCustomDialog(void);

// overridables
public:
	virtual INT_PTR DoModal(void);
	virtual BOOL OnInitDialog(void);

// attributes
public:
	LPCTSTR m_lpszDlgInitName;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __CustomDialog_h

// end of file
