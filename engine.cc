#include "engine.h"
#include "connection.h"
#include "socket.h"

constexpr auto FD_READ = 0;
constexpr auto FD_WRITE = 1;

// TODO: extract this

class IdleEngineProcess {
 private:
  int _rd_channel;
  int _wr_channel;

 public:
  IdleEngineProcess() {
    int pipefds[2];
		// TODO: extract pipe creation
    auto rs = pipe(pipefds);
    _rd_channel = pipefds[FD_READ];
    _wr_channel = pipefds[FD_WRITE];
  };
  ~IdleEngineProcess(){};
  int GetReadChannel();
  int GetWriteChannel();
  void Run();
};

int IdleEngineProcess::GetReadChannel() { return _rd_channel; }
int IdleEngineProcess::GetWriteChannel() { return _wr_channel; }
void IdleEngineProcess::Run() {
  while (true) {
    EngineMsg msg;
    read(_rd_channel, &msg, sizeof(msg));
    std::cout << msg.conn_id << " > " << msg.msg << std::endl;
  }
  _exit(0);
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

pid_t CreateChildProcess() {
  auto pid = fork();
  if (pid == -1) {
    throw_exception("Could not create the engine process");
  }

  return pid;
}

int main() {
  // TODO: extract all to ListenerProcess class?
  IdleEngineProcess iep;
  if (CreateChildProcess() == 0) iep.Run();

  Socket socket(54000);
  while (true) {
    auto conn = socket.AcceptConnection();
    conn->SetWriteChannel(iep.GetWriteChannel());
    if (CreateChildProcess() == 0) {
      conn->Run();
    }
  }

  // TODO: wait also for engine process and others?
  // waitpid(pid, nullptr, 0);
  return 0;
}
