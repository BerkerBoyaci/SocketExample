#pragma once

#include <string>
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
#include <memory>

#pragma comment(lib , "ws2_32.lib")

namespace aricanli::network {
	class Socket
	{
	public:
		explicit Socket(const Socket& ) = delete;
		explicit Socket(Socket&&) = delete;
		Socket& operator=(const Socket&) = delete;
		Socket operator=(Socket&&) = delete;
		virtual ~Socket();
	protected:
		enum class TypeSocket { BlockingSocket, NonBlockingSocket };
		//explicit Socket(Socket s) noexcept;
		explicit Socket();
		virtual void close();
		virtual void start();
		//virtual void startup();
		virtual void end();
	protected:
		SOCKET m_socket;
		WSADATA wsa;
		addrinfo address;
	};

}

