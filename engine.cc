#include "engine.h"

bool RGroupUpdate(RGroup* rgroup, RGroupUpdateEvt* ev) {
    if (rgroup == nullptr || ev == nullptr) {
        return false;
    }

    Resource *res = &( rgroup->map->at(ev->rgroup_id) );
    if (res == nullptr) {
        return false;
    }

    res->int_val += ev->int_diff;
    res->str_val = ev->str_val;

    return true;
}

Resource ResourceCreate(RMeta *meta, RIntVal int_val) {
    Resource resource;
    resource.meta = meta;
    resource.int_val = int_val;
    return resource;
}

RGroup* RGroupCreate(RMeta *rmeta) {
    static RGroup rg;
    static RMap_t map;
    rg.meta = rmeta;
    rg.map = &map;
    return &rg;
}

int ChildProcess() {
    return 0;
}

int ParentProcess(pid_t child_pid) {
    RGroup rg;
    UEventPQueue equeue;
    bool stop = false;

    while (!stop) {
        Tick();
        if (equeue.empty()) continue;
        while (GetTick() >= equeue.top().tick) {
            RGroupUpdateEvt ev = equeue.top();
            equeue.pop();
            RGroupUpdate(&rg, &ev);

            if (equeue.empty()) break;
        }
    }

    /* Wait for child to exit. */
    int status;
    waitpid(child_pid, &status, 0);

    return 0;
}

int main (int argc, char *argv[]) {
    // TODO:
    // 1. Create a socket
    // 2. Bind the socket to an IP / port
    // 3. Mark the socket for listening
    // 4. Accept a call
    // 5. Close the listening socket
    // ...
    // L. Close socket

    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        return 1;
    }

    return child_pid == 0
           ? ChildProcess()
           : ParentProcess(child_pid);
}
