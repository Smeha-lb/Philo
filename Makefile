NAME		= philo
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
RM			= rm -f

SRCDIR		= .
OBJDIR		= obj

SRCFILES	= main.c \
			  init.c \
			  philosopher.c \
			  monitor.c \
			  utils.c

SRCS		= $(addprefix $(SRCDIR)/, $(SRCFILES))
OBJS		= $(addprefix $(OBJDIR)/, $(SRCFILES:.c=.o))

HEADERS		= philo.h

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(NAME) created successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@echo "Removing object files and directory..."
	@$(RM) -r $(OBJDIR)

fclean: clean
	@echo "Removing $(NAME)..."
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re 