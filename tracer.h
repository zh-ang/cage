#ifndef CAGE_TRACER_H
#define CAGE_TRACER_H

#include <sys/ptrace.h>

#include "init.h"

struct _result_t {
    int termsig;
    int memory_usage;
    int time_elapsed;
    int exitcode;
}

int trace(pid_t pid, struct _init_t *info) ;

#endif
