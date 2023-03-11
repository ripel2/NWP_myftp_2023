#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

#include <unistd.h>
#include <stdio.h>

Test(create_passive_socket, basic_1) {
    passive_t *passive = create_passive_socket();
    int fd = passive->fd;

    cr_assert_not_null(passive);
    cr_assert_neq(passive->fd, -1);
    cr_assert_eq(passive->addr.sin_family, AF_INET);
    cr_assert_eq(passive->addr.sin_addr.s_addr, INADDR_ANY);
    cr_assert_eq(passive->addr.sin_port, 0);
    destroy_passive_socket(passive);
    cr_assert_eq(close(fd), -1);
}
