/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** header file for client handling
*/

#pragma once

#include <netinet/in.h>
#include <stdbool.h>

#include "server.h"

/* 1024 is the upper limit for select and fds 0, 1 and 2 cannot be used */
/* thus the absolute maximum of clients is 1021 */
#define MAX_CLIENTS __FD_SETSIZE - 3

#define READ_BUFFER_SIZE 1024

typedef struct server_s server_t;

typedef enum transfer_type {
    NONE,
    ACTIVE,
    PASSIVE
} transfer_type_t;

typedef struct active_s {
    int fd;
    struct sockaddr_in addr;
    int addr_len;
} active_t;

typedef struct passive_s {
    int fd;
    struct sockaddr_in addr;
    int addr_len;
} passive_t;

typedef struct client_s {
    int fd;
    char *username;
    char *path;
    char *buffer;
    ssize_t buffer_size;
    bool logged_in;
    transfer_type_t transfer_type;
    void *transfer_socket;
    struct client_s *next;
    char *queued_message;
    ssize_t queued_message_size;
    bool needs_exit;
} client_t;

typedef client_t *client_list_t;

size_t get_client_count(client_list_t);

bool add_client(client_list_t *, int fd);
bool remove_client(client_list_t *, int fd);

ssize_t client_printf(client_t *, const char *, ...);
void thread_send(client_t *, char *);

void welcome_client(server_t *, client_t *);

passive_t *create_passive_socket(void);
void destroy_passive_socket(passive_t *);

bool connect_active_socket(active_t *);
active_t *create_active_socket(char *, int);
void destroy_active_socket(active_t *);
