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

static void strip_line_end(client_t *client)
{
    if (client->buffer[client->buffer_size - 1] == '\n') {
        client->buffer[client->buffer_size - 1] = '\0';
        client->buffer_size--;
    }
    if (client->buffer[client->buffer_size - 1] == '\r') {
        client->buffer[client->buffer_size - 1] = '\0';
        client->buffer_size--;
    }
}

static bool read_client_line_finish(client_t *client, ssize_t ret)
{
    while (ret == READ_BUFFER_SIZE) {
        client->buffer_size += ret;
        client->buffer = realloc(client->buffer,
        client->buffer_size + READ_BUFFER_SIZE + 1);
        if (client->buffer == NULL) {
            return false;
        }
        ret = read(client->fd,
        client->buffer + client->buffer_size, READ_BUFFER_SIZE);
        if (ret == -1) {
            client->buffer_size = -1;
            return true;
        }
    }
    client->buffer_size += ret;
    client->buffer[client->buffer_size] = '\0';
    strip_line_end(client);
    return true;
}

static bool read_client_line(client_t *client)
{
    ssize_t ret = 0;

    client->buffer_size = 0;
    if (client->buffer == NULL) {
        client->buffer = malloc(sizeof(char) * (READ_BUFFER_SIZE + 1));
        if (client->buffer == NULL)
            return false;
    }
    ret = read(client->fd, client->buffer, READ_BUFFER_SIZE);
    if (ret == -1) {
        client->buffer_size = -1;
        return true;
    }
    return read_client_line_finish(client, ret);
}

bool handle_client_read(server_t *server, client_t *client)
{
    if (FD_ISSET(client->fd, &server->read_fds) == 0)
        return false;
    if (read_client_line(client) == false)
        LOG_ERROR("malloc failed on client %d", client->fd);
    if (client->buffer_size <= 0) {
        remove_client(&server->clients, client->fd);
        return true;
    }
    LOG_DEBUG("Received %ld bytes from client %d", client->buffer_size,
    client->fd);
    FD_CLR(client->fd, &server->read_fds);
    handle_command(server, client);
    return false;
}

void handle_client_write(server_t *server, client_t *client)
{
    if (FD_ISSET(client->fd, &server->write_fds) == 0) {
        LOG_DEBUG("Client %d is not ready to write", client->fd);
        return;
    }
    if (client->queued_message == NULL) {
        LOG_DEBUG("Client %d has no message to send", client->fd);
        return;
    }
    LOG_DEBUG("Sending %ld bytes to client %d", client->queued_message_size,
    client->fd);
    write(client->fd, client->queued_message, client->queued_message_size);
    free(client->queued_message);
    client->queued_message = NULL;
    client->queued_message_size = 0;
    FD_CLR(client->fd, &server->write_fds);
}
