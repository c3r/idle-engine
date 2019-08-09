#include <iostream>
#include <map>
#include <queue>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unistd.h>

struct RMeta {
    const char* name; 
    uint32_t id;
};

struct Resource {
    const RMeta *meta;
    uint32_t int_val;
    char* str_val;
};

typedef std::map<uint32_t, Resource> RMap_t; 

struct RGroup {
    RMeta* meta;
    RMap_t *map;
};

struct UpdateEvent {
    uint32_t tick;
    uint32_t rg_id;
    int32_t int_diff;
    char* str_val;
};

struct UEventCmp {
    bool operator() (UpdateEvent &q1, UpdateEvent &q2) { 
        return q1.tick > q2.tick; 
    }
};

typedef std::vector<UpdateEvent> UEventVec_t;
typedef std::priority_queue<UpdateEvent, UEventVec_t, UEventCmp> UEventPQueue_t;
        
uint32_t TICK = 0;
const int kDefaultTickDt = 1;
void Tick(int dt) { sleep(dt); TICK++; }
void Tick() { Tick(kDefaultTickDt); }
int32_t GetTick() { return TICK; }
