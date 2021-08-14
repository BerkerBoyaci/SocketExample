#include "Server.h"

namespace aricanli::network {
	//const std::string& ip, int port, int connections, TypeSocket socketType)
	Server::Server(const std::string& port, int connections, TypeSocket socketType ):
		Socket{ std::string{}, port ,connections, socketType } {
	}

	Server::~Server(){ 	}

	void Server::send_line(const std::string& send_lines) const {
		auto error = send(m_socket, send_lines.c_str(), send_lines.length(), 0);
		if (error == SOCKET_ERROR)
			std::cout << "Error in send_line()" << WSAGetLastError << "\n";
	}

}



