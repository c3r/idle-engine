#include <iostream>
#include <map>
#include <queue>
#include <cstdint>
#include <unistd.h>

struct ResourceMeta {
    char* name;
    uint32_t id;
};

struct Resource {
    const ResourceMeta *meta;
    uint32_t int_val;
};

struct UpdateQuery {
    uint32_t fire_time;
    uint32_t entity_id;
    uint32_t resource_id;
    int32_t int_diff;
};

struct ResourceEntity { 
    uint32_t id;
    std::map<int, Resource> resources; 
};

uint32_t GLOBAL_TICK = 0;
const ResourceMeta kMetaGold   = { "gold",     1 };
const ResourceMeta kMetaLumber = { "lumber",   2 };
const ResourceMeta kMetaOil    = { "oil",      3 };

struct UpdateQueryCmp {
    bool operator() (UpdateQuery &q1, UpdateQuery &q2) {
        return q1.fire_time > q2.fire_time;     
    }
};
