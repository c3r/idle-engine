#include "connection.h"
#include "engine.h"
#include "pipe.h"
#include "socket.h"

inline pid_t CreateChildProcess() {
  auto pid = fork();
  if (pid == -1) {
    throw_exception("Could not create child process");
  }

  return pid;
}

int main() {
  Pipe p;

  // Engine process
  IdleEngineProcess iep(p.GetReadChannel());
  if (CreateChildProcess() == 0) {
    iep.Run();
  }

  // Fork a new process for every incoming connection
  Socket socket;
  while (true) {
    auto conn = socket.AcceptConnection();
    conn.SetWriteChannel(p.GetWriteChannel());
    if (CreateChildProcess() == 0) {
      conn.Run();
    }
  }

  // Wait for children to exit
  pid_t wpid;
  int status = 0;
  while ((wpid = wait(&status)) > 0)
    ;
  return 0;
}
