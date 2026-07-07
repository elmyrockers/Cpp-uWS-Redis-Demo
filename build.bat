@echo off
chcp 65001>nul
title Cpp uWS Redis Demo - Build
echo.
echo ╔══════════════════════════════════════╗
echo ║   Cpp uWS Redis Demo - Building...   ║
echo ╚══════════════════════════════════════╝
echo.
wsl bash -c "cd $(wslpath '%cd%') && docker compose up --build"
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed. Check the output above.
)
pause