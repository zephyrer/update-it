// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// FilesPage.h - interface of the CFilesPage class

#if !defined(__FilesPage_h)
#define __FilesPage_h

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
	DWORD m_cbFiles;
	CStringList m_listExclude;
	CImageList m_imageList;
	CMap<CString, LPCTSTR, int, int> m_mapIcons;
	int m_iDefIcon;

// implementation helpers
private:
	BOOL IsFileMatchesExcludeList(LPCTSTR pszFilePath);
	void SearchForFiles(LPCTSTR pszFolder, BOOL fRecurse, CTime timeMin, int iRelative);
	void CleanupFileList(void);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __FilesPage_h

// end of file
