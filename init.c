#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "init.h"

const struct option long_options[] = {
    {"version",     no_argument,        0,  'v'},
    {"help",        no_argument,        0,  'h'},
    {"stdin",       required_argument,  0,  'i'},
    {"stdout",      required_argument,  0,  'o'},
    {"stderr",      required_argument,  0,  'e'},
    {"time-limit",  required_argument,  0,  'T'},
    {"mem-limit",   required_argument,  0,  'M'},
    {"out-limit",   required_argument,  0,  'O'},
    {"watch",       no_argument,        0,  'w'},
    {"work-dir",    required_argument,  0,  'd'},
    {"uid",         required_argument,  0,  'u'},
    {"gid",         required_argument,  0,  'g'},
    {0,             0,                  0,  0}
};

void show_help () {
    printf("Usage: cage [options] -c [cmd] ...\n");
    printf("Description ...\n\n");
    printf("Options:\n");
    printf("  %-28s%s\n", "-c", "Command (required)");
    printf("  %-28s%s\n", "-v, --version", "Show Version");
    printf("  %-28s%s\n", "-h, --help", "Show Help");
}

void show_version () {
    printf("Version: Cage 0.0.1 (alpha)\n");
    printf("Built: Oct 18 2012 02:33:02\n");
    printf("\n");
    printf("Written by Jay (i@zh-ang.com)\n");
}

struct _init_t init_info;

/*
 * return 0 if success
 * return 1 if exit
 */
int init (int argc, char* argv[]) {
    int c, i;
    init_info.child_stdin = NULL;
    init_info.child_stdout = NULL;
    init_info.child_stderr = NULL;
    init_info.child_uid = 0;
    init_info.child_gid = 0;
    init_info.time_limit = 2;
    init_info.memory_limit= 67108864;
    init_info.output_limit= 4194304;
    init_info.work_dir = NULL;
    init_info.cmd = NULL;
    while ((c=getopt_long(argc, argv, "vc:i:o:e:T:M:O:wt:d:h?", long_options, &i))!=-1) {
        switch (c) {
            case '?':
                fprintf(stderr, "Try `cage --help' for more information.\n");
                return 1;
            case 'h':
                show_help();
                return 1;
            case 'v':
                show_version();
                return 1;
            case 'c':
                init_info.cmd = strdup(optarg);
                break;
        }
    }

    if (init_info.work_dir == NULL) {
        init_info.work_dir = getcwd(NULL, 0);
    }

    if (!init_info.cmd) {
        fprintf(stderr, "missing \"-c\".\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "Try `cage --help' for more information.\n");
        return 1;
    }

    return 0;
}

