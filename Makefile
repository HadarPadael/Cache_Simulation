CC = gcc
CFLAGS = -O3 -g

SRC = main.c cache_funcs.c simulation.c
OBJ = $(SRC:.c=.o)
DEPS = cache_funcs.h cache_struct.h simulation.h
EXECUTABLE = cache

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(OBJ)

.PHONY: all clean