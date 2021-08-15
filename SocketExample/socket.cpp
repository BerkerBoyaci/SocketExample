#include "socket.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define WIN32_LEAN_AND_MEAN
// Need to link with Ws2_32.lib

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include "Client.h"
#pragma comment(lib, "ws2_32.lib")
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
	const SOCKET Socket::getSocket() const
	{
		return m_socket;
	}
	void Socket::blocking_mode() {
		// Enable non-blocking or blocking mode 
		if (socketType == TypeSocket::BlockingSocket) {
			u_long sType = 0;
			auto error = ioctlsocket(m_socket, FIONBIO, &sType);
			if (error == SOCKET_ERROR)
				throw SocketException{ "Error in blocking mode." };
		}
		if (socketType == TypeSocket::NonBlockingSocket) {
			u_long sType = 1;
			auto error = ioctlsocket(m_socket, FIONBIO, &sType);
			if (error == SOCKET_ERROR)
				throw SocketException{"Error in non-blocking mode."};
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
	
	void Socket::receive_until() {
		// Receive until the peer closes the connection
		do {
			auto iResult = recv(m_socket, buffer, 4096, 0);
			if (iResult > 0) 
				std::cout << "Bytes received: \n" << iResult << "\n";
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
}