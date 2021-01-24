.PHONY: all clean fclean re
NAME = philo_two
FLAGS = -Wall -Werror -Wextra
SOURCES_DIR = ./
SOURCES_LIST =	main.c\
				utils.c\
				dinner_helper.c

SOURCES = $(addprefix $(SOURCES_DIR), $(SOURCES_LIST))
OBJECTS_DIR = obj/
OBJECTS_LIST = $(patsubst %.c, %.o, $(SOURCES_LIST))
OBJECTS	= $(addprefix $(OBJECTS_DIR), $(OBJECTS_LIST))
HEADER = $(addsuffix .h, $(NAME))

all: $(NAME)
$(NAME): $(OBJECTS_DIR) $(OBJECTS) $(HEADER)
	@gcc $(FLAGS)  -o $(NAME) $(OBJECTS)
	@echo "$(NAME) compiled"

$(OBJECTS_DIR):
	@mkdir -p $(OBJECTS_DIR)
	@echo "$(NAME)'s $(OBJECTS_DIR) created"

$(OBJECTS_DIR)%.o : $(SOURCES_DIR)%.c $(HEADER)
	@gcc $(FLAGS) -c $< -o $@
	@echo "$< compiled to $@"

clean:
	@rm -rf $(OBJECTS_DIR)
	@echo "$(NAME)'s $(OBJECTS_DIR) deleted"

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) deleted"

re:
	@$(MAKE) fclean
	@$(MAKE) all