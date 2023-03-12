/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** pwd command
*/

#include "myftp.h"
#include "server.h"
#include "commands.h"
#include "client.h"
#include "logger.h"

#include <string.h>

void pwd_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    if (!client->logged_in) {
        LOG_WARNING("Client %d sent PWD command while not logged in",
        client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    LOG_DEBUG("Client %d sent PWD command", client->fd);
    client_printf(client, "%d \"%s\" is current directory.\r\n", 257,
    client->path);
}
