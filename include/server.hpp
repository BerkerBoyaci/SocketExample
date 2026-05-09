#pragma once

#include "common.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <iostream>

namespace socketlab::network {

    class ServerException : public SocketException {
    public:
        using SocketException::SocketException;
    };

    class Server : public SocketBase
    {
    public:
        Server(int port, int connections,
               TypeSocket socketType = TypeSocket::BlockingSocket,
               IpVersion  ipVersion  = IpVersion::IPv4);
        Server(const Server&) = delete;
        Server(Server&&)      = delete;
        Server& operator=(const Server&) = delete;
        Server  operator=(Server&&)      = delete;
        ~Server();

        void        accept_connections();
        void        send_bytes(const std::string& data) const;
        std::string recv_bytes() const;

        // Low-level primitives for multi-client examples (fork, select, epoll, …)
        int  get_fd()  const { return m_socket; }
        void start_listening() const { listen_socket(); }
        int  accept_one();
        int  accept_one(sockaddr_storage& addr, socklen_t& len);
        static void        send_to(int fd, const std::string& data);
        static std::string recv_from(int fd);
        static void        set_fd_blocking(int fd, TypeSocket type);

        void set_port(int p)   { port = p; }
        int  get_port() const  { return port; }

    protected:
        void bind_socket();
        void listen_socket() const;
        void blocking_mode() const;

    private:
        int             port;
        int             connections;
        int             m_socket;
        int             client_socket;
        sockaddr_storage socketAddress{};
    };

} // namespace socketlab::network
