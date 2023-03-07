/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** login commands (USER and PASS)
*/

#include "myftp.h"
#include "server.h"
#include "commands.h"
#include "client.h"
#include "logger.h"

#include <string.h>

void user_command(server_t *server, client_t *client)
{
    char *username = strtok(client->buffer, " ");

    FD_SET(client->fd, &server->write_fds);
    if (username)
        username = strtok(NULL, " ");
    if (!username || strcspn(username, " \n\t\r\v\f") == 0) {
        LOG_DEBUG("Client %d sent invalid USER command", client->fd);
        client_printf(client, "%d %s.\r\n", 501, "Invalid arguments");
        return;
    }
    client->username = strdup(username);
    if (client->username == NULL) {
        LOG_ERROR("malloc failed on client %d", client->fd);
        client_printf(client, "%d %s.\r\n", 500, "Internal server error");
        return;
    }
    LOG_INFO("Client %d wants to login as %s", client->fd, username);
    client_printf(client, "%d %s.\r\n", 331, "User name okay, need password");
}

static void pass_command_check_password(server_t *server, client_t *client,
char *password)
{
    if ((strcmp(client->username, "Anonymous") == 0) &&
    (password == NULL || (strcspn(password, " \n\t\r\v\f") == 0))) {
        LOG_INFO("Client %d logged in successfully as %s", client->fd,
        client->username);
        client->logged_in = true;
        client->path = strdup(server->default_home);
        client_printf(client, "%d %s.\r\n", 230, "User logged in, proceed");
    } else {
        LOG_INFO("Client %d sent invalid password", client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
    }
}

void pass_command(server_t *server, client_t *client)
{
    char *password = NULL;

    FD_SET(client->fd, &server->write_fds);
    if (client->buffer[4] != ' ') {
        LOG_DEBUG("Client %d sent invalid PASS command", client->fd);
        client_printf(client, "%d %s.\r\n", 501, "Invalid arguments");
        return;
    }
    password = strtok(client->buffer, " ");
    FD_SET(client->fd, &server->write_fds);
    if (password)
        password = strtok(NULL, " ");
    else
        password = "";
    if (client->username == NULL) {
        LOG_DEBUG("Client %d sent PASS command before USER", client->fd);
        client_printf(client, "%d %s.\r\n", 503, "Bad sequence of commands");
        return;
    }
    pass_command_check_password(server, client, password);
}

void quit_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    LOG_INFO("Client %d sent QUIT command", client->fd);
    client_printf(client, "%d %s.\r\n", 221,
    "Service closing control connection");
    client->needs_exit = true;
}
