CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude -static-libstdc++ -static-libgcc
BUILDDIR := build

COMMON_SRCS := src/socket.cpp src/server.cpp src/client.cpp
SERVER_SRCS := $(COMMON_SRCS) example/server_main.cpp
CLIENT_SRCS := $(COMMON_SRCS) example/client_main.cpp

.PHONY: all server client clean

all: server client

server: $(SERVER_SRCS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $(BUILDDIR)/server $(SERVER_SRCS)

client: $(CLIENT_SRCS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $(BUILDDIR)/client $(CLIENT_SRCS)

clean:
	rm -rf $(BUILDDIR)
