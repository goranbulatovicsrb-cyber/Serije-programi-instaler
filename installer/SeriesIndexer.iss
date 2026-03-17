; ============================================================
;  SeriesIndexer v2.0 — Inno Setup Installer Script
;  Kompajliraj sa: Inno Setup 6.x  (https://jrsoftware.org)
; ============================================================

#define AppName      "SeriesIndexer"
#define AppVersion   "2.0"
#define AppPublisher "SeriesIndexer"
#define AppURL       "https://github.com/YOUR_USERNAME/SeriesIndexer"
#define AppExeName   "SeriesIndexer.exe"

[Setup]
AppId={{8F3A2C1D-4E5B-4F6A-9D8E-1C2B3A4D5E6F}
AppName={#AppName}
AppVersion={#AppVersion}
AppPublisherURL={#AppURL}
AppSupportURL={#AppURL}
AppUpdatesURL={#AppURL}/releases
DefaultDirName={autopf}\{#AppName}
DefaultGroupName={#AppName}
DisableProgramGroupPage=yes
; No admin rights required
PrivilegesRequired=lowest
OutputBaseFilename=SeriesIndexer_v{#AppVersion}_Setup
SetupIconFile=installer\app_icon.ico
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
WizardResizable=yes
UninstallDisplayIcon={app}\{#AppExeName}
UninstallDisplayName={#AppName} v{#AppVersion}
CloseApplications=yes
RestartIfNeededByRun=no

; Installer appearance
WizardImageFile=installer\wizard_side.bmp
WizardSmallImageFile=installer\wizard_top.bmp

[Languages]
Name: "english";    MessagesFile: "compiler:Default.isl"
; Add Bosnian/Serbian if available, otherwise English is fine

[Tasks]
Name: "desktopicon";  Description: "Napravi ikonu na desktopu";       GroupDescription: "Dodatne opcije:"; Flags: unchecked
Name: "startmenu";    Description: "Dodaj u Start meni";              GroupDescription: "Dodatne opcije:"; Flags: checkedonce
Name: "autostart";    Description: "Pokretaj sa Windowsom (opciono)"; GroupDescription: "Dodatne opcije:"; Flags: unchecked

[Files]
; Main executable
Source: "build\Release\{#AppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; Qt5 runtime DLLs
Source: "build\Release\Qt5Core.dll";        DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Gui.dll";         DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Widgets.dll";     DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Network.dll";     DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Sql.dll";         DestDir: "{app}"; Flags: ignoreversion

; Qt plugins
Source: "build\Release\platforms\*";        DestDir: "{app}\platforms";        Flags: ignoreversion recursesubdirs
Source: "build\Release\sqldrivers\*";       DestDir: "{app}\sqldrivers";       Flags: ignoreversion recursesubdirs
Source: "build\Release\imageformats\*";     DestDir: "{app}\imageformats";     Flags: ignoreversion recursesubdirs
Source: "build\Release\styles\*";           DestDir: "{app}\styles";           Flags: ignoreversion recursesubdirs

; Visual C++ runtime (if using MSVC)
Source: "build\Release\vcruntime140.dll";   DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "build\Release\vcruntime140_1.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "build\Release\msvcp140.dll";       DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

; Seed database
Source: "series_seed.db"; DestDir: "{app}"; Flags: ignoreversion

; README
Source: "README.md"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#AppName}";                           Filename: "{app}\{#AppExeName}"
Name: "{group}\Deinstalacija";                         Filename: "{uninstallexe}"
Name: "{autodesktop}\{#AppName}";                      Filename: "{app}\{#AppExeName}"; Tasks: desktopicon

[Registry]
; Save install path
Root: HKCU; Subkey: "Software\SeriesIndexer"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"

; Autostart (optional)
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; \
  ValueType: string; ValueName: "SeriesIndexer"; ValueData: """{app}\{#AppExeName}"""; \
  Tasks: autostart; Flags: uninsdeletevalue

[Run]
Filename: "{app}\{#AppExeName}"; \
  Description: "Pokrenite {#AppName} odmah"; \
  Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: dirifempty; Name: "{app}"

[Code]
// ── Custom wizard pages ──────────────────────────────────────

// Check if Python is installed (needed for Excel import/export)
function IsPythonInstalled(): Boolean;
var
  ResultCode: Integer;
begin
  Result := Exec('cmd.exe', '/C python --version > nul 2>&1', '', SW_HIDE, ewWaitUntilTerminated, ResultCode)
    and (ResultCode = 0);
  if not Result then
    Result := Exec('cmd.exe', '/C python3 --version > nul 2>&1', '', SW_HIDE, ewWaitUntilTerminated, ResultCode)
      and (ResultCode = 0);
end;

procedure InitializeWizard();
begin
  // Custom welcome text
  WizardForm.WelcomeLabel2.Caption :=
    'Ovaj čarobnjak će instalirati SeriesIndexer v2.0 na vaš računar.' + #13#10 + #13#10 +
    'SeriesIndexer je program za upravljanje kolekcijom filmova i serija ' +
    'sa IMDB integracijom i uvozom/izvozom Excel fajlova.' + #13#10 + #13#10 +
    'Preporučuje se da zatvorite sve ostale programe prije nastavka.';
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;

  // On finish page — check Python
  if CurPageID = wpReady then begin
    if not IsPythonInstalled() then begin
      if MsgBox(
        'Python nije pronađen na vašem sistemu.' + #13#10 + #13#10 +
        'Python je potreban za uvoz/izvoz Excel (.xlsx) fajlova.' + #13#10 +
        'Možete ga preuzeti besplatno sa python.org' + #13#10 + #13#10 +
        'Nastaviti instalaciju bez Pythona? ' +
        '(Excel uvoz/izvoz neće raditi dok ne instalirate Python)',
        mbConfirmation, MB_YESNO) = IDNO then
        Result := False;
    end;
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  ResultCode: Integer;
begin
  if CurStep = ssPostInstall then begin
    // Try to install Python packages silently if Python exists
    if IsPythonInstalled() then begin
      Exec('cmd.exe',
        '/C python -m pip install pandas openpyxl --quiet --user',
        '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
  end;
end;
