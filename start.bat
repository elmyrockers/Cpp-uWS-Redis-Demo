@echo off
chcp 65001>nul
title Cpp uWS Redis Demo
color 0F
echo.
echo ╔══════════════════════════════════════╗
echo ║        Cpp uWS Redis Demo            ║
echo ╚══════════════════════════════════════╝
echo.
echo 📁 %cd%
echo 🌐 http://localhost:3000
echo 🔌 ws://localhost:9001
echo.
echo ▶ Starting...
echo.
wsl bash -c "cd $(wslpath '%cd%') && docker-compose up --build"
echo.
echo [INFO] Stopped.
pause