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

void handle_command(server_t *, client_t *);
