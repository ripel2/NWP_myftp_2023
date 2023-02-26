/*
** EPITECH PROJECT, 2023
** myftp
** File description:
** myftp logger
*/

#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

static void logger_print_time(void)
{
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    fprintf(stderr, "\033[1;90m[%02d:%02d:%02d.%3ld]\033[0m\t",
    tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec / 1000);
}

static void logger_print_level(log_level_t level)
{
    switch (level) {
        case LOG_DEBUG:
            fprintf(stderr, "\033[1;34m[DEBUG]\033[0m\t");
            break;
        case LOG_INFO:
            fprintf(stderr, "\033[1;96m[INFO]\033[0m\t");
            break;
        case LOG_WARNING:
            fprintf(stderr, "\033[1;93m[WARNING]\033[0m\t");
            break;
        case LOG_ERROR:
            fprintf(stderr, "\033[1;91m[ERROR]\033[0m\t");
            break;
        case LOG_FATAL:
            fprintf(stderr, "\033[1;31m[FATAL]\033[0m\t");
            break;
    }
}

void logger_printf(log_level_t level, const char *format, ...)
{
    va_list args;

    logger_print_time();
    logger_print_level(level);
    va_start(args, format);
    vfprintf(stderr, format, args);
    if (strlen(format) != 0 && format[strlen(format) - 1] != '\n')
        fprintf(stderr, "\n");
    va_end(args);
}

void exit_fatal(const char *format, ...)
{
    va_list args;

    logger_print_time();
    logger_print_level(LOG_FATAL);
    va_start(args, format);
    vfprintf(stderr, format, args);
    if (strlen(format) != 0 && format[strlen(format) - 1] != '\n')
        fprintf(stderr, "\n");
    va_end(args);
    exit(84);
}
