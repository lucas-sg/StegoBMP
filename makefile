CC = gcc
CFLAGS = -Wall

test: ./stego/lsbEncrypt.c ./tests/testSuite.c ./tests/testLsb.c
	$(CC) -o test ./tests/testLsb.c ./stego/lsbEncrypt.c ./tests/testSuite.c  $(CFLAGS)

clean: 
	rm f *.o ./tests/*.o