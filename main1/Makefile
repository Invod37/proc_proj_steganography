CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o bmp_utils.o steg_utils.o steg_encode.o steg_decode.o 
TARGET = stegtool
SRCDIR = src

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c $(SRCDIR)/steg_encode.h $(SRCDIR)/steg_decode.h
	$(CC) $(CFLAGS) -c main.c

bmp_utils.o: $(SRCDIR)/bmp_utils.c $(SRCDIR)/bmp_utils.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/bmp_utils.c

steg_utils.o: $(SRCDIR)/steg_utils.c $(SRCDIR)/steg_utils.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/steg_utils.c	

steg_encode.o: $(SRCDIR)/steg_encode.c $(SRCDIR)/steg_encode.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/steg_encode.c

steg_decode.o: $(SRCDIR)/steg_decode.c $(SRCDIR)/steg_decode.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/steg_decode.c

clean:
	rm -f $(OBJ) $(TARGET)