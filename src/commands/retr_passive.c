/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** retr command
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

static void retr_passive_transfer(client_t *client, int ns)
{
    char *path = client->buffer + 5;
    int fd = open(path, O_RDONLY);
    char buffer[1024] = {0};
    ssize_t ret = 0;

    if (fd < 0) {
        LOG_ERROR("Failed to open file");
        thread_send(client, "550 Failed to open file.\r\n");
        exit(0);
    }
    ret = read(fd, buffer, 1024);
    while (ret > 0) {
        write(ns, buffer, ret);
        memset(buffer, 0, 1024);
        ret = read(fd, buffer, 1024);
    }
    close(ns);
    thread_send(client,
    "226 Closing data connection, file transfer successful.\r\n");
}

void retr_command_passive(server_t *server, client_t *client)
{
    passive_t *passive = client->transfer_socket;
    int new_socket = 0;
    pid_t pid = 0;

    (void)server;
    pid = fork();
    if (pid < 0) {
        thread_send(client, "550 Fork failed.\r\n");
        return;
    }
    if (pid == 0) {
        chdir(client->path);
        new_socket = accept(passive->fd, (struct sockaddr *)&passive->addr,
        (socklen_t *)&passive->addr_len);
        retr_passive_transfer(client, new_socket);
        exit(0);
    } else {
        destroy_passive_socket(passive);
        client->transfer_socket = NULL;
    }
}
