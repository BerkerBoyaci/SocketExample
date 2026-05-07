#include <iostream>
#include <string>
#include <unistd.h>
#include "server.hpp"

// -------------------------------------------------------
// Echo Server (Blocking) – server echoes back every message
// received from the client until the connection is closed.
// -------------------------------------------------------

int main() {
    std::cout << std::unitbuf;  // unbuffered – Docker'da anlık çıktı
    try {
        socketlab::network::Server server(8080, 1,
            socketlab::network::TypeSocket::BlockingSocket);
        server.accept_connections();
        std::cout << "Echo server ready. Waiting for data...\n";

        for (;;) {
            std::string received = server.recv_bytes();
            if (received.empty()) {
                std::cout << "Client disconnected.\n";
                break;
            }
            std::cout << "Echo: " << received;
            server.send_bytes(received);
        }
    }
    catch (const socketlab::network::ServerException& ex) {
        std::cerr << ex.what() << "\n";
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }
    return 0;
}
