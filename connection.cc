#include "connection.h"
#include "exception.h"
#include "resource.h"
#include "socket.h"

#define CONST_S(x) constexpr char x[]
CONST_S(CN_RECV_SOCKET) = "Could not receive a message from socket";
CONST_S(CN_SEND_SOCKET) = "Could not send a message on a socket";
CONST_S(QUIT_CMD) = "quit";
CONST_S(RECV_QC) = "Received quit command from connection ";
CONST_S(ATTEMPT_CLOSE) = ". Attempting to close connection.";
CONST_S(CN_CLOSE_SOCKET) = "Could not close client socket for connection.";

#define CLOSING_CONNECTION_FOR(conn_fd) \
  "Closing socket for connection " << conn_fd << "..." << std::endl

#define SOCKET_ALREADY_CLOSED(conn_fd)                                    \
  "Socket for connection " << conn_fd << " alredy closed. Nothing to do." \
                           << std::endl

#define SOCKET_CLOSED(conn_fd) \
  "Socket for connection " << conn_fd << " closed." << std::endl

#define ATTEMPT_CLOSE(_id) RECV_QC << _id << ATTEMPT_CLOSE << std::endl

// TODO: extract this
// ================================================================================
inline std::string& trim(std::string& str,
                         const std::string& chars = "\t\n\v\f\r ") {
  str.erase(0, str.find_first_not_of(chars));
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}
// =================================================================================

inline bool IsQuitCmd(RGroupUpdateEvt& evt) {
  return trim(evt.str_val) == QUIT_CMD;
}

inline RGroupUpdateEvt QuitCmd(int id) { return {id, 0, 0, 0, "quit"}; }

void Connection::Run() {
  while (true) {
    RGroupUpdateEvt evt = ReceiveMsg();
    if (IsQuitCmd(evt)) {
      std::cout << ATTEMPT_CLOSE(_id);
      break;
    }
    write(_wr_channel, &evt, sizeof(evt));
  }

  Close();
  _exit(0);
}

int Connection::GetWriteChannel() { return _wr_channel; }
int Connection::GetReadChannel() { return _rd_channel; }
void Connection::SetWriteChannel(int wr_channel) { _wr_channel = wr_channel; }

RGroupUpdateEvt Connection::ReceiveMsg() const {
  RGroupUpdateEvt evt;
  auto bytes_recv = recv(_id, &evt, sizeof(evt), 0);
  if (bytes_recv == -1) {
    throw_exception(CN_RECV_SOCKET);
    // TODO: close connections
  }

  if (bytes_recv == 0) {
    return QuitCmd(_id);
  }

  return evt;
}

void Connection::SendMsg(std::string msg) const {
  if (send(_id, msg.c_str(), sizeof(msg.c_str()) + 1, 0) == -1) {
    throw_exception(CN_SEND_SOCKET);
  }
}

inline int IsFdValid(int fd) {
  return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void Connection::Close() {
  std::cout << CLOSING_CONNECTION_FOR(_id);
  if (!IsFdValid(_id)) {
    std::cout << SOCKET_ALREADY_CLOSED(_id);
    return;
  }

  if (shutdown(_id, SHUT_RDWR) == -1 || close(_id) == -1) {
    throw_exception(CN_CLOSE_SOCKET);
  }

  std::cout << SOCKET_CLOSED(_id);
}
