#pragma once
#ifdef __APPLE__
  #include <sys/wait.h>
#else
  #include <wait.h>
#endif
#include <unistd.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include "exception.h"

constexpr auto FD_READ = 0;
constexpr auto FD_WRITE = 1;

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
