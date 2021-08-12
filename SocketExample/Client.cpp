#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include "Client.h"



namespace aricanli::network {

	Client::Client(const std::string& host, const std::string& port, TypeSocket socketType) :
	host{host} , port{port}, socketType{ socketType }
	{
		// Initialize Winsock
		startup();

		// Create socket
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
			throw ClientException{ "Socket creation failed." };

		// Fill in a hint structure
		ZeroMemory(&socket_address, sizeof(socket_address));
		if (( host_ent = gethostbyname(host.c_str()) )== 0) {
			error = strerror(errno);
			throw error;
		}
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;	
	}

	Client::~Client() {
		// Close socket and clean up
		closesocket(m_socket);
		shutdown(m_socket, SD_SEND);
		freeaddrinfo(addrs);
		WSACleanup();
	}
	void Client::startup() {
		// Initialize Winsock
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			throw ClientException{ "WSAStartup failed error" };
		}	
	}
	void Client::listen_socket() const {
		listen(m_socket, 5);
	}
	void Client::connect_socket() {
		blocking_mode();
		// Attempt to connect to an address until one succeeds
		 int iResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &addrs);

		 for (addrinfo* addr = addrs; addr != nullptr; addr = addr->ai_next) {
			 // Create a SOCKET for connecting to server
			 int sd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
			 if (sd == -1)
				 throw ClientException{ "Error: socket can not created in connect_socket()" };
			 // Connect to server.
			 if (connect(m_socket, addr->ai_addr, addr->ai_addrlen) == 0)
				 break;
			 
		 }		
	}
	void Client::send_line(const std::string& send_lines) const {
		send(m_socket, send_lines.c_str(), send_lines.length(), 0);
	}
	void Client::receive_until() const{
		// Receive until the peer closes the connection
		do {

			iResult = recv(m_socket, buffer, 4096, 0);
			if (iResult > 0) {
				std::cout <<"Bytes received: \n"<< iResult<<"\n";
			}				
			else if (iResult == 0)
				std::cout << "Connection closed\n";
			else
				throw ClientException{ "Error: receive failed in receive_until()" };

		} while (iResult > 0);
	}
	void Client::blocking_mode() {
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
	void Client::receive() {
		// receive lines and echo 
		do {
			std::cout << " > ";
			std::getline(std::cin, userInput);

			if (userInput.size() > 0) {

				int sendResult = send(m_socket, userInput.c_str(), userInput.size() + 1, 0);
				if (sendResult != SOCKET_ERROR) {
					ZeroMemory(buffer, 4096);
					int bytesReceived = recv(m_socket, buffer, 4096, 0);
					if (bytesReceived > 0) {
						std::cout << "Server>" << std::string(buffer, 0, bytesReceived) << "\n";
					}
				}
			}
		} while (userInput.size() > 0);
	}
	std::string Client::receive_bytes() {
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