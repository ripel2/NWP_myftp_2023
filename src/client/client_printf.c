/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** helper function to printf in a client
*/

#include "client.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

ssize_t client_printf(client_t *client, const char *format, ...)
{
    va_list args;
    ssize_t ret;
    char buffer[BUFSIZ];

    va_start(args, format);
    ret = vsnprintf(buffer, BUFSIZ, format, args);
    va_end(args);
    if (ret == -1)
        return ret;
    client->queued_message = strdup(buffer);
    if (client->queued_message == NULL)
        return -1;
    client->queued_message_size = ret;
    return ret;
}
