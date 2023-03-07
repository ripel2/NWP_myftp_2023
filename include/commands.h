/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** myftp commands
*/

#pragma once

#include "server.h"
#include "client.h"

extern const char *COMMANDS[];

void user_command(server_t *, client_t *);
void pass_command(server_t *, client_t *);
void noop_command(server_t *, client_t *);
void pwd_command(server_t *, client_t *);
void cwd_inner(server_t *, client_t *, int);
void cwd_command(server_t *, client_t *);
void cdup_command(server_t *, client_t *);
void quit_command(server_t *, client_t *);
void help_command(server_t *, client_t *);
void pasv_command(server_t *, client_t *);

void handle_command(server_t *, client_t *);
