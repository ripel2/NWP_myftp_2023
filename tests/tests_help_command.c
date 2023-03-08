#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "commands.h"

Test(help_command, basic_no_args) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("HELP");
    client.buffer_size = 16;
    client.logged_in = true;
    handle_command(&server, &client);
    cr_assert(strncmp(client.queued_message, "214-The following commands are recognized", 40) == 0);
}

Test(help_command, test_one_function) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("HELP USER");
    client.buffer_size = 16;
    client.logged_in = true;
    handle_command(&server, &client);
    cr_assert(strncmp(client.queued_message, "214", 3) == 0);

}