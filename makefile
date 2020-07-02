CC = gcc
CFLAGS = -Wall -L/usr/lib -lssl -lcrypto

stegobmp: 
	$(CC) -o stegobmp main.c ./stego/lsbEmbed.c ./stego/lsbExtract.c cryptoUtils.c parser.c ./stego/rc4.c extract.c ./stego/lsbHelper.c embed.c fileParser.c $(CFLAGS)

testSuite: 
	$(CC) -o testSuite tests/testSuite.c tests/testParser.c tests/testRC4.c tests/testLsb.c tests/testLsbHelper.c ./stego/lsbEmbed.c ./stego/lsbExtract.c cryptoUtils.c parser.c ./stego/rc4.c extract.c ./stego/lsbHelper.c embed.c fileParser.c $(CFLAGS)
	
clean: 
	rm f *.o ./tests/*.o
