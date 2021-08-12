#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <memory>
#include "socket.h"
#include "Client.h"
#include "Server.h"
#include <string>

using namespace aricanli;

void client_blocking_example();
void client_nonblocking_example();
void server_blocking_example();
void server_nonblocking_example();

int main()
{
	client_blocking_example();

	return 0;
}

void server_blocking_example() {
	try {
		network::Server server(80, 1, network::Server::TypeSocket::BlockingSocket);
		server.accept_connections();

		for (;;)
		{
			std::string szMessage = "Welcome to the server!\r\n";
			server.send_bytes(szMessage);
			Sleep(1000);
		}
	}
	catch (const network::ServerException& ex) {
		std::cout << ex.what();
	}
	catch (...) {
		std::cout << " unknown exception";
	}

}

void server_nonblocking_example() {
	try {
		network::Server server(80, 1);
		server.set_blocking_type(network::Server::TypeSocket::NonBlockingSocket);
		server.accept_connections();

		// Main loop
		for (;;)
		{
			std::string szMessage = "Welcome to the server!\r\n";
			server.send_bytes(szMessage);
			Sleep(1000);
		}
	}
	catch (const network::ServerException& ex) {
		std::cout << ex.what();
	}
	catch (...) {
		std::cout << " unknown exception";
	}
}

void client_blocking_example() {
	try {
		network::Client client{ "192.168.1.21","22" };
		client.set_blocking_type(network::Client::TypeSocket::BlockingSocket);
		client.connect_socket();
		client.send_line("connected");
		client.send_line(client.get_ip());
		client.send_line(client.get_port());

		while (1) {
			client.receive_until();
		}
	}
	catch (const network::ClientException& ex) {
		std::cout << ex.what();
	}
	catch (...) {
		std::cout << " unknown exception";
	}

}
void client_nonblocking_example() {
	try {
		network::Client client{ "192.168.1.21","22",network::Client::TypeSocket::NonBlockingSocket };
		client.connect_socket();
		client.send_line("GET / HTTP/1.0");
		client.send_line("Host: www.google.com");
		client.send_line("");

		while (1) {
			client.receive_until();
		}
	}
	catch (const network::ClientException& ex) {
		std::cout << ex.what();
	}
	catch (...) {
		std::cout << " unknown exception";
	}

}