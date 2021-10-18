FILES = $(wildcard ./*.c)
OBJS = $(patsubst ./%.c, %.o, $(FILES))

OUT	= out
CC = gcc
DEBUG = -g0
FLAGS = -c -I. -std=c17\
				-Wall -Wextra -Wpedantic -Wuninitialized\
				-Wundef -Wcast-align -Wstrict-overflow=2 -Wwrite-strings\
				-Wno-format-nonliteral
LFLAGS = 

valgrind valgrind_extreme valgrind_leakcheck : DEBUG = -g3

build: $(OBJS)
	$(CC) $(DEBUG) $(OBJS) -o $(OUT) $(LFLAGS)


$(OBJS): %.o: %.c
	$(CC) $(DEBUG) $(FLAGS) $<


rebuild: clean build


clean:
	rm -f $(OBJS) $(OUT)


run: $(OUT)
	./$(OUT)


valgrind: rebuild
	valgrind ./$(OUT)


valgrind_leakcheck: rebuild
	valgrind --leak-check=full ./$(OUT)


valgrind_extreme: rebuild
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes ./$(OUT)