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

// BrowseFtpDialog.h - interface of the CBrowseFtpDialog class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__BrowseFtpDialog_h)
#define __BrowseFtpDialog_h

class CBrowseFtpDialog: public CCustomDialog
{
	DECLARE_DYNAMIC(CBrowseFtpDialog)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CBrowseFtpDialog(CWnd* pParentWnd = NULL);
	virtual ~CBrowseFtpDialog(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// message map functions
protected:

// attributes
public:

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif   // __BrowseFtpDialog_h

// end of file
