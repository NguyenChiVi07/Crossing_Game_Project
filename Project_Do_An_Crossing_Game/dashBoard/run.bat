@echo off
echo [Building] Compiling source files...

:: Lenh gom va bien dich code
g++ mainDashboard.cpp dashBoard.cpp -o dashboard_app.exe -lsfml-graphics -lsfml-window -lsfml-system

:: Kiem tra loi
if %errorlevel% neq 0 (
    echo [Error] Compilation failed! Please check the syntax.
    pause
    exit /b %errorlevel%
)

echo [Success] Compilation finished. Launching Dashboard...
dashboard_app.exe