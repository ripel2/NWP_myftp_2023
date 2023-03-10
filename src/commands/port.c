/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** port command
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

static void create_port_struct(client_t *client, int *ip, int *port)
{
    char ipstr[16] = {0};
    int cport = 0;

    sprintf(ipstr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    cport = port[0] * 256 + port[1];
    client->transfer_socket = create_active_socket(ipstr, cport);
    if (client->transfer_socket == NULL) {
        client_printf(client, "%d %s.\r\n", 425, "Can't open data connection");
        LOG_DEBUG("Client %d can't open data connection, failed", client->fd);
        return;
    }
    client->transfer_type = ACTIVE;
    client_printf(client, "%d %s.\r\n", 200, "Okay");
    LOG_DEBUG("Client %d PORT command successful %s:%d", client->fd,
    ip, cport);
}

static bool port_parse_buffer(int *ip, int *port, client_t *client)
{
    if (sscanf(client->buffer, "PORT %d,%d,%d,%d,%d,%d",
    &ip[0], &ip[1], &ip[2], &ip[3], &port[0], &port[1]) != 6) {
        client_printf(client, "%d %s.\r\n", 501, "Invalid arguments");
        LOG_DEBUG("Client %d sent invalid arguments for PORT command",
        client->fd);
        return false;
    }
    return true;
}

void port_command(server_t *server, client_t *client)
{
    int ip[4] = {0};
    int port[2] = {0};

    FD_SET(client->fd, &server->write_fds);
    if (client->logged_in == false) {
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        LOG_DEBUG("Client %d not logged in", client->fd);
        return;
    }
    if (client->transfer_socket != NULL) {
        client_printf(client, "%d %s.\r\n", 425, "Can't open data connection");
        LOG_DEBUG("Client %d can't open data connection, already opened",
        client->fd);
        return;
    }
    if (port_parse_buffer(ip, port, client) == false)
        return;
    create_port_struct(client, ip, port);
}
