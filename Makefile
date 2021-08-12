SHELL				=	/bin/sh

# Executable name
NAME				=	woody_woodpacker

SRC_PAYLOADS_PATH	=	./payloads/

PAYLOAD_64_NAME		=	payload_64
PAYLOAD_32_NAME		=	payload_32

# Compilation mode
WALL				=	yes
WEXTRA				=	yes
WERROR				=	no
FSANITIZE			=	no
DEBUG				=	no
O2					=	no

CC					:=	gcc
AS					:= nasm
AS_FLAG				:= -f elf64
GEN					:=	Generation in mode

ifeq ($(WALL), yes)
	CC				:=	$(CC) -Wall
	GEN				:=	$(GEN) all
endif

ifeq ($(WEXTRA), yes)
	CC				:=	$(CC) -Wextra
	GEN				:=	$(GEN) extra
endif

ifeq ($(WERROR), yes)
	CC				:=	$(CC) -Werror
	GEN				:=	$(GEN) error
endif

ifeq ($(FSANITIZE), yes)
	CC				:=	$(CC) -fsanitize=address
	GEN				:=	$(GEN) sanitize
endif

ifeq ($(DEBUG), yes)
	CC				:=	$(CC) -g
	GEN				:=	$(GEN) debug
endif

ifeq ($(O2),yes)
	CC				:=	$(CC) -O2
	GEN				:=	$(GEN) O2
endif

ifeq ($(GEN), "Generation in mode")
	GEN				:=	$(GEN) no flags
endif

# Name
SRC_NAME			=	main.c						\
						error.c						\
						infect_elf_32.c				\
						infect_elf_64.c				\
						utils.c						\
						utils_elf.c					\
						utils_payload.c				\
						silvio_text_infection_32.c	\
						silvio_text_infection_64.c	\
						crypto.c					\

ASM_SRC_NAME		=	xor_cipher.asm		\

SRC_PAYLOAD_64_NAME	=	test64.asm				\

SRC_PAYLOAD_32_NAME	=	test32.asm					\

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

all: art $(NAME) $(SRC_PAYLOADS_PATH)$(PAYLOAD_64_NAME) $(SRC_PAYLOADS_PATH)$(PAYLOAD_32_NAME)

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
	$(AS) $(AS_FLAG) $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

payloads: clean_payloads $(SRC_PAYLOADS_PATH)$(PAYLOAD_64_NAME) $(SRC_PAYLOADS_PATH)$(PAYLOAD_32_NAME)

clean_payloads:
	@rm -f $(SRC_PAYLOADS_PATH)$(PAYLOAD_32_NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(PAYLOAD_32_NAME)
	@rm -f $(SRC_PAYLOADS_PATH)$(PAYLOAD_64_NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(PAYLOAD_64_NAME)

$(SRC_PAYLOADS_PATH)$(PAYLOAD_64_NAME):
	@echo "\n$(_WHITE)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING $(PAYLOAD_64_NAME)$(_END)"
	@echo "$(_WHITE)====================================================$(_END)"
	@nasm -f bin -o $(SRC_PAYLOADS_PATH)$(PAYLOAD_64_NAME) $(SRC_PAYLOADS_PATH)$(SRC_PAYLOAD_64_NAME)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"

$(SRC_PAYLOADS_PATH)$(PAYLOAD_32_NAME):
	@echo "\n$(_WHITE)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING $(PAYLOAD_32_NAME)$(_END)"
	@echo "$(_WHITE)====================================================$(_END)"
	@nasm -f bin  -o $(SRC_PAYLOADS_PATH)$(PAYLOAD_32_NAME) $(SRC_PAYLOADS_PATH)$(SRC_PAYLOAD_32_NAME)
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

fclean: clean clean_payloads
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
	@echo "$(_YELLOW)   make payloads                       generates payloads binaries        "
	@echo "$(_YELLOW)   make clean_payloads                 clean payloads binaries            "
	@echo "$(_YELLOW)   make clean                          remove the generated files         "
	@echo "$(_YELLOW)   make tests                          launch tests scripts               "
	@echo "$(_YELLOW)   make help                           prints this message                $(_END)"

.PHONY: all art clean fclean re check payloads help tests clean_payloads

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