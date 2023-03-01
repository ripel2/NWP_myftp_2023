/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** client handler
*/

#include "server.h"
#include "client.h"
#include "logger.h"
#include "commands.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void handle_clients(server_t *server)
{
    client_t *client = (client_t *)server->clients;
    client_t *next = NULL;
    bool client_removed = false;

    while (client != NULL) {
        next = client->next;
        client_removed = handle_client_read(server, client);
        if (client_removed == false)
            handle_client_write(server, client);
        client = next;
    }
}
