#include "engine.h"
#include "connection.h"
#include "socket.h"

pid_t CreateChildProcess() {
  auto pid = fork();
  if (pid == -1) {
    throw_exception("Could not create the engine process");
  }

  return pid;
}

IdleEngineProcess* EngineProcess() {
  IdleEngineProcess iep;
  if (CreateChildProcess() == 0) {
    iep.Run();
  }
  return &iep;
}

void ConnectionProcess(Connection& conn, int wc) {
  conn.SetWriteChannel(wc);
  if (CreateChildProcess() == 0) {
    conn.Run();
  }
}

int main() {
  IdleEngineProcess* iep = EngineProcess();
  Socket socket;
  while (true) {
    auto conn = socket.AcceptConnection();
    ConnectionProcess(conn, iep->GetWriteChannel());
  }

  // TODO: wait also for engine process and others?
  // waitpid(pid, nullptr, 0);
  return 0;
}
