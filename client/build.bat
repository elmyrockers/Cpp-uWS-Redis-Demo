@echo off
setlocal
echo [WSL] Checking for existing image: http-server...

:: Check if the image exists in Docker
wsl bash -c "docker images -q http-server | grep -q ."
if %errorlevel% equ 0 (
    echo [SKIP] Image 'http-server' already exists.
    echo [INFO] Run 'start.bat' to start the container.
    pause
    exit /b
)

echo [STATUS] Image not found. Starting build...

:: Execute the build inside WSL
wsl bash -c "cd $(wslpath '%cd%') && docker build -f dockerfile -t http-server ."
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed. Check the output above.
    pause
    exit /b
)

echo [SUCCESS] Build complete. You can now use start.bat.
pause