#pragma once
#include <map>
#include <queue>
#include <string>
#include <vector>

typedef std::string RName;
typedef std::string RStrVal;
typedef uint32_t RIdent;
typedef uint32_t RIntVal;

struct RMeta {
  RName name;
  RIdent id;
};

struct Resource {
  const RMeta *meta;
  RIntVal int_val;
  RStrVal str_val;
};

typedef std::map<uint32_t, Resource> RMap;

struct RGroup {
  RMeta *meta;
  RMap map;
};

struct RGroupUpdateEvt {
  int ConnId;
  int32_t Tick;
  RIdent RGroupId;
  RIntVal IntDiff;
  RStrVal StrVal;
};

struct UEventCmp {
  bool operator()(RGroupUpdateEvt &evt1, RGroupUpdateEvt &evt2) {
    return evt1.Tick > evt2.Tick;
  }
};

typedef std::vector<RGroupUpdateEvt> UEventVec;
typedef std::priority_queue<RGroupUpdateEvt, UEventVec, UEventCmp> UEventPQueue;
