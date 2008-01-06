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

;; SetupUniversal.iss - setup script for Inno Setup compiler
;; NOTE that this file is generated automatically by NAnt.exe

[Setup]
AppName=UpdateIt!
AppVerName=UpdateIt! 1.3.4134
AppID={{78461E4F-C4AD-4488-97F7-773CCA325839}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://zarezky.spb.ru/
AppSupportURL=http://zarezky.spb.ru/projects/update_it.html
AppUpdatesURL=http://zarezky.spb.ru/projects/update_it.html
AppVersion=1.3.4134
DefaultDirName={pf}\PowerGadgets\UpdateIt
DefaultGroupName=PowerGadgets\UpdateIt
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=UpdateIt-1.3.4134-setup-universal
VersionInfoVersion=1.3.4134
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
function HasLangEn(): Boolean;
begin
	Result := (IsComponentSelected('mui')) or (ActiveLanguage() = 'en');
end;

function HasLangRu(): Boolean;
begin
	Result := (IsComponentSelected('mui')) or (ActiveLanguage() = 'ru');
end;

procedure CurPageChanged(CurPageID: Integer);
var
  OptionsPage: TWizardPage;
  PageSurface: TNewNotebookPage;
  SetupTypesCombo: TComboBox;
begin
  if (CurPageID = wpSelectComponents) and (ActiveLanguage() = 'ru') then
  begin
    OptionsPage := PageFromID(CurPageID);
    PageSurface := OptionsPage.Surface;
    SetupTypesCombo := TComboBox(PageSurface.Controls[2]);
    if (SetupTypesCombo <> nil) then
    begin
      if (SetupTypesCombo.Items[3] <> 'Выборочная установка') then
      begin
        SetupTypesCombo.Items[3] := 'Выборочная установка';
      end;
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
		if (HasLangEn()) then
		begin
			RegWriteStringValue(hRootHive, LangsKeyName + '\en', '', ExpandConstant('{app}\mfc71enu.dll'));
			RegWriteStringValue(hRootHive, LangsKeyName + '\en', 'LangDLL', ExpandConstant('{app}\Languages\English_USA.1252.dll'));
		end;
		if (HasLangRu) then
		begin
			RegWriteStringValue(hRootHive, LangsKeyName + '\ru', '', ExpandConstant('{app}\mfc71rus.dll'));
			RegWriteStringValue(hRootHive, LangsKeyName + '\ru', 'LangDLL', ExpandConstant('{app}\Languages\Russian_Russia.1251.dll'));
		end;
		if (not IsComponentSelected('mui')) then
		begin
			RegWriteStringValue(hRootHive, LangsKeyName, '', ActiveLanguage());
		end
		else begin
			RegWriteStringValue(hRootHive, LangsKeyName, '', 'en;ru');
		end;
		RegWriteStringValue(hRootHive, LangsKeyName, 'Current', ActiveLanguage());
	end;
	Result := True;
end;

[Types]
Name: "typical"; Description: "Typical Installation"; Languages: en
Name: "typical"; Description: "Стандартная установка"; Languages: ru
Name: "compact"; Description: "Compact Installation"; Languages: en
Name: "compact"; Description: "Компактная установка"; Languages: ru
Name: "full"; Description: "Full Installation"; Languages: en
Name: "full"; Description: "Полная установка"; Languages: ru
Name: "custom"; Description: "Custom Installation"; Flags: iscustom

[Components]
Name: "core"; Description: "UpdateIt! Core Files"; Types: compact typical full custom; Flags: fixed; Languages: en
Name: "core"; Description: "Исполняемые файлы UpdateIt!"; Types: compact typical full custom; Flags: fixed; Languages: ru
Name: "runtimes"; Description: "Application Runtimes"; Types: typical full custom; Languages: en
Name: "runtimes"; Description: "Библиотеки времени выполнения"; Types: typical full custom; Languages: ru
Name: "mui"; Description: "MUI Support"; Types: typical full custom; Languages: en
Name: "mui"; Description: "Поддержка многоязыковго интефейса"; Types: typical full custom; Languages: ru
Name: "sources"; Description: "Source Code"; Types: full custom; Languages: en
Name: "sources"; Description: "Исходные тексты"; Types: full custom; Languages: ru

[Files]
Source: "..\Output\x86\Release\MBCS\UpdateIt.exe"; DestDir: "{app}"; Components: core
Source: "..\HTML\UpdateIt.chm"; DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: ".\ApacheLicense.rtf"; DestDir: "{app}"; Components: core; Flags: ignoreversion

Source: "..\Languages\English_USA.1252\Output\x86\Release\MBCS\English_USA.1252.dll"; DestDir: "{app}\Languages"; Components: core; Check: HasLangEn
Source: "..\Redist\mfc71enu.dll"; DestDir: "{app}"; Components: core; Check: HasLangEn; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Output\x86\Release\MBCS\Russian_Russia.1251.dll"; DestDir: "{app}\Languages"; Components: core; Check: HasLangRu
Source: "..\Redist\mfc71rus.dll"; DestDir: "{app}"; Components: core; Check: HasLangRu; Flags: ignoreversion

Source: "..\..\Repository\OpenSSL\redist\ssleay32.dll"; DestDir: "{app}"; Components: core
Source: "..\..\Repository\OpenSSL\redist\libeay32.dll"; DestDir: "{app}"; Components: core

Source: "..\Redist\mfc71.dll"; DestDir: "{app}"; Components: runtimes; Flags: ignoreversion
Source: "..\Redist\msvcr71.dll"; DestDir: "{app}"; Components: runtimes; Flags: ignoreversion
Source: "..\Redist\msvcp71.dll"; DestDir: "{app}"; Components: runtimes; Flags: ignoreversion

Source: "..\..\Repository\AfxGadgets\AfxGadgets71.vcproj"; DestDir: "{app}\Sources\Repository\AfxGadgets"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\AfxGadgets\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\AfxGadgets\Source"; Components: sources; Flags: ignoreversion

Source: "..\..\Repository\CodeProject\CodeProject71.vcproj"; DestDir: "{app}\Sources\Repository\CodeProject"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\CodeProject\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\CodeProject\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\CodeProject\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\CodeProject\Source"; Components: sources; Flags: ignoreversion

Source: "..\..\Repository\Naughter\Naughter71.vcproj"; DestDir: "{app}\Sources\Repository\Naughter"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Naughter\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Naughter\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Naughter\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\Naughter\Source"; Components: sources; Flags: ignoreversion recursesubdirs

Source: "..\..\Repository\OpenSSL\include\openssl\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\OpenSSL\include\openssl"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\lib\VC\ssleay32MDd.lib"; DestDir: "{app}\Sources\Repository\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\lib\VC\libeay32MDd.lib"; DestDir: "{app}\Sources\Repository\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\lib\VC\ssleay32MD.lib"; DestDir: "{app}\Sources\Repository\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\lib\VC\libeay32MD.lib"; DestDir: "{app}\Sources\Repository\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion

Source: "..\..\Repository\ZipArchive\ZipArchive71.vcproj"; DestDir: "{app}\Sources\Repository\ZipArchive"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\ZipArchive\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\MiscText\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\ZipArchive\MiscText"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\ZipArchive\Source"; Components: sources; Flags: ignoreversion

Source: "..\..\Repository\zlib\zlib71.vcproj"; DestDir: "{app}\Sources\Repository\zlib"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\zlib\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\zlib\Source"; Components: sources; Flags: ignoreversion

Source: "..\Languages\English_USA.1252\English_USA.1252.vcproj"; DestDir: "{app}\Sources\UpddateIt\Languages\English_USA.1252"; Components: sources; Flags: ignoreversion
Source: "..\Languages\English_USA.1252\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpddateIt\Languages\English_USA.1252\Source"; Components: sources; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Russian_Russia.1251.vcproj"; DestDir: "{app}\Sources\UpddateIt\Languages\Russian_Russia.1251"; Components: sources; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpddateIt\Languages\Russian_Russia.1251\Source"; Components: sources; Flags: ignoreversion

Source: "..\UpdateIt71.vcproj"; DestDir: "{app}\Sources\UpddateIt"; Components: sources; Flags: ignoreversion
Source: "..\UpdateIt71.sln"; DestDir: "{app}\Sources\UpddateIt"; Components: sources; Flags: ignoreversion
Source: "..\HTML\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpddateIt\HTML"; Components: sources; Flags: ignoreversion
Source: "..\HTML\images\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpddateIt\HTML\images"; Components: sources; Flags: ignoreversion
Source: "..\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpddateIt\Source"; Components: sources; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"
Name: "{group}\UpdateIt! Documentation"; Filename: "{app}\UpdateIt.chm"
Name: "{group}\UpdateIt! License"; Filename: "{app}\ApacheLicense.rtf"
Name: "{group}\UpdateIt! on the Web"; Filename: "{app}\UpdateIt.url"
Name: "{group}\Build UpdateIt!"; FileName: "{app}\Sources\UpddateIt\UpdateIt71.sln"; Components: sources
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
