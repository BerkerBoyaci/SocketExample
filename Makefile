CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -Iinclude -static-libstdc++ -static-libgcc
BUILDDIR := build
EXAMPLE  ?= 01_hello_server

COMMON_SRCS := src/socket.cpp src/server.cpp src/client.cpp
SERVER_SRCS := $(COMMON_SRCS) example/$(EXAMPLE)/server_main.cpp
CLIENT_SRCS := $(COMMON_SRCS) example/$(EXAMPLE)/client_main.cpp

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
