#include <iostream>
#include <string>
#include <unistd.h>
#include "server.hpp"

// -------------------------------------------------------
// Examples – uncomment the one you want to run in main()
// -------------------------------------------------------

void blocking_example();
void nonblocking_example();

int main() {
    std::cout << std::unitbuf;  // unbuffered – Docker'da anlık çıktı
    blocking_example();
    return 0;
}

// Blocking: accept one client, send a greeting every second
void blocking_example() {
    try {
        socketlab::network::Server server(8080, 1,
            socketlab::network::TypeSocket::BlockingSocket);
        server.accept_connections();

        for (;;) {
            std::string msg = "Hello from SocketLab server!\r\n";
            server.send_bytes(msg);
            sleep(1);
        }
    }
    catch (const socketlab::network::ServerException& ex) {
        std::cerr << ex.what() << "\n";
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }
}

// Non-blocking: same flow with O_NONBLOCK on the accepted socket
void nonblocking_example() {
    try {
        socketlab::network::Server server(8080, 1,
            socketlab::network::TypeSocket::NonBlockingSocket);
        server.accept_connections();

        for (;;) {
            std::string msg = "Hello from SocketLab server (non-blocking)!\r\n";
            server.send_bytes(msg);
            sleep(1);
        }
    }
    catch (const socketlab::network::ServerException& ex) {
        std::cerr << ex.what() << "\n";
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }
}
