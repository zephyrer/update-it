// UpdateIt! application.
// Copyright (c) 2002-2008 by Elijah Zarezky,
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

// ProgressPage.h - interface of the CProgressPage class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__ProgressPage_h)
#define __ProgressPage_h

#if defined(__INTEL_COMPILER)
// remark #444: destructor for base class is not virtual
#pragma warning(disable: 444)
#endif   // __INTEL_COMPILER

//! Encapsulates the "Processing files" step of the UpdateIt! wizard.
class CProgressPage: public CProgressPageBase, public CZipActionCallback
{
	DECLARE_DYNAMIC(CProgressPage)
	DECLARE_MESSAGE_MAP()

// construction
public:
	CProgressPage(void);

// overridables
protected:
	virtual void ZipTargetFolder(LPCTSTR pszTarget, const CListCtrl& listFiles, LPCTSTR pszZipPath);

// CZipActionCallback overridables
public:
	virtual bool Callback(ZIP_SIZE_TYPE uProgress);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#if defined(__INTEL_COMPILER)
#pragma warning(default: 444)
#endif   // __INTEL_COMPILER

#endif   // __ProgressPage_h

// end of file
