CC = gcc
CFLAGS = -W -WALL
TARGET = scrapy


$(TARGET) : scrapy.o parse.o
	$(CC) $(CFLAGS) -g -o $(TARGET) scrapy.o parse.o -lsocket -lnsl

scrapy.o : scrapy.c
	$(CC) $(CFLAGS) -g -c -o scrapy.o scrapy.c

parse.o : parse.c
	$(CC) $(CFLAGS) -g -c -o parse.o parse.c -lsocket -lnsl

clean :
	rm *.o scrapy
