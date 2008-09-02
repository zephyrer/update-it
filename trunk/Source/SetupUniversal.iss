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
AppVerName=UpdateIt! 1.4.4386
AppID={{78461E4F-C4AD-4488-97F7-773CCA325839}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://zarezky.spb.ru/
AppSupportURL=http://zarezky.spb.ru/projects/update_it.html
AppUpdatesURL=http://zarezky.spb.ru/projects/update_it.html
AppVersion=1.4.4386
DefaultDirName={pf}\PowerGadgets\UpdateIt
DefaultGroupName=PowerGadgets\UpdateIt
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=UpdateIt-1.4.4386-setup-universal
VersionInfoVersion=1.4.4386
MinVersion=4.1.2222,5.0.2195
WizardImageFile=compiler:WizModernImage-IS.bmp
WizardSmallImageFile=compiler:WizModernSmallImage-IS.bmp
LicenseFile=ApacheLicense.rtf
LanguageDetectionMethod=none
ShowLanguageDialog=yes
AppMutex=UpdateIt.Instance.03E2CBEF-1504-45d8-8BD2-79719D32E29F

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

function IsWinSxS(): Boolean;
var
	osVersion: TWindowsVersion;
begin
	GetWindowsVersionEx(osVersion);
	Result := ((osVersion.Major = 5) and (osVersion.Minor >= 1)) or (osVersion.Major > 5);
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
      if (SetupTypesCombo.Items[3] <> '���������� ���������') then
      begin
        SetupTypesCombo.Items[3] := '���������� ���������';
      end;
    end;
  end;
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

		if (HasLangEn()) then
		begin
			RegWriteStringValue(hRootHive, LangsKeyName + '\en', '', AfxLangPath + '\mfc90enu.dll');
			RegWriteStringValue(hRootHive, LangsKeyName + '\en', 'LangDLL', ExpandConstant('{app}\English_USA.1252.dll'));
		end;

		if (HasLangRu) then
		begin
			RegWriteStringValue(hRootHive, LangsKeyName + '\ru', '', AfxLangPath + '\mfc90rus.dll');
			RegWriteStringValue(hRootHive, LangsKeyName + '\ru', 'LangDLL', ExpandConstant('{app}\Russian_Russia.1251.dll'));
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

[InstallDelete]
Type: files; Name: "{app}\msvcr71.dll"
Type: files; Name: "{app}\msvcp71.dll"
Type: files; Name: "{app}\mfc71.dll"
Type: files; Name: "{app}\mfc71enu.dll"
Type: files; Name: "{app}\mfc71rus.dll"
Type: filesandordirs; Name: "{app}\Languages"
Type: filesandordirs; Name: "{app}\Sources"

[Types]
Name: "typical"; Description: "Typical Installation"; Languages: en
Name: "typical"; Description: "����������� ���������"; Languages: ru
Name: "compact"; Description: "Compact Installation"; Languages: en
Name: "compact"; Description: "���������� ���������"; Languages: ru
Name: "full"; Description: "Full Installation"; Languages: en
Name: "full"; Description: "������ ���������"; Languages: ru
Name: "custom"; Description: "Custom Installation"; Flags: iscustom

[Components]
Name: "core"; Description: "UpdateIt! Core Files"; Types: compact typical full custom; Flags: fixed; Languages: en
Name: "core"; Description: "����������� ����� UpdateIt!"; Types: compact typical full custom; Flags: fixed; Languages: ru
Name: "runtimes"; Description: "Application Runtimes"; Types: typical full custom; Languages: en
Name: "runtimes"; Description: "���������� ������� ����������"; Types: typical full custom; Languages: ru
Name: "mui"; Description: "MUI Support"; Types: typical full custom; Languages: en
Name: "mui"; Description: "��������� �������������� ���������"; Types: typical full custom; Languages: ru
Name: "sources"; Description: "Source Code"; Types: full custom; Languages: en
Name: "sources"; Description: "�������� ������"; Types: full custom; Languages: ru

[Files]
Source: "..\Output.2008\x86\Release\MBCS\UpdateIt.exe"; DestDir: "{app}"; Components: core
Source: "..\HTML\UpdateIt.0409.chm"; DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: "..\HTML\UpdateIt.0419.chm"; DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: ".\ApacheLicense.rtf"; DestDir: "{app}"; Components: core; Flags: ignoreversion

Source: "..\OpenSSL\redist\ssleay32.dll"; DestDir: "{app}"; Components: core
Source: "..\OpenSSL\redist\libeay32.dll"; DestDir: "{app}"; Components: core

Source: "..\Languages\English_USA.1252\Output.2008\x86\Release\MBCS\English_USA.1252.dll"; DestDir: "{app}"; Components: core; Check: HasLangEn
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90enu.dll"; DestDir: "{app}"; Components: core; Check: HasLangEn; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90enu.dll"; DestDir: "{app}\Microsoft.VC90.MFC\Microsoft.VC90.MFCLOC"; Components: core; Check: HasLangEn; MinVersion: 0,5.01.2600

Source: "..\Languages\Russian_Russia.1251\Output.2008\x86\Release\MBCS\Russian_Russia.1251.dll"; DestDir: "{app}"; Components: core; Check: HasLangEn
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90rus.dll"; DestDir: "{app}"; Components: core; Check: HasLangEn; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90rus.dll"; DestDir: "{app}\Microsoft.VC90.MFC\Microsoft.VC90.MFCLOC"; Components: core; Check: HasLangEn; MinVersion: 0,5.01.2600

Source: "..\Redist\Microsoft.VC90.MFCLOC\Microsoft.VC90.MFCLOC.manifest"; DestDir: "{app}\Microsoft.VC90.MFC\Microsoft.VC90.MFCLOC"; Components: core; MinVersion: 0,5.01.2600

Source: "..\Redist\Microsoft.VC90.CRT\msvcr90.dll"; DestDir: "{app}"; Components: runtimes; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.CRT\msvcp90.dll"; DestDir: "{app}"; Components: runtimes; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.CRT\msvcm90.dll"; DestDir: "{app}"; Components: runtimes; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion

Source: "..\Redist\Microsoft.VC90.CRT\msvcr90.dll"; DestDir: "{app}\Microsoft.VC90.CRT"; Components: runtimes; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.CRT\msvcp90.dll"; DestDir: "{app}\Microsoft.VC90.CRT"; Components: runtimes; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.CRT\msvcm90.dll"; DestDir: "{app}\Microsoft.VC90.CRT"; Components: runtimes; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.CRT\Microsoft.VC90.CRT.manifest"; DestDir: "{app}\Microsoft.VC90.CRT"; Components: runtimes; MinVersion: 0,5.01.2600

Source: "..\Redist\Microsoft.VC90.MFC\mfc90.dll"; DestDir: "{app}"; Components: runtimes; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion
Source: "..\Redist\Microsoft.VC90.MFC\mfcm90.dll"; DestDir: "{app}"; Components: runtimes; OnlyBelowVersion: 0,5.01.2600; Flags: ignoreversion

Source: "..\Redist\Microsoft.VC90.MFC\mfc90.dll"; DestDir: "{app}\Microsoft.VC90.MFC"; Components: runtimes; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.MFC\mfcm90.dll"; DestDir: "{app}\Microsoft.VC90.MFC"; Components: runtimes; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.MFC\Microsoft.VC90.MFC.manifest"; DestDir: "{app}\Microsoft.VC90.MFC"; Components: runtimes; MinVersion: 0,5.01.2600

Source: "..\..\Repository\AfxGadgets\AfxGadgets.2008.vcproj"; DestDir: "{app}\Sources\Repository\AfxGadgets"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\AfxGadgets\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\AfxGadgets\Source"; Components: sources; Flags: ignoreversion

Source: "..\..\Repository\CodeProject\CodeProject.2008.vcproj"; DestDir: "{app}\Sources\Repository\CodeProject"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\CodeProject\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\CodeProject\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\CodeProject\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\CodeProject\Source"; Components: sources; Flags: ignoreversion

Source: "..\..\Repository\Naughter\Naughter.2008.vcproj"; DestDir: "{app}\Sources\Repository\Naughter"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Naughter\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Naughter\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Naughter\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\Naughter\Source"; Components: sources; Flags: ignoreversion recursesubdirs

Source: "..\OpenSSL\include\openssl\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpdateIt\OpenSSL\include\openssl"; Components: sources; Flags: ignoreversion
Source: "..\OpenSSL\lib\VC\ssleay32MD.lib"; DestDir: "{app}\Sources\UpdateIt\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\OpenSSL\lib\VC\ssleay32MDd.lib"; DestDir: "{app}\Sources\UpdateIt\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\OpenSSL\lib\VC\libeay32MD.lib"; DestDir: "{app}\Sources\UpdateIt\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\OpenSSL\lib\VC\libeay32MDd.lib"; DestDir: "{app}\Sources\UpdateIt\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\OpenSSL\redist\ssleay32.dll"; DestDir: "{app}\Sources\UpdateIt\OpenSSL\redist"; Components: sources; Flags: ignoreversion
Source: "..\OpenSSL\redist\libeay32.dll"; DestDir: "{app}\Sources\UpdateIt\OpenSSL\redist"; Components: sources; Flags: ignoreversion

Source: "..\..\Repository\ZipArchive\ZipArchive.2008.vcproj"; DestDir: "{app}\Sources\Repository\ZipArchive"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\ZipArchive\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\MiscText\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\ZipArchive\MiscText"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\ZipArchive\Source"; Components: sources; Flags: ignoreversion

Source: "..\..\Repository\zlib\zlib.2008.vcproj"; DestDir: "{app}\Sources\Repository\zlib"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\zlib\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\zlib\Source"; Components: sources; Flags: ignoreversion

Source: "..\Languages\English_USA.1252\English_USA.1252.2008.vcproj"; DestDir: "{app}\Sources\UpdateIt\Languages\English_USA.1252"; Components: sources; Flags: ignoreversion
Source: "..\Languages\English_USA.1252\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Languages\English_USA.1252\Source"; Components: sources; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Russian_Russia.1251.2008.vcproj"; DestDir: "{app}\Sources\UpdateIt\Languages\Russian_Russia.1251"; Components: sources; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Languages\Russian_Russia.1251\Source"; Components: sources; Flags: ignoreversion

Source: "..\Redist\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpdateIt\Redist"; Components: sources; Flags: ignoreversion recursesubdirs

Source: "..\UpdateIt.2008.vcproj"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\UpdateIt.2008.sln"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\UpdateIt.2008.build"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\VerifyVer.build.inc"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\StampVer.build.inc"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\CreateDoc.build.inc"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\CleanDoc.build.inc"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\HTML\*"; Excludes: ".svn, *.chm"; DestDir: "{app}\Sources\UpdateIt\HTML"; Components: sources; Flags: ignoreversion
Source: "..\HTML\images\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpdateIt\HTML\images"; Components: sources; Flags: ignoreversion
Source: "..\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Source"; Components: sources; Flags: ignoreversion recursesubdirs

[Icons]
Name: "{group}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"
Name: "{group}\Documentation (English)"; Filename: "{app}\UpdateIt.0409.chm"; Languages: en
Name: "{group}\������������ (�� ����������)"; Filename: "{app}\UpdateIt.0409.chm"; Languages: ru
Name: "{group}\Documentation (Russian)"; Filename: "{app}\UpdateIt.0419.chm"; Languages: en
Name: "{group}\������������ (�� �������)"; Filename: "{app}\UpdateIt.0419.chm"; Languages: ru
Name: "{group}\License Agreement"; Filename: "{app}\ApacheLicense.rtf"; Languages: en
Name: "{group}\������������ ����������"; Filename: "{app}\ApacheLicense.rtf"; Languages: ru
Name: "{group}\UpdateIt! on the Web"; Filename: "{app}\UpdateIt.url"; Languages: en
Name: "{group}\UpdateIt! � ��������"; Filename: "{app}\UpdateIt.url"; Languages: ru
Name: "{group}\Build UpdateIt!"; FileName: "{app}\Sources\UpdateIt\UpdateIt.2008.sln"; Components: sources; Languages: en
Name: "{group}\������� UpdateIt!"; FileName: "{app}\Sources\UpdateIt\UpdateIt.2008.sln"; Components: sources; Languages: ru
Name: "{group}\Uninstall UpdateIt!"; Filename: "{uninstallexe}"; Languages: en
Name: "{group}\������� UpdateIt!"; Filename: "{uninstallexe}"; Languages: ru
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
