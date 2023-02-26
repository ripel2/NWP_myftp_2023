/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** myftp argument handling
*/

#include "myftp.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

static int check_port(char **av)
{
    int port = 0;

    for (size_t c = 0; av[1][c]; c++) {
        if (av[1][c] < '0' || av[1][c] > '9') {
            fprintf(stderr, "%s: port \"%s\" is not a number\n", av[0], av[1]);
            return 84;
        }
    }
    port = atoi(av[1]);
    if (port < 1024 || port > 65535) {
        fprintf(stderr, "%s: port \"%s\" is out of range\n", av[0], av[1]);
        return 84;
    }
    return 0;
}

static int check_path(char **av)
{
    struct stat info = {0};

    if (stat(av[2], &info) != 0) {
        fprintf(stderr, "%s: path \"%s\" does not exist\n", av[0], av[2]);
        return 84;
    }
    if (!S_ISDIR(info.st_mode)) {
        fprintf(stderr, "%s: path \"%s\" is not a directory\n", av[0], av[2]);
        return 84;
    }
    return 0;
}

int check_args(int ac, char **av)
{
    if (ac != 3) {
        fprintf(stderr, "%s: usage: ./myftp port path\n", av[0]);
        return 84;
    }
    if (check_port(av) == 84) {
        return 84;
    }
    if (check_path(av) == 84) {
        return 84;
    }
    return 0;
}
