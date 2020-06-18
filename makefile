CC = gcc
CFLAGS = -Wall

test: parser.c ./tests/testSuite.c ./tests/testParser.c ./tests/testRC4.c
	$(CC) -o test parser.c ./tests/testSuite.c ./tests/testParser.c ./tests/testRC4.c $(CFLAGS)

clean:
	rm -f *.o ./tests/*.o