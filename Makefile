CC = gcc

CFLAGS = -Wall -ggdb -std=c99

DEPS = KeyExp.h aesC.h

OBJ = KeyExp.o AES.o aesC.o

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

AES : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY : clean

clean:
	-rm -f $(OBJ)
	-rm -f 
