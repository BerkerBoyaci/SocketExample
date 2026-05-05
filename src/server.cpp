#include "server.hpp"
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>

namespace socketlab::network {

    Server::Server(int port, int connections, TypeSocket socketType, IpVersion ipVersion)
        : port{port}, 
          connections{connections},
          socketType{socketType},
          ipVersion{ipVersion},
          m_socket{-1},
          client_socket{-1}
    {
        m_socket = socket(to_af(ipVersion), SOCK_STREAM, IPPROTO_TCP);
        if (m_socket == -1)
            throw ServerException{"Error: Socket creation failed."};

        // Allow port reuse to avoid "Address already in use" on restart
        int opt = 1;
        setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        memset(&socketAddress, 0, sizeof(socketAddress));
        if (ipVersion == IpVersion::IPv6) {
            auto& addr6       = reinterpret_cast<sockaddr_in6&>(socketAddress);
            addr6.sin6_family = AF_INET6;
            addr6.sin6_addr   = in6addr_any;
            addr6.sin6_port   = htons(port);
        } else {
            auto& addr4           = reinterpret_cast<sockaddr_in&>(socketAddress);
            addr4.sin_family      = AF_INET;
            addr4.sin_addr.s_addr = INADDR_ANY;
            addr4.sin_port        = htons(port);
        }

        bind_socket();
    }

    Server::~Server()
    {
        if (client_socket != -1) 
        {
            shutdown(client_socket, SHUT_WR);
            ::close(client_socket);
        }
        if (m_socket != -1)
            ::close(m_socket);
    }

    void Server::bind_socket() 
    {
        socklen_t addrLen = (ipVersion == IpVersion::IPv6)
            ? static_cast<socklen_t>(sizeof(sockaddr_in6))
            : static_cast<socklen_t>(sizeof(sockaddr_in));
        if (bind(m_socket, reinterpret_cast<sockaddr*>(&socketAddress), addrLen) == -1) 
        {
            ::close(m_socket);
            throw ServerException{"Error: bind() failed."};
        }
    }

    void Server::listen_socket() const 
    {
        if (listen(m_socket, connections) == -1)
            throw ServerException{"Error: listen() failed."};
    }

    void Server::accept_connections() 
    {
        listen_socket();
        client_socket = -1;
        while (client_socket == -1)
        {
            std::cout << "Waiting for incoming connections...\n";
            client_socket = accept(m_socket, nullptr, nullptr);
        }
        blocking_mode();
        std::cout << "Client connected!\n";
    }

    void Server::blocking_mode() const 
    {
        int flags = fcntl(client_socket, F_GETFL, 0);
        if (socketType == TypeSocket::NonBlockingSocket)
            fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
        else
            fcntl(client_socket, F_SETFL, flags & ~O_NONBLOCK);
    }

    void Server::send_bytes(const std::string& data) const
    {
        send(client_socket, data.c_str(), data.length(), 0);
    }

} // namespace socketlab::network
