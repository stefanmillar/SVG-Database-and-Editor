CFLAGS = -Wall -g -std=c11
BIN = bin/
INC = include/
SRC = src/
XML_PATH = libxml2/include/
APP_PATH = ../

# Standard common makefile
parser: $(APP_PATH)libsvgparse.so remove

$(APP_PATH)libsvgparse.so: SVGParser.o LinkedListAPI.o
	gcc -shared -o $(APP_PATH)libsvgparse.so SVGParser.o LinkedListAPI.o -lxml2 -lm

SVGParser.o: $(SRC)SVGParser.c $(INC)LinkedListAPI.h $(INC)SVGParser.h
	gcc -c -fpic $(CFLAGS) -I$(XML_PATH) -I$(INC) $(SRC)SVGParser.c -o SVGParser.o

LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	gcc -c -fpic $(CFLAGS) -I$(INC) $(SRC)LinkedListAPI.c -o LinkedListAPI.o

test: parser  main.o
	$(CC) $(CFLAGS) -I$(XML_PATH) -lxml2 -o test main.o -lsvgparse -L./bin

main.o: $(SRC)SVGParser.c $(INC)SVGParser.h $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	$(CC) $(CFLAGS) -I$(XML_PATH) -Iinclude/ -lxml2 -lm -c -fpic src/main.c

clean:
	rm -r *.o $(APP_PATH)libsvgparse.so $(APP_PATH)*.o test

remove:
	rm -r *.o
