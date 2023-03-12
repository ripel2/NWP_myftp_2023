/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** active socket functions
*/

#include "client.h"
#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

static active_t *init_active_socket_values(char *ip, int port)
{
    active_t *active = malloc(sizeof(active_t));

    if (active == NULL) {
        LOG_ERROR("malloc: %s", strerror(errno));
        return NULL;
    }
    active->fd = -1;
    active->addr.sin_family = AF_INET;
    active->addr.sin_addr.s_addr = inet_addr(ip);
    active->addr.sin_port = htons(port);
    active->addr_len = sizeof(active->addr);
    return active;
}

bool connect_active_socket(active_t *active)
{
    active->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (active->fd < 0) {
        LOG_ERROR("Can't create active socket, socket: %s", strerror(errno));
        free(active);
        return false;
    }
    if (connect(active->fd, (struct sockaddr *)&active->addr,
    (socklen_t)active->addr_len) < 0) {
        LOG_ERROR("Can't create active socket, connect: %s", strerror(errno));
        free(active);
        return false;
    }
    return true;
}

active_t *create_active_socket(char *ip, int port)
{
    active_t *active = init_active_socket_values(ip, port);

    if (active == NULL) {
        return NULL;
    }
    if (connect_active_socket(active) == false) {
        return NULL;
    }
    return active;
}

void destroy_active_socket(active_t *active)
{
    if (active->fd != -1)
        close(active->fd);
    free(active);
}
