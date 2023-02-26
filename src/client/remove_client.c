/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** remove client
*/

#include "client.h"
#include "logger.h"

#include <stdlib.h>

bool remove_client(client_list_t *list, int fd)
{
    client_t *client = *list;

    if (client->fd == fd) {
        *list = client->next;
        free(client);
        LOG_DEBUG("Removed client %d from list", fd);
        return true;
    }
    while (client->next) {
        if (client->next->fd == fd) {
            client->next = client->next->next;
            free(client->next);
            LOG_DEBUG("Removed client %d from list", fd);
            return true;
        }
        client = client->next;
    }
    LOG_DEBUG("Client %d not found... what ??", fd);
    return false;
}
