#include "socket.hpp"
#include <stdexcept>

namespace socketlab::network {

    Socket::Socket() : m_socket{-1} {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket == -1)
            throw std::runtime_error("Socket creation failed.");
    }

    void Socket::close_socket() {
        if (m_socket != -1) {
            ::close(m_socket);
            m_socket = -1;
        }
    }

    Socket::~Socket() {
        close_socket();
    }

} // namespace socketlab::network
