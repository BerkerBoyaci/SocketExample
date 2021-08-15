#include "Server.h"

namespace aricanli::network {
	//const std::string& ip, int port, int connections, TypeSocket socketType)
	Server::Server(const std::string& port, int connections, TypeSocket socketType ):
		Socket{ std::string{}, port ,connections, socketType }
		{
	}

	Server::~Server(){ 	}
	void Server::bind_socket() {
		memset(&socketAddress, 0, sizeof(socketAddress));
		socketAddress.sin_family = PF_INET;
		socketAddress.sin_addr.s_addr = INADDR_ANY;
		inet_pton(AF_INET, ip.c_str(), &socketAddress.sin_addr.s_addr);
		socketAddress.sin_port = htons(std::stoi(port));

		auto tmp_socket = bind(Socket::m_socket, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(sockaddr_in));
		if (tmp_socket == SOCKET_ERROR) {
			throw SocketException{ "Error : socket can not bind in bind_socket()" };
		}
	}

	void Server::accept_connections() {
		//listen_socket();
		// Wait for a connection.
		std::cout << "Waiting for incoming connections...\r\n";
		auto tmp_socket = accept(m_socket, nullptr, nullptr);
		if (tmp_socket == INVALID_SOCKET)
			throw SocketException{ " Server error : accept_connections() " };
	
		std::cout << "Server connect end \n";
	}

	void Server::send_line(const std::string& send_lines) const {
		Socket::send_line(send_lines);
	}
	void Server::create_server() {
		this->bind_socket();
		Socket::listen_socket();
		Socket::blocking_mode();
		this->accept_connections();
	}
	void Server::receive_until()  {
		Socket::receive_until();
	}

}