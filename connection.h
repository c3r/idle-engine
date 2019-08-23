#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include "resource.h"

constexpr auto kMsgBufSize = sizeof(RGroupUpdateEvt);

class Socket;

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
  RGroupUpdateEvt ReceiveMsg() const;
  void SendMsg(std::string msg) const;
  void Close();
};