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

// ProgressPageBase.h - interface of the CProgressPageBase class

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__ProgressPageBase_h)
#define __ProgressPageBase_h

//////////////////////////////////////////////////////////////////////////////////////////////
// avoid unwanted ICL warnings

#if defined(__INTEL_COMPILER)
// remark #444: destructor for base class is not virtual
#pragma warning(disable: 444)
#endif   // __INTEL_COMPILER

//////////////////////////////////////////////////////////////////////////////////////////////
// dependent includes

#include "BetterPropPage.h"
#include "WindowsVersion.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CProgressPageBase

//! Encapsulates the "Processing files" step of the UpdateIt! wizard.
class CProgressPageBase: public CBetterPropPage
{
	DECLARE_DYNAMIC(CProgressPageBase)
	DECLARE_MESSAGE_MAP()

// construction
public:
	CProgressPageBase(void);

// overridables
public:
	virtual BOOL OnSetActive(void);
	virtual void OnBecameActive(void);
	virtual BOOL OnWizardFinish(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void ZipTargetFolder(LPCTSTR pszTarget, const CListCtrl& listFiles, LPCTSTR pszZipPath) = 0;

// attributes
public:
	CWindowsVersion m_winVer;
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
	void SendZippedFolder(const CString& strZipPath);
	void CreateFtpFolder(CFtpConnection* pFtpConn, LPCTSTR pszFolder);
	BOOL UploadFile(LPCTSTR pszSrcPath, LPCTSTR pszFtpPath, CFtpConnection* pFtpConn);
	void UploadFiles(LPCTSTR pszSource, const CListCtrl& listFiles);

// properties
private:
	UINT GetFileCopyAviID(void);
	__declspec(property(get = GetFileCopyAviID)) UINT m_idrFileCopy;
	UINT GetFileMoveAviID(void);
	__declspec(property(get = GetFileMoveAviID)) UINT m_idrFileMove;
	UINT GetFileDeleteAviID(void);
	__declspec(property(get = GetFileDeleteAviID)) UINT m_idrFileDelete;
	UINT GetFileTrashAviID(void);
	__declspec(property(get = GetFileTrashAviID)) UINT m_idrFileTrash;
	UINT GetFileZipAviID(void);
	__declspec(property(get = GetFileZipAviID)) UINT m_idrFileZip;
	UINT GetFileUploadAviID(void);
	__declspec(property(get = GetFileUploadAviID)) UINT m_idrFileUpload;
	UINT GetFileSendAviID(void);
	__declspec(property(get = GetFileSendAviID)) UINT m_idrFileSend;

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

//////////////////////////////////////////////////////////////////////////////////////////////
// inlines

inline UINT CProgressPageBase::GetFileCopyAviID(void)
{
	return (m_winVer.WinNT() < _WIN32_WINNT_WINXP ? IDR_FILECOPY_2000 : IDR_FILECOPY_XP);
}

inline UINT CProgressPageBase::GetFileMoveAviID(void)
{
	return (m_winVer.WinNT() < _WIN32_WINNT_WINXP ? IDR_FILEMOVE_2000 : IDR_FILEMOVE_XP);
}

inline UINT CProgressPageBase::GetFileDeleteAviID(void)
{
	return (m_winVer.WinNT() < _WIN32_WINNT_WINXP ? IDR_FILEDELETE_2000 : IDR_FILEDELETE_XP);
}

inline UINT CProgressPageBase::GetFileTrashAviID(void)
{
	return (m_winVer.WinNT() < _WIN32_WINNT_WINXP ? IDR_FILETRASH_2000 : IDR_FILETRASH_XP);
}

inline UINT CProgressPageBase::GetFileZipAviID(void)
{
	return (m_winVer.WinNT() < _WIN32_WINNT_WINXP ? IDR_FILEZIP_2000 : IDR_FILEZIP_XP);
}

inline UINT CProgressPageBase::GetFileUploadAviID(void)
{
	return (m_winVer.WinNT() < _WIN32_WINNT_WINXP ? IDR_FILEUPLOAD_2000 : IDR_FILEUPLOAD_XP);
}

inline UINT CProgressPageBase::GetFileSendAviID(void)
{
	// TODO: obtain XP-style animation
	return (IDR_FILESEND_2000);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// restore unwanted ICL warnings

#if defined(__INTEL_COMPILER)
#pragma warning(default: 444)
#endif   // __INTEL_COMPILER

#endif   // __ProgressPageBase_h

// end of file
