;; UpdateIt! application.
;; Copyright (c) 2002-2005 by Elijah Zarezky,
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

[Setup]
AppName=UpdateIt!
AppVerName=UpdateIt! 1.0.3410
AppID={{78461E4F-C4AD-4488-97F7-773CCA325839}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://zarezky.spb.ru/
AppSupportURL=http://zarezky.spb.ru/projects/update_it.html
AppUpdatesURL=http://zarezky.spb.ru/projects/update_it.html
AppVersion=1.0.3410
DefaultDirName={pf}\PowerGadgets\UpdateIt
DefaultGroupName=PowerGadgets\UpdateIt
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=UpdateIt-1.0.3410-setup-universal
VersionInfoVersion=1.0.3410
MinVersion=4.1.2222,5.0.2195
WizardImageFile=compiler:WizModernImage-IS.bmp
WizardSmallImageFile=compiler:WizModernSmallImage-IS.bmp
LicenseFile=ApacheLicense.rtf

[LangOptions]
DialogFontName=Tahoma
DialogFontSize=8

[Types]
Name: "typical"; Description: "Typical Installation"
Name: "compact"; Description: "Compact Installation"
Name: "full"; Description: "Full Installation"
Name: "custom"; Description: "Custom Installation"; Flags: iscustom

[Components]
Name: "core"; Description: "UpdateIt! Core Files"; Types: compact typical full custom; Flags: fixed
Name: "runtimes"; Description: "Application Runtimes"; Types: typical full custom
Name: "sources"; Description: "Source Code"; Types: full custom

[Files]
Source: "..\70_ReleaseA\UpdateIt.exe"; DestDir: "{app}"; Components: core
Source: "..\HTML\UpdateIt.chm"; DestDir: "{app}"; Components: core; Flags: ignoreversion
Source: ".\ApacheLicense.rtf"; DestDir: "{app}"; Components: core
Source: "..\Redist\mfc70.dll"; DestDir: "{app}"; Components: runtimes
Source: "..\Redist\msvcr70.dll"; DestDir: "{app}"; Components: runtimes
Source: "..\Redist\msvcp70.dll"; DestDir: "{app}"; Components: runtimes
Source: "..\..\Repository\AfxGadgets\AfxGadgets.vcproj"; DestDir: "{app}\Sources\Repository\AfxGadgets"; Components: sources
Source: "..\..\Repository\AfxGadgets\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\AfxGadgets\Source"; Components: sources
Source: "..\..\Repository\CodeProject\CodeProject.vcproj"; DestDir: "{app}\Sources\Repository\CodeProject"; Components: sources
Source: "..\..\Repository\CodeProject\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\CodeProject\Help"; Components: sources
Source: "..\..\Repository\CodeProject\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\CodeProject\Source"; Components: sources
Source: "..\..\Repository\Naughter\Naughter.vcproj"; DestDir: "{app}\Sources\Repository\Naughter"; Components: sources
Source: "..\..\Repository\Naughter\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\Naughter\Help"; Components: sources
Source: "..\..\Repository\Naughter\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\Naughter\Source"; Components: sources
Source: "..\..\Repository\ZipArchive\ZipArchive.vcproj"; DestDir: "{app}\Sources\Repository\ZipArchive"; Components: sources
Source: "..\..\Repository\ZipArchive\Help\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\ZipArchive\Help"; Components: sources
Source: "..\..\Repository\ZipArchive\MiscText\*"; Excludes: ".svn"; DestDir: "{app}\Sources\Repository\ZipArchive\MiscText"; Components: sources
Source: "..\..\Repository\ZipArchive\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\ZipArchive\Source"; Components: sources
Source: "..\..\Repository\zlib\zlib.vcproj"; DestDir: "{app}\Sources\Repository\zlib"; Components: sources
Source: "..\..\Repository\zlib\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\Repository\zlib\Source"; Components: sources
Source: "..\UpdateIt70.vcproj"; DestDir: "{app}\Sources\UpddateIt"; Components: sources
Source: "..\UpdateIt70.sln"; DestDir: "{app}\Sources\UpddateIt"; Components: sources
Source: "..\HTML\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpddateIt\HTML"; Components: sources
Source: "..\HTML\images\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpddateIt\HTML\images"; Components: sources
Source: "..\Source\*"; Excludes: ".svn, *.aps"; DestDir: "{app}\Sources\UpddateIt\Source"; Components: sources
Source: "..\Source\BinRes\*"; Excludes: ".svn"; DestDir: "{app}\Sources\UpddateIt\Source\BinRes"; Components: sources

[Icons]
Name: "{group}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"
Name: "{group}\UpdateIt! Documentation"; Filename: "{app}\UpdateIt.chm"
Name: "{group}\UpdateIt! License"; Filename: "{app}\ApacheLicense.rtf"
Name: "{group}\UpdateIt! on the Web"; Filename: "{app}\UpdateIt.url"
Name: "{group}\Build UpdateIt!"; FileName: "{app}\Sources\UpddateIt\UpdateIt70.sln"; Components: sources
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
