#ifndef CAGE_INIT_H
#define CAGE_INIT_H

int init (int argc, char* argv[]) ;

struct _init_t {
    char watch;
    char *child_stdin, *child_stdout, *child_stderr;
    int child_uid, child_gid;
    int time_limit, memory_limit, output_limit;
    char *work_dir;
    char *cmd;
};

#endif
