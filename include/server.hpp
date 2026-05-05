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

    class Server
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

        void accept_connections();
        void send_bytes(const std::string& data) const;

        void       set_blocking_type(const TypeSocket& t) { socketType = t; }
        TypeSocket get_blocking_type() const              { return socketType; }
        void       set_ip_version(IpVersion v)            { ipVersion = v; }
        IpVersion  get_ip_version() const                 { return ipVersion; }
        void       set_port(int p)                        { port = p; }
        int        get_port() const                       { return port; }

    protected:
        void bind_socket();
        void listen_socket() const;
        void blocking_mode() const;

    private:
        int             port;
        int             connections;
        int             m_socket;
        int             client_socket;
        TypeSocket      socketType;
        IpVersion       ipVersion;
        sockaddr_storage socketAddress{};
    };

} // namespace socketlab::network
