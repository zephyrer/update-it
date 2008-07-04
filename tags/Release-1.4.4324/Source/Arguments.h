// UpdateIt! application.
// Copyright (c) 2002-2008 by Elijah Zarezky,
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

// Arguments.h - names of the supported command-line arguments

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif   // _MSC_VER

#if !defined(__Arguments_h)
#define __Arguments_h

//////////////////////////////////////////////////////////////////////////////////////////////
// HKEY_CURRENT_USER\Software\Elijah Zarezky\UpdateIt!\Font suppressors

// NOTE that these arguments should be used together to take effect!
// example: -Font-FaceName:Verdana -Font-PointSize:8

#define SZ_ARG_FONT_FACENAME _T("Font-FaceName")
#define SZ_ARG_FONT_POINTSIZE _T("Font-PointSize")

//////////////////////////////////////////////////////////////////////////////////////////////
// arguments related to the "Options" page

#define SZ_ARG_OPTIONS_SOURCE _T("SourceDir-Name")
#define SZ_ARG_OPTIONS_RECURSE _T("SourceDir-Recurce")
#define SZ_ARG_OPTIONS_EXCLUDE _T("SourceDir-Exclude")
#define SZ_ARG_OPTIONS_TARGET _T("TargetDir-Name")
#define SZ_ARG_OPTIONS_CLEANUP _T("TargetDir-CleanupFirst")
#define SZ_ARG_OPTIONS_RECYCLE _T("UseRecycleBin")
#define SZ_ARG_OPTIONS_WRITETIME _T("SourceDir-WriteTime")
#define SZ_ARG_OPTIONS_COMPARE _T("CompareContents")

//////////////////////////////////////////////////////////////////////////////////////////////
// action related arguments

#define SZ_ARG_ACTION_MOVE _T("MoveFiles")
#define SZ_ARG_ACTION_UPLOAD _T("UploadFiles")
#define SZ_ARG_ACTION_ZIP _T("ZipFiles")
#define SZ_ARG_ACTION_SEND _T("SendFiles")

//////////////////////////////////////////////////////////////////////////////////////////////
// FTP related arguments

#define SZ_ARG_FTP_SERVER _T("FTP-Server")
#define SZ_ARG_FTP_PORT _T("FTP-Port")
#define SZ_ARG_FTP_LOGIN _T("FTP-Login")
#define SZ_ARG_FTP_PASSWORD _T("FTP-Password")
#define SZ_ARG_FTP_ROOT _T("FTP-Root")
#define SZ_ARG_FTP_PASSIVE _T("FTP-UsePassiveMode")

//////////////////////////////////////////////////////////////////////////////////////////////
// SMTP related arguments

#define SZ_ARG_SMTP_ENABLE _T("SMTP-Enable")
#define SZ_ARG_SMTP_FROM _T("SMTP-From")
#define SZ_ARG_SMTP_MAILTO _T("SMTP-MailTo")
#define SZ_ARG_SMTP_SUBJ _T("SMTP-Subject")
#define SZ_ARG_SMTP_HOST _T("SMTP-Host")
#define SZ_ARG_SMTP_PORT _T("SMTP-Port")
#define SZ_ARG_SMTP_AUTHENTICATION _T("SMTP-Authentication")
#define SZ_ARG_SMTP_USERNAME _T("SMTP-UserName")
#define SZ_ARG_SMTP_PASSWORD _T("SMTP-Password")
#define SZ_ARG_SMTP_USE_SSL _T("SMTP-UseSSL")
#define SZ_ARG_SMTP_CHARSET _T("SMTP-CharSet")

//////////////////////////////////////////////////////////////////////////////////////////////
// ZIPping related arguments

#define SZ_ARG_ZIP_METHOD _T("ZIP-Method")
#define SZ_ARG_ZIP_COMPRESSION _T("ZIP-Compression")
#define SZ_ARG_ZIP_ENCRYPTION _T("ZIP-Encryption")
#define SZ_ARG_ZIP_PASSWORD _T("ZIP-Password")

//////////////////////////////////////////////////////////////////////////////////////////////
// "master" argument - specifies name of the text file with command-line arguments

// NOTE that if this argument is used and valid all other arguments will be ignored!

#define SZ_ARG_RESPONSE_FILE _T("ResponseFile")

// both relative or absolute pathes are accepted:
// -ArgumentsFile:"..\..\My Settigs\SomeArgiments.txt"
// -ArgumentsFile:OtherArguments.txt
// -ArgumentsFile:"C:\Program Files\PowerGadgets\UpdateIt\MyArguments.txt"

// you can use separate lines for each argument or merge several arguments into the
// one line as shown below:
// -Font-FaceName:Verdana -Font-PointSize:8
// -SourceDir-Name:"C:\Documents and Settings\Elijah Zarezky\My Documents\Zarezky.spb.ru"
// -SourceDir-WriteTime:"23:56:09 03.02.1972"

// lines started with '#' character treated as a comments and ignored

//////////////////////////////////////////////////////////////////////////////////////////////
// another "master" argument - prevents saving of users input in the system registry

#define SZ_ARGV_DONT_SAVE_INPUT _T("DontSaveInput")

#endif   // __Arguments_h

// end of file
