/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** myftp main entry point
*/

#include "myftp.h"

int main(int ac, char **av)
{
    if (check_args(ac, av) == 84)
        return 84;
    myftp(av[1], av[2]);
    return 0;
}
