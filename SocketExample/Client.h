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

	class Client
	{
	public:
		enum class TypeSocket : u_long { BlockingSocket = 0, NonBlockingSocket = 1 };
		Client(const std::string& host, const std::string& port, TypeSocket socketType = TypeSocket::BlockingSocket);
		Client(const Client&) = delete;
		Client(Client&&) = delete;
		Client& operator=(const Client&) = delete;
		Client operator=(Client&&) = delete;
		~Client();
		void connect_socket();

		std::string receive_line();
		std::string receive_bytes();
		void receive_until() const;
		void receive();
		void listen_socket() const ;
		void send_line(const std::string& send_lines) const;

		void set_blocking_type(const TypeSocket& typeSocket) {
			this->socketType = typeSocket;
		}
		TypeSocket get_blocking_type() const {
			return socketType;
		}
		void set_port(std::string port) {
			this->port = port;
		}
		std::string get_port() const{
			return port;
		}		
		void set_ip(const std::string& port) {
			this->host = host;
		}
		std::string get_ip() const {
			return host;
		}
	protected:
		void startup();
		void blocking_mode();
		//void bind_socket();

	private:
		std::string port;
		int connections;
		mutable char buffer[4096];
		TypeSocket socketType;
		std::string userInput;
		std::string host;
		WSADATA wsaData;
		sockaddr_in socket_address;
		addrinfo hints = {} , * addrs;
		hostent* host_ent;
		SOCKET m_socket;
		std::string error;
		mutable int iResult;
	};
}
