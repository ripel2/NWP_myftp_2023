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

const char *HELP_MESSAGES[][2] = {
    {"USER", "Specify user for authentication"},
    {"PASS", "Specify password for authentication"},
    {"CWD", "Change working directory"},
    {"CDUP", "Change to parent directory"},
    {"QUIT", "Disconnection"},
    {"DELE", "Delete file on the server"},
    {"PWD", "Print working directory"},
    {"PASV", "Enable \"passive\" mode for data transfer"},
    {"PORT", "Enable \"active\" mode for data transfer"},
    {"HELP", "List available commands"},
    {"NOOP", "Do nothing"},
    {"RETR", "Download file from server to client"},
    {"STOR", "Upload file from client to server"},
    {"LIST", "List files in current directory"},
    {NULL, NULL}
};

static void help_build_command_list(char *buffer)
{
    strcat(buffer, "214-The following commands are recognized:\r\n");
    for (size_t c = 0; HELP_MESSAGES[c][0] != NULL; c++) {
        strcat(buffer, "\t");
        strcat(buffer, HELP_MESSAGES[c][0]);
        if (HELP_MESSAGES[c + 1][0] != NULL)
            strcat(buffer, "\t");
    }
    strcat(buffer, "\r\n214 Help OK.\r\n");
}

static void help_command_list(client_t *client)
{
    char *buffer = NULL;
    size_t length = 45;

    for (size_t c = 0; HELP_MESSAGES[c][0] != NULL; c++)
        length += strlen(HELP_MESSAGES[c][0]) + 3;
    buffer = calloc(length + 1, sizeof(char));
    if (buffer == NULL) {
        LOG_ERROR("Failed to allocate memory for HELP command");
        client_printf(client, "%d %s.\r\n", 500, "Failed to allocate memory");
        return;
    }
    help_build_command_list(buffer);
    client_printf(client, "%s", buffer);
    free(buffer);
}

static void help_command_specific(client_t *client)
{
    char *command = client->buffer + 5;
    size_t length = strcspn(command, " \n\t\r\v\f");

    command[length] = '\0';
    for (size_t c = 0; HELP_MESSAGES[c][0] != NULL; c++) {
        if (strcmp(command, HELP_MESSAGES[c][0]) == 0) {
            client_printf(client, "214 %s.\r\n", HELP_MESSAGES[c][1]);
            return;
        }
    }
    client_printf(client, "%d %s.\r\n", 500,
    "Unknown command for HELP command");
}

void help_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    if (!client->logged_in) {
        LOG_DEBUG("Client %d sent HELP command while not logged in", client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    LOG_DEBUG("Client %d sent HELP command", client->fd);
    if (strcspn(client->buffer + 5, " \n\t\r\v\f") == 0) {
        help_command_list(client);
    } else {
        help_command_specific(client);
    }
}