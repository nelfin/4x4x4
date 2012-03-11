CC := gcc

CFLAGS  :=
LDFLAGS :=

PROGS := worker state_functions

SRC1 := worker.c
SRC2 := state_functions.c

all: $(PROGS);

worker: $(SRC1)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

state_functions: $(SRC2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean veryclean
clean:
	-rm *.o

veryclean:
	-rm $(PROGS)

