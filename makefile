CC = gcc
CFLAGS = -Wall -lm

# We should fix this makefile

# testsParser: parser.c ./tests/testParser.c ./tests/testSuite.c ./tests/testRC4.c
# 	$(CC) -o parser parser.c ./tests/testParser.c ./tests/testSuite.c ./tests/testRC4.c $(CFLAGS)

testsLSB: ./tests/testLsb.c  ./stego/lsbEmbed.c ./tests/testLsb.c ./stego/lsbExtract.c
	$(CC) -o testLSB ./tests/testLsb.c ./stego/lsbEmbed.c ./stego/lsbExtract.c bmpParser.c  embed.c ./stego/lsbHelper.c $(CFLAGS)

main: 
	$(CC) -o main main.c ./stego/lsbEmbed.c ./stego/lsbExtract.c parser.c bmpParser.c extract.c embed.c ./stego/lsbHelper.c $(CFLAGS)

# bmpParserTest: 
# 	$(CC) -o main ./tests/bmpParser.c ./stego/lsbEmbed.c ./stego/lsbExtract.c parser.c bmpParser.c extract.c embed.c $(CFLAGS)


clean: 
	rm f *.o ./tests/*.o
