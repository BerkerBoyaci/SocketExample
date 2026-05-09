# SocketLab – Docker Compose helper (PowerShell)
# Usage:
#   .\run.ps1                                  # default example (01_hello_server)
#   .\run.ps1 02_echo_server_blocking          # interactive echo example
#   .\run.ps1 01_hello_server down             # stop containers
#
# Interactive examples (e.g. echo) start the server in the background and
# attach the client with a live stdin so you can type messages.

param(
    [string]$Example = "01_hello_server",
    [string]$Action  = "up"
)

$env:EXAMPLE = $Example

# Examples that need interactive stdin on the client side
$interactiveExamples = @("02_echo_server_blocking", "03_echo_server_fork")

switch ($Action) {
    "up" {
        if ($interactiveExamples -contains $Example) {
            Write-Host "Building images..."
            docker compose build
            Write-Host "Starting server in background..."
            docker compose up -d server
            Write-Host "Waiting for server DNS to register..."
            Start-Sleep -Seconds 2
            Write-Host "Attaching interactive client (type messages, empty line to quit)..."
            docker compose run --no-deps --rm client
            Write-Host "Tearing down..."
            docker compose down
        } else {
            docker compose up --build
        }
    }
    "down" { docker compose down }
    default {
        Write-Error "Unknown action '$Action'. Use 'up' or 'down'."
        exit 1
    }
}
