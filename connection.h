#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <string>

constexpr auto kMsgBufSize = 4096;

class Socket;

struct EngineMsg {
  int conn_id;
  char msg[kMsgBufSize];
};

class Connection {
 private:
  Socket* _socket;
  int _rd_channel;
  int _wr_channel;

 public:
  int _id = 0;
  void Run();
  int GetWriteChannel();
  int GetReadChannel();
  void SetWriteChannel(int wr_channel);
  void SetReadChannel(int rd_channel);
  Connection(int id, Socket* s) : _id(id), _socket(s) {}
  ~Connection(){};
  std::string ReceiveMsg() const;
  void SendMsg(std::string msg) const;
  void Close();
};