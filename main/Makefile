CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o steg_encode.o steg_decode.o
TARGET = stegtool

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c steg_encode.h steg_decode.h
	$(CC) $(CFLAGS) -c main.c

steg_encode.o: steg_encode.c steg_encode.h
	$(CC) $(CFLAGS) -c steg_encode.c

steg_decode.o: steg_decode.c steg_decode.h
	$(CC) $(CFLAGS) -c steg_decode.c

clean:
	rm -f $(OBJ) $(TARGET)