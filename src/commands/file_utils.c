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

void file_open_error(client_t *client)
{
    LOG_ERROR("Client %d failed to open file", client->fd);
    thread_send(client, "550 Failed to open file.\r\n");
    exit(0);
}

void file_write_error(client_t *client)
{
    LOG_ERROR("Client %d failed to write to file", client->fd);
    thread_send(client, "550 Failed to write to file.\r\n");
    exit(0);
}

void file_transfer_success(client_t *client)
{
    LOG_DEBUG("Client %d file transfer successful", client->fd);
    thread_send(client,
    "226 Closing data connection, file transfer successful.\r\n");
}
