#include "socket.h"


namespace aricanli::network {
	Socket::Socket() : m_socket{ 0 } {
		start();
		m_socket = socket(AF_INET, SOCK_STREAM, 0);

		if (m_socket == INVALID_SOCKET)
			throw "INVALID SOCKET";
	}
	/*
	Socket::Socket(SOCKET _socket) :m_socket{ _socket } {
		start();
	}
	*/

	void Socket::start() {
		if (WSAStartup(MAKEWORD(2, 2), &wsa))
			throw "Could not start.";
	}

	void Socket::close() {
		closesocket(m_socket);
	 }

	void Socket::end() {
		WSACleanup();
	}

	Socket::~Socket() {
		end();
		close();
	}
}