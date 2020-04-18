@echo off
pushd %~dp0\..\
call vendor\binary\premake5.exe vs2019
popd
PAUSE