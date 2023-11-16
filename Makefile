C_FLAGS := -Werror -Wall -Wpedantic -std=c11 -ggdb
LIBS=-lm

fft: src/dft.c
	$(CC) $(C_FLAGS) -o dft src/dft.c $(LIBS)
