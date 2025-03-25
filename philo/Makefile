MAKEFLAGS += --no-print-directory

CFLAGS += -Wall -Wextra -Werror
LDFLAGS = -lpthread

.PHONY: all clean fclean re

all: philo

philo: philo.o
	$(CC) $(CFLAGS) -o philo philo.o $(LDFLAGS)

philo.o: philo.c
	$(CC) $(CFLAGS) -o philo.o philo.c

clean:
	rm -f philo.o

fclean: clean
	rm -f philo

re:
	$(MAKE) fclean
	$(MAKE) all
