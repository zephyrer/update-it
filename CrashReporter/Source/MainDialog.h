// CrashReporter application.
// Copyright (c) 2009 by Elijah Zarezky,
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

// MainDialog.h - interface of the CMainDialog class

// initially generated by AfxScratch v1.1.4778 on 14.08.2009 at 18:13:40
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

#if !defined(__MainDialog_h)
#define __MainDialog_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

class CMainDialog: public CDialog
{
	DECLARE_DYNAMIC(CMainDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CMainDialog(CWnd* pParentWnd = NULL);
	virtual ~CMainDialog(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);

// attributes
public:
	HICON m_hIcon;
	HICON m_hSmIcon;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif   // __MainDialog_h

// end of file