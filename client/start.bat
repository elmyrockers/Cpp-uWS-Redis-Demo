@echo off
chcp 65001>nul
title Golang HTTP Server
color 0F
echo.
echo ╔══════════════════════════════════════╗
echo ║          Golang HTTP Server          ║
echo ╚══════════════════════════════════════╝
echo.
echo 📁 %cd%
echo 🌐 http://localhost:3000
echo.
echo ▶ Starting...
echo.
wsl bash -c "docker rm -f http-server 2>/dev/null 1>/dev/null; docker run -it --rm --name http-server --network host http-server"
echo.
echo [INFO] Server stopped.
pause