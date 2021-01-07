CC = g++
CFLAGS = -Wall -g

run: main
	./main

main: main.o tgaimage.o
	$(CC) $(CFLAGS) -o main main.o tgaimage.o
	
main.o: main.cpp tgaimage.h
	$(CC) $(CFLAGS) -c main.cpp
	
tgaimage.o: tgaimage.cpp tgaimage.h

clean:
	rm -rf *.o
	rm -rf *.tga
