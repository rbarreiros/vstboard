Name "VstBoard"
OutFile "VstBoard_Setup.exe"
SetDateSave on
SetDatablockOptimize on
CRCCheck on
;SilentInstall normal
;BGGradient 000000 800000 FFFFFF
;InstallColors FF8080 000030
XPStyle on
InstallDir "$PROGRAMFILES\VstBoard"
InstallDirRegKey HKLM "Software\CtrlBrk\VstBoard\" "Install_Dir"

LicenseText "License Agreeemt. Please review the license terms before installing VstBoard. If you accept the term of the agreement, click I Agree to continue. You must accept the agreement to install VstBoard"
LicenseData "license.txt"

RequestExecutionLevel admin

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
File "libgcc_s_dw2-1.dll"
File "mingwm10.dll"
File "QtCore4.dll"
File "QtGui4.dll"
File "VstBoard.exe"


;AutoCloseWindow false
;ShowInstDetails show

WriteRegStr HKLM SOFTWARE\CtrlBrk\VstBoard "Install_Dir" "$INSTDIR"

; write uninstall strings
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\VstBoard" "DisplayName" "VstBoard"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\VstBoard" "UninstallString" '"$INSTDIR\uninst.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Example2" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Example2" "NoRepair" 1
  WriteUninstaller "uninst.exe"

SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

CreateDirectory "$SMPROGRAMS\VstBoard"
CreateShortCut "$SMPROGRAMS\VstBoard\Uninstall VstBoard.lnk" "$INSTDIR\uninst.exe"
CreateShortCut "$SMPROGRAMS\VstBoard\VstBoard.lnk" "$INSTDIR\VstBoard.exe"

SectionEnd

;--------------------------------

; Uninstaller


UninstallText "This will uninstall VstBoard. Hit next to continue."
UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\nsis1-uninstall.ico"


Section "Uninstall"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\VstBoard"
  DeleteRegKey HKLM "SOFTWARE\CtrlBrk\VstBoard"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\VstBoard.exe"
  Delete "$INSTDIR\uninst.exe"
  Delete "$SMPROGRAMS\VstBoard\*.*"
  RMDir "$SMPROGRAMS\VstBoard"
  RMDir "$INSTDIR"

  IfFileExists "$INSTDIR" 0 NoErrorMsg
    MessageBox MB_OK "Note: $INSTDIR could not be removed!" IDOK 0 ; skipped if file doesn't exist
  NoErrorMsg:

SectionEnd