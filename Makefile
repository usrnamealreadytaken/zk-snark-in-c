CC  = clang
CFLAGS  = -static-libsan -fno-omit-frame-pointer -std=gnu11 -Wall -Wextra -Wpedantic -Werror -O0 -g -ftrapv -Wno-comment
LFLAGS  =  -static-libsan -fsanitize=address -fno-omit-frame-pointer -lm -lrt -lpbc -lgmp -L/usr/local/lib/
# CFLAGS  = -std=gnu11 -Wall -Wextra -Wpedantic -Werror -O0 -g -ftrapv -Wno-comment 
# LFLAGS  = -fno-omit-frame-pointer -lm -lrt -lpbc -lgmp -lgsl -L. -L/usr/local/lib/

MAIN := main
OBJ := obj/
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, $(OBJ)%.o, $(SOURCES))

all: dir install run

reinstall: clean all

vall: dir install
	# @valgrind -s --num-callers=30 --leak-check=full ./$(MAIN)
	@valgrind -s --num-callers=30 --leak-check=no --vgdb-error=1 ./$(MAIN)
#	@valgrind -s --num-callers=30 --leak-check=full ./$(MAIN) 2>&1 | head -n -10

run:
	@./$(MAIN)

install: $(OBJECTS)
	@$(CC) $^ -o $(MAIN) $(LFLAGS)

dir:
	@mkdir -p $(OBJ)

%.o: ../%.c
	@$(CC) $(CFLAGS) -c $< -o $@ 
	@echo CC $(subst obj/../, "", $<)

clean:  
	@\rm -f $(OBJECTS)
	@echo rm $(subst obj/, "", $(OBJECTS)) $(MAIN)
	@\rm -f $(OBJ)*.d
	@\rm -f $(MAIN)
