CC := gcc

CFLAGS  := -Wall -Iinclude
LDFLAGS :=

PROGS := worker

SRC1 := worker.c state_functions.c

.PHONY: all debug
all: $(PROGS);

debug: CFLAGS += -DDEBUG -g
debug: all

worker: $(SRC1)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: pdf
pdf: report.pdf;

report.pdf: report.tex
	latexmk -pdf report.tex

.PHONY: clean veryclean
clean:
	-rm *.o *.aux *.aux.bak *.log

veryclean:
	-rm $(PROGS)

