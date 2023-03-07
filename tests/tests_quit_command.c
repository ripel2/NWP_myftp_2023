#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(quit_command, basic) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("QUIT");
    client.buffer_size = 4;
    quit_command(&server, &client);
    cr_assert(client.needs_exit);
}
