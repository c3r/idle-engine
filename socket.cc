#include "socket.h"
#include "connection.h"

#define CONST_S(x) constexpr char x[]
CONST_S(CN_CRATE_SOCKET) = "Could not create socket";
CONST_S(CN_BIND_SOCKET) = "Could not bind listening socket to network address";
CONST_S(CN_LISTEN_SOCKET) = "Could not mark socket for listening";
CONST_S(CN_ACCEPT_CONN) = "Could not accept a connection on a socket";

int Socket::Create() {
  auto res = socket(kSockDomain, kSockType, kSockProtocol);
  if (res == -1) {
    throw_exception(CN_CRATE_SOCKET);
  }
  return res;
}

void Socket::Listen() {
  sockaddr_in hint{.sin_family = kSockDomain,
                   .sin_port = htons(port_),
                   .sin_addr.s_addr = kBindAddr};

  if (bind(listening_sock_, (sockaddr*)&hint, sizeof(hint)) == -1) {
    throw_exception(CN_BIND_SOCKET);
  }

  if (listen(listening_sock_, SOMAXCONN) == -1) {
    throw_exception(CN_LISTEN_SOCKET);
  }
}

Connection Socket::AcceptConnection() {
  sockaddr_in client;
  socklen_t client_size = sizeof(client);
  int conn_fd = accept(listening_sock_, (sockaddr*)&client, &client_size);
  if (conn_fd == -1) {
    throw_exception(CN_ACCEPT_CONN);
  }

  // TODO: check for max connections limit (SOMAXCONN)
  return Connection(conn_fd);
}

