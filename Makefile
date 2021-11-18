NAME		=	minishell

SRCFD		=	srcs
PARSER_PATH	=	folder_check_comm
BUILD_PATH	= 	folder_buildin

SRC			=	minishell.c \
				make_arguments.c \
				work_command.c \
				work_command_utils.c \
				mallocs.c \
				binary_work.c \
				free.c \
				open_redirect.c \
				ft_signal.c	\
				open_redirect_utils.c \
				work_comm_utils_2.c \
				minishell_utils.c

PARSER_FILE	=	check_command.c \
				dollar_main.c \
				dollar_utils.c \
				make_list.c

BUILD_FILE	=	ft_echo.c \
				ft_env.c \
				ft_cd.c \
				ft_exp_add_var.c \
				ft_exp_del_var.c \
				ft_export.c \
				ft_unset.c \
				ft_exit.c

SRCS 	= $(addprefix $(SRCFD)/, $(SRC)) $(addprefix $(PARSER_PATH)/, $(PARSER_FILE)) $(addprefix $(BUILD_PATH)/, $(BUILD_FILE))

OBJS		=	$(SRCS:.c=.o)
LIBFT		=	libft.a

CC			=	gcc
CFLAGS		=	-g -Wall -Wextra -Werror # -fsanitize=address
INCLUDES	=	-I libft/ -I includes/
FTFLAGS		=	-lft
FLAG_signal = -lreadline -L/Users/gvenonat/.brew/Cellar/readline/8.1/lib/ -I/Users/gvenonat/.brew/Cellar/readline/8.1/include

RM			=	rm -f

all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT) includes/minishell.h
			$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) -I includes/ -L . $(FTFLAGS) $(FLAG_signal)

$(LIBFT): libft/
			@$(MAKE) -C libft/
			@mv libft/$(LIBFT) .

clean:
			$(RM) $(OBJS)
			$(MAKE) -C libft/ fclean
			$(RM) $(LIBFT)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re

