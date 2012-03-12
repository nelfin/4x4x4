CC := gcc

CFLAGS  := -Wall -Iinclude
LDFLAGS :=

PROGS := worker state_functions

SRC1 := worker.c
SRC2 := state_functions.c

.PHONY: all debug
all: $(PROGS);

debug: CFLAGS += -DDEBUG -g
debug: all

worker: $(SRC1)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

state_functions: $(SRC2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean veryclean
clean:
	-rm *.o

veryclean:
	-rm $(PROGS)

