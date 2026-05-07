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
$interactiveExamples = @("02_echo_server_blocking")

switch ($Action) {
    "up" {
        if ($interactiveExamples -contains $Example) {
            Write-Host "Starting server in background..."
            docker compose up --build -d server
            Write-Host "Attaching interactive client (type messages, empty line to quit)..."
            docker compose run --build --rm client
            Write-Host "Stopping server..."
            docker compose stop server
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
