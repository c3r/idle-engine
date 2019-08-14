#pragma once
#include <arpa/inet.h>
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

#define DEFAULT_LISTENING_PORT 54000

class Connection;
typedef std::vector<std::unique_ptr<Connection>> ConnectionsVec;

class Socket {
 private:
  const int kSockDomain = AF_INET;
  const int kSockType = SOCK_STREAM;
  const int kSockProtocol = 0;

  std::string addr_;

  int port_;
  int listening_sock_;
  ConnectionsVec live_connections_;

  char host_[NI_MAXHOST];
  char svc_[NI_MAXSERV];

  int Create();
  void Listen();

 public:
  Socket(std::string addr, uint16_t port) : addr_(addr), port_(port) {
    listening_sock_ = Create();
    Listen();
  }
  ~Socket(){
      // if (!client_sock_) Close();
  };

  std::unique_ptr<Connection> AcceptConnection();
  void Close(int conn);
};
