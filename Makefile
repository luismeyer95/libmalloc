
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CC			=	clang-6.0
endif
ifeq ($(UNAME), Darwin)
	CC			=	gcc
endif

FWARN		=	-Wall -Wextra -Werror
FDEBUG		=	-g3
# FOPTI		=	-O3
FLAGS		=	$(FWARN) $(FDEBUG) $(FOPTI)

ALIGNMENT	=	16

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		=	libft_malloc_$(HOSTTYPE).so
LIBFT		=	libft

SRCS_DIR	=	srcs
OBJS_DIR  	=	objs
INC_DIR		=	includes libft

SRCS	=	malloc.c \
			realloc.c \
			free.c \
			show_alloc_mem.c \
			show_alloc_mem_ex.c \
			globals.c \
			helpers.c \
			debug_flags.c \
			arena.c \
			mprintf.c

INCLUDE		=	$(addprefix includes/, \
				libft_malloc.h \
				libft_malloc_internals.h)

LIBFT_DIR = libft
LIBFT_INCLUDE = $(LIBFT_DIR)/libft.h
LIBFT_FILES = $(shell find libft/*.h libft/*.c -type f)
LIBFT = $(LIBFT_DIR)/libft.a

LDFLAGS = -lpthread

OBJS		= $(SRCS:.c=.o)

all:	$(LIBFT) $(NAME)

$(LIBFT): $(LIBFT_FILES) 
	$(MAKE) -C libft

$(NAME): $(addprefix $(OBJS_DIR)/, $(OBJS)) $(LIBFT)
	$(CC) $(LDFLAGS) $(FLAGS) $(addprefix $(OBJS_DIR)/, $(OBJS)) -Wl,--whole-archive $(LIBFT) -Wl,--no-whole-archive -shared -o $(NAME)
	ln -sf $(NAME) libft_malloc.so

main: $(NAME)
	$(CC) $(FLAGS) -rdynamic -D ALIGNMENT=$(ALIGNMENT) $(addprefix -I, $(INC_DIR)) -lpthread main.c $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDE)
	$(CC) $(FLAGS) -D ALIGNMENT=$(ALIGNMENT) -c -fPIC $(addprefix -I, $(INC_DIR)) $< -o $@

clean:
	rm -f $(addprefix $(OBJS_DIR)/, $(OBJS))

fclean: clean
	rm -f $(NAME) libft_malloc.so a.out
	make fclean -C libft

re: fclean all

.PHONY: clean fclean re main
