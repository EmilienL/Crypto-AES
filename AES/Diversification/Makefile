CC = gcc

INCLUDE = -I/usr/local/include -I/usr/include
LIB = -L/usr/local/lib -L/usr/lib -lm -g -std=c99 -Wall

all: compiler
compiler: 
	$(CC)  -o diversification $(INCLUDE) diversification.c $(LIB)
clean: 
	-rm *.o *~
	-rm diversification

