#include "connection.h"
#include "exception.h"
#include "socket.h"

std::string Connection::ReceiveMsg() const {
  char msgbuf[MSG_BUF_SIZE];
  memset(&msgbuf, 0, MSG_BUF_SIZE);

  auto bytes_recv = recv(m_id, msgbuf, MSG_BUF_SIZE, 0);
  if (bytes_recv == -1)
    throw_exception("Could not receive a message from socket");

  if (bytes_recv == 0) return "quit";

  return std::string(msgbuf, 0, bytes_recv);
};

void Connection::SendMsg(std::string msg) const {
  if (send(m_id, msg.c_str(), sizeof(msg.c_str()) + 1, 0) == -1)
    throw_exception("Could not send a message on a socket");
};

void Connection::Close() { m_socket->Close(m_id); }