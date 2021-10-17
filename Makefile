FILES = $(wildcard ./*.c)
OBJS = $(patsubst ./%.c, %.o, $(FILES))

OUT	= out
CC = gcc
DEBUG = -g0
FLAGS = $(DEBUG) -c -I. -std=c17\
				-Wall -Wextra -Wpedantic -Wuninitialized\
				-Wundef -Wcast-align -Wstrict-overflow=2 -Wwrite-strings\
				-Wno-format-nonliteral
LFLAGS = 


build: $(OBJS)
	$(CC) $(DEBUG) $(OBJS) -o $(OUT) $(LFLAGS)


$(OBJS): %.o: %.c
	$(CC) $(FLAGS) $<


rebuild: clean build


clean:
	rm -f $(OBJS) $(OUT)


run: $(OUT)
	./$(OUT)
