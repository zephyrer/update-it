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

// construction
public:
	CFilesPage(void);

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
	afx_msg void OnGetDispInfo(NMHDR* pHdr, LRESULT* pnResult);
	afx_msg void OnColumnClick(NMHDR* pHdr, LRESULT* pnResult);
	afx_msg void OnButtonRemove(void);

// attributes
public:
	CStatic m_textInfo;
	enum {
		// column indices
		I_NAME,
		I_EXTENSION,
		I_PATH,
		I_DATE,
		I_TIME,
		NUM_COLUMNS		// should be the LAST enumerator!
	};
	CListCtrl m_listFiles;
	CButton m_buttonRemove;
	DWORD m_cbFiles;

// implementation helpers
private:
	void SearchForFiles(LPCTSTR pszFolder, BOOL fRecurse, CTime timeMin, int iRelative);
	void CleanupFileList(void);
	static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM nData);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __FilesPage_h

// end of file
