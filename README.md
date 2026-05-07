# SocketLab

A C++20 laboratory project for exploring POSIX TCP socket programming.  
All platform code targets Linux; there are no Winsock dependencies. The project runs either natively on Linux or inside Docker (recommended for Windows/macOS hosts).

---

## Repository Layout

```
SocketLab/
├── include/
│   ├── common.hpp          # SocketBase, TypeSocket, IpVersion, SocketException
│   ├── server.hpp          # Server class declaration
│   ├── client.hpp          # Client class declaration
│   └── socket.hpp          # Shared socket utilities
├── src/
│   ├── socket.cpp
│   ├── server.cpp
│   └── client.cpp
├── example/
│   ├── 01_hello_server/    # Server pushes greeting messages; client receives them
│   │   ├── server_main.cpp
│   │   └── client_main.cpp
│   └── 02_echo_server_blocking/   # Server echoes every message back to the client
│       ├── server_main.cpp
│       └── client_main.cpp
├── Makefile
├── Dockerfile
├── docker-compose.yml
├── run.bat                 # Windows CMD launcher
├── run.ps1                 # PowerShell launcher
└── README.md
```

---

## Build System

The Makefile exposes an `EXAMPLE` variable that selects which subdirectory under `example/` is compiled.

```
EXAMPLE   default: 01_hello_server
```

Compilation produces two binaries under `build/`:

| Target | Binary |
|--------|--------|
| `make server` | `build/server` |
| `make client` | `build/client` |
| `make` / `make all` | both |
| `make clean` | removes `build/` |

Build is performed inside the Docker `builder` stage (gcc 13, C++20). Native builds on Linux require GCC ≥ 13 with C++20 support.

```bash
# Linux – native
make EXAMPLE=02_echo_server_blocking
./build/server &
./build/client localhost
```

---

## Running with Docker

### Helper scripts (Windows)

```cmd
rem CMD
run.bat                              # 01_hello_server (default)
run.bat 02_echo_server_blocking      # Echo Server – interactive session
run.bat 02_echo_server_blocking down # Stop and remove containers
```

```powershell
# PowerShell
.\run.ps1                              # 01_hello_server (default)
.\run.ps1 02_echo_server_blocking      # Echo Server – interactive session
.\run.ps1 02_echo_server_blocking down # Stop and remove containers
```

### Manual Docker commands

```bash
# Non-interactive examples (e.g. 01_hello_server)
EXAMPLE=01_hello_server docker compose up --build      # Linux/macOS
set EXAMPLE=01_hello_server && docker compose up --build  # CMD

# Interactive examples (e.g. 02_echo_server_blocking)
# The client requires a live stdin, so it must be launched with `docker compose run`
# instead of `docker compose up`.
docker compose up --build -d server
docker compose run --rm client
docker compose stop server
```

`docker compose up` multiplexes stdout from all services and cannot attach stdin to a single container. `docker compose run` allocates a dedicated TTY for the container, enabling interactive input.

### Teardown

```bash
docker compose down            # stop and remove containers
docker compose down --rmi all  # also remove built images
```

---

## Class Reference

### `SocketBase` (`include/common.hpp`)

Base class for both `Server` and `Client`. Stores `TypeSocket` and `IpVersion` state.

| Member | Description |
|--------|-------------|
| `TypeSocket::BlockingSocket` | `fcntl` clears `O_NONBLOCK` on the socket fd |
| `TypeSocket::NonBlockingSocket` | `fcntl` sets `O_NONBLOCK` on the socket fd |
| `IpVersion::IPv4` / `IpVersion::IPv6` | Controls `AF_INET` vs `AF_INET6` in all `socket()` calls |

### `Server` (`include/server.hpp`)

Wraps the server-side lifecycle: `socket` → `setsockopt(SO_REUSEADDR)` → `bind` → `listen` → `accept`.  
Supports a single concurrent client connection.

| Method | Description |
|--------|-------------|
| `Server(port, backlog, type, ip)` | Creates and binds the listening socket |
| `accept_connections()` | Blocks until one client connects; applies blocking mode to the accepted fd |
| `send_bytes(const std::string&)` | `send()` to the accepted client socket |
| `recv_bytes()` | `recv()` from the accepted client socket; returns empty string on EOF |

### `Client` (`include/client.hpp`)

Wraps the client-side lifecycle: `socket` → `getaddrinfo` → `connect`.

| Method | Description |
|--------|-------------|
| `Client(host, port, type, ip)` | Creates the socket and resolves the remote address via `getaddrinfo` |
| `connect_socket()` | Iterates `addrinfo` results and calls `connect()`; throws `ClientException` on failure |
| `send_raw(std::span<const std::byte>)` | Performs a complete `send()` loop until all bytes are written |
| `receive_until()` | `recv()` loop until the remote closes the connection |
| `receive_echo()` | Interactive send/receive loop driven by `std::getline`; exits on empty input |

---

## Examples

### 01 – Hello Server

The server sends a `"Hello from SocketLab server!\r\n"` message once per second indefinitely. The client connects, optionally sends a binary payload, then enters `receive_until()`.

Demonstrates: `accept` → `send` loop; raw byte transmission from the client side.

### 02 – Echo Server (Blocking)

The server reads each message from the client via `recv_bytes()` and immediately writes it back with `send_bytes()`. The client uses `receive_echo()` for a line-oriented interactive session.

Demonstrates: full-duplex exchange over a single blocking TCP connection; clean EOF detection.

---

## Learning Path

1. `include/common.hpp` — examine `SocketBase`, `TypeSocket`, and the `SocketException` hierarchy  
2. `src/server.cpp` — trace the `bind` → `listen` → `accept` → `blocking_mode` call sequence  
3. `src/client.cpp` — trace `getaddrinfo` → `connect` → `send_raw` / `receive_until`  
4. `example/01_hello_server` — observe unidirectional streaming over TCP  
5. `example/02_echo_server_blocking` — observe full-duplex exchange and EOF handling  
6. `docker-compose.yml` — inspect how two containers communicate over a user-defined bridge network
