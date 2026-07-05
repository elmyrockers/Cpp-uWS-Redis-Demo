@echo off
setlocal
echo [WSL] Checking for existing image: ws-client...

:: Check if the image exists in Docker
wsl bash -c "docker images -q ws-client | grep -q ."
if %errorlevel% equ 0 (
    echo [SKIP] Image 'ws-client' already exists.
    echo [INFO] Run 'start.bat' to start the container.
    pause
    exit /b
)

echo [STATUS] Image not found. Starting build...

:: Execute the build inside WSL
wsl bash -c "cd $(wslpath '%cd%') && docker build -f dockerfile -t ws-client ."
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed. Check the output above.
    pause
    exit /b
)

echo [SUCCESS] Build complete. You can now use start.bat.
pause