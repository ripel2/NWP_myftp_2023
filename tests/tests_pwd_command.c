#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(pwd_command, basic) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("PWD");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 3;
    pwd_command(&server, &client);
    cr_assert_str_eq(client.queued_message, "257 \"/\" is current directory.\r\n");
}

Test(pwd_command, not_logged_in) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("PWD");
    client.path = strdup("/");
    client.logged_in = false;
    client.buffer_size = 3;
    pwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "530", 3), 0);
}