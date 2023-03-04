#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(user_command, basic) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("USER Anonymous");
    client.buffer_size = 16;
    user_command(&server, &client);
    cr_assert_str_eq(client.username, "Anonymous");
}

Test(user_command, no_username) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("USER");
    client.buffer_size = 5;
    client.username = NULL;
    user_command(&server, &client);
    cr_assert_null(client.username);
}
