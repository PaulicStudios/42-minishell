NAME	:=	minishell

CC		:=	cc
CFLAGS	?=  -Wextra -Wall -Werror
DEBUG ?=  0
CFLAGS += -DDEBUG=$(DEBUG)
LDFLAGS	:=	-lreadline -lft
LIBFT	:=	lib/libft

VPATH	:=	src \
			src/buildins \
			src/prompt \
			src/input_parsing \
			src/signals \
			src/utils \
			src/exec \
			src/env \
			inc

INC		:=	colors.h \
			minishell.h

SOURCES	:=	main.c \
			cd_buildin.c \
			error_buildins.c \
			exec_buildins.c \
			pwd_buildin.c \
			env_buildin.c \
			echo_buildin.c \
			exit_buildin.c \
			unset_buildin.c \
			export_buildin.c \
			exec_pipe.c \
			exec_heredoc.c \
			prompt.c \
			tokenize.c \
			tokenize_utils.c \
			tokenize_utils_1.c \
			tokenize_utils_2.c \
			input_validator.c \
			input_validator_utils.c \
			input_validator_utils_1.c \
			expander.c \
			expander_util.c \
			expander_util_2.c \
			expander_util_3.c \
			token_utils.c \
			ctrl.c \
			fd_utils.c \
			exec_process.c \
			org_tokens.c \
			org_tokens_utils.c \
			exec_tokens.c \
			exec_tokens_utils.c \
			exec_process_utils.c \
			exec_utils.c \
			exec_files.c \
			exec_tokens_utils_parseon.c \
			parse_process.c \
			parse_process_utils.c \
			init_utils.c \
			env_add.c \
			env_check.c \
			env_manage.c \
			env_replace.c \
			env_utils.c \
			error_msg.c \
			file_utils.c \
			ast.c \
			ast_utils.c \
			ast_utils_1.c \
			precedence.c \
			rearrange_tokens.c \
			rearrange_tokens_utils.c \
			rearrange_tokens_utils_1.c \
			rearrange_tokens_utils_2.c \
			next_is.c \
			retokenize.c \
			retokenize_utils.c \
			retokenize_utils_1.c \
			token_utils_2.c \
			cleanup.c \
			expand_heredoc.c \
			exec_heredoc_utils.c \
			wildcards.c \
			wildcards_tokens.c \
			wildcards_files.c \
			build_prompt.c

OBJDIR	:=	obj
OBJECTS	:=	$(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

all: $(NAME)

$(NAME): $(OBJECTS) $(INC)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME) -L $(LIBFT)

clean:
	make -C $(LIBFT) clean
	rm -rf $(OBJDIR)

fclean: clean
	make -C $(LIBFT) fclean
	rm -f $(NAME)
	rm -f $(TESTNAME)

re: fclean all

$(OBJDIR)/%.o: %.c $(INC)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re
