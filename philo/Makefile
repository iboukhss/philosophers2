NAME = philo

SRCS = philo.c philo_routine.c monitor_routine.c philo_utils.c sim_utils.c utils.c
OBJS = $(SRCS:.c=.o)

MAKEFLAGS += --no-print-directory
CFLAGS += -Wall -Wextra -Werror
LDLIBS = -lpthread

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all
