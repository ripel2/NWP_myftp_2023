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

void cdup_command(server_t *server, client_t *client)
{
    free(client->buffer);
    client->buffer = strdup("CWD ..");
    client->buffer_size = 7;
    LOG_INFO("Client %d sent CDUP command, running as CWD", client->fd);
    cwd_command(server, client);
}
