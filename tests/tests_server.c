#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(init_server, init_server) {
    server_t server = {0};

    init_server(&server, "4242", "/tmp");
    cr_assert_eq(server.addr.sin_port, htons(4242));
    cr_assert_str_eq(server.default_home, "/tmp");
    cr_assert_gt(server.max_fd, 2);
    close(server.fd);
}
