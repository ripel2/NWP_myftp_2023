/*
** EPITECH PROJECT, 2023
** my_ftp
** File description:
** client count
*/

#include "client.h"

#include <sys/types.h>

size_t get_client_count(client_list_t list)
{
    size_t count = 0;
    client_t *client = list;

    while (client) {
        count++;
        client = client->next;
    }
    return count;
}
