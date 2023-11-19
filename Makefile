C_FLAGS := -Werror -Wall -Wpedantic -std=c11 -ggdb
LIBS=-lm

dft: src/dft.c
	$(CC) $(C_FLAGS) -o dft src/dft.c $(LIBS)

fft: src/fft.c
	$(CC) $(C_FLAGS) -o fft src/fft.c $(LIBS)
