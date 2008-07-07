;; UpdateIt! application.
;; Copyright (c) 2002-2008 by Elijah Zarezky,
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
AppVerName=UpdateIt! 1.4.4325
AppID={{78461E4F-C4AD-4488-97F7-773CCA325839}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://zarezky.spb.ru/
AppSupportURL=http://zarezky.spb.ru/projects/update_it.html
AppUpdatesURL=http://zarezky.spb.ru/projects/update_it.html
AppVersion=1.4.4325
DefaultDirName={pf}\PowerGadgets\UpdateIt
DefaultGroupName=PowerGadgets\UpdateIt
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=UpdateIt-1.4.4325-setup
VersionInfoVersion=1.4.4325
MinVersion=4.1.2222,5.0.2195
WizardImageFile=compiler:WizModernImage-IS.bmp
WizardSmallImageFile=compiler:WizModernSmallImage-IS.bmp
LicenseFile=ApacheLicense.rtf
LanguageDetectionMethod=none
ShowLanguageDialog=yes

[Languages]
; English
Name: "en"; MessagesFile: "compiler:Default.isl"
; Russian
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"; LicenseFile: ".\ApacheLicense.rtf"

[LangOptions]
LanguageCodePage=0
DialogFontName=Tahoma
DialogFontSize=8
en.LanguageName=English
ru.LanguageName=Russian

[Messages]
SelectLanguageTitle=Language Selection
SelectLanguageLabel=Please select UpdateIt! language:

[Code]
function IsWinSxS(): Boolean;
var
	osVersion: TWindowsVersion;
begin
	GetWindowsVersionEx(osVersion);
	Result := ((osVersion.Major = 5) and (osVersion.Minor >= 1)) or (osVersion.Major > 5);
end;

function NextButtonClick(CurPageID: Integer): Boolean;
var
	AfxLangPath: String;
	hRootHive: Integer;
	LangsKeyName: String;
begin
	if (CurPageID = wpReady) then
	begin
		if (IsWinSxS()) then
		begin
			AfxLangPath := ExpandConstant('{app}\Microsoft.VC90.MFC\Microsoft.VC90.MFCLOC');
		end
		else begin
			AfxLangPath := ExpandConstant('{app}');
		end;

		hRootHive := HKEY_CURRENT_USER;
		LangsKeyName := 'Software\Elijah Zarezky\UpdateIt!\Languages';

		RegWriteStringValue(hRootHive, LangsKeyName + '\en', '', AfxLangPath + '\mfc90enu.dll');
		RegWriteStringValue(hRootHive, LangsKeyName + '\en', 'LangDLL', ExpandConstant('{app}\English_USA.1252.dll'));
		RegWriteStringValue(hRootHive, LangsKeyName + '\ru', '', AfxLangPath + '\mfc90rus.dll');
		RegWriteStringValue(hRootHive, LangsKeyName + '\ru', 'LangDLL', ExpandConstant('{app}\Russian_Russia.1251.dll'));
		
		RegWriteStringValue(hRootHive, LangsKeyName, '', 'en;ru');
		RegWriteStringValue(hRootHive, LangsKeyName, 'Current', ActiveLanguage());
	end;
	
	Result := True;
end;

[Files]
Source: "..\Output.2008\x86\Release\MBCS\UpdateIt.exe"; DestDir: "{app}"
Source: "..\HTML\UpdateIt.0409.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\HTML\UpdateIt.0419.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\ApacheLicense.rtf"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\OpenSSL\redist\ssleay32.dll"; DestDir: "{app}"
Source: "..\OpenSSL\redist\libeay32.dll"; DestDir: "{app}"

Source: "..\Languages\English_USA.1252\Output.2008\x86\Release\MBCS\English_USA.1252.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90enu.dll"; DestDir: "{app}"; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90enu.dll"; DestDir: "{app}\Microsoft.VC90.MFC\Microsoft.VC90.MFCLOC"; MinVersion: 0,5.01.2600

Source: "..\Languages\Russian_Russia.1251\Output.2008\x86\Release\MBCS\Russian_Russia.1251.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90rus.dll"; DestDir: "{app}"; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90rus.dll"; DestDir: "{app}\Microsoft.VC90.MFC\Microsoft.VC90.MFCLOC"; MinVersion: 0,5.01.2600

Source: "..\Redist\Microsoft.VC90.MFCLOC\Microsoft.VC90.MFCLOC.manifest"; DestDir: "{app}\Microsoft.VC90.MFC\Microsoft.VC90.MFCLOC"; MinVersion: 0,5.01.2600

Source: "..\Redist\Microsoft.VC90.CRT\msvcr90.dll"; DestDir: "{app}"; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.CRT\msvcp90.dll"; DestDir: "{app}"; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.CRT\msvcm90.dll"; DestDir: "{app}"; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion

Source: "..\Redist\Microsoft.VC90.CRT\msvcr90.dll"; DestDir: "{app}\Microsoft.VC90.CRT"; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.CRT\msvcp90.dll"; DestDir: "{app}\Microsoft.VC90.CRT"; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.CRT\msvcm90.dll"; DestDir: "{app}\Microsoft.VC90.CRT"; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.CRT\Microsoft.VC90.CRT.manifest"; DestDir: "{app}\Microsoft.VC90.CRT"; MinVersion: 0,5.01.2600

Source: "..\Redist\Microsoft.VC90.MFC\mfc90.dll"; DestDir: "{app}"; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.MFC\mfcm90.dll"; DestDir: "{app}"; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion

Source: "..\Redist\Microsoft.VC90.MFC\mfc90.dll"; DestDir: "{app}\Microsoft.VC90.MFC"; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.MFC\mfcm90.dll"; DestDir: "{app}\Microsoft.VC90.MFC"; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.MFC\Microsoft.VC90.MFC.manifest"; DestDir: "{app}\Microsoft.VC90.MFC"; MinVersion: 0,5.01.2600

[Icons]
Name: "{group}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"
Name: "{group}\Documentation (English)"; Filename: "{app}\UpdateIt.0409.chm"; Languages: en
Name: "{group}\Документация (на английском)"; Filename: "{app}\UpdateIt.0409.chm"; Languages: ru
Name: "{group}\Documentation (Russian)"; Filename: "{app}\UpdateIt.0419.chm"; Languages: en
Name: "{group}\Документация (на русском)"; Filename: "{app}\UpdateIt.0419.chm"; Languages: ru
Name: "{group}\License Agreement"; Filename: "{app}\ApacheLicense.rtf"; Languages: en
Name: "{group}\Лицензионное соглашение"; Filename: "{app}\ApacheLicense.rtf"; Languages: ru
Name: "{group}\UpdateIt! on the Web"; Filename: "{app}\UpdateIt.url"; Languages: en
Name: "{group}\UpdateIt! в Интернет"; Filename: "{app}\UpdateIt.url"; Languages: ru
Name: "{group}\Uninstall UpdateIt!"; Filename: "{uninstallexe}"; Languages: en
Name: "{group}\Удалить UpdateIt!"; Filename: "{uninstallexe}"; Languages: ru
Name: "{userdesktop}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; Tasks: quicklaunchicon

[INI]
Filename: "{app}\UpdateIt.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://zarezky.spb.ru/projects/update_it.html"

[Registry]
Root: HKCU; Subkey: "Software\Elijah Zarezky"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Locale"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Locale"; ValueType: string; ValueName: "LC_ALL"; ValueData: "Russian_Russia.1251"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Options"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\Options"; ValueType: string; ValueName: "Exclude"; ValueData: "*\.svn\*.*,*.obj,*.pch,*.sbr,*.res,*.tlb,*.bsc,*.idb,*.pdb,*.plg,*.aps,*.opt,*.ncb,*.exe,*.dll,*.ocx,*.lib"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\SMTP"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\SMTP"; ValueType: string; ValueName: "charset"; ValueData: """windows-1251"""; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\Elijah Zarezky\UpdateIt!\SMTP"; ValueType: dword; ValueName: "Enable"; ValueData: "1"; Flags: createvalueifdoesntexist

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; Flags: unchecked
Name: quicklaunchicon; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Run]
Filename: "{app}\UpdateIt.exe"; Description: "Launch UpdateIt!"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\UpdateIt.url"

;; end of file
