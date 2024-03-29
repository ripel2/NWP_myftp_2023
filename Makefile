##
## EPITECH PROJECT, 2023
## myftp
## File description:
## Makefile for the myftp project
##

DEFAULT     =   "\033[00m"
RED         =   "\033[1;31m"
GREEN       =   "\033[1;32m"
BLUE		=   "\033[1;34m"
TEAL        =   "\033[1;36m"
EOL         =   "\n"

GCC			=	gcc
RM			=	-rm

NAME		=	myftp

MAIN		=	src/main.c

SRC			=	src/myftp.c	\
				src/arg_check.c	\
				src/logger/logger.c	\
				src/client/add_client.c	\
				src/client/remove_client.c	\
				src/client/client_printf.c	\
				src/client/welcome_client.c	\
				src/client/client_count.c	\
				src/client/passive_socket.c	\
				src/client/active_socket.c	\
				src/server/init_server.c	\
				src/server/destroy_server.c	\
				src/server/handle_client.c	\
				src/server/handle_clients.c	\
				src/server/server_loop.c	\
				src/commands/login.c	\
				src/commands/file_utils.c	\
				src/commands/parser.c	\
				src/commands/thread_send.c	\
				src/commands/noop.c	\
				src/commands/pwd.c	\
				src/commands/cwd.c	\
				src/commands/cdup.c	\
				src/commands/help.c	\
				src/commands/pasv.c	\
				src/commands/port.c	\
				src/commands/dele.c	\
				src/commands/retr.c	\
				src/commands/stor.c	\
				src/commands/list.c	\

TESTS		=	tests/tests_arg_check.c	\
				tests/tests_parser.c	\
				tests/tests_user_command.c	\
				tests/tests_pass_command.c	\
				tests/tests_quit_command.c	\
				tests/tests_pwd_command.c	\
				tests/tests_cwd_command.c	\
				tests/tests_cdup_command.c	\
				tests/tests_help_command.c	\
				tests/tests_noop_command.c	\
				tests/tests_client.c	\
				tests/tests_server.c	\
				tests/tests_file_utils.c	\
				tests/tests_transfer_sockets.c	\

OBJ			=	$(SRC:%.c=%.o) $(MAIN:%.c=%.o)

TESTS_OBJ	=	$(SRC:%.c=%.o) $(TESTS:%.c=%.o)

CFLAGS		=	-Wall -Wextra -Wshadow -Iinclude
TESTS_FLAGS	=	-lcriterion
COVERAGE	=	--coverage -fprofile-arcs -ftest-coverage

%.o: %.c
	@$(GCC) $(CFLAGS) -c $< -o $@ && \
	printf $(GREEN)"[+] Compiled $@ "$(DEFAULT)"\n" || \
	printf $(RED)"[-] Failed compiling $@ "$(DEFAULT)"\n"

all:		$(NAME)

$(NAME):	$(OBJ)
	@printf $(BLUE)"[+] Linking objects into "$(NAME)" "$(DEFAULT)"\n"
	@$(GCC) -o $@ $(OBJ) $(LDFLAGS) && \
	printf $(TEAL)"[+] Compiled "$@" "$(DEFAULT)"\n" || \
	printf $(RED)"[-] Failed compiling "$@" "$(DEFAULT)"\n"

clean:
	@$(RM) -f $(OBJ)
	@$(RM) -f *~ "#*#" -r

fclean:		clean
	@$(RM) -f $(NAME)

re:			fclean $(NAME)

delete_coverage_files:
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete

tests_run:	LDFLAGS += $(TESTS_FLAGS)
tests_run:	CFLAGS += $(COVERAGE)
tests_run:	delete_coverage_files clean $(TESTS_OBJ)
	@$(GCC) -o unit_tests $(TESTS_OBJ) $(CFLAGS) $(LDFLAGS) -Iinclude
	./unit_tests --always-succeed
	$(RM) unit_tests
	$(MAKE) clean

coverage:	tests_run
	gcovr . --exclude tests/
	gcovr .	-b --exclude tests/
	$(MAKE) delete_coverage_files

debug:		CFLAGS += -g3
debug:		re

.PHONY:	all clean fclean re delete_coverage_files tests_run debug
