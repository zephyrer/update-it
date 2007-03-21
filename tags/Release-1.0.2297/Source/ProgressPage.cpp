// UpdateIt! application.
// Copyright (c) 2002-2004 by Elijah Zarezky,
// All rights reserved.

// ProgressPage.cpp - implementation of the CProgressPage class

#include "stdafx.h"
#include "Resource.h"
#include "BetterPropPage.h"
#include "AboutPage.h"
#include "OptionsPage.h"
#include "FilesList.h"
#include "FilesPage.h"
#include "ActionPage.h"
#include "ProgressPage.h"
#include "CustomPropSheet.h"
#include "MainWizard.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif	// _DEBUG

// object model
IMPLEMENT_DYNAMIC(CProgressPage, CBetterPropPage)

// message map
BEGIN_MESSAGE_MAP(CProgressPage, CBetterPropPage)
END_MESSAGE_MAP()

CProgressPage::CProgressPage(void):
CBetterPropPage(IDD_PAGE_PROGRESS)
{
	m_psp.dwFlags |= PSP_PREMATURE;
}

BOOL CProgressPage::OnSetActive(void)
{
	BOOL fSuccess = CBetterPropPage::OnSetActive();
	if (fSuccess) {
		CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
		ASSERT(pWiz != NULL);
		pWiz->SetWizardButtons(PSWIZB_DISABLEDFINISH);
		pWiz->GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		CMenu* pSysMenu = pWiz->GetSystemMenu(FALSE);
		ASSERT_VALID(pSysMenu);
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	}
	return (fSuccess);
}

void CProgressPage::OnBecameActive(void)
{
	using CMainWizard::I_OPTIONS;
	using CMainWizard::I_FILES;
	using CMainWizard::I_ACTION;

	CString strWorking;
	CString strZipPath;

	CBetterPropPage::OnBecameActive();
	CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
	ASSERT(pWiz != NULL);
	COptionsPage* pOptionsPage = DYNAMIC_DOWNCAST(COptionsPage, pWiz->GetPage(I_OPTIONS));
	ASSERT(pOptionsPage != NULL);
	CFilesPage* pFilesPage = DYNAMIC_DOWNCAST(CFilesPage, pWiz->GetPage(I_FILES));
	ASSERT(pFilesPage != NULL);
	CActionPage* pActionPage = DYNAMIC_DOWNCAST(CActionPage, pWiz->GetPage(I_ACTION));
	ASSERT(pActionPage != NULL);
	// erase target folder (if needed)
	if (pOptionsPage->m_nCleanup == BST_CHECKED) {
		strWorking.LoadString(IDS_DELETING_FILES);
		m_textWorking.SetWindowText(strWorking);
		BOOL fCanUndo = pOptionsPage->m_nRecycle == BST_CHECKED;
		m_animateBanner.Open(fCanUndo ? IDR_FILETRASH : IDR_FILEDELETE);
		m_animateBanner.ShowWindow(SW_SHOW);
		m_animateBanner.Play(0, (UINT)-1, (UINT)-1);
		EraseFolder(pOptionsPage->m_strTarget, fCanUndo);
		m_animateBanner.ShowWindow(SW_HIDE);
		m_animateBanner.Stop();
		m_animateBanner.Close();
		PumpWaitingMessages();
	}
	// show needed controls
	m_textFile.ShowWindow(SW_SHOW);
	m_progressFile.ShowWindow(SW_SHOW);
	m_textTotal.ShowWindow(SW_SHOW);
	m_progressTotal.ShowWindow(SW_SHOW);
	// process files
	BOOL fDeleteSrc = pActionPage->m_nAction == 1;
	strWorking.LoadString(fDeleteSrc ? IDS_MOVING_FILES : IDS_COPYING_FILES);
	m_textWorking.SetWindowText(strWorking);
	m_animateBanner.Open(fDeleteSrc ? IDR_FILEMOVE : IDR_FILECOPY);
	m_animateBanner.ShowWindow(SW_SHOW);
	m_animateBanner.Play(0, (UINT)-1, (UINT)-1);
	int nLower = min(0UL, INT_MAX - pFilesPage->m_cbFiles);
	int nUpper = nLower + pFilesPage->m_cbFiles;
	m_progressTotal.SetRange32(nLower, nUpper);
	m_progressTotal.SetPos(nLower);
	CListCtrl& listFiles = pFilesPage->m_listFiles;
	CopyFiles(pOptionsPage->m_strSource, pOptionsPage->m_strTarget, listFiles, fDeleteSrc);
	m_animateBanner.ShowWindow(SW_HIDE);
	m_animateBanner.Stop();
	m_animateBanner.Close();
	PumpWaitingMessages();
	// upload files (if needed)
	if (pActionPage->m_nUpload == BST_CHECKED) {
		strWorking.LoadString(IDS_PREPARING_UPLOAD);
		m_textWorking.SetWindowText(strWorking);
		m_animateBanner.Open(IDR_FILEUPLOAD);
		m_animateBanner.ShowWindow(SW_SHOW);
		m_animateBanner.Play(0, (UINT)-1, (UINT)-1);
		m_textFile.SetWindowText(NULL);
		m_progressFile.SetRange(0, 0);
		m_progressFile.SetPos(0);
		m_progressTotal.SetRange32(nLower, nUpper);
		m_progressTotal.SetPos(nLower);
		UploadFiles(pOptionsPage->m_strSource, listFiles);
		m_animateBanner.ShowWindow(SW_HIDE);
		m_animateBanner.Stop();
		m_animateBanner.Close();
		PumpWaitingMessages();
	}
	// hide unneeded controls
	m_textFile.ShowWindow(SW_HIDE);
	m_progressFile.ShowWindow(SW_HIDE);
	// pack targer folder (if needed)
	if (pActionPage->m_nZip == BST_CHECKED) {
		strWorking.LoadString(IDS_ZIPPING_FOLDER);
		m_textWorking.SetWindowText(strWorking);
		m_animateBanner.Open(IDR_ZIPPING);
		m_animateBanner.ShowWindow(SW_SHOW);
		m_progressTotal.SetPos(nLower);
		m_animateBanner.Play(0, (UINT)-1, (UINT)-1);
		strZipPath = pOptionsPage->m_strTarget + _T(".zip");
		ZipTargetFolder(pOptionsPage->m_strTarget, listFiles, strZipPath);
		m_animateBanner.ShowWindow(SW_HIDE);
		m_animateBanner.Stop();
		m_animateBanner.Close();
		PumpWaitingMessages();
	}
	// hide unneeded controls
	m_textTotal.ShowWindow(SW_HIDE);
	m_progressTotal.ShowWindow(SW_HIDE);
	// send previously packed folder (if needed)
	if (pActionPage->m_nSend == BST_CHECKED) {
		strWorking.LoadString(IDS_SENDING_MAIL);
		m_textWorking.SetWindowText(strWorking);
		m_animateBanner.Open(IDR_FILESEND);
		m_animateBanner.ShowWindow(SW_SHOW);
		m_animateBanner.Play(0, (UINT)-1, (UINT)-1);
		SendZippedFolder(strZipPath);
		m_animateBanner.ShowWindow(SW_HIDE);
		m_animateBanner.Stop();
		m_animateBanner.Close();
		PumpWaitingMessages();
	}
	// all done!!
	strWorking.LoadString(IDS_ALL_DONE);
	m_textWorking.SetWindowText(strWorking);
	// remember options
	CTime timeNow = CTime::GetCurrentTime();
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("Times"), pOptionsPage->m_strSource, timeNow.GetTime());
	pApp->WriteProfileString(_T("Targets"), pOptionsPage->m_strSource, pOptionsPage->m_strTarget);
	// setup the buttons
	pWiz->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
	pWiz->GetDlgItem(IDCANCEL)->EnableWindow();
	CMenu* pSysMenu = pWiz->GetSystemMenu(FALSE);
	ASSERT_VALID(pSysMenu);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
}

BOOL CProgressPage::OnWizardFinish(void)
{
	return (CBetterPropPage::OnWizardFinish());
}

void CProgressPage::DoDataExchange(CDataExchange* pDX)
{
	CBetterPropPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WORKING, m_textWorking);
	DDX_Control(pDX, IDC_ANIMATE_BANNER, m_animateBanner);
	DDX_Control(pDX, IDC_STATIC_FILE, m_textFile);
	DDX_Control(pDX, IDC_PROGRESS_FILE, m_progressFile);
	DDX_Control(pDX, IDC_STATIC_TOTAL, m_textTotal);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_progressTotal);
}

bool CProgressPage::Callback(int iProgress)
{
	m_textTotal.SetWindowText(m_szFileInZip);
	m_progressTotal.OffsetPos(iProgress);
	PumpWaitingMessages();
	return (true);
}

void CProgressPage::CreateSubFolder(LPCTSTR pszRoot, LPCTSTR pszFolder)
{
	TCHAR szTemp[_MAX_PATH];

	ASSERT(pszRoot != NULL && *pszRoot != 0);

	::CreateDirectory(pszRoot, NULL);
	if (pszFolder != NULL && *pszFolder != 0) {
		CString strPath(pszRoot);
		if (strPath[strPath.GetLength() - 1] != _T('\\')) {
			strPath += _T('\\');
		}
		if (_tcschr(pszFolder, _T('\\')) != NULL) {
			::lstrcpy(szTemp, pszFolder);
			LPTSTR pszSub = _tcstok(szTemp, _T("\\"));
			do {
				strPath += pszSub;
				::CreateDirectory(strPath, NULL);
				strPath += _T('\\');
			} while ((pszSub = _tcstok(NULL, _T("\\"))) != NULL);
		}
		else {
			strPath += pszFolder;
			::CreateDirectory(strPath, NULL);
		}
	}
}

void CProgressPage::EraseFolder(LPCTSTR pszFolder, BOOL fCanUndo)
{
	CFileFind finder;

	ASSERT(pszFolder != NULL && *pszFolder != 0);
	CString strWildcard(pszFolder);
	if (strWildcard[strWildcard.GetLength() - 1] != _T('\\')) {
		strWildcard += _T('\\');
	}
	strWildcard += _T("*.*");
	BOOL fStop = !finder.FindFile(strWildcard);
	while (!fStop) {
		fStop = !finder.FindNextFile();
		if (!finder.IsDots()) {		// skip "." and ".."
			CString strPath = finder.GetFilePath();
			if (finder.IsDirectory()) {
				// search recursively
				EraseFolder(strPath, fCanUndo);
				::RemoveDirectory(strPath);
			}
			else {
				DeleteFile(strPath, fCanUndo);
			}
			PumpWaitingMessages();
		}
	}
	finder.Close();
}

BOOL CProgressPage::CopyFile(LPCTSTR pszSrcPath, LPCTSTR pszTargPath, BOOL fDeleteSrc)
{
	CFileException err;
	CFile fileIn, fileOut;
	UINT cbRead;
	BOOL fSuccess;

	// verify parameters
	ASSERT(pszSrcPath != NULL && *pszSrcPath != 0);
	ASSERT(pszTargPath != NULL && *pszTargPath != 0);

	// open source file for reading
	enum { modeIn = CFile::modeRead | CFile::shareExclusive };
	if (!fileIn.Open(pszSrcPath, modeIn, &err)) {
		// unable to open source file
		err.ReportError(MB_ICONSTOP | MB_OK);
		return (FALSE);
	}

	// create target file for writing
	enum { modeOut = CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive };
	if (!fileOut.Open(pszTargPath, modeOut, &err)) {
		// unable to create target file
		err.ReportError(MB_ICONSTOP | MB_OK);
		fileIn.Close();
		return (FALSE);
	}

	// copy data
	enum { BUF_SIZE = 4096 };
	BYTE* pbBuf = new BYTE[BUF_SIZE];
	try {
		while ((cbRead = fileIn.Read(pbBuf, BUF_SIZE)) > 0) {
			fileOut.Write(pbBuf, cbRead);
			m_progressFile.OffsetPos(cbRead);
			m_progressTotal.OffsetPos(cbRead);
			PumpWaitingMessages();
		}
		fSuccess = TRUE;
	}
	catch (CFileException* pErr) {
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
		fSuccess = FALSE;
	}
	delete[] pbBuf;

	// close files
	fileOut.Close();
	fileIn.Close();

	// delete source file (if needed)
	if (fSuccess && fDeleteSrc) {
		DeleteFile(pszSrcPath);
	}

	// successfully copied/moved
	return (fSuccess);
}

int CProgressPage::DeleteFile(LPCTSTR pszPath, BOOL fCanUndo)
{
	SHFILEOPSTRUCT fos;

	memset(&fos, 0, sizeof(fos));
	fos.wFunc = FO_DELETE;
	int cchFrom = ::lstrlen(pszPath) + 2;
	fos.pFrom = new TCHAR[cchFrom];
	::lstrcpy(const_cast<LPTSTR>(fos.pFrom), pszPath);
	const_cast<LPTSTR>(fos.pFrom)[cchFrom - 1] = 0;
	fos.fFlags = FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION;
	if (fCanUndo) {
		fos.fFlags |= FOF_ALLOWUNDO;
	}
	int nResult = ::SHFileOperation(&fos);
	delete[] const_cast<LPTSTR>(fos.pFrom);
	return (nResult);
}

void CProgressPage::CopyFiles(LPCTSTR pszSource, LPCTSTR pszTarget, const CListCtrl& listFiles, BOOL fDeleteSrc)
{
	int cItems = listFiles.GetItemCount();
	for (int i = 0; i < cItems; ++i) {
		FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(listFiles.GetItemData(i));
		ASSERT(pData != NULL);
		// construct the relative name
		CString strRelativeName(pData->szFolder);
		if (!strRelativeName.IsEmpty()) {
			strRelativeName += _T('\\');
		}
		strRelativeName += pData->szName;
		strRelativeName += _T('.');
		strRelativeName += pData->szExt;
		// prepare path names...
		CString strSrcPath(pszSource);
		if (strSrcPath[strSrcPath.GetLength() - 1] != _T('\\')) {
			strSrcPath += _T('\\');
		}
		strSrcPath += strRelativeName;
		CString strTargPath(pszTarget);
		if (strTargPath[strTargPath.GetLength() - 1] != _T('\\')) {
			strTargPath += _T('\\');
		}
		strTargPath += strRelativeName;
		// ...and controls
		m_textFile.SetWindowText(strRelativeName);
		int nLower = min(0UL, INT_MAX - pData->cbLength);
		int nUpper = nLower + pData->cbLength;
		m_progressFile.SetRange32(nLower, nUpper);
		m_progressFile.SetPos(nLower);
		// create the target folder
		CreateSubFolder(pszTarget, pData->szFolder);
		// now copy the file
		CopyFile(strSrcPath, strTargPath, fDeleteSrc);
		// adjust modification and access time of the copied file
		struct _utimbuf utmb = { CTime::GetCurrentTime().GetTime(), pData->timeWrite.GetTime() };
		_tutime(strTargPath, &utmb);
	}
}

void CProgressPage::ZipTargetFolder(LPCTSTR pszTarget, const CListCtrl& listFiles, LPCTSTR pszZipPath)
{
	CZipArchive zipArch;

	try {
		zipArch.SetCallback(this, CZipArchive::cbAdd);
		zipArch.Open(pszZipPath, CZipArchive::zipCreate);
		CString strTemp(pszTarget);
		zipArch.SetRootPath(strTemp.Left(strTemp.ReverseFind(_T('\\'))));
		int cFiles = cFiles = listFiles.GetItemCount();
		for (int i = 0; i < cFiles; ++i) {
			CString strFilePath(pszTarget);
			if (strFilePath[strFilePath.GetLength() - 1] != _T('\\')) {
				strFilePath += _T('\\');
			}
			FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(listFiles.GetItemData(i));
			ASSERT(pData != NULL);
			if (pData->szFolder[0] != 0) {
				strFilePath += pData->szFolder;
				strFilePath += _T('\\');
			}
			strFilePath += pData->szName;
			strFilePath += _T('.');
			strFilePath += pData->szExt;
			zipArch.AddNewFile(strFilePath, Z_BEST_COMPRESSION, false);
		}
		zipArch.Close();
	}
	catch (CZipException* pErr) {
		AfxMessageBox(pErr->GetErrorDescription(), MB_ICONSTOP | MB_OK);
		delete pErr;
		zipArch.Close();
	}
}

void CProgressPage::SendZippedFolder(const CString& strZipPath)
{
	using CMainWizard::I_ACTION;
	using CSMTPConnection::NoLoginMethod;

	CSMTPConnection smtpConn;
	CSMTPMessage smtpMsg;
	CSMTPBodyPart smtpTextPart;
	CSMTPBodyPart smtpZipPart;

	CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
	ASSERT(pWiz != NULL);
	CActionPage* pActionPage = DYNAMIC_DOWNCAST(CActionPage, pWiz->GetPage(I_ACTION));
	ASSERT(pActionPage != NULL);

	smtpMsg.m_sXMailer = _T("UpdateIt/1.0");
	smtpMsg.m_From = CSMTPAddress(pActionPage->m_strFrom);
	CSMTPAddress smtpAddr(pActionPage->m_strTo);
	smtpMsg.AddRecipient(smtpAddr);
	smtpMsg.m_sSubject = pActionPage->m_strSubject;
	smtpZipPart.SetFilename(strZipPath);
	smtpTextPart.SetText(pActionPage->m_strBody);
	CWinApp* pApp = AfxGetApp();
	CString strCharSet = pApp->GetProfileString(_T("SMTP"), _T("charset"));
	if (strCharSet.IsEmpty()) {
		strCharSet.Format(IDS_CHARSET_FORMAT, ::GetACP());
	}
	smtpTextPart.SetCharset(strCharSet);
	smtpMsg.AddBodyPart(smtpTextPart);
	smtpMsg.AddBodyPart(smtpZipPart);
	smtpConn.Connect(pActionPage->m_strHost, NoLoginMethod, NULL, NULL, pActionPage->m_nPort);
	if (!smtpConn.SendMessage(smtpMsg)) {
		AfxMessageBox(IDS_SEND_FAILED, MB_ICONSTOP | MB_OK);
	}
	smtpConn.Disconnect();
}

void CProgressPage::CreateFtpFolder(CFtpConnection* pFtpConn, LPCTSTR pszFolder)
{
	ASSERT_VALID(pFtpConn);
	ASSERT(pszFolder != NULL && *pszFolder != 0);

	TCHAR szTemp[_MAX_PATH];
	CString strExisting;

	::lstrcpy(szTemp, pszFolder);
	LPTSTR pszPart = _tcstok(szTemp, _T("/"));
	// phase one --
	while (pszPart != NULL) {
		CFtpFileFind ftpFinder(pFtpConn);
		CString strTest(strExisting);
		strTest += _T('/');
		strTest += pszPart;
		if (!ftpFinder.FindFile(strTest)) {
			break;
		}
		else {
			ftpFinder.FindNextFile();
			if (!ftpFinder.IsDirectory()) {
				pFtpConn->Remove(strTest);
				break;
			}
		}
		strExisting = strTest;
		pszPart = _tcstok(NULL, _T("/"));
	}
	// phase two --
	if (pszPart != NULL) {
		do {
			strExisting += _T('/');
			strExisting += pszPart;
			pFtpConn->CreateDirectory(strExisting);
		} while ((pszPart = _tcstok(NULL, _T("/"))) != NULL);
	}
}

BOOL CProgressPage::UploadFile(LPCTSTR pszSrcPath, LPCTSTR pszFtpPath, CFtpConnection* pFtpConn)
{
	CFileException err;
	CFile fileIn;
	CInternetFile* pFileOut;
	UINT cbRead;
	BOOL fSuccess;

	// verify parameters
	ASSERT(pszSrcPath != NULL && *pszSrcPath != 0);
	ASSERT(pszFtpPath != NULL && *pszFtpPath != 0);
	ASSERT_VALID(pFtpConn);

	// open source file for reading
	enum { modeIn = CFile::modeRead | CFile::shareExclusive };
	if (!fileIn.Open(pszSrcPath, modeIn, &err)) {
		// unable to open source file
		err.ReportError(MB_ICONSTOP | MB_OK);
		return (FALSE);
	}

	// open target file for writing
	try {
		pFileOut = pFtpConn->OpenFile(pszFtpPath, GENERIC_WRITE);
	}
	catch (CInternetException* pErr) {
		// unable to open target file
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
		fileIn.Close();
		return (FALSE);
	}

	// copy data
	enum { BUF_SIZE = 4096 };
	BYTE* pbBuf = new BYTE[BUF_SIZE];
	try {
		while ((cbRead = fileIn.Read(pbBuf, BUF_SIZE)) > 0) {
			pFileOut->Write(pbBuf, cbRead);
			m_progressFile.OffsetPos(cbRead);
			m_progressTotal.OffsetPos(cbRead);
			PumpWaitingMessages();
		}
		fSuccess = TRUE;
	}
	catch (CFileException* pErr) {
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
		fSuccess = FALSE;
	}
	catch (CInternetException* pErr) {
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
		fSuccess = FALSE;
	}
	delete[] pbBuf;

	// close files
	pFileOut->Close();
	delete pFileOut;
	fileIn.Close();

	// successfully uploaded
	return (fSuccess);
}

void CProgressPage::UploadFiles(LPCTSTR pszSource, const CListCtrl& listFiles)
{
	using CMainWizard::I_ACTION;

	CString strWorking;

	ASSERT(pszSource != NULL && *pszSource != 0);

	CMainWizard* pWiz = DYNAMIC_DOWNCAST(CMainWizard, GetParent());
	ASSERT(pWiz != NULL);
	CActionPage* pActionPage = DYNAMIC_DOWNCAST(CActionPage, pWiz->GetPage(I_ACTION));
	ASSERT(pActionPage != NULL);

	try {
		strWorking.LoadString(IDS_STARTING_FTP);
		m_textWorking.SetWindowText(strWorking);
		CInternetSession ftpSession(_T("UpdateIt/1.0"));
		strWorking.LoadString(IDS_CONNECTING_FTP);
		m_textWorking.SetWindowText(strWorking);
		CFtpConnection* pFtpConn = ftpSession.GetFtpConnection(pActionPage->m_strServer,
			pActionPage->m_strLogin, pActionPage->m_strPassword, pActionPage->m_nFtpPort);
		ASSERT(pFtpConn != NULL);
		pFtpConn->SetCurrentDirectory(_T("/"));
		strWorking.LoadString(IDS_UPLOADING_FILES);
		m_textWorking.SetWindowText(strWorking);
		int cFiles = listFiles.GetItemCount();
		for (int i = 0; i < cFiles; ++i) {
			FILE_DATA* pData = reinterpret_cast<FILE_DATA*>(listFiles.GetItemData(i));
			ASSERT(pData != NULL);
			CString strNameExt(pData->szName);
			strNameExt += _T('.');
			strNameExt += pData->szExt;
			// construct fully qualified source filename
			CString strSrcPath(pszSource);
			if (strSrcPath[strSrcPath.GetLength() - 1] != _T('\\')) {
				strSrcPath += _T('\\');
			}
			CString strFolder(pData->szFolder);
			if (!strFolder.IsEmpty()) {
				strSrcPath += strFolder + _T('\\');
			}
			strSrcPath += strNameExt;
			// construct fully qualified target filename
			CString strFtpPath(pActionPage->m_strRoot);
			if (!strFolder.IsEmpty()) {
				strFolder.Replace(_T('\\'), _T('/'));
				strFtpPath += strFolder + _T('/');
			}
			strFtpPath += strNameExt;
			// prepare controls
			m_textFile.SetWindowText(strFtpPath);
			int nLower = min(0UL, INT_MAX - pData->cbLength);
			int nUpper = nLower + pData->cbLength;
			m_progressFile.SetRange32(nLower, nUpper);
			m_progressFile.SetPos(nLower);
			// ensure that the target folder exists
			int cchFolder = strFtpPath.ReverseFind(_T('/'));
			if (cchFolder > 0) {
				CreateFtpFolder(pFtpConn, strFtpPath.Left(cchFolder));
			}
			// now upload the file
			UploadFile(strSrcPath, strFtpPath, pFtpConn);
		}
		strWorking.LoadString(IDS_DISCONNECTING_FTP);
		m_textWorking.SetWindowText(strWorking);
		pFtpConn->Close();
		delete pFtpConn;
		strWorking.LoadString(IDS_FINISHING_FTP);
		m_textWorking.SetWindowText(strWorking);
		ftpSession.Close();
	}
	catch (CInternetException* pErr) {
		pErr->ReportError(MB_ICONSTOP | MB_OK);
		pErr->Delete();
	}
}

#if defined(_DEBUG)

//! This member function performs a validity check on this object by checking its
//! internal state. In the Debug version of the application, may assert and thus
//! terminate the program with a message that lists the line number and filename
//! where the assertion failed.
//! @brief performs a validity check on the object
void CProgressPage::AssertValid(void) const
{
	// first perform inherited validity check...
	CBetterPropPage::AssertValid();
	// ...and then verify own state as well
	ASSERT_VALID(&m_textWorking);
	ASSERT_VALID(&m_animateBanner);
	ASSERT_VALID(&m_textFile);
	ASSERT_VALID(&m_progressFile);
	ASSERT_VALID(&m_textTotal);
	ASSERT_VALID(&m_progressTotal);
}

//! This member function prints data members of this class (in the Debug version
//! of the application).
//! @brief dumps the contents of the object to a CDumpContext object
//! @param dumpCtx the diagnostic dump context for dumping, usually afxDump.
void CProgressPage::Dump(CDumpContext& dumpCtx) const
{
	try {
		// first invoke inherited dumper...
		CBetterPropPage::Dump(dumpCtx);
		// ...and then dump own unique members
		dumpCtx << "m_textWorking = " << m_textWorking;
		dumpCtx << "\nm_animateBanner = " << m_animateBanner;
		dumpCtx << "\nm_textFile = " << m_textFile;
		dumpCtx << "\nm_progressFile = " << m_progressFile;
		dumpCtx << "\nm_textTotal = " << m_textTotal;
		dumpCtx << "\nm_progressTotal = " << m_progressTotal;
	}
	catch (CFileException* pXcpt) {
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif	// _DEBUG

// end of file
