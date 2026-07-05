@echo off
chcp 65001>nul
title Golang WebSocket Client
color 0F
echo.
echo ╔══════════════════════════════════════╗
echo ║       Golang WebSocket Client        ║
echo ╚══════════════════════════════════════╝
echo.
echo 📁 %cd%
echo 🌐 http://localhost:3000
echo.
echo ▶ Starting...
echo.
wsl bash -c "docker rm -f ws-client 2>/dev/null 1>/dev/null; docker run -it --rm --name ws-client --network host ws-client"
echo.
echo [INFO] Server stopped.
pause