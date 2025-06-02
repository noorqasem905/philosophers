CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
NAME = philosophers

SRCS =	srcs/philosophers.c 	\
		srcs/process_input.c	\
		srcs/handling_philo.c

all: $(NAME)



LIBS = -lpthread

OBJS_DIR =includes/build
OBJS = $(SRCS:srcs/%.c=$(OBJS_DIR)/%.o)

LIBFT = libft
#HEADERS = includes/
HIDE = $(if $(set),, @)
HIDED = $(if $(DIR),, --no-print-directory)

$(NAME): $(OBJS) $(LIBFT)
	$(HIDE)make $(HIDED) -C libft/printf
	$(HIDE)make $(HIDED) -C libft
	$(HIDE)make $(HIDED) -C libft bonus
	$(HIDE)$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -Llibft/printf -lftprintf -Llibft -lft -g

$(OBJS_DIR)/%.o: srcs/%.c 
	$(HIDE)mkdir -p $(OBJS_DIR)
	$(HIDE)$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(HIDE)make $(HIDED) -C libft/printf clean
	$(HIDE)make $(HIDED) -C libft clean
	$(HIDE)rm -f $(OBJS)
	$(HIDE)rm -f $(HEADERS)
	$(HIDE)rm -f $(HEADERS)

fclean:
	$(HIDE)make $(HIDED) -C libft/printf fclean
	$(HIDE)make $(HIDED) -C libft fclean
	$(HIDE)rm -f $(OBJS)
	$(HIDE)rm -f $(NAME)
	$(HIDE)rm -df $(OBJS_DIR)

re: fclean all

.PHONY: all clean fclean re
