#pragma once

#include <exception>
#include <string>
#include <sys/socket.h>

namespace socketlab::network
{

    enum class TypeSocket
    {
        BlockingSocket    = 0,
        NonBlockingSocket = 1
    };

    enum class IpVersion 
    {
        IPv4 = AF_INET,
        IPv6 = AF_INET6
    };

    constexpr int to_af(IpVersion v) noexcept 
    {
        return static_cast<int>(v);
    }

    class SocketException : public std::exception 
    {
    public:
        explicit SocketException(const char* message)        noexcept : m_message(message) {}
        explicit SocketException(const std::string& message) noexcept : m_message(message) {}
        virtual ~SocketException() noexcept {}
        const char* what() const noexcept override { return m_message.c_str(); }
    protected:
        std::string m_message{"Socket Error"};
    };

    class SocketBase 
    {
    public:
        void       set_blocking_type(const TypeSocket& t) { socketType = t; }
        TypeSocket get_blocking_type() const              { return socketType; }
        void       set_ip_version(IpVersion v)            { ipVersion = v; }
        IpVersion  get_ip_version() const                 { return ipVersion; }

    protected:
        explicit SocketBase(TypeSocket socketType, IpVersion ipVersion)
            : socketType{socketType}, ipVersion{ipVersion} {}

        TypeSocket socketType;
        IpVersion  ipVersion;
    };

} // namespace socketlab::network
