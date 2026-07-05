@echo off
wsl bash -c "cd $(wslpath '%cd%') && docker-compose down"