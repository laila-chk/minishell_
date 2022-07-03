exec	=	minishell

sources =	$(wildcard src/*.c) $(wildcard src/libft_tools/*.c)

objects =	$(sources:.c=.o)

flags	=	-Wall -Wextra -Werror -lreadline -L /Users/waboutzo/.brew/opt/readline/lib -I /Users/waboutzo/.brew/opt/readline/include -g -fsanitize=address

CC		=	gcc

RM		= 	rm -f


all :	$(exec)
	make clean

$(exec)	: $(objects)
	@stty -echoctl
	$(CC) $(objects) $(flags) -o $(exec)

%.o	: %.c include/minishell.h
	$(CC) $(flags) -c %< -o $@

clean	:
	$(RM) src/*.o
	$(RM) src/libft_tools/*.o

fclean : clean
	$(RM) $(exec)

re : fclean all
