#include "engine.h"
#include "connection.h"
#include "socket.h"

bool RGroupUpdate(RGroup* rgroup, RGroupUpdateEvt* ev) {
  if (rgroup == nullptr || ev == nullptr) {
    return false;
  }

  Resource* res = &(rgroup->map.at(ev->rgroup_id));
  if (res == nullptr) {
    return false;
  }

  res->int_val += ev->int_diff;
  res->str_val = ev->str_val;

  return true;
}

Resource ResourceCreate(RMeta* meta, RIntVal int_val) {
  Resource resource;
  resource.meta = meta;
  resource.int_val = int_val;
  return resource;
}

RGroup RGroupCreate(RMeta* rmeta) {
  RGroup rg;
  RMap_t map;
  rg.meta = rmeta;
  rg.map = map;
  return rg;
}

/*
 * Connection process
 */
void ConnectionProcess(std::unique_ptr<Connection> conn, int& wc, int& rc) {
  while (true) {
    auto msg = conn->ReceiveMsg();
    if (msg == "quit") {
      conn->Close();
      break;
    }
    std::cout << conn->m_id << "> " << msg << std::endl;
  }
  conn->Close();
  _exit(0);
}

/*
 * Engine process
 */
void EngineProcess(int& wc, int& rc) {
  while (true) {
    Tick();
    std::cout << GetTick() << std::endl;
  }
  _exit(0);
}

#define READ 0
#define WRITE 1

int CreatePipe(int* pipefds) {
  auto rs = pipe(pipefds);
  if (rs == -1) {
    throw_exception("Cannot create a pipe");
  }

  return rs;
}

pid_t CreateProcess() {
  pid_t pid = fork();
  if (pid == -1) {
    throw_exception("Could not create the engine process");
  }

  return pid;
}

int main() {
  int ctp[2], ptc[2];
  CreatePipe(ctp);
  CreatePipe(ptc);

  if (CreateProcess() == 0) {
    EngineProcess(ptc[WRITE], ctp[READ]);
  }

  Socket socket("0.0.0.0", 54000);
  pid_t pid;
  while (true) {
    auto conn = socket.AcceptConnection();
    if (!(pid = CreateProcess())) {
      ConnectionProcess(std::move(conn), ctp[WRITE], ptc[READ]);
    }
  }

  // TODO: wait also for engine process and others?
  waitpid(pid, nullptr, 0);
  return 0;
}
