/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** stor command
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
#include <fcntl.h>

static void stor_transfer(client_t *client, int ns)
{
    int fd = open(client->buffer + 5, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char buffer[1024] = {0};
    ssize_t ret = 0;

    if (fd < 0)
        file_open_error(client);
    ret = read(ns, buffer, 1024);
    while (ret > 0) {
        if (write(fd, buffer, ret) < 0)
            file_write_error(client);
        memset(buffer, 0, 1024);
        ret = read(ns, buffer, 1024);
    }
    close(ns);
    file_transfer_success(client);
}

static void stor_command_active(client_t *client)
{
    active_t *active = client->transfer_socket;
    pid_t pid = 0;

    pid = fork();
    if (pid < 0) {
        LOG_ERROR("Fork failed");
        thread_send(client, "550 Fork failed.\r\n");
        return;
    }
    if (pid == 0) {
        chdir(client->path);
        stor_transfer(client, active->fd);
        exit(0);
    } else {
        destroy_active_socket(active);
        client->transfer_socket = NULL;
    }
}

static void stor_command_passive(client_t *client)
{
    passive_t *passive = client->transfer_socket;
    pid_t pid = 0;
    int new_socket = 0;

    pid = fork();
    if (pid < 0) {
        LOG_ERROR("Fork failed");
        thread_send(client, "550 Fork failed.\r\n");
        return;
    }
    if (pid == 0) {
        chdir(client->path);
        new_socket = accept(passive->fd, (struct sockaddr *)&passive->addr,
        (socklen_t *)&passive->addr_len);
        stor_transfer(client, new_socket);
        exit(0);
    } else {
        destroy_passive_socket(passive);
        client->transfer_socket = NULL;
    }
}

void stor_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    if (!client->logged_in) {
        LOG_DEBUG("Client %d tried to STOR without logged in", client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    if (client->transfer_socket == NULL) {
        LOG_DEBUG("Client %d tried to STOR without using PORT or PASV first",
        client->fd);
        client_printf(client, "%d %s.\r\n", 425, "Use PORT or PASV first");
        return;
    }
    client_printf(client, "%d %s.\r\n", 150,
    "Opening data connection for file transfer");
    if (client->transfer_type == ACTIVE) {
        stor_command_active(client);
    } else if (client->transfer_type == PASSIVE) {
        stor_command_passive(client);
    }
}
