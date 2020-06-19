CC = gcc
CFLAGS = -Wall

# We should fix this makefile

# testsParser: parser.c ./tests/testParser.c ./tests/testSuite.c
# 	$(CC) -o parser parser.c ./tests/testParser.c ./tests/testSuite.c $(CFLAGS)

testsLSB: ./stego/lsbEncrypt.c ./tests/testLsb.c
	$(CC) -o testLSB ./tests/testLsb.c ./stego/lsbEncrypt.c $(CFLAGS)

clean: 
	rm f *.o ./tests/*.o
