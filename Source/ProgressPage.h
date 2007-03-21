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

// ProgressPage.h - interface of the CProgressPage class

#if !defined(__ProgressPage_h)
#define __ProgressPage_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

#if defined(__INTEL_COMPILER)
// remark #444: destructor for base class is not virtual
#pragma warning(disable: 444)
#endif	// __INTEL_COMPILER

//! Encapsulates the "Processing files" step of the UpdateIt! wizard.
class CProgressPage: public CBetterPropPage, public CZipActionCallback
{
	DECLARE_DYNAMIC(CProgressPage)
	DECLARE_MESSAGE_MAP()

// construction
public:
	CProgressPage(void);

// overridables
public:
	virtual BOOL OnSetActive(void);
	virtual void OnBecameActive(void);
	virtual BOOL OnWizardFinish(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// CZipActionCallback overridables
public:
	virtual bool Callback(int iProgress);

// attributes
public:
	CStatic m_textWorking;
	CAnimateCtrl m_animateBanner;
	CStatic m_textFile;
	CProgressCtrl m_progressFile;
	CStatic m_textTotal;
	CProgressCtrl m_progressTotal;

// implementation helpers
private:
	void CreateSubFolder(LPCTSTR pszRoot, LPCTSTR pszFolder);
	void EraseFolder(LPCTSTR pszFolder, BOOL fCanUndo);
	BOOL CopyFile(LPCTSTR pszSrcPath, LPCTSTR pszTargPath, BOOL fDeleteSrc);
	int DeleteFile(LPCTSTR pszPath, BOOL fCanUndo = FALSE);
	void CopyFiles(LPCTSTR pszSource, LPCTSTR pszTarget, const CListCtrl& listFiles, BOOL fDeleteSrc);
	void ZipTargetFolder(LPCTSTR pszTarget, const CListCtrl& listFiles, LPCTSTR pszZipPath);
	void SendZippedFolder(const CString& strZipPath);
	void CreateFtpFolder(CFtpConnection* pFtpConn, LPCTSTR pszFolder);
	BOOL UploadFile(LPCTSTR pszSrcPath, LPCTSTR pszFtpPath, CFtpConnection* pFtpConn);
	void UploadFiles(LPCTSTR pszSource, const CListCtrl& listFiles);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#if defined(__INTEL_COMPILER)
#pragma warning(default: 444)
#endif	// __INTEL_COMPILER

#endif	// __ProgressPage_h

// end of file
