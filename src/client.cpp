#include "client.hpp"
#include <cstring>
#include <fcntl.h>

namespace socketlab::network
 {

    Client::Client(const std::string& host, const std::string& port, TypeSocket socketType, IpVersion ipVersion)
        : host{host}, port{port}, socketType{socketType}, ipVersion{ipVersion}, m_socket{-1}
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

    void Client::send_line(const std::string& data) const 
    {
        send(m_socket, data.c_str(), data.length(), 0);
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
        // Interactive send/receive loop
        std::string userInput;
        do 
        {
            std::cout << " > ";
            std::getline(std::cin, userInput);
            if (!userInput.empty()) {
                send(m_socket, userInput.c_str(), userInput.size(), 0);
                memset(buffer, 0, sizeof(buffer));
                int bytesReceived = recv(m_socket, buffer, sizeof(buffer), 0);
                if (bytesReceived > 0)
                    std::cout << "Server> " << std::string(buffer, 0, bytesReceived) << "\n";
            }
        } while (!userInput.empty());
    }

} // namespace socketlab::network
