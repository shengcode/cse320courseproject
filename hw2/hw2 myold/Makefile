CC = gcc
CFLAGS = 
BIN = utfconverter

SRC = $(wildcard *.c)

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

debug: $(SRC)
	$(CC) $(CFLAGS) $^ -g -DCSE320 -o $(BIN) 


clean:
	rm -f *.o $(BIN)

