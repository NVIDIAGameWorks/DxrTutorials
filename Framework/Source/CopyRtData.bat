@echo off
rem %1==projectDir %2==outputdir
setlocal

IF not exist %2\Data\ mkdir %2\Data
IF not exist %2\Data\FalcorRT mkdir %2\Data\FalcorRT
copy %1\API\DXR\DxrCommon.hlsli %2\Data\FalcorRT >nul
copy %1\Effects\SVGF\Shaders\*.* %2\Data\FalcorRT>nul