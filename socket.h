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

#define MSG_BUF_SIZE 4096
#define DEFAULT_LISTENING_PORT 54000

class Socket {
 private:
  const int kSockDomain = AF_INET;
  const int kSockType = SOCK_STREAM;
  const int kSockProtocol = 0;

  std::string addr_;

  int port_;
  int listening_sock_;
  int live_connections_[SOMAXCONN];
  int bytes_recv_;

  char host_[NI_MAXHOST];
  char svc_[NI_MAXSERV];

  int Create();
  void Bind();

 public:
  Socket(std::string addr, uint16_t port) : addr_(addr), port_(port) {
    listening_sock_ = Create();
    Bind();
  }
  ~Socket(){
      // if (!client_sock_) Close();
  };

  int AcceptConnection();
  std::string ReceiveMsg(int conn);
  int SendMsg(int conn, std::string);
  int Close(int conn);
};
