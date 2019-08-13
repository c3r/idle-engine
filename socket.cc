#include "socket.h"

int Socket::Create() {
  auto res = socket(kSockDomain, kSockType, kSockProtocol);
  if (res == -1) {
    throw_exception("Could not create socket");
  }

  return res;
}

void Socket::Bind() {
  sockaddr_in hint;
  hint.sin_family = kSockDomain;
  hint.sin_port = htons(port_);

  if (inet_pton(kSockDomain, addr_.c_str(), &hint.sin_addr) == -1) {
    throw_exception("Could not convert address string to network address");
  }

  if (bind(listening_sock_, (sockaddr *)&hint, sizeof(hint)) == -1) {
    throw_exception("Could not bind listening socket to network address");
  }

  if (listen(listening_sock_, SOMAXCONN) == -1) {
    throw_exception("Could not mark socket for listening");
  }
}

int Socket::AcceptConnection() {
  sockaddr_in client;
  socklen_t client_size = sizeof(client);
  int new_conn = accept(listening_sock_, (sockaddr *)&client, &client_size);
  if (new_conn == -1) {
    throw_exception("Could not accept a connection on a socket");
  }
  live_connections_[new_conn] = new_conn;
  return new_conn;
}

std::string Socket::ReceiveMsg(int conn) {
  char msgbuf[MSG_BUF_SIZE];
  memset(&msgbuf, 0, MSG_BUF_SIZE);

  bytes_recv_ = recv(conn, msgbuf, MSG_BUF_SIZE, 0);
  if (bytes_recv_ == -1) {
    throw_exception("Could not receive a message from socket");
  }

  if (bytes_recv_ == 0) {
    return "quit";
  }

  return std::string(msgbuf, 0, bytes_recv_);
}

int Socket::SendMsg(int conn, std::string msg) {
  if (send(conn, msg.c_str(), sizeof(msg.c_str()) + 1, 0) == -1) {
    throw_exception("Could not send a message on a socket");
  }

  return 0;
}

int Socket::Close(int conn) {
  if (close(conn) == -1) {
    throw_exception("Could not close client socket");
  }

  return 0;
}
