#define MyAppName "VisualScriptCAD"
#define MyAppVersion "0.0.1"
#define MyAppPublisher "Viktor Kovacs"
#define MyAppURL "http://www.kovacsv.hu/"
#define MyAppExeName "VisualScriptCAD.exe"

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} v{#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf64}\{#MyAppName}
DisableProgramGroupPage=yes
OutputBaseFilename={#MyAppName} v{#MyAppVersion} Installer
OutputDir="..\Build\Installer"
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked;

[Files]
Source: "..\Build\Release\VisualScriptCAD.exe"; DestDir: "{app}"; Flags: ignoreversion;
Source: "..\Build\Release\CGAL-vc140-mt-4.13.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "..\Build\Release\libgmp-10.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "..\Build\Release\libmpfr-4.dll"; DestDir: "{app}"; Flags: ignoreversion;
Source: "Redist\vc_redist.x64.exe"; DestDir: "{app}"; Flags: deleteafterinstall;

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\vc_redist.x64.exe"; Parameters: /quiet /norestart; StatusMsg: Installing redistributable package
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
