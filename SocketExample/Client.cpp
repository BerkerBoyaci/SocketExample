#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include "Client.h"


namespace aricanli::network {

	Client::Client(const std::string& ip, const std::string& port, TypeSocket socketType) :
		Socket(ip, port ,4, socketType ) { 	}

	Client::~Client() {	}
	// Client creation ->
	void Client::connect_socket() {
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
				std::cout << "Client created.\n";

			std::string send_lines{ "Client created." };
			auto error = send(m_socket, send_lines.c_str(), send_lines.length(), 0);
			if (error == SOCKET_ERROR)
				std::cout << "Error in send()" << WSAGetLastError << "\n";
		}
	}
	void Client::send_line(const std::string& send_lines) const {
		Socket::send_line(send_lines);
	}
}