#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(get_client_count, empty) {
    server_t server = {0};

    cr_assert_eq(get_client_count(server.clients), 0);
}

Test(get_client_count, one) {
    server_t server = {0};
    client_t client = {0};

    server.clients = &client;
    cr_assert_eq(get_client_count(server.clients), 1);
}

Test(get_client_count, two) {
    server_t server = {0};
    client_t client = {0};
    client_t client2 = {0};

    server.clients = &client;
    client.next = &client2;
    cr_assert_eq(get_client_count(server.clients), 2);
}

Test(get_client_count, three) {
    server_t server = {0};
    client_t client = {0};
    client_t client2 = {0};
    client_t client3 = {0};

    server.clients = &client;
    client.next = &client2;
    client2.next = &client3;
    cr_assert_eq(get_client_count(server.clients), 3);
}

Test(client_printf, basic) {
    client_t client = {0};

    client_printf(&client, "%i %s\r\n", 200, "OK");
    cr_assert_str_eq(client.queued_message, "200 OK\r\n");
}

Test(remove_client, basic) {
    client_list_t list = {0};
    add_client(&list, 0);
    cr_assert_eq(get_client_count(list), 1);
    remove_client(&list, 0);
    cr_assert_eq(get_client_count(list), 0);
}

Test(remove_client, two) {
    client_list_t list = {0};
    add_client(&list, 0);
    add_client(&list, 1);
    cr_assert_eq(get_client_count(list), 2);
    remove_client(&list, 0);
    cr_assert_eq(get_client_count(list), 1);
    remove_client(&list, 1);
    cr_assert_eq(get_client_count(list), 0);
}

Test(remove_client, unknown_fd) {
    client_list_t list = {0};
    add_client(&list, 0);
    cr_assert_eq(get_client_count(list), 1);
    remove_client(&list, 42);
    cr_assert_eq(get_client_count(list), 1);
}