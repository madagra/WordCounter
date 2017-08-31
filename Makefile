CC ?= gcc
CFLAGS = -O3 -Wall -pedantic -std=c99 
INC = -I./ 
EXE = wordcount

SRC = common.c parser.c hash_table.c wordcount.c
OBJ = $(SRC:.c=.o) 

default: $(EXE)

debug: CFLAGS += -DDEBUG=1 -g
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $(EXE) $(OBJ)

check: $(EXE)
	@cd test; echo "Testing word counter program"; \
	../$(EXE) *.txt > CHECK.output 

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $<  -o $@

.PHONY: clean

clean:
	rm -rf *.o $(EXE) test/CHECK.output
