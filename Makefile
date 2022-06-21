NAME 		= 	philo

CPL 		= 	clang
FLAGS 		= 	-Wall -Wextra -Werror -lpthread
INCLUDES	=   -I ./includes

SRCS		=	./srcs/main.c \
				./srcs/diner_routine.c \
				./srcs/init.c \
				./srcs/print.c \
				./srcs/time.c \
				./srcs/utils.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS)
	$(CPL) $(FLAGS) $(INCLUDES) $(SRCS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

fsan: $(SRCS)
	$(CPL) $(FLAGS) $(INCLUDES) $(SRCS) -fsanitize=address -g3 -o $(NAME)

.PHONY: all clean fclean re
