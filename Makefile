CC := gcc

CFLAGS  := -Wall -Iinclude -pg
LDFLAGS :=

PROGS := worker

SRC1 := worker.c state_functions.c

.PHONY: all debug
all: $(PROGS);

debug: CFLAGS += -DDEBUG -g
debug: all

worker: $(SRC1)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: clean veryclean
clean:
	-rm *.o

veryclean:
	-rm $(PROGS)

