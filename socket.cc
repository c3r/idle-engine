#include "socket.h"
#include "connection.h"

int Socket::Create() {
  auto res = socket(kSockDomain, kSockType, kSockProtocol);
  if (res == -1) {
    throw_exception("Could not create socket");
  }
  return res;
}

void Socket::Listen() {
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

std::unique_ptr<Connection> Socket::AcceptConnection() {
  sockaddr_in client;
  socklen_t client_size = sizeof(client);

  int conn_fd = accept(listening_sock_, (sockaddr *)&client, &client_size);
  if (conn_fd == -1) {
    throw_exception("Could not accept a connection on a socket");
  }

  // TODO: check for max connections limit (SOMAXCONN)
  return std::make_unique<Connection>(conn_fd, this);
}

int IsFdValid(int fd) { return fcntl(fd, F_GETFD) != -1 || errno != EBADF; }

void Socket::Close(int conn) {
  std::cout << "Closing socket for connection " << conn << "..." << std::endl;
  if (!IsFdValid(conn)) {
    std::cout << "Socket for connection " << conn
              << " alredy closed. Nothing to do." << std::endl;
    return;
  }

  if (shutdown(conn, SHUT_RDWR) == -1 || close(conn) == -1) {
    throw_exception("Could not close client socket");
  }

  std::cout << "Socket for connection " << conn << " closed." << std::endl;
}
