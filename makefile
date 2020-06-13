CC = gcc
CFLAGS = -Wall

test: parser.c ./tests/testSuite.c ./tests/testParser.c
	$(CC) -o test parser.c ./tests/testSuite.c ./tests/testParser.c $(CFLAGS)

clean:
	rm -f *.o ./tests/*.o