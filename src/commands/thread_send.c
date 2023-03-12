/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** thread send
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

void thread_send(client_t *client, char *buffer)
{
    fd_set write_fds = {0};
    int ret = 0;

    FD_SET(client->fd, &write_fds);
    ret = select(client->fd + 1, NULL, &write_fds, NULL, NULL);
    if (ret < 0) {
        LOG_ERROR("Select failed");
        exit(0);
    }
    if (FD_ISSET(client->fd, &write_fds)) {
        ret = write(client->fd, buffer, strlen(buffer));
        LOG_DEBUG("Sent %d bytes to client from forked thread", ret);
    }
}
