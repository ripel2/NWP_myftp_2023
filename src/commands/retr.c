/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** retr command
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

static void retr_check_file(server_t *server, client_t *client)
{
    char *path = client->buffer + 5;

    if (!path_exists(path)) {
        LOG_DEBUG("Client %d tried to RETR a non-existing file", client->fd);
        client_printf(client, "%d %s.\r\n", 550, "File not found");
        return;
    }
    if (!path_is_file(path)) {
        LOG_DEBUG("Client %d tried to RETR a directory", client->fd);
        client_printf(client, "%d %s.\r\n", 550, "Can't RETR a directory");
        return;
    }
    client_printf(client, "%d %s.\r\n", 150,
    "File status okay; about to open data connection");
    if (client->transfer_type == ACTIVE) {
        retr_command_active(server, client);
    } else if (client->transfer_type == PASSIVE) {
        retr_command_passive(server, client);
    }
}

void retr_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    if (!client->logged_in) {
        LOG_DEBUG("Client %d tried to RETR without logged in", client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    if (client->transfer_socket == NULL) {
        LOG_DEBUG("Client %d tried to RETR without using PORT or PASV first",
        client->fd);
        client_printf(client, "%d %s.\r\n", 425, "Use PORT or PASV first");
        return;
    }
    retr_check_file(server, client);
}
