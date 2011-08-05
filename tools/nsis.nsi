Name "VstBoard"
RequestExecutionLevel admin

OutFile "${OutFile}"
SetDateSave on
SetDatablockOptimize on
CRCCheck on
XPStyle on
SetCompressor lzma

InstallDir "$LOCALAPPDATA\VstBoard\"

!include LogicLib.nsh
!define START_LINK_DIR "$SMPROGRAMS\VstBoard"
!define START_LINK_RUN "$SMPROGRAMS\VstBoard\VstBoard.lnk"
!define START_LINK_UNINSTALLER "$SMPROGRAMS\VstBoard\Uninstall VstBoard.lnk"
!define REG_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\VstBoard"
!define REG_SETTINGS "Software\CtrlBrk\VstBoard"
!define UNINSTALLER_NAME "uninst.exe"
!define WEBSITE_LINK "http://vstboard.blogspot.com/"

LicenseText "License Agreeemt. Please review the license terms before installing VstBoard. If you accept the term of the agreement, click I Agree to continue. You must accept the agreement to install VstBoard"
LicenseData "license.txt"

Var InstFolder
Var VstDir

Page license
Page components

PageEx directory
  PageCallbacks defaultInstDir "" getInstDir
  Caption ": Installation Folder"
PageExEnd

PageEx directory
  PageCallbacks defaultVstDir "" getVstDir
  Caption ": Vst Plugins Folder"
PageExEnd

Page instfiles

UninstPage uninstConfirm
UninstPage instfiles


Function defaultInstDir
  StrCpy $INSTDIR "$LOCALAPPDATA\VstBoard"
FunctionEnd

Function getInstDir
  StrCpy $InstFolder $INSTDIR
FunctionEnd

Function defaultVstDir
  ReadRegStr $INSTDIR HKLM "SOFTWARE\VST" "VSTPluginsPath"
  ${If} $INSTDIR == ""
    StrCpy $INSTDIR "$PROGRAMFILES\Vstplugins\"
  ${EndIf}
FunctionEnd

Function getVstDir
  StrCpy $VstDir "$INSTDIR\"
  ReadRegStr $1 HKLM "SOFTWARE\VST" "VSTPluginsPath"
FunctionEnd

Section "VstBoard (required)"
	SetShellVarContext current
	SectionIn RO
	SetOutPath $InstFolder
	File "QtCore4.dll"
	File "QtGui4.dll"
	File "QtScript4.dll"
	File "QtSolutions_MFCMigrationFramework-head.dll"
	File "VstBoard.exe"
	File "VstBoardPlugin.dll"
	File "license.txt"
	File "GPL.txt"
	File "LGPL.txt"

	WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayName" "VstBoard"
	WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayIcon" "$\"$InstFolder\VstBoard.exe$\""
	WriteRegStr HKCU "${REG_UNINSTALL}" "Publisher" "CtrlBrk"
	WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayVersion" "a0.0.0.1"
	WriteRegDWord HKCU "${REG_UNINSTALL}" "EstimatedSize" 15000 ;KB
	WriteRegStr HKCU "${REG_UNINSTALL}" "HelpLink" "${WEBSITE_LINK}"
	WriteRegStr HKCU "${REG_UNINSTALL}" "URLInfoAbout" "${WEBSITE_LINK}"
	WriteRegStr HKCU "${REG_UNINSTALL}" "InstallLocation" "$\"$InstFolder$\""
	WriteRegStr HKCU "${REG_UNINSTALL}" "InstallSource" "$\"$EXEDIR$\""
	WriteRegDWord HKCU "${REG_UNINSTALL}" "NoModify" 1
	WriteRegDWord HKCU "${REG_UNINSTALL}" "NoRepair" 1
	WriteRegStr HKCU "${REG_UNINSTALL}" "UninstallString" "$\"$InstFolder\${UNINSTALLER_NAME}$\""
	WriteRegStr HKCU "${REG_UNINSTALL}" "Comments" "Uninstalls VstBoard."
	WriteRegStr HKCU "${REG_SETTINGS}" "InstallLocation" "$\"$InstFolder$\""
	WriteUninstaller $InstFolder\uninst.exe

	SetOutPath $VstDir
	WriteRegStr HKCU "${REG_SETTINGS}" "pluginInstallDir" $VstDir
	File "VstBoardEffect.dll"
	File "VstBoardInstrument.dll"
	
SectionEnd

Section "Start Menu Shortcuts"
	SetShellVarContext current
	SetOutPath $InstFolder
	CreateDirectory "${START_LINK_DIR}"
	CreateShortCut "${START_LINK_RUN}" "$InstFolder\VstBoard.exe"
	CreateShortCut "${START_LINK_UNINSTALLER}" "$InstFolder\uninst.exe"
SectionEnd

Section "-VcRedist"
	SetOutPath "$TEMP"
	SetOverwrite on
	File vcredist_${ARCH}.exe
	ExecWait '"$TEMP\vcredist_${ARCH}.exe" /q'
	Delete "$TEMP\vcredist_${ARCH}.exe"
SectionEnd

Section "Uninstall"
        SetShellVarContext current
	
	ReadRegStr $VstDir HKCU "${REG_SETTINGS}" "pluginInstallDir"
	Delete "$VstDir\VstBoardEffect.dll"
	Delete "$VstDir\VstBoardInstrument.dll"
	
	DeleteRegKey HKCU "${REG_UNINSTALL}"
	DeleteRegKey HKCU "${REG_SETTINGS}"
	Delete "$INSTDIR\QtCore4.dll"
	Delete "$INSTDIR\QtGui4.dll"
	Delete "$INSTDIR\QtScript4.dll"
	Delete "$INSTDIR\QtSolutions_MFCMigrationFramework-head.dll"
	Delete "$INSTDIR\VstBoard.exe"
	Delete "$INSTDIR\VstBoardPlugin.dll"
	Delete "$INSTDIR\license.txt"
	Delete "$INSTDIR\GPL.txt"
	Delete "$INSTDIR\LGPL.txt"
	Delete "$INSTDIR\uninst.exe"
	RMDir "$INSTDIR"
	Delete "${START_LINK_RUN}"
	Delete "${START_LINK_UNINSTALLER}"
	RMDir "${START_LINK_DIR}"
SectionEnd

