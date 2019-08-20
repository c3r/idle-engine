#include "engine.h"
#include "connection.h"
#include "socket.h"

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  str.erase(0, str.find_first_not_of(chars));
  return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
  return ltrim(rtrim(str, chars), chars);
}

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
  RMap map;
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
    trim(msg);
    std::cout << conn->m_id << "> " << msg << std::endl;
    if (msg == "quit") {
      std::cout << "Received quit command from connection " << conn->m_id
                << ". Attempting to close connection." << std::endl;
      break;
    }
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
    // std::cout << GetTick() << std::endl;
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
  auto pid = fork();
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
