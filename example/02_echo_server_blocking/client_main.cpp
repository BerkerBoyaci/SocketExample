#include <iostream>
#include <string>
#include <unistd.h>
#include "client.hpp"

// -------------------------------------------------------
// Echo Server (Blocking) – interactive client
// Usage: ./client [host]
//   host defaults to "server" (Docker Compose service name)
//
// Type a message and press Enter – the server will echo it back.
// Exit: empty line | /exit | /quit | Ctrl+D (Linux) | Ctrl+Z then Enter (Windows)
// -------------------------------------------------------

int main(int argc, char* argv[]) {
    std::cout << std::unitbuf;  // unbuffered – Docker'da anlık çıktı
    std::string host = (argc > 1) ? argv[1] : "server";

    // Server'ın listen() durumuna geçmesi için kısa bekle
    sleep(1);
    try {
        socketlab::network::Client client{host, "8080",
            socketlab::network::TypeSocket::BlockingSocket};
        client.connect_socket();
        std::cout << "Connected to " << host << ":8080 (echo mode)\n";
        std::cout << "Type a message and press Enter.\n";
        std::cout << "Exit: empty line | /exit | /quit | Ctrl+D\n";

        client.receive_echo();
    }
    catch (const socketlab::network::ClientException& ex) {
        std::cerr << ex.what() << "\n";
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }
    return 0;
}
