# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Files
SRCS = main.c utils.c action.c temp.c monitor.c routine.c
OBJS = $(addprefix obj/, $(SRCS:.c=.o))
DEPS = header.h

# Output
NAME = philo

# Default rule
all: $(NAME)

# Link object files to create executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files into .o files inside obj/
obj/%.o: %.c $(DEPS)
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -rf obj

# Clean everything
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
