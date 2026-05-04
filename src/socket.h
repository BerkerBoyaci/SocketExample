#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>

namespace socketlab::network {

    // Base POSIX socket wrapper (learning reference)
    class Socket {
    public:
        explicit Socket(const Socket&) = delete;
        explicit Socket(Socket&&)      = delete;
        Socket& operator=(const Socket&) = delete;
        Socket  operator=(Socket&&)      = delete;
        virtual ~Socket();
    protected:
        enum class TypeSocket 
        {
            BlockingSocket,
            NonBlockingSocket 
        };
        
        explicit Socket();
        virtual void close_socket();
    protected:
        int      m_socket;
        addrinfo address{};
    };

} // namespace socketlab::network
