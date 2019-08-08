#include "engine.h"

bool update_entity(ResourceGroup* rg, UpdateQuery* uq) 
{
    if (rg == nullptr || uq == nullptr) { return false; }

    ResourceGroup *r = &rg->resources[uq->resource_id];
    if (r == nullptr) { return false; }

    r->int_val += uq->int_diff;
    r->str_val = up->str_val;
    
    return true;
}

int main (int argc, char *argv[]) 
{
    std::map<uint32_t, ResourceGroup> resource_mapping;
    std::priority_queue<UpdateQuery, std::vector<UpdateQuery>, UpdateQueryCmp> 
        queue;

    bool stop = false;
    while (!stop) {
        sleep(1);
	TICK++;
        if (queue.empty()) continue; 

        while (TICK >= queue.top().fire_time) {
	    UpdateQuery q = queue.top();
            queue.pop();
            update_group(&resource_mapping[q.resource_group_id], &q);
            if (queue.empty()) break;
        }
    }
}
