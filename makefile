CC = gcc
CFLAGS = -Wall

# We should fix this makefile

# testsParser: parser.c ./tests/testParser.c ./tests/testSuite.c ./tests/testRC4.c
# 	$(CC) -o parser parser.c ./tests/testParser.c ./tests/testSuite.c ./tests/testRC4.c $(CFLAGS)

testsLSB: ./stego/lsbEncrypt.c ./tests/testLsb.c ./stego/lsbDecrypt.c
	$(CC) -o testLSB ./tests/testLsb.c ./stego/lsbEncrypt.c ./stego/lsbDecrypt.c $(CFLAGS)

clean: 
	rm f *.o ./tests/*.o
