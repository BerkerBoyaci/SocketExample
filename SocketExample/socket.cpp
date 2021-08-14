#include "socket.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include "Client.h"

namespace aricanli::network {
	bool Socket::m_isInitWSA = false;

	Socket::Socket(const std::string& ip, const std::string& port, int connections, TypeSocket socketType) :
	ip{ ip }, port{ port }, connections{ connections }, socketType{ socketType }
	 {
		// Bind the ip address and port to a socket
		
		memset(&socketAddress, 0, sizeof(socketAddress));
		socketAddress.sin_family = PF_INET;
		socketAddress.sin_addr.s_addr = INADDR_ANY;
		inet_pton(AF_INET, ip.c_str(), &socketAddress.sin_addr.s_addr);
		socketAddress.sin_port = htons(std::stoi(port));

		// Create socket
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
			throw SocketException{ "Socket creation failed." };
	}
	Socket::~Socket() {
		closesocket(m_socket);
	}
	bool Socket::startup() {
		if (!m_isInitWSA) {
			WSADATA wsa;
			if (WSAStartup(MAKEWORD(2, 2), &wsa))
				throw "Could not start.";
		}
	}
	void Socket::end() {
		WSACleanup();
	}
	void Socket::blocking_mode() {
		// Enable non-blocking or blocking mode 
		if (socketType == TypeSocket::BlockingSocket) {
			u_long sType = 0;
			ioctlsocket(m_socket, FIONBIO, &sType);
		}
		if (socketType == TypeSocket::NonBlockingSocket) {
			u_long sType = 1;
			ioctlsocket(m_socket, FIONBIO, &sType);
		}
	}

	void Socket::listen_socket() const {
		//Tell winsock the socket is for listening
		auto lst = listen(m_socket, connections);
		if (lst == SOCKET_ERROR)
			throw SocketException{"Socket error in listen_socket();\n"};
	}

	void Socket::send_line(const std::string& send_lines) const {
		auto error = send(m_socket, send_lines.c_str(), send_lines.length(), 0);
		if (error == SOCKET_ERROR)
			std::cout << "Error in send_line()" << WSAGetLastError << "\n";
	}
	void Socket::receive_until() const {
		// Receive until the peer closes the connection
		do {
			auto iResult = recv(m_socket, buffer, 4096, 0);
			if (iResult > 0) {
				std::cout << "Bytes received: \n" << iResult << "\n";
			}
			else if (iResult == 0)
				std::cout << "Connection closed\n";
			else
				throw SocketException{ "Error: receive failed in receive_until()" };

		} while (iResult > 0);
	}

	std::string Socket::receive_bytes() {
		std::string receivedBytes;
		while (true) {
			u_long arg = 0;
			if (ioctlsocket(m_socket, FIONREAD, &arg) != 0)
				break;
			if (arg == 0)
				break;
			if (arg > 4096)
				arg = 4096;

			int received = recv(m_socket, buffer, arg, 0);
			if (received <= 0)
				break;

			std::string tmp;
			tmp.assign(buffer, received);
			receivedBytes += tmp;
		}
		return receivedBytes;
	}

	// Server create
	void Socket::bind_socket() {
		auto tmp_socket = bind(m_socket, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(sockaddr_in));
		if (tmp_socket == SOCKET_ERROR) {
			throw SocketException{ "Error : socket can not bind in bind_socket()" };
		}
		//m_socket = tmp_socket;
	}

	void Socket::accept_connections() {
		//listen_socket();
		// Wait for a connection.
		std::cout << "Waiting for incoming connections...\r\n";
		auto tmp_socket = accept(m_socket, nullptr, nullptr);
		if (tmp_socket == SOCKET_ERROR)
			throw SocketException{ " Server error : accept_connections() " };

		std::cout << "Server connect end \n";
	}
	
	// Client creation ->
	void Socket::connect_socket() {
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		// Attempt to connect to an address until one succeeds
		int iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &addrs);

		for (addrinfo* addr = addrs; addr != nullptr; addr = addr->ai_next) {
			// Create a SOCKET for connecting to server
			SOCKET sd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
			if (sd == -1)
				throw SocketException{ "Error: socket can not created in connect_socket()" };
			// Connect to server.
			auto tmp_socket = connect(m_socket, NULL, NULL);
			if (tmp_socket == 0)
				throw SocketException{ "Socket error in connect_socket()" };
			else
				std::cout << "socket succedd.\n";
		}
	}
}