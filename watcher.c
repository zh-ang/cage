#include <unistd.h>
#include <sys/ptrace.h>

#include "init.h"

#ifdef __i386
#define REG_SYSCALL orig_eax
#define REG_RET eax
#define REG_ARG0 ebx
#define REG_ARG1 ecx
#endif

#ifdef __x86_64
#define REG_SYSCALL orig_rax
#define REG_RET rax
#define REG_ARG0 rdi
#define REG_ARG1 rsi
#endif

int watch(pid_t pid,struct _init_t *info) {
    int status;
    while (waitpid(pid, &status, 0) > 0) {
    }
}

