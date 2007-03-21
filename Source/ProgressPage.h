// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// ProgressPage.h - interface of the CProgressPage class

#if !defined(__ProgressPage_h)
#define __ProgressPage_h

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

#endif	// __ProgressPage_h

// end of file
