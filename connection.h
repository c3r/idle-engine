#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include "resource.h"

class Socket;

class Connection {
 private:
  int m_Id = 0;
  int m_ReadChannel;
  int m_WriteChannel;

 public:
  void Run();
  int GetWriteChannel();
  int GetReadChannel();
  void SetWriteChannel(int fd);
  void SetReadChannel(int fd);
  Connection(int id) : m_Id(id) {}
  ~Connection(){};
  RGroupUpdateEvt ReceiveMsg() const;
  void SendMsg(std::string msg) const;
  void Close();
};
