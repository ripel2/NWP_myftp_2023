/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** remove client
*/

#include "client.h"
#include "logger.h"

#include <stdlib.h>

static void free_client(client_t *client)
{
    free(client->username);
    free(client->path);
    free(client->buffer);
    free(client->queued_message);
    free(client);
}

bool remove_client(client_list_t *list, int fd)
{
    client_t *client = *list;
    client_t *tmp = NULL;

    if (client->fd == fd) {
        *list = client->next;
        free_client(client);
        LOG_DEBUG("Removed client %d from list", fd);
        return true;
    }
    while (client->next) {
        if (client->next->fd == fd) {
            tmp = client->next;
            client->next = client->next->next;
            free_client(tmp);
            LOG_DEBUG("Removed client %d from list", fd);
            return true;
        }
        client = client->next;
    }
    return false;
}
