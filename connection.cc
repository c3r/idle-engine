#include "connection.h"
#include "exception.h"
#include "socket.h"

// TODO: extract this
// ================================================================================
std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  str.erase(0, str.find_first_not_of(chars));
  return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  return ltrim(rtrim(str, chars), chars);
}
// =================================================================================

EngineMsg CreateMsg(std::string& str, int conn_id) {
  EngineMsg engine_msg;
  str.copy(engine_msg.msg, str.size() + 1);
  engine_msg.msg[str.size()] = '\0';
  engine_msg.conn_id = conn_id;
  return engine_msg;
}

void Connection::Run() {
  while (true) {
    auto msg = ReceiveMsg();
    trim(msg);
    if (msg == "quit") {
      std::cout << "Received quit command from connection " << _id
                << ". Attempting to close connection." << std::endl;
      break;
    }
    EngineMsg engine_msg = CreateMsg(msg, _id);
    write(_wr_channel, &engine_msg, sizeof(engine_msg));
  }
  Close();
  _exit(0);
}

int Connection::GetWriteChannel() { return _wr_channel; }
int Connection::GetReadChannel() { return _rd_channel; }

void Connection::SetWriteChannel(int wr_channel) { _wr_channel = wr_channel; }
void Connection::SetReadChannel(int rd_channel) { _rd_channel = rd_channel; }

std::string Connection::ReceiveMsg() const {
  char msgbuf[kMsgBufSize];
  memset(&msgbuf, 0, kMsgBufSize);

  auto bytes_recv = recv(_id, msgbuf, kMsgBufSize, 0);
  if (bytes_recv == -1) {
    throw_exception("Could not receive a message from socket");
  }

  if (bytes_recv == 0) {
    return "quit";
  }

  return std::string(msgbuf, 0, bytes_recv);
}

void Connection::SendMsg(std::string msg) const {
  if (send(_id, msg.c_str(), sizeof(msg.c_str()) + 1, 0) == -1) {
    throw_exception("Could not send a message on a socket");
  }
}

void Connection::Close() { _socket->Close(_id); }