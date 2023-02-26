/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** myftp header file
*/

#pragma once

#include <netinet/in.h>
#include <stdbool.h>

// Main functions

int myftp(char *, char *);
int check_args(int, char **);
