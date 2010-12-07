Name "VstBoard"
RequestExecutionLevel user
OutFile "vstboard_win32_setup.exe"
SetDateSave on
SetDatablockOptimize on
CRCCheck on
;SilentInstall normal
;BGGradient 000000 800000 FFFFFF
;InstallColors FF8080 000030
XPStyle on
InstallDir "$LOCALAPPDATA\VstBoard"

!define START_LINK_DIR "$SMPROGRAMS\VstBoard"
!define START_LINK_RUN "$SMPROGRAMS\VstBoard\VstBoard.lnk"
!define START_LINK_UNINSTALLER "$SMPROGRAMS\VstBoard\Uninstall VstBoard.lnk"
!define REG_UNINSTALL "Software\Microsoft\Windows\CurrentVersion\Uninstall\VstBoard"
!define REG_SETTINGS "Software\CtrlBrk\VstBoard"
!define REG_SETTINGS_PLUGIN "Software\CtrlBrk\VstBoardPlugin"
!define UNINSTALLER_NAME "uninst.exe"
!define WEBSITE_LINK "http://vstboard.blogspot.com/"

LicenseText "License Agreeemt. Please review the license terms before installing VstBoard. If you accept the term of the agreement, click I Agree to continue. You must accept the agreement to install VstBoard"
LicenseData "license.txt"

;--------------------------------

; Pages

Page license
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install

Section "VstBoard (required)"

	SectionIn RO
	SetOutPath $INSTDIR
	File "QtCore4.dll"
	File "QtGui4.dll"
	File "VstBoard.exe"
        File "VstBoardPlugin.dll"
        File "*.qm"
	File "license.txt"
	File "GPL.txt"
	File "LGPL.txt"

	WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayName" "VstBoard"
	WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayIcon" "$\"$INSTDIR\VstBoard.exe$\""
	WriteRegStr HKCU "${REG_UNINSTALL}" "Publisher" "Raphaël François"
	WriteRegStr HKCU "${REG_UNINSTALL}" "DisplayVersion" "a0.0.0.1"
	WriteRegDWord HKCU "${REG_UNINSTALL}" "EstimatedSize" 4500 ;KB
	WriteRegStr HKCU "${REG_UNINSTALL}" "HelpLink" "${WEBSITE_LINK}"
	WriteRegStr HKCU "${REG_UNINSTALL}" "URLInfoAbout" "${WEBSITE_LINK}"
	WriteRegStr HKCU "${REG_UNINSTALL}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKCU "${REG_UNINSTALL}" "InstallSource" "$\"$EXEDIR$\""
	WriteRegDWord HKCU "${REG_UNINSTALL}" "NoModify" 1
	WriteRegDWord HKCU "${REG_UNINSTALL}" "NoRepair" 1
	WriteRegStr HKCU "${REG_UNINSTALL}" "UninstallString" "$\"$INSTDIR\${UNINSTALLER_NAME}$\""
	WriteRegStr HKCU "${REG_UNINSTALL}" "Comments" "Uninstalls VstBoard."

	WriteUninstaller "uninst.exe"

SectionEnd

Section "Start Menu Shortcuts"
	SetShellVarContext current
	CreateDirectory "${START_LINK_DIR}"
	CreateShortCut "${START_LINK_RUN}" "$INSTDIR\VstBoard.exe"
	CreateShortCut "${START_LINK_UNINSTALLER}" "$INSTDIR\uninst.exe"
SectionEnd

Section "Uninstall"
	SetShellVarContext current
	DeleteRegKey HKCU "${REG_UNINSTALL}"
	DeleteRegKey HKCU "${REG_SETTINGS}"
        DeleteRegKey HKCU "${REG_SETTINGS_PLUGIN}"
	Delete "$INSTDIR\QtCore4.dll"
	Delete "$INSTDIR\QtGui4.dll"
	Delete "$INSTDIR\VstBoard.exe"
        Delete "$INSTDIR\VstBoardPlugin.dll"
        Delete "$INSTDIR\*.qm"
	Delete "$INSTDIR\license.txt"
	Delete "$INSTDIR\GPL.txt"
	Delete "$INSTDIR\LGPL.txt"
	Delete "$INSTDIR\uninst.exe"
	RMDir "$INSTDIR"
	Delete "${START_LINK_RUN}"
	Delete "${START_LINK_UNINSTALLER}"
    RMDir "${START_LINK_DIR}"
SectionEnd
