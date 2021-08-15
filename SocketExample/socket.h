#pragma once

#include <string>
#ifdef _WIN32
#ifndef _WIN32_WINNT
#endif
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  
#include <unistd.h> 
#endif
#include <memory>

#pragma comment(lib , "ws2_32.lib")

namespace aricanli::network {

	class SocketException : public std::exception {
	public:
		explicit SocketException(const char* message) noexcept : m_message(message) {}
		explicit SocketException(const std::string& message) noexcept : m_message(message) {}
		virtual ~SocketException() noexcept {}

		virtual const char* what() const noexcept {
			return m_message.c_str();
		}
	protected:
		std::string m_message{ "Invalid Server Query" };
	};

	class Socket
	{
	public:
		enum class TypeSocket : u_long { BlockingSocket = 0, NonBlockingSocket = 1 };
		explicit Socket(const std::string& ip = "192.168.1.1", const std::string& port = "80",
						int connections = 1, TypeSocket socketType = TypeSocket::BlockingSocket);

		explicit Socket(const Socket& ) noexcept = delete;
		explicit Socket(Socket&&) noexcept = delete;
		Socket& operator=(const Socket&) = delete;
		Socket operator=(Socket&&) = delete;
		virtual ~Socket();
		static bool startup();
		static void end();
		const SOCKET getSocket() const;
		
		virtual void send_line(const std::string& send_lines) const;
		virtual void receive_until();
		std::string receive_bytes();


		virtual void blocking_mode();
		virtual void listen_socket() const;
		//

		virtual void set_blocking_type(const TypeSocket& typeSocket) {
			this->socketType = typeSocket;
		}
		virtual TypeSocket get_blocking_type() const {
			return socketType;
		}
		virtual void set_port(const std::string& port) {
			this->port = port;
		}
		virtual std::string get_port() const {
			return port;
		}
		virtual void set_ip(const std::string& ip) {
			this->ip = ip;
		}
		virtual std::string get_ip() const {
			return ip;
		}

	protected:
		std::string ip;
		std::string port;
		int connections;
		SOCKET m_socket;
		addrinfo address;
		TypeSocket socketType;
		static bool m_isInitWSA;
		
	private:
		sockaddr_in socketAddress;
		mutable int iResult;
		mutable char buffer[4096];
		addrinfo hints = {}, * addrs;
	};

}

