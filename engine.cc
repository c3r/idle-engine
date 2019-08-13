#include "engine.h"
#include "socket.h"

int CreatePipe(int* pipefds) {
  auto rs = pipe(pipefds);
  if (rs == -1) {
    throw_exception("Cannot create a pipe");
  }

  return rs;
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
  RMap_t map;
  rg.meta = rmeta;
  rg.map = map;
  return rg;
}

/*
 * Child process
 *
 * 0 bytes means that client dropped the connection, so we need to close the
 * socket. If there were some bytes received, just read them write to the server
 * engine, and wait for respone If there is any response from the engine server,
 * just forward the message to the client
 */
int Child(Socket s, int conn, int& write_channel, int& read_channel) {
  char readbuf[4096];
  memset(readbuf, 0, sizeof(readbuf));
  try {
    while (true) {
      auto msg = s.ReceiveMsg(conn);
      if (msg == "quit") {
        s.Close(conn);
        break;
      }
      std::cout << conn << "> " << msg << std::endl;
      // write(write_channel, msg.c_str(), sizeof(msg.c_str()));
      // if (read(read_channel, readbuf, sizeof(readbuf))) {
      //  s.SendMsg(readbuf);
      //  memset(readbuf, 0, sizeof(readbuf));
      //}
    }
    s.Close(conn);
    return 0;
  } catch (std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
    return 1;
  }
}

// =============================================================================
// Parent process
//
// ...
// =============================================================================
int Parent(int& write_channel, int& read_channel) {
  char readbuf[4096];
  bool stop = false;

  memset(readbuf, 0, sizeof(readbuf));
  while (!stop) {
    Tick();
    std::cout << GetTick() << std::endl;
    // if (read(read_channel, readbuf, sizeof(readbuf)) > 0) {
    //  std::cout << "CLIENT> " << readbuf << std::endl;
    //  memset(readbuf, 0, sizeof(readbuf));
    //  write(write_channel, "Tank you\n", sizeof("Tank you\n"));
    //} else {
    //  continue;
    //}

    // if (equeue.empty()) {
    //  continue;
    //}

    // while (GetTick() >= equeue.top().tick) {
    //  RGroupUpdateEvt ev = equeue.top();
    //  equeue.pop();
    //  RGroupUpdate(&rg, &ev);
    //  if (equeue.empty()) {
    //    break;
    //  }
    //}
  }
}

#define READ 0
#define WRITE 1

int main() {
  int ctp[2], ptc[2];
  CreatePipe(ctp);
  CreatePipe(ptc);

  Socket socket("0.0.0.0", 54000);
  while (1) {
    std::cout << "Listening for connections..." << std::endl;
    int conn = socket.AcceptConnection();
    pid_t pid = fork();
    std::cout << "New connection (" << conn << ") accepted and forked to "
              << pid << std::endl;
    switch (pid) {
      case -1:
        // throw_exception
        break;
      case 0:
        // close(ctp[READ]);
        // close(ptc[WRITE]);
        return Child(socket, conn, ctp[WRITE], ptc[READ]);
      default:
        continue;
        // close(ptc[READ]);
        // close(ctp[WRITE]);
        // Parent(ptc[WRITE], ctp[READ]);
    }
    waitpid(pid, nullptr, 0);
    return 0;
  }
}
