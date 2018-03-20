@set PM_DISABLE_VS_WARNING=true
@call "%~dp0packman\packman.cmd " pull "%~dp0dependencies.xml" --platform win
@if errorlevel 1 exit /b 1
