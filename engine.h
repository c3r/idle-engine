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

typedef std::string RName;
typedef std::string RStrVal;
typedef uint32_t RIdent;
typedef uint32_t RIntVal;

struct RMeta {
  RName name;
  RIdent id;
};

struct Resource {
  const RMeta* meta;
  RIntVal int_val;
  RStrVal str_val;
};

typedef std::map<uint32_t, Resource> RMap;

struct RGroup {
  RMeta* meta;
  RMap map;
};

struct RGroupUpdateEvt {
  int32_t tick;
  RIdent rgroup_id;
  RIntVal int_diff;
  RStrVal str_val;
};

struct UEventCmp {
  bool operator()(RGroupUpdateEvt& evt1, RGroupUpdateEvt& evt2) {
    return evt1.tick > evt2.tick;
  }
};

typedef std::vector<RGroupUpdateEvt> UEventVec;
typedef std::priority_queue<RGroupUpdateEvt, UEventVec, UEventCmp> UEventPQueue;

uint32_t TICK = 0;
const int kDefaultTickDt = 1;
void Tick(int dt) {
  sleep(dt);
  TICK++;
}
void Tick() { Tick(kDefaultTickDt); }
int32_t GetTick() { return TICK; }
