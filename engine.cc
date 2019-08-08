#include "engine.h"

bool UpdateGroup(ResourceGroup* rg, UpdateEvent* ev) 
{
    if (rg == nullptr || ev == nullptr) { return false; }

    ResourceGroup *r = &rg->resources[ev->rg_id];
    if (r == nullptr) { return false; }

    r->int_val += ev->int_diff;
    r->str_val = ev->str_val;
    
    return true;
}

int main (int argc, char *argv[]) 
{


    std::map<uint32_t, ResourceGroup> resource_mapping;
    std::priority_queue<UpdateEvent, std::vector<UpdateEvent>, UpdateEventCmp> 
        queue;

    bool stop = false;
    while (!stop) 
    {
        Tick();
        if (queue.empty()) continue; 

        while (GetTick() >= queue.top().tick) 
        {
            UpdateEvent ev = queue.top(); 
            queue.pop();
            UpdateGroup(&resource_mapping[ev.rg_id], &ev);

            if (queue.empty()) break;
        }
    }
}
