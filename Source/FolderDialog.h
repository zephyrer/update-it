// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// FolderDialog.h - interface of the CFolderDialog class

#if !defined(__FolderDialog_h)
#define __FolderDialog_h

class CFolderDialog: public CDialog
{
	DECLARE_DYNAMIC(CFolderDialog)

// construction/destruction
public:
	CFolderDialog(LPCTSTR pszTitle = NULL, CWnd* pParentWnd = NULL);
	CFolderDialog(UINT idTitle, CWnd* pParentWnd = NULL);
	virtual ~CFolderDialog(void);

// operations
public:
	virtual int DoModal(void);
	void EnableOK(BOOL fEnable = TRUE);
	void SetSelection(ITEMIDLIST* pidl);
	void SetSelection(LPCTSTR pszPath);
	void SetStatusText(LPCTSTR pszText);
	CString GetFolderPath(void) const;

// overridables
protected:
	virtual void OnInitialized(void);
	virtual void OnSelChanged(const ITEMIDLIST* pidl, const CString& strPath);
	virtual BOOL OnValidateFailed(LPCTSTR pszName);

// attributes
public:
	BROWSEINFO m_bi;
	CString m_strTitle;
	ITEMIDLIST* m_pidl;

// implementation helpers
protected:
	static int CALLBACK BrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

// diagnostic services
#if defined(_DEBUG)
public:
	virtual void AssertValid(void) const;
	virtual void Dump(CDumpContext& dumpCtx) const;
#endif
};

#endif	// __FolderDialog_h

// end of file
