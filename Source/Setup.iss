;; UpdateIt! application.
;; Copyright (c) 2002-2004 by Elijah Zarezky,
;; All rights reserved.

;; Setup.iss - setup script for Inno Setup compiler

[Setup]
AppName=UpdateIt!
AppVerName=UpdateIt! 1.0.2461
AppID={{78461E4F-C4AD-4488-97F7-773CCA325839}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://home.peterstar.ru/zarezky
AppSupportURL=http://home.peterstar.ru/zarezky/projects/update_it.html
AppUpdatesURL=http://home.peterstar.ru/zarezky/projects/update_it.html
AppVersion=1.0.2461
DefaultDirName={pf}\PowerGadgets\UpdateIt
DefaultGroupName=PowerGadgets\UpdateIt
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=UpdateIt-1.0.2461-setup
VersionInfoVersion=1.0.2461
MinVersion=4.1.2222,5.0.2195

[LangOptions]
DialogFontName=Tahoma
DialogFontSize=8

[Files]
Source: "..\ReleaseA\UpdateIt.exe"; DestDir: "{app}"
Source: "..\HTML\UpdateIt.chm"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"
Name: "{group}\UpdateIt! Documentation"; Filename: "{app}\UpdateIt.chm"
Name: "{group}\UpdateIt! on the Web"; Filename: "{app}\UpdateIt.url"
Name: "{group}\Uninstall UpdateIt!"; Filename: "{uninstallexe}"
Name: "{userdesktop}\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\UpdateIt!"; Filename: "{app}\UpdateIt.exe"; Tasks: quicklaunchicon

[INI]
Filename: "{app}\UpdateIt.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://home.peterstar.ru/zarezky/projects/update_it.html"

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
