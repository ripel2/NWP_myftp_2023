#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(pass_command, basic) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    server.default_home = strdup("/tmp");
    client.fd = 0;
    client.username = strdup("Anonymous");
    client.buffer = strdup("PASS ");
    client.buffer_size = 5;
    pass_command(&server, &client);
    cr_assert(client.logged_in);
}

Test(pass_command, no_password) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    server.default_home = strdup("/tmp");
    client.fd = 0;
    client.username = strdup("Anonymous");
    client.buffer = strdup("PASS");
    client.buffer_size = 4;
    pass_command(&server, &client);
    cr_assert_not(client.logged_in);
}

Test(pass_command, no_username) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    server.default_home = strdup("/tmp");
    client.fd = 0;
    client.username = NULL;
    client.buffer = strdup("PASS");
    client.buffer_size = 4;
    pass_command(&server, &client);
    cr_assert_not(client.logged_in);
}

Test(pass_command, bad_username) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    server.default_home = strdup("/tmp");
    client.fd = 0;
    client.username = strdup("ketchak");
    client.buffer = strdup("PASS ");
    client.buffer_size = 5;
    pass_command(&server, &client);
    cr_assert_not(client.logged_in);
}
