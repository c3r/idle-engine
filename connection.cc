#include "connection.h"
#include "exception.h"
#include "resource.h"
#include "socket.h"
#include "string_constants.h"

// TODO: extract this
// ================================================================================
inline std::string &trim(std::string &str,
                         const std::string &chars = "\t\n\v\f\r ") {
  str.erase(0, str.find_first_not_of(chars));
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}

inline std::string &trim(const char *str) {
  size_t s = sizeof(*str) / sizeof(char);
  std::string str_str(s, *str);
  return trim(str);
}
// =================================================================================

inline bool IsQuitCmd(RGroupUpdateEvt &evt) {
  return trim(evt.StrVal) == StringConstants::QuitCmd;
}

inline RGroupUpdateEvt QuitCmd(int id) { return {id, 0, 0, 0, "quit"}; }

void Connection::Run() {
  while (true) {
    RGroupUpdateEvt evt = ReceiveMsg();
    if (IsQuitCmd(evt)) {
      std::cout << StringConstants::RecvQuitCmd << m_Id
                << StringConstants::AttemptClose << std::endl;
      break;
    }
    write(m_WriteChannel, &evt, sizeof(evt));
  }

  Close();
  _exit(0);
}

int Connection::GetWriteChannel() { return m_WriteChannel; }
int Connection::GetReadChannel() { return m_ReadChannel; }
void Connection::SetWriteChannel(int fd) { m_WriteChannel = fd; }

RGroupUpdateEvt Connection::ReceiveMsg() const {
  const size_t bufsize = 4096;
  char recvbuf[bufsize];
  memset(&recvbuf, 0, bufsize);

  auto bytes_recv = recv(m_Id, &recvbuf, 4096, 0);
  if (bytes_recv == -1) {
    throw_exception(StringConstants::ErrRecvSoc);
    // TODO: close connections
  }

  if (bytes_recv == 0) {
    return QuitCmd(m_Id);
  }

  RGroupUpdateEvt evt { m_Id, 0, 0, 0, std::string(recvbuf) };
  return evt;
}

void Connection::SendMsg(std::string msg) const {
  if (send(m_Id, msg.c_str(), sizeof(msg.c_str()) + 1, 0) == -1) {
    throw_exception(StringConstants::ErrSendSoc);
  }
}

inline int IsFdValid(int fd) {
  return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void Connection::Close() {
  std::cout << "Closing socket for connection " << m_Id << "..." << std::endl;

  if (!IsFdValid(m_Id)) {
    std::cout << "Socket for connection " << m_Id
              << " alredy closed. Nothing to do." << std::endl;
    return;
  }

  if (shutdown(m_Id, SHUT_RDWR) == -1 || close(m_Id) == -1) {
    throw_exception(StringConstants::ErrCloseSoc);
  }

  std::cout << "Socket for connection " << m_Id << " closed." << std::endl;
}
