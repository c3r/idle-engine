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
    while (!stop) 
    {
        if (queue.empty()) { stop = true; break; }

        UpdateQuery query = queue.top();
        while (TICK++ >= query.fire_time) 
        {
            queue.pop();
            update_group(&resource_mapping[query.resource_group_id], &query);
        }
        sleep(1);
    }
}
