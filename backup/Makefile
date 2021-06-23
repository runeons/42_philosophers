NAME 		= 	philosophers

CC 			= 	clang
CFLAGS 		= 	-Wall -Wextra -Werror -g3
SRCS		=	main.c \
				diner_routine.c \
				forks.c \
				init.c \
				print.c \
				time.c \
				utils.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
