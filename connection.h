#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <string>

#define MSG_BUF_SIZE 4096

class Socket;

class Connection {
 private:
  Socket* m_socket;

 public:
  int m_id;
  Connection(int id, Socket* s) : m_id(id), m_socket(s) {}
  ~Connection(){};
  std::string ReceiveMsg() const;
  void SendMsg(std::string msg) const;
  void Close();
};