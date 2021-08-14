#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <memory>
#include "socket.h"
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 
#endif
#include <winsock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  
#include <unistd.h> 
#endif
#pragma comment(lib,"ws2_32.lib")

namespace aricanli::network {

	class ServerException : public std::exception {
	public:
		explicit ServerException(const char* message) noexcept : m_message(message) {}
		explicit ServerException(const std::string& message) noexcept : m_message(message) {}
		virtual ~ServerException() noexcept {}

		virtual const char* what() const noexcept {
			return m_message.c_str();
		}

	protected:
		std::string m_message{ "Invalid Server Query" };
	};

	class Server : public Socket
	{
	public:
		
		explicit Server(const std::string& port , int connections , TypeSocket socketType = TypeSocket::BlockingSocket) ;
		explicit Server(const Server&) noexcept= delete;
		explicit Server(Server&&) noexcept = delete;
		Server& operator=(const Server&) = delete;
		Server operator=(Server&&) = delete;
		~Server();
		//void bind_socket();
		//void accept_connections();

		virtual void send_line(const std::string& send_lines) const override;
		
	protected:
		
	private:
		SOCKET tmp_socket;
		sockaddr_in socketAddress;
	};
}
