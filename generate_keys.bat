@echo off
chcp 65001>nul
echo.
echo ╔══════════════════════════════════════╗
echo ║          Generate JWT Keys           ║
echo ╚══════════════════════════════════════╝
echo.
echo ▶ Generating RSA key pair...
echo.
wsl bash -c "cd $(wslpath '%cd%') && mkdir -p server/keys client/keys && openssl genrsa -out server/keys/private.pem 2048 && openssl rsa -in server/keys/private.pem -pubout -out server/keys/public.pem && cp server/keys/private.pem client/keys/private.pem && cp server/keys/public.pem client/keys/public.pem"
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Failed to generate keys. Make sure openssl is installed in WSL.
) else (
    echo.
    echo [SUCCESS] Keys generated:
    echo    server/keys/private.pem
    echo    server/keys/public.pem
    echo    client/keys/private.pem
    echo    client/keys/public.pem
)
pause