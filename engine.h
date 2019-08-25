#pragma once
#ifdef __APPLE__
#include <sys/wait.h>
#else
#include <wait.h>
#endif
#include "exception.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <unistd.h>
#include <utility>

class IdleEngineProcess {
private:
    int _rd_channel;
    int _wr_channel;

public:
    IdleEngineProcess(int rd_channel)
        : _rd_channel(rd_channel) {};

    ~IdleEngineProcess() {};
    int GetReadChannel();
    int GetWriteChannel();
    void Run();
};
