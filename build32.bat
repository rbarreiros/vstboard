set NSIS_PATH="c:\Program Files (x86)\NSIS\makensis.exe"
set BUILD_PATH=..\vstboard.32\buildall-build-desktop

call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /xp /x86 /Release
call "C:\Qt\4.7.3-32-2008\bin\qtvars.bat"

mkdir "%BUILD_PATH%/installer"
copy /y "*.txt" "%BUILD_PATH%/installer"
copy /y ".\tools\nsis.nsi" "%BUILD_PATH%/installer"

cd "%BUILD_PATH%"
qmake -r C:\Users\CtrlBrk\Documents\VstBoard\src\buildall.pro
nmake clean
del "%BUILD_PATH%\portaudio\release\portaudio.lib"
del "%BUILD_PATH%\portmidi\release\portmidi.lib"
nmake release

pause

copy /y ".\dllLoader\release\vstboard.dll" ".\installer"
copy /y ".\vstboard\release\vstboard.exe" ".\installer"
copy /y ".\vstdll\release\VstBoardPlugin.dll" ".\installer"
copy /y "%QTDIR%\bin\QtCore4.dll" ".\installer"
copy /y "%QTDIR%\bin\QtGui4.dll" ".\installer"
copy /y "%QTDIR%\bin\QtScript4.dll" ".\installer"
copy /y "%QTDIR%\bin\QtSolutions_MFCMigrationFramework-head.dll" ".\installer"
rem copy /y "%WindowsSDKDir%\Redist\VC\vcredist_x86.exe" ".\installer"

set MPRESS_PATH="../../vstboard/tools/mpress/mpress.exe"
%MPRESS_PATH% -q ".\installer\VstBoard.exe"
%MPRESS_PATH% -q ".\installer\VstBoardPlugin.dll"
%MPRESS_PATH% -q ".\installer\VstBoard.dll"
%MPRESS_PATH% -q ".\installer\QtGui4.dll"
%MPRESS_PATH% -q ".\installer\QtCore4.dll"
%MPRESS_PATH% -q ".\installer\QtScript4.dll"
%MPRESS_PATH% -q ".\installer\QtSolutions_MFCMigrationFramework-head.dll"

cd "installer"
%NSIS_PATH% /DARCH=x86 /DOutFile="vstboard_win32_setup.exe" nsis.nsi

pause
