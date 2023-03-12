/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** helper function to printf in a client
*/

#include "client.h"
#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static passive_t *init_passive_socket_call(void)
{
    passive_t *passive = malloc(sizeof(passive_t));

    if (passive == NULL) {
        LOG_ERROR("malloc: %s", strerror(errno));
        return NULL;
    }
    passive->fd = -1;
    passive->addr.sin_family = AF_INET;
    passive->addr.sin_addr.s_addr = INADDR_ANY;
    passive->addr.sin_port = 0;
    passive->addr_len = sizeof(passive->addr);
    passive->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (passive->fd < 0) {
        LOG_ERROR("socket: %s", strerror(errno));
        free(passive);
        return NULL;
    }
    return passive;
}

passive_t *create_passive_socket(void)
{
    passive_t *passive = init_passive_socket_call();

    if (passive == NULL)
        return NULL;
    if (bind(passive->fd, (struct sockaddr *)&passive->addr,
    passive->addr_len) < 0) {
        LOG_ERROR("Can't create passive socket, bind: %s", strerror(errno));
        free(passive);
        return NULL;
    }
    if (listen(passive->fd, 1) < 0) {
        LOG_ERROR("Can't create passive socket, listen: %s", strerror(errno));
        free(passive);
        return NULL;
    }
    return passive;
}

void destroy_passive_socket(passive_t *passive)
{
    if (passive == NULL)
        return;
    close(passive->fd);
    free(passive);
}
