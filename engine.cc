#include "engine.h"
#include "connection.h"

int IdleEngineProcess::GetReadChannel() { return _rd_channel; }
int IdleEngineProcess::GetWriteChannel() { return _wr_channel; }

inline void PrintEvt(RGroupUpdateEvt &evt) {
  std::cout << " < " << evt.ConnId << " > " << evt.StrVal << std::endl;
}

void IdleEngineProcess::Run() {
  while (true) {
    RGroupUpdateEvt evt;
    read(_rd_channel, &evt, sizeof(evt));
    PrintEvt(evt);
  }
  _exit(0);
}
