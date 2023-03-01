/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** welcome the client when it connects
*/

#include "client.h"
#include "logger.h"

void welcome_client(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    client_printf(client, "%d %s.\r\n", 220, "Service ready for new user");
    LOG_DEBUG("Client %d has been welcomed", client->fd);
}
