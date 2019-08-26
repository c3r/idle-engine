#pragma once
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "exception.h"

constexpr auto kDefaultPort = 54000;
constexpr auto kSockDomain = AF_INET;
constexpr auto kSockType = SOCK_STREAM;
constexpr auto kSockProtocol = 0;
constexpr auto kBindAddr = INADDR_ANY;

class Connection;
typedef std::vector<std::unique_ptr<Connection> > ConnectionsVec;

class Socket {
 private:
  int port_;
  int listening_sock_;

  int Create();
  void Listen();

 public:
  Socket(uint16_t port = kDefaultPort) : port_(port) {
    listening_sock_ = Create();
    Listen();
  }
  ~Socket(){
      // TODO: close all connections
  };

  Connection AcceptConnection();
  void Close(int fd);
};
