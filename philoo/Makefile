CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -Iincludes -g#-fsanitize=thread -g
NAME = philo

SRCS =	srcs/philosophers.c 	\
		srcs/process_input.c	\
		srcs/handling_philo.c	\
		srcs/setup_philo.c		\
		srcs/utils_philo.c		\
		srcs/home_philo.c		\
		srcs/routine.c

all: $(NAME)

LIBS = -lpthread
Header = includes/philo.h
OBJS_DIR =includes/build
OBJS = $(SRCS:srcs/%.c=$(OBJS_DIR)/%.o)

HIDE = $(if $(set),, @)
HIDED = $(if $(DIR),, --no-print-directory)

$(NAME): $(OBJS) $(Header)
	$(HIDE)$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: srcs/%.c 
	$(HIDE)mkdir -p $(OBJS_DIR)
	$(HIDE)$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(HIDE)rm -f $(OBJS)

fclean: clean
	$(HIDE)rm -f $(NAME)
	$(HIDE)rm -df $(OBJS_DIR)

re: fclean all

.PHONY: all clean fclean re
