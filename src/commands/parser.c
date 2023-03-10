/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** commands parser
*/

#include "commands.h"
#include "logger.h"

#include <string.h>

const char *COMMANDS[] = {
    "USER",
    "PASS",
    "NOOP",
    "PWD",
    "CWD",
    "CDUP",
    "QUIT",
    "HELP",
    "DELE",
    "PASV",
    "RETR",
    "STOR",
    "LIST",
    "PORT",
    NULL
};

static void (*COMMANDS_FUNCS[])(server_t *server, client_t *client) = {
    &user_command,
    &pass_command,
    &noop_command,
    &pwd_command,
    &cwd_command,
    &cdup_command,
    &quit_command,
    &help_command,
    &dele_command,
    &pasv_command,
    &retr_command,
    &stor_command,
    &list_command,
    &port_command,
    NULL
};

void handle_command(server_t *server, client_t *client)
{
    for (size_t c = 0; COMMANDS[c]; c++) {
        if (strncmp(client->buffer, COMMANDS[c], strlen(COMMANDS[c])) == 0) {
            COMMANDS_FUNCS[c](server, client);
            return;
        }
    }
    FD_SET(client->fd, &server->write_fds);
    LOG_DEBUG("Client %d sent unknown command", client->fd);
    client_printf(client, "%d %s.\r\n", 500, "Unknown command");
}
