#ifndef PEN_WHEEL_PROCESSES_H
#define PEN_WHEEL_PROCESSES_H

#include <string>

struct process {
    int pid;
    std::string name;
};

class processes {
public:
    static process get_active_process();
};


#endif
