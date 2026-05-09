@echo off
REM SocketLab – Docker Compose helper (CMD)
REM Usage:
REM   run.bat                               -> default example (01_hello_server)
REM   run.bat 02_echo_server_blocking       -> interactive echo example
REM   run.bat 02_echo_server_blocking down  -> stop containers
REM
REM Interactive examples (e.g. echo) start the server in the background and
REM attach the client with a live stdin so you can type messages.

SET EXAMPLE=%~1
IF "%EXAMPLE%"=="" SET EXAMPLE=01_hello_server

SET ACTION=%~2
IF "%ACTION%"=="" SET ACTION=up

IF /I "%ACTION%"=="down" (
    docker compose down
    GOTO :EOF
)

IF /I "%ACTION%"=="up" (
    IF "%EXAMPLE%"=="02_echo_server_blocking" GOTO :interactive
    IF "%EXAMPLE%"=="03_echo_server_fork"      GOTO :interactive
    docker compose up --build
    GOTO :EOF
)

:interactive
echo Building images...
docker compose build
echo Starting server in background...
docker compose up -d server
echo Waiting for server DNS to register...
timeout /t 2 /nobreak >nul
echo Attaching interactive client ^(type messages, empty line to quit^)...
docker compose run --no-deps --rm client
echo Tearing down...
docker compose down
GOTO :EOF

echo Unknown action "%ACTION%". Use "up" or "down".
exit /b 1
