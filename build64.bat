set NSIS_PATH="c:\Program Files (x86)\NSIS\makensis.exe"
set BUILD_PATH=..\vstboard.64\buildall-build-desktop

call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /xp /x64 /Release
call "C:\Qt\4.7.3-64-2010\bin\qtvars.bat"

mkdir "%BUILD_PATH%/installer"
copy /y "*.txt" "%BUILD_PATH%/installer"
copy /y ".\tools\nsis.nsi" "%BUILD_PATH%/installer"

cd "%BUILD_PATH%"
rem qmake -r C:\Users\CtrlBrk\Documents\VstBoard\src\buildall.pro
rem nmake clean
rem nmake release

pause

copy /y ".\dllLoader\release\vstboard.dll" ".\installer"
copy /y ".\vstboard\release\vstboard.exe" ".\installer"
copy /y ".\vstdll\release\VstBoardPlugin.dll" ".\installer"
copy /y "%QTDIR%\bin\QtCore4.dll" ".\installer"
copy /y "%QTDIR%\bin\QtGui4.dll" ".\installer"
copy /y "%QTDIR%\bin\QtScript4.dll" ".\installer"
copy /y "%QTDIR%\bin\QtSolutions_MFCMigrationFramework-head.dll" ".\installer"
copy /y "%WindowsSDKDir%\Redist\VC\vcredist_x64.exe" ".\installer"

rem set MPRESS_PATH="../../vstboard/tools/mpress/mpress.exe"
rem %MPRESS_PATH% -q ".\installer\VstBoard.exe"
rem %MPRESS_PATH% -q ".\installer\VstBoardPlugin.dll"
rem %MPRESS_PATH% -q ".\installer\VstBoard.dll"
rem %MPRESS_PATH% -q ".\installer\QtGui4.dll"
rem %MPRESS_PATH% -q ".\installer\QtCore4.dll"
rem %MPRESS_PATH% -q ".\installer\QtScript4.dll"
rem %MPRESS_PATH% -q ".\installer\QtSolutions_MFCMigrationFramework-head.dll"

cd "installer"
%NSIS_PATH% /DARCH=x64 /DOutFile="vstboard_win64_setup.exe" nsis.nsi

pause
