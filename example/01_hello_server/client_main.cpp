#include <iostream>
#include <string>
#include <span>
#include <array>
#include <cstddef>
#include <unistd.h>
#include "client.hpp"

// -------------------------------------------------------
// Hello Server – client connects and receives greetings
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
            socketlab::network::TypeSocket::BlockingSocket};
        client.connect_socket();
        std::cout << "Connected to " << host << ":8080\n";

        // send_raw: std::string → std::span<const std::byte>
        std::string msg = "Hello from client!\r\n";
        client.send_raw(std::as_bytes(std::span{msg}));

        // send_raw: binary buffer
        std::array<std::byte, 4> ping{std::byte{0x50}, std::byte{0x49}, std::byte{0x4E}, std::byte{0x47}};
        client.send_raw(std::span{ping});

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
            socketlab::network::TypeSocket::NonBlockingSocket};
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
