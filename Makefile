
CC = gcc

CFLAGS =  -Wextra -Werror -Wall

NAME = pipex

RM = rm -rf

SRCS = 	pipex.c\
		pipex_helper.c\
		libft/libft.a\

$(NAME) :
	make all -C libft
	gcc $(CFLAGS) $(SRCS) -o $(NAME)

all : $(NAME)

fclean : clean
	$(RM) $(NAME)
	make fclean -C libft

clean :
	$(RM) $(NAME)
	make clean -C libft

re : fclean all
