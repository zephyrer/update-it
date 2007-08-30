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

// FilesPage.h - interface of the CFilesPage class

#if !defined(__FilesPage_h)
#define __FilesPage_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif	// _MSC_VER

//! Encapsulates the "Selecting files" step of the UpdateIt! wizard.
class CFilesPage: public CBetterPropPage
{
	DECLARE_DYNAMIC(CFilesPage)
	DECLARE_MESSAGE_MAP()

// construction/destruction
public:
	CFilesPage(void);
	virtual ~CFilesPage(void);

// overridables
public:
	virtual BOOL OnInitDialog(void);
	virtual BOOL OnSetActive(void);
	virtual void OnBecameActive(void);
	virtual LRESULT OnWizardBack(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// message map functions
protected:
	afx_msg void OnDestroy(void);
	afx_msg void OnItemChanged(NMHDR* pHdr, LRESULT* pnResult);
	afx_msg void OnButtonRemove(void);

// attributes
public:
	BOOL m_fShowGrid;
	CStatic m_textInfo;
	CFilesList m_listFiles;
	CButton m_buttonRemove;
#if (_MFC_VER < 0x0700)
	DWORD m_cbFiles;
#else
	ULONGLONG m_cbFiles;
#endif	// _MFC_VER
	CStringList m_listExclude;
	CImageList m_imageList;
	CMap<CString, LPCTSTR, int, int> m_mapIcons;
	int m_iDefIcon;

// implementation helpers
private:
	BOOL IsFileMatchesExcludeList(LPCTSTR pszFilePath);
	void SearchForFiles(LPCTSTR pszFolder, BOOL fRecurse, CTime timeMin, int iRelative);
	void CleanupFileList(void);
	BOOL CompareContents(LPCTSTR pszRelativeName);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __FilesPage_h

// end of file
