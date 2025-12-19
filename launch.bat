@echo off
echo NIKITA GAME LAUNCHER
echo.
if exist "compile.bat" (
    echo Running compile.bat...
    call compile.bat
) else (
    echo Files in this game:
    dir
    echo.
    echo No compile.bat found
)
pause