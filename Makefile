CC := gcc

CFLAGS  :=
LDFLAGS :=

PROGS := worker

SRC1 := worker.c

all: $(PROGS);

worker: $(SRC1)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean veryclean
clean:
	-rm *.o

veryclean:
	-rm $(PROGS)

