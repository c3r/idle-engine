#include "engine.h"
#include "socket.h"

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

int main () {

    pid_t child_pid = fork();

    if (child_pid == 0) { return 0; }

	try {
		Socket s("0.0.0.0", 54000);
		while (true) {
			int br = s.ReceiveMsg();
			switch(br) { case 0 : break; case -1 : break; }
			std::cout << "> " << s.GetMsg() << std::endl;
			s.SendMsg("Tank you");
		}
		s.Close();
	} catch (std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    return child_pid == 0
           ? ChildProcess()
           : ParentProcess(child_pid);
}
