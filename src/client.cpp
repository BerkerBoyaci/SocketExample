#include "client.hpp"
#include <cstring>
#include <fcntl.h>

namespace socketlab::network
 {

    Client::Client(const std::string& host, const std::string& port, TypeSocket socketType, IpVersion ipVersion)
        : SocketBase{socketType, ipVersion},
          host{host}, port{port}, m_socket{-1}
    {
        m_socket = socket(to_af(ipVersion), SOCK_STREAM, IPPROTO_TCP);
        if (m_socket == -1)
            throw ClientException{"Socket creation failed."};

        memset(&hints, 0, sizeof(hints));
        hints.ai_family   = to_af(ipVersion);
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
    }

    Client::~Client() 
    {
        if (addrs)
            freeaddrinfo(addrs);
        if (m_socket != -1) 
        {
            shutdown(m_socket, SHUT_WR);
            ::close(m_socket);
        }
    }

    void Client::blocking_mode() 
    {
        int flags = fcntl(m_socket, F_GETFL, 0);
        if (socketType == TypeSocket::NonBlockingSocket)
            fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
        else
            fcntl(m_socket, F_SETFL, flags & ~O_NONBLOCK);
    }

    void Client::connect_socket()
    {
        blocking_mode();
        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &addrs) != 0)
            throw ClientException{"getaddrinfo() failed."};

        bool connected = false;
        for (addrinfo* addr = addrs; addr != nullptr; addr = addr->ai_next) 
        {
            if (connect(m_socket, addr->ai_addr, addr->ai_addrlen) == 0)
            {
                connected = true;
                break;
            }
        }
        if (!connected)
            throw ClientException{"Could not connect to server."};
    }

    void Client::send_raw(std::span<const std::byte> data) const
    {
        const std::byte* ptr = data.data();
        std::size_t remaining = data.size();
        while (remaining > 0)
        {
            ssize_t sent = ::send(m_socket, ptr, remaining, 0);
            if (sent <= 0) throw ClientException{"send() failed."};
            ptr       += sent;
            remaining -= static_cast<std::size_t>(sent);
        }
    }

    void Client::receive_until() const
    {
        do
        {
            iResult = recv(m_socket, buffer, sizeof(buffer), 0);
            if (iResult > 0)
                std::cout << "Bytes received : " << iResult << "\n";
            else if (iResult == 0)
                std::cout << "Connection closed.\n";
            else
                throw ClientException{"recv() failed in receive_until()."};
        } while (iResult > 0);
    }

    void Client::receive_echo() 
    {
        // Interactive send/receive loop.
        // Exit conditions: empty line, "/exit", "/quit", or EOF (Ctrl+D / Ctrl+Z).
        std::string userInput;
        for (;;)
        {
            std::cout << " > ";
            if (!std::getline(std::cin, userInput)) {
                // EOF – Ctrl+D (Linux) / Ctrl+Z (Windows)
                std::cout << "\n";
                break;
            }
            if (userInput.empty() ||
                userInput == "/exit" ||
                userInput == "/quit")
                break;
            send(m_socket, userInput.c_str(), userInput.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(m_socket, buffer, sizeof(buffer), 0);
            if (bytesReceived > 0)
                std::cout << "Server> " << std::string(buffer, 0, bytesReceived) << "\n";
            else
                break; // server closed the connection
        }
    }

} // namespace socketlab::network
