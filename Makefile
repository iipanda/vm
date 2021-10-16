OBJS = main.o hashmap.o 
SOURCE = main.c hashmap.c
HEADER = hashmap.h
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

main.o: main.c
	$(CC) $(FLAGS) main.c 

hashmap.o: hashmap.c
	$(CC) $(FLAGS) hashmap.c 


rebuild: clean build


clean:
	rm -f $(OBJS) $(OUT)


run: $(OUT)
	./$(OUT)
