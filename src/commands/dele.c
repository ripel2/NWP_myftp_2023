/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** help command
*/

#include "myftp.h"
#include "server.h"
#include "commands.h"
#include "client.h"
#include "logger.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

static void dele_delete_file(client_t *client, char *path)
{
    if (!path_exists(path)) {
        LOG_DEBUG("Client %d tried to delete a non-existing file", client->fd);
        client_printf(client, "%d %s.\r\n", 550, "File not found");
        return;
    }
    if (!path_is_file(path)) {
        LOG_DEBUG("Client %d tried to delete a non-file", client->fd);
        client_printf(client, "%d %s.\r\n", 550, "Not a file");
        return;
    }
    if (remove(path) != 0) {
        LOG_DEBUG("Client %d tried to delete a file he doesn't own",
        client->fd);
        client_printf(client, "%d %s.\r\n", 550, "Permission denied");
        return;
    }
    LOG_DEBUG("Client %d deleted file %s", client->fd, path);
    client_printf(client, "%d %s.\r\n", 250, "File deleted");
}

void dele_command(server_t *server, client_t *client)
{
    char path[PATH_MAX];

    FD_SET(client->fd, &server->write_fds);
    if (client->username == NULL) {
        LOG_DEBUG("Client %d sent command before logging in", client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    if (client->buffer[4] != ' ' ||
    strcspn(client->buffer + 5, " \n\r\t\v\f") == 0) {
        LOG_DEBUG("Client %d sent invalid command", client->fd);
        client_printf(client, "%d %s.\r\n", 501, "Invalid arguments");
        return;
    }
    sprintf(path, "%s/%s", client->path, client->buffer + 5);
    dele_delete_file(client, path);
}
