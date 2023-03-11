#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(cdup_command, not_logged_in) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CDUP");
    client.buffer_size = 3;
    cdup_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "530", 3), 0);
}

Test(cdup_command, basic_1) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CDUP");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 4;
    cdup_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "200", 3), 0);
}

Test(cdup_command, basic_2) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CDUP");
    client.path = strdup("/tmp");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 4;
    cdup_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "200", 3), 0);
    cr_assert_not_null(strstr(client.path, "/"));
}
