SHELL				=	/bin/sh

# Executable name
NAME				=	woody_woodpacker

SRC_PAYLOADS_PATH	=	./payloads/


# Compilation mode
WALL				=	yes
WEXTRA				=	yes
WSHADOW				=	yes
WERROR				=	no
FSANITIZE			=	no
DEBUG				=	no
O2					=	no

CC					:=	gcc
AS					:= nasm
AS_FLAG				:= -f elf64
GEN					:=	Generation in mode

LONG_BITS := $(shell getconf LONG_BIT)
ifeq ($(LONG_BITS),32)
# Define for 32bits
CC				:= $(CC) -D ARCH_32
PAYLOAD_NAME		=	payload_32
SRC_PAYLOAD_NAME	=	test32.asm
else
# Define for 64bits
PAYLOAD_NAME		=	payload_64
SRC_PAYLOAD_NAME	=	test64.asm
endif

ifeq ($(WALL), yes)
	CC				+=	-Wall
	GEN				+=	 all
endif

ifeq ($(WEXTRA), yes)
	CC				+=	-Wextra
	GEN				+=	extra
endif

ifeq ($(WSHADOW), yes)
	CC				+=	-Wshadow
	GEN				+=	shadow
endif

ifeq ($(WERROR), yes)
	CC				+=	-Werror
	GEN				+=	error
endif

ifeq ($(FSANITIZE), yes)
	CC				+=	-fsanitize=address
	GEN				+=	sanitize
endif

ifeq ($(DEBUG), yes)
	CC				+=	-g
	GEN				+=	debug
endif

ifeq ($(O2),yes)
	CC				+=	-O2
	GEN				+=	O2
endif

ifeq ($(GEN), "Generation in mode")
	GEN				+=	no flags
endif

# Name
SRC_NAME			=	main.c						\
						error.c						\
						utils.c						\
						utils_elf.c					\
						utils_payload.c				\
						silvio_text_infection.c		\
						crypto.c					\

ASM_SRC_NAME		=	xor_cipher.asm		\

INCLUDE_NAME		=	woody_woodpacker.h	\

TESTS_FILES	= ./tests/test*.sh

# Path
ASM_SRC_PATH	=	./asm/

SRC_PATH			=	./src/

ASM_OBJ_PATH	= 	./obj_asm/

OBJ_PATH 			=	./obj/

INCLUDE_PATH		=	./include/

# Name + Path
ASM_SRC			= 	$(addprefix $(ASM_SRC_PATH), $(ASM_SRC_NAME))

SRC					=	$(addprefix $(SRC_PATH), $(SRC_NAME))

ASM_OBJ			=	$(patsubst $(ASM_SRC_PATH)%.asm, $(ASM_OBJ_PATH)%.o,	$(ASM_SRC))

OBJ					=	$(patsubst $(SRC_PATH)%.c, $(OBJ_PATH)%.o,	$(SRC))

INCLUDE				=	$(addprefix $(INCLUDE_PATH), $(INCLUDE_NAME))

# Text format
_END		=	\033[0m
_GRAS		=	\033[1m
_SOUL		=	\033[4m
_CLIG		=	\033[5m
_SURL		=	\033[7m

# Colors
_BLACK		=	\033[30m
_RED		=	\033[31m
_GREEN		=	\033[32m
_YELLOW		=	\033[33m
_BLUE		=	\033[34m
_PURPLE		=	\033[35m
_CYAN		=	\033[36m
_GREY		=	\033[37m

# Background
_IBLACK		=	\033[40m
_IRED		=	\033[41m
_IGREEN		=	\033[42m
_IYELLOW	=	\033[43m
_IBLUE		=	\033[44m
_IPURPLE	=	\033[45m
_ICYAN		=	\033[46m
_IGREY		=	\033[47m

all: art $(NAME) $(SRC_PAYLOADS_PATH)$(PAYLOAD_NAME)

$(NAME): $(ASM_OBJ) $(OBJ)
	@echo "\n$(NAME) : $(GEN)"
	@echo "\n$(_WHITE)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING $(NAME)$(_END)"
	@echo "$(_WHITE)====================================================$(_END)"
	@$(CC) -o $(NAME) $(OBJ) $(ASM_OBJ)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INCLUDE)
	@mkdir -p $(OBJ_PATH)
	@$(CC) -I $(INCLUDE_PATH) -I $(INCLUDE_PATH) -c $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

$(ASM_OBJ_PATH)%.o: $(ASM_SRC_PATH)%.asm
	@mkdir -p $(ASM_OBJ_PATH)
	@$(AS) $(AS_FLAG) $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

payload: clean_payload $(SRC_PAYLOADS_PATH)$(PAYLOAD_NAME)

clean_payload:
	@rm -f $(SRC_PAYLOADS_PATH)$(PAYLOAD_NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(PAYLOAD_NAME)"$(_END)"

$(SRC_PAYLOADS_PATH)$(PAYLOAD_NAME):
	@echo "\n$(_WHITE)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING $(PAYLOAD_NAME)$(_END)"
	@echo "$(_WHITE)====================================================$(_END)"
	@nasm -f bin -o $(SRC_PAYLOADS_PATH)$(PAYLOAD_NAME) $(SRC_PAYLOADS_PATH)$(SRC_PAYLOAD_NAME)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"

tests: all
	@echo "\n$(_WHITE)====================================================$(_END)"
	@echo "$(_YELLOW)		LAUNCHING TESTS$(_END)"
	@echo "$(_WHITE)====================================================$(_END)"
	@for f in $(TESTS_FILES);  do sh $${f}; done;

clean:
	@rm -rf $(OBJ_PATH) 2> /dev/null || true
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(OBJ_PATH)"$(_END)"
	@rm -rf $(ASM_OBJ_PATH) 2> /dev/null || true
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(ASM_OBJ_PATH)"$(_END)"

fclean: clean clean_payload
	@rm -f $(NAME) woody
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(NAME)
	@echo "$(_END)"

re: fclean all


help:
	@echo "$(_YELLOW)Makefile for generating binary infectors."
	@echo "$(_YELLOW)Usage:                                                                    "
	@echo "$(_YELLOW)   make                                runs all                           "
	@echo "$(_YELLOW)   make all                            generates all binaries             "
	@echo "$(_YELLOW)   make art                            print a bird                       "
	@echo "$(_YELLOW)   make payload                        generates payload binary           "
	@echo "$(_YELLOW)   make clean_payload                  clean payload binary               "
	@echo "$(_YELLOW)   make clean                          remove the generated files         "
	@echo "$(_YELLOW)   make tests                          launch tests scripts               "
	@echo "$(_YELLOW)   make help                           prints this message                $(_END)"

.PHONY: all art clean fclean re check payload help tests clean_payload

art:
	@echo "$(_CYAN)"
	@echo "       '-------------------.. "
	@echo "         '\\_________         \`-.-----. "
	@echo "             '\\_______          \`\\    \`.                   $(_GREEN)  __   ____ $(_CYAN)"
	@echo "                  \\____           \`\\   :                    $(_GREEN)/_ | | .-.\\ $(_CYAN)"
	@echo "    '._____         (____           \`\\.\`.             .--._ $(_GREEN)  \\\\' / $(_CYAN)"
	@echo "     \\____  \"'..____.(_______          \`~-.________.-'  $(_RED)@$(_CYAN). \\___${_GREEN}\\.'${_CYAN}___ "
	@echo "     )___     ___                                         /___________\\ "
	@echo "      )___..''   '--.                    _.----------.____\`----$(_GREEN))'($(_CYAN)---' "
	@echo "                   _:-'   ,          _.-'                $(_GREEN)     /( \\\\ $(_CYAN)"
	@echo "               .-~~ __..-'~~~~~~~~~~'                    $(_GREEN)   .'    '\\ $(_CYAN)"
	@echo "               \\ \\~~ \\ \\                             $(_GREEN)      / '    ) \\ $(_CYAN)"
	@echo "                \\ \\   \\ \\_.-'                        $(_GREEN)     |      )   ' $(_CYAN)"
	@echo "                 \\ \\_ _;-,,'                           $(_GREEN)   ( (      ) ) $(_CYAN)"
	@echo "                 ;-,,'                                   $(_GREEN)  \"--------\" $(_CYAN)"
	@echo ""
	@echo "$(_END)"