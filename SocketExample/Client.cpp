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
		Socket(ip, port ,1, socketType ) { 	}

	Client::~Client() {	}

	void Client::send_line(const std::string& send_lines) const {
		std::cout << "GET : " << send_lines << "\nlength:"<< send_lines.length() << "\n\n";
		auto error = send(m_socket, send_lines.c_str(), send_lines.length(), 0);
		if (error == SOCKET_ERROR)
			std::cout << "Error in send_line()" << WSAGetLastError << "\n";
	}
}