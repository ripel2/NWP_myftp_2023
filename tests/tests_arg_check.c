#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "myftp.h"

Test(check_args, basic) {
    char *av[] = {"./myftp", "4242", "/", NULL};
    int ac = 3;

    cr_assert_eq(check_args(ac, av), 0);
}

Test(check_args, too_many_args) {
    char *av[] = {"./myftp", "4242", "/", "pugo", NULL};
    int ac = 4;

    cr_assert_eq(check_args(ac, av), 84);
}

Test(check_args, too_few_args) {
    char *av[] = {"./myftp", "4242", NULL};
    int ac = 2;

    cr_assert_eq(check_args(ac, av), 84);
}

Test(check_args, invalid_port) {
    char *av[] = {"./myftp", "lucas", "/", NULL};
    int ac = 3;

    cr_assert_eq(check_args(ac, av), 84);
}

Test(check_args, invalid_port_2) {
    char *av[] = {"./myftp", "42i", "/", NULL};
    int ac = 3;

    cr_assert_eq(check_args(ac, av), 84);
}

Test(check_args, invalid_port_3) {
    char *av[] = {"./myftp", "50)", "/", NULL};
    int ac = 3;

    cr_assert_eq(check_args(ac, av), 84);
}

Test(check_args, invalid_path) {
    char *av[] = {"./myftp", "4242", "tom", NULL};
    int ac = 3;

    cr_assert_eq(check_args(ac, av), 84);
}

Test(check_args, path_is_file) {
    char *av[] = {"./myftp", "4242", "Makefile", NULL};
    int ac = 3;

    cr_assert_eq(check_args(ac, av), 84);
}

Test(check_args, port_out_of_range) {
    char *av[] = {"./myftp", "65536", "/", NULL};
    int ac = 3;

    cr_assert_eq(check_args(ac, av), 84);
}

Test(check_args, port_out_of_range2) {
    char *av[] = {"./myftp", "0", "/", NULL};
    int ac = 3;

    cr_assert_eq(check_args(ac, av), 84);
}
