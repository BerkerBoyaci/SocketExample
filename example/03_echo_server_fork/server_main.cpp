#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "server.hpp"

static void sigchld_handler(int)
{
    while (waitpid(-1, nullptr, WNOHANG) > 0)
        ;
}

int main()
{
    std::cout << std::unitbuf;

    struct sigaction sa{};
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, nullptr);

    try {
        socketlab::network::Server server(8080, 10,
            socketlab::network::TypeSocket::BlockingSocket);
        server.start_listening();
        std::cout << "Multi-Client Echo Server (fork) ready. Port 8080...\n";

        for (;;) {
            int client_fd = server.accept_one();
            if (client_fd == -1)
                continue;

            std::cout << "New client connected, PID=" << getpid()
                      << " calling fork()...\n";

            pid_t pid = fork();
            if (pid < 0) {
                std::cerr << "Error: fork() failed!\n";
                ::close(client_fd);
                continue;
            }

            if (pid == 0) {
                ::close(server.get_fd());
                std::cout << "[Child " << getpid()
                          << "] Echo loop started.\n";

                for (;;) {
                    std::string msg =
                        socketlab::network::Server::recv_from(client_fd);
                    if (msg.empty()) {
                        std::cout << "[Child " << getpid()
                                  << "] Client disconnected.\n";
                        break;
                    }
                    std::cout << "[Child " << getpid() << "] Echo: " << msg;
                    socketlab::network::Server::send_to(client_fd, msg);
                }

                ::close(client_fd);
                _exit(0);
            }

            ::close(client_fd);
        }
    }
    catch (const socketlab::network::ServerException& ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}
