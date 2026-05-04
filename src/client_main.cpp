#include <iostream>
#include <string>
#include <unistd.h>
#include "client.h"

// -------------------------------------------------------
// Usage: ./client [host]
//   host defaults to "server" (Docker Compose service name)
// -------------------------------------------------------

void blocking_example(const std::string& host);
void nonblocking_example(const std::string& host);

int main(int argc, char* argv[]) {
    std::cout << std::unitbuf;  // unbuffered – Docker'da anlık çıktı
    std::string host = (argc > 1) ? argv[1] : "server";
    blocking_example(host);
    return 0;
}

void blocking_example(const std::string& host) {
    // Server'ın listen() durumuna geçmesi için kısa bekle
    sleep(1);
    try {
        socketlab::network::Client client{host, "8080",
            socketlab::network::Client::TypeSocket::BlockingSocket};
        client.connect_socket();
        std::cout << "Connected to " << host << ":8080\n";
        client.receive_until();
    }
    catch (const socketlab::network::ClientException& ex) {
        std::cerr << ex.what() << "\n";
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }
}

void nonblocking_example(const std::string& host) {
    try {
        socketlab::network::Client client{host, "8080",
            socketlab::network::Client::TypeSocket::NonBlockingSocket};
        client.connect_socket();
        std::cout << "Connected (non-blocking) to " << host << ":8080\n";
        client.receive_until();
    }
    catch (const socketlab::network::ClientException& ex) {
        std::cerr << ex.what() << "\n";
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }
}
