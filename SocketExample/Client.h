#pragma once
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 
#endif
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  
#include <unistd.h> 
#endif
#include <string>
#include <iostream>
#include "socket.h"
#pragma comment(lib,"ws2_32.lib")

namespace aricanli::network {
	class ClientException : public std::exception {
	public:
		explicit ClientException(const char* message) noexcept : m_message(message) {}
		explicit ClientException(const std::string& message) noexcept : m_message(message) {}
		virtual ~ClientException() noexcept {}

		virtual const char* what() const noexcept {
			return m_message.c_str();
		}
	protected:
		std::string m_message{ "Invalid Server Query" };
	};

	class Client : public Socket
	{
	public:
		
		explicit Client(const std::string& host, const std::string& port, TypeSocket socketType = TypeSocket::BlockingSocket) ;
		explicit Client(const Client&) noexcept = delete;
		explicit Client(Client&&) noexcept = delete;
		Client& operator=(const Client&) = delete;
		Client operator=(Client&&) = delete;
		~Client();

		virtual void send_line(const std::string& send_lines) const override;
		void connect_socket() ;
	private:
		WSADATA wsaData;
		sockaddr_in socket_address;
		addrinfo hints = {} , * addrs;
		hostent* host_ent;
		std::string error;		
	};
}
