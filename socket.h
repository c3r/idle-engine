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
#include <stdexcept>
#include <string>

#define MSG_BUF_SIZE 4096
#define DEFAULT_LISTENING_PORT 54000

class Socket {
 private:
  const int kSockDomain = AF_INET;
  const int kSockType = SOCK_STREAM;
  const int kSockProtocol = 0;

  std::string addr_;
  sockaddr_in client_;

  int port_;
  int listening_sock_;
  int client_sock_;
  int bytes_recv_;

  char host_[NI_MAXHOST];
  char svc_[NI_MAXSERV];
  char msgbuf_[MSG_BUF_SIZE];

  int Create();

  void MarkForListening();
  void Accept();
  void Bind();
  void CloseListening();

 public:
  Socket(std::string addr, uint16_t port) : addr_(addr), port_(port) {
    listening_sock_ = Create();
    Bind();
    MarkForListening();
    Accept();
  }
  ~Socket() {
    if (!client_sock_) Close();
  };

  std::string GetMsg();

  int ReceiveMsg();
  int SendMsg(std::string);
  int Close();
};
