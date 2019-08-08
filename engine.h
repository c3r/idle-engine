#include <iostream>
#include <map>
#include <queue>
#include <cstdint>
#include <unistd.h>

struct ResourceMeta {
    char* name;
    uint32_t id;
};

struct ResourceGroup {
    const ResourceMeta *meta;
    uint32_t id;
    std::map<int, ResourceGroup> resources; 
    uint32_t int_val;
    char* str_val;
};

struct UpdateEvent {
    uint32_t tick;
    uint32_t rg_id;
    int32_t int_diff;
    char* str_val;
};

struct UpdateEventCmp {
    bool operator() (UpdateEvent &q1, UpdateEvent &q2) { return q1.tick > q2.tick; }
};

uint32_t TICK = 0;
const int kDefaultTickDt = 1;
void Tick(int dt) { sleep(dt); TICK++; }
void Tick() { Tick(kDefaultTickDt); }
int32_t GetTick() { return TICK; }
