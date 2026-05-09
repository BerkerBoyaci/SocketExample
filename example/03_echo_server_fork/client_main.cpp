#include <iostream>
#include <string>
#include <unistd.h>
#include "client.hpp"

int main(int argc, char* argv[])
{
    std::cout << std::unitbuf;
    std::string host = (argc > 1) ? argv[1] : "server";

    sleep(1);
    try {
        socketlab::network::Client client{host, "8080",
            socketlab::network::TypeSocket::BlockingSocket};
        client.connect_socket();
        std::cout << "Connected to " << host << ":8080 (fork echo mode)\n";
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
