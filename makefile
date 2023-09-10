BNS = parser
C = gcc
CFLAGS = -std=gnu99 -Wall -pedantic -g

all: $(BINS)

parser: parser.c
	$(C) $(CFLAGS) -o parser parser.c

clean:
	rm $(BINS)

