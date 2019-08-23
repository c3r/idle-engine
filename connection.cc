#include "connection.h"
#include "exception.h"
#include "socket.h"
#include "resource.h"

// TODO: extract this
// ================================================================================
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  str.erase(0, str.find_first_not_of(chars));
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}
// =================================================================================

//EngineMsg CreateMsg(const std::string& msg_str, int conn_id) {
//  EngineMsg engine_msg;
//  msg_str.copy(engine_msg.msg_str, msg_str.size() + 1);
//  engine_msg.msg_str[msg_str.size()] = '\0';
//  engine_msg.conn_id = conn_id;
//  return engine_msg;
//}

void Connection::Run() {
  while (true) {
    auto msg = ReceiveMsg();

    if (msg.msg_str == "quit") {
      std::cout << "Received quit command from connection " << _id
                << ". Attempting to close connection." << std::endl;
      break;
    }

    write(_wr_channel, &msg, sizeof(msg));
  }
  Close();
  _exit(0);
}

int Connection::GetWriteChannel() { return _wr_channel; }
int Connection::GetReadChannel() { return _rd_channel; }

void Connection::SetWriteChannel(int wr_channel) { _wr_channel = wr_channel; }
void Connection::SetReadChannel(int rd_channel) { _rd_channel = rd_channel; }

RGroupUpdateEvt Connection::ReceiveMsg() const {
  char msgbuf[sizeof(RGroupUpdateEvt)];
  memset(&msgbuf, 0, sizeof(RGroupUpdateEvt));

  auto bytes_recv = recv(_id, msgbuf, sizeof(RGroupUpdateEvt), 0);
  if (bytes_recv == -1) {
    throw_exception("Could not receive a message from socket");
  }

  //if (bytes_recv == 0) {
  //  return {0, "quit"};
  //}

  std::string str = std::string(msgbuf, 0, bytes_recv);
  EngineMsg msg = CreateMsg(trim(str), _id);
  return msg;
}

void Connection::SendMsg(std::string msg) const {
  if (send(_id, msg.c_str(), sizeof(msg.c_str()) + 1, 0) == -1) {
    throw_exception("Could not send a message on a socket");
  }
}

void Connection::Close() { _socket->Close(_id); }