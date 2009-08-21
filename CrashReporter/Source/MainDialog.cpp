// CrashReporter application.
// Copyright (c) 2009 by Elijah Zarezky,
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

// MainDialog.cpp - implementation of the CMainDialog class
// Portions copyright (c) 2003 by Hans Dietrich (hdietrich2@hotmail.com)

// initially generated by AfxScratch v1.1.4778 on 14.08.2009 at 18:13:40
// visit http://zarezky.spb.ru/projects/afx_scratch.html for more info

//////////////////////////////////////////////////////////////////////////////////////////////
// PCH includes

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// resource includes

#include "../../Languages/English_USA.1252/Source/Resource.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// other includes

#include "DetailsDialog.h"
#include "MainDialog.h"
#include "CrashReporterApp.h"
#include "../../Common/CrashFileNames.h"
#include "../../Common/IniDefines.h"
#include "GetFileSizeAsString.h"
#include "../../Common/MiniVersion.h"
#include "BackupFile.h"
#include "../../Common/Registry.h"
#include "../../Common/RegistryDefines.h"
#include "WriteRegistry.h"
#include "../../Common/EmailDefines.h"

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)   // From WINUSER.H
#endif

#define MAX_USER_COMMENTS_SIZE (64 * 1024)

//////////////////////////////////////////////////////////////////////////////////////////////
// debugging support

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif   // _DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// object model

IMPLEMENT_DYNAMIC(CMainDialog, CDialog)

//////////////////////////////////////////////////////////////////////////////////////////////
// message map

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_APP_ICON, OnAppIcon)
	ON_BN_CLICKED(IDC_DO_NOT_SEND_ERROR, OnDoNotSend)
	ON_BN_CLICKED(IDC_SEND_ERROR, OnSend)
	// this message is sent by XHyperLink
	ON_REGISTERED_MESSAGE(WM_XHYPERLINK_CLICKED, OnClickHere)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////
// construction/destruction

CMainDialog::CMainDialog(CWnd* pParentWnd):
CDialog(IDD_XCRASHREPORT_MAIN, pParentWnd),
m_hIcon(NULL),
m_hSmIcon(NULL),
m_hLinkCursor(NULL),
m_nFilesInZip(0),
m_bOverIcon(FALSE),
m_hPrevCursor(NULL)
{
	CCrashReporterApp* pApp = DYNAMIC_DOWNCAST(CCrashReporterApp, AfxGetApp());
	ASSERT_VALID(pApp);

	// load dialog's icons
	m_hIcon = pApp->LoadIcon(IDI_APP_ICON);
	m_hSmIcon = pApp->LoadSmIcon(MAKEINTRESOURCE(IDI_APP_ICON));
}

CMainDialog::~CMainDialog(void)
{
	for (int i = 0; i < m_aFileDetails.GetSize(); i++)
	{
		FILEDETAILS *fd = (FILEDETAILS *) m_aFileDetails[i];
		if (fd)
			delete fd;
	}

	if (m_hLinkCursor)
		DestroyCursor(m_hLinkCursor);
	m_hLinkCursor = NULL;

	::DestroyIcon(m_hSmIcon);
	::DestroyIcon(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// overridables

BOOL CMainDialog::OnInitDialog(void)
{
	__super::OnInitDialog();

	// set dialog's icons
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hSmIcon, FALSE);

	// get our path name - we assume that crashed exe is in same directory
	ZeroMemory(m_szModulePath, sizeof(m_szModulePath));
	::GetModuleFileName(NULL, m_szModulePath, _countof(m_szModulePath) - 2);
	TCHAR *cp = _tcsrchr(m_szModulePath, _T('\\'));
	if (*cp)
	{
		*(cp + 1) = _T('\0');   // remove file name
	}

	InitializeDisplay();
	GetRegistryDetails();
	GetFileDetails();

	m_nFilesInZip = ZipFiles();

	LoadHandCursor();
	SetTimer(1, 80, NULL);

	// initialized
	return (TRUE);
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WHAT, m_What);
	DDX_Control(pDX, IDC_CLICK_HERE, m_ClickHere);
	DDX_Control(pDX, IDC_PLEASE_TELL_US, m_PleaseTellUs);
	DDX_Control(pDX, IDC_BANNER, m_Banner);
	DDX_Control(pDX, IDC_APP_ICON, m_Icon);
}

void CMainDialog::OnOK(void) 
{
	// no-op: do not let Enter close the dialog	
}

void CMainDialog::OnCancel() 
{
	// no-op: do not let Cancel close the dialog	
}

//////////////////////////////////////////////////////////////////////////////////////////////
// message map functions

void CMainDialog::OnTimer(UINT /*nIDEvent*/) 
{
	CPoint point(GetMessagePos());
	ScreenToClient(&point);

	CRect rect;
	m_Icon.GetWindowRect(&rect);
	ScreenToClient(&rect);

	// check if cursor is over icon
	if (!m_bOverIcon && rect.PtInRect(point))
	{
		m_bOverIcon = TRUE;
		if (m_hLinkCursor)
			m_hPrevCursor = ::SetCursor(m_hLinkCursor);
	}
	else if (m_bOverIcon && !rect.PtInRect(point))
	{
		m_bOverIcon = FALSE;
		if (m_hPrevCursor)
			::SetCursor(m_hPrevCursor);
	}
}

BOOL CMainDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bOverIcon && m_hLinkCursor)
	{
		::SetCursor(m_hLinkCursor);
		return TRUE;
	}
	else {
		return __super::OnSetCursor(pWnd, nHitTest, message);
	}
}

void CMainDialog::OnAppIcon(void) 
{
	CCrashReporterApp* pApp = DYNAMIC_DOWNCAST(CCrashReporterApp, AfxGetApp());
	ASSERT_VALID(pApp);

	CString strExe = m_szModulePath;
	strExe += pApp->m_strUpdateItExe;

	SHELLEXECUTEINFO sei = { 0 };

	ZeroMemory(&sei,sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.lpFile = strExe;
	sei.lpVerb = _T("properties");
	sei.fMask  = SEE_MASK_INVOKEIDLIST;
	sei.nShow  = SW_SHOWNORMAL;

	::ShellExecuteEx(&sei); 
}

void CMainDialog::OnDoNotSend(void) 
{
	__super::OnCancel();
}

LRESULT CMainDialog::OnClickHere(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_aFileDetails.GetSize() > 0)
	{
		CDetailsDialog dlgDetails(&m_aFileDetails);
		dlgDetails.DoModal();
	}

	return 0;
}

void CMainDialog::OnSend(void)
{
	CCrashReporterApp* pApp = DYNAMIC_DOWNCAST(CCrashReporterApp, AfxGetApp());
	ASSERT_VALID(pApp);

	CString strIniPath = pApp->m_strAppDataPath;
	strIniPath += INI_FILE_NAME;
	CIniAppSettings iniFile(strIniPath);

	TCHAR szSubject[256] = { 0 };
	_sntprintf(szSubject, _countof(szSubject) - 2, _T("Error report for %s"), pApp->m_strUpdateItExe);

	TCHAR szMessage[MAX_USER_COMMENTS_SIZE] = { 0 };
	m_What.GetWindowText(szMessage, _countof(szMessage) - 2);

	TCHAR szAttachmentPath[MAX_PATH * 2] = { 0 };

	// create new zip file, in case user unchecked some files
	m_nFilesInZip = ZipFiles();

	if (m_nFilesInZip > 0)
	{
		_tcsncpy(szAttachmentPath, m_strZipFile, _countof(szAttachmentPath) - 2);
	}
	else if (szMessage[0] == _T('\0'))
	{
		// no files in zip - check if there is a message
		AfxMessageBox(IDS_NO_ZIP_AND_MESSAGE, MB_ICONSTOP | MB_OK);
		return;
	}

	try
	{
		CSmtpConnection smtpConn;
		CSmtpMessage smtpMsg;
		CSmtpBodyPart smtpTextPart;
		CSmtpBodyPart smtpZipPart;

		CString strFrom = pApp->GetProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_FROM);
		if (strFrom.IsEmpty())
		{
			static const TCHAR szDefFrom[] = _T("anonymous@unknown.org");
			::GetPrivateProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_FROM, szDefFrom, strFrom.GetBuffer(256), 256, strIniPath);
			strFrom.ReleaseBuffer();
		}

		CString strTo(XCRASHREPORT_SEND_TO_ADDRESS);

		CString strHost = pApp->GetProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_HOST);
		if (strHost.IsEmpty())
		{
			static const TCHAR szDefHost[] = _T("mail.unknown.org");
			::GetPrivateProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_HOST, szDefHost, strHost.GetBuffer(256), 256, strIniPath);
			strHost.ReleaseBuffer();
		}

		INTERNET_PORT nPort = LOWORD(pApp->GetProfileInt(SZ_REGK_SMTP, SZ_REGV_SMTP_PORT, IPPORT_SMTP));
		if (nPort == INTERNET_INVALID_PORT_NUMBER || nPort > INTERNET_MAX_PORT_NUMBER_VALUE)
		{
			nPort = IPPORT_SMTP;
		}

		smtpMsg.m_sXMailer = _T("CrashReporter/1.0");
		smtpMsg.m_From = CSmtpAddress(strFrom);
		smtpMsg.ParseMultipleRecipients(strTo, smtpMsg.m_To);
		smtpMsg.m_sSubject = szSubject;
		smtpZipPart.SetFilename(szAttachmentPath);
		smtpTextPart.SetText(szMessage);
#if defined(UNICODE) || defined(_UNICODE)
		smtpTextPart.SetCharset(_T("UTF-8"));
#else
		CString strCharSet = pApp->GetProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_CHARSET);
		if (strCharSet.IsEmpty())
		{
			strCharSet.Format(IDS_CHARSET_FORMAT, ::GetACP());
			TCHAR szIniCharSet[256] = { 0 };
			::GetPrivateProfileString(SZ_REGK_SMTP, SZ_REGV_SMTP_CHARSET, strCharSet, szIniCharSet, _countof(szIniCharSet), strIniPath);
			if (strCharSet.CompareNoCase(szIniCharSet) != 0)
			{
				strCharSet = szIniCharSet;
			}
		}
		smtpTextPart.SetCharset(strCharSet);
#endif   // UNICODE
		smtpMsg.AddBodyPart(smtpTextPart);
		smtpMsg.AddBodyPart(smtpZipPart);

		smtpConn.Connect(strHost, CSmtpConnection::AUTH_NONE, NULL, NULL, nPort);
		smtpConn.SendMessage(smtpMsg);
		smtpConn.Disconnect();
	}
	catch (CSmtpException* pErr)
	{
		AfxMessageBox(pErr->GetErrorMessage(), MB_ICONSTOP | MB_OK);
		delete pErr;
	}
	catch (CAppSettingsException* pErr)
	{
		AfxMessageBox(pErr->GetErrorMessage(), MB_ICONSTOP | MB_OK);
		delete pErr;
	}

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// implementation helpers

void CMainDialog::GetFileDetails(void)
{
	// first try to read file details from INI file - 
	// INI file has format:
	//     [FilesToAdd]
	//     File001=<file-path>,<description>,<file-type>
	//     File002=<file-path>,<description>,<file-type>
	//     etc.

	TCHAR szBuf[2000];
	CString strEntry       = _T("");
	CString strPath        = _T("");
	CString strDescription = _T("");
	CString strType        = _T("");
	CString strFileSize    = _T("");

	CCrashReporterApp* pApp = DYNAMIC_DOWNCAST(CCrashReporterApp, AfxGetApp());
	ASSERT_VALID(pApp);

	CString strIniPath = pApp->m_strAppDataPath;
	strIniPath += INI_FILE_NAME;

	int nCount = 0;

	for (int n = 1; n <= MAX_INI_ITEMS; n++)
	{
		szBuf[0]       = _T('\0');
		strPath        = _T("");
		strDescription = _T("");
		strType        = _T("");
		strFileSize    = _T("");

		strEntry.Format(INI_FILE_TEMPLATE, n);

		::GetPrivateProfileString(INI_FILE_SECTION, 
								  strEntry, 
								  _T(""), 
								  szBuf, 
								  _countof(szBuf)-10,
								  strIniPath);

		if (szBuf[0] == _T('\0'))
			break;

		// get path
		TCHAR *delims = _T(",\r\n");
		TCHAR *cp = _tcstok(szBuf, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad file entry\n"));
			break;
		}
		strPath = cp;

		// get description
		cp = _tcstok(NULL, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad file entry\n"));
			break;
		}
		strDescription = cp;

		// get type
		cp = _tcstok(NULL, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad file entry\n"));
			break;
		}
		strType = cp;

		if (strPath.IsEmpty())
			continue;

		CString strFilePath = strPath;
		if (strPath.Find(_T('\\')) == -1)
		{
			// no path, so use exe's directory
			strFilePath = m_szModulePath;
			strFilePath += strPath;
		}

		if (!GetFileSizeAsString(strFilePath, strFileSize))
			continue;

		TRACE(_T("adding file %s from ini\n"), strFilePath);

		FILEDETAILS *fd    = new FILEDETAILS;
		ASSERT(fd);
		fd->strFilePath    = strFilePath;
		fd->strDescription = strDescription;
		fd->strType        = strType;
		fd->strFileSize    = strFileSize;

		m_aFileDetails.Add((DWORD) fd);

		nCount++;
	}

	// if there are no files, try to add the default files -
	// these names are defined in CrashFileNames.h.
	if (nCount == 0)
	{
		TRACE(_T("no files added from ini, using defaults\n"));

		CCrashReporterApp* pApp = DYNAMIC_DOWNCAST(CCrashReporterApp, AfxGetApp());
		ASSERT_VALID(pApp);

		CString strFilePath = pApp->m_strAppDataPath;
		strFilePath += XCRASHREPORT_ERROR_LOG_FILE;
		if (_taccess(strFilePath, 00) == 0)
		{
			if (GetFileSizeAsString(strFilePath, strFileSize))
			{
				TRACE(_T("adding default file %s\n"), 
					strFilePath);
				FILEDETAILS *fd    = new FILEDETAILS;
				ASSERT(fd);
				fd->strFilePath    = strFilePath;
				fd->strDescription = _T("Crash log");
				fd->strType        = _T("Text Document");
				fd->strFileSize    = strFileSize;
				m_aFileDetails.Add((DWORD) fd);
			}
		}

		strFilePath = pApp->m_strAppDataPath;
		strFilePath += XCRASHREPORT_MINI_DUMP_FILE;
		if (_taccess(strFilePath, 00) == 0)
		{
			if (GetFileSizeAsString(strFilePath, strFileSize))
			{
				TRACE(_T("adding default file %s\n"), 
					strFilePath);
				FILEDETAILS *fd    = new FILEDETAILS;
				ASSERT(fd);
				fd->strFilePath    = strFilePath;
				fd->strDescription = _T("Crash Dump");
				fd->strType        = _T("DMP File");
				fd->strFileSize    = strFileSize;
				m_aFileDetails.Add((DWORD) fd);
			}
		}
	}

#ifdef _DEBUG
	for (int i = 0; i < m_aFileDetails.GetSize(); i++)
	{
		FILEDETAILS *fd = (FILEDETAILS *) m_aFileDetails[i];
		TRACE(_T("fd[%d] = %s\n"), i, fd->strFilePath);
	}
#endif

}

void CMainDialog::GetRegistryDetails(void)
{
	TRACE(_T("in CDlg::GetRegistryDetails\n"));

#ifdef XCRASHREPORT_DUMP_REGISTRY

	// first try to read registry details from INI file - 
	// INI file has format:
	//     [RegistryToAdd]
	//     Registry001=<registry-path>,<description>
	//     Registry002=<registry-path>,<description>
	//     etc.
	//
	// For each entry, registry file RegistryNNN.txt
	// will be created.

	TCHAR szBuf[2000];
	CString strEntry       = _T("");
	CString strPath        = _T("");
	CString strDescription = _T("");
	CString strType        = _T("Text Document");	// default for reg files
	CString strFileSize    = _T("");
	CString strFilePath    = _T("");

	CCrashReporterApp* pApp = DYNAMIC_DOWNCAST(CCrashReporterApp, AfxGetApp());
	ASSERT_VALID(pApp);

	CString strIniPath = pApp->m_strAppDataPath;
	strIniPath += INI_FILE_NAME;

	int nCount = 0;

	for (int n = 1; n <= MAX_INI_ITEMS; n++)
	{
		szBuf[0]       = _T('\0');
		strPath        = _T("");
		strDescription = _T("");
		strFileSize    = _T("");

		strEntry.Format(INI_REG_TEMPLATE, n);

		::GetPrivateProfileString(INI_REG_SECTION, 
								  strEntry, 
								  _T(""), 
								  szBuf, 
								  _countof(szBuf)-10,
								  strIniPath);

		if (szBuf[0] == _T('\0'))
			break;

		// get registry path
		TCHAR *delims = _T(",\r\n");
		TCHAR *cp = _tcstok(szBuf, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad registry entry\n"));
			break;
		}

		strPath = cp;

		if (strPath.IsEmpty())
			continue;

		// get description
		cp = _tcstok(NULL, delims);
		if (!cp)
		{
			TRACE(_T("ERROR - bad registry entry\n"));
			break;
		}

		strDescription = cp;

		// dump registry to file - note we do not use ".reg"
		// because we don't want people double-clicking it
		// into the registry
		CString strTemp = _T("");
		strTemp.Format(INI_REG_TEMPLATE, n);
		strFilePath.Format(_T("%s%s.txt"),
			m_szModulePath, strTemp);

		if (!WriteRegistryTreeToFile(strPath, strFilePath))
		{
			TRACE(_T("ERROR - Could not write registry hive %s\n"), strPath);
			continue;
		}

		if (!GetFileSizeAsString(strFilePath, strFileSize))
			continue;

		TRACE(_T("adding reg file %s from ini\n"), strFilePath);

		FILEDETAILS *fd    = new FILEDETAILS;
		ASSERT(fd);
		fd->strFilePath    = strFilePath;
		fd->strDescription = strDescription;
		fd->strType        = strType;
		fd->strFileSize    = strFileSize;

		m_aFileDetails.Add((DWORD) fd);

		nCount++;
	}

	// if there are no files, try to add the default reg entry -
	// this reg path is defined in RegistryDefines.h.
	if (nCount == 0)
	{
		TRACE(_T("no registry files added from ini, using default\n"));

		// dump registry to file
		CString strKey = XCRASHREPORT_REGISTRY_KEY;
		if (!strKey.IsEmpty())
		{
			CString strFile = XCRASHREPORT_REGISTRY_DUMP_FILE;
			if (!strFile.IsEmpty())
			{
				strFilePath = m_szModulePath;
				strFilePath += strFile;

				if (!WriteRegistryTreeToFile(strKey, strFilePath))
				{
					TRACE(_T("ERROR - Could not write registry hive %s\n"), 
						strKey);
				}
				else
				{
					if (GetFileSizeAsString(strFilePath, strFileSize))
					{
						TRACE(_T("adding default reg file %s\n"), 
							strFilePath);
						FILEDETAILS *fd    = new FILEDETAILS;
						ASSERT(fd);
						fd->strFilePath    = strFilePath;
						fd->strDescription = _T("Registry File");
						fd->strType        = strType;
						fd->strFileSize    = strFileSize;
						m_aFileDetails.Add((DWORD) fd);
					}
				}
			}
		}
	}

#ifdef _DEBUG
	for (int i = 0; i < m_aFileDetails.GetSize(); i++)
	{
		FILEDETAILS *fd = (FILEDETAILS *) m_aFileDetails[i];
		TRACE(_T("fd[%d] = %s\n"), i, fd->strFilePath);
	}
#endif

#endif   // XCRASHREPORT_DUMP_REGISTRY

}

void CMainDialog::InitializeDisplay(void)
{
	CCrashReporterApp* pApp = DYNAMIC_DOWNCAST(CCrashReporterApp, AfxGetApp());
	ASSERT_VALID(pApp);

	m_Banner.SetTextColor(RGB(0,0,0));
	m_Banner.SetBackgroundColor(RGB(255,255,255));
	m_Banner.SetBold(TRUE);

	// get path to exe that crashed - we assume it is in same directory
	CString strExe = m_szModulePath;
	strExe += pApp->m_strUpdateItExe;

	// get version resource
	CMiniVersion ver(strExe);
	TCHAR szBuf[1000];
	szBuf[0] = _T('\0');

	// try to get description
	ver.GetFileDescription(szBuf, _countof(szBuf)-2);

	// if that failed try to get product name
	if (szBuf[0] == _T('\0'))
		ver.GetProductName(szBuf, _countof(szBuf)-2);

	ver.Release();

	// if that failed just use exe name
	if (szBuf[0] == _T('\0'))
		_tcscpy(szBuf, pApp->m_strUpdateItExe);

	SetWindowText(szBuf);

	CString strBanner;
	strBanner.Format(_T("%s has encountered a problem and needs to close.  ")
					 _T("We are sorry for the inconvenience."),
					 szBuf);

	m_Banner.SetWindowText(strBanner);
	m_Banner.SetMargins(22, 8);

	// get icon of crashed app
	HICON hIcon = ::ExtractIcon(AfxGetInstanceHandle(), strExe, 0);

	m_Icon.SetBackgroundColor(RGB(255,255,255), FALSE);
	if (hIcon)
		m_Icon.SetIcon(hIcon, FALSE);

	m_PleaseTellUs.SetBold(TRUE);

	m_What.SetLimitText(128*1024);	// set edit box to 128 KB

    m_ClickHere.SetColours(m_ClickHere.GetLinkColour(),
						   RGB(0,0,255), 
						   m_ClickHere.GetHoverColour());
	m_ClickHere.EnableURL(FALSE);
	m_ClickHere.EnableTooltip(FALSE);
	m_ClickHere.SetNotifyParent(TRUE);
}

void CMainDialog::LoadHandCursor(void)
{
	if (m_hLinkCursor == NULL)				// No cursor handle - try to load one
	{
		// First try to load the Win98 / Windows 2000 hand cursor

		TRACE(_T("loading from IDC_HAND\n"));
		m_hLinkCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);

		if (m_hLinkCursor == NULL)   // Still no cursor handle - load the WinHelp hand cursor
		{
			// The following appeared in Paul DiLascia's Jan 1998 MSJ articles.
			// It loads a "hand" cursor from the winhlp32.exe module.

			TRACE(_T("loading from winhlp32\n"));

			// Get the windows directory
			CString strWndDir;
			GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
			strWndDir.ReleaseBuffer();

			strWndDir += _T("\\winhlp32.exe");

			// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
			HMODULE hModule = LoadLibrary(strWndDir);
			if (hModule) 
			{
				HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
				if (hHandCursor)
					m_hLinkCursor = CopyCursor(hHandCursor);
				FreeLibrary(hModule);
			}
		}
	}
}

int CMainDialog::ZipFiles()
{
	CCrashReporterApp* pApp = DYNAMIC_DOWNCAST(CCrashReporterApp, AfxGetApp());
	ASSERT_VALID(pApp);

	int nFiles = 0;

	// generate zip file name
	m_strZipFile = pApp->m_strAppDataPath;
	m_strZipFile += pApp->m_strUpdateItExe;
	int index = m_strZipFile.ReverseFind(_T('.'));
	if (index > 0)
		m_strZipFile = m_strZipFile.Left(index);
	m_strZipFile += _T(".zip");
	TRACE(_T("m_strZipPath = <%s>\n"), m_strZipFile);

	// backup old zip if it exists
	BackupFile(m_strZipFile);

	HZIP hZip = NULL;

	for (int i = 0; i < m_aFileDetails.GetSize(); i++)
	{
		FILEDETAILS *fd = (FILEDETAILS *) m_aFileDetails[i];

		if (fd)
		{
			// make sure file exists
			if ((_taccess(fd->strFilePath, 00) == 0) && fd->bSend)
			{
				// if this is first file, create zip now
				if (!hZip)
				{
					hZip = CreateZip((LPVOID)(LPCTSTR)m_strZipFile, 0, ZIP_FILENAME);

					if (!hZip)
					{
						TRACE(_T("ERROR - failed to create %s\n"), m_strZipFile);
						break;
					}
				}

				// zip is open, add file

				CString strName = fd->strFilePath;
				int index = strName.ReverseFind(_T('\\'));
				if (index > 0)
					strName = strName.Right(strName.GetLength()-index-1);
				TRACE(_T("strName=<%s>\n"), strName);

				ZRESULT zr = ZipAdd(hZip, 
									strName, 
									(LPVOID)(LPCTSTR)fd->strFilePath, 
									0, 
									ZIP_FILENAME);
				if (zr == ZR_OK)
				{
					nFiles++;
				}
				else
				{
					TRACE(_T("ERROR - failed to add '%s' to zip\n"), fd->strFilePath);
				}
			}
		}
	}

	if (hZip)
		CloseZip(hZip);

	return nFiles;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// diagnostic services

#if defined(_DEBUG)

void CMainDialog::AssertValid(void) const
{
	// first perform inherited validity check...
	__super::AssertValid();

	// ...and then verify our own state as well
	ASSERT_VALID(&m_Banner);
	ASSERT_VALID(&m_Icon);
	ASSERT_VALID(&m_PleaseTellUs);
	ASSERT_VALID(&m_What);
	ASSERT_VALID(&m_ClickHere);
}

void CMainDialog::Dump(CDumpContext& dumpCtx) const
{
	try
	{
		// first invoke inherited dumper...
		__super::Dump(dumpCtx);

		// ...and then dump own unique members
		dumpCtx << "m_hIcon = " << m_hIcon;
		dumpCtx << "\nm_hSmIcon = " << m_hSmIcon;
		dumpCtx << "\nm_szModulePath = " << m_szModulePath;
		dumpCtx << "\nm_aFileDetails = " << m_aFileDetails;
		dumpCtx << "\nm_Banner = " << m_Banner;
		dumpCtx << "\nm_Icon = " << m_Icon;
		dumpCtx << "\nm_PleaseTellUs = " << m_PleaseTellUs;
		dumpCtx << "\nm_What = " << m_What;
		dumpCtx << "\nm_ClickHere = " << m_ClickHere;
		dumpCtx << "\nm_hLinkCursor = " << m_hLinkCursor;
		dumpCtx << "\nm_strZipFile = " << m_strZipFile;
		dumpCtx << "\nm_nFilesInZip = " << m_nFilesInZip;
		dumpCtx << "\nm_bOverIcon = " << m_bOverIcon;
	}
	catch (CFileException* pXcpt)
	{
		pXcpt->ReportError();
		pXcpt->Delete();
	}
}

#endif   // _DEBUG

// end of file
