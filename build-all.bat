@echo off
wsl.exe ./scripts/build.sh Debug
wsl.exe ./scripts/build.sh Release
wsl.exe ./scripts/iso.sh
pause