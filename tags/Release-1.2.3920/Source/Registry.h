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

// Registry.h - registry key and value names

#if !defined(__Registry_h)
#define __Registry_h

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Font

#define SZ_REGK_FONT _T("Font")

#define SZ_REGV_FONT_FACENAME _T("FaceName")
#define SZ_REGV_FONT_SIZE _T("PointSize")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Languages

#define SZ_REGK_LANGUAGES _T("Languages")

#define SZ_REGV_LANGUAGES_CURRENT _T("Current")
#define SZ_REGV_LANG_DLL _T("LangDLL")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Options

#define SZ_REGK_OPTIONS _T("Options")

#define SZ_REGV_OPTIONS_SOURCE _T("Source")
#define SZ_REGV_OPTIONS_RECURSE _T("Recurse")
#define SZ_REGV_OPTIONS_EXCLUDE _T("Exclude")
#define SZ_REGV_OPTIONS_TARGET _T("Target")
#define SZ_REGV_OPTIONS_CLEANUP _T("Cleanup")
#define SZ_REGV_OPTIONS_RECYCLE _T("Recycle")
#define SZ_REGV_OPTIONS_COMPARE _T("Compare")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Times

#define SZ_REGK_TIMES _T("Times")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Targets

#define SZ_REGK_TARGETS _T("Targets")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Files

#define SZ_REGK_FILES _T("Files")

#define SZ_REGV_FILES_SHOW_GRID _T("ShowGrid")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Action

#define SZ_REGK_ACTION _T("Action")

#define SZ_REGV_ACTION_ACTION _T("Action")
#define SZ_REGV_ACTION_UPLOAD _T("Upload")
#define SZ_REGV_ACTION_ZIP _T("Zip")
#define SZ_REGV_ACTION_SEND _T("Send")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\FTP

#define SZ_REGK_FTP _T("FTP")

#define SZ_REGV_FTP_SERVER _T("Server")
#define SZ_REGV_FTP_PORT _T("Port")
#define SZ_REGV_FTP_LOGIN _T("Login")
#define SZ_REGV_FTP_PASSWORD _T("Password")
#define SZ_REGV_FTP_ROOT _T("Root")
#define SZ_REGV_FTP_PASSIVE _T("Passive")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\SMTP

#define SZ_REGK_SMTP _T("SMTP")

#define SZ_REGV_SMTP_ENABLE _T("Enable")
#define SZ_REGV_SMTP_FROM _T("from")
#define SZ_REGV_SMTP_MAILTO _T("mailto")
#define SZ_REGV_SMTP_SUBJ _T("subj")
#define SZ_REGV_SMTP_HOST _T("host")
#define SZ_REGV_SMTP_PORT _T("port")
#define SZ_REGV_SMTP_BODY _T("body")
#define SZ_REGV_SMTP_AUTHENTICATION _T("Authentication")
#define SZ_REGV_SMTP_USERNAME _T("UserName")
#define SZ_REGV_SMTP_PASSWORD _T("Password")
#define SZ_REGV_SMTP_USE_SSL _T("UseSSL")
#define SZ_REGV_SMTP_CHARSET _T("charset")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Locale

#define SZ_REGK_LOCALE _T("Locale")

#define SZ_REGV_LOCALE_LC_ALL _T("LC_ALL")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Tips

#define SZ_REGK_TIPS _T("Tips")

#define SZ_REGV_TIPS_INITIAL_DELAY _T("InitialDelay")
#define SZ_REGV_TIPS_AUTO_POP_DELAY _T("AutoPopDelay")
#define SZ_REGV_TIPS_MAX_WIDTH _T("MaxWidth")
#define SZ_REGV_TIPS_BK_COLOR _T("BkColor")
#define SZ_REGV_TIPS_TEXT_COLOR _T("TextColor")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\ZIP

#define SZ_REGK_ZIP _T("ZIP")

#define SZ_REGV_ZIP_COMPR_LEVEL _T("ComprLevel")
#define SZ_REGV_ZIP_ENCR_METHOD _T("EncrMethod")
#define SZ_REGV_ZIP_PASSWORD _T("Password")

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\FontSubstitutes

#define SZ_REGK_FONT_SUBSTITUTES _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\FontSubstitutes")

#define SZ_REGV_MS_SHELL_DLG _T("MS Shell Dlg")
#define SZ_REGV_MS_SHELL_DLG_2 _T("MS Shell Dlg 2")

#endif   // __Registry_h

// end of file
