/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** cdup command
*/

#include "myftp.h"
#include "server.h"
#include "commands.h"
#include "client.h"
#include "logger.h"

#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

bool path_exists(const char *path)
{
    struct stat buffer;

    return (stat(path, &buffer) == 0);
}

bool path_is_file(const char *path)
{
    struct stat buffer;

    if (stat(path, &buffer) != 0)
        return false;
    return S_ISREG(buffer.st_mode);
}
