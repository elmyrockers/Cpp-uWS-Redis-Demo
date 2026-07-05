@echo off
chcp 65001>nul
title C++ WebSocket Server
color 0F
echo.
echo ╔══════════════════════════════════════╗
echo ║        C++ WebSocket Server          ║
echo ╚══════════════════════════════════════╝
echo.
echo 📁 %cd%
echo 🌐 ws://localhost:9001
echo.
echo ▶ Starting...
echo.
wsl bash -c "docker rm -f ws-server 2>/dev/null 1>/dev/null; docker run -it --rm --name ws-server --network host ws-server"
echo.
echo [INFO] Server stopped.
pause