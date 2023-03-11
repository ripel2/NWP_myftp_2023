#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(noop_command, basic) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    client.fd = 0;
    client.buffer = strdup("NOOP");
    client.buffer_size = 4;
    noop_command(&server, &client);
    cr_assert_eq(strncmp(client.queued_message, "200", 3), 0);
}
