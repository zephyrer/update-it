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
AppVerName=UpdateIt! 1.4.4324
AppID={{78461E4F-C4AD-4488-97F7-773CCA325839}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://zarezky.spb.ru/
AppSupportURL=http://zarezky.spb.ru/projects/update_it.html
AppUpdatesURL=http://zarezky.spb.ru/projects/update_it.html
AppVersion=1.4.4324
DefaultDirName={pf}\PowerGadgets\UpdateIt
DefaultGroupName=PowerGadgets\UpdateIt
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=UpdateIt-1.4.4324-setup
VersionInfoVersion=1.4.4324
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
function NextButtonClick(CurPageID: Integer): Boolean;
var
	hRootHive: Integer;
	LangsKeyName: String;
begin
	if (CurPageID = wpReady) then
	begin
		hRootHive := HKEY_CURRENT_USER;
		LangsKeyName := 'Software\Elijah Zarezky\UpdateIt!\Languages';
		RegWriteStringValue(hRootHive, LangsKeyName + '\en', '', ExpandConstant('{app}\mfc71enu.dll'));
		RegWriteStringValue(hRootHive, LangsKeyName + '\en', 'LangDLL', ExpandConstant('{app}\Languages\English_USA.1252.dll'));
		RegWriteStringValue(hRootHive, LangsKeyName + '\ru', '', ExpandConstant('{app}\mfc71rus.dll'));
		RegWriteStringValue(hRootHive, LangsKeyName + '\ru', 'LangDLL', ExpandConstant('{app}\Languages\Russian_Russia.1251.dll'));
		RegWriteStringValue(hRootHive, LangsKeyName, '', 'en;ru');
		RegWriteStringValue(hRootHive, LangsKeyName, 'Current', ActiveLanguage());
	end;
	Result := True;
end;

[Files]
Source: "..\Output.2003\x86\Release\MBCS\UpdateIt.exe"; DestDir: "{app}"
Source: "..\HTML\UpdateIt.0409.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\HTML\UpdateIt.0419.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\ApacheLicense.rtf"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\..\Repository\OpenSSL\redist\ssleay32.dll"; DestDir: "{app}"
Source: "..\..\Repository\OpenSSL\redist\libeay32.dll"; DestDir: "{app}"
Source: "..\Redist\mfc71.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Redist\msvcr71.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Redist\msvcp71.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Languages\English_USA.1252\Output.2003\x86\Release\MBCS\English_USA.1252.dll"; DestDir: "{app}\Languages"
Source: "..\Redist\mfc71enu.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Output.2003\x86\Release\MBCS\Russian_Russia.1251.dll"; DestDir: "{app}\Languages"
Source: "..\Redist\mfc71rus.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"
Name: "{group}\UpdateIt! Documentation (English)"; Filename: "{app}\UpdateIt.0409.chm"
Name: "{group}\UpdateIt! Documentation (Russian)"; Filename: "{app}\UpdateIt.0419.chm"
Name: "{group}\UpdateIt! License"; Filename: "{app}\ApacheLicense.rtf"
Name: "{group}\UpdateIt! on the Web"; Filename: "{app}\UpdateIt.url"
Name: "{group}\Uninstall UpdateIt!"; Filename: "{uninstallexe}"
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
