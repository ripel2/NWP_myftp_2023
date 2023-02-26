/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** myftp logger
*/

#pragma once

#define PRINT_DEBUG 1

#include <stdio.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
} log_level_t;

void logger_printf(log_level_t, const char *, ...);
void exit_fatal(const char *, ...);

#define LOG_DEBUG(format, ...) logger(LOG_DEBUG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) logger(LOG_INFO, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) logger(LOG_WARNING, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) logger(LOG_ERROR, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) logger(LOG_FATAL, format, ##__VA_ARGS__)
