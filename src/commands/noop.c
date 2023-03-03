/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** noop command
*/

#include "myftp.h"
#include "server.h"
#include "commands.h"
#include "client.h"
#include "logger.h"

#include <string.h>

void noop_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    LOG_DEBUG("Client %d sent NOOP command", client->fd);
    client_printf(client, "%d %s.\r\n", 200, "Command okay");
}
