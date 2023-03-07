/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** add client
*/

#include "client.h"
#include "logger.h"

#include <stdlib.h>

static void init_client(client_t *client, int fd)
{
    client->fd = fd;
    client->username = NULL;
    client->path = NULL;
    client->buffer = NULL;
    client->buffer_size = 0;
    client->logged_in = false;
    client->transfer_type = NONE;
    client->transfer_socket = NULL;
    client->next = NULL;
    client->queued_message = NULL;
    client->needs_exit = false;
}

bool add_client(client_list_t *list, int fd)
{
    client_t *client = malloc(sizeof(client_t));

    if (!client)
        return false;
    init_client(client, fd);
    LOG_DEBUG("Created client %d and added it to list", fd);
    if (!*list) {
        *list = client;
        return true;
    }
    client->next = *list;
    *list = client;
    return true;
}
