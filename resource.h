#pragma once
#include <string>
#include <map>
#include <vector>
#include <queue>

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