CC = gcc
CFLAGS = -Wall

# We should fix this makefile

# testsParser: parser.c ./tests/testParser.c ./tests/testSuite.c ./tests/testRC4.c
# 	$(CC) -o parser parser.c ./tests/testParser.c ./tests/testSuite.c ./tests/testRC4.c $(CFLAGS)

# testsLSB: ./stego/lsbEncrypt.c ./tests/testLsb.c ./stego/lsbDecrypt.c
# 	$(CC) -o testLSB ./stego/lsbEncrypt.c ./stego/lsbDecrypt.c $(CFLAGS)

main: 
	$(CC) -o main main.c ./stego/lsbEncrypt.c ./stego/lsbDecrypt.c parser.c bmpParser.c extract.c embed.c $(CFLAGS)

clean: 
	rm f *.o ./tests/*.o
