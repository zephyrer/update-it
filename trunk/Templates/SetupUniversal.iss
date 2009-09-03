;; UpdateIt! application.
;; Copyright (c) 2002-2009 by Elijah Zarezky,
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
AppVerName=UpdateIt! @VER_MAJOR@.@VER_MINOR@.@VER_BUILD@
AppID={{78461E4F-C4AD-4488-97F7-773CCA325839}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://zarezky.spb.ru/
AppSupportURL=http://zarezky.spb.ru/projects/update_it.html
AppUpdatesURL=http://zarezky.spb.ru/projects/update_it.html
AppVersion=@VER_MAJOR@.@VER_MINOR@.@VER_BUILD@
AppCopyright=Copyright © 2002–2009 by Elijah Zarezky
DefaultDirName={pf}\Elijah Zarezky\UpdateIt
DefaultGroupName=Elijah Zarezky\UpdateIt
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=UpdateIt-@VER_MAJOR@.@VER_MINOR@.@VER_BUILD@-setup-universal
VersionInfoVersion=@VER_MAJOR@.@VER_MINOR@.@VER_BUILD@.@SVN_REV@
VersionInfoProductName=Power Gadgets
VersionInfoProductVersion=@VER_MAJOR@.@VER_MINOR@
MinVersion=4.1.2222,5.0.2195
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
LanguageCodePage=0
DialogFontName=MS Shell Dlg 2
DialogFontSize=8
en.LanguageName=English
ru.LanguageName=Russian

[Messages]
SelectLanguageTitle=Language Selection
SelectLanguageLabel=Please select UpdateIt! language:

[Code]
function HasLangEn(): Boolean;
begin
	Result := (IsComponentSelected('muien')) or (ActiveLanguage() = 'en');
end;

function HasLangRu(): Boolean;
begin
	Result := (IsComponentSelected('muiru')) or (ActiveLanguage() = 'ru');
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
			RegWriteStringValue(hRootHive, LangsKeyName + '\en', '', '.\mfc90enu.dll');
			RegWriteStringValue(hRootHive, LangsKeyName + '\en', 'LangDLL', '.\English_USA.1252.dll');
		end;

		if (HasLangRu) then
		begin
			RegWriteStringValue(hRootHive, LangsKeyName + '\ru', '', '.\mfc90rus.dll');
			RegWriteStringValue(hRootHive, LangsKeyName + '\ru', 'LangDLL', '.\Russian_Russia.1251.dll');
		end;

		if (IsComponentSelected('muien') or IsComponentSelected('muiru')) then
		begin
			RegWriteStringValue(hRootHive, LangsKeyName, '', 'en;ru');
		end
		else begin
			RegWriteStringValue(hRootHive, LangsKeyName, '', ActiveLanguage());
		end;

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
Name: "muiru"; Description: "Russian Interface Support"; Types: typical full custom; Languages: en
Name: "muien"; Description: "Поддержка английского интерфейса"; Types: typical full custom; Languages: ru
Name: "sources"; Description: "Source Code"; Types: full custom; Languages: en
Name: "sources"; Description: "Исходные тексты"; Types: full custom; Languages: ru

[Files]
;; core application files
Source: "..\Output.2008\x86\@BUILD_TYPE@\@CHARACTER_SET@\UpdateIt.exe"; DestDir: "{app}"; Components: core
Source: "..\CrashReporter\Output.2008\x86\@BUILD_TYPE@\@CHARACTER_SET@\CrashReporter.exe"; DestDir: "{app}"; Components: core
Source: "..\HTML\UpdateIt.0409.chm"; DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: "..\HTML\UpdateIt.0419.chm"; DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: ".\ApacheLicense.rtf"; DestDir: "{app}"; Components: core; Flags: ignoreversion

;; application localizations
Source: "..\Languages\English_USA.1252\Output.2008\x86\@BUILD_TYPE@\@CHARACTER_SET@\English_USA.1252.dll"; DestDir: "{app}"; Components: core; Languages: en
Source: "..\Languages\English_USA.1252\Output.2008\x86\@BUILD_TYPE@\@CHARACTER_SET@\English_USA.1252.dll"; DestDir: "{app}"; Components: muien; Languages: ru
Source: "..\Languages\Russian_Russia.1251\Output.2008\x86\@BUILD_TYPE@\@CHARACTER_SET@\Russian_Russia.1251.dll"; DestDir: "{app}"; Components: core; Languages: ru
Source: "..\Languages\Russian_Russia.1251\Output.2008\x86\@BUILD_TYPE@\@CHARACTER_SET@\Russian_Russia.1251.dll"; DestDir: "{app}"; Components: muiru; Languages: en

;; CRT redistributables
Source: "..\Redist\Microsoft.VC90.CRT\msvcr90.dll"; DestDir: "{app}"; Components: runtimes
Source: "..\Redist\Microsoft.VC90.CRT\msvcp90.dll"; DestDir: "{app}"; Components: runtimes
Source: "..\Redist\Microsoft.VC90.CRT\msvcm90.dll"; DestDir: "{app}"; Components: runtimes
Source: "..\Redist\Microsoft.VC90.CRT\Microsoft.VC90.CRT.manifest"; DestDir: "{app}"; Components: runtimes; MinVersion: 0,5.01.2600

;; MFC library redistributables
Source: "..\Redist\Microsoft.VC90.MFC\mfc90.dll"; DestDir: "{app}"; Components: runtimes
Source: "..\Redist\Microsoft.VC90.MFC\mfcm90.dll"; DestDir: "{app}"; Components: runtimes
Source: "..\Redist\Microsoft.VC90.MFC\Microsoft.VC90.MFC.manifest"; DestDir: "{app}"; Components: runtimes; MinVersion: 0,5.01.2600

;; MFC library localizations
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90enu.dll"; DestDir: "{app}"; Components: core; Languages: en
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90enu.dll"; DestDir: "{app}"; Components: muien; Languages: ru
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90rus.dll"; DestDir: "{app}"; Components: core; Languages: ru
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90rus.dll"; DestDir: "{app}"; Components: muiru; Languages: en
Source: "..\Redist\Microsoft.VC90.MFCLOC\Microsoft.VC90.MFCLOC.manifest"; DestDir: "{app}"; Components: core; MinVersion: 0,5.01.2600

;; OpenSSL redistributables
Source: "..\..\Repository\OpenSSL\redist\ssleay32.dll"; DestDir: "{app}"; Components: core
Source: "..\..\Repository\OpenSSL\redist\libeay32.dll"; DestDir: "{app}"; Components: core

;; DbgHelp redistributables
Source: "..\Redist\dbghelp.dll"; DestDir: "{app}";

;; AfxGadgets library sources
Source: "..\..\Repository\AfxGadgets\AfxGadgets.2008.vcproj"; DestDir: "{app}\Sources\Repository\AfxGadgets"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\AfxGadgets\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\AfxGadgets\Source"; Components: sources; Flags: ignoreversion

;; CodeProject library sources
Source: "..\..\Repository\CodeProject\CodeProject.2008.vcproj"; DestDir: "{app}\Sources\Repository\CodeProject"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\CodeProject\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\CodeProject\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\CodeProject\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\CodeProject\Source"; Components: sources; Flags: ignoreversion

;; Naughter library sources
Source: "..\..\Repository\Naughter\Naughter.2008.vcproj"; DestDir: "{app}\Sources\Repository\Naughter"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Naughter\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Naughter\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Naughter\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\Naughter\Source"; Components: sources; Flags: ignoreversion recursesubdirs

;; ZipArchive library sources
Source: "..\..\Repository\ZipArchive\ZipArchive.2008.vcproj"; DestDir: "{app}\Sources\Repository\ZipArchive"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\ZipArchive\Help"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\MiscText\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\ZipArchive\MiscText"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\ZipArchive\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\ZipArchive\Source"; Components: sources; Flags: ignoreversion

;; zlib library sources
Source: "..\..\Repository\zlib\zlib.2008.vcproj"; DestDir: "{app}\Sources\Repository\zlib"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\zlib\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\zlib\Source"; Components: sources; Flags: ignoreversion

;; libjpeg library sources
Source: "..\..\Repository\Graphics\libjpeg\changelog.txt"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\coderules.txt"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\filelist.txt"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\install.txt"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\jconfig.txt"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\libjpeg.2008.vcproj"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\libjpeg.txt"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\readme.txt"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\structure.txt"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libjpeg\Source\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Graphics\libjpeg\Source"; Components: sources; Flags: ignoreversion

;; libmng library sources
Source: "..\..\Repository\Graphics\libmng\changes.txt"; DestDir: "{app}\Sources\Repository\Graphics\libmng"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libmng\libmng.2008.vcproj"; DestDir: "{app}\Sources\Repository\Graphics\libmng"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libmng\license.txt"; DestDir: "{app}\Sources\Repository\Graphics\libmng"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libmng\readme.txt"; DestDir: "{app}\Sources\Repository\Graphics\libmng"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libmng\Source\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Graphics\libmng\Source"; Components: sources; Flags: ignoreversion

;; libpng library sources
Source: "..\..\Repository\Graphics\libpng\changes.txt"; DestDir: "{app}\Sources\Repository\Graphics\libpng"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libpng\libpng.2008.vcproj"; DestDir: "{app}\Sources\Repository\Graphics\libpng"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libpng\libpng.txt"; DestDir: "{app}\Sources\Repository\Graphics\libpng"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libpng\license.txt"; DestDir: "{app}\Sources\Repository\Graphics\libpng"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libpng\Source\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Graphics\libpng\Source"; Components: sources; Flags: ignoreversion

;; libtiff library sources
Source: "..\..\Repository\Graphics\libtiff\changelog.txt"; DestDir: "{app}\Sources\Repository\Graphics\libtiff"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libtiff\libtiff.2008.vcproj"; DestDir: "{app}\Sources\Repository\Graphics\libtiff"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libtiff\readme.txt"; DestDir: "{app}\Sources\Repository\Graphics\libtiff"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\libtiff\HTML\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Graphics\libtiff\HTML"; Components: sources; Flags: ignoreversion recursesubdirs
Source: "..\..\Repository\Graphics\libtiff\Source\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Graphics\libtiff\Source"; Components: sources; Flags: ignoreversion

;; OpenEXR library sources
Source: "..\..\Repository\Graphics\OpenEXR\authors.txt"; DestDir: "{app}\Sources\Repository\Graphics\OpenEXR"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\OpenEXR\changelog.txt"; DestDir: "{app}\Sources\Repository\Graphics\OpenEXR"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\OpenEXR\copying.txt"; DestDir: "{app}\Sources\Repository\Graphics\OpenEXR"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\OpenEXR\license.txt"; DestDir: "{app}\Sources\Repository\Graphics\OpenEXR"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\OpenEXR\OpenEXR.2008.vcproj"; DestDir: "{app}\Sources\Repository\Graphics\OpenEXR"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\OpenEXR\Source\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Graphics\OpenEXR\Source"; Components: sources; Flags: ignoreversion recursesubdirs

;; OpenJPEG library sources
Source: "..\..\Repository\Graphics\OpenJPEG\changelog.txt"; DestDir: "{app}\Sources\Repository\Graphics\OpenJPEG"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\OpenJPEG\license.txt"; DestDir: "{app}\Sources\Repository\Graphics\OpenJPEG"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\OpenJPEG\OpenJPEG.2008.vcproj"; DestDir: "{app}\Sources\Repository\Graphics\OpenJPEG"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\Graphics\OpenJPEG\Source\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Graphics\OpenJPEG\Source"; Components: sources; Flags: ignoreversion

;; FreeImage library sources
Source: "..\..\Repository\FreeImage\FreeImage.2008.vcproj"; DestDir: "{app}\Sources\Repository\FreeImage"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\FreeImage\license-fi.txt"; DestDir: "{app}\Sources\Repository\FreeImage"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\FreeImage\license-gpl.txt"; DestDir: "{app}\Sources\Repository\FreeImage"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\FreeImage\whatsnew.txt"; DestDir: "{app}\Sources\Repository\FreeImage"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\FreeImage\Include\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\FreeImage\Include"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\FreeImage\Source\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\FreeImage\Source"; Components: sources; Flags: ignoreversion recursesubdirs
Source: "..\..\Repository\FreeImage\Toolkit\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\FreeImage\Toolkit"; Components: sources; Flags: ignoreversion

;; FreeImagePlus library sources
Source: "..\..\Repository\FreeImagePlus\FreeImagePlus.2008.vcproj"; DestDir: "{app}\Sources\Repository\FreeImagePlus"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\FreeImagePlus\WhatsNew.txt"; DestDir: "{app}\Sources\Repository\FreeImagePlus"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\FreeImagePlus\Source\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\FreeImagePlus\Source"; Components: sources; Flags: ignoreversion

;; OpenSSL headers and import libraries
Source: "..\..\Repository\OpenSSL\include\openssl\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\OpenSSL\include\openssl"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\lib\VC\ssleay32MD.lib"; DestDir: "{app}\Sources\Repository\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\lib\VC\ssleay32MDd.lib"; DestDir: "{app}\Sources\Repository\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\lib\VC\libeay32MD.lib"; DestDir: "{app}\Sources\Repository\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\lib\VC\libeay32MDd.lib"; DestDir: "{app}\Sources\Repository\OpenSSL\lib\VC"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\redist\ssleay32.dll"; DestDir: "{app}\Sources\Repository\OpenSSL\redist"; Components: sources; Flags: ignoreversion
Source: "..\..\Repository\OpenSSL\redist\libeay32.dll"; DestDir: "{app}\Sources\Repository\OpenSSL\redist"; Components: sources; Flags: ignoreversion

;; CRT/MFC/DbgHelp redistributables
Source: "..\Redist\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpdateIt\Redist"; Components: sources; Flags: recursesubdirs

;; UpdateIt! application sources
Source: "..\UpdateIt.2008.vcproj"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\UpdateIt.2008.sln"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\UpdateIt.2008.build"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\VerifyVer.build.inc"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\StampVer.build.inc"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\CreateDoc.build.inc"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\CleanDoc.build.inc"; DestDir: "{app}\Sources\UpdateIt"; Components: sources; Flags: ignoreversion
Source: "..\FreeImage\FreeImage.2008.vcproj"; DestDir: "{app}\Sources\UpdateIt\FreeImage"; Components: sources; Flags: ignoreversion
Source: "..\HTML\*"; Excludes: ".svn, *.chm"; DestDir: "{app}\Sources\UpdateIt\HTML"; Components: sources; Flags: ignoreversion
Source: "..\HTML\images\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpdateIt\HTML\images"; Components: sources; Flags: ignoreversion
Source: "..\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Source"; Components: sources; Flags: ignoreversion recursesubdirs
Source: "..\Templates\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Templates"; Components: sources; Flags: ignoreversion recursesubdirs
Source: "..\Common\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Source"; Components: sources; Flags: ignoreversion recursesubdirs

;; CrashReporter sources
Source: "..\CrashReporter\CrashReporter.2008.vcproj"; DestDir: "{app}\Sources\UpdateIt\CrashReporter"; Components: sources; Flags: ignoreversion
Source: "..\CrashReporter\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\CrashReporter\Source"; Components: sources; Flags: ignoreversion
Source: "..\CrashReporter\Templates\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\CrashReporter\Templates"; Components: sources; Flags: ignoreversion

;; UpdateIt! localizations sources
Source: "..\Languages\English_USA.1252\English_USA.1252.2008.vcproj"; DestDir: "{app}\Sources\UpdateIt\Languages\English_USA.1252"; Components: sources; Flags: ignoreversion
Source: "..\Languages\English_USA.1252\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Languages\English_USA.1252\Source"; Components: sources; Flags: ignoreversion
Source: "..\Languages\English_USA.1252\Templates\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Languages\English_USA.1252\Templates"; Components: sources; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Russian_Russia.1251.2008.vcproj"; DestDir: "{app}\Sources\UpdateIt\Languages\Russian_Russia.1251"; Components: sources; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Languages\Russian_Russia.1251\Source"; Components: sources; Flags: ignoreversion
Source: "..\Languages\Russian_Russia.1251\Templates\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpdateIt\Languages\Russian_Russia.1251\Templates"; Components: sources; Flags: ignoreversion

;; DbgHelp headers and import libraries
Source: "..\Include\dbghelp.h"; DestDir: "{app}\Sources\UpdateIt\Include"; Components: sources; Flags: ignoreversion
Source: "..\Lib\dbghelp.lib"; DestDir: "{app}\Sources\UpdateIt\Lib"; Components: sources; Flags: ignoreversion

[Icons]
Name: "{group}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; WorkingDir: "{app}"
Name: "{group}\Documentation (English)"; Filename: "{app}\UpdateIt.0409.chm"; Languages: en
Name: "{group}\Документация (на английском)"; Filename: "{app}\UpdateIt.0409.chm"; Languages: ru
Name: "{group}\Documentation (Russian)"; Filename: "{app}\UpdateIt.0419.chm"; Languages: en
Name: "{group}\Документация (на русском)"; Filename: "{app}\UpdateIt.0419.chm"; Languages: ru
Name: "{group}\License Agreement"; Filename: "{app}\ApacheLicense.rtf"; Languages: en
Name: "{group}\Лицензионное соглашение"; Filename: "{app}\ApacheLicense.rtf"; Languages: ru
Name: "{group}\UpdateIt! on the Web"; Filename: "{app}\UpdateIt.url"; Languages: en
Name: "{group}\UpdateIt! в Интернет"; Filename: "{app}\UpdateIt.url"; Languages: ru
Name: "{group}\Build UpdateIt!"; FileName: "{app}\Sources\UpdateIt\UpdateIt.2008.sln"; Components: sources; Languages: en
Name: "{group}\Собрать UpdateIt!"; FileName: "{app}\Sources\UpdateIt\UpdateIt.2008.sln"; Components: sources; Languages: ru
Name: "{group}\Uninstall UpdateIt!"; Filename: "{uninstallexe}"; Languages: en
Name: "{group}\Удалить UpdateIt!"; Filename: "{uninstallexe}"; Languages: ru
Name: "{userdesktop}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; WorkingDir: "{app}"; Tasks: quicklaunchicon

[INI]
Filename: "{app}\UpdateIt.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://zarezky.spb.ru/projects/update_it.html"
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

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; Flags: unchecked
Name: quicklaunchicon; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Run]
Filename: "{app}\UpdateIt.exe"; Description: "Launch UpdateIt!"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\UpdateIt.url"
Type: files; Name: "{userappdata}\Elijah Zarezky\UpdateIt\CrashReporter.ini"

;; end of file
