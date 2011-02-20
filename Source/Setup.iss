;; UpdateIt! application.
;; Copyright (c) 2002-2011 by Elijah Zarezky,
;; All rights reserved.

;; Licensed under the Apache License, Version 2.0 (the "License");
;; you may not use this file except in compliance with the License.
;; You may obtain a copy of the License at
;;
;; http://www.apache.org/licenses/LICENSE-2.0
;;
;; Unless required by applicable law or agreed to in writing, software
;; distributed under the License is distributed on an "AS IS" BASIS,
;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;; See the License for the specific language governing permissions and
;; limitations under the License.

;; Setup.iss - setup script for Inno Setup compiler
;; NOTE that this file is generated automatically by NAnt.exe

[Setup]
AppName=UpdateIt!
AppVerName=UpdateIt! 1.6.5716
AppID={{78461E4F-C4AD-4488-97F7-773CCA325839}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://zarezky.spb.ru/
AppSupportURL=http://zarezky.spb.ru/projects/update_it.html
AppUpdatesURL=http://zarezky.spb.ru/projects/update_it.html
AppVersion=1.6.5716
AppCopyright=Copyright © 2002–2011 by Elijah Zarezky
DefaultDirName={pf}\Elijah Zarezky\UpdateIt
DefaultGroupName=Elijah Zarezky\UpdateIt
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=UpdateIt-1.6.5716.dev-setup
VersionInfoVersion=1.6.5716.399
VersionInfoProductName=Power Gadgets
VersionInfoProductVersion=1.6
MinVersion=0,5.0.2195
WizardImageFile=compiler:WizModernImage-IS.bmp
WizardSmallImageFile=compiler:WizModernSmallImage-IS.bmp
LicenseFile=ApacheLicense.rtf
LanguageDetectionMethod=none
ShowLanguageDialog=yes
AppMutex=UpdateIt.Instance.03E2CBEF-1504-45d8-8BD2-79719D32E29F

[Languages]
;; English
Name: "en"; MessagesFile: "compiler:Default.isl"
;; Russian
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"; LicenseFile: ".\ApacheLicense.rtf"

[LangOptions]
en.LanguageCodePage=1252
ru.LanguageCodePage=1251
DialogFontName=MS Shell Dlg 2
DialogFontSize=8
en.LanguageName=English
ru.LanguageName=Russian

[Messages]
SelectLanguageTitle=Language Selection
SelectLanguageLabel=Please select UpdateIt! language:

[Code]
//  Code Page default values
const
	CP_ACP = 0;
	CP_OEMCP = 1;
	CP_MACCP = 2;
	CP_THREAD_ACP = 3;
	CP_SYMBOL = 42;
	CP_UTF7 = 65000;
	CP_UTF8 = 65001;

// MBCS and Unicode translation flags
const
	MB_PRECOMPOSED = $0001;
	MB_COMPOSITE = $0002;
	MB_USEGLYPHCHARS = $0004;
	MB_ERR_INVALID_CHARS = $0008;

	WC_COMPOSITECHECK = $0200;
	WC_DISCARDNS = $0010;
	WC_SEPCHARS = $0020;
	WC_DEFAULTCHAR = $0040;
	WC_ERR_INVALID_CHARS = $0080;
	WC_NO_BEST_FIT_CHARS = $0400;

	DefFlagsMB = MB_PRECOMPOSED;
	DefFlagsWC = WC_COMPOSITECHECK or WC_DISCARDNS or WC_SEPCHARS or WC_DEFAULTCHAR;

function MultiByteToWideChar(CodePage, Flags: Integer; SrcStr: AnsiString; SrcLen: Integer; DestStr: String; DestLen: Integer): Integer;
external 'MultiByteToWideChar@kernel32.dll stdcall';
function WideCharToMultiByte(CodePage, Flags: Integer; SrcStr: String; SrcLen: Integer; DestStr: AnsiString; DestLen: Integer; DefChar: Integer; DefCharUsed: Integer): Integer;
external 'WideCharToMultiByte@kernel32.dll stdcall';

function AnsiStringToString(const SrcStr: AnsiString; CodePage: Integer): String;
var
	WideLen: Integer;
begin
	if (SrcStr <> '') then
	begin
		WideLen := MultiByteToWideChar(CodePage, DefFlagsMB, SrcStr, -1, Result, 0);
		SetLength(Result, WideLen - 1);
		MultiByteToWideChar(CodePage, DefFlagsMB, SrcStr, -1, Result, WideLen - 1);
	end
	else begin
		Result := '';
	end;
end;

function StringToAnsiString(const SrcStr: String; CodePage: Integer): AnsiString;
var
	AnsiLen: Integer;
begin
	if (SrcStr <> '') then
	begin
		AnsiLen := WideCharToMultiByte(CodePage, DefFlagsWC, SrcStr, -1, Result, 0, 0, 0);
		SetLength(Result, AnsiLen - 1);
		WideCharToMultiByte(CodePage, DefFlagsWC, SrcStr, -1, Result, AnsiLen - 1, 0, 0);
	end
	else begin
		Result := '';
	end;
end;

function IsWinSxS(): Boolean;
var
	osVersion: TWindowsVersion;
begin
	GetWindowsVersionEx(osVersion);
	Result := ((osVersion.Major = 5) and (osVersion.Minor >= 1)) or (osVersion.Major > 5);
end;

const
	MagicStr = '{6DF4C042-C237-46b2-A205-C41DAAA4A0F5}';

procedure AdjustAfxConfig();
var
	AfxConfigPath: String;
	AfxConfigXML: AnsiString;
	WideXML: String;
begin
	if (IsWinSxS()) then
	begin
		AfxConfigPath := ExpandConstant(CurrentFileName());
		if (LoadStringFromFile(AfxConfigPath, AfxConfigXML)) then
		begin
			WideXML := AnsiStringToString(AfxConfigXML, CP_ACP);
			StringChangeEx(WideXML, MagicStr, ExpandConstant('{app}'), True);
			SaveStringToFile(AfxConfigPath, StringToAnsiString(WideXML, CP_ACP), False);
		end;
	end;
end;

function NextButtonClick(CurPageID: Integer): Boolean;
var
	hRootHive: Integer;
	LangsKeyName: String;
begin
	if (CurPageID = wpReady) then
	begin
		hRootHive := HKEY_CURRENT_USER;
		LangsKeyName := 'Software\Elijah Zarezky\UpdateIt!\Languages';

		RegWriteStringValue(hRootHive, LangsKeyName + '\en', '', '.\mfc90enu.dll');
		RegWriteStringValue(hRootHive, LangsKeyName + '\en', 'LangDLL', '.\English_USA.1252.dll');
		RegWriteStringValue(hRootHive, LangsKeyName + '\ru', '', '.\mfc90rus.dll');
		RegWriteStringValue(hRootHive, LangsKeyName + '\ru', 'LangDLL', '.\Russian_Russia.1251.dll');
		
		RegWriteStringValue(hRootHive, LangsKeyName, '', 'en;ru');
		RegWriteStringValue(hRootHive, LangsKeyName, 'Current', ActiveLanguage());
	end;

	Result := True;
end;

[InstallDelete]
;; from 1.4 release
Type: files; Name: "{app}\msvcr71.dll"
Type: files; Name: "{app}\msvcp71.dll"
Type: files; Name: "{app}\mfc71.dll"
Type: files; Name: "{app}\mfc71enu.dll"
Type: files; Name: "{app}\mfc71rus.dll"
Type: filesandordirs; Name: "{app}\Languages"
Type: files; Name: "{app}\UpdateIt.chm"
;; from 1.5 pre-release
Type: filesandordirs; Name: "{app}\Microsoft.VC90.CRT"
Type: filesandordirs; Name: "{app}\Microsoft.VC90.MFC"
Type: files; Name: "{app}\Microsoft.VC90.MFC.manifest"
Type: files; Name: "{app}\mfc90.dll"
Type: files; Name: "{app}\mfcm90.dll"
;; from pre-1.6 release
Type: files; Name: "{group}\UpdateIt! on the Web.lnk"
Type: files; Name: "{group}\UpdateIt! в Интернет.lnk"
Type: files; Name: "{app}\UpdateIt.url"

[Files]
;; core application files
Source: "..\Output.2008\x86\Release\Unicode\UpdateIt.exe"; DestDir: "{app}"
Source: "..\CrashReporter\Output.2008\x86\Release\Unicode\CrashReporter.exe"; DestDir: "{app}"
Source: "..\Detours\Bin\detoured.dll"; DestDir: "{app}"
Source: "..\HTML\UpdateIt.0409.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\HTML\UpdateIt.0419.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\ApacheLicense.rtf"; DestDir: "{app}"; Flags: ignoreversion

;; application localizations
Source: "..\Languages\English_USA.1252\Output.2008\x86\Release\Unicode\English_USA.1252.dll"; DestDir: "{app}"
Source: "..\Languages\Russian_Russia.1251\Output.2008\x86\Release\Unicode\Russian_Russia.1251.dll"; DestDir: "{app}"

;; CRT redistributables
Source: "..\Redist\Microsoft.VC90.CRT\msvcr90.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.CRT\msvcp90.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.CRT\msvcm90.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.CRT\Microsoft.VC90.PrivateCRT.manifest"; DestDir: "{app}"; MinVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.CRT\Microsoft.VC90.CRT.manifest"; DestDir: "{app}"; MinVersion: 0,5.01.2600; Flags: ignoreversion

;; MFC library redistributables
Source: "..\Redist\Microsoft.VC90.MFC\mfc90u.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFC\mfc90u.dll.1000.config"; DestDir: "{app}"; MinVersion: 0,5.01.2600; AfterInstall: AdjustAfxConfig; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.MFC\mfcm90u.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFC\Microsoft.VC90.PrivateMFC.manifest"; DestDir: "{app}"; MinVersion: 0,5.01.2600; Flags: ignoreversion

;; MFC library localizations
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90enu.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90rus.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFCLOC\Microsoft.VC90.MFCLOC.manifest"; DestDir: "{app}"; MinVersion: 0,5.01.2600; Flags: ignoreversion

;; OpenSSL redistributables
Source: "..\OpenSSL\redist\ssleay32.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\libeay32.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\4758cca.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\aep.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\atalla.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\capi.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\chil.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\cswift.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\gmp.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\gost.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\nuron.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\padlock.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\sureware.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\ubsec.dll"; DestDir: "{app}"

;; DbgHelp redistributables
Source: "..\Redist\dbghelp.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; WorkingDir: "{app}"
Name: "{group}\Documentation (English)"; Filename: "{app}\UpdateIt.0409.chm"; Languages: en
Name: "{group}\Документация (на английском)"; Filename: "{app}\UpdateIt.0409.chm"; Languages: ru
Name: "{group}\Documentation (Russian)"; Filename: "{app}\UpdateIt.0419.chm"; Languages: en
Name: "{group}\Документация (на русском)"; Filename: "{app}\UpdateIt.0419.chm"; Languages: ru
Name: "{group}\License Agreement"; Filename: "{app}\ApacheLicense.rtf"; Languages: en
Name: "{group}\Лицензионное соглашение"; Filename: "{app}\ApacheLicense.rtf"; Languages: ru
Name: "{group}\Uninstall UpdateIt!"; Filename: "{uninstallexe}"; Languages: en
Name: "{group}\Удалить UpdateIt!"; Filename: "{uninstallexe}"; Languages: ru
Name: "{userdesktop}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; WorkingDir: "{app}"; Tasks: quicklaunchicon

[INI]
Filename: "{group}\UpdateIt! on the Web.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://zarezky.spb.ru/projects/update-it.en.html"; Languages: en
Filename: "{group}\UpdateIt! on the Web.url"; Section: "InternetShortcut"; Key: "IconFile"; String: ""; Languages: en
Filename: "{group}\UpdateIt! on the Web.url"; Section: "InternetShortcut"; Key: "IconIndex"; String: "0"; Languages: en
Filename: "{group}\UpdateIt! on the Web.url"; Section: "InternetShortcut"; Key: "Hotkey"; String: "0"; Languages: en
Filename: "{group}\UpdateIt! on the Web.url"; Section: "InternetShortcut"; Key: "WorkingDirectory"; String: ""; Languages: en
Filename: "{group}\UpdateIt! on the Web.url"; Section: "InternetShortcut"; Key: "ShowCommand"; String: "1"; Languages: en
Filename: "{group}\UpdateIt! в Интернет.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://zarezky.spb.ru/projects/update-it.html"; Languages: ru
Filename: "{group}\UpdateIt! в Интернет.url"; Section: "InternetShortcut"; Key: "IconFile"; String: ""; Languages: ru
Filename: "{group}\UpdateIt! в Интернет.url"; Section: "InternetShortcut"; Key: "IconIndex"; String: "0"; Languages: ru
Filename: "{group}\UpdateIt! в Интернет.url"; Section: "InternetShortcut"; Key: "Hotkey"; String: "0"; Languages: ru
Filename: "{group}\UpdateIt! в Интернет.url"; Section: "InternetShortcut"; Key: "WorkingDirectory"; String: ""; Languages: ru
Filename: "{group}\UpdateIt! в Интернет.url"; Section: "InternetShortcut"; Key: "ShowCommand"; String: "1"; Languages: ru
Filename: "{userappdata}\Elijah Zarezky\UpdateIt\CrashReporter.ini"; Section: "SMTP"; Key: "from"; String: "anonymous@unknown.org"; Flags: createkeyifdoesntexist
Filename: "{userappdata}\Elijah Zarezky\UpdateIt\CrashReporter.ini"; Section: "SMTP"; Key: "host"; String: "mail.unknown.org"; Flags: createkeyifdoesntexist
Filename: "{userappdata}\Elijah Zarezky\UpdateIt\CrashReporter.ini"; Section: "SMTP"; Key: "port"; String: "25"; Flags: createkeyifdoesntexist
Filename: "{userappdata}\Elijah Zarezky\UpdateIt\CrashReporter.ini"; Section: "SMTP"; Key: "charset"; String: "windows-1252"; Flags: createkeyifdoesntexist; Languages: en
Filename: "{userappdata}\Elijah Zarezky\UpdateIt\CrashReporter.ini"; Section: "SMTP"; Key: "charset"; String: "windows-1251"; Flags: createkeyifdoesntexist; Languages: ru

[Registry]
Root: HKCU; Subkey: "Software\Elijah Zarezky"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!"; ValueType: dword; ValueName: "FirstLaunch"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Locale"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Locale"; ValueType: string; ValueName: "LC_ALL"; ValueData: "English_USA.1252"; Flags: createvalueifdoesntexist; Languages: en
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Locale"; ValueType: string; ValueName: "LC_ALL"; ValueData: "Russian_Russia.1251"; Flags: createvalueifdoesntexist; Languages: ru
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Options"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Options"; ValueType: string; ValueName: "Exclude"; ValueData: "*\.svn\*.*,*.obj,*.pch,*.sbr,*.res,*.tlb,*.bsc,*.idb,*.pdb,*.plg,*.aps,*.opt,*.ncb,*.exe,*.dll,*.ocx,*.lib"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\SMTP"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\SMTP"; ValueType: string; ValueName: "charset"; ValueData: "windows-1252"; Flags: createvalueifdoesntexist; Languages: en
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\SMTP"; ValueType: string; ValueName: "charset"; ValueData: "windows-1251"; Flags: createvalueifdoesntexist; Languages: ru
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\SMTP"; ValueType: dword; ValueName: "Enable"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Catchpit"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Catchpit"; ValueType: dword; ValueName: "TortoiseShell.dll"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Catchpit"; ValueType: dword; ValueName: "TortoiseOverlays.dll"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Catchpit"; ValueType: dword; ValueName: "TortoiseSVN.dll"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Catchpit"; ValueType: dword; ValueName: "TortoiseStub.dll"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Catchpit"; ValueType: dword; ValueName: "PhoneBrowser.dll"; ValueData: "1"; Flags: createvalueifdoesntexist

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; Flags: unchecked
Name: quicklaunchicon; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Run]
Filename: "{app}\UpdateIt.exe"; Description: "Launch UpdateIt!"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{group}\UpdateIt! on the Web.url"
Type: files; Name: "{group}\UpdateIt! в Интернет.url"
Type: files; Name: "{userappdata}\Elijah Zarezky\UpdateIt\CrashReporter.ini"

;; end of file
