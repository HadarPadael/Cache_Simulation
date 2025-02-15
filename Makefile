CC = gcc
CFLAGS = -O3 -g

SRC = code/main.c code/cache_funcs.c code/simulation.c
OBJ = $(SRC:.c=.o)
DEPS = headers/cache_funcs.h headers/cache_struct.h headers/simulation.h
EXECUTABLE = cache

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(OBJ)

.PHONY: all clean