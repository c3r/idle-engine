#include "engine.h"
#include "connection.h"

int IdleEngineProcess::GetReadChannel() { return _rd_channel; }

int IdleEngineProcess::GetWriteChannel() { return _wr_channel; }

void IdleEngineProcess::Run() {
  while (true) {
    EngineMsg msg;
    read(_rd_channel, &msg, sizeof(msg));
    std::cout << " < " << msg.conn_id << " > " << msg.msg_str << std::endl;
  }
  _exit(0);
}
