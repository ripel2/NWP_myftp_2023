#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "commands.h"

Test(handle_command, basic) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("NOOP");
    client.buffer_size = 16;
    handle_command(&server, &client);
}

Test(handle_command, user) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("USER Anonymous");
    client.buffer_size = 16;
    handle_command(&server, &client);
    cr_assert_str_eq(client.username, "Anonymous");
}

Test(handle_command, no_command) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("");
    client.buffer_size = 0;
    handle_command(&server, &client);
    cr_assert_null(client.username);
}

Test(handle_command, bad_command) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("KETCHAK");
    client.buffer_size = 8;
    handle_command(&server, &client);
    cr_assert_null(client.username);
}