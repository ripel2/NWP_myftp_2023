/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** cwd command
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

static void resolve_dir(client_t *client, char *path)
{
    char *real_path = NULL;

    if (chdir(path) == -1) {
        LOG_ERROR("Failed to resolve directory %s", path);
        return;
    }
    real_path = getcwd(NULL, 0);
    if (real_path == NULL) {
        LOG_ERROR("Failed to resolve directory %s", path);
        return;
    }
    free(client->path);
    client->path = real_path;
}

static void cwd_change_dir(client_t *client, char *path, int return_code)
{
    struct stat st;

    if (stat(path, &st) == -1) {
        LOG_ERROR("Failed to change directory to %s, stat failed", path);
        client_printf(client, "%d %s.\r\n", 550, "Failed to change directory");
        return;
    }
    if (!S_ISDIR(st.st_mode)) {
        LOG_ERROR("Failed to change directory to %s, not a directory", path);
        client_printf(client, "%d %s.\r\n", 550, "Failed to change directory");
        return;
    }
    free(client->path);
    client->path = path;
    resolve_dir(client, path);
    client_printf(client, "%d %s.\r\n", return_code,
    "Requested file action okay, completed");
    LOG_INFO("Client %d changed directory to %s", client->fd, client->path);
}

static void cwd_create_path(client_t *client, char *path, int return_code)
{
    char *new_path = NULL;

    if (path[0] == '/') {
        new_path = strdup(path);
    } else {
        new_path = malloc(sizeof(char) * (strlen(client->path) +
        strlen(path) + 2));
        if (new_path != NULL)
            sprintf(new_path, "%s/%s", client->path, path);
    }
    if (new_path == NULL) {
        client_printf(client, "%d %s.\r\n", 550, "Failed to change directory");
        return;
    }
    cwd_change_dir(client, new_path, return_code);
}

void cwd_inner(server_t *server, client_t *client, int return_code)
{
    FD_SET(client->fd, &server->write_fds);
    if (!client->logged_in) {
        LOG_ERROR("Client %d sent CWD command while not logged in",
        client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    LOG_DEBUG("Client %d sent CWD command", client->fd);
    if (client->buffer[3] == '\0' ||
    (client->buffer[3] == ' ' && client->buffer[4] == '\0')) {
        client_printf(client, "%d %s.\r\n", 550,
        "Failed to change directory");
        return;
    }
    if (strcspn(client->buffer + 4, "\r\n\t ") == 0) {
        client_printf(client, "%d %s.\r\n", 550,
        "Failed to change directory");
        return;
    }
    cwd_create_path(client, client->buffer + 4, return_code);
}

void cwd_command(server_t *server, client_t *client)
{
    cwd_inner(server, client, 250);
}
