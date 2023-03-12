/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** list command
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
#include <sys/wait.h>

static void build_list_command(client_t *client, char **args)
{
    char *path = client->buffer_size <= 4 ? client->path : client->buffer + 5;
    char buf2[8192] = {0};

    if (path[0] == '/')
        sprintf(buf2, "ls -l %s", path);
    else
        sprintf(buf2, "ls -l %s/%s", client->path, path);
    sprintf(args[2], "%s &>/dev/null && %s 2>&1 | sed 's/$/\\r/'",
    buf2, buf2);
}

static void list_exec_ls_and_pipe(client_t *client, int fd)
{
    char arg2buf[8192] = {0};
    char *args[4] = {"/bin/bash", "-c", arg2buf, NULL};
    int pid = fork();
    int ret = 0;
    int status = 0;

    if (pid == 0) {
        build_list_command(client, args);
        dup2(fd, 1);
        ret = execve(args[0], args, NULL);
        exit(ret < 0 ? 2 : 0);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            thread_send(client,
            "226 Closing data connection, transfer successful.\r\n");
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            thread_send(client, "550 Failed to execute ls.\r\n");
    }
}

void list_command_passive(server_t *server, client_t *client)
{
    passive_t *passive = client->transfer_socket;
    int new_socket = 0;
    pid_t pid = 0;

    (void)server;
    pid = fork();
    if (pid < 0) {
        LOG_ERROR("Fork failed");
        client_printf(client, "%d %s.\r\n", 550, "Fork failed");
        return;
    }
    if (pid == 0) {
        new_socket = accept(passive->fd, (struct sockaddr *)&passive->addr,
        (socklen_t *)&passive->addr_len);
        list_exec_ls_and_pipe(client, new_socket);
        exit(0);
    } else {
        destroy_passive_socket(passive);
        client->transfer_socket = NULL;
    }
}

void list_command_active(server_t *server, client_t *client)
{
    active_t *active = client->transfer_socket;
    pid_t pid = 0;

    (void)server;
    pid = fork();
    if (pid < 0) {
        LOG_ERROR("Fork failed");
        client_printf(client, "%d %s.\r\n", 550, "Fork failed");
        return;
    }
    if (pid == 0) {
        list_exec_ls_and_pipe(client, active->fd);
        exit(0);
    } else {
        destroy_active_socket(active);
        client->transfer_socket = NULL;
    }
}

void list_command(server_t *server, client_t *client)
{
    FD_SET(client->fd, &server->write_fds);
    if (!client->logged_in) {
        LOG_WARNING("Client %d tried to LIST without logged in", client->fd);
        client_printf(client, "%d %s.\r\n", 530, "Not logged in");
        return;
    }
    if (client->transfer_socket == NULL) {
        LOG_DEBUG("Client %d tried to LIST without using PORT or PASV first",
        client->fd);
        client_printf(client, "%d %s.\r\n", 425, "Use PORT or PASV first");
        return;
    }
    client_printf(client, "%d %s.\r\n", 150,
    "Opening ASCII mode data connection for file list");
    if (client->transfer_type == ACTIVE) {
        list_command_active(server, client);
    } else if (client->transfer_type == PASSIVE) {
        list_command_passive(server, client);
    }
}
