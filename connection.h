#pragma once
#include "resource.h"
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

class Socket;

class Connection
{
private:
  int _id = 0;
  Socket* _socket;
  int _rd_channel;
  int _wr_channel;

public:
  void Run();
  int GetWriteChannel();
  int GetReadChannel();
  void SetWriteChannel(int wr_channel);
  void SetReadChannel(int rd_channel);
  Connection(int id) : _id(id) {}
  ~Connection(){};
  RGroupUpdateEvt ReceiveMsg() const;
  void SendMsg(std::string msg) const;
  void Close();
};