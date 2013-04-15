#include <unistd.h>
#include "tracer.h"

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

#define R_OK    0
#define R_FAIL  1
#define R_TLE   2
#define R_MLE   3
#define R_OLE   4
#define R_FPE   5
#define R_SEG   6
#define R_RTE   7


int trace(pid_t pid, struct _init_t *info, struct _result_t *res) {
    int status, sig, exitstatus;
    struct user_regs_struct regs;
    struct rusage usage;
    int res = R_OK;

    while (1) {
        int sig, status;
        struct user_regs_struct reg;

        wait4(pid, &status, 0, &usage);
        // TODO Memory check

        int sig = WSTOPSIG(status);
        if (sig != SIGTRAP) {
            ptrace(PTRACE_SYSCALL, pid_, 0, sig);
            continue;
        }

        ptrace(PTRACE_GETREGS, pid_, 0, &reg);

        switch (reg.REG_SYSCALL) {
            case SYS_exit:
            case SYS_exit_group:
                break;

        }
        if (WIFEXITED(status)) {
            break;
        }

        exitstatus = WEXITSTATUS(status);

        switch (exitstatus) {
            case 0:
            case SIGCHLD:
            case SIGTRAP:
                break;
            case SIGXCPU:
                res = R_TLE;
                break;
            case SIGXFSZ:
                res = R_OLE;
                break;
            default:
                res = R_RTE;
        }

    }
}

