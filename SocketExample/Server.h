#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <memory>
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

	class Server 
	{
	public:
		enum class TypeSocket : u_long { BlockingSocket = 0, NonBlockingSocket = 1};
		Server(int port , int connections , TypeSocket socketType = TypeSocket::BlockingSocket);
		Server(const Server&) = delete;
		Server(Server&&) = delete;
		Server& operator=(const Server&) = delete;
		Server operator=(Server&&) = delete;
		~Server();
		void accept_connections();
		void send_bytes(const std::string&) const;

		void set_blocking_type(const TypeSocket& typeSocket) {
			this->socketType = typeSocket;
		}
		TypeSocket get_blocking_type() const{
			return socketType;
		}
		void set_port(int port) {
			this->port = port;
		}
		int get_port() const{
			return port;
		}
	protected:
		void startup();
		void bind_socket();
		void listen_socket() const; 
		void blocking_mode() const;
		
	private:
		int port;
		int connections;
		SOCKET m_socket;
		WSADATA WsaDat;
		TypeSocket socketType;
		sockaddr_in socketAddress;
		mutable u_long sType;
	};
}
