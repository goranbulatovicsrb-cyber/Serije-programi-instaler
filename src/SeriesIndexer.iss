; SeriesIndexer v2.0 - Inno Setup Installer

#define AppName      "SeriesIndexer"
#define AppVersion   "2.0"
#define AppExeName   "SeriesIndexer.exe"

[Setup]
AppId={{8F3A2C1D-4E5B-4F6A-9D8E-1C2B3A4D5E6F}
AppName={#AppName}
AppVersion={#AppVersion}
DefaultDirName={autopf}\{#AppName}
DefaultGroupName={#AppName}
DisableProgramGroupPage=yes
PrivilegesRequired=lowest
OutputBaseFilename=SeriesIndexer_v{#AppVersion}_Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
UninstallDisplayName={#AppName} v{#AppVersion}
CloseApplications=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Napravi ikonu na desktopu"; Flags: unchecked

[Files]
Source: "build\Release\{#AppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Core.dll";    DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Gui.dll";     DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\Qt5Sql.dll";     DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\platforms\*";    DestDir: "{app}\platforms";    Flags: ignoreversion recursesubdirs
Source: "build\Release\sqldrivers\*";   DestDir: "{app}\sqldrivers";   Flags: ignoreversion recursesubdirs
Source: "build\Release\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs
Source: "build\Release\styles\*";       DestDir: "{app}\styles";       Flags: ignoreversion recursesubdirs skipifsourcedoesntexist
Source: "build\Release\vcruntime140.dll";   DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "build\Release\vcruntime140_1.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "build\Release\msvcp140.dll";       DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "series_seed.db"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.md";      DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#AppName}";       Filename: "{app}\{#AppExeName}"
Name: "{group}\Deinstalacija";    Filename: "{uninstallexe}"
Name: "{autodesktop}\{#AppName}"; Filename: "{app}\{#AppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#AppExeName}"; Description: "Pokrenite {#AppName}"; Flags: nowait postinstall skipifsilent

[Code]
function IsPythonInstalled(): Boolean;
var ResultCode: Integer;
begin
  Result := Exec('cmd.exe', '/C python --version', '', SW_HIDE, ewWaitUntilTerminated, ResultCode) and (ResultCode = 0);
  if not Result then
    Result := Exec('cmd.exe', '/C python3 --version', '', SW_HIDE, ewWaitUntilTerminated, ResultCode) and (ResultCode = 0);
end;

procedure CurStepChanged(CurStep: TSetupStep);
var ResultCode: Integer;
begin
  if CurStep = ssPostInstall then begin
    if IsPythonInstalled() then
      Exec('cmd.exe', '/C python -m pip install pandas openpyxl --quiet --user', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
  end;
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;
  if CurPageID = wpReady then begin
    if not IsPythonInstalled() then begin
      if MsgBox('Python nije pronađen. Potreban je za Excel uvoz/izvoz.' + #13#10 + 'Nastaviti bez Pythona?',
        mbConfirmation, MB_YESNO) = IDNO then
        Result := False;
    end;
  end;
end;
