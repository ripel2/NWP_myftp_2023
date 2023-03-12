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
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>

static void send_pasv_answer(server_t *server, client_t *client)
{
    passive_t *passive = (passive_t *)client->transfer_socket;
    int port = 0;
    int ip[4] = {0};

    (void)server;
    if (getsockname(passive->fd, (struct sockaddr *)&passive->addr,
    (socklen_t *)&passive->addr_len) < 0) {
        LOG_ERROR("getsockname: %s", strerror(errno));
        client_printf(client, "%d %s.\r\n", 425, "Can't open data connection");
        return;
    }
    port = ntohs(passive->addr.sin_port);
    ip[0] = (passive->addr.sin_addr.s_addr >> 0) & 0xFF;
    ip[1] = (passive->addr.sin_addr.s_addr >> 8) & 0xFF;
    ip[2] = (passive->addr.sin_addr.s_addr >> 16) & 0xFF;
    ip[3] = (passive->addr.sin_addr.s_addr >> 24) & 0xFF;
    client_printf(client, "%d Entering Passive Mode (%d,%d,%d,%d,%d,%d).\r\n",
    227, ip[0], ip[1], ip[2], ip[3], port / 256, port % 256);
    LOG_DEBUG("Client %d PASV successful, IP %s PORT %d", client->fd,
    inet_ntoa(passive->addr.sin_addr), port);
}

void pasv_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    if (client->logged_in == false) {
        LOG_WARNING("Client %d wants to PASV but not logged in", client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    if (client->transfer_socket != NULL) {
        LOG_ERROR("Client %d already has a socket open", client->fd);
        client_printf(client, "%d %s.\r\n", 425, "Can't open data connection");
        return;
    }
    client->transfer_socket = create_passive_socket();
    if (client->transfer_socket == NULL) {
        LOG_ERROR("Client %d failed to create a socket", client->fd);
        client_printf(client, "%d %s.\r\n", 425, "Can't open data connection");
        return;
    }
    LOG_DEBUG("Client %d created a passive socket", client->fd);
    client->transfer_type = PASSIVE;
    send_pasv_answer(server, client);
}
