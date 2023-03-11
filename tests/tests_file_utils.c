#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"
#include "server.h"
#include "client.h"
#include "commands.h"

Test(path_exists, basic_1) {
    cr_assert_eq(path_exists("/"), true);
}

Test(path_exists, basic_2) {
    cr_assert_eq(path_exists("/tmp"), true);
}

Test(path_exists, basic_3) {
    cr_assert_eq(path_exists("/tmp/.."), true);
}

Test(path_exists, basic_4) {
    cr_assert_eq(path_exists("///./"), true);
}

Test(path_exists, local_1) {
    cr_assert_eq(path_exists("./Makefile"), true);
}

Test(path_exists, local_2) {
    cr_assert_eq(path_exists("./tests"), true);
}

Test(path_is_file, basic_1) {
    cr_assert_eq(path_is_file("/"), false);
}

Test(path_is_file, basic_2) {
    cr_assert_eq(path_is_file("/tmp"), false);
}

Test(path_is_file, basic_3) {
    cr_assert_eq(path_is_file("./Makefile"), true);
}

Test(path_is_file, basic_4) {
    cr_assert_eq(path_is_file("./tests"), false);
}
