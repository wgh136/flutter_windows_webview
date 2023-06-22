@echo off
set VSCMD_START_DIR=.
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
lib /machine:"amd64" %*
