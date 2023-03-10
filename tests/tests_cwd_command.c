#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(cwd_command, not_logged_in) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD");
    client.buffer_size = 3;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "530", 3), 0);
}

Test(cwd_command, basic_1) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD /");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 5;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "250", 3), 0);
}

Test(cwd_command, basic_2) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD /tmp");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 8;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "250", 3), 0);
    cr_assert_not_null(strstr(client.path, "/tmp"));
}

Test(cwd_command, resolve_1) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD /tmp/..");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 11;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "250", 3), 0);
    cr_assert_not_null(strstr(client.path, "/"));
}

Test(cwd_command, resolve_2) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD /tmp/../..");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 14;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "250", 3), 0);
    cr_assert_not_null(strstr(client.path, "/"));
}

Test(cwd_command, resolve_3) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD /.//tmp/../.");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 17;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "250", 3), 0);
    cr_assert_not_null(strstr(client.path, "/"));
}

Test(cwd_command, resolve_4) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD /tmp/./../tmp");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 18;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "250", 3), 0);
    cr_assert_not_null(strstr(client.path, "/tmp"));
}

Test(cwd_command, file) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD /proc/stat");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 14;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "550", 3), 0);
}

Test(cwd_command, no_args) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 3;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "550", 3), 0);
}

Test(cwd_command, no_args_2) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD ");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 4;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "550", 3), 0);
}

Test(cwd_command, no_args_3) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("CWD  ");
    client.path = strdup("/");
    client.username = strdup("Anonymous");
    client.logged_in = true;
    client.buffer_size = 4;
    cwd_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "550", 3), 0);
}