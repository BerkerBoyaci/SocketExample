#include "Server.h"

namespace aricanli::network {
	Server::Server(int port, int connections, TypeSocket socketType ):
		port{ port }, connections{ connections }, socketType{ socketType }
	{
		// Initilaize winsock
		startup();

		// Create a socket
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
			throw ServerException{ "Error : Socket creation failed in constructor" };

		// Bind the ip address and port to a socket
		memset(&socketAddress , 0, sizeof(socketAddress));
		socketAddress.sin_family = PF_INET;
		socketAddress.sin_addr.s_addr = INADDR_ANY;
		socketAddress.sin_port = htons(port);
		bind_socket();
	}

	Server::~Server(){ 
		// Close socket and clean up
		WSACleanup();
		closesocket(m_socket);
		shutdown(m_socket, SD_SEND);
	}
	void Server::bind_socket() {
		// Bind the ip address and port to a socket
		if (bind(m_socket, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(sockaddr_in)) == SOCKET_ERROR) {
			closesocket(m_socket);
			throw ServerException{ "Error : socket can not bind in bind_socket()" };
		}
	}
	void Server::listen_socket() const{
		//Tell winsock the socket is for listening
		listen(m_socket, connections);
	}
	void Server::startup() {
		// Initilaize winsock
		if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
			throw ServerException{ "Error : WSA Initialization failed in startup()" };
	}
	void Server::accept_connections() {
		listen_socket();
		// Wait for a connection.
		SOCKET TempSock = SOCKET_ERROR;
		while (TempSock == SOCKET_ERROR)
		{
			std::cout << "Waiting for incoming connections...\r\n";
			TempSock = accept(m_socket, nullptr, nullptr);
		}
		blocking_mode();
			
		m_socket = TempSock;
		std::cout << "Client connected!\n";
	}
	void Server::blocking_mode() const{
		// Enable non-blocking or blocking mode 
		if (socketType == TypeSocket::BlockingSocket) {
			sType = 0;
			ioctlsocket(m_socket, FIONBIO, &sType);
		}
		if (socketType == TypeSocket::NonBlockingSocket) {
			sType = 1;
			ioctlsocket(m_socket, FIONBIO, &sType);
		}
	}
	void Server::send_bytes(const std::string& send_bytes) const {
		send(m_socket, send_bytes.c_str() , send_bytes.length() , 0);
	}
}