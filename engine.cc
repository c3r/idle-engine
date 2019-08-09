#include "engine.h"

bool UpdateResource(RGroup* rgroup, UpdateEvent* ev) 
{
    if (rgroup == nullptr || ev == nullptr) { return false; }

    Resource *res = &rgroup->map->at(ev->rg_id);
    if (res == nullptr) { return false; }

    res->int_val += ev->int_diff;
    res->str_val = ev->str_val;
    
    return true;
}

RGroup* CreateResourceMapping(const char* name, uint32_t id)
{
    RMeta rmeta = { name, id };
    static RGroup rg;
    static RMap_t map;
    rg.meta = &rmeta;
    rg.map = &map; 
    return &rg;
}

int main (int argc, char *argv[]) 
{
    RGroup* rg = CreateResourceMapping("aaa", 1);
    UEventPQueue_t equeue;

    bool stop = false;
    while (!stop) 
    {
        Tick();
        if (equeue.empty()) continue; 
        while (GetTick() >= equeue.top().tick) 
        {
            UpdateEvent ev = equeue.top(); 
            equeue.pop();
            UpdateResource(rg, &ev);

            if (equeue.empty()) break;
        }
    }
}
