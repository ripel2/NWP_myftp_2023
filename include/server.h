/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** server
*/

#pragma once

#include "client.h"

#define LISTEN_BACKLOG 10

typedef struct client_s client_t;
typedef struct client_s *client_list_t;

typedef struct server_s {
    int fd;
    int max_fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    fd_set read_fds;
    fd_set write_fds;
    client_list_t clients;
    char *default_home;
} server_t;

void init_server(server_t *, char *port, char *path);
void handle_clients(server_t *);
void server_loop(server_t *);
void destroy_server(server_t *);
