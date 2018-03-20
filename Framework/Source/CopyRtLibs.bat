::@echo off
rem %1==config %2==platformname %3==outputdir
setlocal

rem set some local variables
SET CONFIG=%1
SET PLATFORM=%2

rem copy libraries
if not exist %3 mkdir %3
SET FALCORRT_PROJECT_DIR=%~dp0

robocopy %FALCOR_DXR_DIR%\bin\x64 %3  *.dll /r:0 >nul
robocopy %FALCOR_DXR_DIR%\tools\x64\ %3  *.dll /r:0 >nul
robocopy %FALCOR_DXR_DIR%\dxcompiler\RelWithDebInfo\bin\ %3  *.dll /r:0 >nul

rem call CopyData
copy /y %FALCORRT_PROJECT_DIR%\CopyRtData.bat %3 >nul
call %FALCORRT_PROJECT_DIR%\CopyRtData.bat %FALCORRT_PROJECT_DIR%\..\Source %3