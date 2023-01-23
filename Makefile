# COLORS
RESET		=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
RED			=	\033[31m

D_PROMPT	=	prompt/get_user_and_path.c
D_PARSE		=	parse/command.c	parse/dollar.c parse/line.c parse/pipe.c parse/execute.c parse/quotes.c parse/signals.c
D_ERROR		=	utils/error_func.c
D_BUILTIN	=	builtin/builtin.c   builtin/export.c   builtin/unset_utils.c   builtin/export_utils_one.c   builtin/export_utils_two.c   builtin/unset.c builtin/pathed.c
D_UTILS		=	utils/free.c utils/utils_two.c utils/utils.c utils/is.c utils/skip.c
D_REDIRECT	=	redirect/redirect.c redirect/heredoc.c redirect/expand_redirect.c
D_MINISHELL	=	main.c


# COMMAND
SUCCESS		=	@echo "$(GREEN)Build successfull !$(RESET)"
REMOVE		=	@echo "$(RED)Deleted !$(RESET)"
PLUSULTRA	=	@echo "$(YELLOW)!PLUS ULTRA ^^ !$(RESET)"


NAME = minishell
OBJ			=	$(D_PROMPT:%.c=%.o) \
				$(D_PARSE:%.c=%.o) \
				$(D_ERROR:%.c=%.o) \
				$(D_UTILS:%.c=%.o) \
				$(D_BUILTIN:%.c=%.o) \
				$(D_REDIRECT:%.c=%.o) \
				$(D_MINISHELL:%.c=%.o)


CC = @cc
CFLAGS = -I lib -Wall -Wextra -Werror -fsanitize=address -g
RM = rm -rf
#LIB		= ./lib/.rdl
READLINE	= -L lib/readline	-lreadline -ltermcap


all: $(LIB) $(NAME)


#$(LIB):
#	make -C ./lib


$(NAME): $(OBJ)
	@echo "\n"
	make -C ./libft
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) ./libft/libft.a $(READLINE)
	$(SUCCESS)
	$(PLUSULTRA)



# Renkleri kaldirdim cok uzur dilerim ama goremiyorum yoksa :(
%.o: %.c
			@#printf "\033[0;30mGenerating minishell objects... %-33.33s\r" $@
			${CC} ${CFLAGS} -c $< -o $@

clean:
	make -C ./libft clean
	$(RM) */*.o
	$(RM) *.o

norm:
	norminette builtin/ parse/ prompt/ redirect/ utils/ main.c 

fclean: clean
	rm -rf libft.a
	$(RM) $(NAME)
	$(REMOVE)

ffclean: fclean
	@make fclean -C lib/

re: fclean all

.PHONY:		all clean fclean re norm
