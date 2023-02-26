/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** myftp main function
*/

#include "myftp.h"
#include "server.h"

int myftp(char *port, char *path)
{
    server_t server;

    init_server(&server, port, path);
    server_loop(&server);
    destroy_server(&server);
    return 0;
}
