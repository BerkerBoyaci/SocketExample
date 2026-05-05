#pragma once

#include <exception>
#include <string>

namespace socketlab::network {

    enum class TypeSocket {
        BlockingSocket    = 0,
        NonBlockingSocket = 1
    };

    class SocketException : public std::exception {
    public:
        explicit SocketException(const char* message)        noexcept : m_message(message) {}
        explicit SocketException(const std::string& message) noexcept : m_message(message) {}
        virtual ~SocketException() noexcept {}
        const char* what() const noexcept override { return m_message.c_str(); }
    protected:
        std::string m_message{"Socket Error"};
    };

} // namespace socketlab::network
