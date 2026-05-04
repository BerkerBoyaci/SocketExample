#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <iostream>

namespace socketlab::network
{

    class ClientException : public std::exception 
    {
    public:
        explicit ClientException(const char* message)        noexcept : m_message(message) {}
        explicit ClientException(const std::string& message) noexcept : m_message(message) {}
        virtual ~ClientException() noexcept {}
        const char* what() const noexcept override { return m_message.c_str(); }
    protected:
        std::string m_message{"Invalid Client Operation"};
    };

    class Client 
    {
    public:
        enum class TypeSocket
        {
             BlockingSocket = 0,
              NonBlockingSocket = 1 
        };

        Client(const std::string& host, const std::string& port,
               TypeSocket socketType = TypeSocket::BlockingSocket);
        Client(const Client&) = delete;
        Client(Client&&)      = delete;
        Client& operator=(const Client&) = delete;
        Client  operator=(Client&&)      = delete;
        ~Client();

        void        connect_socket();
        void        send_line(const std::string& data) const;
        void        receive_until() const;
        void        receive_echo();

        void        set_blocking_type(const TypeSocket& t) { socketType = t; }
        TypeSocket  get_blocking_type() const              { return socketType; }
        void        set_port(const std::string& p)         { port = p; }
        std::string get_port() const                       { return port; }
        void        set_host(const std::string& h)         { host = h; }
        std::string get_host() const                       { return host; }

    protected:
        void        blocking_mode();

    private:
        std::string     host;
        std::string     port;
        TypeSocket      socketType;
        addrinfo        hints{};
        addrinfo*       addrs{nullptr};
        int             m_socket;
        mutable char    buffer[4096]{};
        mutable int     iResult{};
    };

} // namespace socketlab::network
