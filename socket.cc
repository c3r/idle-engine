#include "socket.h"
#include "connection.h"
#include "string_constants.h"


int Socket::Create() {
  auto res = socket(kSockDomain, kSockType, kSockProtocol);
  if (res == -1) {
    throw_exception(StringConstants::ErrCreateSoc);
  }
  return res;
}

void Socket::Listen() {
  sockaddr_in hint;
  hint.sin_family = kSockDomain, hint.sin_port = htons(port_);
  hint.sin_addr.s_addr = kBindAddr;

  if (bind(listening_sock_, (sockaddr *)&hint, sizeof(hint)) == -1) {
    throw_exception(StringConstants::ErrBindSoc);
  }

  if (listen(listening_sock_, SOMAXCONN) == -1) {
    throw_exception(StringConstants::ErrListenSoc);
  }
}

Connection Socket::AcceptConnection() {
  sockaddr_in client;
  socklen_t client_size = sizeof(client);
  int conn_fd = accept(listening_sock_, (sockaddr *)&client, &client_size);
  if (conn_fd == -1) {
    throw_exception(StringConstants::ErrAcceptCon);
  }

  // TODO: check for max connections limit (SOMAXCONN)
  return Connection(conn_fd);
}
