/*
 * Ensimag - Projet système
 * Copyright (C) 2013 - Damien Dejean <dam.dejean@gmail.com>
 */

#include "sysapi.h"

const char *tests[21] = {
        "test0",
        "test1",
        "test2",
        "test3",
        "test4",
        "test5",
        "test6",
        "test7",
        "test8",
        "test9",
        "test10",
        "test11",
        "test12",
        "test13",
        "test14",
        "test15",
        "test16",
        "test17",
        "test18",
        "test19",
        "test20",
};

int main(void)
{
        int i;
        int pid;
        int ret;

        for (i = 0; i < 20; i++) {
                printf("Test %s : ", tests[i]);
                pid = start(tests[i], 4000, 128, NULL);
                waitpid(pid, &ret);
                assert(ret == 0);
        }
}

