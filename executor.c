#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/ptrace.h>

#include "executor.h"

void die(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    raise(SIGKILL);
}

int run(struct _init_t *info) {

    struct rlimit limit;
    int fd, r;

    if (info->child_stdin) {
        fd = open(info->child_stdin, O_RDONLY, 0777);
        if (fd<0) die("Cage: open child_stdin[%s] failed", info->child_stdin);
        r = dup2(fd, 0);
        if (r<0) die("Cage: dup2 child_stdin[%s] failed", info->child_stdin);
    }

    if (info->child_stdout) {
        fd = open(info->child_stdout, O_RDWR | O_CREAT | O_TRUNC , 0777);
        if (fd<0) die("Cage: open child_stdout[%s] failed", info->child_stdout);
        r = dup2(fd, 1);
        if (r<0) die("Cage: dup2 child_stdout[%s] failed", info->child_stdout);
    }

    if (info->child_stderr) {
        fd = open(info->child_stderr, O_RDWR, 0777);
        if (fd<0) die("Cage: open child_stderr[%s] failed", info->child_stderr);
        r = dup2(fd, 2);
        if (r<0) die("Cage: dup2 child_stderr[%s] failed", info->child_stderr);
    }

    if (info->time_limit) {
        limit.rlim_cur = limit.rlim_max = info->time_limit;
        r = setrlimit(RLIMIT_CPU, &limit);
        if (r<0) die("Cage: setrlimit(%d, %d) failed.", RLIMIT_CPU, limit.rlim_max);
    }

    if (info->memory_limit) {
        limit.rlim_cur = limit.rlim_max = info->memory_limit;
        r = setrlimit(RLIMIT_DATA, &limit);
        if (r<0) die("Cage: setrlimit(%d, %d) failed.", RLIMIT_DATA, limit.rlim_max);
    }

    if (info->memory_limit) {
        limit.rlim_cur = limit.rlim_max = info->memory_limit + 16777216; // plus 16M
        r = setrlimit(RLIMIT_AS, &limit);
        if (r<0) die("Cage: setrlimit(%d, %d) failed.", RLIMIT_AS, limit.rlim_max);
    }

    if (1) {
        limit.rlim_cur = limit.rlim_max = 33554432; // Stack is always limited to 32M
        r = setrlimit(RLIMIT_STACK, &limit);
        if (r<0) die("Cage: setrlimit(%d, %d) failed.", RLIMIT_STACK, limit.rlim_max);
    }

    if (info->output_limit) {
        limit.rlim_cur = limit.rlim_max = info->output_limit;
        r = setrlimit(RLIMIT_FSIZE, &limit);
        if (r<0) die("Cage: setrlimit(%d, %d) failed.", RLIMIT_FSIZE, info->output_limit);
    }

    if (info->work_dir) {
        r = chdir(info->work_dir);
        if (r<0) die("Cage: chdir(%s) failed.", info->time_limit);
    }

    if (1) {
        limit.rlim_cur = limit.rlim_max = 5; // Openfile is always limited to 5
        r = setrlimit(RLIMIT_NOFILE, &limit);
        if (r<0) die("Cage: setrlimit(%d, %d) failed.", RLIMIT_NOFILE, limit.rlim_max);
    }

    if (1) {
        limit.rlim_cur = limit.rlim_max = 1; // Num of proc is always limited to 1
        r = setrlimit(RLIMIT_NOFILE, &limit);
        if (r<0) die("Cage: setrlimit(%d, %d) failed.", RLIMIT_NPROC, limit.rlim_max);
    }

    if (1) {
        limit.rlim_cur = limit.rlim_max = 0;
        r = setrlimit(RLIMIT_CORE, &limit);
        if (r<0) die("Cage: setrlimit(%d, %d) failed.", RLIMIT_NPROC, limit.rlim_max);
    }

    if (info->watch) {
        r = ptrace(PTRACE_TRACEME, 0, 0, 0);
        if (r<0) die("Cage: failed to TRACEME");
    }

    execl("/bin/sh", "sh", "-c", info->cmd, NULL);
    die("Cage: fail to execl /bin/bash for %s", info->cmd);
    return -2;

}

